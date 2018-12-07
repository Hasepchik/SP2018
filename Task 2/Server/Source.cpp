#define _CRT_SECURE_NO_WARNINGS

#include<conio.h>
#include<cstdlib>
#include<iostream>
#include<Windows.h>
using namespace std;

int directorySize(char firstName[], char r[])
{
	_WIN32_FIND_DATAA dataStruct;
	HANDLE hData;
	DWORD size = 0;
	int fullSize = 0;

	char fName[1000] = { '\0' };

	strcat(fName, firstName);
	strcat(fName, "\\*");

	hData = FindFirstFile(fName, &dataStruct);

	FindNextFile(hData, &dataStruct);
	while (FindNextFile(hData, &dataStruct) != 0)
	{
		if ((dataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(r, "-r") == 0))
		{
			char fullname[1000] = { '\0' };
			char* name = dataStruct.cFileName;
			strcat(fullname, firstName);
			strcat(fullname, "\\");
			strcat(fullname, name);
			int folderSize = directorySize(fullname, r);

			cout << "Folder Name = " << fullname << endl;
			cout << "Folder Size = " << folderSize << endl << endl;

			fullSize += folderSize;
		}
		else if (dataStruct.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			char* name = dataStruct.cFileName;
			size = dataStruct.nFileSizeLow;
			fullSize += size;
			cout << name << " = " << size << endl;
		}
		else if ((dataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(r, "-r") != 0))
		{
			size = dataStruct.nFileSizeLow;
			fullSize += size;
		}
	}
	FindClose(hData);
	return fullSize;
}

int main(int argc, char *argv[]) {
	HANDLE hNamedPipe;
	HANDLE hData;
	WIN32_FIND_DATA dataStruct;
	LPSTR PipeName = NULL;
	DWORD cbRead;
	DWORD cbWritten;
	
	char buf[50] = { '\0' };
	char folderName[300] = { '\0' };
	char r[10] = { '\0' };
	int fullSize = 0;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-c") == 0)
		{
			strcat(buf, "\\\\.\\pipe\\");
			strcat(buf, argv[2]);
			PipeName = buf;
		}
	}
	
	cout << "Pipe name: " << PipeName << endl;
	cout << "Wait for client" << endl;

	hNamedPipe = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);
	ConnectNamedPipe(hNamedPipe, NULL);

	ReadFile(hNamedPipe, folderName, 300, &cbRead, NULL);
	ReadFile(hNamedPipe, r, 10, &cbRead, NULL);

	hData = FindFirstFile(folderName, &dataStruct);

	if ((dataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		fullSize = directorySize(folderName, r);
		cout << endl << "Full size = " << fullSize << endl;
		WriteFile(hNamedPipe, &fullSize, sizeof(fullSize) + 1, &cbWritten, NULL);
	}

	else if (dataStruct.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		char* name = dataStruct.cFileName;
		fullSize = dataStruct.nFileSizeLow;
		cout << name << " = " << fullSize << endl;
		WriteFile(hNamedPipe, &fullSize, sizeof(fullSize) + 1, &cbWritten, NULL);
	}

	WriteFile(hNamedPipe, &fullSize, sizeof(fullSize) + 1, &cbWritten, NULL);

	CloseHandle(hNamedPipe);
}