# Julia set generator

Quick and dirty Julia set (and mandelbrot) generator.

## Theory

We look at complex quadratic polynomials of the form F(z) = z^2 + c where z is a complex parameter and c is a complex constant.

For Julia we use pick a constant c and create an image of where (x, y) are the real and iamginary parts of z, the pixel is coloured based on where interation of F(z) seems bound (the magnetude of z is less than THRESH after ITERATIONS iterations).

For Mandelbrot we set z = 0 and (x, y) become the real and imaginary parts of C again we look for the iterations to blow up.

## Sample output

### Julia

Default is F(z) = z -0.8 + 0.156i (suggested on wikipedia and CUDA by example).

![Sample Julia output for -0.8 + 0.156i](samples/julia-sample.png)

### Mandelbrot
![Sample Mandelbrot output](samples/mandelbrot-sample.png)

## Compilation

```
$ gcc -O3 -o julia julia.c -lm -lpthread
$ gcc -O3 -o mandelbrot mandelbrot.c -lm -lpthread
```

It's exceptionally multithreadable. By default it creates 12 threads. Most settings can only and should be set in the code, on my aging i5 laptop even having command line scaling (a single FP calulation per pixel) adds 10% overhead on some settings...

The python version is of course rather slow since it does no threading.

Make sure to update all files for height and width if you use them otherwise you will end up with tearing in the output.

Colors are set in data2img.sh in the sed command.

## Dependencies

* pthread
* getopt
* imagemagick or python with PIL


## Usage

### Mandelbrot

```
$ ./bin/mandelbrot
$ ./bin/data2img.sh julia
$ eog julia.png
```

### Julia

```
$ ./bin/julia
$ ./bin/jd2png.py
$ eog julia.png
```

### Slow python version

```
$ ./bin/julia.py
```

#### References and Reading

* https://developer.nvidia.com/cuda-example
* https://en.wikipedia.org/wiki/Julia_set
* https://en.wikipedia.org/wiki/Mandelbrot_set
