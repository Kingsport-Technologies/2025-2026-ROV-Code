FROM ubuntu:26.04

RUN apt-get update 
RUN apt-get install -y build-essential git curl wget cmake qt6-base-dev qt6-declarative-dev qt6-multimedia-dev libsdl3-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev mingw-w64 python3-pip pipx
SHELL ["/bin/bash", "-c"]
RUN python3 -m venv .venv && source .venv/bin/activate && \
    pip install aqtinstall && \
    aqt install-qt linux desktop 6.8.2 linux_gcc_64 -O ~/Qt && \
    aqt install-qt windows desktop 6.8.2 win64_mingw -O ~/Qt && \
    aqt install-qt windows desktop 6.8.2 win64_mingw -m qtmultimedia -O /root/Qt && \
    aqt install-qt linux desktop 6.8.2 linux_gcc_64 -m qtmultimedia -O /root/Qt
RUN mkdir -p /root/SDL3
RUN wget https://github.com/libsdl-org/SDL/releases/download/release-3.4.20/SDL3-devel-3.4.20-mingw.tar.gz
RUN tar -xzf SDL3-devel-3.4.20-mingw.tar.gz -C /root/SDL3 --strip-components=1
