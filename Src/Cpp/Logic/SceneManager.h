#pragma once

#include<vector>
#include<DirectXMath.h>
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）

#include"..\Tools\ErrorLogger.h"
#include"..\Object\Object.h"


class SceneManager
{

public:

	SceneManager();
	~SceneManager();

	/// <summary>
	/// 场景管理器初始化
	/// </summary>
	/// <param name="device">DirectX设备</param>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <returns>是否初始化成功</returns>
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// 每帧对象更新
	/// </summary>
	/// <param name="dt">单帧时间</param>
	void Update(float dt);

	/// <summary>
	/// 窗口缩放后调用，根据新窗口大小修改相关内容
	/// </summary>
	/// <param name="clientWidth">新窗口宽度</param>
	/// <param name="clientHeight">新窗口高度</param>
	void OnWindowResize(int clientWidth, int clientHeight);

private:
	/// <summary>
	/// 场景内容初始化
	/// </summary>
	/// <returns>相关内容是否初始化成功</returns>
	bool InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

public:
	static Object* mainCamera;//场景主相机
	static std::vector<Object*> lights;//场景光源列表
	static std::vector<Object*> objects;//场景对象列表

};