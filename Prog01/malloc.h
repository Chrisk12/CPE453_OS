#include <errno.h>
#include <unistd.h>

typedef struct Memblock Memblock;

struct  Memblock {

   int free;
   Memblock *next;
   Memblock *prev;
   size_t size;
   void *data;

};
