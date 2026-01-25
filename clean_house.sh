#!/bin/sh
set -e

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Removing build artifacts under: $ROOT_DIR"
rm -rf "$ROOT_DIR"/build*
rm -rf "$ROOT_DIR"/dist

echo "Clean complete."
