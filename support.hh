#ifndef SUPPORT_FILE_H
#define SUPPORT_FILE_H

unsigned char bigarray[1024 * 1024];
int offset = 0;

extern "C" void *memset(void *s, int c,  decltype(sizeof(0)) len)
{
   auto p=(unsigned char*)s;
   while(len--)
   {
      *p++ = (unsigned char)c;
   }
   return s;
}

void *operator new(decltype(sizeof(0)) n) noexcept(false)
{
   void *ptr = &bigarray[offset];
   offset+= n;
   return ptr;
}

void operator delete(void * p) throw()
{
}

void *operator new[](decltype(sizeof(0)) n) noexcept(false)
{
   void *ptr = &bigarray[offset];
   offset+= n;
   return ptr;
}

void operator delete[](void *p) throw()
                       {
                       }

class js_color {
public:
   unsigned char a,b,g,r;
   js_color(unsigned char _a, unsigned char _b, unsigned char _g, unsigned char _r) : a(_a), b(_b), g(_g), r(_r) {
   }

   js_color operator* ( js_color color ) const {
      return js_color( (a * color.a) >> 8,
                       (b * color.b) >> 8,
                       (g * color.g) >> 8,
                       (r * color.r) >> 8 );
   }

   operator unsigned int() const {
      return (a << 24) | (b << 16) | (g << 8) | r;
   }

};
const js_color js_red(0xFF, 0x00, 0x00, 0xFF);
const js_color js_green(0xFF, 0x00, 0xFF, 0x00);
const js_color js_blue(0xFF, 0xFF, 0x00, 0x00);
const js_color js_black(0xFF, 0x00, 0x00, 0x00);
const js_color js_white(0xFF, 0xFF, 0xff, 0xFF);


namespace std {
   template <class T>
   struct remove_reference { typedef T type; };

   template <class T>
   struct remove_reference<T&> { typedef T type; };

   template <class T> struct remove_reference<T &&> { typedef T type; };

   template <typename T>
   typename remove_reference<T>::type&& move(T&& arg)
   {
      return static_cast<typename remove_reference<T>::type&&>(arg);
   }

   template<typename T> void swap(T& t1, T& t2) {
      T temp = move(t1); // or T temp(std::move(t1));
      t1 = move(t2);
      t2 = move(temp);
   }

   template <class T>
   inline T&& forward(typename std::remove_reference<T>::type& t) noexcept
   {
      return static_cast<T&&>(t);
   }

   template <class T>
   inline T&& forward(typename std::remove_reference<T>::type&& t) noexcept
   {
      return static_cast<T&&>(t);
   }

   template<class T, class U = T>
   constexpr // since C++20
   T exchange(T& obj, U&& new_value)
   {
      T old_value = move(obj);
      obj = forward<U>(new_value);
      return old_value;
   }

   template<class T>
   const T& max(const T& a, const T& b)
   {
      return (a < b) ? b : a;
   }

} // namespace std

inline char* strcpy(char* destination, const char* source)
{
    char *ptr = destination;
    while (*source != '\0')
    {
        *destination++ = *source++;
    }
    *destination = '\0';
    return ptr;
}


template<typename type>
class Array2D {
   int y;
   type *data;
public:
   // We can't do a copy since we don't know
   // what size to allocate the array.
   Array2D(const Array2D&) = delete;
   Array2D& operator=(const Array2D&) = delete;

   Array2D(Array2D&& move) noexcept :
      y(std::exchange(move.y, 0)),
      data(std::exchange(move.data, (type*)nullptr)) {}

   Array2D& operator=(Array2D&& move) noexcept {
      std::swap(y, move.y);
      std::swap(data, move.data);
      return *this;
   }

   // implement an iterator that walks whole array
   // have some get x,y pos maybe too to optimize other stuff

   Array2D( int _x, int _y ) : y(_y) {
      data = new type[_x*y];
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
