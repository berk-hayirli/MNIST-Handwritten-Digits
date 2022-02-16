#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()	//7832
{
	const int byte_val = 0x36;
	static int cc = 0;

	FILE* fp4 = fopen("t10k-labels.idx1-ubyte", "rb");
	for (int l = 0; l < 8; l++)
		fgetc(fp4);

	unsigned char labels[10000];
	for (int a = 0; a < 10000; a++)
	{
		unsigned char ch = fgetc(fp4);
		labels[a] = ch;
	}
	fclose(fp4);

	FILE *fp5 = fopen("test_labels.txt", "w");
	FILE *cch = fopen("test_cch.txt", "w");

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
		
		unsigned int start_x, start_y;
		int break_flag=0;
		int finished_flag=0;
		for (int b = 1; b < 27; b++)
		{
			for (int c = 1; c < 27; c++)
				if(image[b][c][0] == 255)
				{
					start_x = b;
					start_y = c;
					break_flag = 1;
					break;
				}
			if(break_flag)
				break;
		}
		
		unsigned int cch_map[400];
		unsigned int cch_traversal[28][28];	
		unsigned int count = 0;
		for(int m = 0; m < 28 ; m++)
			for(int n = 0; n < 28 ; n++)
				cch_traversal[m][n] = 0;
		
		unsigned int x = start_x ,y = start_y;
		
		for( ; 1 ; count++)
		{
			if(image[x][y+1][0] == 255 && cch_traversal[x][y+1] == 0){            //0
				cch_map[count] = 0; 
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;
				y++;							
			}
			else if(image[x+1][y+1][0] == 255 && cch_traversal[x+1][y+1] == 0){   //1
				cch_map[count] = 1;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;
				x++;
				y++;
			}
			else if(image[x+1][y][0] == 255 && cch_traversal[x+1][y] == 0){       //2
				cch_map[count] = 2;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;	
				x++;
			}	
			else if(image[x+1][y-1][0] == 255 && cch_traversal[x+1][y-1] == 0){   //3
				cch_map[count] = 3;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;	
				x++;
				y--;
			}
			else if(image[x][y-1][0] == 255 && cch_traversal[x][y-1] == 0){       //4
				cch_map[count] = 4;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;	
				y--;
			}	
			else if(image[x-1][y-1][0] == 255 && cch_traversal[x-1][y-1] == 0){   //5
				cch_map[count] = 5;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;
				x--;
				y--;
			}
			else if(image[x-1][y][0] == 255 && cch_traversal[x-1][y] == 0){       //6
				cch_map[count] = 6;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;	
				x--;
			}
			else if(image[x-1][y+1][0] == 255 && cch_traversal[x-1][y+1] == 0){   //7
				cch_map[count] = 7;
				cch_traversal[x][y] = 1;
				image[x][y][2] = 120;	
				x--;
				y++;
			}
			else
			{
				image[x][y][2] = 120;
				if(((start_x == x-1) || (start_x == x) || (start_x == x+1)) && ((start_y == y-1) || (start_y == y) || (start_y == y+1)))
					finished_flag = 1;				
				break;
			}
		}
		count++;
		if(count <= 30)
			cc++;
		unsigned int cch_hist[8] = {0,0,0,0,0,0,0,0};
		for(int k = 0; k < count ; k++)
			if(cch_map[k] == 0)			
				cch_hist[0]++;
			else if(cch_map[k] == 1)			
				cch_hist[1]++;
			else if(cch_map[k] == 2)			
				cch_hist[2]++;
			else if(cch_map[k] == 3)			
				cch_hist[3]++;
			else if(cch_map[k] == 4)			
				cch_hist[4]++;
			else if(cch_map[k] == 5)			
				cch_hist[5]++;
			else if(cch_map[k] == 6)			
				cch_hist[6]++;
			else if(cch_map[k] == 7)			
				cch_hist[7]++;

		
		unsigned int dcch_hist[8] = {0,0,0,0,0,0,0,0};
		for(int k = 0; k < count-1 ; k++)
		{
			int dif = cch_map[k+1] - cch_map[k];
			if(dif < 0)
				dcch_hist[8 + dif]++;
			else
				dcch_hist[dif]++;
		}

		if(finished_flag)
		{
			fprintf(fp5,"%d\n",(int)labels[a]); 	
			fprintf(cch, "%d %d %d %d %d %d %d %d "
					,cch_hist[0],cch_hist[1],cch_hist[2],cch_hist[3],cch_hist[4],cch_hist[5],cch_hist[6],cch_hist[7]);
			fprintf(cch, "%d %d %d %d %d %d %d %d\n"
					,dcch_hist[0],dcch_hist[1],dcch_hist[2],dcch_hist[3],dcch_hist[4],dcch_hist[5],dcch_hist[6],dcch_hist[7]);
		}	

		fseek( fp2, byte_val, SEEK_SET );
		for (int b = 0; b < 28; b++)
			for (int c = 0; c < 28; c++)
				for (int d = 0; d < 3; d++)
					fputc(image[b][c][d], fp2);
		
		fclose(fp2);	
	}
	fclose(cch);
	fclose(fp5);
}




