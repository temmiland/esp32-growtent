#!/bin/bash

# Load WiFi credentials from .env file and update settings.ino
# Note: Values in .env must be already properly escaped for C++ strings

ENV_FILE=".env"
SETTINGS_FILE="settings.ino"

# Check if .env exists
if [ ! -f "$ENV_FILE" ]; then
    echo "❌ Error: $ENV_FILE not found!"
    echo "Copy .env.example to .env and fill in your WiFi credentials"
    echo "  cp .env.example .env"
    exit 1
fi

# Check if .env exists
if [ ! -f "$ENV_FILE" ]; then
    echo "❌ Error: $ENV_FILE not found!"
    exit 1
fi

# Extract values from .env (with quotes)
SSID_VALUE=$(grep "^WIFI_SSID=" "$ENV_FILE" | cut -d'=' -f2-)
PASSWORD_VALUE=$(grep "^WIFI_PASSWORD=" "$ENV_FILE" | cut -d'=' -f2-)

echo "📝 Updating WiFi credentials..."

# Rebuild file line by line
{
  while IFS= read -r line; do
    if [[ "$line" == "const char *ssid = "* ]]; then
      echo "const char *ssid = $SSID_VALUE;"
    elif [[ "$line" == "const char *password = "* ]]; then
      echo "const char *password = $PASSWORD_VALUE;"
    else
      echo "$line"
    fi
  done < "$SETTINGS_FILE"
} > "$SETTINGS_FILE.tmp" && mv "$SETTINGS_FILE.tmp" "$SETTINGS_FILE"

echo "✓ WiFi credentials updated in $SETTINGS_FILE"

echo "✓ WiFi credentials updated in $SETTINGS_FILE"
