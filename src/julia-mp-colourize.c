#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <complex.h>
#include <openacc.h>

#define HEIGHT 1080*4
#define WIDTH 1920*4
#define SCALEX 2.0
#define SCALEY 1.0
#define ITERATIONS 255
#define THRESH 1000
#define C -0.4 + 0.6 * I;
#define PI 3.1415
#define PHI 0.5*PI

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
#pragma acc paralel
{
#pragma omp parallel for private (y) collapse (2)
#pragma acc loop independent
  for(x = 0; WIDTH > x; ++x) {
    #pragma acc loop independent
    for(y = 0; HEIGHT > y; ++y) {
      float sx, sy, ssx, ssy;
      ssx = SCALEX * (WIDTH/2 - x) / (WIDTH/2);
      ssy = SCALEY * (HEIGHT/2 - y) / (HEIGHT/2);
      sx = ssx * cosf(PHI) - ssy * sinf(PHI);
      sy = ssx * sinf(PHI) + ssy * cosf(PHI);
      data[x + y*WIDTH] = isJulia(sx, sy);
    }
  }
}
  FILE *fp;
  fp = fopen("julia.data", "wb");
  fwrite(data, WIDTH * HEIGHT, 1, fp);
  fclose(fp);
}
