# Build Instruction for Lua

```
rm -r lua/lua.c
rm -r lua/onelua.c
```

go to `lua/loaconf.h` line `125` and change `0` to `1`
```
/*
@@ LUA_32BITS enables Lua with 32-bit integers and 32-bit floats.
*/
//---#define LUA_32BITS	0
#define LUA_32BITS	1
```
after that run
```
mkdir -p ../libs/
make
cp -r include ../libs/
cp -r lib ../libs/
make clean
```