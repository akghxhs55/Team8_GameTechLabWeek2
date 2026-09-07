#pragma once
#include "Editor/Core/FEditor.h"
#include "Editor/UI/Imgui/FImguiManager.h"
#include "Editor/UI/Imgui/FImguiPropertyWindow.h"


class FEditorApplication final {
	FEditor Editor;

	FImguiManager ImguiManager;
	FImguiPropertyWindow PropertyWindow;
public:
	static FEditorApplication& Get()
	{
		static FEditorApplication Instance;
		return Instance;
	}

	//복사 연산자와 복사 대입 연산자 삭제
	FEditorApplication(const FEditorApplication&) = delete;
	FEditorApplication& operator=(const FEditorApplication&) = delete;

	//이동 연산자와 이동 대입연산자 삭제
	FEditorApplication(FEditorApplication&&) = delete;
	FEditorApplication& operator=(FEditorApplication&&) = delete;


private:
	FEditorApplication() = default;
	~FEditorApplication() = default;
};