#!/bin/bash

# Cross-platform Serial monitor for ESP32
# Works on macOS, Linux, and Windows (WSL/Git Bash)
# Auto-detects ESP32 across different USB adapters

set -e

BAUD=115200
OS=""

# Detect operating system
detect_os() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
    elif [[ "$OSTYPE" == "linux"* ]]; then
        OS="linux"
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
        OS="windows"
    else
        OS="unknown"
    fi
}

# Find ESP32 ports across different platforms
find_esp32_ports() {
    local ports=()

    case "$OS" in
        macos)
            # macOS: Use cu.* (call-out) for bidirectional serial communication
            # Covers: CP2102, CH340, FTDI, and generic USB serial adapters
            for port in /dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.SLAB_USBtoUART* /dev/cu.wchusbserial*; do
                [ -e "$port" ] && ports+=("$port")
            done
            ;;
        linux)
            # Linux: ttyUSB*, ttyACM*, ttyS* for various adapters
            for port in /dev/ttyUSB* /dev/ttyACM* /dev/ttyS*; do
                [ -e "$port" ] && ports+=("$port")
            done
            ;;
        windows)
            # Windows: COM ports (COM3, COM4, etc)
            # Typically COM1-COM9, but can go higher
            for port in /dev/ttyS* COM{1..9}; do
                if [ -e "$port" ] || [ -c "$port" ] 2>/dev/null; then
                    ports+=("$port")
                fi
            done
            ;;
    esac

    # Remove duplicates and sort
    if [ ${#ports[@]} -gt 0 ]; then
        printf '%s\n' "${ports[@]}" | sort -u
    fi
}

# Connect to serial port with appropriate commands for the OS
connect_port() {
    local port="$1"

    case "$OS" in
        macos)
            stty -f "$port" $BAUD raw -echo
            cat "$port"
            ;;
        linux)
            stty -F "$port" $BAUD raw -echo
            cat "$port"
            ;;
        windows)
            # Windows serial communication
            mode "$port" baud=$BAUD parity=N data=8 stop=1 2>/dev/null || true
            cat < "$port"
            ;;
        *)
            echo "❌ Unsupported OS"
            exit 1
            ;;
    esac
}

main() {
    detect_os

    echo "🔍 Searching for ESP32 on $OS..."
    echo ""

    # Read ports into array (compatible with older Bash versions)
    PORTS=()
    while IFS= read -r port; do
        [ -n "$port" ] && PORTS+=("$port")
    done < <(find_esp32_ports)

    if [ ${#PORTS[@]} -eq 0 ]; then
        echo "❌ No ESP32 serial ports found!"
        echo ""
        echo "ℹ️  Please make sure:"
        echo "   • ESP32 is connected via USB"
        echo "   • You have permission to access /dev/* (Linux users may need sudo)"
        echo "   • Driver is installed (CH340, CP2102, FTDI, etc.)"
        echo "   • Port is not in use by another application"
        exit 1
    fi

    # Select port
    if [ -z "$1" ]; then
        if [ ${#PORTS[@]} -eq 1 ]; then
            PORT="${PORTS[0]}"
            echo "✅ Found ESP32 at: $PORT"
        else
            echo "📡 Multiple ports available:"
            for i in "${!PORTS[@]}"; do
                echo "  [$((i+1))] ${PORTS[$i]}"
            done
            echo ""
            read -p "Select port (1-${#PORTS[@]}): " choice
            PORT="${PORTS[$((choice-1))]}"
        fi
    else
        PORT="$1"
    fi

    if ! [ -e "$PORT" ] && ! [ -c "$PORT" ] 2>/dev/null; then
        echo "❌ Port $PORT not found"
        exit 1
    fi

    echo ""
    echo "📡 Connecting to $PORT at $BAUD baud..."
    echo "📝 Press Ctrl+C to exit"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""

    # Connect and handle Ctrl+C gracefully
    trap 'echo ""; echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; echo "👋 Disconnected"; exit 0' INT

    connect_port "$PORT"
}

main "$@"
