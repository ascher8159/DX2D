#include "stdafx.h"
#include "Device.h"

//1
int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    InitWindow(hInstance, nShowCmd); //2
    InitDirct3D(hInstance);          //3
    Running();                       //4
    Destroy();                       //5

    //������ ��ȯ
    DestroyWindow(Hwnd);
    UnregisterClass(Title.c_str(), hInstance);

	return 0;
}

HWND Hwnd = nullptr;
wstring Title = L"DX_2D";

IDXGISwapChain* SwapChain = nullptr;
ID3D11Device* Device = nullptr;
ID3D11DeviceContext* DeviceContext = nullptr;
ID3D11RenderTargetView* RTV = nullptr;

Keyboard* Key = nullptr;

void InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ������ Ŭ���� ����
    {
        WNDCLASSEX wcx;
        wcx.cbSize = sizeof(WNDCLASSEX);
        wcx.style = CS_HREDRAW | CS_VREDRAW;
        wcx.lpfnWndProc = WndProc;
        wcx.cbClsExtra = NULL;
        wcx.cbWndExtra = NULL;
        wcx.hInstance = hInstance;
        wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcx.hbrBackground = (HBRUSH)WHITE_BRUSH;
        wcx.lpszMenuName = NULL;
        wcx.lpszClassName = Title.c_str();
        wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        WORD check = RegisterClassEx(&wcx);
        assert(check != NULL);
    }

	//������ Ŭ���� ���� & ������ ���
	{
		Hwnd = CreateWindowEx
		(
			NULL,
			Title.c_str(),
			Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			Width,
			Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);
		assert(Hwnd != NULL);

		//���� ����Ͱ� �����ͼ� ��ġ ����
		RECT rect = { 0, 0, (LONG)Width, (LONG)Height };
		UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)Width) / 2;
		UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)Height) / 2;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow
		(
			Hwnd, centerX, centerY,
			rect.right - rect.left, rect.bottom - rect.top,
			TRUE
		);

		ShowWindow(Hwnd, nCmdShow);
		UpdateWindow(Hwnd);
	}
}

void InitDirct3D(HINSTANCE hInstance)
{
	//Buffer ����
	DXGI_MODE_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));
	desc.Width = Width;
	desc.Height = Height;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//SwapChain ����
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferDesc = desc;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 1;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.OutputWindow = Hwnd;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//����� ����
	vector<D3D_FEATURE_LEVEL> feature_level =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//Create Device & SwapChain
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,            // ����� ȯ��
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		feature_level.data(),
		feature_level.size(),
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		NULL,
		&DeviceContext
	);
	assert(SUCCEEDED(hr));

	//Get BackBuffer
	ID3D11Texture2D* BackBuffer;
	//GPU�� ��� �ϴ� ���
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));

	// RTV �����ϰ� ȭ�鿡 ��Ÿ���� �����(Resource)�ڿ� ����
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
	assert(SUCCEEDED(hr));
	BackBuffer->Release(); //��ȯ(����)

	//OM Set
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);

	//Create Viewport
	{
		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = (float)Width;	  //ȭ��ũ�� ��ü ���
		viewPort.Height = (float)Height;  //ȭ��ũ�� ��ü ���

		DeviceContext->RSSetViewports(1, &viewPort);
	}
}

WPARAM Running()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	// �̱���
	ImGui::Create(Hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark(); //�׸� (���ص���)
	
	//System
	Key = new Keyboard();

	InitScene();
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //����
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ImGui::Update();
			Update();
			Render();
		}
	}
	DestroyScene();

	SafeDelete(Key);
	ImGui::Delete();
    return msg.wParam;
}

void Destroy()
{
	SafeRelease(RTV);
	SafeRelease(SwapChain);
	SafeRelease(DeviceContext);
	SafeRelease(Device);
}

/*------------------Msg Event (Win32API)*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc(hwnd, msg, wParam, lParam))
		return true;

	switch (msg){ 
		case WM_DESTROY: PostQuitMessage(0); break; 
	
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
