#pragma once
#include<comdef.h>
#include"DataTypeConverter.h"

//定义宏函数
#define COM_ERROR_IF_FAILED(hr , msg )if(FAILED(hr)) throw COMException(hr,msg,__FILE__,__FUNCTION__,__LINE__)


class COMException 
{
public:
	/// <summary>
	/// 构造函数（string版本），根据具体信息构造错误字段
	/// </summary>
	/// <param name="hr">HRESULT结果</param>
	/// <param name="msg">自定义错误信息</param>
	/// <param name="file">错误所在文件</param>
	/// <param name="function">错误所在方法</param>
	/// <param name="line">//错误所在代码行</param>
	COMException(HRESULT hr,const std::string& msg,const std::string& file,const std::string& function,int line)
	{
		_com_error error(hr);

		this->m_specificMsg = L"Message : " + DataTypeConverter::StringToWideString(msg)+ L"\nError : ";
		this->m_specificMsg += error.ErrorMessage();
		this->m_specificMsg += L"\nFile: " + DataTypeConverter::StringToWideString(file);
		this->m_specificMsg += L"\nFunction: " + DataTypeConverter::StringToWideString(function);
		this->m_specificMsg += L"\nLine: " + DataTypeConverter::StringToWideString(std::to_string(line));
	}

	/// <summary>
	/// 构造函数（wstring版本），根据具体信息构造错误字段
	/// </summary>
	/// <param name="hr">HRESULT结果</param>
	/// <param name="msg">自定义错误信息</param>
	/// <param name="file">错误所在文件</param>
	/// <param name="function">错误所在方法</param>
	/// <param name="line">//错误所在代码行</param>
	COMException(HRESULT hr, const std::wstring& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);

		this->m_specificMsg = L"Message : " + msg + L"\nError : ";
		this->m_specificMsg += error.ErrorMessage();
		this->m_specificMsg += L"\nFile: " + DataTypeConverter::StringToWideString(file);
		this->m_specificMsg += L"\nFunction: " + DataTypeConverter::StringToWideString(function);
		this->m_specificMsg += L"\nLine: " + DataTypeConverter::StringToWideString(std::to_string(line));
	}

	/// <summary>
	/// 获取错误消息
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetSpecificMessage() const
	{
		return this->m_specificMsg.c_str();
	}

private:
	std::wstring m_specificMsg;
};