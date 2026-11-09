#!/bin/bash
BEST=./bestseq/build/bestseq
PAR=./parallel/build/parallel
fail=0
for img in ImageProcessing/images/aya.pgm largerImages/aya1024.pgm largerImages/aya2048.pgm largerImages/aya4096.pgm; do
  for op in gaussFilter otsuBinarize sobelFilter; do
    $BEST $op "$img" > /dev/null && mv output.pgm /tmp/ref.pgm
    for t in 1 2 3 4 5 6 7 8 9 10; do
      OMP_NUM_THREADS=$t $PAR $op "$img" > /dev/null && mv output.pgm /tmp/par.pgm
      cmp -s /tmp/ref.pgm /tmp/par.pgm || { echo "DIFFERENT: $op $img threads=$t"; fail=1; }
    done
    echo "$op $(basename "$img"): checked 1-10 threads"
  done
done
[ $fail = 0 ] && echo "ALL IDENTICAL" || echo "SOME DIFFERENT"