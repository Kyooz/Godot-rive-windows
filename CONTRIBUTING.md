## Contributing Guidelines (Essentials)

We keep this repository lean and reproducible. Please avoid vendoring large third-party trees or committing build outputs.

- Do NOT commit vcpkg artifacts
  - Use an external vcpkg installation and set VCPKG_ROOT (+ VCPKG_TRIPLET), or pass SCons args vcpkg_root=... vcpkg_triplet=...
  - See docs/windows-build.en.md (and the Chinese doc) for setup

- Do NOT commit full rive-cpp-legacy; only the Skia renderer source is tracked
  - thirdparty/rive-cpp-legacy/skia/renderer/include/**
  - thirdparty/rive-cpp-legacy/skia/renderer/src/**

- Do NOT commit build outputs
  - demo/bin/*.dll, *.lib, *.pdb
  - godot-cpp/bin/*, godot-cpp/**/*.obj|.lib|.exp|.pdb

- Submodules
  - We use upstream godot-cpp as a submodule; avoid changing its pointer unless intended
  - If you accidentally change it, restore via:
    - git submodule update --init --recursive

Thanks for contributing!

