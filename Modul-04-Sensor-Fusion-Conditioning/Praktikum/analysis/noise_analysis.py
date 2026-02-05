#!/usr/bin/env python3
"""
noise_analysis.py - FFT Analysis for Sensor Noise
Modul 4: Sensor Fusion & Conditioning

Performs frequency analysis on raw sensor data to identify noise sources.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
from scipy.fft import fft, fftfreq
import pandas as pd
import sys

def load_data(filename):
    """Load CSV data from serial output"""
    try:
        df = pd.read_csv(filename)
        print(f"Loaded {len(df)} samples from {filename}")
        return df
    except Exception as e:
        print(f"Error loading file: {e}")
        sys.exit(1)

def compute_fft(data, sample_rate):
    """Compute FFT and return frequency & magnitude"""
    N = len(data)
    
    # Apply Hanning window to reduce spectral leakage
    window = np.hanning(N)
    data_windowed = data * window
    
    # Compute FFT
    yf = fft(data_windowed)
    xf = fftfreq(N, 1/sample_rate)
    
    # Only positive frequencies
    positive_freq_idx = xf >= 0
    xf = xf[positive_freq_idx]
    yf = 2.0/N * np.abs(yf[positive_freq_idx])
    
    return xf, yf

def plot_time_domain(df, signal_name='enc_raw'):
    """Plot time-domain signal"""
    plt.figure(figsize=(12, 4))
    plt.plot(df['time'] / 1000.0, df[signal_name], linewidth=0.5)
    plt.xlabel('Time (s)')
    plt.ylabel('Position (mm)')
    plt.title(f'Time Domain - {signal_name}')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

def plot_fft(xf, yf, title='FFT Spectrum'):
    """Plot FFT magnitude spectrum"""
    plt.figure(figsize=(12, 6))
    
    # Linear scale
    plt.subplot(2, 1, 1)
    plt.plot(xf, yf, linewidth=0.8)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.title(f'{title} - Linear Scale')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 50])  # Focus on 0-50 Hz
    
    # Log scale
    plt.subplot(2, 1, 2)
    plt.semilogy(xf, yf, linewidth=0.8)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (log)')
    plt.title(f'{title} - Log Scale')
    plt.grid(True, alpha=0.3, which='both')
    plt.xlim([0, 50])
    
    plt.tight_layout()

def compute_snr(signal_power, noise_power):
    """Compute Signal-to-Noise Ratio in dB"""
    snr_db = 10 * np.log10(signal_power / noise_power)
    return snr_db

def analyze_noise_bands(xf, yf, bands):
    """Analyze noise in specific frequency bands"""
    results = {}
    
    for band_name, (f_low, f_high) in bands.items():
        mask = (xf >= f_low) & (xf <= f_high)
        power = np.sum(yf[mask]**2)
        peak_freq = xf[mask][np.argmax(yf[mask])] if np.any(mask) else 0
        peak_mag = np.max(yf[mask]) if np.any(mask) else 0
        
        results[band_name] = {
            'power': power,
            'peak_freq': peak_freq,
            'peak_magnitude': peak_mag
        }
    
    return results

def main():
    if len(sys.argv) < 2:
        print("Usage: python noise_analysis.py <data.csv>")
        print("Example: python noise_analysis.py data/exp1_noise.csv")
        sys.exit(1)
    
    filename = sys.argv[1]
    
    # Load data
    df = load_data(filename)
    
    # Parameters
    SAMPLE_RATE = 100  # Hz (from config.h)
    
    # Define noise bands
    noise_bands = {
        'DC': (0, 0.5),
        'Low Freq Drift': (0.5, 2),
        'Mechanical Vibration': (2, 10),
        'Electrical (50 Hz)': (48, 52),
        'High Freq Noise': (10, SAMPLE_RATE/2)
    }
    
    # Analyze raw encoder signal
    print("\n" + "="*60)
    print("FFT ANALYSIS - Raw Encoder Signal")
    print("="*60)
    
    xf, yf = compute_fft(df['enc_raw'].values, SAMPLE_RATE)
    noise_analysis = analyze_noise_bands(xf, yf, noise_bands)
    
    print("\nNoise Band Analysis:")
    for band_name, metrics in noise_analysis.items():
        print(f"\n{band_name}:")
        print(f"  Total Power: {metrics['power']:.4e}")
        print(f"  Peak Frequency: {metrics['peak_freq']:.2f} Hz")
        print(f"  Peak Magnitude: {metrics['peak_magnitude']:.4f}")
    
    # Total signal power vs noise power
    signal_band = (0, 5)  # Assume signal is < 5 Hz
    noise_band = (5, SAMPLE_RATE/2)
    
    signal_power = np.sum(yf[(xf >= signal_band[0]) & (xf <= signal_band[1])]**2)
    noise_power = np.sum(yf[(xf >= noise_band[0]) & (xf <= noise_band[1])]**2)
    
    snr = compute_snr(signal_power, noise_power)
    print(f"\n{'='*60}")
    print(f"Signal Power (0-5 Hz): {signal_power:.4e}")
    print(f"Noise Power (5-{SAMPLE_RATE/2} Hz): {noise_power:.4e}")
    print(f"SNR: {snr:.2f} dB")
    print(f"{'='*60}\n")
    
    # Plot time domain
    plot_time_domain(df, 'enc_raw')
    
    # Plot FFT
    plot_fft(xf, yf, 'Raw Encoder FFT')
    
    # If filtered data exists, compare
    if 'enc_filt' in df.columns:
        print("\nComparing with filtered signal...")
        xf_filt, yf_filt = compute_fft(df['enc_filt'].values, SAMPLE_RATE)
        
        noise_power_filt = np.sum(yf_filt[(xf_filt >= noise_band[0]) & (xf_filt <= noise_band[1])]**2)
        signal_power_filt = np.sum(yf_filt[(xf_filt >= signal_band[0]) & (xf_filt <= signal_band[1])]**2)
        snr_filt = compute_snr(signal_power_filt, noise_power_filt)
        
        noise_reduction = 10 * np.log10(noise_power / noise_power_filt)
        
        print(f"Filtered SNR: {snr_filt:.2f} dB")
        print(f"SNR Improvement: {snr_filt - snr:.2f} dB")
        print(f"Noise Reduction: {noise_reduction:.2f} dB")
        
        # Plot comparison
        plt.figure(figsize=(12, 6))
        plt.subplot(2, 1, 1)
        plt.plot(xf, yf, label='Raw', alpha=0.7)
        plt.plot(xf_filt, yf_filt, label='Filtered', alpha=0.7)
        plt.xlabel('Frequency (Hz)')
        plt.ylabel('Magnitude')
        plt.title('FFT Comparison - Linear Scale')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.xlim([0, 50])
        
        plt.subplot(2, 1, 2)
        plt.semilogy(xf, yf, label='Raw', alpha=0.7)
        plt.semilogy(xf_filt, yf_filt, label='Filtered', alpha=0.7)
        plt.xlabel('Frequency (Hz)')
        plt.ylabel('Magnitude (log)')
        plt.title('FFT Comparison - Log Scale')
        plt.legend()
        plt.grid(True, alpha=0.3, which='both')
        plt.xlim([0, 50])
        
        plt.tight_layout()
    
    plt.show()

if __name__ == '__main__':
    main()
