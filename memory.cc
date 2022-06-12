unsigned char bigarray[1024 * 1024];
int offset = 0;


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

extern "C" void *memset(void *s, int c,  decltype(sizeof(0)) len)
{
   auto p=(unsigned char*)s;
   while(len--)
   {
      *p++ = (unsigned char)c;
   }
   return s;
}

