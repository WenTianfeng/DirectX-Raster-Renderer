#pragma once

#include<Windows.h>
#include"DataTypeConverter.h"
#include"COMException.h"

class ErrorLogger 
{
public:
	/// <summary>
	/// 只显示指定文本消息
	/// </summary>
	/// <param name="message">指定文本消息</param>
	static void Log(std::string message);

	/// <summary>
	/// 显示指定文本消息以及HRESULT错误原因
	/// </summary>
	/// <param name="hr">HRESULT结果</param>
	/// <param name="message">指定文本消息</param>
	static void Log(HRESULT hr, std::string message);

	/// <summary>
	/// 显示指定文本消息以及HRESULT错误原因
	/// </summary>
	/// <param name="hr">HRESULT结果</param>
	/// <param name="message">指定文本消息</param>
	static void Log(HRESULT hr, std::wstring message);

	/// <summary>
	/// 显示捕捉到的exception错误信息
	/// </summary>
	/// <param name="comException"></param>
	static void Log(COMException& comException);
};