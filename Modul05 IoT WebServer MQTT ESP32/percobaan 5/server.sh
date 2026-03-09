#!/bin/bash
# =============================================================================
#  Percobaan 5 — Real-time WebSocket Dashboard
#  Script manajemen Backend & Frontend
# =============================================================================

set -euo pipefail

# ─── ANSI colors ─────────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BACKEND_DIR="$SCRIPT_DIR/Server/Backend"
FRONTEND_DIR="$SCRIPT_DIR/Server/Frontend"

BACKEND_PORT=8005
FRONTEND_PORT=9005

# ─── Helpers ─────────────────────────────────────────────────────────────────
print_header() {
  echo -e ""
  echo -e "${BOLD}${BLUE}╔══════════════════════════════════════════════════╗${RESET}"
  echo -e "${BOLD}${BLUE}║   Percobaan 5 — Real-time WebSocket Dashboard   ║${RESET}"
  echo -e "${BOLD}${BLUE}╚══════════════════════════════════════════════════╝${RESET}"
  echo -e ""
}

print_menu() {
  echo -e "${BOLD}  Pilih opsi:${RESET}"
  echo -e "  ${CYAN}1)${RESET} Start Backend      ${YELLOW}(port ${BACKEND_PORT})${RESET}"
  echo -e "  ${CYAN}2)${RESET} Start Frontend Dev ${YELLOW}(port ${FRONTEND_PORT})${RESET}"
  echo -e "  ${CYAN}3)${RESET} Install Backend dependencies  (npm install)"
  echo -e "  ${CYAN}4)${RESET} Install Frontend dependencies (npm install)"
  echo -e "  ${CYAN}5)${RESET} Build Frontend     (npm run build)"
  echo -e "  ${CYAN}6)${RESET} ${GREEN}Start All${RESET}          (backend bg + frontend fg)"
  echo -e "  ${CYAN}q)${RESET} Keluar"
  echo -e ""
}

print_urls() {
  echo -e ""
  echo -e "  ${GREEN}✔  URLs:${RESET}"
  echo -e "     Backend API : ${YELLOW}http://localhost:${BACKEND_PORT}${RESET}"
  echo -e "     WebSocket   : ${YELLOW}ws://localhost:${BACKEND_PORT}/ws${RESET}"
  echo -e "     Frontend    : ${YELLOW}http://localhost:${FRONTEND_PORT}${RESET}"
  echo -e ""
}

info()    { echo -e "  ${CYAN}[INFO]${RESET}  $*"; }
success() { echo -e "  ${GREEN}[OK]${RESET}    $*"; }
warn()    { echo -e "  ${YELLOW}[WARN]${RESET}  $*"; }
error()   { echo -e "  ${RED}[ERR]${RESET}   $*"; }

check_node() {
  if ! command -v node &>/dev/null; then
    error "Node.js tidak ditemukan. Install Node.js >= 20 terlebih dahulu."
    exit 1
  fi
  local ver
  ver=$(node -e "process.stdout.write(process.versions.node)")
  info "Node.js versi: ${ver}"
}

# ─── Actions ─────────────────────────────────────────────────────────────────
start_backend() {
  check_node
  if [ ! -d "$BACKEND_DIR/node_modules" ]; then
    warn "node_modules tidak ditemukan. Jalankan opsi 3 (install) terlebih dahulu."
    read -rp "  Install sekarang? [y/N] " ans
    if [[ "$ans" =~ ^[Yy]$ ]]; then install_backend; fi
  fi
  info "Memulai Backend di port ${BACKEND_PORT}…"
  print_urls
  cd "$BACKEND_DIR" && npm run dev
}

start_frontend() {
  check_node
  if [ ! -d "$FRONTEND_DIR/node_modules" ]; then
    warn "node_modules tidak ditemukan. Jalankan opsi 4 (install) terlebih dahulu."
    read -rp "  Install sekarang? [y/N] " ans
    if [[ "$ans" =~ ^[Yy]$ ]]; then install_frontend; fi
  fi
  info "Memulai Frontend Dev di port ${FRONTEND_PORT}…"
  print_urls
  cd "$FRONTEND_DIR" && npm run dev
}

install_backend() {
  check_node
  info "Installing backend dependencies…"
  cd "$BACKEND_DIR" && npm install
  success "Backend dependencies installed."
}

install_frontend() {
  check_node
  info "Installing frontend dependencies…"
  cd "$FRONTEND_DIR" && npm install
  success "Frontend dependencies installed."
}

build_frontend() {
  check_node
  info "Building frontend…"
  cd "$FRONTEND_DIR" && npm run build
  success "Frontend build selesai. Output: $FRONTEND_DIR/dist"
}

start_all() {
  check_node
  info "Memulai Backend di background (port ${BACKEND_PORT})…"
  cd "$BACKEND_DIR"
  npm run dev &
  BACKEND_PID=$!
  success "Backend PID: ${BACKEND_PID}"

  sleep 2

  info "Memulai Frontend Dev di foreground (port ${FRONTEND_PORT})…"
  print_urls
  cd "$FRONTEND_DIR" && npm run dev

  # Cleanup on exit
  kill "$BACKEND_PID" 2>/dev/null && info "Backend (PID ${BACKEND_PID}) dihentikan." || true
}

# ─── Main ─────────────────────────────────────────────────────────────────────
print_header

if [ $# -gt 0 ]; then
  case "$1" in
    1) start_backend ;;
    2) start_frontend ;;
    3) install_backend ;;
    4) install_frontend ;;
    5) build_frontend ;;
    6) start_all ;;
    *) error "Opsi tidak dikenal: $1" ;;
  esac
  exit 0
fi

while true; do
  print_menu
  read -rp "  Pilihan: " choice
  echo ""
  case "$choice" in
    1) start_backend ;;
    2) start_frontend ;;
    3) install_backend ;;
    4) install_frontend ;;
    5) build_frontend ;;
    6) start_all ;;
    q|Q) info "Keluar."; exit 0 ;;
    *) warn "Pilihan tidak valid." ;;
  esac
done
