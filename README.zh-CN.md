# Godot Rive（Windows 版）

面向 Godot 4.1+ 的 Rive 集成（GDExtension）。本仓库以精简可复现为目标：不内置 vcpkg 产物，仅保留最小的 Rive Legacy Skia 渲染器源码，其余依赖由使用者本地安装与编译。

> 提示
>
> 本项目参考了 macOS 方向的实现：https://github.com/kibble-cabal/godot-rive
> 上述仓库主要面向 macOS，并包含与本项目不同的依赖获取和构建路径。本文档聚焦 Windows 构建；如需 macOS 构建，请参考该上游仓库或自行适配。

## 目录

- [项目策略](#项目策略)
- [功能](#功能)
- [构建](#构建)
- [安装](#安装)
- [致谢与参考](#致谢与参考)

## 项目策略

- 不内置 vcpkg 包
  - 使用外部安装的 vcpkg，并通过 VCPKG_ROOT（与 VCPKG_TRIPLET）或 SCons 参数 vcpkg_root=... vcpkg_triplet=... 告知路径
  - 详见 docs/windows-build.md
- 仅保留 Rive Legacy Skia 渲染器的最小源码
  - thirdparty/rive-cpp-legacy/skia/renderer/include/**
  - thirdparty/rive-cpp-legacy/skia/renderer/src/**
- 忽略构建产物
  - godot-cpp/bin、demo/bin、以及 *.obj/*.lib/*.exp/*.pdb 等
- 子模块
  - 使用上游 godot-cpp 子模块；请勿随意修改其指针。如有偏移，可用 git submodule update --init --recursive 恢复

## 功能

- 加载 .riv 文件（画板、动画、状态机）
- 输入事件（hover、pressed 等）
- 在编辑器和代码中修改状态机属性
- 运行时交互 API
- 针对 Godot 优化

## 构建

- Windows 用户：请阅读 docs/windows-build.md
- macOS 用户：本仓库聚焦 Windows。macOS 可参考上游仓库 https://github.com/kibble-cabal/godot-rive 或按需适配本仓库脚本（build/build.py、build/SConstruct）

必备工具：

- Python 3
- git
- scons
- Visual Studio 2022（MSBuild, Windows SDK）
- vcpkg（外部安装，使用 x64-windows-static triplet）

快速开始（仅演示主构建脚本的帮助信息；实际流程请遵循 Windows 文档）：

```bash
cd build
python build.py --help
```

完整 Windows 步骤请见 docs/windows-build.md（包含：用 premake 生成 rive-cpp VS 工程 → 用 MSBuild 构建 rive.lib 与文本依赖 → 用 SCons 构建 GDExtension 并链接 vcpkg Skia）。

## 安装

> 说明
>
> 目前仅提供 macOS universal（debug/release）的预编译；其他平台请按照文档在本地构建。

1. 复制 demo/bin、demo/icons 与 demo/rive.gdextension 到你的 Godot 工程
2. 根据你的路径修改 rive.gdextension 中的库位置

## 致谢与参考

- 上游参考（macOS 方向）：https://github.com/kibble-cabal/godot-rive
- Rive 核心库：https://github.com/rive-app/rive-cpp
- Skia 图形库：https://github.com/google/skia

本仓库与上游主要差异：
- 平台重心：本仓库聚焦 Windows；上游聚焦 macOS
- 依赖管理：本仓库使用外部 vcpkg（不内置），上游走另一套依赖/构建路径（如 GN/Ninja）
- 渲染器接入：本仓库内联编译 Rive Legacy Skia 渲染器源码（与当前 rive-cpp API 适配）

