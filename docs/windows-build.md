# Godot Rive - Windows 构建指南

本指南记录如何在 Windows 上从源码编译本项目（Rive + Skia + Godot GDExtension）。建议在一台联网良好的 Windows 10/11 x64 机器上进行。

> 说明：本仓库默认脚本主要在 macOS 上测试。Windows 下需要手动准备 Skia，并在 Windows 环境中运行 premake/make 或 MSVC 工具链。

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

1) 确保 Skia 静态库已编译完成并位于 `thirdparty/rive-cpp/skia/dependencies/skia/out/Static/`（Windows 版本）。

2) 在仓库根目录打开命令行：
   ```bat
   cd build
   python build.py --platform=windows --target=release -j8
   ```
   - 如需 Debug：`--target=debug`

3) 成功后产物：
   - 扩展动态库：`demo\bin\librive.windows.template_<config>.*`（具体后缀视 scons 配置与平台而定）
   - 扩展描述：`demo\bin\librive.gdextension`

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

