import { Component, OnInit, ViewChild } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';

export interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;
  kelembaban: number;
  cahaya: number;
  created_at: string;
}

export interface Stats {
  avg_suhu: number | null;
  avg_kelembaban: number | null;
  avg_cahaya: number | null;
  max_suhu: number | null;
  min_suhu: number | null;
  count: number;
}

@Component({
  selector: 'app-data',
  templateUrl: './data.component.html',
})
export class DataComponent implements OnInit {
  @ViewChild(MatPaginator) paginator!: MatPaginator;

  displayedColumns = ['id', 'device_id', 'suhu', 'kelembaban', 'cahaya', 'created_at'];
  dataSource = new MatTableDataSource<SensorReading>([]);
  stats: Stats | null = null;
  loading = false;

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    this.fetchAll();
  }

  fetchAll(): void {
    this.loading = true;
    this.http.get<SensorReading[]>('/api/sensors/history?limit=100').subscribe({
      next: (rows) => {
        this.dataSource.data = rows;
        this.loading = false;
        setTimeout(() => {
          this.dataSource.paginator = this.paginator;
        });
      },
      error: () => {
        this.loading = false;
      },
    });

    this.http.get<Stats>('/api/sensors/stats').subscribe({
      next: (s) => (this.stats = s),
    });
  }
}
