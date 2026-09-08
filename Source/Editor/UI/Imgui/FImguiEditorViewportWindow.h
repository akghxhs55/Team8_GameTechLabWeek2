#pragma once
#include "Editor/Core/FEditor.h"
#include "Runtime/Math/FVector2.h"

// 3D 씬 위를 덮는 투명한 ImGui 창.
// - 다른 패널(ControlPanel, Property 등)이 이 창 위에 그려지므로,
//   ImGui 의 hover/active 판정이 "다른 패널에 가려지지 않은 뷰포트 영역"만 걸러준다.
// - 활성 뷰포트의 화면 사각형(TopLeft/Length)과 focus/hover 상태를 갱신한다.
// - 뷰포트 위에서 클릭이 발생하면 피킹을 수행한다.
class FImguiEditorViewportWindow final
{

public:
	void Process(FEditor& Editor);

private:
	void HandlePicking(FEditor& Editor, FEditorViewport& Viewport, FVector2 ViewportSize);
};
