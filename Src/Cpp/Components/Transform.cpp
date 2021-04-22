#include"Transform.h"

Transform::Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale) :
	m_position(position),
	m_rotation(rotation),
	m_scale(scale)
{
	this->m_componentName = "Transform";
}

void Transform::UpdateProperties(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);
}

DirectX::XMFLOAT3 Transform::GetScale() const
{
	return this->m_scale;
}

DirectX::XMFLOAT3 Transform::GetRotation() const
{
	return this->m_rotation;
}

DirectX::XMFLOAT3 Transform::GetPosition() const
{
	return this->m_position;
}

DirectX::XMFLOAT3 Transform::GetRightAxis() const
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_rotation));
	DirectX::XMFLOAT3 right;
	DirectX::XMStoreFloat3(&right, R.r[0]);
	return right;
}

DirectX::XMFLOAT3 Transform::GetUpAxis() const
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_rotation));
	DirectX::XMFLOAT3 up;
	DirectX::XMStoreFloat3(&up, R.r[1]);
	return up;
}

DirectX::XMFLOAT3 Transform::GetForwardAxis() const
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_rotation));
	DirectX::XMFLOAT3 forward;
	DirectX::XMStoreFloat3(&forward, R.r[2]);
	return forward;
}

DirectX::XMFLOAT4X4 Transform::GetLocalToWorldMatrix() const
{
	DirectX::XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetLocalToWorldMatrixXM());
	return res;
}

DirectX::XMMATRIX Transform::GetLocalToWorldMatrixXM() const
{
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&this->m_scale);
	DirectX::XMVECTOR rotationVec = XMLoadFloat3(&this->m_rotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&this->m_position);
	DirectX::XMMATRIX World = DirectX::XMMatrixScalingFromVector(scaleVec) * DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVec) * DirectX::XMMatrixTranslationFromVector(positionVec);
	return World;
}


DirectX::XMFLOAT4X4 Transform::GetWorldToLocalMatrix() const
{
	DirectX::XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetWorldToLocalMatrixXM());
	return res;
}

DirectX::XMMATRIX Transform::GetWorldToLocalMatrixXM() const
{
	DirectX::XMMATRIX InvWorld = XMMatrixInverse(nullptr, GetLocalToWorldMatrixXM());
	return InvWorld;
}

// 设置对象缩放比例
void Transform::SetScale(const DirectX::XMFLOAT3 & scale)
{
	this->m_scale = scale;
}
// 设置对象缩放比例
void Transform::SetScale(float x, float y, float z)
{
	this->m_scale = DirectX::XMFLOAT3(x, y, z);
}

// 设置对象欧拉角(弧度制)
// 对象将以Z-X-Y轴顺序旋转
void Transform::SetRotation(const DirectX::XMFLOAT3 & eulerAnglesInRadian)
{
	this->m_rotation = eulerAnglesInRadian;
}
// 设置对象欧拉角(弧度制)
// 对象将以Z-X-Y轴顺序旋转
void Transform::SetRotation(float x, float y, float z)
{
	this->m_rotation = DirectX::XMFLOAT3(x, y, z);
}

// 设置对象位置
void Transform::SetPosition(const DirectX::XMFLOAT3 & position)
{
	this->m_position = position;
}
// 设置对象位置
void Transform::SetPosition(float x, float y, float z)
{
	this->m_position = DirectX::XMFLOAT3(x, y, z);
}

// 指定欧拉角旋转对象
void Transform::Rotate(const DirectX::XMFLOAT3 & eulerAnglesInRadian)
{
	DirectX::XMVECTOR newRotationVec = DirectX::XMVectorAdd(XMLoadFloat3(&this->m_rotation), XMLoadFloat3(&eulerAnglesInRadian));
	XMStoreFloat3(&this->m_rotation, newRotationVec);
}

// 指定以原点为中心绕轴旋转
void Transform::RotateAxis(const DirectX::XMFLOAT3 & axis, float radian)
{
	DirectX::XMVECTOR rotationVec = XMLoadFloat3(&this->m_rotation);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVec) * DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
	DirectX::XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, R);
	this->m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}
// 指定以point为旋转中心绕轴旋转
void Transform::RotateAround(const DirectX::XMFLOAT3 & point, const DirectX::XMFLOAT3 & axis, float radian)
{
	DirectX::XMVECTOR rotationVec = XMLoadFloat3(&this->m_rotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&this->m_position);
	DirectX::XMVECTOR centerVec = XMLoadFloat3(&point);


	// 以point作为原点进行旋转
	DirectX::XMMATRIX RT = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVec) *
		DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSubtract(positionVec, centerVec));

	RT *= DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
	RT *= DirectX::XMMatrixTranslationFromVector(centerVec);
	DirectX::XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, RT);
	this->m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
	XMStoreFloat3(&this->m_position, RT.r[3]);
}


// 沿着某一方向平移
void Transform::Translate(const DirectX::XMFLOAT3 & direction, float magnitude)
{
	DirectX::XMVECTOR directionVec = DirectX::XMVector3Normalize(XMLoadFloat3(&direction));
	DirectX::XMVECTOR newPosition = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(magnitude), directionVec, XMLoadFloat3(&this->m_position));
	XMStoreFloat3(&this->m_position, newPosition);
}

// 观察某一点
void Transform::LookAt(const DirectX::XMFLOAT3 & target, const DirectX::XMFLOAT3 & up)
{
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&this->m_position), XMLoadFloat3(&target), XMLoadFloat3(&up));
	DirectX::XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	this->m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}
// 沿着某一方向观察
void Transform::LookTo(const DirectX::XMFLOAT3 & direction, const DirectX::XMFLOAT3 & up)
{
	DirectX::XMMATRIX View = DirectX::XMMatrixLookToLH(XMLoadFloat3(&this->m_position), XMLoadFloat3(&direction), XMLoadFloat3(&up));
	DirectX::XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	this->m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

DirectX::XMFLOAT3 Transform::GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4 & rotationMatrix)
{
	float c = sqrtf(1.0f - rotationMatrix(2, 1) * rotationMatrix(2, 1));
	if (isnan(c))
		c = 0.0f;
	DirectX::XMFLOAT3 rotation = {};
	rotation.z = atan2f(rotationMatrix(0, 1), rotationMatrix(1, 1));
	rotation.x = atan2f(-rotationMatrix(2, 1), c);
	rotation.y = atan2f(rotationMatrix(2, 0), rotationMatrix(2, 2));
	return rotation;
}