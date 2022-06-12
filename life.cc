#include "life.hh"

GameOfLife::GameOfLife()  : world(HEIGHT,WIDTH), otherWorld(HEIGHT,WIDTH) {
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
            world[i][j] = shape.figure[ i - y ][j - x ] == 'X' ? LIVE : DEAD;
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

void GameOfLife::update() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         otherWorld[i][j] =
            GameOfLife::getState(world[i][j] , i , j);
      }
   }
   std::swap(world, otherWorld);
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

