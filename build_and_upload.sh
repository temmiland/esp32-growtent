#!/bin/bash
set -x  # Show all commands being executed

# Build and upload sketch to ESP32 using Arduino CLI
# Compiler automatically combines all .ino files

# Setup WiFi credentials from .env file
bash setup_wifi.sh
if [ $? -ne 0 ]; then
    exit 1
fi

BOARD="esp32:esp32:XIAO_ESP32C6"

# Auto-detect port or use provided one
if [ -z "$1" ]; then
    # Try to find XIAO_ESP32C6, else take first ESP32 device
    PORT=$(arduino-cli board list | grep -i "esp32" | awk '{print $1}' | head -1)
    if [ -z "$PORT" ]; then
        echo "❌ Error: No ESP32 board found!"
        echo "Connect your board and try again."
        arduino-cli board list
        exit 1
    fi
else
    PORT="$1"
fi

echo "Using port: $PORT"

echo "🔨 Compiling sketch (combining all .ino files)..."
echo "Using partition scheme: huge_app (3MB)"
arduino-cli compile --fqbn "$BOARD:PartitionScheme=huge_app" -v .

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi

echo "✓ Compilation successful!"
echo ""
echo "📱 Ready to upload to ESP32"
echo "Port: $PORT"
echo ""
read -p "Continue upload? (y/n) " -n 1 -r
echo

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "❌ Upload cancelled"
    exit 0
fi

echo "Uploading..."
arduino-cli upload -p $PORT --fqbn "$BOARD:PartitionScheme=huge_app" .

if [ $? -ne 0 ]; then
    echo "❌ Upload failed!"
    echo "Check the port with: ls /dev/cu.usbserial-*"
    exit 1
fi

echo "✓ Upload successful!"
