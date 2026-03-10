import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';

// Angular Material
import { MatToolbarModule }   from '@angular/material/toolbar';
import { MatButtonModule }    from '@angular/material/button';
import { MatCardModule }      from '@angular/material/card';
import { MatInputModule }     from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatTableModule }     from '@angular/material/table';
import { MatPaginatorModule } from '@angular/material/paginator';
import { MatIconModule }      from '@angular/material/icon';
import { MatChipsModule }     from '@angular/material/chips';
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent }     from './app.component';
import { JwtInterceptor }   from './core/interceptors/jwt.interceptor';

import { LoginComponent }     from './features/login/login.component';
import { DashboardComponent } from './features/dashboard/dashboard.component';
import { DataComponent }      from './features/data/data.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    DashboardComponent,
    DataComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    HttpClientModule,
    ReactiveFormsModule,
    FormsModule,
    AppRoutingModule,
    MatToolbarModule,
    MatButtonModule,
    MatCardModule,
    MatInputModule,
    MatFormFieldModule,
    MatTableModule,
    MatPaginatorModule,
    MatIconModule,
    MatChipsModule,
    MatProgressSpinnerModule,
  ],
  providers: [
    { provide: HTTP_INTERCEPTORS, useClass: JwtInterceptor, multi: true },
  ],
  bootstrap: [AppComponent],
})
export class AppModule {}
