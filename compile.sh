bare metal:

clang++  --target=wasm32 -std=c++17 -O3 -fno-use-cxa-atexit -nostdlib -Wl,--no-entry -Wl,--export-all -o bare_metal_wasm.wasm graphics.cc memory.cc life.cc

emscripten:

emcc life.cc graphics.cc -o life.html --shell-file=shell.html  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -sEXPORTED_FUNCTIONS=_getWidth,_getHeight,_click,_rightclick,_keypress,_get_buffer_address 
