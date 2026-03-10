import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MatSnackBar } from '@angular/material/snack-bar';
import { environment } from '../../../environments/environment';

interface Device {
  id: number;
  device_id: string;
  name: string;
  description: string | null;
  location: string | null;
  status: string;
  last_seen: string | null;
  auto_mode: boolean;
  fan_state: boolean;
  pump_state: boolean;
  thresholds: Record<string, number>;
  firmware_version: string | null;
  created_at: string;
}

@Component({
  selector: 'app-devices',
  templateUrl: './devices.component.html',
})
export class DevicesComponent implements OnInit {
  devices: Device[] = [];
  loading = false;
  showAddForm = false;
  newDevice = { device_id: '', name: '', description: '', location: '' };
  displayedColumns = ['status', 'device_id', 'name', 'location', 'last_seen', 'mode', 'actuators', 'firmware', 'actions'];
  private readonly API = environment.apiUrl;

  constructor(private http: HttpClient, private snack: MatSnackBar) {}

  ngOnInit(): void {
    this.loadDevices();
  }

  loadDevices(): void {
    this.loading = true;
    this.http.get<Device[]>(`${this.API}/api/devices`).subscribe({
      next: devices => { this.devices = devices; this.loading = false; },
      error: () => { this.loading = false; },
    });
  }

  addDevice(): void {
    if (!this.newDevice.device_id || !this.newDevice.name) return;
    this.http.post<Device>(`${this.API}/api/devices`, this.newDevice).subscribe({
      next: device => {
        this.devices = [device, ...this.devices];
        this.showAddForm = false;
        this.newDevice = { device_id: '', name: '', description: '', location: '' };
        this.snack.open('Device added', 'OK', { duration: 2000, panelClass: 'success-snack' });
      },
      error: err => {
        this.snack.open(err.error?.detail ?? 'Failed to add device', 'OK', { duration: 3000, panelClass: 'error-snack' });
      },
    });
  }

  deleteDevice(device: Device): void {
    if (!confirm(`Delete device ${device.name}?`)) return;
    this.http.delete(`${this.API}/api/devices/${device.device_id}`).subscribe({
      next: () => {
        this.devices = this.devices.filter(d => d.device_id !== device.device_id);
        this.snack.open('Device deleted', 'OK', { duration: 2000 });
      },
    });
  }

  toggleMode(device: Device): void {
    const newMode = !device.auto_mode;
    this.http.post(`${this.API}/api/devices/${device.device_id}/mode?auto_mode=${newMode}`, {}).subscribe({
      next: () => {
        device.auto_mode = newMode;
        this.snack.open(`Mode changed to ${newMode ? 'AUTO' : 'MANUAL'}`, 'OK', { duration: 2000 });
      },
    });
  }

  controlActuator(device: Device, actuator: string, state: boolean): void {
    this.http.post(`${this.API}/api/devices/${device.device_id}/actuator`, { actuator, state }).subscribe({
      next: () => {
        if (actuator === 'fan') device.fan_state = state;
        if (actuator === 'pump') device.pump_state = state;
        this.snack.open(`${actuator} turned ${state ? 'ON' : 'OFF'}`, 'OK', { duration: 2000 });
      },
      error: err => {
        this.snack.open(err.error?.detail ?? 'Command failed', 'OK', { duration: 3000, panelClass: 'error-snack' });
      },
    });
  }
}
