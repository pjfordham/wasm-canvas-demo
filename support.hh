#ifndef SUPPORT_FILE_H
#define SUPPORT_FILE_H



template<typename type>
class Array2D {
   int y;
   type bigarray[1024];
   type *data;
public:
   // We can't do a copy since we don't know
   // what size to allocate the array.
   Array2D(const Array2D&) = delete;
   Array2D& operator=(const Array2D&) = delete;

   // Array2D(Array2D&& move) noexcept :
   //    y(std::exchange(move.y, 0)),
   //    data(std::exchange(move.data, nullptr)) {}

   // Array2D& operator=(Array2D&& move) noexcept {
   //    std::swap(y, move.y);
   //    std::swap(data, move.data);
   //    return *this;
   // }

   // implement an iterator that walks whole array
   // have some get x,y pos maybe too to optimize other stuff

   Array2D( int _x, int _y ) : y(_y) {
      data = &bigarray[0];
   }

   type const *operator[](int i) const {
      return data + (i * y);
   }

   type *operator[](int i) {
      return data + (i * y);
   }

   ~Array2D() {
      // delete[] data;
   }
};

#endif // SUPPORT_FILE_H