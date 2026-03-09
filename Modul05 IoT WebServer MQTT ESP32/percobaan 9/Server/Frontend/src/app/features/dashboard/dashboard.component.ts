import { Component, OnInit, OnDestroy } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Subscription } from 'rxjs';
import { WsService } from '../../core/websocket/ws.service';
import { environment } from '../../../environments/environment';

interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;
  kelembaban: number;
  cahaya: number;
  tanah: number;
  fan_state: boolean;
  pump_state: boolean;
  auto_mode: boolean;
  created_at: string;
}

interface DashboardSummary {
  total_devices: number;
  online_devices: number;
  active_alerts: number;
  total_readings: number;
  latest_readings: SensorReading[];
  devices: any[];
}

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
})
export class DashboardComponent implements OnInit, OnDestroy {
  summary: DashboardSummary | null = null;
  latestReading: SensorReading | null = null;
  loading = true;
  lastUpdated = new Date();

  private sub = new Subscription();
  private refreshInterval: ReturnType<typeof setInterval> | null = null;
  private readonly API = environment.apiUrl;

  constructor(private http: HttpClient, private ws: WsService) {}

  ngOnInit(): void {
    this.loadSummary();

    this.sub.add(
      this.ws.messages$.subscribe(msg => {
        if (msg?.type === 'sensor') {
          this.latestReading = msg.data as SensorReading;
          this.lastUpdated = new Date();
          if (this.summary) {
            const idx = this.summary.latest_readings.findIndex(
              r => r.device_id === msg.data.device_id
            );
            if (idx >= 0) {
              this.summary.latest_readings[idx] = msg.data;
            } else {
              this.summary.latest_readings.unshift(msg.data);
              if (this.summary.latest_readings.length > 5) {
                this.summary.latest_readings.pop();
              }
            }
          }
        } else if (msg?.type === 'alert' && msg.data?.status === 'active') {
          if (this.summary) this.summary.active_alerts++;
        } else if (msg?.type === 'device_status') {
          if (this.summary) {
            const dev = this.summary.devices.find(d => d.device_id === msg.data.device_id);
            if (dev) Object.assign(dev, msg.data);
          }
        }
      })
    );

    this.refreshInterval = setInterval(() => this.loadSummary(), 30000);
  }

  ngOnDestroy(): void {
    this.sub.unsubscribe();
    if (this.refreshInterval) clearInterval(this.refreshInterval);
  }

  loadSummary(): void {
    this.http.get<DashboardSummary>(`${this.API}/api/dashboard/summary`).subscribe({
      next: data => {
        this.summary = data;
        if (data.latest_readings.length > 0) {
          this.latestReading = data.latest_readings[0];
        }
        this.loading = false;
        this.lastUpdated = new Date();
      },
      error: () => { this.loading = false; },
    });
  }

  getSensorColor(type: string, value: number): string {
    const thresholds: Record<string, { warn: number; ok: number; higher: boolean }> = {
      suhu:      { warn: 30, ok: 25,  higher: true },
      kelembaban:{ warn: 40, ok: 60,  higher: false },
      cahaya:    { warn: 500, ok: 1000, higher: false },
      tanah:     { warn: 30, ok: 50,  higher: false },
    };
    const t = thresholds[type];
    if (!t) return '';
    if (t.higher) return value > t.warn ? 'warn' : value > t.ok ? 'accent' : 'primary';
    return value < t.warn ? 'warn' : value < t.ok ? 'accent' : 'primary';
  }
}
