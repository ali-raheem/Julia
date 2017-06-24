#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char palette[16][4] = {"\x00\x00\x00",
			 "\x10\x10\x10",
			 "\x20\x20\x20",
			 "\xBE\x26\x33",
			 "\xE0\x6F\x8B",
			 "\x49\x3C\x2B",
			 "\xA4\x64\x22",
			 "\xEB\x89\x31",
			 "\xF7\xE2\x6B",
			 "\x2F\x48\x4E",
			 "\x44\x89\x1a",
			 "\xA3\xce\x27",
			 "\x1B\x26\x32",
			 "\x00\x57\x84",
			 "\x31\xa2\xf2",
			 "\xB2\xDC\xEF"};
  if(3 != argc) {
    printf("Usage: %s in_data_file out_rgb_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int fd;
  fd = open(argv[1], O_RDONLY);
  assert(-1 != fd);

  struct stat fs;
  fstat(fd, &fs);

  puts("mmapping file.");
  char *jd;
  jd = mmap(NULL, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
  assert(MAP_FAILED != jd);
  assert(-1 != close(fd));
  
  puts("malloc");
  char *bmpRGB = (char *) malloc(fs.st_size*3);
  assert(NULL != bmpRGB);
  
  puts("Colourising");
  size_t i;
  for(i = 0; fs.st_size > i; ++i) {
    int j = (jd[i] / 16) % 16;
    memcpy(&bmpRGB[3 * i], palette[j], 3);
  }
  
  munmap(jd, fs.st_size);  
  puts("Writing output file");

  fd = open(argv[2], O_WRONLY|O_CREAT);
  assert(-1 != fd);
  write(fd, bmpRGB, 3 * fs.st_size);
  close(fd);
}
