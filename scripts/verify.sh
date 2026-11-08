#!/bin/bash
# verify.sh - checks that the parallel build produces byte-identical results
# (output.pgm AND printed text) to the sequential baseline, for every operation,
# at several thread counts.
#
# img1 and img2 must be the SAME size (two-image operations crash otherwise).
# Prints "verify: N identical, 0 mismatched" when everything matches.

if [ $# -lt 4 ]; then
  echo "usage: $0 <baseline_binary> <parallel_binary> <img1.pgm> <img2.pgm> [threads...]"
  exit 1
fi

abspath() { echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"; }
BASE=$(abspath "$1"); PAR=$(abspath "$2"); A=$(abspath "$3"); B=$(abspath "$4")
shift 4
THREADS="$*"
[ -z "$THREADS" ] && THREADS="1 2 4 8"

for f in "$BASE" "$PAR" "$A" "$B"; do
  [ -e "$f" ] || { echo "not found: $f"; exit 1; }
done

TMP=$(mktemp -d)
pass=0; fail=0

# run <binary> <threads> <folder> <args...>  - runs in its own folder so output.pgm doesn't clash
run() {
  local bin=$1 t=$2 d=$3; shift 3
  rm -rf "$d"; mkdir -p "$d"
  (cd "$d" && OMP_NUM_THREADS=$t "$bin" "$@" > stdout.txt 2>/dev/null)
}

# check <label> <args...>  - baseline once, parallel at each thread count, compare
check() {
  local label=$1; shift
  run "$BASE" 1 "$TMP/base" "$@"
  if [ ! -s "$TMP/base/output.pgm" ]; then
    fail=$((fail + 1))
    echo "FAIL: $label - the BASELINE wrote no output.pgm. Its output was:"
    head -5 "$TMP/base/stdout.txt" | sed 's/^/      /'
    return
  fi
  for t in $THREADS; do
    run "$PAR" "$t" "$TMP/par" "$@"
    if [ ! -s "$TMP/par/output.pgm" ]; then
      fail=$((fail + 1))
      echo "FAIL: $label threads=$t - the PARALLEL build wrote no output.pgm. Its output was:"
      head -5 "$TMP/par/stdout.txt" | sed 's/^/      /'
    elif ! cmp -s "$TMP/base/output.pgm" "$TMP/par/output.pgm"; then
      fail=$((fail + 1)); echo "MISMATCH: $label threads=$t - output.pgm differs"
    elif ! cmp -s "$TMP/base/stdout.txt" "$TMP/par/stdout.txt"; then
      fail=$((fail + 1)); echo "MISMATCH: $label threads=$t - printed text differs:"
      diff "$TMP/base/stdout.txt" "$TMP/par/stdout.txt" | head -4 | sed 's/^/      /'
    else
      pass=$((pass + 1))
    fi
  done
  echo "  checked $label"
}

for op in not otsuBinarize linearContrast histogramEquilization sobelFilter prewittFilter \
          gaussFilter smoothingFilter robertFilter laplacienConvo sharpen edgeDetect \
          mirror erosion printHistogram luminance calculateContrast; do
  check "$op" "$op" "$A"
done
for op in and nand or xor add sub; do
  check "$op" "$A" "$op" "$B"
done
check "mult 2" mult 2 "$A"
check "linearContrastSaturation" linearContrastSaturation "$A" 100 150

echo "verify: $pass identical, $fail mismatched  (threads tested: $THREADS)"
rm -rf "$TMP"
[ $fail -eq 0 ]