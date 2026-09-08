#pragma once
#include "Editor/Core/FEditor.h"
#include "Runtime/Core/Log.h"

class FImguiConsoleWindow final {
public:
	bool bIsOpened = true;
	void Process(FEditor& Editor);
};