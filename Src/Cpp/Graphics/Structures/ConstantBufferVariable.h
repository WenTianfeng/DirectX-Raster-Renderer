#pragma once

#include<string>
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）


class ConstantBufferVariable
{
public:
	ConstantBufferVariable(std::string pname, UINT pstartOffset, UINT psize, D3D_SHADER_VARIABLE_CLASS pvariableClass, D3D_SHADER_VARIABLE_TYPE pvariableType);

public:
	std::string name;//变量名
	UINT startOffset;//起始偏移值
	UINT size;//大小
	D3D_SHADER_VARIABLE_CLASS variableClass;//说明它是标量、矢量、矩阵、对象，还是类型
	D3D_SHADER_VARIABLE_TYPE variableType;//说明它是BOOL、INT、FLOAT，还是别的类型

};