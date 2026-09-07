#include "FImguiControlPanelWindow.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

void FImguiControlPanelWindow::Process(FEditor& Editor)
{
    ImGui::Begin("Jungle Control Panel");

    ImGui::Text("Hello Jungle World!");
    ImGui::Text("FPS %.0f (%.0f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Separator();

    static int primitive = 0;
    const char* primitives[] = { "Sphere", "Cube", "Plane", "Capsule", "Cylinder" };
    ImGui::SetNextItemWidth(540.0f);
    ImGui::Combo("##Primitive", &primitive, primitives, IM_ARRAYSIZE(primitives));
    ImGui::SameLine();
    ImGui::Text("Primitive");

    static int spawnCount = 1;
    ImGui::Button("Spawn");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(550.0f);
    ImGui::InputInt("##SpawnCount", &spawnCount);
    ImGui::SameLine();
    ImGui::Text("Number of spawn");

    static char sceneName[128] = "Default";
    ImGui::SetNextItemWidth(540.0f);
    ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName));
    ImGui::SameLine();
    ImGui::Text("Scene Name");

    ImGui::Button("New scene");
    ImGui::Button("Save scene");
    ImGui::Button("Load scene");

    ImGui::Separator();

    static bool orthogonal = false;
    ImGui::Checkbox("Orthogonal", &orthogonal);

    static float fov = 60.0f;
    ImGui::SetNextItemWidth(540.0f);
    ImGui::InputFloat("##FOV", &fov, 0.0f, 0.0f, "%.3f");
    ImGui::SameLine();
    ImGui::Text("FOV");

    static float cameraLocation[3] = { 0.479f, 2.618f, -4.136f };
    ImGui::SetNextItemWidth(540.0f);
    ImGui::InputFloat3("##CameraLocation", cameraLocation, "%.3f");
    ImGui::SameLine();
    ImGui::Text("Camera Location");

    static float cameraRotation[3] = { 0.350f, -0.150f, 0.000f };
    ImGui::SetNextItemWidth(540.0f);
    ImGui::InputFloat3("##CameraRotation", cameraRotation, "%.3f");
    ImGui::SameLine();
    ImGui::Text("Camera Rotation");

    ImGui::End();
}
