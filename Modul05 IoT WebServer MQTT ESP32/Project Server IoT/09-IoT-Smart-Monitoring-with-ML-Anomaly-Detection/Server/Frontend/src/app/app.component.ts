import { Component, OnInit, OnDestroy } from '@angular/core';
import { Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { AuthService } from './core/auth/auth.service';
import { WsService } from './core/websocket/ws.service';
import { MatSnackBar } from '@angular/material/snack-bar';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
})
export class AppComponent implements OnInit, OnDestroy {
  title = 'IoT Monitor';
  isLoggedIn = false;
  username = '';
  activeAlertCount = 0;

  navItems = [
    { label: 'Dashboard',  icon: 'dashboard',    route: '/dashboard' },
    { label: 'Analytics',  icon: 'analytics',    route: '/analytics' },
    { label: 'Alerts',     icon: 'notifications', route: '/alerts' },
    { label: 'Devices',    icon: 'devices',       route: '/devices' },
    { label: 'Settings',   icon: 'settings',      route: '/settings' },
  ];

  private subs = new Subscription();

  constructor(
    private auth: AuthService,
    private ws: WsService,
    private router: Router,
    private snack: MatSnackBar,
  ) {}

  ngOnInit(): void {
    this.subs.add(
      this.auth.currentUser$.subscribe(user => {
        this.isLoggedIn = !!user;
        this.username = user?.username ?? '';
        if (user) {
          this.ws.connect();
        } else {
          this.ws.disconnect();
        }
      })
    );

    this.subs.add(
      this.ws.messages$.subscribe(msg => {
        if (msg?.type === 'alert' && msg.data?.status === 'active') {
          this.activeAlertCount++;
          this.snack.open(`⚠ Alert: ${msg.data.message}`, 'Dismiss', {
            duration: 5000,
            panelClass: 'error-snack',
          });
        }
      })
    );
  }

  ngOnDestroy(): void {
    this.subs.unsubscribe();
  }

  logout(): void {
    this.auth.logout();
    this.router.navigate(['/login']);
  }
}
