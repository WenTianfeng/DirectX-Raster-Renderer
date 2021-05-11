#include "CameraControl.h"

CameraControl::CameraControl() :
	m_viewingMode(false),
	m_moveSpeed(100.0f)
{
}

void CameraControl::Update(float dt)
{
	if (m_viewingMode)
	{
		if (Keyboard::GetInstance()->KeyIsPressed(VK_ESCAPE)) {
			m_viewingMode = false;
		}

		std::queue<MouseEvent> mouseEventBuffer = Mouse::GetInstance()->GetEventBuffer();

		while (!mouseEventBuffer.empty())
		{
			MouseEvent me = mouseEventBuffer.front();
			mouseEventBuffer.pop();

			//当发生RAW_MOVE
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				//相机自我旋转
				owner->GetComponent<Transform>()->Rotate(DirectX::XMFLOAT3((float)me.GetPosY() * dt / 2, (float)me.GetPosX() * dt / 2, 0));

				//限制相机旋转范围
				if (owner->GetComponent<Transform>()->GetRotation().x > 0.375f * DirectX::XM_PI) {
					owner->GetComponent<Transform>()->SetRotation(0.375f * DirectX::XM_PI,
						owner->GetComponent<Transform>()->GetRotation().y, owner->GetComponent<Transform>()->GetRotation().z);
				}

				if (owner->GetComponent<Transform>()->GetRotation().x < -0.375f * DirectX::XM_PI) {
					owner->GetComponent<Transform>()->SetRotation(-0.375f * DirectX::XM_PI,
						owner->GetComponent<Transform>()->GetRotation().y, owner->GetComponent<Transform>()->GetRotation().z);
				}
			}
		}

		//向前移动
		if (Keyboard::GetInstance()->KeyIsPressed('W'))
		{
			owner->GetComponent<Transform>()->Translate(owner->GetComponent<Transform>()->GetForwardAxis(), m_moveSpeed * dt);
		}
		//向后移动
		if (Keyboard::GetInstance()->KeyIsPressed('S'))
		{
			owner->GetComponent<Transform>()->Translate(owner->GetComponent<Transform>()->GetForwardAxis(), -m_moveSpeed * dt);
		}
		//向左移动
		if (Keyboard::GetInstance()->KeyIsPressed('A'))
		{
			owner->GetComponent<Transform>()->Translate(owner->GetComponent<Transform>()->GetRightAxis(), -m_moveSpeed * dt);
		}
		//向右移动
		if (Keyboard::GetInstance()->KeyIsPressed('D'))
		{
			owner->GetComponent<Transform>()->Translate(owner->GetComponent<Transform>()->GetRightAxis(), m_moveSpeed * dt);
		}
		//向上移动
		if (Keyboard::GetInstance()->KeyIsPressed('E'))
		{
			owner->GetComponent<Transform>()->Translate(DirectX::XMFLOAT3(0, 1, 0), m_moveSpeed * dt);
		}
		//向下移动
		if (Keyboard::GetInstance()->KeyIsPressed('Q'))
		{
			owner->GetComponent<Transform>()->Translate(DirectX::XMFLOAT3(0, 1, 0), -m_moveSpeed * dt);
		}
	}
}

void CameraControl::UpdateProperties(bool viewingMode, float moveSpeed)
{
	m_viewingMode = viewingMode;
	m_moveSpeed = moveSpeed;
}

bool CameraControl::GetViewingMode()
{
	return this->m_viewingMode;
}

float CameraControl::GetMoveSpeed()
{
	return this->m_moveSpeed;
}

void CameraControl::SetExamineMode(bool isExamineMode)
{
	this->m_viewingMode = isExamineMode;
}
