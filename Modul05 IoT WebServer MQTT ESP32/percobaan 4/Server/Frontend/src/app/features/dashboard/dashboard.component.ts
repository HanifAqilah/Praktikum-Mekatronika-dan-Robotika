import {
  Component,
  OnInit,
  OnDestroy,
  ViewChild,
  ElementRef,
  AfterViewInit,
} from '@angular/core';
import { AuthService } from '../../core/auth/auth.service';
import {
  Chart,
  LineElement,
  LineController,
  CategoryScale,
  LinearScale,
  PointElement,
  Tooltip,
  Legend,
  Filler,
} from 'chart.js';

Chart.register(LineElement, LineController, CategoryScale, LinearScale, PointElement, Tooltip, Legend, Filler);

export interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;
  kelembaban: number;
  cahaya: number;
  created_at: string;
}

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
})
export class DashboardComponent implements OnInit, OnDestroy, AfterViewInit {
  @ViewChild('suhuChart') chartRef!: ElementRef<HTMLCanvasElement>;

  wsConnected = false;
  wsError = '';
  latest: SensorReading | null = null;

  private ws: WebSocket | null = null;
  private chart: Chart | null = null;
  private suhuHistory: number[] = [];
  private timeLabels: string[] = [];
  private readonly MAX_POINTS = 20;

  constructor(private auth: AuthService) {}

  ngOnInit(): void {
    this.connectWebSocket();
  }

  ngAfterViewInit(): void {
    this.initChart();
  }

  ngOnDestroy(): void {
    this.ws?.close();
    this.chart?.destroy();
  }

  private connectWebSocket(): void {
    const token = this.auth.getToken();
    if (!token) return;

    const wsUrl = `ws://localhost:8004/api/ws/${token}`;
    this.ws = new WebSocket(wsUrl);

    this.ws.onopen = () => {
      this.wsConnected = true;
      this.wsError = '';
    };

    this.ws.onmessage = (event: MessageEvent) => {
      try {
        const data: SensorReading = JSON.parse(event.data);
        this.latest = data;
        this.updateChart(data);
      } catch {
        // ignore
      }
    };

    this.ws.onerror = () => {
      this.wsConnected = false;
      this.wsError = 'WebSocket error. Pastikan backend berjalan.';
    };

    this.ws.onclose = () => {
      this.wsConnected = false;
    };
  }

  private initChart(): void {
    const ctx = this.chartRef?.nativeElement?.getContext('2d');
    if (!ctx) return;

    this.chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: this.timeLabels,
        datasets: [
          {
            label: 'Suhu (°C)',
            data: this.suhuHistory,
            borderColor: '#f97316',
            backgroundColor: 'rgba(249,115,22,0.15)',
            borderWidth: 2,
            pointRadius: 3,
            fill: true,
            tension: 0.4,
          },
        ],
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        animation: { duration: 300 },
        plugins: { legend: { labels: { color: '#8b949e' } } },
        scales: {
          x: { ticks: { color: '#8b949e' }, grid: { color: 'rgba(48,54,61,0.8)' } },
          y: { ticks: { color: '#8b949e' }, grid: { color: 'rgba(48,54,61,0.8)' } },
        },
      },
    });
  }

  private updateChart(data: SensorReading): void {
    const label = new Date(data.created_at).toLocaleTimeString('id-ID');
    this.timeLabels.push(label);
    this.suhuHistory.push(data.suhu);

    if (this.timeLabels.length > this.MAX_POINTS) {
      this.timeLabels.shift();
      this.suhuHistory.shift();
    }

    this.chart?.update('none');
  }
}
