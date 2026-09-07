#include "FImguiManager.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"


bool FImguiManager::Initialize_ImplWin32DX11(HWND& Window, ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	bool success = true;
	success &= ImGui_ImplWin32_Init((void*)Window);
	success &= ImGui_ImplDX11_Init(Device, Context);
	if (success) ImplType = EImplType::Win32DX11;
	return success;
}

void FImguiManager::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void FImguiManager::RenderUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
