cc = gcc

programs = wta computeFF smoothFF synthesize dat2wav gtes_example

.PHONY: all clean
	

all : $(programs) 
	

clean : 
	rm -f $(programs)
	rm -f auto_*.csv
	rm -f gtes_*.csv
	rm -f gestures_*.csv
	rm -f r3.*.dat
	rm -f gtes*exampleGTE.dat
	rm -f resample*wav
	rm -f song_r3*
	rm -f env_example.dat
	rm -f example.dat
	rm -f example40.dat

computeFF : computeFF.c realft.c four1.c nrutil.c
	$(cc) $< -lm -O3 -o $@ $(wordlist 2, 4, $^)

synthesize : synthesize.c rk4.c
	$(cc) $^ -lm -O3 -o $@

dat2wav : dat2wav.c
	$(cc) -std=c99 $^ -lm -O3 -o $@

% : %.c
	$(cc) $< -lm -O3 -o $@
