#!/usr/bin/env bash
# server.sh — Greenhouse Monitoring System management script
set -euo pipefail

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FRONTEND_DIR="$BASE_DIR/Server/Frontend"
API_DIR="$BASE_DIR/Server/Backend/api"
BRIDGE_DIR="$BASE_DIR/Server/Backend/mqtt-bridge"
EDGE_DIR="$BASE_DIR/EdgePython"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; CYAN='\033[0;36m'; NC='\033[0m'

log_info()  { echo -e "${GREEN}[INFO]${NC}  $*"; }
log_warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
log_error() { echo -e "${RED}[ERROR]${NC} $*"; }
log_title() { echo -e "\n${CYAN}=== $* ===${NC}\n"; }

check_command() {
    if ! command -v "$1" &>/dev/null; then
        log_error "Command '$1' not found. Please install it first."
        return 1
    fi
}

ensure_mosquitto_config() {
    mkdir -p "$BASE_DIR/mosquitto/config"
    if [[ ! -f "$BASE_DIR/mosquitto/config/mosquitto.conf" ]]; then
        cat > "$BASE_DIR/mosquitto/config/mosquitto.conf" <<'EOF'
listener 1883
allow_anonymous true
listener 9001
protocol websockets
allow_anonymous true
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log
log_dest stdout
EOF
        log_info "Created mosquitto.conf"
    fi
}

docker_up() {
    log_title "Starting Docker Services"
    check_command docker
    ensure_mosquitto_config
    cd "$BASE_DIR"
    docker compose up -d
    log_info "Waiting for services to be healthy..."
    sleep 5
    docker compose ps
}

docker_down() {
    log_title "Stopping Docker Services"
    cd "$BASE_DIR"
    docker compose down
}

docker_logs() {
    cd "$BASE_DIR"
    docker compose logs -f "${1:-}"
}

install_deps() {
    log_title "Installing All Dependencies"

    log_info "Installing FastAPI backend dependencies..."
    check_command python3
    cd "$API_DIR"
    python3 -m pip install -r requirements.txt --quiet
    log_info "FastAPI deps installed."

    log_info "Installing MQTT Bridge dependencies..."
    check_command node
    check_command npm
    cd "$BRIDGE_DIR"
    npm install --silent
    log_info "MQTT Bridge deps installed."

    log_info "Installing Frontend dependencies..."
    cd "$FRONTEND_DIR"
    npm install --silent
    log_info "Frontend deps installed."

    log_info "Installing EdgePython dependencies..."
    check_command python3
    cd "$EDGE_DIR"
    python3 -m pip install -r requirements.txt --quiet
    log_info "EdgePython deps installed."

    log_info "All dependencies installed successfully!"
}

build_all() {
    log_title "Building All Services"

    log_info "Building React Frontend..."
    cd "$FRONTEND_DIR"
    npm run build
    log_info "Frontend build complete."
}

start_api() {
    log_title "Starting FastAPI Backend (port 8010)"
    cd "$API_DIR"
    uvicorn main:app --host 0.0.0.0 --port 8010 --reload &
    API_PID=$!
    log_info "FastAPI started with PID $API_PID"
    echo $API_PID > /tmp/greenhouse_api.pid
}

start_bridge() {
    log_title "Starting MQTT Bridge (port 3010)"
    cd "$BRIDGE_DIR"
    node src/index.js &
    BRIDGE_PID=$!
    log_info "MQTT Bridge started with PID $BRIDGE_PID"
    echo $BRIDGE_PID > /tmp/greenhouse_bridge.pid
}

start_frontend() {
    log_title "Starting Frontend Dev Server"
    cd "$FRONTEND_DIR"
    npm run dev &
    FRONT_PID=$!
    log_info "Frontend started with PID $FRONT_PID"
    echo $FRONT_PID > /tmp/greenhouse_frontend.pid
}

start_edge() {
    log_title "Starting EdgePython"
    cd "$EDGE_DIR"
    python3 main.py &
    EDGE_PID=$!
    log_info "EdgePython started with PID $EDGE_PID"
    echo $EDGE_PID > /tmp/greenhouse_edge.pid
}

start_all() {
    log_title "Starting All Services"
    docker_up
    sleep 3
    start_api
    sleep 2
    start_bridge
    sleep 1
    start_frontend
    sleep 1
    start_edge
    log_info ""
    log_info "All services started!"
    log_info "  Frontend:    http://localhost:5173"
    log_info "  API:         http://localhost:8010/docs"
    log_info "  MQTT Bridge: ws://localhost:3010"
    log_info "  Metrics:     http://localhost:8001/metrics"
    log_info "  MQTT:        mqtt://localhost:1883"
    log_info ""
    log_warn "Press Ctrl+C to view logs. Run './server.sh stop' to stop all."
    wait
}

stop_all() {
    log_title "Stopping All Services"
    for pid_file in /tmp/greenhouse_api.pid /tmp/greenhouse_bridge.pid /tmp/greenhouse_frontend.pid /tmp/greenhouse_edge.pid; do
        if [[ -f "$pid_file" ]]; then
            pid=$(cat "$pid_file")
            if kill -0 "$pid" 2>/dev/null; then
                kill "$pid"
                log_info "Stopped PID $pid"
            fi
            rm -f "$pid_file"
        fi
    done
    docker_down
    log_info "All services stopped."
}

train_model() {
    log_title "Training ML Model (IsolationForest)"
    cd "$EDGE_DIR"
    python3 - <<'PYEOF'
import os, json, pickle, numpy as np
from sklearn.ensemble import IsolationForest

np.random.seed(42)
n_normal = 800
n_anom = 50

suhu_n       = np.random.normal(28, 3, n_normal)
kelembaban_n = np.random.normal(65, 8, n_normal)
cahaya_n     = np.random.normal(500, 100, n_normal)
tanah_n      = np.random.normal(60, 10, n_normal)
X_normal = np.column_stack([suhu_n, kelembaban_n, cahaya_n, tanah_n])

suhu_a       = np.random.uniform(45, 60, n_anom)
kelembaban_a = np.random.uniform(0, 15, n_anom)
cahaya_a     = np.random.uniform(900, 1023, n_anom)
tanah_a      = np.random.uniform(0, 10, n_anom)
X_anom = np.column_stack([suhu_a, kelembaban_a, cahaya_a, tanah_a])

X = np.vstack([X_normal, X_anom])
model = IsolationForest(n_estimators=100, contamination=0.05, random_state=42)
model.fit(X)

os.makedirs("models", exist_ok=True)
with open("models/greenhouse_model.pkl", "wb") as f:
    pickle.dump(model, f)

meta = {"features": ["suhu", "kelembaban", "cahaya", "tanah"],
        "trained_samples": len(X), "contamination": 0.05}
with open("models/greenhouse_model_meta.json", "w") as f:
    json.dump(meta, f, indent=2)

print(f"Model trained on {len(X)} samples and saved to models/greenhouse_model.pkl")
PYEOF
}

run_edge_only() {
    log_title "Running EdgePython (foreground)"
    cd "$EDGE_DIR"
    python3 main.py
}

show_status() {
    log_title "Service Status"
    echo -e "${CYAN}Docker containers:${NC}"
    cd "$BASE_DIR"
    docker compose ps 2>/dev/null || log_warn "Docker not running"

    echo -e "\n${CYAN}Background processes:${NC}"
    for name_pid in "API:/tmp/greenhouse_api.pid" "Bridge:/tmp/greenhouse_bridge.pid" "Frontend:/tmp/greenhouse_frontend.pid" "Edge:/tmp/greenhouse_edge.pid"; do
        name="${name_pid%%:*}"
        pid_file="${name_pid##*:}"
        if [[ -f "$pid_file" ]]; then
            pid=$(cat "$pid_file")
            if kill -0 "$pid" 2>/dev/null; then
                echo -e "  ${GREEN}●${NC} $name (PID $pid)"
            else
                echo -e "  ${RED}●${NC} $name (PID $pid — not running)"
            fi
        else
            echo -e "  ${YELLOW}○${NC} $name (not started)"
        fi
    done
}

print_menu() {
    echo -e "${CYAN}"
    cat <<'BANNER'
  ╔═══════════════════════════════════════════════╗
  ║     🌿  Greenhouse Monitoring System  🌿      ║
  ╚═══════════════════════════════════════════════╝
BANNER
    echo -e "${NC}"
    echo "  1) Start all services"
    echo "  2) Stop all services"
    echo "  3) Docker up (DB + Redis + MQTT)"
    echo "  4) Docker down"
    echo "  5) Docker logs"
    echo "  6) Install all dependencies"
    echo "  7) Build frontend"
    echo "  8) Run EdgePython (foreground)"
    echo "  9) Train ML model"
    echo " 10) Show service status"
    echo "  q) Quit"
    echo ""
}

if [[ $# -gt 0 ]]; then
    case "$1" in
        start)       start_all ;;
        stop)        stop_all ;;
        up)          docker_up ;;
        down)        docker_down ;;
        logs)        docker_logs "${2:-}" ;;
        install)     install_deps ;;
        build)       build_all ;;
        edge)        run_edge_only ;;
        train)       train_model ;;
        status)      show_status ;;
        *)           log_error "Unknown command: $1"; exit 1 ;;
    esac
    exit 0
fi

while true; do
    print_menu
    read -rp "Select option: " choice
    case "$choice" in
        1) start_all ;;
        2) stop_all ;;
        3) docker_up ;;
        4) docker_down ;;
        5) docker_logs ;;
        6) install_deps ;;
        7) build_all ;;
        8) run_edge_only ;;
        9) train_model ;;
        10) show_status ;;
        q|Q) log_info "Goodbye!"; exit 0 ;;
        *) log_warn "Invalid option: $choice" ;;
    esac
done
