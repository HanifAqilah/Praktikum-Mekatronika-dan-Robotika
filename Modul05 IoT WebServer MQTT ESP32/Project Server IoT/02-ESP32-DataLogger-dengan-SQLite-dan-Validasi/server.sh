#!/usr/bin/env bash
# ============================================================
#  server.sh – Helper script untuk Percobaan 2
#  ESP32 DataLogger (SQLite + Zod + Parcel)
# ============================================================

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
BOLD='\033[1m'
RESET='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BACKEND_DIR="${SCRIPT_DIR}/Server/Backend"
FRONTEND_DIR="${SCRIPT_DIR}/Server/Frontend"
BACKEND_URL="http://localhost:3002"

print_banner() {
  echo -e "${CYAN}${BOLD}"
  echo "╔══════════════════════════════════════════════╗"
  echo "║   ESP32 DataLogger – Percobaan 2             ║"
  echo "║   SQLite + Zod + Express + Parcel            ║"
  echo "╚══════════════════════════════════════════════╝"
  echo -e "${RESET}"
}

print_menu() {
  echo -e "${BOLD}Pilih aksi:${RESET}"
  echo -e "  ${GREEN}1)${RESET} Start Backend Server"
  echo -e "  ${YELLOW}2)${RESET} Install Backend Dependencies (npm install)"
  echo -e "  ${CYAN}3)${RESET} Build Frontend (Parcel)"
  echo -e "  ${MAGENTA}4)${RESET} Install Frontend Dependencies (npm install)"
  echo -e "  ${YELLOW}5)${RESET} Dev Mode – Frontend Parcel Dev Server (port 1234)"
  echo -e "  ${RED}6)${RESET} Keluar"
  echo ""
  printf "Masukkan pilihan [1-6]: "
}

check_dir() {
  local dir="$1"
  local label="$2"
  if [ ! -d "$dir" ]; then
    echo -e "${RED}[!] Direktori ${label} tidak ditemukan: ${dir}${RESET}"
    return 1
  fi
  return 0
}

start_backend() {
  check_dir "$BACKEND_DIR" "backend" || return 1
  echo -e "\n${GREEN}[+] Menjalankan backend di ${BACKEND_URL} ...${RESET}"
  echo -e "${YELLOW}    Tekan Ctrl+C untuk menghentikan.${RESET}\n"
  # Ensure .env exists (copy from example if not)
  if [ ! -f "${BACKEND_DIR}/.env" ] && [ -f "${BACKEND_DIR}/.env.example" ]; then
    cp "${BACKEND_DIR}/.env.example" "${BACKEND_DIR}/.env"
    echo -e "${YELLOW}[+] .env dibuat dari .env.example${RESET}"
  fi
  cd "$BACKEND_DIR" && node src/index.js
}

install_backend() {
  check_dir "$BACKEND_DIR" "backend" || return 1
  echo -e "\n${YELLOW}[+] Menginstall backend dependencies ...${RESET}"
  cd "$BACKEND_DIR" && npm install
  echo -e "${GREEN}[✓] Backend dependencies berhasil diinstall.${RESET}\n"
}

build_frontend() {
  check_dir "$FRONTEND_DIR" "frontend" || return 1
  if [ ! -d "${FRONTEND_DIR}/node_modules" ]; then
    echo -e "${YELLOW}[!] node_modules belum ada. Jalankan Install Frontend (opsi 4) terlebih dahulu.${RESET}"
    return 1
  fi
  echo -e "\n${CYAN}[+] Membangun frontend dengan Parcel ...${RESET}"
  cd "$FRONTEND_DIR" && npm run build
  echo -e "${GREEN}[✓] Frontend berhasil di-build ke ${FRONTEND_DIR}/dist${RESET}\n"
}

install_frontend() {
  check_dir "$FRONTEND_DIR" "frontend" || return 1
  echo -e "\n${MAGENTA}[+] Menginstall frontend dependencies ...${RESET}"
  cd "$FRONTEND_DIR" && npm install
  echo -e "${GREEN}[✓] Frontend dependencies berhasil diinstall.${RESET}\n"
}

dev_frontend() {
  check_dir "$FRONTEND_DIR" "frontend" || return 1
  echo -e "\n${MAGENTA}[+] Menjalankan Parcel dev server di port 1234 ...${RESET}"
  echo -e "${YELLOW}    Tekan Ctrl+C untuk menghentikan.${RESET}\n"
  cd "$FRONTEND_DIR" && npm run dev
}

# ── Main ──────────────────────────────────────────────────────
print_banner

while true; do
  print_menu
  read -r choice
  echo ""
  case "$choice" in
    1) start_backend ;;
    2) install_backend ;;
    3) build_frontend ;;
    4) install_frontend ;;
    5) dev_frontend ;;
    6) echo -e "${CYAN}Sampai jumpa!${RESET}"; exit 0 ;;
    *) echo -e "${RED}[!] Pilihan tidak valid. Coba lagi.${RESET}\n" ;;
  esac
done
