#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#define HEIGHT 1080
#define WIDTH 1920
#define SCALEY 1.0
#define SCALEX 2.0
#define ITERATIONS 200 //20 //15
#define THRESH 1000

#define NUM_THREADS 12

typedef struct {
  int i;
  void *data;
} targs_t;


int isJulia(const float x, const float y) {
  float complex z;
  float complex c;
  z = 0;
  c = x + y * I;
  int i;
  for(i = 0; ITERATIONS > i; ++i) {
    z = cpowf(z, 2) + c;
  }
  if(THRESH < cabsf(z))
    return 0;
  return 1;
}

void *thread(void *args) {
  int x, y;
  targs_t *targs = args;
  void *data = targs->data;
  int offset = 0;
  float sx, sy;
  for(x = 0; WIDTH > x; ++x) {
    for(y = targs->i; HEIGHT > y; y += NUM_THREADS) {
      sx = SCALEX * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALEY * (HEIGHT/2 - y) / (HEIGHT/2);
      if(isJulia(sx, sy))
        memcpy(data + offset,
               "\x01", 1);
      else
        memcpy(data + offset,
               "\x00", 1);
      offset = (x + y * WIDTH);
    }
  }
}
int main(int argc, char** argv) {
  int f;
  char *outfile = strdup("julia.data");
  while (-1 != (f = getopt (argc, argv, "o:")))
    switch (f) {
    case 'o':
      outfile = strdup(optarg);
      break;
  }
  void *data;
  data = (void *) malloc (WIDTH * HEIGHT);
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
  assert(NULL != fp);
  fwrite(data, WIDTH * HEIGHT, 1, fp);
  fclose(fp);
}
