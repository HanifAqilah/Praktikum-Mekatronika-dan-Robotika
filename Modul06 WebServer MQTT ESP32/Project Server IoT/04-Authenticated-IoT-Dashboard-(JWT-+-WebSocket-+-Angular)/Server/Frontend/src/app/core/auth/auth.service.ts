import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, tap } from 'rxjs';

interface TokenResponse {
  access_token: string;
  token_type: string;
}

@Injectable({ providedIn: 'root' })
export class AuthService {
  private readonly TOKEN_KEY = 'iot_jwt_token';

  constructor(private http: HttpClient) {}

  login(username: string, password: string): Observable<TokenResponse> {
    return this.http
      .post<TokenResponse>('/api/auth/login', { username, password })
      .pipe(
        tap((res) => {
          localStorage.setItem(this.TOKEN_KEY, res.access_token);
        })
      );
  }

  logout(): void {
    localStorage.removeItem(this.TOKEN_KEY);
  }

  getToken(): string | null {
    return localStorage.getItem(this.TOKEN_KEY);
  }

  isLoggedIn(): boolean {
    const token = this.getToken();
    if (!token) return false;
    try {
      const payload = this._decodePayload(token);
      return payload.exp * 1000 > Date.now();
    } catch {
      return false;
    }
  }

  getUsername(): string {
    const token = this.getToken();
    if (!token) return '';
    try {
      return this._decodePayload(token).sub ?? '';
    } catch {
      return '';
    }
  }

  private _decodePayload(token: string): Record<string, any> {
    const base64 = token.split('.')[1].replace(/-/g, '+').replace(/_/g, '/');
    const json = decodeURIComponent(
      atob(base64)
        .split('')
        .map((c) => '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2))
        .join('')
    );
    return JSON.parse(json);
  }
}
