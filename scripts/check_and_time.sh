#!/bin/bash
# usage: bash scripts/check_and_time.sh results/<name>.csv
CSV=$1
BIN=${2:-./bestseq/build/bestseq}
cmake --build "$(dirname "$BIN")" || exit 1
echo "Testing: $BIN"

mkdir -p results/ref
for op in gaussFilter otsuBinarize sobelFilter; do
  ref=results/ref/$op.pgm
  if [ ! -f "$ref" ]; then                      # make reference outputs once, from the original
    ./baseline/build/baseline $op largerImages/aya1024.pgm > /dev/null && mv output.pgm "$ref"
  fi
  $BIN $op largerImages/aya1024.pgm > /dev/null && mv output.pgm /tmp/out_best.pgm
  if cmp -s "$ref" /tmp/out_best.pgm; then echo "$op: IDENTICAL"
  else echo "$op: DIFFERENT - stopping"; exit 1; fi
done

echo "image,op,rep,read_ms,compute_ms,write_ms,total_ms" > "$CSV"
for img in aya1024.pgm aya2048.pgm aya4096.pgm; do
  for op in gaussFilter otsuBinarize sobelFilter; do
    for r in 1 2 3 4 5 6 7 8 9 10; do
      line=$(IMG_TIMING=1 $BIN $op "largerImages/$img" 2>&1 >/dev/null | grep TIMING)
      [ -z "$line" ] && { echo "No TIMING for $op $img"; exit 1; }
      echo "$line" | sed -E "s/TIMING threads=[0-9]+ read_ms=([0-9.]+) compute_ms=([-0-9.]+) write_ms=([0-9.]+) total_ms=([0-9.]+)/$img,$op,$r,\1,\2,\3,\4/" >> "$CSV"
    done
  done
  echo "$img done"
done
echo "DONE"