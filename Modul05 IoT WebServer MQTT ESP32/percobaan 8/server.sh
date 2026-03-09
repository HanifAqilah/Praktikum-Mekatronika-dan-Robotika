#!/usr/bin/env bash
# server.sh — Percobaan 8 Management Script
# Praktikum Mekatronika dan Robotika

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BACKEND_DIR="$SCRIPT_DIR/Server/Backend"
FRONTEND_DIR="$SCRIPT_DIR/Server/Frontend"
EDGE_DIR="$SCRIPT_DIR/EdgePython"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
RESET='\033[0m'

# PID files
BACKEND_PID_FILE="/tmp/p8_backend.pid"
FRONTEND_PID_FILE="/tmp/p8_frontend.pid"
EDGE_PID_FILE="/tmp/p8_edge.pid"

print_banner() {
  echo -e "${CYAN}"
  echo "╔══════════════════════════════════════════════════════════╗"
  echo "║     Percobaan 8 — IoT WebServer MQTT ESP32              ║"
  echo "║     Praktikum Mekatronika dan Robotika                  ║"
  echo "╚══════════════════════════════════════════════════════════╝"
  echo -e "${RESET}"
}

print_menu() {
  echo -e "${WHITE}┌─────────────────────────────────────────┐${RESET}"
  echo -e "${WHITE}│           MENU PERCOBAAN 8              │${RESET}"
  echo -e "${WHITE}├─────────────────────────────────────────┤${RESET}"
  echo -e "${WHITE}│  ${GREEN}1)${WHITE} Start Backend     (port 8008)       │${RESET}"
  echo -e "${WHITE}│  ${GREEN}2)${WHITE} Start Frontend    (port 9008)       │${RESET}"
  echo -e "${WHITE}│  ${GREEN}3)${WHITE} Start EdgePython                    │${RESET}"
  echo -e "${WHITE}│  ${GREEN}4)${WHITE} Docker Up  (PostgreSQL)             │${RESET}"
  echo -e "${WHITE}│  ${GREEN}5)${WHITE} Docker Down                         │${RESET}"
  echo -e "${WHITE}│  ${GREEN}6)${WHITE} Install All Dependencies            │${RESET}"
  echo -e "${WHITE}│  ${GREEN}7)${WHITE} Build Frontend                      │${RESET}"
  echo -e "${WHITE}│  ${GREEN}8)${WHITE} Start All  (Docker + Back + Front)  │${RESET}"
  echo -e "${WHITE}│  ${GREEN}9)${WHITE} Stop All                            │${RESET}"
  echo -e "${WHITE}│  ${GREEN}s)${WHITE} Status                              │${RESET}"
  echo -e "${WHITE}│  ${GREEN}q)${WHITE} Quit                                │${RESET}"
  echo -e "${WHITE}└─────────────────────────────────────────┘${RESET}"
}

ensure_env() {
  local dir="$1"
  local env_file="$dir/.env"
  local example="$dir/.env.example"
  if [[ ! -f "$env_file" && -f "$example" ]]; then
    echo -e "${YELLOW}  Creating .env from .env.example in $dir${RESET}"
    cp "$example" "$env_file"
  fi
}

start_backend() {
  echo -e "${CYAN}► Starting Backend (Node.js)...${RESET}"
  ensure_env "$BACKEND_DIR"
  if [[ ! -d "$BACKEND_DIR/node_modules" ]]; then
    echo -e "${YELLOW}  node_modules not found, running npm install...${RESET}"
    (cd "$BACKEND_DIR" && npm install --silent)
  fi
  (cd "$BACKEND_DIR" && nohup node src/index.js > /tmp/p8_backend.log 2>&1 &
   echo $! > "$BACKEND_PID_FILE"
   echo -e "${GREEN}  Backend started (PID $(cat "$BACKEND_PID_FILE")). Logs: /tmp/p8_backend.log${RESET}"
  )
}

start_frontend() {
  echo -e "${CYAN}► Starting Frontend (Vite)...${RESET}"
  if [[ ! -d "$FRONTEND_DIR/node_modules" ]]; then
    echo -e "${YELLOW}  node_modules not found, running npm install...${RESET}"
    (cd "$FRONTEND_DIR" && npm install --silent)
  fi
  (cd "$FRONTEND_DIR" && nohup npm run dev > /tmp/p8_frontend.log 2>&1 &
   echo $! > "$FRONTEND_PID_FILE"
   echo -e "${GREEN}  Frontend started (PID $(cat "$FRONTEND_PID_FILE")). Logs: /tmp/p8_frontend.log${RESET}"
   echo -e "${GREEN}  URL: http://localhost:9008${RESET}"
  )
}

start_edge() {
  echo -e "${CYAN}► Starting EdgePython...${RESET}"
  ensure_env "$EDGE_DIR"
  if ! python3 -c "import paho" 2>/dev/null; then
    echo -e "${YELLOW}  Installing Python dependencies...${RESET}"
    pip3 install -r "$EDGE_DIR/requirements.txt" -q
  fi
  (cd "$EDGE_DIR" && nohup python3 main.py > /tmp/p8_edge.log 2>&1 &
   echo $! > "$EDGE_PID_FILE"
   echo -e "${GREEN}  EdgePython started (PID $(cat "$EDGE_PID_FILE")). Logs: /tmp/p8_edge.log${RESET}"
  )
}

docker_up() {
  echo -e "${CYAN}► Starting Docker (PostgreSQL)...${RESET}"
  ensure_env "$SCRIPT_DIR"
  (cd "$SCRIPT_DIR" && docker compose up -d)
  echo -e "${GREEN}  PostgreSQL running on port 5432${RESET}"
}

docker_down() {
  echo -e "${CYAN}► Stopping Docker containers...${RESET}"
  (cd "$SCRIPT_DIR" && docker compose down)
  echo -e "${YELLOW}  Containers stopped.${RESET}"
}

install_all() {
  echo -e "${CYAN}► Installing all dependencies...${RESET}"
  echo -e "${WHITE}  [1/2] Backend npm install...${RESET}"
  (cd "$BACKEND_DIR" && npm install)
  echo -e "${WHITE}  [2/2] Frontend npm install...${RESET}"
  (cd "$FRONTEND_DIR" && npm install)
  echo -e "${WHITE}  [3/3] EdgePython pip install...${RESET}"
  pip3 install -r "$EDGE_DIR/requirements.txt"
  echo -e "${GREEN}  All dependencies installed!${RESET}"
}

build_frontend() {
  echo -e "${CYAN}► Building Frontend...${RESET}"
  if [[ ! -d "$FRONTEND_DIR/node_modules" ]]; then
    (cd "$FRONTEND_DIR" && npm install --silent)
  fi
  (cd "$FRONTEND_DIR" && npm run build)
  echo -e "${GREEN}  Build complete! Output: $FRONTEND_DIR/dist${RESET}"
}

stop_process() {
  local name="$1"
  local pid_file="$2"
  if [[ -f "$pid_file" ]]; then
    local pid
    pid=$(cat "$pid_file")
    if kill -0 "$pid" 2>/dev/null; then
      kill "$pid"
      echo -e "${YELLOW}  Stopped $name (PID $pid)${RESET}"
    else
      echo -e "${YELLOW}  $name not running${RESET}"
    fi
    rm -f "$pid_file"
  else
    echo -e "${YELLOW}  $name PID file not found${RESET}"
  fi
}

stop_all() {
  echo -e "${CYAN}► Stopping all services...${RESET}"
  stop_process "Backend"    "$BACKEND_PID_FILE"
  stop_process "Frontend"   "$FRONTEND_PID_FILE"
  stop_process "EdgePython" "$EDGE_PID_FILE"
}

start_all() {
  docker_up
  sleep 3
  start_backend
  sleep 2
  start_frontend
  echo -e "${GREEN}✅ All services started!${RESET}"
  echo -e "${WHITE}   Backend  → http://localhost:8008/api/status${RESET}"
  echo -e "${WHITE}   Frontend → http://localhost:9008${RESET}"
}

show_status() {
  echo -e "${CYAN}── Service Status ─────────────────────────────${RESET}"

  check_pid() {
    local name="$1" pid_file="$2"
    if [[ -f "$pid_file" ]]; then
      local pid
      pid=$(cat "$pid_file")
      if kill -0 "$pid" 2>/dev/null; then
        echo -e "  ${GREEN}● $name${RESET} running (PID $pid)"
      else
        echo -e "  ${RED}● $name${RESET} stopped (stale PID $pid)"
      fi
    else
      echo -e "  ${RED}● $name${RESET} not started"
    fi
  }

  check_pid "Backend"    "$BACKEND_PID_FILE"
  check_pid "Frontend"   "$FRONTEND_PID_FILE"
  check_pid "EdgePython" "$EDGE_PID_FILE"

  echo -e ""
  if docker ps --filter "name=percobaan8_postgres" --format "{{.Names}}" 2>/dev/null | grep -q "percobaan8"; then
    echo -e "  ${GREEN}● PostgreSQL${RESET} running (Docker)"
  else
    echo -e "  ${RED}● PostgreSQL${RESET} not running"
  fi
  echo -e "${CYAN}───────────────────────────────────────────────${RESET}"
}

# ─── Interactive menu ─────────────────────────────────────
if [[ $# -gt 0 ]]; then
  case "$1" in
    backend)  start_backend ;;
    frontend) start_frontend ;;
    edge)     start_edge ;;
    up)       docker_up ;;
    down)     docker_down ;;
    install)  install_all ;;
    build)    build_frontend ;;
    all)      start_all ;;
    stop)     stop_all ;;
    status)   show_status ;;
    *) echo "Usage: $0 [backend|frontend|edge|up|down|install|build|all|stop|status]" ;;
  esac
  exit 0
fi

print_banner
while true; do
  print_menu
  echo -ne "${WHITE}Pilih opsi: ${RESET}"
  read -r choice
  echo ""

  case "$choice" in
    1) start_backend ;;
    2) start_frontend ;;
    3) start_edge ;;
    4) docker_up ;;
    5) docker_down ;;
    6) install_all ;;
    7) build_frontend ;;
    8) start_all ;;
    9) stop_all ;;
    s|S) show_status ;;
    q|Q) echo -e "${CYAN}Goodbye!${RESET}"; exit 0 ;;
    *) echo -e "${RED}Opsi tidak valid: $choice${RESET}" ;;
  esac
  echo ""
done
