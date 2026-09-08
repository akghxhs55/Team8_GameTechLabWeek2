#pragma once
#include "FImguiConsoleWindow.h"
#include "FImguiControlPanelWindow.h"
#include "FImguiEditorViewportWindow.h"
#include "FImguiPropertyWindow.h"

class FImguiToolbar final {
public:
	void Process(FImguiConsoleWindow& ConsoleWindow,
		FImguiControlPanelWindow& ControlPanelWindow,
		FImguiPropertyWindow& PropertyWindow);
};