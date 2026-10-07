# Nabil App Setup Guide

This guide covers setting up your local development environment and configuring/building the project using CMake presets. On **Windows** dependencies come from `vcpkg`; on **macOS** they come from Homebrew (`brew`); on **Linux** they can come from Nix or the distribution package manager.

> **Storage & Time Warning (Windows/vcpkg only):**  
> The `vcpkg` path needs at least **100 GB of free disk space**, and the initial dependency build can take several hours. The macOS/Homebrew path is much lighter: it uses prebuilt bottles plus one ~10 minute source build of OCCT.

---

## Linux Setup

Linux builds use GCC or Clang, CMake, Ninja, Qt 6, VTK, and OpenCASCADE. The Nix flake is the recommended option because it provides the required OpenCASCADE 8.0.1 build and keeps the dependency versions consistent across Linux distributions. Native distribution packages can also be used, but some repositories provide an older OpenCASCADE release.

### NixOS or Nix

NixOS users can use the repository's flake directly. The same instructions work on other Linux distributions with the Nix package manager installed.

1. **Install Nix** using the [official installation instructions](https://nixos.org/download/), if it is not already installed.
2. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```
3. **Enter the development shell:**
   ```bash
   nix develop
   ```

   This supplies the compiler, CMake, Ninja, Qt 6, VTK, OpenCASCADE 8.0.1, and the Linux graphics dependencies. Keep this shell active while configuring, building, and running the application.

4. **Configure and build:**
   ```bash
   cmake --preset linux-release
   cmake --build --preset linux-release
   ```

   Use `linux-debug` instead of `linux-release` for a debug build.

5. **Run:**
   ```bash
   ./build/linux-release/coco
   ```

### Native Distribution Packages

Install the packages for your distribution before configuring the project. The package names below cover the compiler, build tools, Qt 6, VTK with Qt/OpenGL support, OpenCASCADE, and the Linux graphics libraries used by the application.

| Distribution | Install command |
| --- | --- |
| Ubuntu/Debian | `sudo apt install build-essential cmake ninja-build pkg-config qt6-base-dev libvtk9-dev libvtk9-qt-dev libocct-foundation-dev libocct-modeling-algorithms-dev libocct-modeling-data-dev libocct-visualization-dev libfreetype-dev libgl-dev libxkbcommon-dev libx11-dev` |
| Fedora/RHEL | `sudo dnf install gcc-c++ cmake ninja-build pkgconf-pkg-config qt6-qtbase-devel vtk-devel opencascade-devel freetype-devel mesa-libGL-devel libxkbcommon-devel libX11-devel` |
| Arch/Manjaro | `sudo pacman -S --needed base-devel cmake ninja pkgconf qt6-base vtk opencascade freetype2 libglvnd libxkbcommon libx11` |

Package names and OpenCASCADE versions vary between releases. If your distribution does not provide OpenCASCADE 8.0.1, use the Nix setup above or build/install OpenCASCADE 8.0.1 separately and set `CMAKE_PREFIX_PATH` to its installation prefix.

> **Disclaimer:** The native Linux build has not been tested on Ubuntu or Fedora. If you run into any issues with the Linux setup, please contact Nabil directly.

After installing the native packages:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```
2. **Configure and build:**
   ```bash
   cmake --preset linux-release
   cmake --build --preset linux-release
   ```
3. **Run:**
   ```bash
   ./build/linux-release/coco
   ```

If CMake cannot locate a dependency installed outside the system prefix, configure with a prefix path, for example:

```bash
cmake --preset linux-release -DCMAKE_PREFIX_PATH=/path/to/dependencies
```

---

## Windows Setup

### Prerequisites

* **Microsoft Visual Studio 2022** (Ensure the *Desktop development with C++* workload is installed)
* **VS Code** with the following extensions:
  * C/C++ (`ms-vscode.cpptools`)
  * CMake Tools (`ms-vscode.cmake-tools`)
* **CMake** ($\ge 3.26$)
* **Ninja**
* **vcpkg**

---

### Tooling Installation (Footnotes)

#### 1. Install Ninja
1. Download `ninja-win.zip` from the [Ninja Releases page](https://github.com/ninja-build/ninja/releases).
2. Extract `ninja.exe` into a persistent directory (e.g., `C:\Tools\`).
3. Add that directory (`C:\Tools\`) to your system **Path** environment variable.
4. Verify in a terminal:
   ```cmd
   ninja --version
   ```

#### 2. Install vcpkg
1. Choose an installation path (denoted below as `{install_path}`, e.g., `C:\`).
2. Clone the repository and run the bootstrap script:
   ```cmd
   cd {install_path}
   git clone https://github.com/microsoft/vcpkg.git VCPKG
   cd VCPKG
   .\bootstrap-vcpkg.bat
   ```
3. Set your environment variables:
   * Add `{install_path}\VCPKG` to your system **Path**.
   * Add a new environment variable: `VCPKG_ROOT` = `{install_path}\VCPKG`.
4. Verify in a terminal:
   ```cmd
   vcpkg --version
   ```

---

### Step-by-Step Setup

1. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```

2. **Create `CMakeUserPresets.json`:**  
   In the project root, create a file named `CMakeUserPresets.json`. Update the `CMAKE_TOOLCHAIN_FILE` paths to match your local `vcpkg` directory:

   ```json
   {
     "version": 3,
     "cmakeMinimumRequired": {
       "major": 3,
       "minor": 26,
       "patch": 0
     },
     "configurePresets": [
       {
         "name": "release-local",
         "inherits": "ninja-vcpkg-release",
         "description": "Local override for toolchain path",
         "hidden": false,
         "cacheVariables": {
           "CMAKE_TOOLCHAIN_FILE": "C:/VCPKG/scripts/buildsystems/vcpkg.cmake",
           "VCPKG_TARGET_TRIPLET": "x64-windows"
         }
       },
       {
         "name": "debug-local",
         "inherits": "ninja-vcpkg-debug",
         "description": "Debug local version",
         "hidden": false,
         "cacheVariables": {
           "CMAKE_TOOLCHAIN_FILE": "C:/VCPKG/scripts/buildsystems/vcpkg.cmake",
           "VCPKG_TARGET_TRIPLET": "x64-windows"
         }
       }
     ]
   }
   ```

3. **Configure the project:**  
   Run the configure preset in your terminal. This triggers package downloads and compilations via `vcpkg`:
   ```bash
   cmake --preset release-local
   ```

4. **Build:**
   ```bash
   cmake --build --preset build-release
   ```

5. **Run:**  
   Execute the compiled binary from the build directory:
   ```cmd
   .\build\release\coco.exe
   ```

---

### Windows Troubleshooting

If CMake behaves unexpectedly or fails to detect compilers:
1. Close VS Code completely.
2. Delete the `build/` directory in the project root.
3. Open the **x64 Native Tools Command Prompt for VS 2022** from your Start Menu.
4. Navigate to the project root and launch VS Code:
   ```cmd
   cd path\to\nabil-app
   code .
   ```
5. Allow the CMake Tools extension to scan and configure the kit.

---

## macOS Setup

macOS builds use **Homebrew** for all dependencies (Qt6, VTK, OCCT) — no `vcpkg` required. The brew-based presets are committed in `CMakePresets.json` (`macos-brew-release` / `macos-brew-debug`), so no local preset file is needed.

### Prerequisites

* **Xcode**: Full installation from the App Store.
* **Homebrew**: Installed via [brew.sh](https://brew.sh/).
* At least **10 GB** of available disk space.

---

### Step-by-Step Setup

1. **Configure developer tools and license:**
   ```bash
   sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
   sudo xcodebuild -license accept
   ```

2. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```

3. **Install build tools and dependencies:**
   ```bash
   brew install cmake ninja qt vtk
   ```

4. **Install OCCT 8.0.1 via brew:**

   Homebrew's stock `opencascade` formula still ships 7.9.3, which is too old for this project. This repo vendors the 8.0.1 formula at `packaging/homebrew/opencascade.rb`. Homebrew only installs formulas that live in a tap, so create a local tap and copy it in (run from the repo root):

   ```bash
   brew tap-new nabil/occt
   cp packaging/homebrew/opencascade.rb "$(brew --repository nabil/occt)/Formula/"
   brew install --build-from-source nabil/occt/opencascade
   ```

   The source build takes roughly 10 minutes on Apple Silicon.

   > **Note:** If a 7.x `opencascade` is already installed, brew refuses to install the tap formula under the same name. Remove the old one first:
   > ```bash
   > brew unpin opencascade 2>/dev/null; brew uninstall opencascade
   > ```
   > Once homebrew-core ships 8.0.1 you can `brew untap nabil/occt` and use the stock formula instead.

5. **Configure:**
   ```bash
   cmake --preset macos-brew-release
   ```
   For a debug build use `macos-brew-debug`. Both presets point CMake at `/opt/homebrew/opt/{opencascade,qtbase,vtk}`.

6. **Build:**
   ```bash
   cmake --build --preset macos-brew-build-release
   ```

7. **Run:**
   ```bash
   open build/brew-release/coco.app
   ```
   (or run the binary directly: `./build/brew-release/coco.app/Contents/MacOS/coco`)
