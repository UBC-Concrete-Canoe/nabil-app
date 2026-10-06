# Nabil App Setup Guide

This guide covers setting up your local development environment and configuring/building the project using CMake presets on Windows and macOS.

* **Windows** installs all dependencies via `vcpkg` (built from source).
* **macOS** installs dependencies via **Homebrew** (prebuilt binaries — much faster).

> **Storage & Time Warning (vcpkg versions):**  
> Ensure you have at least **100 GB of free disk space**. Initial configuration installs all dependencies via `vcpkg`, which can take several hours depending on your machine.

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

Using homebrew is faster and takes up less storage. Also using vcpkg makes us use latest version of OCCT, while flake.nix mnetions that we should 7.9.3.

### Prerequisites

* **Xcode**: Full installation from the App Store.
* **Homebrew**: Installed via [brew.sh](https://brew.sh/).

---

### Step-by-Step Setup

1. **Configure developer tools and license:**
   ```bash
   sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
   sudo xcodebuild -license accept
   ```

2. **Verify standard toolchains:**
   ```bash
   cmake --version
   make --version
   clang --version
   ```
   *If any tool is missing, install it via Homebrew (e.g., `brew install cmake`).*

3. **Install build tools and project dependencies:**
   ```bash
   brew install cmake ninja pkg-config opencascade qt vtk
   ```

   > **Note:** `brew upgrade` can bump `opencascade` to a newer major version (8.x) that changes deprecated APIs. To stay on 7.9.3, pin it:
   > ```bash
   > brew pin opencascade
   > ```

4. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```

5. **Create `CMakeUserPresets.json`:**  
   In the root of the project, create `CMakeUserPresets.json`:

   ```json
   {
     "version": 3,
     "configurePresets": [
       {
         "name": "macos-brew-release",
         "generator": "Ninja",
         "binaryDir": "${sourceDir}/build/brew-release",
         "cacheVariables": {
           "CMAKE_BUILD_TYPE": "Release",
           "CMAKE_PREFIX_PATH": "/opt/homebrew;/opt/homebrew/opt/opencascade;/opt/homebrew/opt/qtbase;/opt/homebrew/opt/vtk"
         }
       },
       {
         "name": "macos-brew-debug",
         "generator": "Ninja",
         "binaryDir": "${sourceDir}/build/brew-debug",
         "cacheVariables": {
           "CMAKE_BUILD_TYPE": "Debug",
           "CMAKE_PREFIX_PATH": "/opt/homebrew;/opt/homebrew/opt/opencascade;/opt/homebrew/opt/qtbase;/opt/homebrew/opt/vtk"
         }
       }
     ],
     "buildPresets": [
       {
         "name": "macos-brew-build-release",
         "configurePreset": "macos-brew-release"
       },
       {
         "name": "macos-brew-build-debug",
         "configurePreset": "macos-brew-debug"
       }
     ]
   }
   ```

6. **Configure:**
   ```bash
   cmake --preset macos-brew-release
   ```

7. **Build:**
   ```bash
   cmake --build --preset macos-brew-build-release
   ```

8. **Run:**  
   ```bash
   open ./build/brew-release/coco.app
   ```
