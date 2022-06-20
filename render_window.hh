#ifndef RENDER_WINDOW_FILE_H
#define RENDER_WINDOW_FILE_H

#include "jslib.hh"

class RectangleShape {
   unsigned int width;
   unsigned int height;
   unsigned int x;
   unsigned int y;
   js_color color;
public:
   constexpr RectangleShape(  unsigned int _width, unsigned int _height )
      : width(_width), height(_height), x(0), y(0), color( js_black ) {
   }
   constexpr void setFillColor( js_color _color ) {
      color = _color;
   }
   constexpr void setPosition( unsigned int _x, unsigned int _y ) {
      x = _x;
      y = _y;
   }
   friend class RenderWindow;
};


class RenderWindow {

   unsigned int *BUFFER;
   unsigned int width;
   unsigned int height;

public:

   RenderWindow(unsigned int _width, unsigned int _height) {
      width = _width;
      height = _height;
      BUFFER = new unsigned int[ width * height ];
   }

   ~RenderWindow() {
      delete BUFFER;
   }

   void clear( js_color color ) {
      for (int i = 0; i < width * height; ++i) {
         BUFFER[i] = color;
      }
   }

   unsigned int getWidth() {
      return width;
   }

   unsigned int getHeight() {
      return height;
   }

   unsigned int *get_buffer_address() {
      return BUFFER;
   }

   void plot(int x, int y, unsigned int color) {
      BUFFER[width * y + x] = color;
   }

   void draw(const RectangleShape &shape) {
      for (int i = shape.x; i < (shape.x + shape.width) ; ++i) {
         for (int j = shape.y; j < (shape.y + shape.height) ; ++j) {
            plot( i, j, shape.color );
         }
      }
   }

};

#endif

