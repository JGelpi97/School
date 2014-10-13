#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1000

typedef struct Node Node;
struct
{
    char from[100];
	char msg[MAX_MESSAGE_SIZE];
	char to[100];
    Node *next;
} Node;


Node *head = NULL;
Node *last = NULL;


long sys_cs1550_send_msg(const char *to, const char *msg, const char *from)
{	
	if (head == NULL)		//If its nothing make it something
	{
		head = malloc(sizeof(Node));
		last = head;
		strncpy(head->msg, msg, MAX_MESSAGE_SIZE);
		strncpy(head->from, from, 100);
		strncpy(head->to, to, 100);		
	}
	else
	{
		last->next = malloc(sizeof(Node));	
		last = last->next;
		strncpy(last->msg, msg, MAX_MESSAGE_SIZE);
		strncpy(last->from, from, 100);
		strncpy(last->to, to, 100);		
	}
	
}

int main(int argc, char *argv[])
{
	char *to = "Joey";
	char *msg = "FUCK THIS";
	char *from = "Bitch";
	sys_cs1550_send_msg(to, msg, from);

}
