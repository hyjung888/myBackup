#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "shlwapi")

#include <fstream>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <locale>
#include <Shlwapi.h>
using namespace std;
#define BUFSIZ 512
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);
void checkAndMakeDirectory(LPCWSTR newDirect);
void copyDirectoryAndFile(char* oldPath, char* newPath, ofstream& fout);
//HANDLE logHandle;
int main(int argc, char* argv[]) {
	//한글출력
	wcout.imbue(locale("kor"));
	
	wchar_t oldDir[BUFSIZ];
	wchar_t newDir[BUFSIZ];
	wchar_t logDir[BUFSIZ];
	//logHandle=CreateFile(logDir, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	ofstream fout("myBackup.log");
	//argc = 3;
	if (argc != 3) {
		cout << "입력 인자갯수가 잘못되었습니다" << endl;
		return -1;
	}

	//argv[1] = "C:\\asd\\1.txt";
	//argv[2] = "C:\\asd\\2";

	CharToWChar(argv[1], oldDir);
	CharToWChar(argv[2], newDir);

	char out[BUFSIZ];

	if ((strlen(argv[1])<4 || argv[1][strlen(argv[1]) - 4] == '.') &&(strlen(argv[1])<4 || argv[2][strlen(argv[1]) - 4] == '.')) {
		CopyFile(oldDir, newDir, NULL);
		fout << argv[1] << " 를 " << argv[2] << "로 복사하였습니다" << endl;
	}
	else if ((strlen(argv[1]) < 4 || argv[1][strlen(argv[1]) - 4] == '.')) {
		checkAndMakeDirectory(newDir);
		WIN32_FIND_DATA  findOldFileData;
		HANDLE hOldFileHandle;
		hOldFileHandle = FindFirstFile(oldDir, &findOldFileData);
		wcscat(newDir, L"\\");
		wcscat(newDir, findOldFileData.cFileName);
		CopyFile(oldDir, newDir, NULL);
		WCharToChar(newDir, out);
		fout << argv[1] << " 를 " << out << "로 복사하였습니다" << endl;
	}
	else {

		copyDirectoryAndFile(argv[1], argv[2],fout);
		/*
		char m_oldDir[250];
		char m_newDir[250];
		wchar_t newDirect[250];
		strcpy(m_oldDir, argv[1]);
		strcat(m_newDir, "\\*.*");
		CharToWChar(m_oldDir, oldDir);
		strcpy(m_newDir, argv[2]);
		strcat(m_newDir, "\\*.*");
		CharToWChar(m_oldDir, oldDir);
		CharToWChar(m_newDir, newDir);
		CharToWChar(argv[2], newDirect);


		WIN32_FIND_DATA  findOldFileData;
		HANDLE hOldFileHandle;

		WIN32_FIND_DATA  findNewFileData;
		HANDLE hNewFileHandle;

		//복사지점 폴더유무 확인

		checkAndMakeDirectory(newDirect);

		hOldFileHandle = FindFirstFile(oldDir, &findOldFileData);
		hNewFileHandle = FindFirstFile(newDir, &findNewFileData);

		if (hOldFileHandle != INVALID_HANDLE_VALUE)   // 파일을 찾은 경우
		{
			FindNextFile(hOldFileHandle, &findOldFileData);
			FindNextFile(hNewFileHandle, &findNewFileData);
			while (FindNextFile(hOldFileHandle, &findOldFileData))
			{
				char name[260];
				WCharToChar(findOldFileData.cFileName, name);
				wcout << findOldFileData.cFileName << endl;
			}
			FindClose(hOldFileHandle);
		}

	*/

		
	}

	system("pause");

	return 1;
}




// char to LPCWSTR
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	size_t convertedChars = 0;
	setlocale(LC_ALL, "KOREAN");
	size_t nLen = strlen(pstrSrc) + 1;
	mbstowcs_s(&convertedChars, pwstrDest, nLen, pstrSrc, _TRUNCATE);
}
// LPCWSTR to char
void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	size_t convertedChars = 0;
	size_t nLen = (wcslen(pwstrSrc) * 2) + 1;
	setlocale(LC_ALL, "KOREAN");
	wcstombs_s(&convertedChars, pstrDest, nLen, pwstrSrc, _TRUNCATE);
}
//if directory is not exist, make directory
void checkAndMakeDirectory(LPCWSTR newDirect) {
	//복사지점 폴더유무 확인
	int flag = PathFileExists(newDirect);
	if (flag != 1)
	{
		CreateDirectory(newDirect, NULL);
	}
}
void copyDirectoryAndFile(char* oldPath, char* newPath,ofstream& fout) {

	char m_oldDir[BUFSIZ];
	char m_newDir[BUFSIZ];

	wchar_t oldDir[BUFSIZ];
	wchar_t newDir[BUFSIZ];
	wchar_t newDirect[BUFSIZ];


	strcpy(m_oldDir, oldPath);
	strcat(m_oldDir, "\\*.*");
	CharToWChar(m_oldDir, oldDir);
	strcpy(m_newDir, newPath);
	strcat(m_newDir, "\\*.*");
	CharToWChar(m_oldDir, oldDir);
	CharToWChar(m_newDir, newDir);
	CharToWChar(newPath, newDirect);


	WIN32_FIND_DATA  findOldFileData;
	HANDLE hOldFileHandle;
	hOldFileHandle = FindFirstFile(oldDir, &findOldFileData);

	WIN32_FIND_DATA  findNewFileData;
	HANDLE hNewFileHandle;

	//복사지점 폴더유무 확인

	checkAndMakeDirectory(newDirect);

	hNewFileHandle = FindFirstFile(newDir, &findNewFileData);

	if (hOldFileHandle != INVALID_HANDLE_VALUE)   // 파일을 찾은 경우
	{
		int oldflag = FindNextFile(hOldFileHandle, &findOldFileData);
		int newflag = FindNextFile(hNewFileHandle, &findNewFileData);
		oldflag = FindNextFile(hOldFileHandle, &findOldFileData);
		newflag = FindNextFile(hNewFileHandle, &findNewFileData);
		while (oldflag != 0)
		{

			char name[260];
			WCharToChar(findOldFileData.cFileName, name);
			//wcout << findOldFileData.cFileName << endl;
			if (findOldFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
			{

				char newOldPath[BUFSIZ];
				strcpy(newOldPath, oldPath);
				strcat(newOldPath, "\\");
				strcat(newOldPath, name);
				char newNewPath[BUFSIZ];
				strcpy(newNewPath, newPath);
				strcat(newNewPath, "\\");
				strcat(newNewPath, name);

				copyDirectoryAndFile(newOldPath, newNewPath,fout);
				oldflag = FindNextFile(hOldFileHandle, &findOldFileData);
			}
			else if (newflag == 1 && wcscmp(findOldFileData.cFileName, findNewFileData.cFileName) > 0) {
				newflag = FindNextFile(hNewFileHandle, &findNewFileData);

			}
			else if (newflag == 1 && wcscmp(findOldFileData.cFileName, findNewFileData.cFileName) < 0) {
				oldflag = FindNextFile(hOldFileHandle, &findOldFileData);
			}
			else if (newflag == 0 || wcscmp(findOldFileData.cFileName, findNewFileData.cFileName) == 0) {
				if (newflag == 0 || findOldFileData.ftLastWriteTime.dwHighDateTime > findNewFileData.ftLastWriteTime.dwHighDateTime) {
					wchar_t w_newOldPath[BUFSIZ];
					wchar_t w_newNewPath[BUFSIZ];
					CharToWChar(oldPath, w_newOldPath);
					wcscat(w_newOldPath, L"\\");
					wcscat(w_newOldPath, findOldFileData.cFileName);
					CharToWChar(newPath, w_newNewPath);
					wcscat(w_newNewPath, L"\\");
					wcscat(w_newNewPath, findOldFileData.cFileName);

					char newOldPath[BUFSIZ];
					char newNewPath[BUFSIZ];
					WCharToChar(w_newOldPath, newOldPath);
					WCharToChar(w_newNewPath, newNewPath);
					CopyFile(w_newOldPath, w_newNewPath, NULL);
					DWORD res = 0;
					fout << newOldPath <<" 를 "<<newNewPath<<"로 복사하였습니다"<< endl;
				}
				oldflag = FindNextFile(hOldFileHandle, &findOldFileData);

			}

		}
		FindClose(hOldFileHandle);
	}

}

