#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define HEIGHT 900
#define WIDTH 900
#define SCALE 1.5
#define ITERATIONS 200 //20 //15
#define THRESH 1000
#define C -0.8 + 0.156 * I;
#define COLOUR1 "\x44\x66\xed"
#define COLOUR2 "\xf9\xfc\x10"

int isJulia(double x, double y) {
  double complex z, c;
  c = C;
  z = x + y * I;
  int i;
  for(i = 0; ITERATIONS > i; ++i) {
    z = cpow(z, 2) + c;
  }
  if(THRESH < cabs(z))
    return 0;
  return 1;
}

int main() {
  void *data;
  data = (void *) malloc (WIDTH * HEIGHT * 3);
  assert(NULL != data);

  int x, y;
  int offset;
  double sx, sy;
  for(x = 0; WIDTH > x; ++x) {
    for(y = 0; HEIGHT > y; ++y) {
      sx = SCALE * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALE * (HEIGHT/2 - y) / (HEIGHT/2);
      if(isJulia(sx, sy))
        memcpy(data + offset,
               COLOUR1, 3);
      else
        memcpy(data + offset,
               COLOUR2, 3);
      offset += 3;
    }
  }
  FILE *fp;
  fp = fopen("julia.data", "wb");
  fwrite(data, 3 * WIDTH * HEIGHT, 1, fp);
  fclose(fp);
}
