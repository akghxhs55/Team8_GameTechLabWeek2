#include "FImguiConsoleWindow.h"
#include "Runtime/Core/Log.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

namespace {
	void ButtonHelper(bool& bShow, int type) {
		ImVec4 BaseColor;
		if (bShow)
			BaseColor = ImGui::GetStyleColorVec4(ImGuiCol_Header);
		else
			BaseColor = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);

		ImVec4 HoverColor{ BaseColor.x * 1.3f, BaseColor.y * 1.3f, BaseColor.z * 1.3f, BaseColor.w };
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, HoverColor);

		const char* name = nullptr;
		switch (type) {
		case 0: name = "Log"; break;
		case 1: name = "Warning"; break;
		case 2: name = "Error"; break;
		default: break;
		}
		auto SelectableWidth = [](const char* Text)
			{
				return ImGui::CalcTextSize(Text).x;
			};
		if (ImGui::Selectable(name, bShow, 0, ImVec2(SelectableWidth(name), 0.0f))) {
			bShow = !bShow;
		}
		ImGui::PopStyleColor();
	}
}

void FImguiConsoleWindow::Process(FEditor& Editor)
{
	if (!bIsOpened)
		return;

	if (!ImGui::Begin("Console Window", &bIsOpened, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Close Console")) {
			bIsOpened = false;
		}
		ImGui::EndPopup();
	}
	bool copy_to_clipboard = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Actions"))
		{
			copy_to_clipboard = ImGui::MenuItem("Copy");
			if (ImGui::MenuItem("Clear")) { FLogManager::Get().Clear(); }
			ImGui::EndMenu();
		}

		ButtonHelper(bShowLog, 0);
		ButtonHelper(bShowWarn, 1);
		ButtonHelper(bShowError, 2);

		//Filter.Draw();
		if (ImGui::InputTextWithHint("##Filter","Filter (inc,-exc)",Filter.InputBuf,IM_ARRAYSIZE(Filter.InputBuf)))
			Filter.Build();

		ImGui::EndMenuBar();
	}

	if (ImGui::SmallButton("Add Debug Text")) { UE_LOG("%d some text", 7); UE_LOG_WARN("some more text"); UE_LOG_ERROR("display very important message here!"); }

	ImGuiStyle& style = ImGui::GetStyle();
	const float footer_height_to_reserve = style.SeparatorSize + style.ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) FLogManager::Get().Clear();;
			ImGui::EndPopup();
		}
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (FString item : FLogManager::Get().GetLogs())
		{
			const char* str = item.c_str();
			if (!Filter.PassFilter(str))
				continue;

			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool has_color = false;
			if (strstr(str, "[ERROR]")) {
				if (!bShowError) continue;
				color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true;
			}
			else if (strstr(str, "[Warning]")) {
				if (!bShowWarn) continue;
				color = ImVec4(0.6f, 0.8f, 0.4f, 1.0f); has_color = true;
			}
			else if (!bShowLog) continue;
			else if (strncmp(str, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(str);
			if (has_color)
				ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();

		// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
		// Using a scrollbar or mouse-wheel will take away from the bottom edge.
		if ((AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);

		ImGui::PopStyleVar();

		
	}
	ImGui::EndChild();

	ImGui::End();
}
