#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#define HEIGHT 8000
#define WIDTH 8000
#define SCALE 1.0
#define ITERATIONS 200
#define THRESH 1000
#define C -0.8 + 0.156 * I;

int isJulia(float x, float y) {
  float complex z;
  z = x + y * I;
  int i;
  for(i = 0; ITERATIONS > i; ++i) {
    z = cpowf(z, 2) + C;
  }
  if(THRESH < cabsf(z))
    return 0;
  return 1;
}

int main() {
  char *data;
  data = (char *) malloc (WIDTH * HEIGHT);
  assert(NULL != data);

  int x, y;
#pragma omp parallel for private (y) collapse (2)
  for(x = 0; WIDTH > x; ++x) {
    for(y = 0; HEIGHT > y; ++y) {
      float sx, sy;
      sx = SCALE * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALE * (HEIGHT/2 - y) / (HEIGHT/2);
      data[x + y*WIDTH] = isJulia(sx, sy);
    }
  }
  
  FILE *fp;
  fp = fopen("julia.data", "wb");
  fwrite(data, WIDTH * HEIGHT, 1, fp);
  fclose(fp);
}
