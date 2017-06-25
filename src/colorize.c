#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  char palette[16][4] = {"\x01\x01\x0f",
                         "\x01\x01\x1f",
			 "\x01\x01\x2f",
			 "\x01\x01\x3f",
			 "\x01\x01\x4f",
			 "\x01\x01\x5f",
			 "\x01\x01\x6f",
			 "\x01\x01\x7f",
			 "\x01\x01\x8f",
			 "\x01\x01\x9f",
			 "\x01\x01\xaf",
			 "\x01\x01\xbf",
			 "\x01\x01\xcf",
			 "\x01\x01\xdf",
			 "\x01\x01\xef",
			 "\x01\x01\xff"};
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
  assert(MAP_FAILED != jd);
  madvise(jd, fs.st_size, MADV_SEQUENTIAL);

  assert(-1 != close(fd));

  puts("Done.\nmalloc");
  char *bmpRGB = (char *) malloc(fs.st_size*3);
  assert(NULL != bmpRGB);
  
  puts("Done.\nColourising...");
  int i;
  for(i = 0; fs.st_size > i; ++i) {
    size_t j = (jd[i] / 16) % 16;
    memcpy(&bmpRGB[3 * i], palette[j], 3);
  }
  munmap(jd, fs.st_size);
  
  puts("Done.\nWriting output file...");  
  fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
  assert(-1 != fd);
  write(fd, bmpRGB, 3 * fs.st_size);
  assert(-1 != close(fd));
}
