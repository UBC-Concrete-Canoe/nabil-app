{
	description = "C++ Dev Setup with Qt6 & OCCT 7.9.3";

	inputs = {
		nixpkgs.url = "github:Nixos/nixpkgs/nixos-unstable";
		flake-parts.url = "github:hercules-ci/flake-parts";
	};

	outputs = inputs@{ nixpkgs, flake-parts, ... }:
		flake-parts.lib.mkFlake { inherit inputs; } {
			systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];

			perSystem = { config, self', pkgs, ... }: 
			let
				# Is it really necessary to do it this way? Well idk :shrug:
				# Likely not, but it's working for me, so might as well right?

				occCustom = pkgs.callPackage ./NixModules/opencascade.nix {};
			in
			{
				packages.opencascade-occt = occCustom;

				devShells.default = pkgs.mkShell {
					nativeBuildInputs = with pkgs; [
						# Build tools
						gcc
						cmake
						ninja
						pkg-config
						gdb

						# Formatter
						cmake-format
						clang-tools
					];
					
					buildInputs = with pkgs; [
						# Qt 6 Modules
						qt6.qtbase
						qt6.qtdeclarative
						qt6.wrapQtAppsHook # Set Qt envvars

						self'.packages.opencascade-occt
					]
					++ lib.optionals stdenv.isLinux [
						qt6.qtwayland
						libGL
						libxkbcommon
						xorg.libX11
					]
					++ lib.optionals stdenv.isDarwin (with darwin.apple_sdk.frameworks; [
						Cocoa
						OpenGL
						IOKit
						AppKit
					]);

					shellHook = ''
						# Qt 6 Environment
						${if pkgs.stdenv.isLinux then ''
							export QT_QPA_PLATFORM="wayland;xcb"
							export QT_QPA_PLATFORM_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins"
							echo "OS: Linux Detected"
						'' else ''
							export QT_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins"
							echo "OS: MacOS Detected"
						''}

						# OpenCascade Environment
						export CASROOT="${self'.packages.opencascade-occt}"

						# Ensure CMake finds our local packages easily
						export CMAKE_PREFIX_PATH="${pkgs.qt6.qtbase}:${self'.packages.opencascade-occt}:$CMAKE_PREFIX_PATH"

						echo "-------------------------------------------------------"
						echo "OpenCascade 7.9.3 + Qt6 Dev Environment Active"
						echo "OCCT Path: $CASROOT"
						echo "-------------------------------------------------------"
					'';
				};
			};
		};
}
