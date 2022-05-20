# EDISON 2.0


## Third-party Libraries
- ThirdParty/OpenCASCADE-wasm-7.6.0
- emsdk (should install by yourself - [Emscripten](https://emscripten.org/docs/getting_started/downloads.html))
- CMake (should install by yourself - [CMake](https://cmake.org/download/))
- Make (should install by yourself - [Make](http://gnuwin32.sourceforge.net/packages/make.htm))

## Scripts
1. npm run make-proj: Generate Makefile at build-wasm/build. Third party directories(OpenCADCADE WebAssembly Files) need to be checked
2. npm run build-wasm Generate: Build wasm and glue javascript-code
3. npm run copy-wasm: Copy wasm and glue javascript files in build-wasm/build folder to src/views folder
4. npm run replace-wasm: Remove wasm and glue javascript files in src/views folder and copy wasm and glue javascript files in build-wasm/build folder to src/views folder
5. npm run start-server: Start Node.js-based Web-Server
6. npm run rm-build: Remove build-wasm/build folder

### Script Order for the First Time
1 &rarr; 2 &rarr; 3 &rarr; 5

### Script Order for the Second Time (C/C++ files are modified)
2 &rarr; 4 &rarr; 5

### Script Order for the Second Time (C/C++ files are added)
Modify CMakeLists.txt &rarr; 6 &rarr; 1 &rarr; 2 &rarr; 4 &rarr; 5