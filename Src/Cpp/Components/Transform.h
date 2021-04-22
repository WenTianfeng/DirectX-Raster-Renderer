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

	void UpdateProperties(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);


	DirectX::XMFLOAT3 GetScale() const;

	DirectX::XMFLOAT3 GetRotation() const;

	DirectX::XMFLOAT3 GetPosition() const;

	//���ؾֲ�����ϵx������
	DirectX::XMFLOAT3 GetRightAxis() const;

	//���ؾֲ�����ϵy������
	DirectX::XMFLOAT3 GetUpAxis() const;

	//���ؾֲ�����ϵz������
	DirectX::XMFLOAT3 GetForwardAxis() const;

	DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const;

	DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;

	DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const;

	DirectX::XMMATRIX GetWorldToLocalMatrixXM() const;

	// ���ö������ű���
	void SetScale(const DirectX::XMFLOAT3& scale);
	// ���ö������ű���
	void SetScale(float x, float y, float z);

	// ���ö���ŷ����(������)
	// ������Z-X-Y��˳����ת
	void SetRotation(const DirectX::XMFLOAT3& eulerAnglesInRadian);
	// ���ö���ŷ����(������)
	// ������Z-X-Y��˳����ת
	void SetRotation(float x, float y, float z);

	// ���ö���λ��
	void SetPosition(const DirectX::XMFLOAT3& position);
	// ���ö���λ��
	void SetPosition(float x, float y, float z);

	// ָ��ŷ������ת����
	void Rotate(const DirectX::XMFLOAT3& eulerAnglesInRadian);
	
	// ָ����ԭ��Ϊ����������ת
	void RotateAxis(const DirectX::XMFLOAT3& axis, float radian);
	// ָ����pointΪ��ת����������ת
	void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float radian);
	

	// ����ĳһ����ƽ��
	void Translate(const DirectX::XMFLOAT3& direction, float magnitude);

	// �۲�ĳһ��
	void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
	// ����ĳһ����۲�
	void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix);

};