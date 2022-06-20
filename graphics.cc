#include "render_window.hh"

#include "jslib.hh"
#include "life.hh"
#include <string.h>
#include <stdio.h>

RenderWindow *window;

const int TILE_SIZE = 10;

const unsigned int SCREEN_WIDTH  = (2+BOARD_SIZE) * TILE_SIZE;
const unsigned int SCREEN_HEIGHT = (2+BOARD_SIZE) * TILE_SIZE;

extern "C" unsigned int *get_buffer_address() { return window->get_buffer_address(); }

extern "C" int strings_size() {
   return 2048;
}

extern "C" int getWidth() {
   return SCREEN_WIDTH;
}

extern "C" int getHeight() {
   return SCREEN_HEIGHT;
}



SubShape( Crab,                                 \
          "....X.....X....",                    \
          "....X.....X....",                    \
          "....XX...XX....",                    \
          "...............",                    \
          "XXX..XX.XX..XXX",                    \
          "..X.X.X.X.X.X..",                    \
          "....XX...XX....",                    \
          "...............",                    \
          "....XX...XX....",                    \
          "..X.X.X.X.X.X..",                    \
          "XXX..XX.XX..XXX",                    \
          "...............",                    \
          "....XX...XX....",                    \
          "....X.....X....",                    \
          "....X.....X...." )

SubShape(RPentomino,                            \
         ".XX",                                 \
         "XX.",                                 \
         ".X." )

SubShape(Glider,                                \
         ".X.",                                 \
         "..X",                                 \
         "XXX" )

SubShape(Blinker, \
         "XXX" )

SubShape(Almond,                                \
         ".X.",                                 \
         "X.X",                                 \
         "X.X",                                 \
         ".X." )

SubShape(SpaceShip,                             \
         "X...X.",                              \
         ".....X",                              \
         "X....X",                              \
         ".XXXXX" )

SubShape(GliderGun,                                    \
        "........................X...........",        \
        "......................X.X...........",        \
        "............XX......XX............XX",        \
        "...........X...X....XX............XX",        \
        "XX........X.....X...XX..............",        \
        "XX........X...X.XX....X.X...........",        \
        "..........X.....X.......X...........",        \
        "...........X...X....................",        \
        "............XX......................" )

SubShape(Pentadecathlon,                       \
        "..X....X..",                          \
        "XX.XXXX.XX",                          \
        "..X....X.." )


GameOfLife gol;

bool running = false;

Shape shapes[] = { Almond(), Glider(), Crab(), RPentomino(), SpaceShip(), Blinker(), GliderGun(), Pentadecathlon() };
unsigned int shapeIndex = 0;

static void draw() {
   for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
         RectangleShape shape(TILE_SIZE, TILE_SIZE);
         shape.setPosition((x+1)*TILE_SIZE, (y+1)*TILE_SIZE);
         auto content = gol.getContent(x, y);
         switch ( content ) {
         case GameOfLife::DEAD:
            shape.setFillColor( js_black );
            break;
         case GameOfLife::LIVE:
            shape.setFillColor( js_green );
            break;
         default:
            shape.setFillColor( js_green * js_color(0xFF, content, content, content));
            break;
         }
         window->draw(shape);
      }
   }
}

extern "C" void init() {
   window = new RenderWindow( (2 + BOARD_SIZE) * TILE_SIZE,
                              (2 + BOARD_SIZE) * TILE_SIZE);
   window->clear( js_blue );
}

extern "C" void pulse() {
   if (running) {
      gol.iterate(1);
      draw();
   }
}

extern "C" void click(int x, int y) {

   auto i = (y / TILE_SIZE) - 1;
   auto j = (x / TILE_SIZE) - 1;

   if ( 0 <= i && i < BOARD_SIZE &&
        0 <= j && j < BOARD_SIZE )
   {
      gol.click( i ,j );
      draw();
   }

}

extern "C" void rightclick(int x, int y) {

   auto i = (y / TILE_SIZE) - 1;
   auto j = (x / TILE_SIZE) - 1;

   if ( 0 <= i && i < BOARD_SIZE &&
        0 <= j && j < BOARD_SIZE )
   {
      gol.addShape(shapes[ shapeIndex ], i,j);
      draw();
   }

}


extern "C" void keypress(char key) {
   switch( key ) {
   case 'i':
      gol.iterate(1);
      break;
   case 'c':
      gol.clear();
      break;
   case '+':
      shapeIndex++;
      shapeIndex = shapeIndex % 8;
      printf("%s\n", shapes[ shapeIndex ].name);
      break;
   case '-':
      shapeIndex--;
      shapeIndex = shapeIndex % 8;
      printf("%s\n", shapes[ shapeIndex ].name);
      break;
   default:
      running = !running;
      break;
   }

   draw();

}
