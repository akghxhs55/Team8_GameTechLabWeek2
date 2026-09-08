#include "FImguiPropertyWindow.h"
#include "Runtime/CoreUObject/USceneComponent.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

void FImguiPropertyWindow::Process(FEditor& Editor)
{
	// TODO: RTTI 구현시 dynamic_cast 대신 자체 방법으로 실행되어야 함
	auto* SceneComponent = dynamic_cast<USceneComponent*>(Editor.GetSelectedObject());
	ImGui::Begin("Jungle Property Window");


	if (SceneComponent)
	{
		ImGui::DragFloat3("Translation", &Editor.SelectedTransform.Location.X, 0.01f);
		if (ImGui::DragFloat3("Rotation (deg)", &Editor.SelectedEulerDegDisplay.X, 0.5f))
		{
			constexpr float RadToDeg = 180.0f / std::numbers::pi_v<float>;
			Editor.SelectedTransform.Rotation = FQuaternion::FromEulerXYZDeg(Editor.SelectedEulerDegDisplay);
		}
		ImGui::DragFloat3("Scale", &Editor.SelectedTransform.Scale3D.X, 0.01f);
	}
	else
	{
		ImGui::TextDisabled("No selection");
	}

	static const char* GizmoModes[4] = { "None", "Translation", "Rotation", "Scale" };
	int SelectedItem = static_cast<int>(Editor.GetGizmo().Mode);

	if (ImGui::Combo("Gizmo Mode", &SelectedItem, GizmoModes, 4))
	{
		Editor.GetGizmo().Mode = static_cast<EGizmoMode>(SelectedItem);
	}

	ImGui::End();
}
