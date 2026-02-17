#!/bin/bash
NP=6
CIPHERTEXT="ciphertext.txt"
DICTIONARY="american-english"
OUTPUT="case5p.txt"
ITERATIONS=40

# Clear previous output
> $OUTPUT

for i in $(seq 1 $ITERATIONS)
do
    mpiexec -n $NP ./a2decrypt "$CIPHERTEXT" "$DICTIONARY" >> $OUTPUT
done

echo "Finished $ITERATIONS runs, output saved to $OUTPUT"