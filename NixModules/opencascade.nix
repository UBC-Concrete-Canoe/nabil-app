# Taken from https://github.com/NixOS/nixpkgs/blob/nixos-unstable/pkgs/by-name/op/opencascade-occt/package.nix 
# Updated for 7.9.3
# NOTE: VTK relies on OCCT, so if you wanna use this, override vtk to use this too
{
	lib,
	stdenv,
	fetchurl,
	fetchpatch,
	cmake,
	ninja,
	rapidjson,
	freetype,
	fontconfig,
	tcl,
	tk,
	libGL,
	libGLU,
	libXext,
	libXmu,
	libXi,
	vtk,
	withVtk ? false,
}:
stdenv.mkDerivation rec {
	pname = "opencascade-occt";
	version = "7.9.3";
	commit = "V${builtins.replaceStrings [ "." ] [ "_" ] version}";

	src = fetchurl {
		url = "https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/V${builtins.replaceStrings [ "." ] [ "_" ] version}.tar.gz";
        hash = "sha256-Xs8JTsaxLVQT37hR2MNZDDVAWK7lVuMuQIvfv4w1fVc=";
	};

	patches = [
		# Seems like this is for an older version of Occt. It fails to patch on 7.9.3
		# fix compilation on darwin against latest version of freetype
		# https://gitlab.freedesktop.org/freetype/freetype/-/merge_requests/330
		#(fetchpatch {
		#	url = "https://github.com/Open-Cascade-SAS/OCCT/commit/7236e83dcc1e7284e66dc61e612154617ef715d6.diff";
		#	hash = "sha256-NoC2mE3DG78Y0c9UWonx1vmXoU4g5XxFUT3eVXqLU60=";
		#})
	];

	nativeBuildInputs = [
		cmake
		ninja
	];

	buildInputs = [
		tcl
		tk
		libGL
		libGLU
		libXext
		libXmu
		libXi
		rapidjson
		freetype
		fontconfig
	]
	++ lib.optional withVtk vtk;

	env.NIX_CFLAGS_COMPILE = "-fpermissive";
	cmakeFlags = [
		(lib.cmakeBool "USE_RAPIDJSON" true)
		# Enable exception handling for release builds.
		(lib.cmakeBool "BUILD_RELEASE_DISABLE_EXCEPTIONS" false)
		# cmake 4 compatibility, old versions upstream need like 3 patches to get to a
		# supported version, so just use the big hammer
		(lib.cmakeFeature "CMAKE_POLICY_VERSION_MINIMUM" "3.10")

		# Use freetype
		(lib.cmakeBool "USE_FREETYPE" true)
	]
	++ lib.optionals withVtk [
		(lib.cmakeBool "USE_VTK" true)
		(lib.cmakeFeature "3RDPARTY_VTK_INCLUDE_DIR" "${lib.getDev vtk}/include/vtk")
	];

	meta = {
		description = "Open CASCADE Technology, libraries for 3D modeling and numerical simulation";
		homepage = "https://www.opencascade.org/";
		license = lib.licenses.lgpl21; # essentially...
		# The special exception defined in the file OCCT_LGPL_EXCEPTION.txt
		# are basically about making the license a little less share-alike.

		# Uhh this is internal build right? So it's not necessary?
		# maintainers = with lib.maintainers; [ amiloradovsky ];
		platforms = lib.platforms.all;
	};
}
