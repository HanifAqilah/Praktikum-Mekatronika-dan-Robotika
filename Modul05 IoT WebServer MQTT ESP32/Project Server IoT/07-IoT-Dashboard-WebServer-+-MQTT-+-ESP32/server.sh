#!/usr/bin/env bash
# =============================================================================
# server.sh – Percobaan 7 Management Script
# Usage: bash server.sh [option]
#   1 | backend    – Start FastAPI backend (port 8007)
#   2 | frontend   – Start Vue dev server (port 9007)
#   3 | edge       – Start EdgePython agent
#   4 | install    – Install all dependencies
#   5 | build      – Build Vue frontend for production
#   0 | menu       – Interactive menu (default)
# =============================================================================

set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; NC='\033[0m'; BOLD='\033[1m'

banner() {
    echo -e "${CYAN}${BOLD}"
    echo "╔════════════════════════════════════════════╗"
    echo "║   Percobaan 7 – IoT Dashboard Manager      ║"
    echo "║   Backend :8007 | Frontend :9007            ║"
    echo "╚════════════════════════════════════════════╝"
    echo -e "${NC}"
}

# ─── Install ──────────────────────────────────────────────────────────────────
install_all() {
    echo -e "${YELLOW}[1/3] Installing Backend dependencies…${NC}"
    cd "$SCRIPT_DIR/Server/Backend"
    if [ ! -f ".env" ]; then
        cp .env.example .env
        echo -e "${GREEN}  Created .env from .env.example (edit MQTT settings!)${NC}"
    fi
    python3 -m pip install --quiet -r requirements.txt
    echo -e "${GREEN}  Backend deps installed.${NC}"

    echo -e "${YELLOW}[2/3] Installing Frontend dependencies…${NC}"
    cd "$SCRIPT_DIR/Server/Frontend"
    npm install --silent
    echo -e "${GREEN}  Frontend deps installed.${NC}"

    echo -e "${YELLOW}[3/3] Installing EdgePython dependencies…${NC}"
    cd "$SCRIPT_DIR/EdgePython"
    if [ ! -f ".env" ]; then
        cp .env.example .env
        echo -e "${GREEN}  Created .env from .env.example${NC}"
    fi
    python3 -m pip install --quiet -r requirements.txt
    echo -e "${GREEN}  EdgePython deps installed.${NC}"

    echo -e "${GREEN}${BOLD}✔ All dependencies installed successfully!${NC}"
}

# ─── Backend ──────────────────────────────────────────────────────────────────
start_backend() {
    echo -e "${CYAN}Starting FastAPI backend on http://localhost:8007${NC}"
    cd "$SCRIPT_DIR/Server/Backend"
    [ ! -f ".env" ] && { echo -e "${RED}Missing .env – run install first${NC}"; exit 1; }
    python3 -m uvicorn main:app --host 0.0.0.0 --port 8007 --reload
}

# ─── Frontend ─────────────────────────────────────────────────────────────────
start_frontend() {
    echo -e "${CYAN}Starting Vue dev server on http://localhost:9007${NC}"
    cd "$SCRIPT_DIR/Server/Frontend"
    [ ! -d "node_modules" ] && { echo -e "${RED}node_modules missing – run install first${NC}"; exit 1; }
    npm run dev
}

# ─── EdgePython ───────────────────────────────────────────────────────────────
start_edge() {
    echo -e "${CYAN}Starting EdgePython agent…${NC}"
    cd "$SCRIPT_DIR/EdgePython"
    [ ! -f ".env" ] && { echo -e "${RED}Missing .env – run install first${NC}"; exit 1; }
    python3 main.py
}

# ─── Build ────────────────────────────────────────────────────────────────────
build_frontend() {
    echo -e "${YELLOW}Building Vue frontend for production…${NC}"
    cd "$SCRIPT_DIR/Server/Frontend"
    [ ! -d "node_modules" ] && { echo -e "${RED}node_modules missing – run install first${NC}"; exit 1; }
    npm run build
    echo -e "${GREEN}Build output: Server/Frontend/dist/${NC}"
}

# ─── Interactive menu ─────────────────────────────────────────────────────────
show_menu() {
    banner
    echo -e "  ${BOLD}1)${NC} Start Backend      (FastAPI + aiomqtt)"
    echo -e "  ${BOLD}2)${NC} Start Frontend Dev  (Vue 3 + Vite)"
    echo -e "  ${BOLD}3)${NC} Start EdgePython    (Rules engine + aggregator)"
    echo -e "  ${BOLD}4)${NC} Install all deps"
    echo -e "  ${BOLD}5)${NC} Build frontend (production)"
    echo -e "  ${BOLD}0)${NC} Exit"
    echo ""
    read -rp "Pilihan: " choice
    case "$choice" in
        1) start_backend ;;
        2) start_frontend ;;
        3) start_edge ;;
        4) install_all ;;
        5) build_frontend ;;
        0) echo "Bye!"; exit 0 ;;
        *) echo -e "${RED}Pilihan tidak valid${NC}"; show_menu ;;
    esac
}

# ─── Entry point ─────────────────────────────────────────────────────────────
case "${1:-menu}" in
    1 | backend)  start_backend ;;
    2 | frontend) start_frontend ;;
    3 | edge)     start_edge ;;
    4 | install)  install_all ;;
    5 | build)    build_frontend ;;
    0 | menu)     show_menu ;;
    *)
        echo "Usage: $0 [1-5|backend|frontend|edge|install|build|menu]"
        exit 1 ;;
esac
