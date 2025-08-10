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

