/*****************************************************
 * Author: Ai Hongwei 
 * Email: ufo008ahw@163.com
 * Last modified: 2015-08-27 02:38
 * Filename: client.cpp
 * Description: client part
 ****************************************************/

#include <cstdio>
#include <iostream>
#include <cassert>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <stropts.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/errno.h>
using namespace std;

#define bufferSize 1024

void passMsg(int sockfd, struct addrinfo *aip) {


	if (ioctl(sockfd, I_SENDFD, 1) < 0) {
		perror("client");
	}

	char sendBuf[bufferSize], recvBuf[bufferSize];
	memset(sendBuf, 0, sizeof(sendBuf));
	memset(recvBuf, 0, sizeof(recvBuf));
	int n;

	while(strcmp(sendBuf, "end") != 0) {
		cout << "send: ";
		cin >> sendBuf;
		
		assert(sendBuf[bufferSize - 1] == '\0');
		if (sendto(sockfd, sendBuf, strlen(sendBuf), 0, aip->ai_addr, aip->ai_addrlen) < 0) {
			perror("send error");
		}

		if (strcmp(sendBuf, "end") != 0) {
			if ((n = recvfrom(sockfd, recvBuf, bufferSize, 0, NULL, NULL)) < 0) {
				perror("receive error");
			}
			recvBuf[n] = '\0';
			cout << "receive: " << recvBuf << endl;
		}

	}
}

int main(int argc,char **argv)
{
	// get socket
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() error!\n");
		exit(1);
	}


	// get client addinfo
	struct addrinfo hint;
	struct addrinfo *adList, *adPtr;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_CANONNAME;
	hint.ai_protocol = 0;
	hint.ai_canonname = 0;
	hint.ai_addrlen = 0;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	// argv[1]: host, argv[2]: serve
	int temp;
	if ( (temp = getaddrinfo(argv[1], argv[2], &hint, &adList)) != 0) {
		perror("Get addr info error\n");
		char aBuf[256];

		perror("ccc");
		cout << gai_strerror(temp) << endl;
		
		struct sockaddr_in *sinp = (sockaddr_in *)adList->ai_addr;
		
		cout << (adList == NULL) << endl;
		//cout << (char *)sinp->sin_addr << endl;
		//inet_ntop(AF_INET, &sinp->sin_addr, aBuf, 256);
		/*
		cout << c << endl;
		*/
		cout << "fdsafds" << endl;

		exit(1);
	}
	if (adList == NULL) {
		perror("no useable adress\n");
		exit(1);	
	}

	// stream data need connect
	if (adList->ai_socktype == SOCK_STREAM) {
		if(connect(sockfd, adList->ai_addr, sizeof(*(adList->ai_addr))) != 0) {
			perror("connect error");
			
			char aBuf[256];
			struct sockaddr_in *sinp = (sockaddr_in *)adList->ai_addr;
			inet_ntop(adList->ai_family, &sinp->sin_addr, aBuf, 256);
			cout << aBuf << endl;
			cout << ntohs(sinp->sin_port) << endl;
			cout << "fdsafds" << endl;
			exit(1);
		}
	}
	
	// pass message
	passMsg(sockfd, adList);

	// close socket
	close(sockfd);
	printf("finished\n");
	return 0;
}        

