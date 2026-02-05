#!/usr/bin/env python3
"""
filter_comparison.py - Compare Filter Performance
Modul 4: Sensor Fusion & Conditioning

Visualizes and compares multiple filter outputs.
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
from scipy import signal

def load_data(filename):
    """Load CSV data from serial output"""
    try:
        df = pd.read_csv(filename)
        print(f"Loaded {len(df)} samples from {filename}")
        return df
    except Exception as e:
        print(f"Error loading file: {e}")
        sys.exit(1)

def compute_metrics(reference, filtered):
    """Compute filter performance metrics"""
    # RMSE
    rmse = np.sqrt(np.mean((filtered - reference)**2))
    
    # MAE
    mae = np.mean(np.abs(filtered - reference))
    
    # Delay (cross-correlation)
    correlation = np.correlate(reference - np.mean(reference), 
                               filtered - np.mean(filtered), 
                               mode='full')
    delay_samples = np.argmax(correlation) - (len(reference) - 1)
    
    # Standard deviation (noise level)
    std_dev = np.std(filtered)
    
    return {
        'RMSE': rmse,
        'MAE': mae,
        'Delay (samples)': delay_samples,
        'Std Dev': std_dev
    }

def plot_time_comparison(df, signals, title='Filter Comparison'):
    """Plot multiple signals in time domain"""
    plt.figure(figsize=(14, 6))
    
    time_s = df['time'].values / 1000.0
    
    for signal_name, label in signals.items():
        if signal_name in df.columns:
            plt.plot(time_s, df[signal_name], label=label, alpha=0.8, linewidth=1.2)
    
    plt.xlabel('Time (s)')
    plt.ylabel('Position (mm)')
    plt.title(title)
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

def plot_error_analysis(df, reference_col, filtered_col, title='Error Analysis'):
    """Plot error between reference and filtered signal"""
    plt.figure(figsize=(14, 8))
    
    time_s = df['time'].values / 1000.0
    error = df[filtered_col] - df[reference_col]
    
    # Time domain error
    plt.subplot(3, 1, 1)
    plt.plot(time_s, error, linewidth=0.8)
    plt.axhline(y=0, color='r', linestyle='--', alpha=0.5)
    plt.xlabel('Time (s)')
    plt.ylabel('Error (mm)')
    plt.title(f'{title} - Error over Time')
    plt.grid(True, alpha=0.3)
    
    # Error histogram
    plt.subplot(3, 1, 2)
    plt.hist(error, bins=50, edgecolor='black', alpha=0.7)
    plt.axvline(x=0, color='r', linestyle='--', alpha=0.5)
    plt.axvline(x=np.mean(error), color='g', linestyle='--', label=f'Mean: {np.mean(error):.3f}')
    plt.xlabel('Error (mm)')
    plt.ylabel('Frequency')
    plt.title('Error Distribution')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Cumulative error
    plt.subplot(3, 1, 3)
    cumulative_error = np.cumsum(np.abs(error))
    plt.plot(time_s, cumulative_error, linewidth=1.2)
    plt.xlabel('Time (s)')
    plt.ylabel('Cumulative Absolute Error (mm)')
    plt.title('Cumulative Error')
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()

def plot_step_response(df, reference_col, filtered_col, step_time_ms=5000, title='Step Response'):
    """Analyze filter response to step input"""
    # Find step region
    step_idx = np.argmin(np.abs(df['time'].values - step_time_ms))
    window = 500  # samples around step
    
    if step_idx - window < 0 or step_idx + window >= len(df):
        print("Warning: Step time outside data range, skipping step response analysis")
        return
    
    time_window = df['time'].values[step_idx-window:step_idx+window] - step_time_ms
    ref_window = df[reference_col].values[step_idx-window:step_idx+window]
    filt_window = df[filtered_col].values[step_idx-window:step_idx+window]
    
    plt.figure(figsize=(12, 6))
    plt.plot(time_window, ref_window, label='Reference', linewidth=1.5)
    plt.plot(time_window, filt_window, label='Filtered', linewidth=1.5)
    plt.axvline(x=0, color='r', linestyle='--', alpha=0.5, label='Step Event')
    plt.xlabel('Time relative to step (ms)')
    plt.ylabel('Position (mm)')
    plt.title(title)
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

def plot_frequency_response(df, sample_rate=100):
    """Plot Bode plot for filters"""
    plt.figure(figsize=(12, 8))
    
    signals = {
        'enc_raw': 'Raw',
        'enc_filt': 'Filtered',
        'pos_fused': 'Fused'
    }
    
    for signal_name, label in signals.items():
        if signal_name not in df.columns:
            continue
        
        # Compute transfer function (output/input)
        if signal_name == 'enc_raw':
            continue
        
        # FFT
        from scipy.fft import fft, fftfreq
        N = len(df[signal_name])
        yf = fft(df[signal_name].values)
        yf_ref = fft(df['enc_raw'].values)
        xf = fftfreq(N, 1/sample_rate)
        
        # Transfer function H(f) = Y(f) / X(f)
        H = yf / (yf_ref + 1e-10)  # Avoid division by zero
        
        # Only positive frequencies
        positive_freq_idx = xf > 0
        xf_pos = xf[positive_freq_idx]
        H_pos = H[positive_freq_idx]
        
        # Magnitude (dB)
        mag_db = 20 * np.log10(np.abs(H_pos) + 1e-10)
        
        # Phase (degrees)
        phase_deg = np.angle(H_pos, deg=True)
        
        # Plot magnitude
        plt.subplot(2, 1, 1)
        plt.semilogx(xf_pos, mag_db, label=label, alpha=0.8)
        
        # Plot phase
        plt.subplot(2, 1, 2)
        plt.semilogx(xf_pos, phase_deg, label=label, alpha=0.8)
    
    plt.subplot(2, 1, 1)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.title('Bode Plot - Magnitude')
    plt.legend()
    plt.grid(True, alpha=0.3, which='both')
    plt.xlim([0.1, sample_rate/2])
    
    plt.subplot(2, 1, 2)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Phase (degrees)')
    plt.title('Bode Plot - Phase')
    plt.legend()
    plt.grid(True, alpha=0.3, which='both')
    plt.xlim([0.1, sample_rate/2])
    
    plt.tight_layout()

def main():
    if len(sys.argv) < 2:
        print("Usage: python filter_comparison.py <data.csv>")
        print("Example: python filter_comparison.py data/exp2_moving_average.csv")
        sys.exit(1)
    
    filename = sys.argv[1]
    
    # Load data
    df = load_data(filename)
    
    # Check available columns
    print(f"\nAvailable columns: {list(df.columns)}")
    
    # Plot time comparison
    signals_to_plot = {
        'enc_raw': 'Raw Encoder',
        'enc_filt': 'Filtered',
        'pos_fused': 'Fused Position',
        'voting': 'Voting Result'
    }
    
    available_signals = {k: v for k, v in signals_to_plot.items() if k in df.columns}
    
    if len(available_signals) > 1:
        plot_time_comparison(df, available_signals, 'Filter Comparison')
    
    # Compute metrics (if filtered exists)
    if 'enc_filt' in df.columns and 'enc_raw' in df.columns:
        print("\n" + "="*60)
        print("PERFORMANCE METRICS - enc_filt vs enc_raw")
        print("="*60)
        metrics = compute_metrics(df['enc_raw'].values, df['enc_filt'].values)
        for metric_name, value in metrics.items():
            print(f"{metric_name}: {value:.4f}")
        
        # Error analysis
        plot_error_analysis(df, 'enc_raw', 'enc_filt', 'Filter Error Analysis')
    
    # Fused position metrics
    if 'pos_fused' in df.columns and 'enc_raw' in df.columns:
        print("\n" + "="*60)
        print("PERFORMANCE METRICS - pos_fused vs enc_raw")
        print("="*60)
        metrics_fused = compute_metrics(df['enc_raw'].values, df['pos_fused'].values)
        for metric_name, value in metrics_fused.items():
            print(f"{metric_name}: {value:.4f}")
    
    # Voting metrics
    if 'voting' in df.columns and 'pos_fused' in df.columns:
        print("\n" + "="*60)
        print("PERFORMANCE METRICS - voting vs pos_fused")
        print("="*60)
        metrics_voting = compute_metrics(df['pos_fused'].values, df['voting'].values)
        for metric_name, value in metrics_voting.items():
            print(f"{metric_name}: {value:.4f}")
    
    # Frequency response (Bode plot)
    if 'enc_filt' in df.columns or 'pos_fused' in df.columns:
        plot_frequency_response(df, sample_rate=100)
    
    plt.show()

if __name__ == '__main__':
    main()
