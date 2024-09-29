#pragma once

namespace ImGui
{
	using HoveredResult = std::pair<bool, bool>;
	using HoveredResultWithSize = std::tuple<bool, bool, float>;

	int   IndexOfKey(const std::vector<std::pair<int, double>>& pair_list, int key);
	float CalcMaxPopupHeightFromItemCount(int items_count);

	void AlignForWidth(float width, float alignment = 0.5f);

	void ExtendWindowPastBorder();

	void LeftAlignedText(const char* label);

	bool ActivateOnHover();
	void UnfocusOnEscape();

	void SelectableColor(ImU32 color);
	void SelectableColorRect(ImU32 color);
	void DisplayWidgetsGroups(std::function<bool()> a_widgets);

	ImVec2 GetNativeViewportSize();
	ImVec2 GetNativeViewportPos();
	ImVec2 GetNativeViewportCenter();
}
