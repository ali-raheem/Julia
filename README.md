# Julia set generator

Quick and dirty Julia set generator.

## Sample output

![Default image](sample.png)

## Compilation

```
$ gcc -lm -lpthread -O3 -o julia julia-pthread.c
```

If you can use pthreads make sure to use this one as it's much faster. The python version is of course rather slow since it does no threading.

Make sure to update all files for height and width if you use them otherwise you will end up with tearing in the output.

## Usage

```
$ ./julia
$ ./jd2png.py
$ eog output.png
```
