# Nabil App Setup Guide

This guide covers setting up your local development environment, installing dependencies via `vcpkg`, and configuring/building the project using CMake presets on Windows and macOS.

> **Storage & Time Warning:**  
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

### Prerequisites

* **Xcode**: Full installation from the App Store.
* **Homebrew**: Installed via [brew.sh](https://brew.sh/).
* At least **100 GB** of available disk space.

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

3. **Install build utilities:**
   ```bash
   brew install ninja pkg-config autoconf automake libtool autoconf-archive
   ```

4. **Install vcpkg:**
   ```bash
   git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
   cd ~/vcpkg
   ./bootstrap-vcpkg.sh
   ~/vcpkg/vcpkg version
   ```

   > **Note:** If you encounter libtool errors during builds, export the GNU tool path:
   > ```bash
   > export PATH="/opt/homebrew/opt/libtool/libexec/gnubin:$PATH"
   > ```

5. **Clone the repository:**
   ```bash
   git clone https://github.com/UBC-Concrete-Canoe/nabil-app
   cd nabil-app
   ```

6. **Create `CMakeUserPresets.json`:**  
   In the root of the project, create `CMakeUserPresets.json`:

   ```json
   {
     "version": 3,
     "configurePresets": [
       {
         "name": "macos-vcpkg-release",
         "inherits": "ninja-vcpkg-release",
         "cacheVariables": {
           "CMAKE_TOOLCHAIN_FILE": "$env{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake",
           "VCPKG_TARGET_TRIPLET": "arm64-osx"
         }
       },
       {
         "name": "macos-vcpkg-debug",
         "inherits": "ninja-vcpkg-debug",
         "cacheVariables": {
           "CMAKE_TOOLCHAIN_FILE": "$env{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake",
           "VCPKG_TARGET_TRIPLET": "arm64-osx"
         }
       }
     ],
     "buildPresets": [
       {
         "name": "macos-build-release",
         "configurePreset": "macos-vcpkg-release"
       },
       {
         "name": "macos-build-debug",
         "configurePreset": "macos-vcpkg-debug"
       }
     ]
   }
   ```

7. **Configure:**
   ```bash
   cmake --preset macos-vcpkg-release
   ```

8. **Build:**
   ```bash
   cmake --build --preset macos-build-release
   ```
