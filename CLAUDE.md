# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is an OpenGL learning sandbox with two parallel implementations:

- **Rust** (`src/`) — uses `glfw` + `gl` + `sdl2` crates (currently a stub)
- **C++** (`cpp/`) — uses GLFW 3.4 directly via CMake (the active implementation)

## Build Commands

### C++ (primary)

```bash
./build-cpp.sh          # configure + build (outputs to cpp/build/)
./cpp/build/opengl-sandbox  # run
```

The script runs `cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Debug && cmake --build cpp/build`.

### Rust

```bash
cargo build
cargo run
```

## Architecture Notes

**C++ setup:** `cpp/CMakeLists.txt` links against a locally-built GLFW at `/Users/defv/dev/libs/glfw-3.4/` (headers) and `build/src/` relative to the repo root (the pre-built `libglfw3`). The repo root `build/` directory is a GLFW CMake build tree, not an application build output.

OpenGL 3.3 Core Profile is targeted. macOS requires `GLFW_OPENGL_FORWARD_COMPAT` and `GL_SILENCE_DEPRECATION`.

`CMAKE_EXPORT_COMPILE_COMMANDS ON` is set, so `cpp/build/compile_commands.json` is generated for clangd (`.gitignore` excludes the clangd cache).

**Rust setup:** Dependencies are `gl` (OpenGL bindings), `glfw` (GLFW bindings), and `sdl2` (SDL2 bindings). The `src/main.rs` entry point is currently a placeholder.
