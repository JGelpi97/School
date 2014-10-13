/*
	FUSE: Filesystem in Userspace
	Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

	This program can be distributed under the terms of the GNU GPL.
	See the file COPYING.

	gcc -Wall `pkg-config fuse --cflags --libs` cs1550.c -o cs1550
	
	./cs1550 -d testmount
*/

/*
	CS1550 Project 4 - 4/18/2014
	Part 2
	Joey Gelpi - jbg29@pitt.edu
	
	
	================================READ ME=======================================================================================
	Everything works perfectly as far as I can tell.
	The only problem is that the program does not handle files larger than one inode can hold. 
	 So the biggest file it can handle is (MAX_DATA_IN_BLOCK * NUM_POINTERS_IN_INODE)
	 Any part of a file that goes over that will not be written.
	================================READ ME=======================================================================================
*/

#define	FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

//size of a disk block
#define	BLOCK_SIZE 512

//we'll use 8.3 filenames
#define	MAX_FILENAME 8
#define	MAX_EXTENSION 3

#define BITMAP_SIZE BLOCK_SIZE * 20

//How many files can there be in one directory?
#define	MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / \
	((MAX_FILENAME + 1) + (MAX_EXTENSION + 1) + sizeof(size_t) + sizeof(long))

//How much data can one block hold?
#define	MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof(unsigned long)) //504 bytes

//How many pointers in an inode?
#define NUM_POINTERS_IN_INODE ((BLOCK_SIZE - sizeof(unsigned int) - sizeof(unsigned long))/sizeof(unsigned long)) //62

struct cs1550_directory_entry
{
	char dname[MAX_FILENAME	+ 1];	//the directory name (plus space for a nul)
	int nFiles;			//How many files are in this directory. 
					//Needs to be less than MAX_FILES_IN_DIR

	struct cs1550_file_directory
	{
		char fname[MAX_FILENAME + 1];	//filename (plus space for nul)
		char fext[MAX_EXTENSION + 1];	//extension (plus space for nul)
		size_t fsize;			//file size
		long nStartBlock;		//where the first block is on disk
	} files[MAX_FILES_IN_DIR];		//There is an array of these
};

typedef struct cs1550_directory_entry cs1550_directory_entry;

struct cs1550_disk_block
{
	//The first 4 bytes will be the value 0xF113DA7A 
	unsigned long magic_number;		// = 4 bytes
	//And all the rest of the space in the block can be used for actual data
	//storage.
	char data[MAX_DATA_IN_BLOCK]; 	// = 508 bytes
};

typedef struct cs1550_disk_block cs1550_disk_block;

struct cs1550_inode
{
	//The first 4 bytes will be the value 0xFFFFFFFF
	unsigned long magic_number;
	//The number of children this node has (either other inodes or data blocks)
	unsigned int children;
	//An array of disk pointers to child nodes (either other inodes or data)
	unsigned long pointers[NUM_POINTERS_IN_INODE];	//if numn of blocks you need is bigger than this, need another inode
};

typedef struct cs1550_inode cs1550_inode;


/*
	Checks .directories to see if the directory exists.
	RETURNS: pointer to the cs1550_directory_entry
	
	NOTE: Must free memory after using
*/
static cs1550_directory_entry* directoryExists(const char *path)
{
	printf("Checking to see if directoryExists.\n");
	printf("path = %s\n", path);
	fflush(stdout);
	cs1550_directory_entry *foundEntry = malloc(sizeof(cs1550_directory_entry));
	
	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));	
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	//Check if name is subdirectory
	if (ret == 1)
	{
		cs1550_directory_entry tempEntry;								//Temp directory enrty
		memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
		
		FILE *file = NULL;
		file = fopen(".directories", "r");		
		
		if (file != NULL)
		{
			while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, file) == 1)
			{				
				if (strcmp(directory, tempEntry.dname) == 0)
				{
					memcpy(foundEntry, &tempEntry, sizeof(cs1550_directory_entry));
					fclose(file);
					return foundEntry;									//Foound it
				}
				memset(&tempEntry, 0, sizeof(cs1550_directory_entry));	//Reset memory
			}
			fclose(file);
			
		}
	}
	return NULL;
}

/*
 * Called whenever the system wants to know the file attributes, including
 * simply whether the file exists or not. 
 *
 * man -s 2 stat will show the fields of a stat structure
 */
static int cs1550_getattr(const char *path, struct stat *stbuf)
{
	printf("getattr was called.\n");
	printf("path = %s\n", path);
	fflush(stdout);

	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
   
	//is path the root dir?
	if (strcmp(path, "/") == 0) 
	{
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} 
	else 
	{
		char directory[strlen(path)];
		char filename[strlen(path)];
		char extension[strlen(path)];
		memset(&directory, 0, strlen(path));
		memset(&filename, 0, strlen(path));
		memset(&extension, 0, strlen(path));
		int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
		
		//Check if name is subdirectory
		if (ret == 1)
		{
			cs1550_directory_entry tempEntry;								//Temp directory enrty
			memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
			
			int found = 0;
			
			FILE *file = NULL;
			file = fopen(".directories", "r");		
			printf("SALDJASKD\n");
			fflush(stdout);
			if (file != NULL)
			{
				while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, file) == 1)
				{
					printf("Read .directories, this entries name is %s.\n", tempEntry.dname);
					if (strcmp(directory, tempEntry.dname) == 0)
					{
						stbuf->st_mode = S_IFDIR | 0755;
						stbuf->st_nlink = 2;
						res = 0;
						found = 1;
						//printf("Found the directory, it is %s.\n", tempEntry.dname);
						//fflush(stdout);
						break;
					}
					memset(&tempEntry, 0, sizeof(cs1550_directory_entry));		//Reset memory
				}
				
				if (found == 0)													//If we didn't find it, return doesn exist
				{
					//printf("Did not find the directory.\n");
					//fflush(stdout);
					res = -ENOENT;
				}
				fclose(file);
			}
			else
			{
				printf("Couldnt open .directories.\n");
				fflush(stdout);
				res = -ENOENT;
			}
			
						
		}
		else if (ret > 1)														//We are in a subdirectory looking at file
		{
			char dir[MAX_FILENAME + 2];
			memset(&dir, 0, MAX_FILENAME + 2);
			strcat(dir, "/");
			strcat(dir, directory);
			dir[MAX_FILENAME + 1] = '\0';
			printf("Searching for the directory '%s'\n", dir);
			fflush(stdout);
			cs1550_directory_entry * curDir = directoryExists(dir);
			if (curDir == NULL)
			{
				res = -ENOENT;		//Pretty sure this can never happen since the directory we are in obv has to exist
			}
			else
			{
				printf("Starting to search for the file in the directory '%s'\n", dir);
				fflush(stdout);
				int i;
				int found = 0;
				for (i = 0; i < curDir->nFiles; i++)
				{		
					
					//If the file matches the one that getattr needs
					if ((strncmp(curDir->files[i].fname, filename, MAX_FILENAME + 1) == 0) && (strncmp(curDir->files[i].fext, extension, MAX_EXTENSION + 1) == 0))
					{
						printf("File found.\n");
						fflush(stdout);
						found = 1;
						stbuf->st_mode = S_IFREG | 0666; 
						stbuf->st_nlink = 1; //file links
						stbuf->st_size = curDir->files[i].fsize;
						res = 0; // no error
					}
				}
				if (found == 0)
				{
					res = -ENOENT;
				}
			}
			free(curDir);
		}
	}
	return res;
}

/* 
 * Called whenever the contents of a directory are desired. Could be from an 'ls'
 * or could even be when a user hits TAB to do autocompletion
 */
static int cs1550_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	
	printf("Reading directory '%d'\n", path);
	fflush(stdout);
	//Since we're building with -Wall (all warnings reported) we need
	//to "use" every parameter, so let's just cast them to void to
	//satisfy the compiler
	(void) offset;
	(void) fi;

	
	
	//We are at the root, print out all directories
	if (strcmp(path, "/") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);	

		cs1550_directory_entry tempEntry;								//Temp directory enrty
		memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
		
		FILE *file = NULL;
		file = fopen(".directories", "r");		
		
		if (file != NULL)
		{
			while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, file) == 1)
			{
				filler(buf, tempEntry.dname, NULL, 0);
			}
		}
		else
		{
			return 0;
		}
		fclose(file);
	}
	else			//If we arent at the root, we have to be at another subdirectory
	{	
		cs1550_directory_entry * curLoc = directoryExists(path);
		if (curLoc != NULL)
		{			
			int i;
			for (i = 0; i < curLoc->nFiles; i++)						//Go through all of the files
			{
				char fi[MAX_FILENAME + 1 + MAX_EXTENSION + 2];
				memset(&fi, 0, MAX_FILENAME + 1 + MAX_EXTENSION + 2);
				fi[MAX_FILENAME + 1 + MAX_EXTENSION + 1] = '\0';
				strcat(fi, curLoc->files[i].fname);
				if (strlen(curLoc->files[i].fext) != 0)
				{
					strcat(fi, ".");
					strcat(fi, curLoc->files[i].fext);
				}
				filler(buf, fi, NULL, 0);
			}
		}
		free(curLoc);
	}

	//the filler function allows us to add entries to the listing
	//read the fuse.h file for a description (in the ../include dir)

	/*
	//add the user stuff (subdirs or files)
	//the +1 skips the leading '/' on the filenames
	filler(buf, newpath + 1, NULL, 0);
	*/
	
	return 0;
}

/* 
 * Creates a directory. We can ignore mode since we're not dealing with
 * permissions, as long as getattr returns appropriate ones for us.
 */
static int cs1550_mkdir(const char *path, mode_t mode)
{
	printf("Making directory.\n");
	fflush(stdout);
	//(void) path;
	(void) mode;	

	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	if (ret != 1)					//ret will be 1 if the path is at the root directory
	{
		printf("Cannot create directory here.\n");
		fflush(stdout);
		return -EPERM;
	}

	if (strlen(directory) > 8)		//Directory name is greater than 8 chars
	{
		return -ENAMETOOLONG;
	}
	
	cs1550_directory_entry newEntry;								//Temp directory enrty
	
	
	memset(&newEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
	
	strncpy(newEntry.dname, directory, MAX_FILENAME + 1);			//Copy fileName
	newEntry.nFiles = 0;											//Number of files is 0 because it is a mkdir call
	
	FILE *file = fopen(".directories", "a");
	fwrite(&newEntry, sizeof(cs1550_directory_entry), 1, file);		//Copy 1 element the size of the entire struct
	fclose(file);
	
	printf("Successfully created directory.\n");
	fflush(stdout);

	return 0;
}

/* 
 * Removes a directory.
 */
static int cs1550_rmdir(const char *path)
{
	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	if (ret == 1)															//It is actually a directory that we are removing
	{	
		FILE *temp = fopen(".tempDirectories", "a");						//Create a temp file to store everything but the dir we are removing
		cs1550_directory_entry *removeDir = directoryExists(path);			//Get that directory entry
		
		if (removeDir != NULL)												//The directory actually exists
		{
			if (removeDir->nFiles != 0)										//The directory is not empty
			{
				free(removeDir);
				fclose(temp);
				return -ENOTEMPTY;
			}
			cs1550_directory_entry tempEntry;								//Temp directory enrty
			memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
			
			FILE *directories = NULL;
			directories = fopen(".directories", "r");
			
			if (directories != NULL)										//The directories file should always exist here because we found an entry to get to this point
			{
				while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, directories) == 1)
				{				
					if (strcmp(tempEntry.dname, removeDir->dname) != 0)		//If the one we are removing and the one we are looking at arent the same, write it
					{
						fwrite(&tempEntry, sizeof(cs1550_directory_entry), 1, temp);
					}
					memset(&tempEntry, 0, sizeof(cs1550_directory_entry));	//Reset memory
				}
				fclose(directories);
			}
			
			fclose(temp);
			system("cp .tempDirectories .directories");						//Copy temp over directories
			system("rm .tempDirectories");									//Remove temp
			free(removeDir);												//Space was allocated for this in directoryExists()
		}
		else																//Directory doesnt exist
		{			
			return -ENOENT;
		}
	}
	else if (ret > 1)														//Not a directory
	{
		return -ENOTDIR;
	}
	
    return 0;	//Success
}

/* 
 * Does the actual creation of a file. Mode and dev can be ignored.
 *
 */
 
 //Cannot add more than 1 file
static int cs1550_mknod(const char *path, mode_t mode, dev_t dev)
{
	printf("Entered mknod\n");
	fflush(stdout);
	(void) mode;
	(void) dev;
	
	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	printf("path = %s\n", path);
	printf("directory = %s\n", directory);
	printf("ret = %d\n", ret);
	fflush(stdout);
	if (ret == 1)			//We are at the root, cant make files here
	{
		printf("At root, cant make file.\n");
		fflush(stdout);
		return -EPERM;
	}
	if (ret > 1)			//We have a filename or a filename.extension
	{
		if (strlen(filename) > 8 || strlen(extension) > 3)
		{
			return -ENAMETOOLONG;
		}
	
		cs1550_directory_entry tempEntry;								//Temp directory enrty
		memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
		
		FILE *file = NULL;
		file = fopen(".directories", "r+");								//Read and write apparently
		
		if (file != NULL)
		{
			while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, file) == 1)
			{				
				if (strcmp(directory, tempEntry.dname) == 0)			//Found match, directory entry is stored in tempEntry, edit it
				{
					if (tempEntry.nFiles != MAX_FILES_IN_DIR)			//If space in directory to put files
					{
						tempEntry.nFiles++;
						int indexOfFile = tempEntry.nFiles - 1;			//Index in the array of the directoryEntry structure
						strncpy(tempEntry.files[indexOfFile].fname, filename, MAX_FILENAME + 1);
						strncpy(tempEntry.files[indexOfFile].fext, extension, MAX_FILENAME + 1);
						tempEntry.files[indexOfFile].fsize = 0;
						
						cs1550_inode tempInode;
						memset(&tempInode, 0, sizeof(cs1550_inode));
						tempInode.magic_number = 0xFFFFFFFF;
						tempInode.children = 0;

						FILE *disk = NULL;
						disk = fopen(".disk", "r+b");
						if (disk != NULL)
						{
							fseek(disk, -BITMAP_SIZE, SEEK_END);							//At the start of our bitmap
							char bitmap[BITMAP_SIZE];
							fread(&bitmap, BITMAP_SIZE, 1, disk);							//Read in the bitmap
							int i;
							for (i = 0; i < BITMAP_SIZE; i++)
							{
								if (bitmap[i] == 0)				//Found a free space in memeory
								{
									printf("Found free space at location '%d'\n", i);
									fflush(stdout);
									tempEntry.files[indexOfFile].nStartBlock = i;			//Where the file starts (its inode)
									bitmap[i] = 1;
									
									fflush(disk);
									fseek(disk, sizeof(cs1550_disk_block) * i, SEEK_SET);	//Seek to open mem location
									fwrite(&tempInode, sizeof(cs1550_inode), 1, disk);		//Write inode
									
									fflush(disk);
									fseek(disk, -BITMAP_SIZE, SEEK_END);					//Seek to start of our bitmap
									fwrite(&bitmap, BITMAP_SIZE, 1, disk);					//Write bitmap back
									break;
								}
							}
						}
						fclose(disk);									
					}
					else																	//We have reached max files in the directory
					{
						fclose(file);
						return -EPERM;
					}
					break;		//We found the directory and handled everything, stop going throuh .directories
				}
				memset(&tempEntry, 0, sizeof(cs1550_directory_entry));						//Reset memory
			}
			//We wrote the inode and updated the bitmap, now to rewrite the directory entry to .directories
			//File pointer is currently at the start of the directory entry after the one we just worked with, go back to that one
			fseek(file, -sizeof(cs1550_directory_entry), SEEK_CUR);
			fwrite(&tempEntry, sizeof(cs1550_directory_entry), 1, file);					//Write updated directory entry
			
		}
		fclose(file);
	}
	return 0;
}

/*
 * Deletes a file
 */
static int cs1550_unlink(const char *path)
{
	printf("Unlink path = %s\n", path);
	fflush(stdout);
    char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	if (ret > 1)							//means we have a file.ext or just file, cant rm a directory
	{
		cs1550_directory_entry tempEntry;								//Temp directory enrty
		memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
		
		FILE *dirs = NULL;
		dirs = fopen(".directories", "r+");								//Read and write apparently
		int found = 0;
		if (dirs != NULL)
		{
			while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, dirs) == 1)	//Read in each directory entry
			{				
				if (strcmp(directory, tempEntry.dname) == 0)			//Found match, directory entry is stored in tempEntry, find if the fileis in there
				{
					int i = 0;
					for (i = 0; i < tempEntry.nFiles; i++)
					{
						if ((strcmp(filename, tempEntry.files[i].fname)) == 0 && (strcmp(extension, tempEntry.files[i].fext) == 0))	//If the files exists
						{
							printf("Found the file to remove, removing.\n");
							fflush(stdout);
							int nStartBlock = tempEntry.files[i].nStartBlock;
							
							
							FILE *disk = NULL;
							disk = fopen(".disk", "r+");							
							fseek(disk, -BITMAP_SIZE, SEEK_END);							//At the start of our bitmap
							char bitmap[BITMAP_SIZE];
							fread(&bitmap, BITMAP_SIZE, 1, disk);
							
							bitmap[nStartBlock] = 0;										//'Free' the inode
							
							fseek(disk, sizeof(cs1550_disk_block) * tempEntry.files[i].nStartBlock, SEEK_SET);						//Seek to the inode of the file								
							cs1550_inode tempINode;
							fread(&tempINode, sizeof(cs1550_inode), 1, disk);														//Read in that inode
							
							//Go through inode children and set bitmap to 0 for their blocks
							int j;						
							for (j = 0; j < tempINode.children; j++)																//Go through each of the inodes data blocks
							{
								bitmap[tempINode.pointers[j]] = 0;
							}
							
							fflush(disk);
							fseek(disk, -BITMAP_SIZE, SEEK_END);							//Seek to start of our bitmap
							fwrite(&bitmap, BITMAP_SIZE, 1, disk);							//Write bitmap back		

							memcpy(&tempEntry.files[i], &tempEntry.files[tempEntry.nFiles - 1], sizeof(struct cs1550_file_directory));		//Copy the last file entry into the one we are removing
							tempEntry.nFiles--;
							
							fflush(dirs);
							fseek(dirs, -sizeof(cs1550_directory_entry), SEEK_CUR);				//Go back one struct to startof the one we just edited
							fwrite(&tempEntry, sizeof(cs1550_directory_entry), 1, dirs);		//Write it								
							
							fclose(disk);
							fclose(dirs);
							return 0;	//Success, we found and removed it
							
						}
					}
					
					
					
				}
			}			
			//If we made it out of this loop, then we didnt find the file and return 0 saying it was a success, so the file cant exist
			fclose(dirs);
			return -ENOENT;
		}
	}
	else if (ret == 1)						//Directory, this cant actually get called, fuse or linux handles it
	{
		return -EISDIR;
	}

    return 0;
}

/* 
 * Read size bytes from file into buf starting from offset
 *
 */
static int cs1550_read(const char *path, char *buf, size_t size, off_t offset,
			  struct fuse_file_info *fi)
{
	(void) buf;
	(void) offset;
	(void) fi;
	(void) path;

	printf("Entered read in path '%s'.\n", path);
	printf("size = '%d'\n", size);
	printf("offset = '%d'\n", offset);	
	fflush(stdout);
	
	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	if (ret == 1)
	{
		return -EISDIR;
	}
	
	cs1550_directory_entry tempEntry;											//Temp directory enrty
	memset(&tempEntry, 0, sizeof(cs1550_directory_entry));						//Set the memory to 0
	
	FILE *dirs = NULL;
	dirs = fopen(".directories", "r+");											//Read and write apparently
	int found = 0;
	if (dirs != NULL)
	{
		while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, dirs) == 1)	//Read in each directory entry
		{				
			if (strcmp(directory, tempEntry.dname) == 0)						//Found match, directory entry is stored in tempEntry, find if the file is in there
			{
				printf("Found directory.\n");
				fflush(stdout);
				
				int i = 0;
				for (i = 0; i < tempEntry.nFiles; i++)
				{
					if ((strcmp(filename, tempEntry.files[i].fname)) == 0 && (strcmp(extension, tempEntry.files[i].fext) == 0))	//If the files exists
					{
						printf("Found file.\n");
						fflush(stdout);

						FILE *disk = NULL;
						disk = fopen(".disk", "r+");
						fseek(disk, sizeof(cs1550_disk_block) * tempEntry.files[i].nStartBlock, SEEK_SET);						//Seek to the inode of the file	
						
						cs1550_inode tempINode;
						fread(&tempINode, sizeof(cs1550_inode), 1, disk);
						
						char fileData[tempEntry.files[i].fsize + size + MAX_DATA_IN_BLOCK + 1];
						memset(fileData, 0, (tempEntry.files[i].fsize + size + MAX_DATA_IN_BLOCK));
						fileData[tempEntry.files[i].fsize + size + MAX_DATA_IN_BLOCK] = '\0';				//Just in case
						
						printf("Reading inode from block '%d'\n", tempEntry.files[i].nStartBlock);
						printf("inodes first pointer = '%d'\n", tempINode.pointers[0]);
						printf("inodes # of children = '%d'\n", tempINode.children);
						fflush(stdout);
						
						int j;
						int indexOfFile = 0;
						for (j = 0; j < tempINode.children; j++)																//Go through each of the inodes data blocks
						{
							fflush(disk);
							printf("Reading block '%d'\n", tempINode.pointers[j]);
							fflush(stdout);
							fseek(disk, sizeof(cs1550_disk_block) * tempINode.pointers[j], SEEK_SET);							//Go to data block
							cs1550_disk_block block;
							fread(&block, sizeof(cs1550_disk_block), 1, disk);													//Read it in
							printf("Block data = '%s'\n", block.data);
							memcpy(&fileData[indexOfFile], block.data, MAX_DATA_IN_BLOCK);										//Copy into our file buffer all the data
							indexOfFile += MAX_DATA_IN_BLOCK;																	//Move over and keep going
						}
						printf("Read in all children of the inode.\n");
						fflush(stdout);
						
						printf("File data = '%s'\n", fileData);
						fflush(stdout);

						memcpy(buf, &fileData[offset], size); 
						return size;
					}
				}
			}
		}
	}
	
	
	//check to make sure path exists
	//check that size is > 0
	//check that offset is <= to the file size
	//read in data
	//set size and return, or error

	size = 0;

	return size;
}

/* 
 * Write size bytes from buf into file starting from offset
 *
 */
static int cs1550_write(const char *path, const char *buf, size_t size, 
			  off_t offset, struct fuse_file_info *fi)
{
	printf("Entered write in path '%s'.\n", path);
	printf("buf = '%s'\n", buf);
	printf("size = '%d'\n", size);
	printf("offset = '%d'\n", offset);	
	fflush(stdout);
	(void) buf;
	(void) offset;
	(void) fi;
	(void) path;

	
	char directory[strlen(path)];
	char filename[strlen(path)];
	char extension[strlen(path)];
	memset(&directory, 0, strlen(path));
	memset(&filename, 0, strlen(path));
	memset(&extension, 0, strlen(path));		
	int ret = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	if (ret == 1)
	{
		return -EISDIR;
	}	
	
	cs1550_directory_entry tempEntry;								//Temp directory enrty
	memset(&tempEntry, 0, sizeof(cs1550_directory_entry));			//Set the memory to 0
	
	FILE *dirs = NULL;
	dirs = fopen(".directories", "r+");								//Read and write apparently
	int found = 0;
	if (dirs != NULL)
	{
		while (fread(&tempEntry, sizeof(cs1550_directory_entry), 1, dirs) == 1)	//Read in each directory entry
		{				
			if (strcmp(directory, tempEntry.dname) == 0)			//Found match, directory entry is stored in tempEntry, find if the file is in there
			{
				printf("Found directory.\n");
				fflush(stdout);
				
				int i = 0;
				for (i = 0; i < tempEntry.nFiles; i++)
				{
					if ((strcmp(filename, tempEntry.files[i].fname)) == 0 && (strcmp(extension, tempEntry.files[i].fext) == 0))	//If the files exists
					{
						printf("Found file.\n");
						fflush(stdout);

						FILE *disk = NULL;
						disk = fopen(".disk", "r+");
						fseek(disk, sizeof(cs1550_disk_block) * tempEntry.files[i].nStartBlock, SEEK_SET);						//Seek to the inode of the file
						
						cs1550_inode tempINode;
						fread(&tempINode, sizeof(cs1550_inode), 1, disk);														//Get that inode
						
						int dataLengthToBeAdded = tempEntry.files[i].fsize - offset;
						if (dataLengthToBeAdded == -1)
						{
							dataLengthToBeAdded = size;
						}
						else if (dataLengthToBeAdded >= 0)
						{
							dataLengthToBeAdded = size - dataLengthToBeAdded;
						}
						else
						{
							fclose(disk);
							fclose(dirs);
							return -EFBIG;
						}
						
						char fileData[tempEntry.files[i].fsize + dataLengthToBeAdded + MAX_DATA_IN_BLOCK];
						memset(fileData, 0, tempEntry.files[i].fsize + dataLengthToBeAdded + MAX_DATA_IN_BLOCK);
						int newFileSize = tempEntry.files[i].fsize + dataLengthToBeAdded;
						int j;
						int indexOfFile = 0;
						printf("Size of fileData[] buffer = '%d'\n", tempEntry.files[i].fsize + dataLengthToBeAdded + MAX_DATA_IN_BLOCK);
						printf("dataLengthToBeAdded = '%d'\n", dataLengthToBeAdded);
						printf("File size = '%d'\n", tempEntry.files[i].fsize);
						printf("newFileSize = '%d'\n", newFileSize);						
						printf("iNode children = '%d'\n", tempINode.children);
						printf("Copying data from .disk into fileData[] buffer.\n");
						fflush(stdout);

						for (j = 0; j < tempINode.children; j++)																//Go through each of the inodes data blocks
						{
							fflush(disk);
							fseek(disk, sizeof(cs1550_disk_block) * tempINode.pointers[j], SEEK_SET);							//Go to data block
							cs1550_disk_block block;
							fread(&block, sizeof(cs1550_disk_block), 1, disk);													//Read it in
							memcpy(&fileData[indexOfFile], block.data, MAX_DATA_IN_BLOCK);										//Copy into our file buffer all the data
							indexOfFile += MAX_DATA_IN_BLOCK;																	//Move over and keep going
						}
						printf("Read in all children of the inode.\n");
						fflush(stdout);
						
						//Now we have the entire file in fileData
						memcpy(&fileData[offset], buf, size);																	//Update the file with the new data
						printf("All data in fileData[] buffer.\nWriting back to disk blocks that already existed.\n");
						fflush(stdout);						
						
						
						indexOfFile = 0;
						for (j = 0; j < tempINode.children; j++)																//Go through each of the inodes data blocks
						{
							fflush(disk);
							fseek(disk, sizeof(cs1550_disk_block) * tempINode.pointers[j], SEEK_SET);							//Seek to that disk block that the inode already owns
							cs1550_disk_block block;
							block.magic_number = 0xF113DA7A;
							memcpy(block.data, &fileData[indexOfFile], MAX_DATA_IN_BLOCK);										//Copy file data to disk block
							fwrite(&block, sizeof(cs1550_disk_block), 1, disk);													//Write disk block to .disk
							indexOfFile += MAX_DATA_IN_BLOCK;
						}
						//Now we have written all the blocks that already existed, we have to write the ones that were added to the file
						printf("Wrote to blocks that already existed.\n");
						fflush(stdout);						
						
						if (dataLengthToBeAdded > 0)			//if we need to add new blocks
						{
							printf("Writing extra blocks.\n");
							fflush(stdout);
							
							int dataLeftToBeAdded = newFileSize - indexOfFile;
							int newBlocksNeeded = (dataLeftToBeAdded / MAX_DATA_IN_BLOCK) + 1;
							int j;
							
							fflush(disk);
							fseek(disk, -BITMAP_SIZE, SEEK_END);							//At the start of our bitmap
							char bitmap[BITMAP_SIZE];
							fread(&bitmap, BITMAP_SIZE, 1, disk);							//Read in the bitmap
							fflush(stdout);
							for (j = 0; j < newBlocksNeeded; j++)
							{
								int k;
								for (k = 0; k < BITMAP_SIZE; k++)
								{
									if (bitmap[k] == 0)
									{
										printf("Found a free block at '%d'\n", k);
										fflush(stdout);									
										bitmap[k] = 1;
										tempINode.children++;
										if (tempINode.children < 62 && k < 10220)								//Program only handles 1 inode a file, also dont write blocks if we are in the bitmap area
										{
											tempINode.pointers[tempINode.children - 1] = k;										
											
											fflush(disk);
											fseek(disk, sizeof(cs1550_disk_block) * k, SEEK_SET);				//Seek to that disk block that the inode already owns
											cs1550_disk_block block;
											block.magic_number = 0xF113DA7A;
											memcpy(block.data, &fileData[indexOfFile], MAX_DATA_IN_BLOCK);										//Copy file data to disk block
											fwrite(&block, sizeof(cs1550_disk_block), 1, disk);													//Write disk block to .disk
											indexOfFile += MAX_DATA_IN_BLOCK;
										}
										break;																	//Dont keep searching for free spaces
										
									}
								}
							}
							fflush(disk);
							fseek(disk, -BITMAP_SIZE, SEEK_END);							//Seek to start of our bitmap
							fwrite(&bitmap, BITMAP_SIZE, 1, disk);							//Write bitmap back	
						}
						printf("Wrote possible extra blocks.\nRewriting directory entry and inode.\n");
						fflush(stdout);
						
						//Rewrite directory entry
						tempEntry.files[i].fsize = newFileSize;
						
						fflush(dirs);
						fseek(dirs, -sizeof(cs1550_directory_entry), SEEK_CUR);
						fwrite(&tempEntry, sizeof(cs1550_directory_entry), 1, dirs);						
						
						printf("Writing inode to block '%d'\n", tempEntry.files[i].nStartBlock);
						printf("That inodes first pointer is '%d'\n", tempINode.pointers[0]);
						fflush(stdout);
						//Rewrite inode
						fflush(disk);
						fseek(disk, sizeof(cs1550_disk_block) * tempEntry.files[i].nStartBlock, SEEK_SET);						//Seek to the inode of the file
						fwrite(&tempINode, sizeof(cs1550_inode), 1, disk);
						fflush(disk);
						
						fclose(disk);
						fclose(dirs);
						
						printf("Aaaaand were done.\n");
						fflush(stdout);
						
						return size;		//Did our work
					}
				}
				fclose(dirs);
				return -EISDIR;
			}
		}
	}
	
	//check to make sure path exists
	//check that size is > 0
	//check that offset is <= to the file size
	//write data
	//set size (should be same as input) and return, or error

	return -1;
}

/******************************************************************************
 *
 *  DO NOT MODIFY ANYTHING BELOW THIS LINE
 *
 *****************************************************************************/

/*
 * truncate is called when a new file is created (with a 0 size) or when an
 * existing file is made shorter. We're not handling deleting files or 
 * truncating existing ones, so all we need to do here is to initialize
 * the appropriate directory entry.
 *
 */
static int cs1550_truncate(const char *path, off_t size)
{
	(void) path;
	(void) size;

    return 0;
}


/* 
 * Called when we open a file
 *
 */
static int cs1550_open(const char *path, struct fuse_file_info *fi)
{
	(void) path;
	(void) fi;
    /*
        //if we can't find the desired file, return an error
        return -ENOENT;
    */

    //It's not really necessary for this project to anything in open

    /* We're not going to worry about permissions for this project, but 
	   if we were and we don't have them to the file we should return an error

        return -EACCES;
    */

    return 0; //success!
}

/*
 * Called when close is called on a file descriptor, but because it might
 * have been dup'ed, this isn't a guarantee we won't ever need the file 
 * again. For us, return success simply to avoid the unimplemented error
 * in the debug log.
 */
static int cs1550_flush (const char *path , struct fuse_file_info *fi)
{
	(void) path;
	(void) fi;

	return 0; //success!
}


//register our new functions as the implementations of the syscalls
static struct fuse_operations hello_oper = {
    .getattr	= cs1550_getattr,
    .readdir	= cs1550_readdir,
    .mkdir	= cs1550_mkdir,
	.rmdir = cs1550_rmdir,
    .read	= cs1550_read,
    .write	= cs1550_write,
	.mknod	= cs1550_mknod,
	.unlink = cs1550_unlink,
	.truncate = cs1550_truncate,
	.flush = cs1550_flush,
	.open	= cs1550_open,
};

//Don't change this.
int main(int argc, char *argv[])
{
	FILE *dirs = NULL;
	dirs = fopen(".directories", "a");
	if (dirs == NULL)
	{
		printf(".directories doesnt exist, creating.\n");
		fflush(stdout);
		system("touch .directories");
	}
	else
	{
		fclose(dirs);
	}
	return fuse_main(argc, argv, &hello_oper, NULL);
}
