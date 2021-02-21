const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
unsigned int BUFFER[WIDTH * HEIGHT];

extern "C" int getWidth() {
   return WIDTH;
}

extern "C" int getHeight() {
   return HEIGHT;
}

extern "C" void plot(int x, int y, int color) {
   BUFFER[WIDTH * y + x] = color;
}

extern "C" void init() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
           // RGBA little endian
           plot( x,y, (y < HEIGHT/2) ? 0xff0000ff : 0xffff0000 );
        }
    }
}

