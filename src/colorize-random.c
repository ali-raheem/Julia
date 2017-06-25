#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_COLOURS 256

int main(int argc, char **argv) {
  /*  char palette[16][4] = {"\x01\x01\x0f",
                         "\x01\xf1\x1f",
			 "\xf1\x01\x2f",
			 "\x51\xb1\x3f",
			 "\x01\x01\x4f",
			 "\x51\x01\x5f",
			 "\x01\x21\x6f",
			 "\x51\xa4\x7f",
			 "\x01\xe1\x8f",
			 "\x01\x51\x9f",
			 "\xa1\x01\xaf",
			 "\xb1\x01\xbf",
			 "\x31\x51\xcf",
			 "\xc1\x41\xdf",
			 "\xe1\x51\xef",
			 "\x01\x01\xff"};*/
  char *palette[NUM_COLOURS][3];
  int f = open("/dev/urandom", O_RDONLY);
  int ii = 0;
  for(ii = 0; ii< NUM_COLOURS; ii++)
    read(f, palette[ii], 3);
  close(f);

  if(3 != argc) {
    printf("Usage: %s in_data_file out_rgb_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  int fd;
  fd = open(argv[1], O_RDONLY);
  assert(-1 != fd);
  
  struct stat fs;
  fstat(fd, &fs);

  puts("mmapping file sharing is caring!");
  unsigned char *jd = (unsigned char *) mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(MAP_FAILED == jd) {
    perror("mmap jd");
  }
  madvise(jd, fs.st_size, MADV_SEQUENTIAL);
  assert(-1 != close(fd));

  puts("Done.\nmalloc");
  char *bmpRGB = (char *) malloc(fs.st_size*3);
  if(NULL == bmpRGB)
    perror("malloc bmpRGB");
  
  puts("Done.\nColourising...");
  int i;
  for(i = 0; fs.st_size > i; ++i) {
    size_t j = (jd[i] / (256/NUM_COLOURS)) % NUM_COLOURS;
    memcpy(&bmpRGB[3 * i], palette[j], 3);
  }
  munmap(jd, fs.st_size);
  
  puts("Done.\nWriting output file...");  
  fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
  assert(-1 != fd);
  write(fd, bmpRGB, 3 * fs.st_size);
  assert(-1 != close(fd));
}
