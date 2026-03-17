import { Component, OnInit } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { MatSnackBar } from '@angular/material/snack-bar';
import { environment } from '../../../environments/environment';

interface Device {
  device_id: string;
  name: string;
  thresholds: Record<string, number>;
  auto_mode: boolean;
}

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
})
export class SettingsComponent implements OnInit {
  devices: Device[] = [];
  selectedDevice: Device | null = null;
  thresholdForm!: FormGroup;
  loading = false;
  saving = false;
  private readonly API = environment.apiUrl;

  constructor(
    private http: HttpClient,
    private fb: FormBuilder,
    private snack: MatSnackBar,
  ) {}

  ngOnInit(): void {
    this.thresholdForm = this.fb.group({
      suhu_max: [30.0, [Validators.required, Validators.min(0), Validators.max(100)]],
      kelembaban_min: [40.0, [Validators.required, Validators.min(0), Validators.max(100)]],
      cahaya_min: [500.0, [Validators.required, Validators.min(0)]],
      tanah_min: [30.0, [Validators.required, Validators.min(0), Validators.max(100)]],
    });

    this.loadDevices();
  }

  loadDevices(): void {
    this.loading = true;
    this.http.get<Device[]>(`${this.API}/api/devices`).subscribe({
      next: devices => {
        this.devices = devices;
        if (devices.length > 0) this.selectDevice(devices[0]);
        this.loading = false;
      },
      error: () => { this.loading = false; },
    });
  }

  selectDevice(device: Device): void {
    this.selectedDevice = device;
    const t = device.thresholds || {};
    this.thresholdForm.patchValue({
      suhu_max: t['suhu_max'] ?? 30,
      kelembaban_min: t['kelembaban_min'] ?? 40,
      cahaya_min: t['cahaya_min'] ?? 500,
      tanah_min: t['tanah_min'] ?? 30,
    });
  }

  saveThresholds(): void {
    if (!this.selectedDevice || this.thresholdForm.invalid) return;
    this.saving = true;
    const payload = this.thresholdForm.value;

    this.http.post(`${this.API}/api/devices/${this.selectedDevice.device_id}/thresholds`, payload).subscribe({
      next: (updated: any) => {
        if (this.selectedDevice) {
          this.selectedDevice.thresholds = updated.thresholds;
          const idx = this.devices.findIndex(d => d.device_id === this.selectedDevice!.device_id);
          if (idx >= 0) this.devices[idx] = updated;
        }
        this.saving = false;
        this.snack.open('Thresholds saved and sent to device via MQTT', 'OK', {
          duration: 3000,
          panelClass: 'success-snack',
        });
      },
      error: () => {
        this.saving = false;
        this.snack.open('Failed to save thresholds', 'OK', { duration: 3000, panelClass: 'error-snack' });
      },
    });
  }
}
