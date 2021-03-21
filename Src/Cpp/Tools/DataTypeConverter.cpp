#include "DataTypeConverter.h"

std::wstring DataTypeConverter::StringToWideString(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

