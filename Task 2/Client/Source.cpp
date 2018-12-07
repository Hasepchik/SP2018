#define _CRT_SECURE_NO_WARNINGS

#include<conio.h>
#include<cstdlib>
#include<iostream>
#include<Windows.h>
using namespace std;

int main(int argc, char* argv[]) {

	LPSTR PipeName = 0;
	DWORD cbWritten;
	DWORD cbRead;
	HANDLE hNamedPipe = NULL;
	HANDLE rNamedPipe = NULL;

	char pipe[50] = { '\0' };
	char folderName[500] = { '\0' };
	char r[10] = { '\0' };
	int fullSize = 0;
	int j = 0;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-c") == 0)
			{
				strcat(pipe, "\\\\.\\pipe\\");
				strcat(pipe, argv[i + 1]);
				PipeName = pipe;
			}
		}

		cout << "Pipe name: " << PipeName << endl;
		hNamedPipe = CreateFile(PipeName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-d") == 0)
			{
				strcat(folderName, argv[i + 1]);
			}
			if (strcmp(argv[i], "-r") == 0)
			{
				strcat(r, argv[i]);
			}
		}
		if (folderName != '\0')
			WriteFile(hNamedPipe, folderName, strlen(folderName) + 1, &cbWritten, NULL);
		if (r != '\0')
			WriteFile(hNamedPipe, r, strlen(r) + 1, &cbWritten, NULL);

		ReadFile(hNamedPipe, &fullSize, sizeof(fullSize) + 1, &cbRead, NULL);

		cout << "fullSize = " << fullSize << endl;
		CloseHandle(hNamedPipe);
	}
}
