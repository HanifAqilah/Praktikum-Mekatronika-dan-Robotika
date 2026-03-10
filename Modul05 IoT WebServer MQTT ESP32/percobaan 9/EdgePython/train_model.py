#!/usr/bin/env python3
"""
train_model.py – Train IsolationForest model for anomaly detection.

Fetches last 1000 sensor readings from the REST API, fits an IsolationForest,
and saves the model + scaler to models/anomaly_model.pkl.

Usage:
    python train_model.py [--api http://localhost:8009] [--token JWT_TOKEN]
"""
import argparse
import json
import os
import sys
from pathlib import Path

import joblib
import numpy as np
import pandas as pd
import requests
from sklearn.ensemble import IsolationForest
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline

MODELS_DIR = Path(__file__).parent / "models"
MODEL_PATH = MODELS_DIR / "anomaly_model.pkl"
FEATURES = ["suhu", "kelembaban", "cahaya", "tanah"]


def fetch_readings(api_url: str, token: str, limit: int = 1000) -> pd.DataFrame:
    """Fetch latest sensor readings from the backend REST API."""
    headers = {"Authorization": f"Bearer {token}"}
    params = {"limit": limit}
    resp = requests.get(f"{api_url}/api/sensors/", headers=headers, params=params, timeout=30)
    resp.raise_for_status()
    data = resp.json()
    if not data:
        print("No data returned from API", file=sys.stderr)
        sys.exit(1)
    df = pd.DataFrame(data)
    print(f"Fetched {len(df)} readings")
    return df


def train(df: pd.DataFrame, contamination: float = 0.05) -> Pipeline:
    """Fit IsolationForest pipeline on sensor features."""
    X = df[FEATURES].dropna().values
    print(f"Training on {len(X)} samples with features: {FEATURES}")

    pipe = Pipeline([
        ("scaler", StandardScaler()),
        ("clf", IsolationForest(
            n_estimators=200,
            max_samples="auto",
            contamination=contamination,
            random_state=42,
            n_jobs=-1,
        )),
    ])
    pipe.fit(X)
    print("Model trained successfully")

    # Quick validation: check fraction of anomalies
    preds = pipe.predict(X)
    n_anomalies = (preds == -1).sum()
    print(f"Training set: {n_anomalies}/{len(X)} samples flagged as anomalies ({n_anomalies/len(X)*100:.1f}%)")
    return pipe


def main():
    parser = argparse.ArgumentParser(description="Train IsolationForest anomaly model")
    parser.add_argument("--api", default=os.getenv("API_URL", "http://localhost:8009"),
                        help="Backend API URL")
    parser.add_argument("--token", default=os.getenv("API_TOKEN", ""),
                        help="JWT access token")
    parser.add_argument("--limit", type=int, default=1000,
                        help="Number of readings to fetch")
    parser.add_argument("--contamination", type=float, default=0.05,
                        help="Expected fraction of anomalies (0.01-0.5)")
    args = parser.parse_args()

    if not args.token:
        print("ERROR: --token (or API_TOKEN env var) is required", file=sys.stderr)
        print("Get a token via: curl -X POST http://localhost:8009/api/auth/login -d 'username=...&password=...'", file=sys.stderr)
        sys.exit(1)

    df = fetch_readings(args.api, args.token, args.limit)
    model = train(df, args.contamination)

    MODELS_DIR.mkdir(parents=True, exist_ok=True)
    joblib.dump({
        "model": model,
        "features": FEATURES,
        "training_size": len(df),
        "contamination": args.contamination,
    }, MODEL_PATH)
    print(f"Model saved to: {MODEL_PATH}")


if __name__ == "__main__":
    main()
