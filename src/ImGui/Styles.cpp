#include "ImGui/Styles.h"

#include "ImGui/Fonts/Manager.h"
#include "ImGui/Renderer.h"
#include "Settings.h"

namespace ImGui
{
	ImVec4 Styles::GetColor(const RE::NiColor& a_color)
	{
		return ImVec4(a_color.r, a_color.g, a_color.b, 1.0f);
	}

	void Styles::SetColor(ImVec4& a_to, const ImVec4& a_from)
	{
		a_to.x = a_from.x;
		a_to.y = a_from.y;
		a_to.z = a_from.z;
	}

	ImU32 Styles::GetColorU32(USER_STYLE a_style) const
	{
		switch (a_style) {
		case USER_STYLE::kIcon:
			return iconU32;
		case USER_STYLE::kIconDisabled:
			return iconDisabledU32;
		case USER_STYLE::kGridLines:
			return gridLinesU32;
		case USER_STYLE::kSliderBorder:
			return sliderBorderU32;
		case USER_STYLE::kSliderBorderActive:
			return sliderBorderActiveU32;
		case USER_STYLE::kFrameBG_Widget:
			return frameBG_WidgetU32;
		case USER_STYLE::kFrameBG_WidgetActive:
			return frameBG_WidgetActiveU32;
		case USER_STYLE::kSelectedText:
			return selectedTextU32;
		case USER_STYLE::kSelectedBG:
			return selectedBGU32;
		default:
			return ImU32();
		}
	}

	ImVec4 Styles::GetColorVec4(USER_STYLE a_style) const
	{
		switch (a_style) {
		case USER_STYLE::kIcon:
			return user.icon;
		case USER_STYLE::kIconDisabled:
			return user.iconDisabled;
		case USER_STYLE::kComboBoxTextBox:
			return user.comboBoxTextBox;
		case USER_STYLE::kComboBoxText:
			return user.comboBoxText;
		default:
			return ImVec4();
		}
	}

	float Styles::GetVar(USER_STYLE a_style) const
	{
		switch (a_style) {
		case USER_STYLE::kGridLines:
			return user.gridThickness;
		default:
			return 1.0f;
		}
	}

	void Styles::OnStyleRefresh()
	{
		ImVec4 hudColor = GetColor(RE::HUDMenuUtils::GetGameplayHUDColor());
		ImVec4 bgColor = GetColor(RE::HUDMenuUtils::GetGameplayHUDBackgroundColor());

		if (hudColor != cachedHUDColor || bgColor != cachedBGColor) {
			cachedHUDColor = hudColor;
			cachedBGColor = bgColor;
			refreshStyle = true;
		}

		if (!refreshStyle) {
			return;
		}

		ImGuiStyle style;
		auto&      colors = style.Colors;

		ImVec4 hudColorAlpha = hudColor;
		hudColorAlpha.z = user.bgAlpha;

		constexpr ImVec4 hudSelectedColor = ImVec4(0.0, 0.0, 0.0, 1.0);
		ImVec4           hudSelectedColorAlpha = ImVec4(0.0, 0.0, 0.0, user.bgAlpha);

		bgColor.w = user.bgAlpha;

		style.WindowBorderSize = 2.5f;
		style.TabBarBorderSize = 2.5f;
		style.TabRounding = 0.0f;
		style.GrabMinSize *= 0.5f;
		style.IndentSpacing *= 1.5f;
		style.ScrollbarRounding = 0.0f;

		user.icon = hudSelectedColor;
		user.iconDisabled = hudColor;

		user.comboBoxText = hudColor;
		user.comboBoxTextBox = hudSelectedColor;

		user.borderSize = 3.500f;
		user.gridThickness = 2.500f;

		colors[ImGuiCol_WindowBg] = bgColor;
		colors[ImGuiCol_ChildBg] = bgColor;

		colors[ImGuiCol_Border] = hudColor;
		colors[ImGuiCol_Separator] = hudColor;

		colors[ImGuiCol_Text] = hudColor;
		colors[ImGuiCol_TextDisabled] = hudSelectedColor;

		colors[ImGuiCol_FrameBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0, 0.0, 0.0, 0.0);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.0, 0.0, 0.0, 0.0);
		user.frameBG_Widget = ImVec4(0.0, 0.0, 0.0, 0.0);
		user.frameBG_WidgetActive = ImVec4(0.0, 0.0, 0.0, 0.0);

		colors[ImGuiCol_SliderGrab] = hudColor;
		colors[ImGuiCol_SliderGrabActive] = hudSelectedColor;

		colors[ImGuiCol_Button] = ImVec4(0.0, 0.0, 0.0, 0.0);
		colors[ImGuiCol_ButtonHovered] = colors[ImGuiCol_Button];
		colors[ImGuiCol_ButtonActive] = colors[ImGuiCol_Button];

		colors[ImGuiCol_Header] = ImVec4(1.0, 0.0, 0.0, 0.5);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.0, 1.0, 0.0, 0.5);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.0, 0.0, 1.0, 0.5);

		colors[ImGuiCol_Tab] = ImVec4(0.0, 0.0, 0.0, 0.0);
		colors[ImGuiCol_TabHovered] = hudColor;
		colors[ImGuiCol_TabActive] = colors[ImGuiCol_TabHovered];
		colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
		colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabHovered];

		colors[ImGuiCol_ScrollbarGrab] = hudColor;
		colors[ImGuiCol_ScrollbarGrabActive] = colors[ImGuiCol_ScrollbarGrab];
		colors[ImGuiCol_ScrollbarGrabHovered] = colors[ImGuiCol_ScrollbarGrab];

		colors[ImGuiCol_NavHighlight] = ImVec4();

		frameBG_WidgetU32 = ColorConvertFloat4ToU32(user.frameBG_Widget);
		frameBG_WidgetActiveU32 = ColorConvertFloat4ToU32(user.frameBG_WidgetActive);
		gridLinesU32 = ColorConvertFloat4ToU32(hudColor);
		sliderBorderU32 = ColorConvertFloat4ToU32(hudColor);
		sliderBorderActiveU32 = ColorConvertFloat4ToU32(hudSelectedColor);
		iconU32 = ColorConvertFloat4ToU32(user.icon);
		iconDisabledU32 = ColorConvertFloat4ToU32(user.iconDisabled);
		selectedTextU32 = ColorConvertFloat4ToU32(hudSelectedColor);
		selectedBGU32 = ColorConvertFloat4ToU32(hudColor);

		style.ScaleAllSizes(Renderer::GetResolutionScale());
		ImGui::GetStyle() = style;

		// reload fonts
		MANAGER(Font)->LoadSettings();
		MANAGER(Font)->ReloadFonts();

		refreshStyle = false;
	}

	ImU32 GetUserStyleColorU32(USER_STYLE a_style)
	{
		return Styles::GetSingleton()->GetColorU32(a_style);
	}

	ImVec4 GetUserStyleColorVec4(USER_STYLE a_style)
	{
		return Styles::GetSingleton()->GetColorVec4(a_style);
	}

	float GetUserStyleVar(USER_STYLE a_style)
	{
		return Styles::GetSingleton()->GetVar(a_style);
	}
}
