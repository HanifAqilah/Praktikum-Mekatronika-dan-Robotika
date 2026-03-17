import { Injectable, OnDestroy } from '@angular/core';
import { Subject, Observable } from 'rxjs';
import { AuthService } from '../auth/auth.service';
import { environment } from '../../../environments/environment';

export interface WsMessage {
  type: 'sensor' | 'alert' | 'device_status';
  data: any;
}

@Injectable({ providedIn: 'root' })
export class WsService implements OnDestroy {
  private ws: WebSocket | null = null;
  private messagesSubject = new Subject<WsMessage>();
  messages$ = this.messagesSubject.asObservable();
  private reconnectTimer: ReturnType<typeof setTimeout> | null = null;
  private shouldReconnect = false;

  constructor(private auth: AuthService) {}

  connect(): void {
    if (this.ws?.readyState === WebSocket.OPEN) return;
    const token = this.auth.token;
    if (!token) return;

    this.shouldReconnect = true;
    const url = `${environment.wsUrl}?token=${encodeURIComponent(token)}`;
    this.ws = new WebSocket(url);

    this.ws.onopen = () => {
      console.log('WebSocket connected');
      if (this.reconnectTimer) {
        clearTimeout(this.reconnectTimer);
        this.reconnectTimer = null;
      }
    };

    this.ws.onmessage = (event: MessageEvent) => {
      try {
        const msg: WsMessage = JSON.parse(event.data as string);
        this.messagesSubject.next(msg);
      } catch {
        if (event.data !== 'pong') {
          console.warn('WS: non-JSON message received', event.data);
        }
      }
    };

    this.ws.onerror = (err) => {
      console.error('WebSocket error', err);
    };

    this.ws.onclose = () => {
      console.log('WebSocket closed');
      this.ws = null;
      if (this.shouldReconnect) {
        this.reconnectTimer = setTimeout(() => this.connect(), 5000);
      }
    };
  }

  disconnect(): void {
    this.shouldReconnect = false;
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
      this.reconnectTimer = null;
    }
    this.ws?.close();
    this.ws = null;
  }

  sendPing(): void {
    if (this.ws?.readyState === WebSocket.OPEN) {
      this.ws.send('ping');
    }
  }

  ngOnDestroy(): void {
    this.disconnect();
  }
}
