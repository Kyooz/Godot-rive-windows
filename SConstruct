#!/usr/bin/env python

import os
import sys

# Add build folder to sys.path for SConscript.common
sys.path.append("build")
exec(open("build/SConscript.common").read())

# Import godot-cpp environment
env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Add source directories
env.Append(CPPPATH=["src/", "src/api/", "src/utils/"])

# Add Rive and dependencies include paths
env.Append(CPPPATH=[
    "thirdparty/rive-cpp/include",
    "thirdparty/rive-cpp-legacy",
    "thirdparty/rive-cpp-legacy/skia/renderer/include",
    "dependencies/15B111469/harfbuzz-6.0.0/src",
    "dependencies/B223B0E94/SheenBidi-2.6/Headers",
    "dependencies/228F558DD/miniaudio-rive_changes"
])

# Add vcpkg Skia include paths
if os.environ.get("VCPKG_ROOT"):
    vcpkg_root = os.environ.get("VCPKG_ROOT")
    vcpkg_triplet = os.environ.get("VCPKG_TRIPLET", "x64-windows-static")
    env.Append(CPPPATH=[
        os.path.join(vcpkg_root, "installed", vcpkg_triplet, "include"),
        os.path.join(vcpkg_root, "installed", vcpkg_triplet, "include", "skia")
    ])
    
    # Add vcpkg lib path
    env.Append(LIBPATH=[
        os.path.join(vcpkg_root, "installed", vcpkg_triplet, "lib")
    ])

# Add Rive lib paths
env.Append(LIBPATH=[
    "thirdparty/rive-cpp/build/windows/bin/release",
    "thirdparty/rive-cpp/dependencies/windows/cache/bin/release"
])

# Add Rive libraries
env.Append(LIBS=["rive", "rive_harfbuzz", "rive_sheenbidi"])

# Add Skia and dependencies libraries (vcpkg)
if os.environ.get("VCPKG_ROOT"):
    env.Append(LIBS=[
        "skia", "skparagraph", "skshaper", "skunicode_core", "skunicode_icu",
        "libpng16", "zlib", "jpeg", "turbojpeg",
        "libwebp", "libwebpdemux", "libwebpmux", "libsharpyuv",
        "brotlienc", "brotlidec", "brotlicommon",
        "libexpatMT", "freetype",
        "icudt", "icuin", "icuio", "icuuc"
    ])

# Add Windows system libraries
if env["platform"] == "windows":
    env.Append(LIBS=["user32", "gdi32", "winmm", "imm32", "ole32", "oleaut32", "version", "uuid", "advapi32"])

# Collect source files
sources = []
sources += Glob("src/*.cpp")
sources += Glob("src/api/*.cpp") 
sources += Glob("src/utils/*.cpp")
sources += Glob("thirdparty/rive-cpp-legacy/skia/renderer/src/*.cpp")

# Build the extension
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/librive.{}.{}.framework/librive.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/bin/librive.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/bin/librive.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/bin/librive{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

env.NoCache(library)
Default(library)
