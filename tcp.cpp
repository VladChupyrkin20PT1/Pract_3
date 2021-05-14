#include <iostream>
#include <string>
#include <cstdlib> 
#include <cstring> 
#include <unistd.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode )
{
	cout << "ErrorCode:"<<exitCode <<endl<< why << endl;
	exit(exitCode);
}
int main()
{
	sockaddr_in * Addr = new (sockaddr_in);
	Addr->sin_family = AF_INET; 
	Addr->sin_port = 0;         
	Addr->sin_addr.s_addr = 0;

	sockaddr_in * rAddr = new (sockaddr_in);
	rAddr->sin_family = AF_INET;    
	rAddr->sin_port = htons(7);  
	rAddr->sin_addr.s_addr = inet_addr("82.179.90.12"); 

	char *buffer = new char[4096];
	strcpy(buffer,"Hello World!");   
	int msgLen = strlen(buffer);         

	int mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == -1) {
		close(mySocket);
		Exception("Error open socket",11);
	}
	int rc = bind(mySocket,(const sockaddr *) Addr, sizeof(sockaddr_in));
	if (rc == -1) {
		close(mySocket);
		Exception("Error bind socket with local address",12);
	}

	rc = connect(mySocket, (const sockaddr*) rAddr, sizeof(sockaddr_in));
	if (rc == -1) {
		close(mySocket);
		Exception("Error connect socket with remote server.", 13);
	}

	rc = send(mySocket, buffer, msgLen, 0);
	if (rc == -1) {
		close(mySocket);
		Exception("Error send message", 14);
	}
	cout << "Send: " << buffer << endl;

	rc = recv(mySocket, buffer, 4096, 0);
	if (rc == -1) {
		close(mySocket);
		Exception("Error receive answer.", 15);
	}
	buffer[rc] = '\0'; 
	cout << "Receive: " << buffer << endl; 
	close(mySocket);

	delete Addr;
	delete rAddr;
	delete[] buffer;
	return 0;
}
