#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>

#define HEIGHT 900
#define WIDTH 900
#define SCALE 1.5
#define ITERATIONS 200 //20 //15
#define THRESH 1000
#define C -0.8 + 0.156 * I;

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
  int x, y;
  double sx, sy;
  FILE *fp;
  fp = fopen("julia.data", "wb");
  for(x = 0; WIDTH > x; ++x) {
    for(y = 0; HEIGHT > y; ++y) {
      sx = SCALE * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALE * (HEIGHT/2 - y) / (HEIGHT/2);
      if(isJulia(sx, sy))
        fwrite("\x44\x66\xed", 3, 1, fp);
      else
        fwrite("\xf9\xfc\x10", 3, 1, fp);
    }
  }
  fclose(fp);
}
