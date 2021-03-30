#pragma once

#include<DirectXMath.h>

#include"Component.h"


class Transform : public Component
{
private:
	DirectX::XMFLOAT3 m_position = { 0,0,0 };
	DirectX::XMFLOAT3 m_rotation = { 0,0,0 };
	DirectX::XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };

public:

	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);

	~Transform() = default;


	DirectX::XMFLOAT3 GetScale() const;

	DirectX::XMFLOAT3 GetRotation() const;

	DirectX::XMFLOAT3 GetPosition() const;


	DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const;

	DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;

	DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const;

	DirectX::XMMATRIX GetWorldToLocalMatrixXM() const;

	// 设置对象缩放比例
	void SetScale(const DirectX::XMFLOAT3& scale);
	// 设置对象缩放比例
	void SetScale(float x, float y, float z);

	// 设置对象欧拉角(弧度制)
	// 对象将以Z-X-Y轴顺序旋转
	void SetRotation(const DirectX::XMFLOAT3& eulerAnglesInRadian);
	// 设置对象欧拉角(弧度制)
	// 对象将以Z-X-Y轴顺序旋转
	void SetRotation(float x, float y, float z);

	// 设置对象位置
	void SetPosition(const DirectX::XMFLOAT3& position);
	// 设置对象位置
	void SetPosition(float x, float y, float z);

	// 指定欧拉角旋转对象
	void Rotate(const DirectX::XMFLOAT3& eulerAnglesInRadian);
	
	// 指定以原点为中心绕轴旋转
	void RotateAxis(const DirectX::XMFLOAT3& axis, float radian);
	// 指定以point为旋转中心绕轴旋转
	void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float radian);
	

	// 沿着某一方向平移
	void Translate(const DirectX::XMFLOAT3& direction, float magnitude);

	// 观察某一点
	void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
	// 沿着某一方向观察
	void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix);

};