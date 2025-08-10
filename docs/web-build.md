## Godot Rive Web (WASM) build guide

This project provides a minimal Web build path for the GDExtension as a WebAssembly side module. The initial goal is to load a minimal extension on the Web to validate the pipeline; rendering may be stubbed initially.

Prerequisites
- Emscripten SDK (recommended: 3.1.43 or 3.1.44)
- Python 3
- scons
- Godot 4.1+

Setup Emscripten (example)
```bash
# Linux/macOS shell example, see official docs for Windows PowerShell usage
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 3.1.44
./emsdk activate 3.1.44
source ./emsdk_env.sh
```
On Windows PowerShell, use `emsdk install/activate` and run `emsdk_env.ps1` in the current session.

Build commands
```bash
cd build
scons platform=web target=template_debug -j8
scons platform=web target=template_release -j8
```

Expected outputs
- demo/bin/librive.web.template_debug.wasm32.wasm
- demo/bin/librive.web.template_release.wasm32.wasm

Godot project setup
- demo/bin/librive.gdextension already includes web.debug and web.release entries pointing to the above paths
- In the Godot editor, ensure export preset is Web and run the export to test loading

Notes
- The Web build currently skips Skia/vcpkg and legacy Skia renderer sources
- Future work may implement a Web renderer path (e.g., CanvasKit bridge or a Godot canvas integration)



## Current progress (feature/web)

- Web side module builds and loads; .gdextension contains web.debug/release entries
- Pre-JS loader dynamically fetches CanvasKit (Skia WASM) and creates a surface for rendering
- C++ ↔ JS bridge (CanvasKitBridge): load/is_ready/version; demo frame rendered and read back via readPixels
- Adapter skeleton in place to map Rive drawing to CanvasKit (polyline path prototype wired)

## Next steps

1) Replace demo drawing with real Rive shapes on Web
   - Extract simple paths (polylines/segments) from current draw flow
   - Use CK polyline pathway to stroke and display real outlines
2) Support fills and closed paths
   - Map fill rules, colors, and alpha to CanvasKit Paint/Path
3) Optional: images and text
   - Bridge images and text shaping when needed; start with minimal viable features
4) Performance
   - Switch to WebGL-backed surface once correctness is verified; keep readPixels for integration
5) Documentation
   - Grow this doc: add emscripten version notes, export caveats, and troubleshooting

## How to pick up next time

- Ensure Emscripten 3.1.43/3.1.44 is active in your shell/session
- Build:
  - cd build
  - scons platform=web target=template_debug -j8
- Export demo to Web in Godot, confirm CanvasKit loads and a frame is rendered
- Continue implementing Rive→CanvasKit mapping in src/web/* per plan above
