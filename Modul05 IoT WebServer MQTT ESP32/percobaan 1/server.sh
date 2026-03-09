#!/usr/bin/env bash
# ============================================================
#  server.sh – Helper script untuk Percobaan 1
#  ESP32 WebServer Monitor
# ============================================================

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

BACKEND_DIR="$(cd "$(dirname "$0")/Server/Backend" && pwd)"
URL="http://localhost:3001"

print_banner() {
  echo -e "${CYAN}${BOLD}"
  echo "╔══════════════════════════════════════════╗"
  echo "║   ESP32 WebServer Monitor – Percobaan 1  ║"
  echo "╚══════════════════════════════════════════╝"
  echo -e "${RESET}"
}

print_menu() {
  echo -e "${BOLD}Pilih aksi:${RESET}"
  echo -e "  ${GREEN}1)${RESET} Start Backend Server"
  echo -e "  ${YELLOW}2)${RESET} Install Dependencies (npm install)"
  echo -e "  ${CYAN}3)${RESET} Buka di Browser"
  echo -e "  ${RED}4)${RESET} Keluar"
  echo ""
  printf "Masukkan pilihan [1-4]: "
}

start_backend() {
  echo -e "\n${GREEN}[+] Menjalankan backend di ${URL} ...${RESET}"
  echo -e "${YELLOW}    Tekan Ctrl+C untuk menghentikan server.${RESET}\n"
  cd "$BACKEND_DIR" || { echo -e "${RED}[!] Direktori backend tidak ditemukan: $BACKEND_DIR${RESET}"; exit 1; }
  node server.js
}

install_deps() {
  echo -e "\n${YELLOW}[+] Menginstall dependencies di $BACKEND_DIR ...${RESET}"
  cd "$BACKEND_DIR" || { echo -e "${RED}[!] Direktori backend tidak ditemukan.${RESET}"; return 1; }
  npm install
  echo -e "${GREEN}[✓] Dependencies berhasil diinstall.${RESET}\n"
}

open_browser() {
  echo -e "\n${CYAN}[+] URL Dashboard: ${BOLD}${URL}${RESET}"
  if command -v xdg-open &>/dev/null; then
    xdg-open "$URL" &
  elif command -v open &>/dev/null; then
    open "$URL"
  elif command -v start &>/dev/null; then
    start "$URL"
  else
    echo -e "${YELLOW}    Buka URL di browser secara manual: ${URL}${RESET}"
  fi
  echo ""
}

# ── Main ──────────────────────────────────────────────────────
print_banner

while true; do
  print_menu
  read -r choice
  echo ""
  case "$choice" in
    1) start_backend ;;
    2) install_deps ;;
    3) open_browser ;;
    4) echo -e "${CYAN}Sampai jumpa!${RESET}"; exit 0 ;;
    *) echo -e "${RED}[!] Pilihan tidak valid. Coba lagi.${RESET}\n" ;;
  esac
done
