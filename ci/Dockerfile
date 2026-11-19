# CI image for nabil-app: toolchain + all dependencies preinstalled.
# Rebuild via .github/workflows/ci-image.yml when dependencies change.
#
# Fedora 44+ is used because its vtk-qt package is built against Qt6
# (GUISupportQt); Debian/Ubuntu ship a Qt5-based libvtk9-qt-dev and
# Fedora 43 still used Qt5.
FROM fedora:44

RUN dnf install -y \
        gcc-c++ \
        cmake \
        ninja-build \
        git \
        curl \
        qt6-qtbase-devel \
        vtk-devel \
        vtk-qt \
        gtest-devel \
        gmock-devel \
        freetype-devel \
        fontconfig-devel \
        rapidjson-devel \
        tbb-devel \
        mesa-libGL-devel \
        libxkbcommon-devel \
        libX11-devel \
        libXext-devel \
        libXmu-devel \
        libXi-devel \
    && dnf clean all

# OCCT 8.0.1 is newer than Fedora's opencascade package, so build it into
# /opt/occt with the same flags as packaging/homebrew/opencascade.rb.
COPY ci/install-occt.sh /tmp/install-occt.sh
RUN chmod +x /tmp/install-occt.sh \
    && /tmp/install-occt.sh /opt/occt \
    && rm /tmp/install-occt.sh

ENV CMAKE_PREFIX_PATH="/opt/occt"
