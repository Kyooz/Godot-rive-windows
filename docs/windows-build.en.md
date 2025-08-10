# Rive Godot Extension — Windows Build Guide (English)

This guide describes how to build the Windows binary of this extension using:

- Visual Studio 2022 (MSBuild)
- premake5 (to generate the rive-cpp solution)
- vcpkg-provided Skia (static), plus its codec/font dependencies
- SCons (to build the Godot extension DLL)

Tested on Windows 10/11 x64, VS 2022 Community.

---

## Prerequisites

- Visual Studio 2022 Build Tools or Community with C++ workload:
  - Desktop development with C++
  - MSBuild, Windows SDK
  - Path to MSBuild (example):
    - "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"

- Python 3.x

- SCons (recommended install via pip):
  - pip install scons

- vcpkg (Skia + dependencies) vendored in this repo:
  - thirdparty\\vcpkg
  - Triplet: x64-windows-static

- premake5 (vendored in thirdparty\\premake; if missing, see below)

Optional:

- If thirdparty\\premake is missing, run:
  - thirdparty\\rive-cpp\\dependencies\\windows\\get_premake5.bat

---

## Directory layout (relevant parts)

- thirdparty\\rive-cpp
  - include, src (Rive core)
  - build\\premake5.lua (solution generator)
  - dependencies\\ (harfbuzz, sheenbidi project files and helpers)

- thirdparty\\vcpkg
  - installed\\x64-windows-static\\{include, lib} (Skia and dependencies)

- build\\SConstruct (extension build script)

- demo\\bin (build output target for the DLL)

---

## 1) Generate Rive solution with premake

Run premake to create a VS 2022 solution using the MSVC toolset (avoids ClangCL toolset issues):

- From repo root (PowerShell or cmd):

  - Using vendored premake (cmd):

    ```bat
    thirdparty\premake\premake5.exe --file=thirdparty\rive-cpp\build\premake5.lua vs2022 --toolset=msc
    ```

  - Or fetch premake via helper script, then run (cmd):

    ```bat
    thirdparty\rive-cpp\dependencies\windows\get_premake5.bat
    thirdparty\premake\premake5.exe --file=thirdparty\rive-cpp\build\premake5.lua vs2022 --toolset=msc
    ```

This generates:

- thirdparty\rive-cpp\build\rive.sln
- thirdparty\rive-cpp\build\rive.vcxproj
- thirdparty\rive-cpp\dependencies\rive_harfbuzz.vcxproj
- thirdparty\rive-cpp\dependencies\rive_sheenbidi.vcxproj

---

## 2) Build Rive static libs with MSBuild

Use the VS 2022 Developer Command Prompt or invoke MSBuild with a full path.

- Build rive core (Release|x64):

  ```bat
  "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\build\rive.sln /t:rive /p:Configuration=Release /p:Platform=x64 /m
  ```

- Build text dependencies (Release|x64):

  ```bat
  "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_harfbuzz.vcxproj /p:Configuration=Release /p:Platform=x64 /m
  "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_sheenbidi.vcxproj /p:Configuration=Release /p:Platform=x64 /m
  ```

Expected artifacts:

- thirdparty\rive-cpp\build\windows\bin\release\rive.lib
- thirdparty\rive-cpp\dependencies\windows\cache\bin\release\rive_harfbuzz.lib
- thirdparty\rive-cpp\dependencies\windows\cache\bin\release\rive_sheenbidi.lib

---

## 3) Build the Godot extension DLL with SCons

The SCons script prefers vcpkg Skia when available under thirdparty\vcpkg and auto-links common Skia dependencies.

- From repo root:

  ```bat
  cd build
  scons platform=windows target=template_release -j4
  ```

Notes:

- If you use your own vcpkg installation, set environment variables before running SCons:

  ```bat
  set VCPKG_ROOT=C:\path\to\vcpkg
  set VCPKG_TRIPLET=x64-windows-static
  ```

- The script auto-detects and links png, jpeg, webp, zlib, brotli, freetype, ICU when present in vcpkg.

Output:

- demo\bin\librive.windows.template_release.x86_64.dll

To build a debug variant:

```bat
scons platform=windows target=template_debug -j4
```

---

## 4) Run the demo

- Open demo\project.godot in Godot.
- Add a RiveViewer node to a scene.
- Load a .riv file and run — the extension renders via Skia.

---

## Troubleshooting

- MSBuild not found
  - Install Visual Studio 2022 Build Tools or Community and use the Developer Command Prompt.
  - Or invoke MSBuild via full path (see examples above).

- Error: ClangCL toolset not found (MSB8020)
  - Generate the solution with MSVC toolset: add `--toolset=msc` to the premake command.

- Link errors for png/jpeg/webp/icu when linking the DLL
  - Ensure triplet is `x64-windows-static`, and the following libraries exist under `thirdparty\vcpkg\installed\x64-windows-static\lib`:
    - `skia.lib`, `skparagraph.lib`, `skshaper.lib`, `skunicode_core.lib`, `skunicode_icu.lib`
    - `libpng16.lib`, `zlib.lib`, `jpeg.lib`, `turbojpeg.lib`
    - `libwebp*.lib`, `brotli*.lib`, `libexpatMT.lib`, `freetype.lib`
    - ICU: `icudt.lib`, `icuin.lib`, `icuio.lib`, `icuuc.lib`

- Skia headers not found (`include/core/Sk*.h`)
  - Ensure SCons includes these paths:
    - `thirdparty\vcpkg\installed\x64-windows-static\include`
    - `thirdparty\vcpkg\installed\x64-windows-static\include\skia`

- Clean builds
  - rive-cpp:

    ```bat
    git -C thirdparty/rive-cpp clean -fdx
    git -C thirdparty/rive-cpp reset --hard HEAD
    ```

  - Re-run premake and MSBuild
  - Re-run SCons

---

## Notes

- The Skia renderer is compiled from source (legacy-compatible) and linked directly into the extension; no separate renderer library is required.
- The project uses static runtime (/MT) to match Skia from vcpkg `x64-windows-static`.
- These instructions reflect the current repo state and SCons logic (auto-detect Skia via `thirdparty\vcpkg` and auto-link common dependencies).

