Build / Run:
- Build: use the provided build script / Makefile for this template.
- Run: run the generated executable from the project root folder (so the program can find shaders in ./shaders).

Controls:
Phase 1 (2 pts) - Static Dots
- The program starts by drawing multiple static dots using VK_PRIMITIVE_TOPOLOGY_POINT_LIST.

Phase 2 (4 pts) - Add Dots by Mouse Click
- Left mouse click adds a dot at the cursor position.

Phase 3 (2 pts) - Reset View
- Press SPACE to reset the view (clear all dots).

Phase 4 (2 pts) - Creativity Feature
- (If implemented) Press 'D' to remove the most recently added dot.
  Note: This feature is intended as the additional creativity option.

Notes:
- Dots are rendered as Vulkan point primitives (POINT_LIST).
- Shader files are loaded from the ./shaders directory.
