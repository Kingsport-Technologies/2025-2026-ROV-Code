FROM ubuntu:24.04

ARG REPO_URL=https://github.com/Kingsport-Technologies/2025-2026-ROV-Code.git
ARG MXE_TARGET=x86_64-w64-mingw32.static
ARG BUILD_JOBS=2

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/opt/mxe/usr/bin:${PATH}"

RUN apt-get update && apt-get install -y --no-install-recommends \
	autoconf \
	automake \
	autopoint \
	bash \
	bison \
	bzip2 \
	clang \
	cmake \
	flex \
	g++ \
	g++-multilib \
	gettext \
	git \
	gperf \
	intltool \
	libc6-dev-i386 \
	libclang-dev \
	libgdk-pixbuf-2.0-dev \
	libgl-dev \
	libltdl-dev \
	libpcre2-dev \
	libssl-dev \
	libtool-bin \
	libxml-parser-perl \
	llvm-dev \
	lzip \
	make \
	openssl \
	p7zip-full \
	patch \
	perl \
	pkg-config \
	python-is-python3 \
	python3 \
	python3-mako \
	python3-packaging \
	python3-setuptools \
	python3-yaml \
	ruby \
	sed \
	sqlite3 \
	unzip \
	wget \
	xz-utils \
	ca-certificates \
	qt6-base-dev \
	qt6-declarative-dev \
	qt6-multimedia-dev \
	libgstreamer1.0-dev \
	libgstreamer-plugins-base1.0-dev \
	libgstreamer-plugins-bad1.0-dev \
	gstreamer1.0-plugins-base-apps \
	gstreamer1.0-x \
	&& rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 https://github.com/libsdl-org/SDL.git /tmp/SDL \
	&& cmake -S /tmp/SDL -B /tmp/SDL/build \
		-DCMAKE_BUILD_TYPE=Release \
		-DSDL_TESTS=OFF \
		-DSDL_TEST_LIBRARY=OFF \
		-DCMAKE_INSTALL_PREFIX=/usr/local \
	&& cmake --build /tmp/SDL/build --parallel ${BUILD_JOBS} \
	&& cmake --install /tmp/SDL/build \
	&& ldconfig \
	&& rm -rf /tmp/SDL
