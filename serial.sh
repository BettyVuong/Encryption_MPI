#!/bin/bash
CIPHERTEXT="ciphertext.txt"
DICTIONARY="american-english"
OUTPUT="case4s.txt"
ITERATIONS=40

# Clear previous output
> $OUTPUT

for i in $(seq 1 $ITERATIONS)
do
    ./a2decrypt_serial "$CIPHERTEXT" "$DICTIONARY" >> $OUTPUT
done

echo "Finished $ITERATIONS runs, output saved to $OUTPUT"