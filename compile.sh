clang++  --target=wasm32 -std=c++17 -O3 -fno-use-cxa-atexit -nostdlib -Wl,--no-entry -Wl,--export-all -o bare_metal_wasm.wasm graphics.cc memory.cc life.cc
