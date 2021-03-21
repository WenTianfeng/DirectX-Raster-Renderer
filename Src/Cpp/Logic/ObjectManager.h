#pragma once

#include<vector>

#include<DirectXMath.h>

#include"Object.h"
#include"Camera.h"
#include"Transform.h"
#include"..\Graphics\MeshRenderer.h"


class ObjectManager
{

public:
	bool Initialize();
	void RenderFrame();
private:
	bool InitializeScene();


private:
	std::vector<Object*> m_objects;


};
