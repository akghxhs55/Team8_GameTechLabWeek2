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

	// 오일러각(도)을 편집 상태로 보관한다.
	// 매 프레임 쿼터니언 -> 오일러 역변환을 하면 Y(pitch)가 GetEulerXYZ 특성상
	// -90~90 으로 접혀서 그 범위를 못 벗어난다. 선택이 바뀔 때만 역변환한다.
	static USceneComponent* CachedComponent = nullptr;
	static float Location[3] = { 0.0f, 0.0f, 0.0f };
	static float RotationDeg[3] = { 0.0f, 0.0f, 0.0f };
	static float Scale[3] = { 1.0f, 1.0f, 1.0f };

	if (SceneComponent)
	{
		constexpr float RadToDeg = 180.0f / 3.14159265358979f;

		FTransform RelativeTransform = SceneComponent->GetRelativeTransform();

		if (CachedComponent != SceneComponent)
		{
			CachedComponent = SceneComponent;

			Location[0] = RelativeTransform.Location[0];
			Location[1] = RelativeTransform.Location[1];
			Location[2] = RelativeTransform.Location[2];

			const FVector EulerRad = RelativeTransform.Rotation.GetEulerXYZ();
			RotationDeg[0] = EulerRad[0] * RadToDeg;
			RotationDeg[1] = EulerRad[1] * RadToDeg;
			RotationDeg[2] = EulerRad[2] * RadToDeg;

			Scale[0] = RelativeTransform.Scale3D[0];
			Scale[1] = RelativeTransform.Scale3D[1];
			Scale[2] = RelativeTransform.Scale3D[2];
		}

		bool bChanged = false;
		bChanged |= ImGui::DragFloat3("Translation", Location, 0.01f);
		bChanged |= ImGui::DragFloat3("Rotation (deg)", RotationDeg, 0.5f);
		bChanged |= ImGui::DragFloat3("Scale", Scale, 0.01f);

		if (bChanged)
		{
			RelativeTransform.Location = { Location[0], Location[1], Location[2] };
			RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(
				FVector{ RotationDeg[0], RotationDeg[1], RotationDeg[2] });
			RelativeTransform.Scale3D = { Scale[0], Scale[1], Scale[2] };
			SceneComponent->SetRelativeTransform(RelativeTransform);
		}
	}
	else
	{
		// 선택 해제 시 캐시를 무효화해서, 같은 오브젝트를 다시 선택해도 최신 값을 읽도록 한다.
		CachedComponent = nullptr;
		ImGui::TextDisabled("No selection");
	}
	ImGui::End();
}
