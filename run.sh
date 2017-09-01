#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

wavfile="$1"
base=$(basename "$1" .wav)
path=$(dirname "$1")
outwav="${path}/auto_${base}.wav"
gestures="${path}/gestures_${base}.csv"
gtes="${path}/gtes_${base}.csv"
echo "converting to 44.1kHz"
sox "${wavfile}" -r 44.1k resample.wav
echo "saving data values"
./wta resample.wav > example.dat
echo "computing fundamental frequency"
./computeFF $(wc -l example.dat) example.dat > r3.I.dat
echo "smoothing fundamental frequency"
./smoothFF r3.I.dat > r3.II.dat
echo "synthesizing song"
./synthesize r3.II.dat r3.III.dat env_example.dat "${gestures}"
echo "gestures saved to ${gestures}"
echo "saving a WAV file"
./dat2wav song_r3.III.dat "${outwav}" 44100
echo "resampling to 40kHz"
sox "${wavfile}" -r 40k resample40.wav
echo "computing GTEs, saving to ${gtes}"
./wta resample40.wav > example40.dat
./gtes_example example40.dat "${gtes}"
