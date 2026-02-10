# CPRE 557 — Computer Project #1: Dots (Vulkan + GLFW)

This project draws dots using Vulkan point primitives and supports interactive dot placement via mouse input.

## Features

- **Phase 1 (Static dots):** Draw multiple dots at startup.
- **Phase 2 (Interactive dots):** Add dots using mouse clicks.
- **Phase 3 (Reset):** Clear all dots with a key press.
- **Phase 4 (Creativity):** Optional extra feature (if implemented).
  
## Build & Run:

### Build
Use the provided course template build workflow (Makefile / script).

### Run (important)
Run the executable with the **project root** as the working directory so shader files can be loaded from:
- `./shaders/*.spv`

If you use **VS Code**, set the working directory in `.vscode/launch.json`:
```json
"cwd": "${workspaceFolder}"
