clang++  --target=wasm32 -O0 -fno-use-cxa-atexit -nostdlib -Wl,--no-entry -Wl,--export-all -o bare_metal_wasm.wasm graphics.cc
