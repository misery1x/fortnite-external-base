#include "overlay.hpp"

// Direct x
#include <D3D11.h>
#include <D3DX11core.h>
#include <D3DX11.h>
#include <D3DX11tex.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <thread>
#include <chrono>

// ImGui
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_dx11.h"
#include "../vendor/imgui/imgui_impl_win32.h"

// Custom
#include "../game/utility/settings.hpp"
#include "../game/features/esp/actors.hpp"
#include "../utility/console/console.hpp"
#include "../game/utility/settings.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// D3D11CreateDeviceAndSwapChain
#pragma comment(lib, "d3d11.lib")

ImFont* font;

ID3D11Device* D3D_Device;
ID3D11DeviceContext* D3D_Device_ctx;
IDXGISwapChain* D3D_Swap_Chain;
ID3D11RenderTargetView* D3D_Render_Target;

void overlay_class::initialize( )
{
	if (!initialize_imgui())
	{
		console.writeline("fn-base encountered an error", true);
		console.beep(325, 6969);
		console.sleep(2000);
		console.exit_process(69);
	}

	CreateThread(0, 0, LPTHREAD_START_ROUTINE(render_loop()), 0, 0, 0);
}

bool overlay_class::initialize_imgui( )
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"Fortnite";
	RegisterClassEx(&wc);

	Globals->WindowHandle = CreateWindowEx( WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"Fortnite", L"Fortnite", WS_POPUP, 0, 0, Globals->ScreenWidth, Globals->ScreenHeight, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (!Globals->WindowHandle)
		return false;

	SetLayeredWindowAttributes(Globals->WindowHandle, RGB(0, 0, 0), 255, LWA_ALPHA);

	LONG ex_style = GetWindowLong(Globals->WindowHandle, GWL_EXSTYLE);
	ex_style |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
	SetWindowLong(Globals->WindowHandle, GWL_EXSTYLE, ex_style);

	ex_style |= WS_EX_TRANSPARENT;
	SetWindowLong(Globals->WindowHandle, GWL_EXSTYLE, ex_style);

	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(Globals->WindowHandle, &margins);

	ShowWindow(Globals->WindowHandle, SW_SHOW);
	UpdateWindow(Globals->WindowHandle);

	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
	swap_chain_description.BufferCount = 2;
	swap_chain_description.BufferDesc.Width = Globals->ScreenWidth;
	swap_chain_description.BufferDesc.Height = Globals->ScreenHeight;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = Globals->WindowHandle;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.Windowed = TRUE;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature_level;
	D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_level_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &D3D_Swap_Chain, &D3D_Device, &feature_level, &D3D_Device_ctx)))
	{
		return false;
	}

	ID3D11Texture2D* back_buffer;
	D3D_Swap_Chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	D3D_Device->CreateRenderTargetView(back_buffer, NULL, &D3D_Render_Target);
	back_buffer->Release();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplWin32_Init(Globals->WindowHandle);
	ImGui_ImplDX11_Init(D3D_Device, D3D_Device_ctx);

	font = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\arialbd.ttf"), 18.f);

	return true;
}

bool overlay_class::render_loop( )
{
	MSG msg = { NULL };
	memset(&msg, 0, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		UpdateWindow(Globals->WindowHandle);
		ShowWindow(Globals->WindowHandle, SW_SHOW);

		if (PeekMessageA(&msg, Globals->WindowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		POINT p_cursor;
		GetCursorPos(&p_cursor);
		io.MousePos.x = p_cursor.x;
		io.MousePos.y = p_cursor.y;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0] = io.MousePos;
		}
		else
		{
			io.MouseDown[0] = false;
		}

		draw_loop();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(Globals->WindowHandle);

	return true;
}

std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
float fps = 0.0f;

void overlay_class::draw_loop( )
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;
	fps = 1.0f / deltaTime.count();

	if (Misc->Watermark)
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		ImGui::Text("fn-base | %.1f FPS", fps);
		ImGui::End();
	}

	actors.draw_esp();
	menu_loop();

	ImGui::Render();
	const float clear_color_with_alpha[4] = { ImVec4(0.0f, 0.0f, 0.0f, 0.0f).x * ImVec4(0.0f, 0.0f, 0.0f, 0.0f).w, ImVec4(0.0f, 0.0f, 0.0f, 0.0f).y * ImVec4(0.0f, 0.0f, 0.0f, 0.0f).w, ImVec4(0.0f, 0.0f, 0.0f, 0.0f).z * ImVec4(0.0f, 0.0f, 0.0f, 0.0f).w, ImVec4(0.0f, 0.0f, 0.0f, 0.0f).w };

	D3D_Device_ctx->OMSetRenderTargets(1, &D3D_Render_Target, nullptr);
	D3D_Device_ctx->ClearRenderTargetView(D3D_Render_Target, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	D3D_Swap_Chain->Present(0, 0);
}

void overlay_class::menu_loop( )
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		Globals->GUIWindow = !Globals->GUIWindow;

	if (Globals->GUIWindow)
	{
		ImGui::SetWindowSize("fn-base", ImVec2(450, 800));
		ImGui::Begin("fn-base", &Globals->GUIWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoResize);

		ImGui::Text("Aimbot");
		ImGui::Checkbox("Enable Aimbot", &Aimbot->Enable);
		ImGui::Checkbox("Draw FOV", &Aimbot->DrawFov);
		ImGui::SliderInt("Field Of View", &Aimbot->FieldOfView, 1, 469);
		ImGui::SliderInt("Smoothness X", &Aimbot->SmoothingX, 1, 15);
		ImGui::SliderInt("Smoothness Y", &Aimbot->SmoothingY, 1, 15);
		ImGui::Separator();
		ImGui::Text("Visuals");
		ImGui::Checkbox("Enable Visuals", &Visuals->Enable);
		ImGui::Checkbox("2D Box", &Visuals->Box);
		ImGui::Checkbox("Skeleton", &Visuals->Skeleton);
		ImGui::Checkbox("Show Bones", &Visuals->DebugBones);
		ImGui::Separator();
		ImGui::Text("Misc");
		ImGui::Checkbox("Show Watermark", &Misc->Watermark);

		ImGui::End();
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}