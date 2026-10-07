{
	description = "C++ Dev Setup with Qt6 & OCCT 8.0.1";

	inputs = {
		nixpkgs.url = "github:Nixos/nixpkgs/nixos-unstable";
		flake-parts.url = "github:hercules-ci/flake-parts";
	};

	outputs =
		inputs@{ nixpkgs, flake-parts, ... }:
		flake-parts.lib.mkFlake { inherit inputs; } {
			systems = [
				"x86_64-linux"
				"aarch64-linux"
			];

			perSystem =
				{ pkgs, ... }:
				let
					vtk = pkgs.vtkWithQt6;
					opencascade = pkgs.callPackage ./NixModules/opencascade.nix {
						inherit vtk;
						withVtk = true;
					};
				in
				{
					packages.opencascade-occt = opencascade;

					devShells.default = pkgs.mkShell {
						nativeBuildInputs = with pkgs; [
							# Build tools
							gcc
							cmake
							ninja
							pkg-config
							gdb
							qt6.wrapQtAppsHook # Set Qt envvars

							# Test suite
							gtest

							# Formatter
							cmake-format
							clang-tools
						];

						buildInputs = with pkgs; [
							# Qt 6 Modules
							qt6.qtbase
							qt6.qtdeclarative

							opencascade
							vtk
							freetype

							qt6.qtwayland
							libGL
							libxkbcommon
							libx11
						];

						shellHook =
						/*nixfmt:disable*/
						''
							# Qt 6 Environment
							export QT_QPA_PLATFORM="wayland;xcb"
							export QT_QPA_PLATFORM_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins"
							echo "OS: Linux Detected"

							# OpenCascade Environment
							export CASROOT="${opencascade}"

							# Ensure CMake finds our local packages easily
							export CMAKE_PREFIX_PATH="${pkgs.qt6.qtbase}:${opencascade}:$CMAKE_PREFIX_PATH"

							echo "-------------------------------------------------------"
							echo "OpenCascade 8.0.1 + Qt6 Dev Environment Active"
							echo "OCCT Path: $CASROOT"
							echo "-------------------------------------------------------"
						'';
						/*nixfmt:enable*/
					};
				};
		};
}
