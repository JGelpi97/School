
#include <stdio.h>
#include <string.h>

#define	BLOCK_SIZE 512

//we'll use 8.3 filenames
#define	MAX_FILENAME 8
#define	MAX_EXTENSION 3

#define BITMAP_SIZE BLOCK_SIZE * 20

//How many files can there be in one directory?
#define	MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / \
	((MAX_FILENAME + 1) + (MAX_EXTENSION + 1) + sizeof(size_t) + sizeof(long))

//How much data can one block hold?
#define	MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof(unsigned long))

//How many pointers in an inode?
#define NUM_POINTERS_IN_INODE ((BLOCK_SIZE - sizeof(unsigned int) - sizeof(unsigned long))/sizeof(unsigned long))

int main(int argc, char *argv[])
{
	printf("max data in block = %d\n", MAX_DATA_IN_BLOCK);
	printf("pointers in node = %d\n", NUM_POINTERS_IN_INODE);
}
