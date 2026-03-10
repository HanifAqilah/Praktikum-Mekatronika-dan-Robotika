import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AuthGuard } from './core/auth/auth.guard';
import { DashboardComponent } from './features/dashboard/dashboard.component';
import { AnalyticsComponent } from './features/analytics/analytics.component';
import { AlertsComponent } from './features/alerts/alerts.component';
import { DevicesComponent } from './features/devices/devices.component';
import { SettingsComponent } from './features/settings/settings.component';
import { LoginComponent } from './features/login/login.component';

const routes: Routes = [
  { path: 'login', component: LoginComponent },
  {
    path: '',
    canActivate: [AuthGuard],
    children: [
      { path: '', redirectTo: 'dashboard', pathMatch: 'full' },
      { path: 'dashboard', component: DashboardComponent },
      { path: 'analytics', component: AnalyticsComponent },
      { path: 'alerts', component: AlertsComponent },
      { path: 'devices', component: DevicesComponent },
      { path: 'settings', component: SettingsComponent },
    ],
  },
  { path: '**', redirectTo: 'dashboard' },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule],
})
export class AppRoutingModule {}
