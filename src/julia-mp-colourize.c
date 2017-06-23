#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <omp.h>
#include <openacc.h>

#define HEIGHT 1000
#define WIDTH 1000
#define SCALE 1.0
#define ITERATIONS 255
#define THRESH 1000
#define C -0.8 + 0.156 * I;

int isJulia(float x, float y) {
  float complex z;
  int i;
  z = x + y * I;
  for(i = 0; ITERATIONS > i; ++i) {
    z = cpowf(z, 2) + C;
    if(THRESH < cabsf(z))
      return i;
  }
  return 0xFF;

}

int main() {
  char *data;
  data = (char *) malloc (WIDTH * HEIGHT);
  assert(NULL != data);

  int x, y;
#pragma omp parallel for private (y) collapse (2)
#pragma acc loop collapse (2)
  //#pragma omp target teams distribute 
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
