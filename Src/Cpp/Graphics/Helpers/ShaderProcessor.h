#pragma once

#include <d3d11_1.h>
#include <wrl/client.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#include"..\Structures\Shader.h"
#include"..\..\Tools\DataTypeConverter.h"

class ShaderProcessor
{
public:
	bool ShaderPreProcess(ID3DBlob* blob);
	ID3D11InputLayout* ObtainInputLayout();
private:


};