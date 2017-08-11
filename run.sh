./wta example.wav > example.dat
./computeFF 66932 example.dat > r3.I.dat
./smoothFF r3.I.dat > r3.II.dat
./synthesize r3.II.dat r3.III.dat env_example.dat
./dat2wav song_r3.III.dat song_r3.III.wav 44100