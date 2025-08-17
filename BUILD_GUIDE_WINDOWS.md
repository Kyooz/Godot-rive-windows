# Guia de Build - Godot Rive Extension para Windows

Este documento detalha o processo completo de compilação da extensão Godot Rive no Windows, executado com sucesso em 2024.

## ✅ Pré-requisitos Instalados

### 1. Visual Studio 2022 Community
- **Localização**: `C:\Program Files\Microsoft Visual Studio\2022\Community`
- **MSBuild**: `C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe`
- **Versão**: 17.12.0+b9d6fef4e
- **Componentes necessários**: Desenvolvimento em C++, Windows SDK

### 2. Python 3.11.5
- **Comando usado**: `python -V`
- **Resultado**: Python 3.11.5
- **Necessário para**: SCons e scripts de build

### 3. SCons 4.8.1
- **Comando usado**: `scons --version`
- **Resultado**: SCons by Steven Knight et al.
- **Necessário para**: Compilação da extensão Godot

### 4. Git 2.47.0
- **Comando usado**: `git --version`
- **Resultado**: git version 2.47.0.windows.2
- **Necessário para**: Clone do vcpkg e dependências

## ✅ Instalação do vcpkg e Skia

### 1. Verificação do vcpkg
```powershell
# Verificar se vcpkg já existe
dir C:\vcpkg
```
- **Status**: vcpkg já estava instalado em `C:\vcpkg`

### 2. Instalação do Skia
```powershell
cd C:\vcpkg
.\vcpkg install skia:x64-windows-static
```
- **Status**: Skia já estava instalado
- **Triplet**: x64-windows-static
- **Localização**: `C:\vcpkg\installed\x64-windows-static`

## ✅ Configuração de Variáveis de Ambiente

```powershell
$env:VCPKG_ROOT = "C:\vcpkg"
$env:VCPKG_TRIPLET = "x64-windows-static"
```

## ✅ Compilação das Bibliotecas Rive

### 1. Correção do Arquivo Premake
**Problema**: Flag `-Wno-atomic-alignment` não é reconhecida pelo MSVC
**Arquivo**: `thirdparty\rive-cpp\build\premake5.lua`

**Correção aplicada**:
```lua
-- ANTES (linha ~125):
filter({ 'files:../src/audio/audio_engine.cpp' })
do
    buildoptions({ '-Wno-atomic-alignment' })
end

-- DEPOIS:
filter({ 'files:../src/audio/audio_engine.cpp', 'options:not toolset=msc' })
do
    buildoptions({ '-Wno-atomic-alignment' })
end
```

### 2. Geração da Solução com Premake5
```powershell
.\thirdparty\premake\premake5.exe --file=thirdparty\rive-cpp\build\premake5.lua vs2022 --toolset=msc
```
- **Resultado**: Solução gerada em `thirdparty\rive-cpp\build\rive.sln`

### 3. Compilação com MSBuild

#### Biblioteca Principal Rive:
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" thirdparty\rive-cpp\build\rive.sln /t:rive /p:Configuration=Release /p:Platform=x64 /m
```
- **Resultado**: ✅ SUCESSO - 1 succeeded, 0 failed

#### Dependência HarfBuzz:
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_harfbuzz.vcxproj /p:Configuration=Release /p:Platform=x64 /m
```
- **Resultado**: ✅ SUCESSO - 1 succeeded, 0 failed

#### Dependência SheenBidi:
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" thirdparty\rive-cpp\dependencies\rive_sheenbidi.vcxproj /p:Configuration=Release /p:Platform=x64 /m
```
- **Resultado**: ✅ SUCESSO - 1 succeeded, 0 failed

## ✅ Criação do Arquivo SConstruct

Como o arquivo `SConstruct` principal não existia no projeto, foi necessário criá-lo baseado na estrutura do projeto e no exemplo do godot-cpp.

**Arquivo criado**: `SConstruct` (na raiz do projeto)

### Configurações principais:
- **Plataforma**: Windows x64
- **Target**: template_release
- **Compilação paralela**: 8 cores (-j8)

### Caminhos de include configurados:
- `src/` - Código fonte da extensão
- `src/api/` - API da extensão
- `src/utils/` - Utilitários
- `thirdparty/rive-cpp/include` - Headers do Rive
- `thirdparty/rive-cpp-legacy` - Rive legacy
- `thirdparty/rive-cpp-legacy/skia/renderer/include` - Headers do Skia renderer
- `dependencies/15B111469/harfbuzz-6.0.0/src` - HarfBuzz
- `dependencies/B223B0E94/SheenBidi-2.6/Headers` - SheenBidi
- `dependencies/228F558DD/miniaudio-rive_changes` - MiniaAudio
- `C:/vcpkg/installed/x64-windows-static/include` - Skia via vcpkg

### Bibliotecas linkadas:
- Rive core: `thirdparty/rive-cpp/build/bin/x64/Release/rive.lib`
- HarfBuzz: `thirdparty/rive-cpp/dependencies/windows/rive_harfbuzz/bin/x64/Release/rive_harfbuzz.lib`
- SheenBidi: `thirdparty/rive-cpp/dependencies/windows/rive_sheenbidi/bin/x64/Release/rive_sheenbidi.lib`
- Skia: `C:/vcpkg/installed/x64-windows-static/lib/skia.lib`

## ✅ Compilação da Extensão Godot

### Comando de build:
```powershell
scons platform=windows target=template_release -j8
```

### Resultado:
- **Status**: ✅ SUCESSO
- **Arquivo gerado**: `demo\bin\librive.windows.template_release.x86_64.dll`
- **Tamanho**: 4,277,248 bytes (4.18 MB)
- **Tempo de compilação**: Aproximadamente 2-3 minutos com 8 cores

### Arquivos de saída:
```
demo\bin\
├── librive.gdextension                           # Configuração da extensão
├── librive.windows.template_release.x86_64.dll   # DLL compilada ✅
├── librive.macos.template_debug.framework\       # Framework macOS (pré-existente)
└── librive.macos.template_release.framework\     # Framework macOS (pré-existente)
```

## 📋 Resumo do Processo Executado

1. ✅ **Verificação de pré-requisitos** - Visual Studio 2022, Python, SCons, Git
2. ✅ **Instalação do vcpkg e Skia** - Skia x64-windows-static via vcpkg
3. ✅ **Correção do arquivo premake** - Remoção de flag incompatível com MSVC
4. ✅ **Geração da solução Rive** - Premake5 com toolset MSVC
5. ✅ **Compilação das bibliotecas Rive** - MSBuild com Release x64
6. ✅ **Compilação das dependências** - HarfBuzz e SheenBidi
7. ✅ **Criação do SConstruct** - Arquivo de build principal para SCons
8. ✅ **Compilação da extensão** - SCons com 8 cores paralelos

## 🎯 Próximos Passos

### Para testar a extensão:
1. Instalar o Godot 4.1+ 
2. Abrir o projeto demo: `demo/project.godot`
3. Executar as cenas de exemplo para verificar funcionamento

### Arquivos importantes gerados:
- `librive.windows.template_release.x86_64.dll` - Extensão compilada
- `SConstruct` - Script de build criado
- Bibliotecas estáticas em `thirdparty/rive-cpp/build/bin/x64/Release/`

## 🔧 Comandos de Build Rápidos

Para recompilar apenas a extensão (após mudanças no código):
```powershell
scons platform=windows target=template_release -j8
```

Para recompilar as bibliotecas Rive (após mudanças no Rive):
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" thirdparty\rive-cpp\build\rive.sln /t:rive /p:Configuration=Release /p:Platform=x64 /m
```

Para limpar e recompilar tudo:
```powershell
scons platform=windows target=template_release -j8 -c  # Limpar
scons platform=windows target=template_release -j8     # Recompilar
```

---

**Build executado com sucesso em**: $(Get-Date -Format "dd/MM/yyyy HH:mm:ss")
**Ambiente**: Windows 10.0.22631, PowerShell, Visual Studio 2022 Community
**Resultado**: ✅ Extensão Godot Rive compilada com sucesso (4.18 MB)
