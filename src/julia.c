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
#define SCALE 1.0
#define ITERATIONS 150 //20 //15
#define THRESH 100
#define C -0.8 + 0.156 * I;

#define NUM_THREADS 12

typedef struct {
  int i;
  void *data;
} targs_t;

static complex float c;
float scale;

int isJulia(const float x, const float y) {
  float complex z;
  z = x + y * I;
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
      sx = SCALE * (WIDTH/2 - x) / (WIDTH/2);
      sy = SCALE * (HEIGHT/2 - y) / (HEIGHT/2);
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
  int f, r_flag = 0, i_flag = 0;
  float real, imag;
  char *outfile = strdup("julia.data");
  //  static complex float c;
  while (-1 != (f = getopt (argc, argv, "o:i:r:")))
    switch (f) {
    case 'o':
      outfile = strdup(optarg);
      break;
    case 'r':
      real = strtof(optarg, NULL);
      r_flag = 1;
      break;
    case 'i':
      imag = strtof(optarg, NULL);
      i_flag = 1;
      break;
    }
  if (r_flag && i_flag) {
    c = real + imag * I;
    printf("C is %f + %f*i\n", real, imag);
  }else{
    c = C;
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
