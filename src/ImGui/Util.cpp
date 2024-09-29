#include "ImGui/Util.h"

#include "ImGui/Renderer.h"
#include "ImGui/Styles.h"

namespace ImGui
{
	int IndexOfKey(const std::vector<std::pair<int, double>>& pair_list, const int key)
	{
		for (size_t i = 0; i < pair_list.size(); ++i) {
			auto& p = pair_list[i];
			if (p.first == key) {
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	// Copied from imgui_widgets.cpp
	float CalcMaxPopupHeightFromItemCount(const int items_count)
	{
		ImGuiContext& g = *GImGui;
		if (items_count <= 0)
			return FLT_MAX;
		return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	// https://github.com/ocornut/imgui/discussions/3862
	void AlignForWidth(float width, float alignment)
	{
		float avail = GetContentRegionAvail().x;
		float off = (avail - width) * alignment;

		if (off > 0.0f) {
			SetCursorPosX(GetCursorPosX() + off);
		}
	}

	void ExtendWindowPastBorder()
	{
		const ImGuiWindow* window = GetCurrentWindowRead();
		const ImGuiWindow* rootWindow = FindWindowByName("##Main");

		const auto borderSize = window->WindowBorderSize;
		const auto newWindowPos = ImVec2{ window->Pos.x - borderSize, window->Pos.y - borderSize };

		rootWindow->DrawList->AddRect(newWindowPos, newWindowPos + ImVec2(window->Size.x + 2 * borderSize, window->Size.y + 2 * borderSize), GetColorU32(ImGuiCol_WindowBg), 0.0f, 0, borderSize);
	}

	void LeftAlignedText(const char* label)
	{
		const auto width = CalcTextSize(label);
		const auto windowSize = ImGui::GetWindowSize();
	
		AlignTextToFramePadding();
		
		const auto hovered = GetFocusID() == GetCurrentWindow()->GetID(label);
		if (hovered) {
			PushStyleColor(ImGuiCol_Text, GetColorU32(ImGuiCol_TextDisabled));
		}
		TextUnformatted(label);
		if (hovered) {
			PopStyleColor();
		}

		SameLine();
		SetCursorPosX(windowSize.x * 0.5f);
		SetNextItemWidth(-FLT_MIN);
	}

	bool ActivateOnHover()
	{
		if (!IsItemActive()) {
			if (IsItemFocused()) {
				ActivateItemByID(GetItemID());
				return true;
			}
		} else {
			UnfocusOnEscape();
		}

		return false;
	}

	void UnfocusOnEscape()
	{
		ImGuiContext& g = *GImGui;
		if (IsKeyReleased(ImGuiKey_Escape) || IsKeyReleased(ImGuiKey_NavGamepadCancel)) {
			g.NavId = 0;
			g.NavDisableHighlight = true;
			SetWindowFocus(nullptr);
		}
	}

	void SelectableColor(ImU32 color)
	{
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();

		// Extend the bounding box across the entire width of the window
		ImVec2 window_pos = ImGui::GetWindowPos();       // Get the top-left corner of the window
		float  window_width = ImGui::GetWindowSize().x;  // Get the width of the window

		p_min.x = window_pos.x;                 // Set p_min.x to the left edge of the window
		p_max.x = window_pos.x + window_width;  // Set p_max.x to the right edge of the window

		// Extend the bounding box by 3 pixels
		p_min.x -= 3;
		p_min.y -= 3;
		p_max.x += 3;
		p_max.y += 3;

		ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, color);
	}

	void SelectableColorRect(ImU32 color)
	{
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();

		// Extend the bounding box by 3 pixels
		p_min.x -= 3;
		p_min.y -= 3;
		p_max.x += 3;
		p_max.y += 3;

		ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, color);
	}

	void DisplayWidgetsGroups(std::function<bool()> a_widgets)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->ChannelsSetCurrent(1);
		ImGui::BeginGroup();
		bool hovered = a_widgets();
		ImGui::EndGroup();
		if (hovered) {
			drawList->ChannelsSetCurrent(0);
			SelectableColor(ImGui::GetUserStyleColorU32(ImGui::USER_STYLE::kSelectedBG));
		}
	}

	ImVec2 GetNativeViewportPos()
	{
		return GetMainViewport()->Pos;  // always 0, 0
	}

	ImVec2 GetNativeViewportSize()
	{
		return GetMainViewport()->Size;
	}

	ImVec2 GetNativeViewportCenter()
	{
		const auto Size = GetNativeViewportSize();
		return { Size.x * 0.5f, Size.y * 0.5f };
	}
}
