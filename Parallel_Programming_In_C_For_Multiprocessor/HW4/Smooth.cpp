#include <pthread.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"
#define NSmooth 1000
using namespace std;

int thread_count;
pthread_t* thread_handles;
pthread_barrier_t barrier;

BMPHEADER bmpHeader;
BMPINFO bmpInfo;
RGBTRIPLE **BMPSaveData = NULL;
RGBTRIPLE **BMPData = NULL;

int readBMP(char *fileName);
int saveBMP(char *fileName);
void swap(RGBTRIPLE *a, RGBTRIPLE *b);
RGBTRIPLE **alloc_memory( int Y, int X );
void* Smooth(void* rank);

int main(int argc, char* argv[])
{
	char infileName[100];
	char outfileName[100];
	strcpy(infileName, "input.bmp");
	strcpy(outfileName, "output2.bmp");

	if (!readBMP(infileName)) puts("Read file fails!!");
	else puts("Read file successfully!!");
	BMPData = alloc_memory( bmpInfo.biHeight, bmpInfo.biWidth);

	thread_count = atoi(argv[1]);
	thread_handles = new pthread_t [thread_count];
	pthread_barrier_init(&barrier, NULL, thread_count);

	for(int thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, &Smooth, (void*)thread);

	for(int thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	if(!saveBMP(outfileName)) puts("Save file fails!!");

	free(BMPData);
	free(BMPSaveData);
	pthread_barrier_destroy(&barrier);
	return 0;
}

int readBMP(char *fileName)
{
	ifstream bmpFile(fileName, ios::in | ios::binary);

	if (!bmpFile){
		puts("It can't open file!!");
		return 0;
	}
	bmpFile.read((char*) &bmpHeader, sizeof(BMPHEADER));

	if(bmpHeader.bfType != 0x4d42){
		puts("This file is not .BMP!!") ;
		return 0;
	}
	bmpFile.read((char*) &bmpInfo, sizeof(BMPINFO));

	if (bmpInfo.biBitCount != 24){
		puts("The file is not 24 bits!!");
		return 0;
	}

	while(bmpInfo.biWidth % 4 != 0) bmpInfo.biWidth++;

	BMPSaveData = alloc_memory(bmpInfo.biHeight, bmpInfo.biWidth);
	bmpFile.read((char*)BMPSaveData[0], bmpInfo.biWidth*sizeof(RGBTRIPLE)*bmpInfo.biHeight);

	bmpFile.close();
	return 1;
}

int saveBMP(char *fileName)
{
	if(bmpHeader.bfType != 0x4d42){
		puts("This file is not .BMP!!") ;
		return 0;
	}

	ofstream newFile(fileName, ios:: out | ios::binary);
	if ( !newFile ){
		puts("The File can't create!!");
		return 0;
	}

	newFile.write((char*)&bmpHeader, sizeof(BMPHEADER));
	newFile.write((char*)&bmpInfo, sizeof(BMPINFO));
	newFile.write((char*)BMPSaveData[0], bmpInfo.biWidth*sizeof(RGBTRIPLE)*bmpInfo.biHeight);

	newFile.close();
	return 1;
}

RGBTRIPLE **alloc_memory(int Y, int X )
{
	RGBTRIPLE **temp = new RGBTRIPLE* [Y];
	RGBTRIPLE *temp2 = new RGBTRIPLE [Y*X];
	memset(temp, 0, sizeof(RGBTRIPLE)*Y);
	memset(temp2, 0, sizeof(RGBTRIPLE)*Y*X);
	for(int i = 0; i < Y; i++){
		temp[i] = &temp2[i*X];
	}
	return temp;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
	RGBTRIPLE *temp = a;
	a = b;
	b = temp;
}

void* Smooth(void* rank)
{
	long my_rank = (long)rank;

	for(int count = 0; count < NSmooth; count++) {
		pthread_barrier_wait(&barrier);
		if(!my_rank) swap(BMPSaveData, BMPData);
		pthread_barrier_wait(&barrier);

		for(int i = my_rank; i < bmpInfo.biHeight; i+=thread_count)
			for(int j = 0; j < bmpInfo.biWidth; j++) {
				int T = i>0 ? i-1 : bmpInfo.biHeight-1;
				int D = i<bmpInfo.biHeight-1 ? i+1 : 0;
				int L = j>0 ? j-1 : bmpInfo.biWidth-1;
				int R = j<bmpInfo.biWidth-1 ? j+1 : 0;

				BMPSaveData[i][j].rgbBlue  = (double) (BMPData[i][j].rgbBlue+BMPData[T][j].rgbBlue+BMPData[D][j].rgbBlue+BMPData[i][L].rgbBlue+BMPData[i][R].rgbBlue)/5+0.5;
				BMPSaveData[i][j].rgbGreen = (double) (BMPData[i][j].rgbGreen+BMPData[T][j].rgbGreen+BMPData[D][j].rgbGreen+BMPData[i][L].rgbGreen+BMPData[i][R].rgbGreen)/5+0.5;
				BMPSaveData[i][j].rgbRed   = (double) (BMPData[i][j].rgbRed+BMPData[T][j].rgbRed+BMPData[D][j].rgbRed+BMPData[i][L].rgbRed+BMPData[i][R].rgbRed)/5+0.5;
			}
	}
}
