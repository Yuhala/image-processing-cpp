#!/bin/bash
# usage: bash scripts/sweep.sh results/sweep.csv
CSV=${1:-results/sweep.csv}
BEST=./bestseq/build/bestseq
PAR=./parallel/build/parallel
REPS=10
cmake --build bestseq/build || exit 1
cmake --build parallel/build || exit 1

echo "impl,image,op,threads,rep,read_ms,compute_ms,write_ms,total_ms" > "$CSV"

run() {   # args: impl binary threads op image rep
  line=$(OMP_NUM_THREADS=$3 IMG_TIMING=1 $2 $4 "largerImages/$5" 2>&1 >/dev/null | grep TIMING)
  if [ -z "$line" ]; then echo "No TIMING: $1 threads=$3 $4 $5"; exit 1; fi
  echo "$line" | sed -E "s/TIMING threads=[0-9]+ read_ms=([0-9.]+) compute_ms=([-0-9.]+) write_ms=([0-9.]+) total_ms=([0-9.]+)/$1,$5,$4,$3,$6,\1,\2,\3,\4/" >> "$CSV"
}

for img in aya1024.pgm aya2048.pgm aya4096.pgm; do
  for op in gaussFilter otsuBinarize sobelFilter; do
    $BEST $op "largerImages/$img" > /dev/null 2>&1                     # warm-up
    for r in $(seq 1 $REPS); do run bestseq $BEST 1 $op $img $r; done
    for t in 1 2 3 4 5 6 7 8 9 10; do
      OMP_NUM_THREADS=$t $PAR $op "largerImages/$img" > /dev/null 2>&1 # warm-up
      for r in $(seq 1 $REPS); do run parallel $PAR $t $op $img $r; done
    done
    echo "$img $op done"
  done
done
echo "DONE"