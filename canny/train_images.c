#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
	unsigned char header[0x36] ={   66,77,102,9,0,0,0,0,0,0,54,0,0,0,40,0,
					0,0,28,0,0,0,28,0,0,0,1,0,24,0,0,0,0,0,
					48,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	mkdir("train_images", ACCESSPERMS);
	FILE* fp3 = fopen("train-images.idx3-ubyte", "rb");
	unsigned char header3[16];
	for (int a = 0; a < 16; a++)
		header3[a] = fgetc(fp3);

	unsigned char**** image = (unsigned char****)malloc(60025 * sizeof(unsigned char***));
	for (int a = 0; a < 60025; a++)
	{
		image[a] = (unsigned char***)malloc(28 * sizeof(unsigned char**));
		for (int b = 0; b < 28; b++)
		{
			image[a][b] = (unsigned char**)malloc(28 * sizeof(unsigned char*));
			for (int c = 0; c < 28; c++)
				image[a][b][c] = (unsigned char*)malloc(3 * sizeof(unsigned char));
				
		}
	}

	for (int a = 0; a < 60025; a++)
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
			{
				unsigned char ch = fgetc(fp3);
				for (int d = 0; d < 3; d++)
					image[a][b][c][d] = ch;
			}

	for (int a = 0; a < 60000; a++)				//number of image i want
	{
		char path[30];
		sprintf(path, "train_images/%d.bmp", a);

		FILE* fp2 = fopen(path, "wb");
		for (int k = 0; k < 0x36; k++)
			fputc(header[k], fp2);
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					fputc(image[a][b][c][d], fp2);
		fclose(fp2);
		
	}
	

}
