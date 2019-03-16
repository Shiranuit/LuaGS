#include "FileSystem.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "FSHelper.h"

void FileSystem::init(lua_State *L)
{
	if (L == nullptr) {
		std::cout << "Error Loading FileSytemAPI" << std::endl;
		exit(84);
	}
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "fs");

	LuaValueHelper::pushCFuncToTable(L, "list", list);
	LuaValueHelper::pushCFuncToTable(L, "isDir", isDir);
	LuaValueHelper::pushCFuncToTable(L, "exists", exists);
	LuaValueHelper::pushCFuncToTable(L, "getSize", getSize);
	LuaValueHelper::pushCFuncToTable(L, "getPath", getPath);
	LuaValueHelper::pushCFuncToTable(L, "makeDir", makeDir);
	LuaValueHelper::pushCFuncToTable(L, "copy", copy);
	LuaValueHelper::pushCFuncToTable(L, "isReadOnly", isReadOnly);
}

int FileSystem::list(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}
	std::string pattern(lua_tostring(L, -1));
	if (pattern.length() == 0)
		pattern.append(".");
	pattern.append("\\*");
	WCHAR wName[_MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, pattern.c_str(), _MAX_PATH, wName, _MAX_PATH);

	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(wName, &data)) != INVALID_HANDLE_VALUE) {
		lua_newtable(L);
		int count = 1;
		do {
			char resultName[_MAX_PATH];
			WideCharToMultiByte(CP_UTF8, 0, data.cFileName, _MAX_PATH, resultName, _MAX_PATH, NULL, NULL);
			lua_pushinteger(L, count);
			lua_pushstring(L, resultName);
			lua_settable(L, -3);
			count += 1;
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
		return 1;
	}
	return 0;
}

int FileSystem::isDir(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}

	std::string dirName(lua_tostring(L, 1));

	lua_pushboolean(L, FSHelper::isDir(dirName)==true ? 1 : 0);
	return 1;
}

int FileSystem::isReadOnly(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}

	std::string dirName(lua_tostring(L, 1));

	lua_pushboolean(L, FSHelper::isReadOnly(dirName)==true ? 1 : 0);
	return 1;
}

int FileSystem::exists(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}
	std::string dirName(lua_tostring(L, -1));

	DWORD ftyp = GetFileAttributesA(dirName.c_str());

	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int FileSystem::getSize(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}

	WCHAR wName[_MAX_PATH];
	std::string path(lua_tostring(L, 1));

	MultiByteToWideChar(CP_UTF8, 0, path.c_str(), _MAX_PATH, wName, _MAX_PATH);

	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(wName, GetFileExInfoStandard, &fad)) {
		luaL_error(L, "File not found: %s\n", path);
		return 0;
	}
	LARGE_INTEGER size;
	size.HighPart = fad.nFileSizeHigh;
	size.LowPart = fad.nFileSizeLow;
	lua_pushinteger(L, size.QuadPart);
	return 1;
}

int FileSystem::getPath(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}

	WCHAR wName[_MAX_PATH];
	WCHAR wResult[_MAX_PATH];
	char result[_MAX_PATH];
	std::string path(lua_tostring(L, 1));

	MultiByteToWideChar(CP_UTF8, 0, path.c_str(), _MAX_PATH, wName, _MAX_PATH);

	int retval = GetFullPathName(wName, _MAX_PATH, wResult, 0);


	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (retval == 0) {
		luaL_error(L, "File not found: %s\n", path);
		return 0;
	}
	WideCharToMultiByte(CP_UTF8, 0, wResult, _MAX_PATH, result, _MAX_PATH, NULL, NULL);

	lua_pushstring(L, result);
	return 1;
}

int FileSystem::makeDir(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) {
		luaL_error(L, "Argument #1 String expected\n");
		return 0;
	}

	WCHAR wName[_MAX_PATH];
	WCHAR wResult[_MAX_PATH];

	std::string path(lua_tostring(L, 1));

	MultiByteToWideChar(CP_UTF8, 0, path.c_str(), _MAX_PATH, wName, _MAX_PATH);


	if (!CreateDirectory(wName, NULL))
	{
		if (ERROR_ALREADY_EXISTS == GetLastError())
			luaL_error(L, "File exists\n");
		else
			luaL_error(L, "Error while creating directory %s\n", path.c_str());
		return 0;
	}
	return 0;
}

int FileSystem::copy(lua_State *L)
{
	if (L == nullptr)
		return 0;
	if (lua_gettop(L) < 2 || !lua_isstring(L, 1)|| !lua_isstring(L, 2)) {
		luaL_error(L, "Argument [String]Source, [String]Destination expected\n");
		return 0;
	}

	WCHAR wSrc[_MAX_PATH];
	WCHAR wDest[_MAX_PATH];
	
	std::string src(lua_tostring(L, 1));
	std::string dest(lua_tostring(L, 2));

	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), _MAX_PATH, wSrc, _MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, dest.c_str(), _MAX_PATH, wDest, _MAX_PATH);

	if (!CopyFile(wSrc, wDest, TRUE))
	{
		if (ERROR_ALREADY_EXISTS == GetLastError())
			luaL_error(L, "File exists\n");
		else if (ERROR_FILE_NOT_FOUND == GetLastError())
			luaL_error(L, "File not found\n");
		else
			luaL_error(L, "Error while copying file %s to %s\n", src.c_str(), dest.c_str());
		return 0;
	}
	return 0;
}