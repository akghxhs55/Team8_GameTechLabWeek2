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

	if (SceneComponent) {
		FTransform RelativeTransform = SceneComponent->GetRelativeTransform();
		float Location[3] = { RelativeTransform.Location[0], RelativeTransform.Location[1], RelativeTransform.Location[2] };
		FVector EulerAngle = RelativeTransform.Rotation.GetEulerXYZ();
		float Rotation[3] = { EulerAngle[0], EulerAngle[1], EulerAngle[2] };
		float Scale[3] = { RelativeTransform.Scale3D[0], RelativeTransform.Scale3D[1], RelativeTransform.Scale3D[2] };
		ImGui::DragFloat3("Translation", Location, 0.01f);
		ImGui::DragFloat3("Rotation", Rotation, 0.01f);
		ImGui::DragFloat3("Scale", Scale, 0.01f);
		RelativeTransform.Location = { Location[0], Location[1], Location[2] };
		EulerAngle[0] = Rotation[0]; EulerAngle[1] = Rotation[1]; EulerAngle[2] = Rotation[2];
		EulerAngle[0] = EulerAngle[0] * 180 / 3.141592f;
		EulerAngle[1] = EulerAngle[1] * 180 / 3.141592f;
		EulerAngle[2] = EulerAngle[2] * 180 / 3.141592f;
		RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(EulerAngle);
		RelativeTransform.Scale3D = { Scale[0], Scale[1], Scale[2] };
		SceneComponent->SetRelativeTransform(RelativeTransform);
		//ImGui::DragFloat3("Rotation", Rotation);
		//ImGui::DragFloat3("Scale", Scale);
	}
	ImGui::End();
}
