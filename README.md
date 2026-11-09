# Parallel Image Processing (C++ / OpenMP)

A CAB401 project that parallelises an existing sequential image-processing
program with OpenMP. The program applies filters and operations (Gaussian blur,
Sobel edge detection, Otsu thresholding, logic and arithmetic on images, and
more) to greyscale `.pgm` images.

It is based on [image-processing-cpp](https://github.com/Yuhala/image-processing-cpp)
by Peterson Yuhala (MIT licence).

## Repository layout

| Folder | What it is |
|---|---|
| `baseline/` | The original sequential program, with timing added. Results are checked against this version. |
| `bestseq/` | Optimised sequential version: faster parsing and output, no threads. |
| `parallel/` | **The OpenMP version.** Same source as `bestseq/`, with the parallel regions added. |
| `ImageProcessing/` | The untouched upstream code, kept for reference. |
| `scripts/` | Build, correctness and benchmarking scripts. |
| `results/` | Measured timings (CSV) and speedup graphs. |

Each program folder has its own `images/` folder of small sample images.

## Requirements

- A C++17 compiler with OpenMP
  - **macOS:** Apple clang (Xcode Command Line Tools) plus `brew install libomp`
  - **Linux:** `g++` (OpenMP is built in) or `clang++` with `libomp-dev`
- CMake 3.16 or newer (`brew install cmake` / `sudo apt install cmake`)
- Optional, for the graphs: Python 3 with `matplotlib` (`pip3 install matplotlib`)

Windows has not been tested. WSL with the Linux steps should work.

## Building

Using CMake (the benchmark scripts expect this layout):

```bash
git clone <this repo> && cd CAB401-image-processing-cpp
for v in baseline bestseq parallel; do
  cmake -S $v -B $v/build -DCMAKE_BUILD_TYPE=Release && cmake --build $v/build
done
```

This produces `baseline/build/baseline`, `bestseq/build/bestseq` and
`parallel/build/parallel`.

Or, without CMake, run `bash scripts/build.sh` to build `app_baseline`,
`app_bestseq` and `app_parallel` in the repo root.

## Running

```bash
cd parallel
./build/parallel gaussFilter images/lena.pgm          # writes output.pgm
OMP_NUM_THREADS=4 ./build/parallel sobelFilter images/aya.pgm
IMG_TIMING=1 ./build/parallel otsuBinarize images/lena.pgm   # also prints timings to stderr
./build/parallel                                       # lists every operation
```

- The result is always written to **`output.pgm` in the current directory**,
  overwriting any previous one.
- `OMP_NUM_THREADS` sets the thread count (default: all cores).
- `IMG_TIMING=1` prints one line to stderr:
  `TIMING threads=… read_ms=… compute_ms=… write_ms=… total_ms=…`
- Input must be a text (P2) PGM whose **second line is a comment**, which is
  the format GIMP writes. The program assumes this layout.

### Operations

| Form | Operations |
|---|---|
| `./app <op> <image>` | `not` `otsuBinarize` `linearContrast` `histogramEquilization` `sobelFilter` `prewittFilter` `gaussFilter` `smoothingFilter` `robertFilter` `laplacienConvo` `sharpen` `edgeDetect` `mirror` `erosion` |
| `./app <op> <image>` (prints a value, no image) | `printHistogram` `luminance` `calculateContrast` |
| `./app <image1> <op> <image2>` (images must be the same size) | `and` `nand` `or` `xor` `add` `sub` |
| `./app mult <factor> <image>` | multiply every pixel by `factor` |
| `./app linearContrastSaturation <image> <smin> <smax>` | contrast stretch with saturation |

To view a `.pgm`, open it in GIMP or Preview, or convert it with ImageMagick:
`magick output.pgm output.png`.

## Checking correctness

The parallel version must produce **byte-identical** output to the original.
To check every operation at several thread counts:

```bash
bash scripts/build.sh
bash scripts/verify.sh ./app_baseline ./app_parallel \
     parallel/images/lena.pgm parallel/images/aya.pgm 1 2 4 8
# -> verify: 75 identical, 0 mismatched
```

## Benchmarking

The benchmarks use 1024², 2048² and 4096² images. They are too large for git,
so generate them first (this takes a few seconds and uses only the Python
standard library):

```bash
python3 scripts/make_large_images.py      # writes largerImages/
```

Then, from the repo root, after the CMake build above:

```bash
bash scripts/verify_threads.sh            # parallel == bestseq for 1–10 threads
bash scripts/sweep.sh results/sweep.csv   # 10 reps × 1–10 threads × 3 sizes × 3 ops
python3 scripts/plot_sweep.py results/sweep.csv   # -> results/speedup_*.png, sweep_summary.csv
```

`scripts/benchmark.py` is a more flexible runner. See the docstring at the top
of the file for its options.

Notes:
- The generated images are scaled with nearest-neighbour rather than GIMP's
  interpolation, so they are the same size and format but not identical
  pixel-for-pixel. If you regenerate them, delete `results/ref/` so
  `scripts/check_and_time.sh` rebuilds its reference outputs from the baseline.
- The results in `results/` were measured on an Apple M2 Pro (10 cores).
  Expect different numbers on other machines.

## Known limitations (inherited from the original program)

- Two-image operations crash if the images are different sizes.
- `scalingNN` is not implemented correctly in the original code. It is left
  unchanged so the outputs still match the baseline.
- Only P2 (text) PGM files with a comment on line 2 are read correctly.

## Licence

MIT. See [LICENSE](LICENSE). The original code is © 2018 Peterson Yuhala.
