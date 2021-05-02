#pragma once

#include<string>
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��
#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��


class ConstantBufferVariable
{
public:
	ConstantBufferVariable(std::string pname, UINT pstartOffset, UINT psize, D3D_SHADER_VARIABLE_CLASS pvariableClass, D3D_SHADER_VARIABLE_TYPE pvariableType);

public:
	std::string name;//������
	UINT startOffset;//��ʼƫ��ֵ
	UINT size;//��С
	D3D_SHADER_VARIABLE_CLASS variableClass;//˵�����Ǳ�����ʸ�������󡢶��󣬻�������
	D3D_SHADER_VARIABLE_TYPE variableType;//˵������BOOL��INT��FLOAT�����Ǳ������

};