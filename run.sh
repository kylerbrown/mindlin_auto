#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

wavfile="$1"
base="${wavfile%.*}"
outwave="auto_$1"
gestures="gestures_${base}.csv"
echo "${wavfile} ${outwave} ${gestures}"
sox "${wavfile}" -r 44.1k resample.wav
./wta resample.wav > example.dat
./computeFF $(wc -l example.dat) example.dat > r3.I.dat
./smoothFF r3.I.dat > r3.II.dat
./synthesize r3.II.dat r3.III.dat env_example.dat
echo "hello"
./dat2wav song_r3.III.dat "auto_${base}.wav" 44100
