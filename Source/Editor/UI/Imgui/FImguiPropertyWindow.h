#pragma once
#include "Editor/Core/FEditor.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

class FImguiPropertyWindow final {
public:
	void Process(FEditor& Editor);
};