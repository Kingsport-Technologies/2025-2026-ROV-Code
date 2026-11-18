# 2025-2026-ROV-Code
Code for ROV 2025-2026!!!
Before running this code you have to :
sudo apt update
sudo apt install libwebsocketpp-dev libboost-all-dev

Builds in dockerfile with:
```bash
rm -rf build-win && mkdir build-win && cd build-win

cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-x86_64.cmake \
      -DQT_HOST_PATH=/root/Qt/6.8.2/gcc_64 \
      ..

cmake --build . -j$(nproc)
```
Save dockerfile with:
```
 docker commit {{Container name}} my-saved-state:v1
 docker save -o /path/to/usb/my-saved-state.tar my-saved-state:v1
```
Dockerfile stored at:

# TODO

- Update all network code to run cross platform Qt networking rather than netinet