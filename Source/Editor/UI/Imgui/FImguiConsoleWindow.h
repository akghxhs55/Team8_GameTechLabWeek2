#pragma once
#include "Editor/Core/FEditor.h"
#include "Runtime/Core/Log.h"
#include "ThirdParty/Imgui/imgui.h"

class FImguiConsoleWindow final {
public:
	bool bIsOpened = true;
	void Process(FEditor& Editor);
	bool bShowLog = true;
	bool bShowWarn = true;
	bool bShowError = true;
	bool AutoScroll = true;
	ImGuiTextFilter Filter;
};