#include <stdio.h>
#include <stdlib.h>

int main()
{
	const int byte_val = 0x36;
	const int gaus_filter[5][5] = { {1,4,7,4,1},
					{4,16,26,16,4},
					{7,26,41,26,7},
					{4,16,26,16,4},
					{1,4,7,4,7},
				      };
	long long int gaus_values[24][24];

	for (int a = 0; a < 10000; a++)				//number of image i want
	{
		unsigned char image[28][28][3];

		char path[30];
		sprintf(path, "test_images/%d.bmp", a);
		FILE* fp2 = fopen(path, "rb+");

		fseek( fp2, byte_val, SEEK_SET );		

		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					image[b][c][d]= fgetc(fp2);

		for (int b = 0; b < 24; b++)
			for (int c = 0; c < 24; c++)	
			{
				gaus_values[b][c] =   (int)image[b+0][c+0][1] * gaus_filter[0][0] +
						      (int)image[b+0][c+1][1] * gaus_filter[0][1] +
						      (int)image[b+0][c+2][1] * gaus_filter[0][2] +
						      (int)image[b+0][c+3][1] * gaus_filter[0][3] +
						      (int)image[b+0][c+4][1] * gaus_filter[0][4] +
						      (int)image[b+1][c+0][1] * gaus_filter[1][0] +
						      (int)image[b+1][c+1][1] * gaus_filter[1][1] +
						      (int)image[b+1][c+2][1] * gaus_filter[1][2] +
						      (int)image[b+1][c+3][1] * gaus_filter[1][3] +
						      (int)image[b+1][c+4][1] * gaus_filter[1][4] +
						      (int)image[b+2][c+0][1] * gaus_filter[2][0] +
						      (int)image[b+2][c+1][1] * gaus_filter[2][1] +
						      (int)image[b+2][c+2][1] * gaus_filter[2][2] +
						      (int)image[b+2][c+3][1] * gaus_filter[2][3] +
						      (int)image[b+2][c+4][1] * gaus_filter[2][4] +
						      (int)image[b+3][c+0][1] * gaus_filter[3][0] +
						      (int)image[b+3][c+1][1] * gaus_filter[3][1] +
						      (int)image[b+3][c+2][1] * gaus_filter[3][2] +
						      (int)image[b+3][c+3][1] * gaus_filter[3][3] +
						      (int)image[b+3][c+4][1] * gaus_filter[3][4] +
						      (int)image[b+4][c+0][1] * gaus_filter[4][0] +
						      (int)image[b+4][c+1][1] * gaus_filter[4][1] +
						      (int)image[b+4][c+2][1] * gaus_filter[4][2] +
						      (int)image[b+4][c+3][1] * gaus_filter[4][3] +
						      (int)image[b+4][c+4][1] * gaus_filter[4][4];
				
				gaus_values[b][c] /= 273;
			}	

		for (int b = 2; b < 26; b++)
			for (int c = 2; c < 26; c++)
				for (int d = 0; d < 3; d++)
					image[b][c][d] = gaus_values[b-2][c-2];

		fseek( fp2, byte_val, SEEK_SET );
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					fputc(image[b][c][d], fp2);
		fclose(fp2);
	}
}



