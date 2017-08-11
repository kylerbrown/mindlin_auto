Updates Kyler Brown August 2017
	No changes to fundamental algorithms, just a few conviences:
	+ A Makefile
	+ Runs both synthesis and GTE extraction
	+ Outputs CSVs for both the GTEs and the gestures themselves
	+ Resamples as appropriate via SoX (synthesis requires 44.1kHz, GTE extraction uses 40kHz)
	

Requirements:
+ Gnu C compliler (gcc)
+ SoX

Setup:
	Compile the code with
	$ make

Usage:
	$ ./run.sh FILE.wav
	(you must run the script from within the code folder)

Output files:
	+ auto_FILE.wav -- the synthetic song
	+ gestures_FILE.csv -- the alpha and beta forcing parameters that generate the sound
	+ gtes_FILE.csv -- the times of the GTEs and their identity (see below and the paper for details)



~~~ original Readme.txt ~~~
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Birdsong synthesis software suite %
%          September 2015           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

README and usage instructions:

-For an automated compilation of all programs, use the provided makefile (./makefile).
-For the example file, you can use the provided script (‘run.sh’) afterwards (sh run.sh).
Example file provided: example.wav (Original recording). Output: song_r3.III.wav (Synthetic copy).

-Following steps compile and run the required programs.


Files required for synthesis: 
- A song recording, sampled at 44.1 kHz (i.e.: example.wav)

1. Convert from WAV file to .dat ASCII
Note that you can convert to .dat on your own, but here we provide a code that does that. 
- Compile ‘wta.c’ (wav-to-ascii) in terminal, by typing:
cc wta.c -lm -o wta
- Run it by giving the input file and the output file name:
./wta example.wav > example.dat

2. Compute Fundamental Frequency (FF) time trace from data.
- Compile ‘computeFF.c’ in terminal, by typing:
gcc computeFF.c -lm -o computeFF realft.c  four1.c nrutil.c
- In order to compute FF, the line count of the .dat file must be given as input to compute_FF. You can get this information in terminal by typing:
cat example.dat | wc -l
- Line count should be 66932 for the example file.
- Then, to compute FF use the following command:
./computeFF 66932 example.dat  > r3.I.dat
- This generates the file ‘r3.I.dat’, which contains the FF computation.

3. Smooth the FF time trace.
- Compile ‘smoothFF.c’ in terminal, by typing:
gcc smoothFF.c -lm -o smoothFF 
- Run this program:
./smoothFF r3.I.dat > r3.II.dat
- This generates the file ‘r3.II.dat’, which contains the smoothed FF trace.

4. Synthesize song.
- Compile ‘synthesize.c’ in terminal, by typing:
gcc synthesize.c rk4.c -lm -o synthesize
- Run it by using:
./synthesize r3.II.dat r3.III.dat env_example.dat
- The synthesis uses a database relating parameters to FF and SCI.
The database is contained in the file ‘OEC.new.dat’, which is a 4-column ASCII file.
1st column is (-alpha), second column is (-beta), third column corresponds to FF and 4th column is the sci.
 
5. Convert synthesized output to .WAV
- Compile ‘dat2wav.c’ in terminal, by typing:
gcc dat2wav.c -o dat2wav -lm -std=c99
- Run it in order to obtain the .WAV with the song synthesis:
./dat2wav song_r3.III.dat song_r3.III.wav 44100
- song_r3.III.wav is the synthesized song, sampled at 44.1 kHz

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Automatic GTE extraction %
%      September 2015      %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

README and instructions.

This program uses the algorithm described in ‘Automatic reconstruction of physiological gestures used in a model of birdsong production’ by Santiago Boari, Yonatan Sanz Perl, Ana Amador, Daniel Margoliash and Gabriel B. Mindlin. 

The following instructions describe the compiling, usage and outputs of the program.

Files required for analysis: 
- A song recording, sampled at 40 kHz (i.e.: exampleGTE.wav)

1. Convert from WAV file to .dat ASCII
Note that you can convert to .dat on your own, but here we provide a code that does that. 
- Compile ‘wta.c’ (wav-to-ascii) in terminal, by typing:
cc wta.c -lm -o wta
- Run it by giving the input file and the output file name:
./wta exampleGTE.wav > exampleGTE.dat
Please note that you can convert from .WAV to ASCII using your own software. If you do so, please name your dat file ‘exampleGTE.dat’. You can adjust this to any other name by editing the name of the input files in ’gtes_example.c’

2. Compile the automatic GTE detection program, by typing:
gcc gtes_example.c -lm -o gtes_example

3. Run it, simply by typing: ./gtes_example
The program will automatically detect the required files, provided you followed previous steps.
This program creates separate files for each GTE type, computed from the sound envelope:
- gtes1.X.dat : syllable beginnings/ends
- gtes2.X.dat : absolute maximum of each syllable
- gtes3.X.dat : intrasyllabic significant minima (see paper for details)
- gtes4.X.dat : last maximum of each syllable

where X is the name of your original file (i.e., exampleGTE in this case).
The output in these files is the timestamp of each GTE, in number of samples.
To get GTE times in miliseconds, divide by 40 (sampling freq.: 40 kHz);


