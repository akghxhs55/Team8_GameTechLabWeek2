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
		float Location[3] = { Editor.SelectedLocation.X, Editor.SelectedLocation.Y, Editor.SelectedLocation.Z };
		float RotationDeg[3] = { Editor.SelectedRotationDeg.X, Editor.SelectedRotationDeg.Y, Editor.SelectedRotationDeg.Z };
		float Scale[3] = { Editor.SelectedScale3D.X, Editor.SelectedScale3D.Y, Editor.SelectedScale3D.Z };


		bool bChanged = false;
		bChanged |= ImGui::DragFloat3("Translation", Location, 0.01f);
		bChanged |= ImGui::DragFloat3("Rotation (deg)", RotationDeg, 0.5f);
		bChanged |= ImGui::DragFloat3("Scale", Scale, 0.01f);

		if (bChanged)
		{
			Editor.SelectedLocation = FVector{ Location[0], Location[1], Location[2] };
			Editor.SelectedRotationDeg = FVector{ RotationDeg[0], RotationDeg[1], RotationDeg[2] };
			Editor.SelectedScale3D = FVector{ Scale[0], Scale[1], Scale[2] };
		}
	}
	else
	{
		ImGui::TextDisabled("No selection");
	}

	static const char* GizmoModes[4] = { "None", "Location", "Rotation", "Scale" };
	int SelectedItem = static_cast<int>(Editor.GetGizmo().Mode);

	if (ImGui::Combo("Gizmo Mode", &SelectedItem, GizmoModes, 4))
	{
		Editor.GetGizmo().Mode = static_cast<EGizmoMode>(SelectedItem);
	}

	ImGui::End();
}
