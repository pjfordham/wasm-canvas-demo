#include "clib.hh"
#include "cpplib.hh"
#include "jslib.hh"
#include "life.hh"

const int TILE_SIZE = 10;

const unsigned int SCREEN_WIDTH  = (2+BOARD_SIZE) * TILE_SIZE;
const unsigned int SCREEN_HEIGHT = (2+BOARD_SIZE) * TILE_SIZE;

unsigned int BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];

char STRINGS[2048] = "Conways game of Life!";

extern "C" int strings_size() {
   return 2048;
}

extern "C" int getWidth() {
   return SCREEN_WIDTH;
}

extern "C" int getHeight() {
   return SCREEN_HEIGHT;
}

extern "C" void plot(int x, int y, unsigned int color) {
   BUFFER[SCREEN_WIDTH * y + x] = color;
}

static void plot_rectange(int x, int y, unsigned int color) {
   for (int i = 0; i < TILE_SIZE; ++i) {
      for (int j = 0; j < TILE_SIZE; ++j) {
         plot( x * TILE_SIZE + i,
               y * TILE_SIZE + j,
               color );
      }
   }
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

SubShape(GliderGun,					\
	 "........................X...........",	\
	 "......................X.X...........",	\
	 "............XX......XX............XX",	\
	 "...........X...X....XX............XX",	\
	 "XX........X.....X...XX..............",	\
	 "XX........X...X.XX....X.X...........",	\
	 "..........X.....X.......X...........",	\
	 "...........X...X....................",	\
	 "............XX......................" )

SubShape(Pentadecathlon,			\
	 "..X....X..",				\
	 "XX.XXXX.XX",				\
	 "..X....X.." )





GameOfLife gol;

bool running = false;

Shape shapes[] = { Almond(), Glider(), Crab(), RPentomino(), SpaceShip(), Blinker(), GliderGun(), Pentadecathlon() };
unsigned int shapeIndex = 0;

static void draw() {
   for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
         auto content = gol.getContent(x, y);
         switch ( content ) {
         case GameOfLife::DEAD:
            plot_rectange(x+1, y+1,js_black);
            break;
         case GameOfLife::LIVE:
         {
            plot_rectange(x+1, y+1,js_green);
            break;
         }
         default:
         {
            plot_rectange(x+1, y+1, js_green * js_color(0xFF, content, content, content));
            break;
         }
         }
      }
   }

}

extern "C" void init() {
   // Clear window to Blue to do blue boarder.
   for( int x=0;x<BOARD_SIZE+2;x++ ){
      for ( int y = 0;y<BOARD_SIZE+2;y++) {
         plot_rectange(x, y, 0xFFFF0000);
      }
   }
   draw();
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
      gol.addShape( shapes[ shapeIndex ], i ,j );
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
      break;
   case '-':
      shapeIndex--;
      break;
   default:
      running = !running;
      break;
   }

   shapeIndex = shapeIndex % 8;
   strcpy( STRINGS, shapes[ shapeIndex ].name );
   draw();

}

extern "C" void atexit() {}
