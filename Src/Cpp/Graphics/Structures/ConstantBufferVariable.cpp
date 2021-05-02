#include "ConstantBufferVariable.h"

ConstantBufferVariable::ConstantBufferVariable(std::string pname, UINT pstartOffset, UINT psize, D3D_SHADER_VARIABLE_CLASS pvariableClass, D3D_SHADER_VARIABLE_TYPE pvariableType):
	name(pname),
	startOffset(pstartOffset),
	size(psize),
	variableClass(pvariableClass),
	variableType(pvariableType)
{
}
