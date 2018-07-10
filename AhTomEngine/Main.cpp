#include <Windows.h>
#include <windowsx.h>

#include <d3d11.h>
#include<d3d.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

IDXGISwapChain *swap_chain;
ID3D11Device *dev;
ID3D11DeviceContext *devcon;
ID3D11RenderTargetView *backbuffer;
ID3D11Buffer *pVBuffer;
ID3D11VertexShader *pVS;
ID3D11PixelShader*pPS;
ID3D11InputLayout *pLayout;    // global


using RGBA = float[4];

struct VERTEX
{
	FLOAT X, Y, Z;
	RGBA Color;
};

void InitD3D(HWND hWnd);
void CleanD3D();
void RenderFrame();
void InitPipeline();
void InitGraphics();

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrivInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	//handle for the window, get's filled by a function
	HWND hWnd;

	//struct which hold information for the window class
	WNDCLASSEX wc;

	//clear out the window class for usage
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wc.hbrBackground = HBRUSH(COLOR_WINDOW);
	wc.lpszClassName = "WindowClass1";

	//register the window class
	RegisterClassEx(&wc);

	//size of the window to be created
	RECT wr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//create the window and use the result as handle;
	hWnd = CreateWindowEx(NULL,					
						"WindowClass1",			//name of the window class
						"My awsome Game",		//title of the window
						WS_OVERLAPPEDWINDOW,	//window style
						100,					//x-pos
						100,					//y-pos
						wr.right - wr.left,		//width
						wr.bottom - wr.top,		//height
						nullptr,				//we have no parent window
						nullptr,				//we aren't using menus
						hInstance,				//application handle
						nullptr);				//used with multiple windows

	//set up and initialize Direct3D
	InitD3D(hWnd);

	//display the window
	ShowWindow(hWnd, nCmdShow);

	//enter the main loop:


	//struct which holds windows event messages
	MSG msg;

	
	while(TRUE)
	{
		//check if any messages are watiting in the queue
		if(PeekMessage(&msg, nullptr, 0,0,PM_REMOVE))
		{
			//translate keystroke messages into right format
			TranslateMessage(&msg);

			//send the message to the WindowProc funtion
			DispatchMessage(&msg);

			//check to see if itÄs time qo quit
			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
		
	}

	//clean up DirectX and COM
	CleanD3D();

	//return this part of the WM_QUIT message to windows
	return msg.wParam;
}

LRESULT __stdcall WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			//close the application entirely
			PostQuitMessage(0);

			return 0;
		}
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}	
	};
}

void InitD3D(HWND hWnd)
{
	//create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;									//one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;					//set back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;					//set back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//how swap chain is to be used
	scd.OutputWindow = hWnd;								//the window to be used
	scd.SampleDesc.Count = 4;								//how many multisamples
	scd.Windowed = TRUE;									//windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		//allow full-screen switching

	//create a device, device context and swap chain using the infomration in the scd struct
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &scd, &swap_chain, &dev, nullptr, &devcon);

	//get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	pBackBuffer->Release();

	//set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, nullptr);

	//set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

void CleanD3D()
{
	swap_chain->SetFullscreenState(FALSE, nullptr); //switch to windowed mode

	//close and realease all existing COM objects
	pLayout->Release();
	pPS->Release();
	pVS->Release();
	pVBuffer->Release();
	swap_chain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

void RenderFrame()
{
	//clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, RGBA{ 0.f, 0.2f, 0.4f, 1.0f });

	//do 3D rendering on the back buffer here

		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

		// select which primtive type we are using
		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw the vertex buffer to the back buffer
		devcon->Draw(3, 0);

	//switch the back buffer and the front buffer
	swap_chain->Present(0, 0);
}

void InitPipeline()
{
	ID3DBlob *VS, *PS;

	ID3DBlob *VSError, *PSError;

	D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr,"VShader","vs_4_0",0,0,&VS, &VSError);
	D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr,"PShader","ps_4_0",0,0,&PS, &PSError);



	//encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

	//set the shader objects
	devcon->VSSetShader(pVS, nullptr, 0);
	devcon->PSSetShader(pPS, nullptr, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//not sure about D3D11_APPEND_ALIGNED_ELEMENT maybe 12 better now
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}

void InitGraphics()
{
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.0f, 0.0f, *RGBA{1.0f, 0.0f, 0.0f, 1.0f} },
		{ 0.45f, -0.5f, 0.0f, *RGBA{0.0f, 1.0f, 0.0f, 1.0f} },
		{ -0.45f, -0.5f, 0.0f, *RGBA{0.0f, 0.0f, 1.0f, 1.0f} }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, nullptr, &pVBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
	devcon->Unmap(pVBuffer, NULL);										//unmap the buffer
}

