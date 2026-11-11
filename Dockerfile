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
	&& rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 https://github.com/mxe/mxe.git /opt/mxe \
	&& make -C /opt/mxe MXE_TARGETS="${MXE_TARGET}" --jobs="${BUILD_JOBS}" JOBS="${BUILD_JOBS}" \
		qt6-qtbase qt6-qtdeclarative qt6-qtmultimedia sdl3 gstreamer gst-plugins-base

WORKDIR /workspace
RUN git clone --depth 1 "${REPO_URL}" source
WORKDIR /workspace/source

RUN "${MXE_TARGET}-cmake" \
		-S . \
		-B build \
		-DBUILD_TARGET=TOPEND \
		-DCMAKE_BUILD_TYPE=Release \
	&& cmake --build build --parallel "${BUILD_JOBS}"