#!/usr/bin/env bash
# server.sh – Management script for Percobaan 9
# Usage: ./server.sh

set -euo pipefail

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BACKEND_DIR="$BASE_DIR/Server/Backend"
FRONTEND_DIR="$BASE_DIR/Server/Frontend"
EDGE_DIR="$BASE_DIR/EdgePython"

BACKEND_PORT=8009
FRONTEND_PORT=9009

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

banner() {
  echo -e "${CYAN}"
  echo "╔═══════════════════════════════════════════════════════════╗"
  echo "║       Percobaan 9 – IoT WebServer MQTT ESP32              ║"
  echo "║       Smart Monitoring with ML Anomaly Detection          ║"
  echo "╚═══════════════════════════════════════════════════════════╝"
  echo -e "${NC}"
}

check_command() {
  if ! command -v "$1" &>/dev/null; then
    echo -e "${RED}ERROR: '$1' is not installed${NC}"
    return 1
  fi
}

# ── Docker ────────────────────────────────────────────────────────────────
docker_up() {
  echo -e "${GREEN}Starting Docker services (PostgreSQL + Mosquitto)...${NC}"
  check_command docker
  check_command docker-compose
  cd "$BASE_DIR"
  docker-compose up -d
  echo -e "${GREEN}Waiting for PostgreSQL to be ready...${NC}"
  until docker exec percobaan9_postgres pg_isready -U iot_user -d iot_db &>/dev/null; do
    echo -n "."
    sleep 1
  done
  echo
  echo -e "${GREEN}Docker services are up${NC}"
  echo "  PostgreSQL : localhost:5432"
  echo "  Mosquitto  : localhost:1883 (WS: 9001)"
}

docker_down() {
  echo -e "${YELLOW}Stopping Docker services...${NC}"
  cd "$BASE_DIR"
  docker-compose down
  echo -e "${GREEN}Docker services stopped${NC}"
}

docker_logs() {
  cd "$BASE_DIR"
  docker-compose logs -f
}

# ── Backend ───────────────────────────────────────────────────────────────
install_backend() {
  echo -e "${GREEN}Installing Backend dependencies...${NC}"
  check_command python3
  check_command pip3
  cd "$BACKEND_DIR"
  python3 -m venv .venv 2>/dev/null || true
  source .venv/bin/activate
  pip install --upgrade pip -q
  pip install -r requirements.txt
  echo -e "${GREEN}Backend installed${NC}"
}

start_backend() {
  echo -e "${GREEN}Starting Backend (FastAPI) on port $BACKEND_PORT...${NC}"
  cd "$BACKEND_DIR"
  if [ ! -f ".env" ]; then
    cp .env.example .env
    echo -e "${YELLOW}Created .env from .env.example – please review settings${NC}"
  fi
  if [ -f ".venv/bin/activate" ]; then
    source .venv/bin/activate
  fi
  uvicorn main:app --host 0.0.0.0 --port "$BACKEND_PORT" --reload
}

start_backend_bg() {
  echo -e "${GREEN}Starting Backend in background...${NC}"
  cd "$BACKEND_DIR"
  if [ ! -f ".env" ]; then
    cp .env.example .env
  fi
  [ -f ".venv/bin/activate" ] && source .venv/bin/activate
  nohup uvicorn main:app --host 0.0.0.0 --port "$BACKEND_PORT" > /tmp/backend_p9.log 2>&1 &
  echo $! > /tmp/backend_p9.pid
  echo -e "${GREEN}Backend started (PID: $(cat /tmp/backend_p9.pid))${NC}"
  echo "  Log: tail -f /tmp/backend_p9.log"
}

stop_backend() {
  if [ -f /tmp/backend_p9.pid ]; then
    kill "$(cat /tmp/backend_p9.pid)" 2>/dev/null && echo -e "${GREEN}Backend stopped${NC}" || true
    rm -f /tmp/backend_p9.pid
  else
    echo -e "${YELLOW}Backend PID file not found${NC}"
  fi
}

# ── Frontend ──────────────────────────────────────────────────────────────
install_frontend() {
  echo -e "${GREEN}Installing Frontend dependencies...${NC}"
  check_command node
  check_command npm
  cd "$FRONTEND_DIR"
  npm install
  echo -e "${GREEN}Frontend installed${NC}"
}

start_frontend() {
  echo -e "${GREEN}Starting Angular dev server on port $FRONTEND_PORT...${NC}"
  cd "$FRONTEND_DIR"
  npx ng serve --port "$FRONTEND_PORT" --open
}

build_frontend() {
  echo -e "${GREEN}Building Angular app for production...${NC}"
  cd "$FRONTEND_DIR"
  npx ng build --configuration production
  echo -e "${GREEN}Build complete: $FRONTEND_DIR/dist/${NC}"
}

# ── EdgePython ────────────────────────────────────────────────────────────
install_edge() {
  echo -e "${GREEN}Installing EdgePython dependencies...${NC}"
  check_command python3
  cd "$EDGE_DIR"
  python3 -m venv .venv 2>/dev/null || true
  source .venv/bin/activate
  pip install --upgrade pip -q
  pip install -r requirements.txt
  echo -e "${GREEN}EdgePython installed${NC}"
}

train_model() {
  echo -e "${GREEN}Training ML anomaly detection model...${NC}"
  cd "$EDGE_DIR"
  [ -f ".venv/bin/activate" ] && source .venv/bin/activate

  read -r -p "Enter API URL [http://localhost:8009]: " api_url
  api_url="${api_url:-http://localhost:8009}"

  read -r -p "Enter JWT token: " token
  if [ -z "$token" ]; then
    echo -e "${RED}Token is required${NC}"
    return 1
  fi

  python3 train_model.py --api "$api_url" --token "$token"
}

start_edge() {
  echo -e "${GREEN}Starting EdgePython anomaly detector...${NC}"
  cd "$EDGE_DIR"
  if [ ! -f "models/anomaly_model.pkl" ]; then
    echo -e "${YELLOW}Model not found. Please train first.${NC}"
    return 1
  fi
  [ -f ".venv/bin/activate" ] && source .venv/bin/activate
  python3 main.py
}

start_edge_bg() {
  echo -e "${GREEN}Starting EdgePython in background...${NC}"
  cd "$EDGE_DIR"
  if [ ! -f "models/anomaly_model.pkl" ]; then
    echo -e "${YELLOW}Model not found. Train first (option 9).${NC}"
    return 1
  fi
  [ -f ".venv/bin/activate" ] && source .venv/bin/activate
  nohup python3 main.py > /tmp/edge_p9.log 2>&1 &
  echo $! > /tmp/edge_p9.pid
  echo -e "${GREEN}EdgePython started (PID: $(cat /tmp/edge_p9.pid))${NC}"
  echo "  Log: tail -f /tmp/edge_p9.log"
}

# ── Install All ───────────────────────────────────────────────────────────
install_all() {
  install_backend
  install_frontend
  install_edge
  echo -e "${GREEN}All dependencies installed${NC}"
}

# ── Status ────────────────────────────────────────────────────────────────
show_status() {
  echo -e "${CYAN}Service Status:${NC}"

  # Docker
  if command -v docker &>/dev/null && docker ps --format '{{.Names}}' 2>/dev/null | grep -q "percobaan9"; then
    echo -e "  Docker   : ${GREEN}Running${NC}"
  else
    echo -e "  Docker   : ${RED}Stopped${NC}"
  fi

  # Backend
  if [ -f /tmp/backend_p9.pid ] && kill -0 "$(cat /tmp/backend_p9.pid)" 2>/dev/null; then
    echo -e "  Backend  : ${GREEN}Running (PID $(cat /tmp/backend_p9.pid))${NC}"
  else
    echo -e "  Backend  : ${RED}Stopped${NC}"
  fi

  # EdgePython
  if [ -f /tmp/edge_p9.pid ] && kill -0 "$(cat /tmp/edge_p9.pid)" 2>/dev/null; then
    echo -e "  Edge ML  : ${GREEN}Running (PID $(cat /tmp/edge_p9.pid))${NC}"
  else
    echo -e "  Edge ML  : ${RED}Stopped${NC}"
  fi

  # Model
  if [ -f "$EDGE_DIR/models/anomaly_model.pkl" ]; then
    echo -e "  ML Model : ${GREEN}Available${NC}"
  else
    echo -e "  ML Model : ${YELLOW}Not trained${NC}"
  fi
}

# ── Menu ──────────────────────────────────────────────────────────────────
show_menu() {
  banner
  show_status
  echo
  echo -e "${YELLOW}Docker:${NC}"
  echo "  1. Docker up   (start PostgreSQL + Mosquitto)"
  echo "  2. Docker down (stop containers)"
  echo "  3. Docker logs"
  echo
  echo -e "${YELLOW}Backend (FastAPI on :$BACKEND_PORT):${NC}"
  echo "  4. Start Backend (foreground)"
  echo "  5. Start Backend (background)"
  echo "  6. Stop Backend"
  echo
  echo -e "${YELLOW}Frontend (Angular on :$FRONTEND_PORT):${NC}"
  echo "  7. Start Frontend (dev server)"
  echo "  8. Build Frontend (production)"
  echo
  echo -e "${YELLOW}EdgePython (ML Anomaly Detection):${NC}"
  echo "  9. Train Model (fetch data + fit IsolationForest)"
  echo " 10. Start EdgePython (foreground)"
  echo " 11. Start EdgePython (background)"
  echo
  echo -e "${YELLOW}Setup:${NC}"
  echo " 12. Install All dependencies"
  echo " 13. Install Backend only"
  echo " 14. Install Frontend only"
  echo " 15. Install EdgePython only"
  echo
  echo "  0. Exit"
  echo
}

main() {
  if [ $# -gt 0 ]; then
    case "$1" in
      docker-up)    docker_up     ;;
      docker-down)  docker_down   ;;
      backend)      start_backend ;;
      frontend)     start_frontend;;
      edge)         start_edge    ;;
      train)        train_model   ;;
      install)      install_all   ;;
      *) echo "Unknown command: $1"; exit 1 ;;
    esac
    return
  fi

  while true; do
    show_menu
    read -r -p "Select option [0-15]: " choice
    echo
    case "$choice" in
      1)  docker_up        ;;
      2)  docker_down      ;;
      3)  docker_logs      ;;
      4)  start_backend    ;;
      5)  start_backend_bg ;;
      6)  stop_backend     ;;
      7)  start_frontend   ;;
      8)  build_frontend   ;;
      9)  train_model      ;;
      10) start_edge       ;;
      11) start_edge_bg    ;;
      12) install_all      ;;
      13) install_backend  ;;
      14) install_frontend ;;
      15) install_edge     ;;
      0)  echo "Goodbye!"; exit 0 ;;
      *)  echo -e "${RED}Invalid option${NC}" ;;
    esac
    echo
    read -r -p "Press Enter to continue..."
  done
}

main "$@"
