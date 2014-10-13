/*
	Joey Gelpi - jbg29
	CS1550 Project 1
	osmsg.c
	Works perfectly.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 1000
#define MAX_USRNAME_SIZE 100

int main(int argc, char *argv[])
{
	char *currUser = getenv("USER");
	
	if (argc == 1 || argc == 3)					//Bad number of arguments
	{
		printf("usage: osmsg -s/-r user message");
		return -1;
	}

	char *action = argv[1];
	
	if (strcmp(action, "-s") == 0 && argc != 4)	//If we don't have all arguments
	{
		printf("usage: osmsg -s/-r user message");
		return -1;
	}

	char *user = argv[2];
	char *msg = argv[3];
	
	if (strcmp(action, "-s") == 0)
	{
		printf("Sending the message '%s' to user '%s'.\n", msg, user);	
		//sys_cs1550_send_msg(const char __user *to, const char __user *msg, const char __user *from)
		long success = syscall(325, user, msg, currUser);
	}
	else if (strcmp(action, "-r") == 0)
	{
		printf("Recieving message(s).\n");
		char returnMsg[MAX_MESSAGE_SIZE];
		char fromUser[MAX_USRNAME_SIZE];
		size_t msgLen = 0;
		size_t fromLen = 0;
		
		int val = syscall(326, currUser, returnMsg, msgLen, fromUser, fromLen);
		if (val != 0)			//We got a message
			printf("%s said: \"%s\"\n", fromUser, returnMsg);

		while (val == 2)		//While there is another message
		{
			val = syscall(326, currUser, returnMsg, msgLen, fromUser, fromLen);
			if (val != 0)		//We got a message
				printf("%s said: \"%s\"\n", fromUser, returnMsg);			
		}
	}
	else	//Didnt do -s/-r
	{
		printf("usage: osmsg -s/-r user message");
		return -1;
	}
}
