# EDISON 2.0


## Third-party Libraries
- ThirdParty/OpenCASCADE-wasm-7.6.0
- emsdk (should install by yourself - [Emscripten](https://emscripten.org/docs/getting_started/downloads.html))
- CMake (should install by yourself - [CMake](https://cmake.org/download/))
- Make (should install by yourself - [Make](http://gnuwin32.sourceforge.net/packages/make.htm))

## Scripts
### Install node_modules
```$ npm install```

1. Generate Makefile at build-wasm/build. Third party directories(OpenCADCADE WebAssembly Files) need to be checked
```$ npm run make-proj```
2. Build wasm and glue javascript-code
```$ npm run build-wasm```
3. Copy wasm and glue javascript files in build-wasm/build folder to src/views folder
```$ npm run copy-wasm```
4. Remove wasm and glue javascript files in src/views folder and copy wasm and glue javascript files in build-wasm/build folder to src/views folder
```$ npm run replace-wasm```
5. Start Node.js-based Web-Server
```$ npm run start-server```
6. Remove build-wasm/build folder
```$ npm run rm-build```

### Script Order for the First Time
1 &rarr; 2 &rarr; 3 &rarr; 5

### Script Order for the Second Time (C/C++ files are modified)
2 &rarr; 4 &rarr; 5

### Script Order for the Second Time (C/C++ files are added)
Modify CMakeLists.txt &rarr; 6 &rarr; 1 &rarr; 2 &rarr; 4 &rarr; 5