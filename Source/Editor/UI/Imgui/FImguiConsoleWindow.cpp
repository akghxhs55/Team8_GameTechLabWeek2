#include "FImguiConsoleWindow.h"
#include "Runtime/Core/Log.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

void FImguiConsoleWindow::Process(FEditor& Editor)
{
	if (!bIsOpened)
		return;

	if (!ImGui::Begin("Console Window", &bIsOpened)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Close Console"))
			bIsOpened = false;
		ImGui::EndPopup();
	}



	ImGui::End();
}
