
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO
		if (read(server,&req,sizeof(struct message))!=sizeof(struct message)) {
			continue;
		}
		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);
		struct message response;
		target = open(req.target, O_WRONLY);
		strcpy(response.msg,req.msg);
		strcpy(response.source,req.source);
		strcpy(response.target,req.target);
		write(target,&response,sizeof(struct message));
		close(target);
		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		// close target FIFO after writing the message
	}
	close(server);
	close(dummyfd);
	return 0;
}
