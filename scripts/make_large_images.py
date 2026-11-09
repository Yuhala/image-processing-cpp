#!/usr/bin/env python3
"""
make_large_images.py - creates the large benchmark images in largerImages/
(aya1024/2048/4096.pgm and bella1024/2048/4096.pgm) by upscaling the small
sample images. These files are too big to keep in git (up to ~60 MB each).

usage (from the repo root):
  python3 scripts/make_large_images.py            # all sizes
  python3 scripts/make_large_images.py 1024 2048  # only these sizes

Uses nearest-neighbour scaling and only the Python standard library. The
original benchmark images were scaled in GIMP, so pixel values differ slightly,
but the size and format match. Timings are comparable; the reference outputs in
results/ref/ will NOT match, so delete them and check_and_time.sh will
regenerate them from the baseline.
"""
import os, sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
SRC_DIR = os.path.join(ROOT, "parallel", "images")
OUT_DIR = os.path.join(ROOT, "largerImages")
SOURCES = ["aya", "bella"]
SIZES = [int(s) for s in sys.argv[1:]] or [1024, 2048, 4096]
PER_LINE = 16  # values per text line (keeps lines under 70 chars, as PGM recommends)


def read_pgm(path):
    """Read a P2 (text) PGM the same way the program does:
    line 1 = magic, line 2 = comment, then width height maxval pixels."""
    with open(path) as f:
        magic = f.readline().strip()
        if magic != "P2":
            sys.exit(f"{path}: expected a P2 (text) PGM, got {magic!r}")
        f.readline()  # comment line
        nums = list(map(int, f.read().split()))
    w, h, maxval = nums[0], nums[1], nums[2]
    px = nums[3:3 + w * h]
    if len(px) != w * h:
        sys.exit(f"{path}: expected {w * h} pixels, found {len(px)}")
    return w, h, maxval, [px[r * w:(r + 1) * w] for r in range(h)]


def write_scaled(src_w, src_h, maxval, rows, size, path):
    col_map = [c * src_w // size for c in range(size)]
    cache = {}  # nearest-neighbour: each output row is a copy of some source row
    with open(path, "w") as f:
        f.write(f"P2\n# CREATOR: make_large_images.py\n{size} {size}\n{maxval}\n")
        for r in range(size):
            sr = r * src_h // size
            text = cache.get(sr)
            if text is None:
                vals = [str(rows[sr][c]) for c in col_map]
                text = "".join(" ".join(vals[i:i + PER_LINE]) + "\n"
                               for i in range(0, size, PER_LINE))
                cache[sr] = text
            f.write(text)


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    for name in SOURCES:
        w, h, maxval, rows = read_pgm(os.path.join(SRC_DIR, name + ".pgm"))
        for size in SIZES:
            out = os.path.join(OUT_DIR, f"{name}{size}.pgm")
            write_scaled(w, h, maxval, rows, size, out)
            print(f"wrote {os.path.relpath(out, ROOT)}  ({size}x{size})")


if __name__ == "__main__":
    main()
