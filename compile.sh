clang++  --target=wasm32 -O2 -nostdlib -Wl,--no-entry -Wl,--export-all -o bare_metal_wasm.wasm graphics.cc

// Building with emscripten seems to be a mistake
emcc graphics.cc -O3   -o bare_metal_wasm.wasm  -s EXPORT_ALL=1 --no-entry
