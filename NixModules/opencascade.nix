{
	lib,
	stdenv,
	fetchurl,
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
stdenv.mkDerivation {
	pname = "opencascade-occt";
	version = "8.0.1";

	src = fetchurl {
		url = "https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/V8_0_1.tar.gz";
		hash = "sha256-DWkT6uS8wJo2U87O1t2hrsEcNaFRPUwGdiybACCSxoo=";
	};

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

	dontWrapQtApps = true;
	env.NIX_CFLAGS_COMPILE = "-fpermissive";

	cmakeFlags = [
		(lib.cmakeBool "USE_RAPIDJSON" true)
		(lib.cmakeBool "BUILD_RELEASE_DISABLE_EXCEPTIONS" false)
		(lib.cmakeFeature "CMAKE_POLICY_VERSION_MINIMUM" "3.10")
		(lib.cmakeBool "USE_FREETYPE" true)
	]
	++ lib.optionals withVtk [
		(lib.cmakeBool "USE_VTK" true)
		(lib.cmakeFeature "3RDPARTY_VTK_INCLUDE_DIR" "${lib.getDev vtk}/include/vtk")
	];

	meta = {
		description = "Open CASCADE Technology, libraries for 3D modeling and numerical simulation";
		homepage = "https://www.opencascade.org/";
		license = lib.licenses.lgpl21;
		platforms = lib.platforms.linux;
	};
}
