import { Component, OnInit, OnDestroy } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { Subscription } from 'rxjs';
import { MatSnackBar } from '@angular/material/snack-bar';
import { WsService } from '../../core/websocket/ws.service';
import { environment } from '../../../environments/environment';

export interface Alert {
  id: number;
  device_id: string;
  alert_type: string;
  message: string;
  threshold_value: number | null;
  actual_value: number | null;
  status: 'active' | 'acknowledged' | 'resolved';
  acknowledged_by: string | null;
  created_at: string;
  updated_at: string;
}

@Component({
  selector: 'app-alerts',
  templateUrl: './alerts.component.html',
})
export class AlertsComponent implements OnInit, OnDestroy {
  alerts: Alert[] = [];
  loading = false;
  filterStatus = '';
  displayedColumns = ['status', 'device_id', 'alert_type', 'message', 'actual_value', 'created_at', 'actions'];
  private sub = new Subscription();
  private readonly API = environment.apiUrl;

  constructor(
    private http: HttpClient,
    private ws: WsService,
    private snack: MatSnackBar,
  ) {}

  ngOnInit(): void {
    this.loadAlerts();
    this.sub.add(
      this.ws.messages$.subscribe(msg => {
        if (msg?.type === 'alert') {
          const existing = this.alerts.findIndex(a => a.id === msg.data.id);
          if (existing >= 0) {
            this.alerts[existing] = msg.data;
            this.alerts = [...this.alerts];
          } else {
            this.alerts = [msg.data, ...this.alerts];
          }
        }
      })
    );
  }

  ngOnDestroy(): void {
    this.sub.unsubscribe();
  }

  loadAlerts(): void {
    this.loading = true;
    let params = new HttpParams().set('limit', '200');
    if (this.filterStatus) params = params.set('status', this.filterStatus);

    this.http.get<Alert[]>(`${this.API}/api/alerts`, { params }).subscribe({
      next: data => { this.alerts = data; this.loading = false; },
      error: () => { this.loading = false; },
    });
  }

  acknowledge(alert: Alert): void {
    const username = localStorage.getItem('user')
      ? JSON.parse(localStorage.getItem('user')!).username
      : 'operator';
    this.http.post<Alert>(`${this.API}/api/alerts/${alert.id}/acknowledge`, { acknowledged_by: username }).subscribe({
      next: updated => {
        const idx = this.alerts.findIndex(a => a.id === updated.id);
        if (idx >= 0) this.alerts[idx] = updated;
        this.alerts = [...this.alerts];
        this.snack.open('Alert acknowledged', 'OK', { duration: 2000 });
      },
    });
  }

  resolve(alert: Alert): void {
    this.http.post<Alert>(`${this.API}/api/alerts/${alert.id}/resolve`, {}).subscribe({
      next: updated => {
        const idx = this.alerts.findIndex(a => a.id === updated.id);
        if (idx >= 0) this.alerts[idx] = updated;
        this.alerts = [...this.alerts];
        this.snack.open('Alert resolved', 'OK', { duration: 2000 });
      },
    });
  }

  delete(alert: Alert): void {
    if (!confirm('Delete this alert?')) return;
    this.http.delete(`${this.API}/api/alerts/${alert.id}`).subscribe({
      next: () => {
        this.alerts = this.alerts.filter(a => a.id !== alert.id);
        this.snack.open('Alert deleted', 'OK', { duration: 2000 });
      },
    });
  }

  getStatusColor(status: string): string {
    return { active: 'warn', acknowledged: 'accent', resolved: 'primary' }[status] || '';
  }

  getAlertTypeLabel(type: string): string {
    const labels: Record<string, string> = {
      suhu_high: '🌡 Suhu Tinggi',
      kelembaban_low: '💧 Kelembaban Rendah',
      cahaya_low: '💡 Cahaya Rendah',
      tanah_low: '🌱 Tanah Kering',
      anomaly: '🤖 Anomali ML',
      device_offline: '📵 Device Offline',
    };
    return labels[type] || type;
  }
}
