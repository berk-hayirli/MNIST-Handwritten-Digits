#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	static double h = 0;
	const int byte_val = 0x36;

	const int sobell_mask[3][3] = { {1,0,-1},
					{2,0,-2},
					{1,0,-1},
				      };

	const int sobell_mask2[3][3] = { {1,2,1},
					 {0,0,0},
					 {-1,-2,-1},
				       };
	int sobell_values[26][26];
	int sobell_values2[26][26];
	double total_sobell[26][26];
	int sobell_degrees[26][26];

	for (int a = 0; a < 10000; a++)		//number of image i want
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

		for (int b = 0; b < 26; b++)
			for (int c = 0; c < 26; c++)
				sobell_values[b][c] = (int)image[b][c][1] * sobell_mask[0][0] +
						      (int)image[b][c+1][1] * sobell_mask[0][1] +
						      (int)image[b][c+2][1] * sobell_mask[0][2] +
						      (int)image[b+1][c][1] * sobell_mask[1][0] +
						      (int)image[b+1][c+1][1] * sobell_mask[1][1] +
						      (int)image[b+1][c+2][1] * sobell_mask[1][2] +
						      (int)image[b+2][c][1] * sobell_mask[2][0] +
						      (int)image[b+2][c+1][1] * sobell_mask[2][1] +
						      (int)image[b+2][c+2][1] * sobell_mask[2][2];
			
		for (int b = 0; b < 26; b++)
			for (int c = 0; c < 26; c++)
				sobell_values2[b][c] = (int)image[b][c][1] * sobell_mask2[0][0] +
						       (int)image[b][c+1][1] * sobell_mask2[0][1] +
						       (int)image[b][c+2][1] * sobell_mask2[0][2] +
						       (int)image[b+1][c][1] * sobell_mask2[1][0] +
						       (int)image[b+1][c+1][1] * sobell_mask2[1][1] +
						       (int)image[b+1][c+2][1] * sobell_mask2[1][2] +
						       (int)image[b+2][c][1] * sobell_mask2[2][0] +
						       (int)image[b+2][c+1][1] * sobell_mask2[2][1] +
						       (int)image[b+2][c+2][1] * sobell_mask2[2][2];

		for (int b = 0; b < 26; b++)
			for (int c = 0; c < 26; c++)
				//total_sobell[b][c] = (int)(((double)255/970) * sqrt(pow(sobell_values[b][c],2) + pow(sobell_values2[b][c],2)));
				//total_sobell[b][c] = (int)(((double)255/1121) * sqrt(pow(sobell_values[b][c],2) + pow(sobell_values2[b][c],2)));
				//total_sobell[b][c] = (int)(((double)255/1116) * sqrt(pow(sobell_values[b][c],2) + pow(sobell_values2[b][c],2)));
				total_sobell[b][c] = sqrt(pow(sobell_values[b][c],2) + pow(sobell_values2[b][c],2));

		for (int b = 0; b < 26; b++)
			for (int c = 0; c < 26; c++)
			{
				if(sobell_values2[b][c] == 0 && sobell_values[b][c] == 0)
					sobell_degrees[b][c] = 180;				//not an edge
				else
				{
					double angle = atan2(sobell_values2[b][c],sobell_values[b][c]) * 180.0 / 3.14159265;

					if((angle >= 0.0 && angle <= 22.5) || (angle >= 157.5 && angle <= 181.0)  ||
					   (angle >= -180.0 && angle <= -157.0) || (angle >= -22.5 && angle <= 0.0))
						sobell_degrees[b][c] = 0;
					else if((angle >= 22.5 && angle <= 67.5) || (angle >= -157.5 && angle <= -112.5))
						sobell_degrees[b][c] = 45;
					else if((angle >= 67.5 && angle <= 112.5) || (angle >= -112.5 && angle <= -67.5))
						sobell_degrees[b][c] = 90;
					else if((angle >= 112.5 && angle <= 157.5) || (angle >= -67.5 && angle <= -22.5))
						sobell_degrees[b][c] = 135;
				}
			}

		int grad_image[28][28];
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				if(b == 0 || c == 0 || b == 27 || c == 27 )
					grad_image[b][c] = 0;
				else
					grad_image[b][c] = (int)total_sobell[b-1][c-1];

		int non_max[28][28];
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				if(b == 0 || c == 0 || b == 27 || c == 27 )
					non_max[b][c] = 0;

		for (int b = 1; b < 27; b++)
			for (int c = 1; c < 27; c++)
			{
				if(sobell_degrees[b-1][c-1] == 0)
				{
					if(grad_image[b][c] >= grad_image[b][c-1] && grad_image[b][c] >= grad_image[b][c+1])
						non_max[b][c] = grad_image[b][c];
					else
						non_max[b][c] = 0;
				}
				else if(sobell_degrees[b-1][c-1] == 45)
				{
					if(grad_image[b][c] >= grad_image[b+1][c+1] && grad_image[b][c] >= grad_image[b-1][c-1])
						non_max[b][c] = grad_image[b][c];
					else
						non_max[b][c] = 0;
				}
				else if(sobell_degrees[b-1][c-1] == 90)
				{
					if(grad_image[b][c] >= grad_image[b-1][c] && grad_image[b][c] >= grad_image[b+1][c])
						non_max[b][c] = grad_image[b][c];
					else
						non_max[b][c] = 0;
				}
				else if(sobell_degrees[b-1][c-1] == 135)
				{
					if(grad_image[b][c] >= grad_image[b-1][c+1] && grad_image[b][c] >= grad_image[b+1][c-1])
						non_max[b][c] = grad_image[b][c];
					else
						non_max[b][c] = 0;
				}	
				else if(sobell_degrees[b-1][c-1] == 180)
					non_max[b][c] = 0;		
			}

		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				if( non_max[b][c] > 677)
					non_max[b][c] = 677;

		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				non_max[b][c] = (int)((double)255/677 * non_max[b][c]);

		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					image[b][c][d] = non_max[b][c];

		fseek( fp2, byte_val, SEEK_SET );
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					fputc(image[b][c][d], fp2);
		fclose(fp2);
	}	
}



