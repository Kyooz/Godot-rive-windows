## Rive Godot Extension — Windows Build Guide

This document explains how to build the Windows binary of this extension using:
- Visual Studio 2022 (MSBuild)
- premake5 (to generate the rive-cpp solution)
- vcpkg-provided Skia (static), plus its codec/font dependencies
- SCons (to build the Godot extension DLL)

Tested on Windows 10/11 x64, VS 2022 Community.

---

### Prerequisites

- Visual Studio 2022 Build Tools or Community with C++ workload:
  - Desktop development with C++
  - MSBuild, Windows SDK
  - Path to MSBuild (example):
    - "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"
- Python 3.x
- SCons (installed via pip recommended):
  - pip install scons
- vcpkg (Skia + dependencies) vendored in this repo at:
  - thirdparty\\vcpkg
  - Triplet used: x64-windows-static
- premake5 (already available via thirdparty\\premake, or use helper script below)

Optional:
- If thirdparty\\premake is missing, run:
  - thirdparty\\rive-cpp\\dependencies\\windows\\get_premake5.bat

---

### Directory layout (relevant parts)

- thirdparty\\rive-cpp
  - include, src (Rive core)
  - build\\premake5.lua (solution generator)
  - dependencies\\(harfbuzz, sheenbidi project files and helpers)
- thirdparty\\vcpkg
  - installed\\x64-windows-static\\{include, lib} (Skia and dependencies)
- build\\SConstruct (extension build script)
- demo\\bin (build output target for the DLL)

---

### 1) Generate Rive solution with premake

Run premake to create a VS 2022 solution targeting MSVC (avoid ClangCL toolset issues):

- From repo root (PowerShell or cmd):

  - Using vendored premake:
    - cmd
      - thirdparty\premake\premake5.exe --file=thirdparty\rive-cpp\build\premake5.lua vs2022 --toolset=msc

  - Or using helper script to fetch premake then run:
    - cmd
      - thirdparty\rive-cpp\dependencies\windows\get_premake5.bat
      - thirdparty\premake\premake5.exe --file=thirdparty\rive-cpp\build\premake5.lua vs2022 --toolset=msc

This generates:
- thirdparty\rive-cpp\build\rive.sln
- thirdparty\rive-cpp\build\rive.vcxproj
- thirdparty\rive-cpp\dependencies\rive_harfbuzz.vcxproj
- thirdparty\rive-cpp\dependencies\rive_sheenbidi.vcxproj

---

### 2) Build Rive static libs with MSBuild

Use the VS 2022 Developer Command Prompt or provide the full MSBuild path.

- Build rive core (Release|x64):
  - "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\build\rive.sln /t:rive /p:Configuration=Release /p:Platform=x64 /m

- Build text deps (Release|x64):
  - "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_harfbuzz.vcxproj /p:Configuration=Release /p:Platform=x64 /m
  - "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_sheenbidi.vcxproj /p:Configuration=Release /p:Platform=x64 /m

Expected artifacts:
- thirdparty\rive-cpp\build\windows\bin\release\rive.lib
- thirdparty\rive-cpp\dependencies\windows\cache\bin\release\rive_harfbuzz.lib
- thirdparty\rive-cpp\dependencies\windows\cache\bin\release\rive_sheenbidi.lib

---

### 3) Build the Godot extension DLL with SCons

The SCons script is configured to prefer vcpkg Skia when available under thirdparty\vcpkg.

- From repo root:
  - cd build
  - scons platform=windows target=template_release -j4

Notes:
- If you maintain your own vcpkg path, you can set environment variables before running scons:
  - set VCPKG_ROOT=C:\\path\\to\\vcpkg
  - set VCPKG_TRIPLET=x64-windows-static
- The script auto-detects common Skia codec/font dependencies (png, jpeg, webp, zlib, brotli, freetype, ICU) and links them if present in vcpkg.

Output:
- demo\bin\librive.windows.template_release.x86_64.dll

To build a debug variant:
- scons platform=windows target=template_debug -j4

---

### 4) Run the demo

- Open demo\project.godot in Godot
- Add a RiveViewer node to a scene
- Load a .riv file and run — the extension renders via Skia

---

### Troubleshooting

- MSBuild not found
  - Install Visual Studio 2022 Build Tools or Community and use the Developer Command Prompt.
  - Or invoke MSBuild via full path (see examples above).

- Error: ClangCL toolset not found (MSB8020)
  - Generate the solution with MSVC toolset: add --toolset=msc to premake command.

- Link errors for png/jpeg/webp/icu when linking DLL
  - Ensure vcpkg triplet is x64-windows-static, and the following libraries exist under thirdparty\vcpkg\installed\x64-windows-static\lib:
    - skia.lib, skparagraph.lib, skshaper.lib, skunicode_core.lib, skunicode_icu.lib
    - libpng16.lib, zlib.lib, jpeg.lib, turbojpeg.lib
    - libwebp*.lib, brotli*.lib, libexpatMT.lib, freetype.lib
    - icu*.lib (icudt, icuin, icuio, icuuc)

- Skia headers not found (include/core/Sk*.h)
  - Ensure SCons picks vcpkg include paths:
    - thirdparty\vcpkg\installed\x64-windows-static\include
    - thirdparty\vcpkg\installed\x64-windows-static\include\skia

- Clean builds
  - rive-cpp (git clean):
    - git -C thirdparty/rive-cpp clean -fdx && git -C thirdparty/rive-cpp reset --hard HEAD
  - Re-run premake and MSBuild steps
  - Re-run SCons

---

### Notes

- The Skia renderer is compiled from source (legacy-compatible) and linked directly into the extension; no separate renderer library is required.
- The project uses static runtime (/MT) to match Skia from vcpkg x64-windows-static.
- The instructions reflect the current repo state and SCons logic (auto-detect Skia via thirdparty\\vcpkg and auto-link common dependencies).

## 1. 环境准备

- 基础工具
  - Git（建议安装 Git for Windows，并启用 Git Bash）
  - Python 3.8+
  - Ninja（可用 `choco install ninja` 或下载官方 zip 放入 PATH）
  - CMake（部分依赖会用到）
  - Visual Studio 2022 或 Build Tools（MSVC、Windows SDK、"使用 C++ 的桌面开发" 工作负载）
  - SCons（`pip install scons`）
- 可选：MSYS2 或 Git Bash（用于运行部分 shell 脚本）

> 注意：Skia 官方使用 GN/Ninja 构建。Windows 下建议使用 GN/Ninja 编译出 Skia 静态库，再进行后续链接。

## 2. 获取源码

- 克隆仓库（确保包含第三方子目录）：
  - `git clone <your-repo-url>`
  - `cd godot-rive`

> 若此前子模块未初始化，可手动执行 `git submodule update --init --recursive`（如需要）。

## 3. 准备 Skia（Windows/x64）

有两种可选方案：

A) 使用 vcpkg 安装 Skia（推荐，配置简单）

1) 安装 vcpkg 并设置环境变量（或在命令行提供参数）：
   - VCPKG_ROOT 指向 vcpkg 根目录
   - 可选：VCPKG_TRIPLET（默认 x64-windows-static）

2) 安装 Skia（静态库示例）：
   ```bat
   %VCPKG_ROOT%\vcpkg install skia[harfbuzz,icu,svg]:x64-windows-static
   ```
   - 如需动态库：使用 x64-windows

3) 使用 scons 构建时，自动从 vcpkg 读取 Skia：
   - 方式一：通过环境变量
     ```bat
     set VCPKG_ROOT=C:\path\to\vcpkg
     set VCPKG_TRIPLET=x64-windows-static
     ```
   - 方式二：通过参数传入
     ```bat
     scons platform=windows target=template_release vcpkg_root=C:\path\to\vcpkg vcpkg_triplet=x64-windows-static
     ```

B) 使用 GN/Ninja 手工构建 Skia（备选）

1) 进入 Skia 源码目录：
   - `cd thirdparty/rive-cpp/skia/dependencies/skia`

2) 同步依赖（需要网络）：
   - `python tools/git-sync-deps`
   - 此步骤会下载 GN、依赖代码等

3) 生成构建配置（GN）：
   - 打开 "x64 Native Tools Command Prompt for VS 2022" 或在有 cl.exe 的环境中执行：
   - 运行：
     ```bat
     bin\gn gen out\Static --args="is_official_build=true is_component_build=false target_os=\"win\" target_cpu=\"x64\" skia_use_gl=false skia_use_d3d=true"
     ```
   - 说明：
     - target_os="win" target_cpu="x64"
     - 选择渲染后端：`skia_use_d3d=true`（Direct3D）或 `skia_use_gl=true`（OpenGL），按需二选一或都开（可能增加编译体积与时间）

4) 使用 Ninja 编译：
   - `ninja -C out\Static skia`
   - 成功后会在 `out\Static\` 生成 `skia` 静态库（libskia.lib 或等效产物）

> 构建耗时较长，请耐心等待。若需要清理全编，删除 `thirdparty/rive-cpp/skia/dependencies/skia/out/` 后重来。

## 4. 编译 rive-cpp 与 Skia 渲染器

- 在 Windows 上，`thirdparty/rive-cpp/build.sh` 是 shell 脚本，建议在 Git Bash 中运行；或者直接用 Premake 生成 VS 解决方案并使用 MSVC 构建。

A. 使用 Git Bash + make（若环境已具备）
- 在仓库根目录打开 Git Bash：
  ```bash
  cd thirdparty/rive-cpp
  ./build.sh -p windows release   # 或 debug
  cd skia/renderer
  ./build.sh -p windows release   # 或 debug
  ```
- 产物会生成到 `thirdparty/rive-cpp/build/windows/bin/<config>/` 与 `thirdparty/rive-cpp/skia/renderer/build/windows/bin/<config>/`

B. 使用 Premake 生成 VS 工程（可选）
- 在 `thirdparty/rive-cpp/build` 下使用 premake5 生成 VS 解决方案，然后在 VS 中构建（确保工具集与运行库与 Skia 一致，静态运行库 /MT）。

> 提示：Windows 下 build/setup_compiler.lua 已为 MSVC 做了部分选项适配（/MT）。

## 5. 构建 Godot 扩展（GDExtension）

### 快速构建方法（推荐）

由于依赖库已预构建，可以直接构建扩展：

1) 首先构建 godot-cpp（如果尚未构建）：
   ```bat
   cd godot-cpp
   scons platform=windows target=template_release
   ```

2) 构建 Rive 扩展：
   ```bat
   cd build
   scons platform=windows target=template_release
   ```

3) 成功后产物：
   - 扩展动态库：`demo\bin\librive.windows.template_release.x86_64.dll`
   - 扩展描述：`demo\bin\librive.gdextension`（需要手动添加 Windows 条目）

### 完整构建方法

如果需要重新构建所有依赖：

1) 确保 Skia 静态库已编译完成并位于 `thirdparty/rive-cpp/skia/dependencies/skia/out/Static/`（Windows 版本）。

2) 在仓库根目录打开命令行：
   ```bat
   cd build
   python build.py --platform=windows --target=release -j8
   ```
   - 如需 Debug：`--target=debug`

> 注意：build.py 不会自动编译 Skia 本体（只检查目录存在），所以第 3 节需要先完成。

## 6. 常见问题与排查

- 链接错误：未能找到 `skia`
  - 确认 Skia 已为 Windows/x64 成功编译，并且 `build/SConstruct` 中的 `LIBPATH` 指向 `.../skia/dependencies/skia/out/Static`
- 运行库不匹配（/MT vs /MD）
  - Skia 与 rive-cpp/renderer、扩展需统一运行库（建议 /MT）。`setup_compiler.lua` 已对 MSVC 设置了 staticruntime=on。
- 平台/架构不匹配
  - Windows 下统一 x64；若需要 x86 或其他 ABI，需相应调整 GN args 与构建链路。
- 清理重编
  - Skia：删除 `thirdparty/rive-cpp/skia/dependencies/skia/out/`
  - rive：删除 `thirdparty/rive-cpp/build/windows/` 与 `thirdparty/rive-cpp/dependencies/windows/cache/`
  - 扩展：`cd build && scons -c`

## 7. 验证

- 在 Windows 上用 Godot 4.5 打开 demo 工程（demo\project.godot），运行查看动画是否正常。
- 若要在你的工程使用，参考 README 的安装步骤复制二进制与 gdextension 文件。

### 更新 gdextension 文件

构建完成后，需要手动更新 `demo/bin/librive.gdextension` 文件以包含 Windows 库：

```ini
[configuration]

entry_symbol = "rive_library_init"
compatibility_minimum = "4.1"

[libraries]

macos.debug = "res://bin/librive.macos.template_debug.framework"
macos.release = "res://bin/librive.macos.template_release.framework"
windows.debug = "res://bin/librive.windows.template_debug.x86_64.dll"
windows.release = "res://bin/librive.windows.template_release.x86_64.dll"
```

## 8. 附录：GN args 示例

- 最小 Direct3D 示例：
  ```
  is_official_build=true
  is_component_build=false
  target_os="win"
  target_cpu="x64"
  skia_use_gl=false
  skia_use_d3d=true
  ```
- OpenGL 示例：
  ```
  is_official_build=true
  is_component_build=false
  target_os="win"
  target_cpu="x64"
  skia_use_gl=true
  skia_use_d3d=false
  ```

