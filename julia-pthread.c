#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define HEIGHT 800
#define WIDTH 800
#define SCALE 1.5
#define ITERATIONS 200 //20 //15
#define THRESH 1000
#define C -0.8 + 0.156 * I;
#define COLOUR1 "\x44\x66\xed"
#define COLOUR2 "\xf9\xfc\x10"

#define NUM_THREADS 4

typedef struct {
  int i;
  void *data;
} targs_t;

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

void *thread(void *args) {
  int x, y;
  targs_t *targs = args;
  void *data = targs->data;
  int offset = 0;
  double sx, sy;
  for(x = 0; WIDTH > x; ++x) {
    for(y = targs->i; HEIGHT > y; y += NUM_THREADS) {
      sx = SCALE * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALE * (HEIGHT/2 - y) / (HEIGHT/2);
      if(isJulia(sx, sy))
        memcpy(data + offset,
               COLOUR1, 3);
      else
        memcpy(data + offset,
               COLOUR2, 3);
      offset = 3 * (x + y * WIDTH);
    }
  }
}
int main() {
  void *data;
  data = (void *) malloc (WIDTH * HEIGHT * 3);
  assert(NULL != data);

  targs_t targs[NUM_THREADS];
  pthread_t tid[NUM_THREADS];
  int i;
  for(i = 0; NUM_THREADS > i; ++i) {
    targs[i].i = i;
    targs[i].data = data;
    pthread_create(&tid[i], NULL, thread, &targs[i]);
  }

  for(i = 0; NUM_THREADS > i; ++i)
    pthread_join(tid[i], NULL);

  FILE *fp;
  fp = fopen("julia.data", "wb");
  fwrite(data, 3 * WIDTH * HEIGHT, 1, fp);
  fclose(fp);
}
