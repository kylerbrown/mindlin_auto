//////////////////////////////////////////
// 				Dat2wav					//
// 			Date: 25-04-2013			// 			                     		
//////////////////////////////////////////

#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

// Defines WAV header as structs, following wav definition at:
//https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

struct WAVHeader {
    char ChunkID[4];
    uint32_t ChunkSize;
    char RIFFType[4];
};

struct FormatHeader {
    char ChunkID[4];
    uint32_t ChunkSize;
    uint16_t CompressionCode;
    uint16_t Channels;
    uint32_t SampleRate;
    uint32_t AvgBytesPerSec;
    uint16_t BlockAlign;
    uint16_t SigBitsPerSamp;
};

struct DataHeader {
    char ChunkID[4];
    uint32_t ChunkSize;

};


int main(int argc, char * argv[]) {

//Checks arguments
if(argc < 3) {
    printf("\nUsage:\n./dat2wav [inputfile] [outputfile] [samplerate]\n");   
    return 1;
}

//Organizes arguments and variables
double length;
int samplerate;
char * input = NULL;
char * output = NULL;
int i = 0;
input=argv[1];
output=argv[2];
samplerate = atoi(argv[3]);
double max = 0,min=300000;
double dtemp;
unsigned int sample_count;
FILE *in, *out;

//Defines time and sample count
	in=fopen(input,"r");
		while(!feof(in)){
			  i++;
			  fscanf(in,"%lf \n",&dtemp);
			  if(dtemp > max) max = dtemp;
			  if(dtemp < min) min = dtemp;
			  }

length=i*(1/(double)samplerate);
sample_count=i;

//Assigns memory
size_t size = sizeof(struct WAVHeader) + sizeof(struct FormatHeader) + sizeof(struct DataHeader) + (length * samplerate * 2);
void * buffer = malloc(size);

//Puts headers to buffer
struct WAVHeader * WAV = (struct WAVHeader *)buffer;
struct FormatHeader * Format = (struct FormatHeader *)(WAV + 1);
struct DataHeader * Data = (struct DataHeader *)(Format + 1);

memcpy(WAV->ChunkID,"RIFF",4);
WAV->ChunkSize = (uint32_t)size - 8;
memcpy(WAV->RIFFType,"WAVE",4);

memcpy(Format->ChunkID,"fmt ",4);
Format->ChunkSize = 16;
Format->CompressionCode = (uint16_t)01;
Format->Channels = (uint16_t)01;
Format->SampleRate = (uint32_t)samplerate;
Format->SigBitsPerSamp = 16; 
Format->BlockAlign = 2;
Format->AvgBytesPerSec = Format->BlockAlign * samplerate;

memcpy(Data->ChunkID, "data",4);
Data->ChunkSize = length * samplerate * 2;

//Scans inputfile
printf("Generating sound...\n");
rewind(in);

short * sound = (short *)(Data + 1);

for (unsigned int j = 0; j < sample_count; j++)
{
    int result;
	float amplitud;	
	fscanf(in,"%lf \n",&dtemp);
	amplitud=rint(65535.0*(dtemp-min)/(max-min)-32768.0);
    result = (long)amplitud;

    if (result > 32767)
    {
        result = 32767;
    }
    else if (result < -32768)
    {
        result = -32768;
    }

    sound[j] = (short)result;
}
fclose(in);

//Writes buffer to file

out = fopen(output, "w");
fwrite(buffer, size, 1, out);
fclose(out);

printf(" %d bytes written to %s\n",size, output);

return 1;

}
