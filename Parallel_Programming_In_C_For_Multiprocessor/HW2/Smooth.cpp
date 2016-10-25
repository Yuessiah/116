#include <mpi.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include "bmp.h"

using namespace std;

#define NSmooth 1000

BMPHEADER bmpHeader;
BMPINFO bmpInfo;
RGBTRIPLE **BMPSaveData = NULL;
RGBTRIPLE **BMPData = NULL;
int upperBound, lowerBound;
RGBTRIPLE *upperRecv = NULL;
RGBTRIPLE *lowerRecv = NULL;

int readBMP( char *fileName);
int saveBMP( char *fileName);
void swap(RGBTRIPLE *a, RGBTRIPLE *b);
RGBTRIPLE **alloc_memory( int Y, int X );

int main(int argc,char *argv[])
{
	int my_rank, comm_sz;
	char *infileName = "input.bmp";
	char *outfileName = "output2.bmp";
	double startwtime = 0.0, endwtime=0;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Barrier(MPI_COMM_WORLD);
	startwtime = MPI_Wtime();

	if ( readBMP( infileName ) )
		cout << "Read file successfully!!" << endl;
	else
		cout << "Read file fails!!" << endl;

	BMPData = alloc_memory( bmpInfo.biHeight, bmpInfo.biWidth );
	lowerRecv = new RGBTRIPLE [ bmpInfo.biWidth ];
	upperRecv = new RGBTRIPLE [ bmpInfo.biWidth ];

	int task_itv = (my_rank != comm_sz-1)? bmpInfo.biHeight/comm_sz : bmpInfo.biHeight-(bmpInfo.biHeight/comm_sz)*(comm_sz-1);
	lowerBound = my_rank*(bmpInfo.biHeight/comm_sz);
	upperBound = lowerBound+task_itv;

	for(int count = 0; count < NSmooth ; count++){
		swap(BMPSaveData,BMPData);

		MPI_Send(BMPData[lowerBound]  , bmpInfo.biWidth*3, MPI_BYTE, (my_rank-1+comm_sz)%comm_sz, 1, MPI_COMM_WORLD);
		MPI_Send(BMPData[upperBound-1], bmpInfo.biWidth*3, MPI_BYTE, (my_rank+1)%comm_sz        , 2, MPI_COMM_WORLD);
		MPI_Recv(lowerRecv, bmpInfo.biWidth*3, MPI_BYTE, (my_rank-1+comm_sz)%comm_sz, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(upperRecv, bmpInfo.biWidth*3, MPI_BYTE, (my_rank+1)%comm_sz        , 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int i = lowerBound; i<upperBound ; i++){
			for(int j =0; j<bmpInfo.biWidth ; j++){
				int T = i>0 ? i-1 : bmpInfo.biHeight-1;
				int D = i<bmpInfo.biHeight-1 ? i+1 : 0;
				int L = j>0 ? j-1 : bmpInfo.biWidth-1;
				int R = j<bmpInfo.biWidth-1 ? j+1 : 0;

				RGBTRIPLE Top   = BMPData[T][j];
				RGBTRIPLE Down  = BMPData[D][j];
				RGBTRIPLE Left  = BMPData[i][L];
				RGBTRIPLE Right = BMPData[i][R];
				if(i == upperBound-1) Top  = upperRecv[j];
				if(i == lowerBound  ) Down = lowerRecv[j];

				BMPSaveData[i][j].rgbBlue  =  (double) (BMPData[i][j].rgbBlue+Top.rgbBlue+Down.rgbBlue+Left.rgbBlue+Right.rgbBlue)/5+0.5;
				BMPSaveData[i][j].rgbGreen =  (double) (BMPData[i][j].rgbGreen+Top.rgbGreen+Down.rgbGreen+Left.rgbGreen+Right.rgbGreen)/5+0.5;
				BMPSaveData[i][j].rgbRed   =  (double) (BMPData[i][j].rgbRed+Top.rgbRed+Down.rgbRed+Left.rgbRed+Right.rgbRed)/5+0.5;
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//Spell all piece of smooth gragh that made from the other processes.
	MPI_Gather(BMPSaveData[lowerBound], (upperBound-lowerBound)*bmpInfo.biWidth*3, MPI_BYTE, BMPSaveData[0], (upperBound-lowerBound)*bmpInfo.biWidth*3, MPI_BYTE, 0, MPI_COMM_WORLD);

	if(my_rank == 0) {
		if ( saveBMP( outfileName ) )
			cout << "Save file successfully!!" << endl;
		else
			cout << "Save file fails!!" << endl;
	}

	endwtime = MPI_Wtime();
	cout << "The execution time = "<< endwtime-startwtime <<endl ;

	free(BMPData);
	free(BMPSaveData);
	MPI_Finalize();

	return 0;
}

int readBMP(char *fileName)
{
	ifstream bmpFile( fileName, ios::in | ios::binary );
	if ( !bmpFile ){
		cout << "It can't open file!!" << endl;
		return 0;
	}

	bmpFile.read( ( char* ) &bmpHeader, sizeof( BMPHEADER ) );

	if( bmpHeader.bfType != 0x4d42 ){
		cout << "This file is not .BMP!!" << endl ;
		return 0;
	}

	bmpFile.read( ( char* ) &bmpInfo, sizeof( BMPINFO ) );

	if ( bmpInfo.biBitCount != 24 ){
		cout << "The file is not 24 bits!!" << endl;
		return 0;
	}

	while( bmpInfo.biWidth % 4 != 0 )
		bmpInfo.biWidth++;

	BMPSaveData = alloc_memory( bmpInfo.biHeight, bmpInfo.biWidth);

	bmpFile.read( (char* )BMPSaveData[0], bmpInfo.biWidth*sizeof(RGBTRIPLE)*bmpInfo.biHeight);

	bmpFile.close();
	return 1;
}

int saveBMP( char *fileName)
{
	if( bmpHeader.bfType != 0x4d42 ){
		cout << "This file is not .BMP!!" << endl ;
		return 0;
	}

	ofstream newFile( fileName,  ios:: out | ios::binary );
	if ( !newFile ){
		cout << "The File can't create!!" << endl;
		return 0;
	}

	newFile.write( ( char* )&bmpHeader, sizeof( BMPHEADER ) );
	newFile.write( ( char* )&bmpInfo, sizeof( BMPINFO ) );

	newFile.write( ( char* )BMPSaveData[0], bmpInfo.biWidth*sizeof(RGBTRIPLE)*bmpInfo.biHeight );

	newFile.close();
	return 1;
}

RGBTRIPLE **alloc_memory(int Y, int X )
{
	RGBTRIPLE **temp = new RGBTRIPLE *[ Y ];
	RGBTRIPLE *temp2 = new RGBTRIPLE [ Y * X ];
	memset( temp, 0, sizeof( RGBTRIPLE ) * Y);
	memset( temp2, 0, sizeof( RGBTRIPLE ) * Y * X );
	for( int i = 0; i < Y; i++){
		temp[ i ] = &temp2[i*X];
	}
	return temp;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
	RGBTRIPLE *temp;
	temp = a;
	a = b;
	b = temp;
}
