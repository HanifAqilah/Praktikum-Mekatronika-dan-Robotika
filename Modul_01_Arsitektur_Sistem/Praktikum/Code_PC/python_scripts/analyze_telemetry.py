"""
Analyze telemetry data from ESP32 serial output

Usage:
    python analyze_telemetry.py <log_file>

Example:
    python analyze_telemetry.py ../../output/test_log_20260205_143000.txt
"""

import sys
import re
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from datetime import datetime

# Set plotting style
sns.set_theme(style="darkgrid")
plt.rcParams['figure.figsize'] = (12, 6)

# State mapping
STATE_MAP = {
    0: 'IDLE',
    1: 'READY',
    2: 'RUNNING',
    3: 'FAULT',
    4: 'ESTOP',
    5: 'RECOVERY'
}

def parse_telemetry_log(filename):
    """Parse telemetry data dari serial log file"""
    
    telemetry_data = []
    events = []
    
    with open(filename, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    current_telemetry = {}
    in_telemetry_block = False
    
    for i, line in enumerate(lines):
        line = line.strip()
        
        # Detect telemetry block
        if '--- Telemetry ---' in line:
            in_telemetry_block = True
            current_telemetry = {'line_num': i}
            continue
        
        if in_telemetry_block:
            # Parse state
            if 'State:' in line:
                match = re.search(r'State:\s*(\d+)', line)
                if match:
                    state_num = int(match.group(1))
                    current_telemetry['state'] = state_num
                    current_telemetry['state_name'] = STATE_MAP.get(state_num, 'UNKNOWN')
            
            # Parse E-Stop status
            if 'E-Stop:' in line:
                current_telemetry['estop'] = 'ACTIVE' in line
            
            # Parse uptime
            if 'Uptime:' in line:
                match = re.search(r'Uptime:\s*(\d+)', line)
                if match:
                    current_telemetry['uptime_sec'] = int(match.group(1))
            
            # End of telemetry block
            if '----------------' in line:
                in_telemetry_block = False
                if current_telemetry:
                    telemetry_data.append(current_telemetry.copy())
        
        # Detect events
        if '🛑 E-STOP ACTIVATED!' in line:
            events.append({'line_num': i, 'event': 'E-STOP', 'severity': 'HIGH'})
        elif 'E-Stop released' in line:
            events.append({'line_num': i, 'event': 'E-Stop Released', 'severity': 'INFO'})
        elif '⚠ Obstacle detected' in line:
            events.append({'line_num': i, 'event': 'Obstacle Detected', 'severity': 'WARN'})
        elif '⚠ Communication timeout!' in line:
            events.append({'line_num': i, 'event': 'Comm Timeout', 'severity': 'WARN'})
        elif 'State changed:' in line:
            match = re.search(r'State changed:\s*(\d+)\s*→\s*(\d+)', line)
            if match:
                from_state = STATE_MAP.get(int(match.group(1)), 'UNKNOWN')
                to_state = STATE_MAP.get(int(match.group(2)), 'UNKNOWN')
                events.append({
                    'line_num': i,
                    'event': f'State: {from_state} → {to_state}',
                    'severity': 'INFO'
                })
    
    return pd.DataFrame(telemetry_data), pd.DataFrame(events)

def plot_state_timeline(df_telemetry, output_file='state_timeline.png'):
    """Plot state vs time"""
    
    if df_telemetry.empty:
        print("No telemetry data to plot")
        return
    
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 8))
    
    # Plot 1: State over time
    ax1.plot(df_telemetry['uptime_sec'], df_telemetry['state'], 
             marker='o', linestyle='-', linewidth=2, markersize=6)
    ax1.set_ylabel('State', fontsize=12, fontweight='bold')
    ax1.set_xlabel('Uptime (seconds)', fontsize=12, fontweight='bold')
    ax1.set_title('System State Timeline', fontsize=14, fontweight='bold')
    ax1.grid(True, alpha=0.3)
    ax1.set_yticks(range(6))
    ax1.set_yticklabels([STATE_MAP.get(i, f'State {i}') for i in range(6)])
    
    # Plot 2: E-Stop status
    df_telemetry['estop_num'] = df_telemetry['estop'].astype(int)
    ax2.fill_between(df_telemetry['uptime_sec'], 0, df_telemetry['estop_num'], 
                      alpha=0.3, color='red', label='E-Stop Active')
    ax2.set_ylabel('E-Stop', fontsize=12, fontweight='bold')
    ax2.set_xlabel('Uptime (seconds)', fontsize=12, fontweight='bold')
    ax2.set_title('E-Stop Status', fontsize=14, fontweight='bold')
    ax2.set_yticks([0, 1])
    ax2.set_yticklabels(['Inactive', 'Active'])
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Saved state timeline plot: {output_file}")
    plt.show()

def print_statistics(df_telemetry, df_events):
    """Print statistics dari telemetry data"""
    
    print("\n" + "="*60)
    print("TELEMETRY ANALYSIS REPORT")
    print("="*60)
    
    if df_telemetry.empty:
        print("No telemetry data found")
        return
    
    # Basic stats
    print(f"\n📊 BASIC STATISTICS:")
    print(f"  Total telemetry records: {len(df_telemetry)}")
    print(f"  Total runtime: {df_telemetry['uptime_sec'].max()} seconds")
    print(f"  Sampling interval: ~{df_telemetry['uptime_sec'].diff().mean():.1f} seconds")
    
    # State distribution
    print(f"\n🔄 STATE DISTRIBUTION:")
    state_counts = df_telemetry['state_name'].value_counts()
    for state, count in state_counts.items():
        percentage = (count / len(df_telemetry)) * 100
        print(f"  {state:12s}: {count:3d} samples ({percentage:5.1f}%)")
    
    # E-Stop statistics
    estop_active_count = df_telemetry['estop'].sum()
    if estop_active_count > 0:
        estop_percentage = (estop_active_count / len(df_telemetry)) * 100
        print(f"\n🛑 E-STOP STATISTICS:")
        print(f"  E-Stop active: {estop_active_count} samples ({estop_percentage:.1f}%)")
    
    # Events
    if not df_events.empty:
        print(f"\n⚠️  EVENTS DETECTED:")
        event_counts = df_events['event'].value_counts()
        for event, count in event_counts.items():
            print(f"  {event:30s}: {count:3d} times")
        
        # High severity events
        high_severity = df_events[df_events['severity'] == 'HIGH']
        if not high_severity.empty:
            print(f"\n  ⚡ HIGH SEVERITY EVENTS: {len(high_severity)}")
            for idx, row in high_severity.iterrows():
                print(f"     - {row['event']} (line {row['line_num']})")
    
    print("\n" + "="*60)

def main():
    if len(sys.argv) < 2:
        print("Usage: python analyze_telemetry.py <log_file>")
        print("Example: python analyze_telemetry.py ../../output/test_log_20260205_143000.txt")
        sys.exit(1)
    
    log_file = sys.argv[1]
    
    print(f"Analyzing log file: {log_file}")
    print("Parsing telemetry data...")
    
    df_telemetry, df_events = parse_telemetry_log(log_file)
    
    # Print statistics
    print_statistics(df_telemetry, df_events)
    
    # Generate plots
    if not df_telemetry.empty:
        output_file = log_file.replace('.txt', '_state_timeline.png')
        plot_state_timeline(df_telemetry, output_file)
    
    # Save to CSV for further analysis
    if not df_telemetry.empty:
        csv_file = log_file.replace('.txt', '_telemetry.csv')
        df_telemetry.to_csv(csv_file, index=False)
        print(f"✓ Saved telemetry data: {csv_file}")
    
    if not df_events.empty:
        csv_file = log_file.replace('.txt', '_events.csv')
        df_events.to_csv(csv_file, index=False)
        print(f"✓ Saved events data: {csv_file}")
    
    print("\n✓ Analysis complete!")

if __name__ == '__main__':
    main()
