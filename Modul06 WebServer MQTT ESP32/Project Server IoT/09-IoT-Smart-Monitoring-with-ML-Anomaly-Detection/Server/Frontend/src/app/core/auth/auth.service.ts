import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { BehaviorSubject, Observable, tap } from 'rxjs';
import { environment } from '../../../environments/environment';

export interface LoginResponse {
  access_token: string;
  refresh_token: string;
  token_type: string;
}

export interface UserInfo {
  id: number;
  username: string;
  email: string;
  role: string;
  is_active: boolean;
}

@Injectable({ providedIn: 'root' })
export class AuthService {
  private readonly API = environment.apiUrl;
  private currentUserSubject = new BehaviorSubject<UserInfo | null>(this.loadUser());
  currentUser$ = this.currentUserSubject.asObservable();

  constructor(private http: HttpClient) {}

  private loadUser(): UserInfo | null {
    const stored = localStorage.getItem('user');
    return stored ? JSON.parse(stored) : null;
  }

  get token(): string | null {
    return localStorage.getItem('access_token');
  }

  get isLoggedIn(): boolean {
    return !!this.token;
  }

  login(username: string, password: string): Observable<LoginResponse> {
    const form = new FormData();
    form.append('username', username);
    form.append('password', password);
    return this.http.post<LoginResponse>(`${this.API}/api/auth/login`, form).pipe(
      tap(res => {
        localStorage.setItem('access_token', res.access_token);
        localStorage.setItem('refresh_token', res.refresh_token);
        this.fetchMe();
      })
    );
  }

  register(username: string, email: string, password: string): Observable<UserInfo> {
    return this.http.post<UserInfo>(`${this.API}/api/auth/register`, { username, email, password });
  }

  refreshToken(): Observable<LoginResponse> {
    const refresh_token = localStorage.getItem('refresh_token') ?? '';
    return this.http.post<LoginResponse>(`${this.API}/api/auth/refresh`, { refresh_token }).pipe(
      tap(res => {
        localStorage.setItem('access_token', res.access_token);
        localStorage.setItem('refresh_token', res.refresh_token);
      })
    );
  }

  fetchMe(): void {
    this.http.get<UserInfo>(`${this.API}/api/auth/me`).subscribe({
      next: user => {
        localStorage.setItem('user', JSON.stringify(user));
        this.currentUserSubject.next(user);
      },
      error: () => this.logout(),
    });
  }

  logout(): void {
    localStorage.removeItem('access_token');
    localStorage.removeItem('refresh_token');
    localStorage.removeItem('user');
    this.currentUserSubject.next(null);
  }
}
