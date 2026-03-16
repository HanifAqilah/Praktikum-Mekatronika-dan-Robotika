#!/usr/bin/env bash
# server.sh – Pengelola server Percobaan 4 (JWT + WebSocket + Angular)
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BACKEND_DIR="$SCRIPT_DIR/Server/Backend"
FRONTEND_DIR="$SCRIPT_DIR/Server/Frontend"

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_menu() {
  echo ""
  echo -e "${BLUE}╔══════════════════════════════════════╗${NC}"
  echo -e "${BLUE}║    Percobaan 4 – Server Manager      ║${NC}"
  echo -e "${BLUE}╚══════════════════════════════════════╝${NC}"
  echo ""
  echo "  1) Start Backend  (FastAPI :8004)"
  echo "  2) Start Frontend (Angular dev :9004)"
  echo "  3) Install Backend dependencies (pip)"
  echo "  4) Install Frontend dependencies (npm)"
  echo "  5) Build Frontend (production)"
  echo "  6) Full Setup (install + start both)"
  echo "  0) Keluar"
  echo ""
}

start_backend() {
  echo -e "${GREEN}[Backend] Menjalankan FastAPI pada port 8004...${NC}"
  cd "$BACKEND_DIR"
  if [ ! -f .env ]; then
    cp .env.example .env
    echo -e "${YELLOW}[Backend] .env dibuat dari .env.example${NC}"
    echo -e "${YELLOW}[Backend] PENTING: Ubah JWT_SECRET di .env sebelum dipakai produksi!${NC}"
  fi
  python main.py
}

start_frontend() {
  echo -e "${GREEN}[Frontend] Menjalankan Angular dev server pada port 9004...${NC}"
  cd "$FRONTEND_DIR"
  npm start
}

install_backend() {
  echo -e "${GREEN}[Backend] Menginstal dependensi Python...${NC}"
  cd "$BACKEND_DIR"
  pip install -r requirements.txt
  echo -e "${GREEN}[Backend] Selesai!${NC}"
}

install_frontend() {
  echo -e "${GREEN}[Frontend] Menginstal dependensi Node.js...${NC}"
  cd "$FRONTEND_DIR"
  npm install
  echo -e "${GREEN}[Frontend] Selesai!${NC}"
}

build_frontend() {
  echo -e "${GREEN}[Frontend] Membangun Angular untuk produksi...${NC}"
  cd "$FRONTEND_DIR"
  npm run build
  echo -e "${GREEN}[Frontend] Build selesai di folder dist/!${NC}"
}

full_setup() {
  install_backend
  install_frontend
  echo -e "${BLUE}[Setup] Memulai backend di background...${NC}"
  cd "$BACKEND_DIR"
  if [ ! -f .env ]; then
    cp .env.example .env
    echo -e "${YELLOW}[Backend] PENTING: Ubah JWT_SECRET di .env!${NC}"
  fi
  python main.py &
  BACKEND_PID=$!
  echo -e "${GREEN}[Backend] PID: $BACKEND_PID${NC}"
  sleep 2
  echo -e "${BLUE}[Setup] Memulai Angular dev server...${NC}"
  cd "$FRONTEND_DIR"
  npm start
}

while true; do
  print_menu
  read -rp "Pilih opsi [0-6]: " choice
  case $choice in
    1) start_backend ;;
    2) start_frontend ;;
    3) install_backend ;;
    4) install_frontend ;;
    5) build_frontend ;;
    6) full_setup ;;
    0) echo "Keluar."; exit 0 ;;
    *) echo -e "${RED}Pilihan tidak valid.${NC}" ;;
  esac
done
