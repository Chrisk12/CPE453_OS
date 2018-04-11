/*
* @Author: chriskim
* @Date:   2018-04-10 09:35:54
* @Last Modified by:   Chris Kim
* @Last Modified time: 2018-04-10 17:14:57
*/
#include <malloc.h>

void *malloc(size_t size) {
   static int first = 1;
   static size_t remaining = 64000;
   static void *headerAddress;
   static void *returnAddress;
   void *next;
   int roundUp = 0;

   if (first) {
      first = 0;

      if(size > 64000){
         remaining = size;
      }

      if ((strbrk(0) % 16) != 0) {
         headerAddress = strbrk(16 - (strbrk(0) % 16));
         headerAddress = strbrk(64000);
      }
      else {
         headerAddress = strbrk(64000);
      }

      if (headerAddress == -1){
         errno = ENOMEM;
         printf("%s\n", strerror(errno));
         return NULL;
      }

      (Memblock *)headerAddress->free = 0;
      (Memblock *)headerAddress->prev = NULL;
      (Memblock *)headerAddress->size = size
      if ((headerAddress + sizeof(Memblock) + size) % 16 == 0) {

         (Memblock *)headerAddress->next = 
          headerAddress + sizeof(Memblock) + size;

      }
      else {

         int temp = headerAddress + sizeof(Memblock) + size;
         roundUp = (headerAddress + sizeof(Memblock) + size) % 16;
         temp += roundUp;

         (Memblock *)headerAddress->next = temp;  

      }

      remaining -= sizeof(Memblock) + size + roundup;

   }
   else {

      if (((Memblock *)headerAddress->free) 
       && (size < (Memblock *)headerAddress->size)) {
         headerAddress = headerAddress;
      }
      else {
         headerAddress = (Memblock *)headerAddress->next;
      }

      if ((size > remaining) && (size > 64000)) {
         
         if (strbrk(size) == -1) {

            errno = ENOMEM;
            printf("%s\n", strerror(errno));
            return NULL;
        }
      
      }


   }

}
