# Godot Rive

An integration of Rive into Godot 4.1+ using GDExtension.

> Warning
>
> This extension is in alpha:
> - You may encounter some bugs
> - It's untested on many platforms
> - Most features are implemented, but the API may change a little

This extensions adds [Rive](https://rive.app) support to Godot 4.

It makes use of the following third-party libraries:

- [`rive-cpp`](https://github.com/rive-app/rive-cpp)

> Note
>
> This repository is focused on Windows. It takes inspiration from a macOS-oriented project: [kibble-cabal/godot-rive](https://github.com/kibble-cabal/godot-rive). That upstream uses a different dependency/build path (e.g., GN/Ninja on macOS). For Windows users, follow the docs in this repo; for macOS, consider the upstream project or adapt our scripts.

- [`skia`](https://github.com/google/skia) (included in `rive-cpp`)

## Table of Contents

- [Godot Rive](#godot-rive)
  - [Table of Contents](#table-of-contents)
  - [Lean repository policy](#lean-repository-policy)
  - [Features](#features)
  - [Building](#building)
  - [Installation](#installation)
  - [Roadmap](#roadmap)
  - [Contributing](#contributing)
  - [Screenshots](#screenshots)

## Lean repository policy

- No vendored vcpkg packages. Use an external vcpkg installation and set VCPKG_ROOT (+ VCPKG_TRIPLET), or pass SCons args vcpkg_root=... vcpkg_triplet=...
  - See docs/windows-build.md (CN) or docs/windows-build.en.md (EN)
- Only the minimal legacy Skia renderer sources are tracked:
  - thirdparty/rive-cpp-legacy/skia/renderer/include/**
  - thirdparty/rive-cpp-legacy/skia/renderer/src/**
- Build outputs are ignored (godot-cpp/bin, demo/bin, *.obj/*.lib/*.exp/*.pdb)
- Submodules: we use upstream godot-cpp; avoid changing its pointer; restore via `git submodule update --init --recursive` if needed

## Features

- Load `.riv` files (artboards, animations, and state machines)
- Listen for input events
- Change state machine properties in-editor and in code
- Robust API for runtime interaction
- Optimized for Godot

## Building

- Windows users: see docs/windows-build.md (CN) or docs/windows-build.en.md (EN)
- macOS users: originally tested on Apple Silicon; you may need to adapt `build/build.py` or `build/SConstruct`

Required tools:

- Python 3
- [git](https://git-scm.com/)
- [scons](https://scons.org/)
- [ninja](https://ninja-build.org/)

Quick start (generic):

```bash
cd build
python build.py
```

Help:

```bash
python build.py --help
```

## Installation

> Note
>
> Prebuilt binaries are currently only provided for macOS universal (debug and release). For other platforms, please build locally following the docs.

1. Copy `demo/bin/`, `demo/icons/`, and `demo/rive.gdextension` to your project folder
2. Update the paths in `rive.gdextension` to match your project folder structure


## Roadmap
- [x] Load `.riv` files
- [x] Run and play Rive animations
- [x] Raster image support
- [x] Input events (hover, pressed, etc.)
- [x] Alignment & size exported properties
- [x] Multiple scenes/artboards
- [x] Dynamic exported properties based on state machine
- [x] API for interaction during runtime
- [x] Add error handling
- [x] Add signals for event listeners (hover, pressed, etc)
- [x] Disable/enable event listeners (hover, pressed, etc) in API and editor
- [x] Optimization
- [x] Static editor preview
- [x] Animated editor preview
- [ ] Add reset button
- [ ] `.riv` ResourceLoader (thumbnails)
- [ ] Other platform support
- [ ] Any missing features

## Contributing

Help would be MUCH appreciated testing and/or building for the following platforms:

- Windows
- Android
- iOS
- Linux
- Web

Feel free to contribute bug fixes (see open issues), documentation, or features as well.

## Screenshots

![In-editor screenshot](screenshots/screenshot_1.png)