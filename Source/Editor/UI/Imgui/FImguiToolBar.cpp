#include "FImguiToolBar.h"
#include "ThirdParty/Imgui/imgui.h"

void FImguiToolbar::Process(FImguiConsoleWindow& ConsoleWindow, FImguiControlPanelWindow& ControlPanelWindow, FImguiPropertyWindow& PropertyWindow)
{
	if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Console")) { ConsoleWindow.bIsOpened = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
	}
}
