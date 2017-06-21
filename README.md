# Julia set generator

Quick and dirty Julia set (and mandelbrot) generator.

## Sample output

![Sample Julia output](samples/julia-64x48.png)
![Sample Mandelbrot output](samples/mandelbrot-64x48.png)

## Compilation

```
$ gcc -O3 -o julia julia.c -lm -lpthread
$ gcc -O3 -o mandelbrot mandelbrot.c -lm -lpthread
```

It's exceptionally multithreadable.

If you can use pthreads make sure to use this one as it's much faster. The python version is of course rather slow since it does no threading.

Make sure to update all files for height and width if you use them otherwise you will end up with tearing in the output.

Colors are set in data2img.sh in the sed command.

## Dependencies

* pthread
* getopt
* imagemagick or python with PIL


## Usage

```
$ ./bin/julia
$ ./bin/jd2png.py
$ eog julia.png
```

or

```
$ ./bin/julia
$ ./bin/data2img.sh julia
$ eog julia.png
```
