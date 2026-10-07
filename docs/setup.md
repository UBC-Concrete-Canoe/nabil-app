# Nabil App Setup Guide

This guide covers setting up your local development environment and configuring/building the project using CMake presets. On **Windows** dependencies come from `vcpkg`; on **macOS** they come from Homebrew (`brew`).

> **Storage & Time Warning (Windows/vcpkg only):**  
> The `vcpkg` path needs at least **100 GB of free disk space**, and the initial dependency build can take several hours. The macOS/Homebrew path is much lighter: it uses prebuilt bottles plus one ~10 minute source build of OCCT.

---

## Linux Support

Building and running the application on Linux is possible. Because toolchain configurations and dependency requirements vary by distribution, please **contact Nabil** directly to get set up.

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
