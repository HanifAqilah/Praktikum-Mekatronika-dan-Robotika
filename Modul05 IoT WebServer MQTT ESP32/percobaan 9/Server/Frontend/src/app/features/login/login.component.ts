import { Component } from '@angular/core';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { Router } from '@angular/router';
import { MatSnackBar } from '@angular/material/snack-bar';
import { AuthService } from '../../core/auth/auth.service';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
})
export class LoginComponent {
  loginForm: FormGroup;
  registerForm: FormGroup;
  showRegister = false;
  loading = false;
  hidePassword = true;

  constructor(
    private fb: FormBuilder,
    private auth: AuthService,
    private router: Router,
    private snack: MatSnackBar,
  ) {
    this.loginForm = this.fb.group({
      username: ['', Validators.required],
      password: ['', Validators.required],
    });
    this.registerForm = this.fb.group({
      username: ['', [Validators.required, Validators.minLength(3)]],
      email: ['', [Validators.required, Validators.email]],
      password: ['', [Validators.required, Validators.minLength(6)]],
    });
  }

  login(): void {
    if (this.loginForm.invalid) return;
    this.loading = true;
    const { username, password } = this.loginForm.value;
    this.auth.login(username, password).subscribe({
      next: () => {
        this.loading = false;
        this.router.navigate(['/dashboard']);
      },
      error: () => {
        this.loading = false;
        this.snack.open('Invalid username or password', 'OK', {
          duration: 3000,
          panelClass: 'error-snack',
        });
      },
    });
  }

  register(): void {
    if (this.registerForm.invalid) return;
    this.loading = true;
    const { username, email, password } = this.registerForm.value;
    this.auth.register(username, email, password).subscribe({
      next: () => {
        this.loading = false;
        this.snack.open('Registration successful! Please login.', 'OK', {
          duration: 3000,
          panelClass: 'success-snack',
        });
        this.showRegister = false;
        this.loginForm.patchValue({ username });
      },
      error: err => {
        this.loading = false;
        this.snack.open(err.error?.detail ?? 'Registration failed', 'OK', {
          duration: 3000,
          panelClass: 'error-snack',
        });
      },
    });
  }
}
