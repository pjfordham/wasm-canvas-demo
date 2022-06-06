
#include "support.hh"

const int BOARD_SIZE = 100;
const int TILE_SIZE = 10;

const unsigned int SCREEN_WIDTH  = (2+BOARD_SIZE) * TILE_SIZE;
const unsigned int SCREEN_HEIGHT = (2+BOARD_SIZE) * TILE_SIZE;

unsigned int BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];

extern "C" int getWidth() {
   return SCREEN_WIDTH;
}

extern "C" int getHeight() {
   return SCREEN_HEIGHT;
}

extern "C" void plot(int x, int y, int color) {
   BUFFER[SCREEN_WIDTH * y + x] = color;
}

void plot_rectange(int x, int y, int color) {
   for (int i = 0; i < TILE_SIZE; ++i) {
      for (int j = 0; j < TILE_SIZE; ++j) {
         plot( x * TILE_SIZE + i,
               y * TILE_SIZE + j,
               color );
      }
   }
}


const int HEIGHT=BOARD_SIZE;
const int WIDTH=BOARD_SIZE;

struct Shape {
public:
   int height;
   int width;
   const char * const* figure;
   const char *name;
};

#define SubShape( NAME, ... )                             \
   struct NAME : public Shape {                           \
      NAME() {                                            \
         static const char *shape[] = { __VA_ARGS__ };    \
         static const char *Name = #NAME;                 \
         name = Name;                                     \
         figure = shape;                                  \
         height = sizeof( shape ) / sizeof( *shape );     \
         width = 0;                      \
      }                                                   \
   };

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

class GameOfLife {
public:

   static const int LIVE = 100;
   static const int DEAD = 0;

   GameOfLife();
   void addShape( Shape shape, int x = -1 , int y = -1);
   void click( int i, int j );

   void print();
   void update();
   void clear();
   int getContent( int i, int j);
   int getState( int state , int x , int y );
   void iterate(unsigned int iterations);
private:
   Array2D<int> world;
   Array2D<int> otherWorld;

};

GameOfLife::GameOfLife() : world(HEIGHT,WIDTH), otherWorld(HEIGHT,WIDTH)  {
   clear();
}

void GameOfLife::clear() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         world[i][j] = DEAD;
      }
   }
}

void GameOfLife::click( int j, int i )
{
   if ( world[i][j] == LIVE ){
      world[i][j] = DEAD;
   } else {
      world[i][j] = LIVE;
   }
}

void GameOfLife::addShape( Shape shape, int x, int y )
{
   for ( int i = y; i - y < shape.height; i++ ) {
      for ( int j = x; j - x < shape.width; j++ ) {
         if ( i < HEIGHT && j < WIDTH ) {
            world[i][j] =
               shape.figure[ i - y ][j - x ] == 'X' ? LIVE : DEAD;
         }
      }
   }
}

void GameOfLife::print() {
   // for ( int i = 0; i < HEIGHT; i++ ) {
   //    for ( int j = 0; j < WIDTH; j++ ) {
   //       std::cout << world[i][j];
   //    }
   //    std::cout << std::endl;
   // }
   // for ( int i = 0; i < WIDTH; i++ ) {
   //    std::cout << '=';
   // }
   // std::cout << std::endl;
}

int GameOfLife::getContent(int i, int j) {
   return world[i][j];
}

namespace std {
   template<class T> 
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

} // namespace std

void GameOfLife::update() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         otherWorld[i][j] =
            GameOfLife::getState(world[i][j] , i , j);
      }
   }
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         world[i][j] = otherWorld[i][j];
      }
   }
   // auto temp = world;
   // world = otherWorld;
   // otherWorld = temp;
}

int GameOfLife::getState( int state, int y, int x ) {
    int neighbors = 0;
    for ( int i = y - 1; i <= y + 1; i++ ) {
       for ( int j = x - 1; j <= x + 1; j++ ) {
          if ( i == y && j == x ) {
             continue;
          }
          if ( i > -1 && i < HEIGHT && j > -1 && j < WIDTH ) {
             if ( world[i][j] == LIVE ) {
                neighbors++;
             }
          }
       }
    }
    if (state == LIVE) {
       return ( neighbors > 1 && neighbors < 4 ) ? LIVE : std::max( 0, world[y][x]-1);
    }
    else {
       return ( neighbors == 3 ) ? LIVE : std::max(0, world[y][x]-1);
    }
}

void GameOfLife::iterate( unsigned int iterations ) {
    for ( int i = 0; i < iterations; i++ ) {
        update();
    }
}




GameOfLife gol;

Shape shapes[] = { Almond(), Glider(), Crab(), RPentomino(), SpaceShip(), Blinker() };
int shapeIndex = 0;

extern "C" void init() {
     // // Clear window to Blue to do blue boarder.
     //  window.clear( sf::Color::Blue );

     //  // draw black background for theatre of life
     //  sf::RectangleShape shape(sf::Vector2f(TILE_SIZE*BOARD_SIZE, TILE_SIZE*BOARD_SIZE));
     //  shape.setPosition( TILE_SIZE, TILE_SIZE);
     //  shape.setFillColor(sf::Color::Black);
     //  window.draw(shape);
}

extern "C" void pulse() {
    plot_rectange(0, 0, 0xFF445566);
    for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
         auto content = gol.getContent(x, y);
         switch ( content ) {
         case GameOfLife::DEAD:
           plot_rectange(x, y, 0xFF000000);
            break;
         case GameOfLife::LIVE:
         {
           plot_rectange(x, y, 0xFFFF0000);
            break;
         }
         default:
         {
           plot_rectange(x, y, 0xFFFF0000 * content);
            break;
         }
         }
      }
   }
}

extern "C" void click(int x, int y) {
  gol.iterate(1);

    gol.click( (y / (int)TILE_SIZE ) - 1,
              (x / (int)TILE_SIZE ) - 1 );
}

