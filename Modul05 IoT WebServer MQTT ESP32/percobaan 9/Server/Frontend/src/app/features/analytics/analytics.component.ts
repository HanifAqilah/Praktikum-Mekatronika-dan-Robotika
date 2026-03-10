import { Component, OnInit } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { ChartConfiguration, ChartType } from 'chart.js';
import { environment } from '../../../environments/environment';

interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;
  kelembaban: number;
  cahaya: number;
  tanah: number;
  created_at: string;
}

interface SensorStats {
  suhu_avg: number;
  suhu_min: number;
  suhu_max: number;
  kelembaban_avg: number;
  kelembaban_min: number;
  kelembaban_max: number;
  cahaya_avg: number;
  tanah_avg: number;
  total_readings: number;
  period_hours: number;
}

interface Device {
  device_id: string;
  name: string;
}

@Component({
  selector: 'app-analytics',
  templateUrl: './analytics.component.html',
})
export class AnalyticsComponent implements OnInit {
  devices: Device[] = [];
  selectedDevice = '';
  selectedHours = 24;
  stats: SensorStats | null = null;
  loading = false;
  private readonly API = environment.apiUrl;

  lineChartData: ChartConfiguration<'line'>['data'] = {
    labels: [],
    datasets: [
      { data: [], label: 'Suhu (°C)', borderColor: '#f44336', backgroundColor: 'rgba(244,67,54,0.1)', tension: 0.3 },
      { data: [], label: 'Kelembaban (%)', borderColor: '#2196f3', backgroundColor: 'rgba(33,150,243,0.1)', tension: 0.3 },
    ],
  };

  lineChartData2: ChartConfiguration<'line'>['data'] = {
    labels: [],
    datasets: [
      { data: [], label: 'Cahaya (lux)', borderColor: '#ff9800', backgroundColor: 'rgba(255,152,0,0.1)', tension: 0.3 },
      { data: [], label: 'Kel. Tanah (%)', borderColor: '#795548', backgroundColor: 'rgba(121,85,72,0.1)', tension: 0.3 },
    ],
  };

  lineChartOptions: ChartConfiguration<'line'>['options'] = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: { legend: { display: true } },
    scales: { x: { ticks: { maxTicksLimit: 12 } }, y: { beginAtZero: false } },
  };

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    this.http.get<Device[]>(`${this.API}/api/devices`).subscribe({
      next: devices => {
        this.devices = devices;
        if (devices.length > 0) {
          this.selectedDevice = devices[0].device_id;
          this.loadData();
        }
      },
    });
  }

  loadData(): void {
    if (!this.selectedDevice) return;
    this.loading = true;

    const params = new HttpParams()
      .set('device_id', this.selectedDevice)
      .set('hours', this.selectedHours);

    this.http.get<SensorReading[]>(`${this.API}/api/sensors/history`, { params }).subscribe({
      next: readings => {
        const labels = readings.map(r => {
          const d = new Date(r.created_at);
          return `${d.getHours().toString().padStart(2,'0')}:${d.getMinutes().toString().padStart(2,'0')}`;
        });
        this.lineChartData = {
          labels,
          datasets: [
            { ...this.lineChartData.datasets[0], data: readings.map(r => r.suhu) },
            { ...this.lineChartData.datasets[1], data: readings.map(r => r.kelembaban) },
          ],
        };
        this.lineChartData2 = {
          labels,
          datasets: [
            { ...this.lineChartData2.datasets[0], data: readings.map(r => r.cahaya) },
            { ...this.lineChartData2.datasets[1], data: readings.map(r => r.tanah) },
          ],
        };
        this.loading = false;
      },
      error: () => { this.loading = false; },
    });

    this.http.get<SensorStats>(`${this.API}/api/sensors/stats`, { params }).subscribe({
      next: stats => { this.stats = stats; },
    });
  }

  exportCsv(): void {
    const params = new HttpParams()
      .set('device_id', this.selectedDevice)
      .set('hours', this.selectedHours);
    this.http.get(`${this.API}/api/sensors/export/csv`, { params, responseType: 'blob' }).subscribe({
      next: blob => {
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `sensor_${this.selectedDevice}_${Date.now()}.csv`;
        a.click();
        URL.revokeObjectURL(url);
      },
    });
  }
}
