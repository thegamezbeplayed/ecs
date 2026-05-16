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
| **Linux Desktop** | [![Linux Desktop](https://github.com/thegamezbeplayed/ecs/actions/workflows/linux-desktop.yml/badge.svg?branch=main)](https://github.com/thegamezbeplayed/ecs/actions/workflows/linux-desktop.yml) | Native Linux build + smoke run |
| **Windows Desktop** | [![Windows Desktop](https://github.com/thegamezbeplayed/ecs/actions/workflows/windows-desktop.yml/badge.svg?branch=main)](https://github.com/thegamezbeplayed/ecs/actions/workflows/windows-desktop.yml) | Native Windows build + smoke run |
| **Web** | [![Web](https://github.com/thegamezbeplayed/ecs/actions/workflows/web.yml/badge.svg?branch=main)](https://github.com/thegamezbeplayed/ecs/actions/workflows/web.yml) | Emscripten build + artifact checks |

### Quick Links
- [All Workflow Runs](https://github.com/thegamezbeplayed/ecs/actions)
- [Linux Desktop Workflow](https://github.com/thegamezbeplayed/ecs/actions/workflows/linux-desktop.yml)
- [Windows Desktop Workflow](https://github.com/thegamezbeplayed/ecs/actions/workflows/windows-desktop.yml)
- [Web Workflow](https://github.com/thegamezbeplayed/ecs/actions/workflows/web.yml)

## Quick Start

### Native Build
```bash
mkdir build && cd build
cmake ..
make
