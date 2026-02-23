# CPRE 557 — Computer Project #1: Dots (Vulkan + GLFW)

This project draws dots using Vulkan point primitives and supports interactive dot placement via mouse input.

<p align="center">
  <img src="assets/teaser.gif" width="560">
</p>

## Features

- **Phase 1 (Static dots):** Draw multiple dots at startup.
- **Phase 2 (Interactive dots):** Add dots using mouse clicks.
- **Phase 3 (Reset View):** Clear all dots with a key press.
- **Phase 4 (Remove dots):** Press 'D' to remove the most recently added dot.
  
## Build & Run:

### Build
Use the provided course template build workflow (Makefile / script).

### Run (important)
Run the executable with the **project root** as the working directory so shader files can be loaded from:
- `./shaders/*.spv`

If you use **VS Code**, set the working directory in `.vscode/launch.json`:
```json
"cwd": "${workspaceFolder}"
```

## Controls

- **Left Click:** Add a dot at the cursor position  
- **SPACE:** Reset view (clear all dots)

## Shader Compilation

Vulkan loads **SPIR-V** shader binaries (`.spv`). If you modify GLSL shaders, recompile them:

```bash
.\compile-win.bat
```

## Notes

- Dots are rendered with `VK_PRIMITIVE_TOPOLOGY_POINT_LIST`.
- Dot placement converts window pixel coordinates to NDC coordinates `[-1, 1]`.
