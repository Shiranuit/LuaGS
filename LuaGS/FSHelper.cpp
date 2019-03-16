#include "FSHelper.h"
#include <Windows.h>
#include <tchar.h>
#include <string>

bool FSHelper::isDir(const std::string &path)
{
	DWORD ftyp = GetFileAttributesA(path.c_str());

	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		return false;
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		return true;
	}

	return false;
}

bool FSHelper::exists(const std::string &file)
{
	WCHAR wName[_MAX_PATH];

	DWORD ftyp = GetFileAttributesA(file.c_str());

	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	return true;
}

unsigned long FSHelper::getSize(const std::string &file)
{
	WCHAR wName[_MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), _MAX_PATH, wName, _MAX_PATH);

	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(wName, GetFileExInfoStandard, &fad)) {
		return -1;
	}
	LARGE_INTEGER size;
	size.HighPart = fad.nFileSizeHigh;
	size.LowPart = fad.nFileSizeLow;
	return size.QuadPart;
}

bool FSHelper::makeDir(const std::string &dir)
{
	WCHAR wName[_MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, dir.c_str(), _MAX_PATH, wName, _MAX_PATH);

	return CreateDirectory(wName, NULL)==0;
}

bool FSHelper::copyFile(const std::string &src, const std::string &dest)
{
	WCHAR wSrc[_MAX_PATH];
	WCHAR wDest[_MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), _MAX_PATH, wSrc, _MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, dest.c_str(), _MAX_PATH, wDest, _MAX_PATH);

	return CopyFile(wSrc, wDest, TRUE)==0;
}

bool FSHelper::isReadOnly(const std::string &path)
{
	DWORD ftyp = GetFileAttributesA(path.c_str());

	if (ftyp != INVALID_FILE_ATTRIBUTES) {
		return ftyp & FILE_ATTRIBUTE_READONLY;
	}
	return false;
}