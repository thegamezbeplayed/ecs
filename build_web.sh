#!/bin/bash
# ==========================================
# Raylib WASDM WebAssembly build script
# ==========================================

# 1. Location of your emsdk (edit this path if needed)
EMSDK_DIR="$HOME/Projects/emsdk"

# 2. Activate Emscripten environment
if [ -d "$EMSDK_DIR" ]; then
  echo "Activating Emscripten environment..."
  source "$EMSDK_DIR/emsdk_env.sh"
else
  echo "❌ Could not find emsdk at $EMSDK_DIR"
  echo "Please clone it first:"
  echo "  git clone https://github.com/emscripten-core/emsdk.git"
  echo "  cd emsdk && ./emsdk install latest && ./emsdk activate latest"
  exit 1
fi

# 3. Build for WebAssembly
echo "Building WebAssembly target..."
mkdir -p build_web
cd build_web || exit 1

emcmake cmake -DPLATFORM=Web .. || { echo "❌ CMake configuration failed"; exit 1; }
emmake make || { echo "❌ Build failed"; exit 1; }

# 4. Serve locally (optional)
if command -v python3 &> /dev/null; then
  echo
  echo "✅ Build complete!"
  echo "To test, run:"
  echo "  cd build_web && python3 -m http.server 8080"
  echo "Then open http://localhost:8080/wasdm.html"
else
  echo "✅ Build complete! Install Python to run a local test server."
fi
