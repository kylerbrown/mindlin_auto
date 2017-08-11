cc = gcc

programs = wta computeFF smoothFF synthesize dat2wav

.PHONY: all clean
	

all : $(programs) 
	

clean : 
	rm -f $(programs)
	

computeFF : computeFF.c realft.c four1.c nrutil.c
	$(cc) $< -lm -o $@ $(wordlist 2, 4, $^)

synthesize : synthesize.c rk4.c
	$(cc) $^ -lm -o $@

dat2wav : dat2wav.c
	$(cc) -std=c99 $^ -lm -o $@

% : %.c
	$(cc) $< -lm -o $@
