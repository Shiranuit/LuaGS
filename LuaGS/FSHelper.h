#pragma once

#include <iostream>
class FSHelper
{
public:
	static bool isDir(const std::string &path);
	static bool isReadOnly(const std::string &path);
	static unsigned long getSize(const std::string &path);
	static bool exists(const std::string &path);
	static bool makeDir(const std::string &path);
	static bool copyFile(const std::string &src, const std::string &dest);
};

