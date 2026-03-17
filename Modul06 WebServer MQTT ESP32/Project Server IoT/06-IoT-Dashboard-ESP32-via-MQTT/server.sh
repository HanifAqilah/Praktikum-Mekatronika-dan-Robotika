#!/usr/bin/env bash
# =============================================================================
#  Percobaan 6 – Server Management Script
#  Usage: bash server.sh
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BACKEND_DIR="$SCRIPT_DIR/Server/Backend"
EDGE_DIR="$SCRIPT_DIR/EdgePython"
ESP32_SKETCH="$SCRIPT_DIR/ESP32/percobaan6/percobaan6.ino"

# ── Color helpers ─────────────────────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

info()    { echo -e "${CYAN}[INFO]${RESET}  $*"; }
success() { echo -e "${GREEN}[OK]${RESET}    $*"; }
warn()    { echo -e "${YELLOW}[WARN]${RESET}  $*"; }
error()   { echo -e "${RED}[ERROR]${RESET} $*" >&2; }

# ── Require a command ─────────────────────────────────────────────────────────
require() {
  command -v "$1" &>/dev/null || { error "Required command '$1' not found."; return 1; }
}

# ═════════════════════════════════════════════════════════════════════════════
# Option 1 – Start Backend
# ═════════════════════════════════════════════════════════════════════════════
start_backend() {
  echo -e "${BOLD}── Starting Backend ────────────────────────────────────${RESET}"
  require node

  if [ ! -d "$BACKEND_DIR/node_modules" ]; then
    warn "node_modules not found – running npm install first…"
    (cd "$BACKEND_DIR" && npm install)
  fi

  # Copy .env.example → .env if missing
  if [ ! -f "$BACKEND_DIR/.env" ]; then
    cp "$BACKEND_DIR/.env.example" "$BACKEND_DIR/.env"
    warn ".env not found; copied from .env.example — edit $BACKEND_DIR/.env before use."
  fi

  info "Starting Node.js server on port ${PORT:-3006}…"
  cd "$BACKEND_DIR" && node server.js
}

# ═════════════════════════════════════════════════════════════════════════════
# Option 2 – Start EdgePython
# ═════════════════════════════════════════════════════════════════════════════
start_edge() {
  echo -e "${BOLD}── Starting EdgePython ─────────────────────────────────${RESET}"
  require python3

  # Prefer venv
  if [ -d "$EDGE_DIR/.venv" ]; then
    PYTHON="$EDGE_DIR/.venv/bin/python3"
  else
    PYTHON="python3"
  fi

  if [ ! -f "$EDGE_DIR/.env" ]; then
    cp "$EDGE_DIR/.env.example" "$EDGE_DIR/.env"
    warn ".env not found; copied from .env.example — edit $EDGE_DIR/.env before use."
  fi

  info "Starting EdgePython MQTT subscriber…"
  cd "$EDGE_DIR" && "$PYTHON" main.py
}

# ═════════════════════════════════════════════════════════════════════════════
# Option 3 – Install dependencies
# ═════════════════════════════════════════════════════════════════════════════
install_deps() {
  echo -e "${BOLD}── Installing Dependencies ─────────────────────────────${RESET}"

  # Node.js
  if require node; then
    info "Installing npm packages for backend…"
    (cd "$BACKEND_DIR" && npm install)
    success "npm packages installed."
  fi

  # Python
  if require python3; then
    info "Creating Python virtual environment in $EDGE_DIR/.venv …"
    python3 -m venv "$EDGE_DIR/.venv"
    "$EDGE_DIR/.venv/bin/pip" install --upgrade pip -q
    "$EDGE_DIR/.venv/bin/pip" install -r "$EDGE_DIR/requirements.txt"
    success "Python packages installed in .venv."
  fi

  success "All dependencies installed."
}

# ═════════════════════════════════════════════════════════════════════════════
# Option 4 – Build All
# ═════════════════════════════════════════════════════════════════════════════
build_all() {
  echo -e "${BOLD}── Build All ───────────────────────────────────────────${RESET}"

  # ── 1. Install Mosquitto ──────────────────────────────────────────────────
  if ! command -v mosquitto &>/dev/null; then
    info "Installing Mosquitto MQTT broker…"
    if command -v apt-get &>/dev/null; then
      sudo apt-get update -qq && sudo apt-get install -y mosquitto mosquitto-clients
      sudo systemctl enable mosquitto --now 2>/dev/null || true
      success "Mosquitto installed and started."
    else
      warn "apt-get not available – install Mosquitto manually."
    fi
  else
    success "Mosquitto already installed: $(mosquitto -h 2>&1 | head -1)"
  fi

  # ── 2. npm install ────────────────────────────────────────────────────────
  if require node; then
    info "Running npm install for backend…"
    (cd "$BACKEND_DIR" && npm install)
    success "npm install complete."
  fi

  # ── 3. Python venv + pip ──────────────────────────────────────────────────
  if require python3; then
    info "Setting up Python venv and installing packages…"
    python3 -m venv "$EDGE_DIR/.venv"
    "$EDGE_DIR/.venv/bin/pip" install --upgrade pip -q
    "$EDGE_DIR/.venv/bin/pip" install -r "$EDGE_DIR/requirements.txt"
    success "Python packages installed."
  fi

  # ── 4. arduino-cli – compile ESP32 sketch ────────────────────────────────
  if command -v arduino-cli &>/dev/null; then
    info "arduino-cli found – installing required libraries…"

    arduino-cli lib install "PubSubClient"  2>/dev/null || true
    arduino-cli lib install "ArduinoJson"   2>/dev/null || true

    # Ensure ESP32 core is installed
    if ! arduino-cli core list 2>/dev/null | grep -q "esp32:esp32"; then
      info "Adding ESP32 board URL and installing core…"
      arduino-cli config add board_manager.additional_urls \
        "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" \
        2>/dev/null || true
      arduino-cli core update-index
      arduino-cli core install esp32:esp32
    fi

    info "Compiling ESP32 sketch: $ESP32_SKETCH"
    if arduino-cli compile \
        --fqbn esp32:esp32:esp32 \
        "$(dirname "$ESP32_SKETCH")" \
        --output-dir "$SCRIPT_DIR/ESP32/build"; then
      success "ESP32 sketch compiled successfully → $SCRIPT_DIR/ESP32/build/"
    else
      error "Sketch compilation failed – check the output above."
    fi
  else
    warn "arduino-cli not found – skipping ESP32 compilation."
    warn "Install from: https://arduino.github.io/arduino-cli/latest/installation/"
  fi

  success "Build All complete."
}

# ═════════════════════════════════════════════════════════════════════════════
# Main Menu
# ═════════════════════════════════════════════════════════════════════════════
main_menu() {
  echo ""
  echo -e "${BOLD}${CYAN}╔══════════════════════════════════════════════╗${RESET}"
  echo -e "${BOLD}${CYAN}║   Percobaan 6 – IoT MQTT ESP32 Dashboard    ║${RESET}"
  echo -e "${BOLD}${CYAN}╚══════════════════════════════════════════════╝${RESET}"
  echo ""
  echo -e "  ${BOLD}1)${RESET} Start Backend        (node Server/Backend/server.js)"
  echo -e "  ${BOLD}2)${RESET} Start EdgePython     (python3 EdgePython/main.py)"
  echo -e "  ${BOLD}3)${RESET} Install Dependencies (npm + pip)"
  echo -e "  ${BOLD}4)${RESET} Build All            (Mosquitto + npm + pip + arduino-cli)"
  echo -e "  ${BOLD}0)${RESET} Exit"
  echo ""
  read -rp "  Pilih menu [0-4]: " CHOICE
  echo ""

  case "$CHOICE" in
    1) start_backend ;;
    2) start_edge    ;;
    3) install_deps  ;;
    4) build_all     ;;
    0) echo "Keluar."; exit 0 ;;
    *) warn "Pilihan tidak valid: '$CHOICE'"; sleep 1; main_menu ;;
  esac
}

# If called with an argument, run that option directly (CI-friendly)
if [[ "${1:-}" =~ ^[1-4]$ ]]; then
  case "$1" in
    1) start_backend ;;
    2) start_edge    ;;
    3) install_deps  ;;
    4) build_all     ;;
  esac
else
  main_menu
fi
