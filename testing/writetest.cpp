#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>     /* uint32_t */
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

/* User has read & write perms, group and others have read permission */ 
const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

typedef struct prova {
   uint32_t first;
   uint32_t second;
} prova_t;

#define FILENAME "file.b"

/* 'Safe' write */
int safewrite( int fd, const void *p, size_t want) {
   int ret;

   errno = 0;
   while (want) {
      ret = write(fd, (uint8_t *)p, want);
      if (ret <= 0) {
         if (errno != EINTR && errno != EAGAIN) {
            return -1;
         }
         errno = 0;
         continue;
      }
      want -= ret;
      p = (uint8_t*) p + ret;
   }
   return 0;
}

int saferead(int fd, const void *p, size_t want) {
   int ret;

   errno = 0;
   while (want) {
      ret = read(fd, (uint8_t*)p, want);
      if( ret == 0 )
         return -1;  /* EOF */
      if (ret <= 0) {
         if( errno != EINTR && errno != EAGAIN ) {
            return -1;
         }
         errno = 0;
         continue;
      }
      want -= ret;
      p = (uint8_t*) p + ret;
   }
   return 0;
}


int main(int argc, char **argv) {
   int fd;
   prova_t result;
   size_t nbytes;

   /* Create file */
   fd = creat(FILENAME, mode);
   if (fd < 0) {
      fprintf(stderr, "Unable to open " FILENAME ": %s\n",
            strerror(errno));
      exit(EXIT_FAILURE);
   }
   nbytes = sizeof(prova_t);

   result.first = 4;
   result.second = 5;

   if (0 != safewrite(fd, &result, nbytes)) {
      fprintf(stderr, "Unable to write to " FILENAME ": %s\n",
            strerror(errno));
      exit(EXIT_FAILURE);
   }

   close(fd);
   fd = -1;

   /* Reopen and read from binary file */
   fd = open(FILENAME, O_RDONLY);
   nbytes = sizeof(prova_t);

   if (0 != saferead(fd, &result, nbytes)) {
      fprintf(stderr, "Unable to read file \"" FILENAME "\": %s\n",
            strerror(errno));
      exit(EXIT_FAILURE);
   }
   close(fd);

   printf( "Read: %d %d (%#.02x%.02x)\n",
         result.first, result.second,
         result.first, result.second);

   return EXIT_SUCCESS;
}