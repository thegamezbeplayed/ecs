# GamezBeMade (GBM)

A lightweight, data-oriented Entity Component System (ECS) written in C, designed for high performance with raylib and Emscripten (Web) support.

## Features
- Pure data components (dumb components preferred)
- Cache-friendly systems
- Web build support (Emscripten + stripped shell.html)
- Observer / Listener patterns for platform events (resize, etc.)
- CMake + custom build scripts

## Workflow Status

| Workflow | Status | Description |
|----------|--------|-------------|
| **Build** | [![Build Status](https://github.com/thegamezbeplayed/ecs/actions/workflows/build.yml/badge.svg)](https://github.com/thegamezbeplayed/ecs/actions/workflows/build.yml) | CI build for native + Web (Emscripten) |
| **Web Build** | (included in Build) | Emscripten compilation + size checks |

**Last successful build**: Recent runs on `main` and feature branches complete in \~1 minute with no failures.

### Quick Links
- [All Workflow Runs](https://github.com/thegamezbeplayed/ecs/actions)
- [Build Workflow Details](https://github.com/thegamezbeplayed/ecs/actions/workflows/build.yml)

## Quick Start

### Native Build
```bash
mkdir build && cd build
cmake ..
make
