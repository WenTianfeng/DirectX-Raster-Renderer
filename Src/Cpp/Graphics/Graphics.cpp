#include "Graphics.h"

#include"ShaderProcessor.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	//初始化DirectX
	if (!InitializeDirectX(hwnd, width, height)) 
	{
		return false;
	}

	
	if (!InitializeEffect()) 
	{
		return false;
	}

	if (!InitializeScene())
	{
		return false;
	}

	if (!InitializeUI(hwnd)) {
		return false;
	}

	return true;
}


bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
//===================创建DirectX设备和交换链==========================
	//交换链信息描述结构体
	DXGI_SWAP_CHAIN_DESC scd = {};
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;//设置为1  则拥有两个缓冲
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;//每像素多重采样次数
	scd.SampleDesc.Quality = 0;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = true;

	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.RefreshRate.Numerator = 60;

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(
			nullptr,    //选择使用的显卡设备  设为 nullptr 交由上层驱动来决定使用哪个显卡
			D3D_DRIVER_TYPE_HARDWARE,     //驱动类型  如果Adapter设为nullptr  则此处设为D3D_DRIVER_TYPE_HARDWARE
			nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr,    //特性等级数组
			0,    
			D3D11_SDK_VERSION,
			&scd,    //交换链信息描述
			m_dxSwapChain.GetAddressOf(),
			m_dxDevice.GetAddressOf(),
			nullptr,    //得到的可用的特性等级
			m_dxDeviceContext.GetAddressOf()
		);

	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Device and Swap Chain.");


//===================创建并获取 backBuffer==========================
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to get Buffer from Swap Chain.");

	//创建 渲染目标视图
	hr = m_dxDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_dxRenderTargetView.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Render Target View.");


//===================创建 深度缓冲/模板缓冲 视图===========================
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//创建2D缓冲
	hr = m_dxDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_dxDepthStencilBuffer.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Depth/Stencil Buffer.");

	//创建深度模板缓冲视图
	hr = m_dxDevice->CreateDepthStencilView(m_dxDepthStencilBuffer.Get(), nullptr, m_dxDepthStencilView.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Depth-Stencil View.");

	//设置渲染目标
	m_dxDeviceContext->OMSetRenderTargets(1, m_dxRenderTargetView.GetAddressOf(), m_dxDepthStencilView.Get());


//===================设置视口========================
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Height = (float)height;
	vp.Width = (float)width;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	//设置视口
	m_dxDeviceContext->RSSetViewports(1, &vp);


//===================设置光栅化状态=======================
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;
	
	//创建光栅化状态
	hr = m_dxDevice->CreateRasterizerState(&rd, m_dxRasterizerState.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Rasterizer State.");

	//设置光栅化状态
	m_dxDeviceContext->RSSetState(m_dxRasterizerState.Get());

	return true;
}

bool Graphics::InitializeEffect()
{

#pragma region 判定shader文件路径
	std::wstring shaderFolderPath;
	if (IsDebuggerPresent()) 
	{
#ifdef _DEBUG
		shaderFolderPath = L"Debug\\";
#else //RELEASE（待修改）
		shaderFolderPath = L"..\\..\\..\\Debug\\";
#endif // DEBUG

	}
#pragma endregion

	//创建输入布局===============================================================================
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{"POSITION", 0 ,DXGI_FORMAT_R32G32_FLOAT, 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
		{"TEXCOORD", 0 ,DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA ,0 }
	};

//顶点着色器===============================================================================
	//初始化顶点着色器
	if (!this->m_vertexShader.Initialize(this->m_dxDevice.Get(), shaderFolderPath + L"VertexShader.cso", ieds, ARRAYSIZE(ieds)))
	{
		return false;
	}
	//设置顶点着色器
	this->m_dxDeviceContext->VSSetShader(this->m_vertexShader.GetShader(), 0, 0);

	//设置输入布局
	this->m_dxDeviceContext->IASetInputLayout(this->m_vertexShader.GetInputLayout());

	ShaderProcessor shaderProcessor;

	shaderProcessor.ShaderPreProcess(this->m_vertexShader.GetBlob());

//像素着色器===============================================================================
	//初始化像素着色器
	if (!this->m_pixelShader.Initialize(this->m_dxDevice.Get(), shaderFolderPath + L"PixelShader.cso"))
	{
		return false;
	}
	//设置像素着色器
	this->m_dxDeviceContext->PSSetShader(this->m_pixelShader.GetShader(), 0, 0);


//设置图元类型===============================================================================
	this->m_dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

//深度模板===============================================================================
	//深度模板测试状态
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS;
	dsc.StencilEnable = false;
	//……………………其余模板缓冲描述待填写……………………

	//创建深度模板缓冲状态
	this->m_dxDevice->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());

	//设置深度模板缓冲状态
	this->m_dxDeviceContext->OMSetDepthStencilState(m_dxDepthStencilState.Get(), 0);

//混合状态===============================================================================
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	
	rtbd.BlendEnable = true;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD; //颜色混合运算方法
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA; //源颜色因子
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //缓冲区颜色因子
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE; //源透明度因子
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO; //缓冲区透明度因子
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD; //透明度混合运算方法
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //颜色遮罩

	bd.RenderTarget[0] = rtbd;

	//创建混合状态
	this->m_dxDevice->CreateBlendState(&bd, this->m_dxBlendState.GetAddressOf());

	//设置混合状态
	this->m_dxDeviceContext->OMSetBlendState(this->m_dxBlendState.Get(), nullptr, 0xffffffff);

//纹理采样设置===============================================================================
	HRESULT hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//创建纹理
	hr = DirectX::CreateWICTextureFromFile(this->m_dxDevice.Get(), L"Resources\\Textures\\transparency.png", nullptr, this->m_mainTexResourceView.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to Create WIC Texture from File.");

	//设置着色器纹理资源
	this->m_dxDeviceContext->PSSetShaderResources(0, 1, this->m_mainTexResourceView.GetAddressOf());

	//创建采样器状态
	hr = this->m_dxDevice->CreateSamplerState(&sampDesc, this->m_dxSamplerState.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to Create DirectX Sampler State.");



	//设置采样器状态
	this->m_dxDeviceContext->PSSetSamplers(0, 1, this->m_dxSamplerState.GetAddressOf());


	return true;
}

bool Graphics::InitializeScene()
{
//顶点缓冲===================================================================================
// 
	//顶点列表
	Vertex_PosTex vertices[] =
	{
		{DirectX::XMFLOAT2(-0.5f , -0.5f) ,DirectX::XMFLOAT2( 0.0f , 0.0f )},
		{DirectX::XMFLOAT2(0.0f , 0.5f) ,DirectX::XMFLOAT2(0.5f , 1.0f)},
		{DirectX::XMFLOAT2(0.5f , -0.5f) ,DirectX::XMFLOAT2(1.0f , 0.0f)},
	};

	//顶点缓冲初始化
	if (!this->m_vertexBuffer.Initialize(this->m_dxDevice.Get(), vertices, ARRAYSIZE(vertices)))
	{
		return false;
	}

	UINT offsets = 0;
	//设置顶点缓冲
	this->m_dxDeviceContext->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), this->m_vertexBuffer.StridePtr(), &offsets);

//索引缓冲===================================================================================

	//索引列表
	DWORD indices[] =
	{
		0,1,2
	};

	//初始化索引缓冲
	if (!this->m_indexBuffer.Initialize(this->m_dxDevice.Get(), indices, ARRAYSIZE(indices)))
	{
		return false;
	}
	//设置索引缓冲
	this->m_dxDeviceContext->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

//常量缓冲===================================================================================

	//常量缓冲初始化
	if (!this->m_constantBuffer.Initialize(this->m_dxDevice.Get())) 
	{
		return false;
	}

	//设置常量缓冲
	this->m_dxDeviceContext->VSSetConstantBuffers(0, 1, this->m_constantBuffer.GetAddressOf());

	//修改常量缓冲数据
	this->m_constantBuffer.bufferData.xoffset = 0.0f;
	this->m_constantBuffer.bufferData.yoffset = 0.0f;

	//更新常量缓冲
	if (!this->m_constantBuffer.UpdateConstantBuffer(this->m_dxDeviceContext.Get()))
	{
		return false;
	}
	

	return true;
}

bool Graphics::InitializeUI(HWND hwnd)
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->m_dxDevice.Get(), this->m_dxDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}


void Graphics::RenderFrame()
{
	//背景颜色
	float bg_color[] = { 0,0,1,1 };
	//刷新渲染目标
	m_dxDeviceContext->ClearRenderTargetView(m_dxRenderTargetView.Get(), bg_color);
	//刷新深度模板缓冲
	m_dxDeviceContext->ClearDepthStencilView(m_dxDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//绘制
	this->m_dxDeviceContext->DrawIndexed(this->m_indexBuffer.IndexCount(), 0, 0);

	//绘制ImGui========================================================
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_dxSwapChain->Present(0u, 0u);

}
