/********************************************************************************
  * @file    DataTypeConverter.h
  * @author  WenTianfeng
  * @version v1.0
  * @date    25-2-2021
  * @brief   Class DataTypeConverter`s definition
  ******************************************************************************
  * @attention
  ******************************************************************************
  */


#pragma once

#include<string>

class DataTypeConverter 
{
public:
	/// <summary>
	/// 将字符串转换为宽字符串(string -> wstring)
	/// </summary>
	/// <param name="str">string 类型变量</param>
	/// <returns>wstring 类型转换结果</returns>
	static std::wstring StringToWideString(std::string str);
};