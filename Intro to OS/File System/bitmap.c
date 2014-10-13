#include <stdio.h>
#include <string.h>

#define	BLOCK_SIZE 512
#define BITMAP_SIZE BLOCK_SIZE * 20

int main(int argc, char *argv[])
{
	FILE *bm = fopen(".disk", "r");
	fseek(bm, -BITMAP_SIZE, SEEK_END);
	char bitmap[BITMAP_SIZE];
	fread(&bitmap, BITMAP_SIZE, 1, bm);
	int i;
	
	for (i = 0; i < BITMAP_SIZE; i++)
	{
		printf("%d", bitmap[i]);
		if (i != 0 && i % 50 == 0)
		{
			printf("\n");
		}
	}
	fclose(bm);
}