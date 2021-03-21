#include<comdef.h>

#include "ErrorLogger.h"


void ErrorLogger::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	//显示消息窗口，MessageBoxA 表示接受ASCII字符串（短字符串）
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + DataTypeConverter::StringToWideString(message) + L"\n" + error.ErrorMessage();
	//显示消息窗口，MessageBoxB 表示可以接受非ASCII字符串（宽字符串）
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " +message + L"\n" + error.ErrorMessage();
	//显示消息窗口，MessageBoxB 表示可以接受非ASCII字符串（宽字符串）
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(COMException& comException)
{
	std::wstring error_message = comException.GetSpecificMessage();
	//显示消息窗口，MessageBoxB 表示可以接受非ASCII字符串（宽字符串）
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

