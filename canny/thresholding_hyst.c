#include <stdio.h>
#include <stdlib.h>

int main()
{
	const int byte_val = 0x36;

	const int LOW_T = 100;
	const int HIGH_T = 120;

	for (int a = 0; a < 60000; a++)				//number of image i want
	{
		unsigned char image[28][28][3];

		char path[30];
		sprintf(path, "train_images/%d.bmp", a);
		FILE* fp2 = fopen(path, "rb+");

		fseek( fp2, byte_val, SEEK_SET );		

		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					image[b][c][d]= fgetc(fp2);

		
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					if(image[b][c][d] >= HIGH_T)
						image[b][c][d] = 255;
					else if(image[b][c][d] < LOW_T)
						image[b][c][d] = 0;
					
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					if(image[b][c][d] != 0 && image[b][c][d] != 255)
						if(image[b+1][c][d] == 255 || image[b-1][c][d] == 255 || image[b][c+1][d] == 255 || image[b][c-1][d] == 255)
							image[b][c][d] = 255;
						else if(image[b+1][c+1][d] == 255 || image[b-1][c-1][d] == 255
						        || image[b-1][c+1][d] == 255 || image[b+1][c-1][d] == 255)
							image[b][c][d] = 255;
						else
							image[b][c][d] = 0;

		fseek( fp2, byte_val, SEEK_SET );
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					fputc(image[b][c][d], fp2);
		fclose(fp2);
	}
}



