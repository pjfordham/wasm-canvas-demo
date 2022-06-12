#ifndef CPPLIB_FILE_H
#define CPPLIB_FILE_H

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

#endif
   
