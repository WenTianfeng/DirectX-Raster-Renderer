#pragma once
#include<comdef.h>
#include"DataTypeConverter.h"

//����꺯��
#define COM_ERROR_IF_FAILED(hr , msg )if(FAILED(hr)) throw COMException(hr,msg,__FILE__,__FUNCTION__,__LINE__)


class COMException 
{
public:
	/// <summary>
	/// ���캯����string�汾�������ݾ�����Ϣ��������ֶ�
	/// </summary>
	/// <param name="hr">HRESULT���</param>
	/// <param name="msg">�Զ��������Ϣ</param>
	/// <param name="file">���������ļ�</param>
	/// <param name="function">�������ڷ���</param>
	/// <param name="line">//�������ڴ�����</param>
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
	/// ���캯����wstring�汾�������ݾ�����Ϣ��������ֶ�
	/// </summary>
	/// <param name="hr">HRESULT���</param>
	/// <param name="msg">�Զ��������Ϣ</param>
	/// <param name="file">���������ļ�</param>
	/// <param name="function">�������ڷ���</param>
	/// <param name="line">//�������ڴ�����</param>
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
	/// ��ȡ������Ϣ
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetSpecificMessage() const
	{
		return this->m_specificMsg.c_str();
	}

private:
	std::wstring m_specificMsg;
};