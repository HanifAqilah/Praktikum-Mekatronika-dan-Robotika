import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { LoginComponent }     from './features/login/login.component';
import { DashboardComponent } from './features/dashboard/dashboard.component';
import { DataComponent }      from './features/data/data.component';
import { AuthGuard }          from './core/auth/auth.guard';

const routes: Routes = [
  { path: 'login', component: LoginComponent },
  { path: '',      component: DashboardComponent, canActivate: [AuthGuard] },
  { path: 'data',  component: DataComponent,      canActivate: [AuthGuard] },
  { path: '**',    redirectTo: '' },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule],
})
export class AppRoutingModule {}
