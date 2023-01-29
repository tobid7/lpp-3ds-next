#Lua

cd lua
mkdir build
cd build
${DEVKITARM}/bin/arm-none-eabi-cmake --toolchain=${DEVKITPRO}/cmake/3DS.cmake ..
make -j4
make install