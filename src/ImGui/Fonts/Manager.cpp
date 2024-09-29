#include "ImGui/Fonts/Manager.h"

#include "ImGui/Fonts/BGSFonts.h"
#include "ImGui/Fonts/IconsFontAwesome6.h"

#include "ImGui/Renderer.h"
#include "ImGui/Styles.h"
#include "ImGui/Util.h"
#include "Input.h"
#include "Settings.h"

namespace Font
{
	void Font::LoadSettings(const CSimpleIniA& a_ini, const char* a_section)
	{
		name = a_ini.GetValue(a_section, "sFont", name.c_str());
		name = R"(Data\Interface\ImGuiFonts\)" + name;

		const auto resolutionScale = ImGui::Renderer::GetResolutionScale();
		
		size = static_cast<float>(a_ini.GetLongValue(a_section, "iSize", 32)) * resolutionScale;
		largeSize = static_cast<float>(a_ini.GetLongValue(a_section, "iLargeSize", 36)) * resolutionScale;

		iconSize = static_cast<float>(a_ini.GetLongValue("Icons", "iSize", 32)) * resolutionScale;
		largeIconSize = static_cast<float>(a_ini.GetLongValue("Icons", "iLargeSize", 36)) * resolutionScale;
	}

	void Font::LoadFont(const std::uint32_t* a_data, const std::uint32_t a_dataSize, const ImVector<ImWchar>& a_ranges, bool a_loadIcons)
	{
		const auto& io = ImGui::GetIO();

		ImFontConfig icon_config;
		icon_config.MergeMode = true;
		icon_config.PixelSnapH = true;
		icon_config.OversampleH = icon_config.OversampleV = 1;

		font = io.Fonts->AddFontFromMemoryCompressedTTF(a_data, a_dataSize, size, nullptr, a_ranges.Data);
		if (a_loadIcons) {
			io.Fonts->AddFontFromFileTTF(R"(Data\Interface\ImGuiFonts\)" FONT_ICON_FILE_NAME_FAS, iconSize, &icon_config, a_ranges.Data);
		}

		ImFontConfig icon_config_large;
		icon_config_large.MergeMode = true;
		icon_config_large.PixelSnapH = true;
		icon_config_large.OversampleH = icon_config_large.OversampleV = 1;

		largeFont = io.Fonts->AddFontFromMemoryCompressedTTF(a_data, a_dataSize, largeSize, nullptr, a_ranges.Data);
		if (a_loadIcons) {
			io.Fonts->AddFontFromFileTTF(R"(Data\Interface\ImGuiFonts\)" FONT_ICON_FILE_NAME_FAS, largeIconSize, &icon_config, a_ranges.Data);
		}
	}

	void Font::LoadFont(const ImVector<ImWchar>& a_ranges, bool a_loadIcons)
	{
		const auto& io = ImGui::GetIO();

		ImFontConfig icon_config;
		icon_config.MergeMode = true;
		icon_config.PixelSnapH = true;
		icon_config.OversampleH = icon_config.OversampleV = 1;

		font = io.Fonts->AddFontFromFileTTF(name.c_str(), size, nullptr, a_ranges.Data);
		if (a_loadIcons) {
			io.Fonts->AddFontFromFileTTF(R"(Data\Interface\ImGuiFonts\)" FONT_ICON_FILE_NAME_FAS, iconSize, &icon_config, a_ranges.Data);
		}

		ImFontConfig icon_config_large;
		icon_config_large.MergeMode = true;
		icon_config_large.PixelSnapH = true;
		icon_config_large.OversampleH = icon_config_large.OversampleV = 1;

		largeFont = io.Fonts->AddFontFromFileTTF(name.c_str(), largeSize, nullptr, a_ranges.Data);
		if (a_loadIcons) {
			io.Fonts->AddFontFromFileTTF(R"(Data\Interface\ImGuiFonts\)" FONT_ICON_FILE_NAME_FAS, largeIconSize, &icon_config, a_ranges.Data);
		}
	}

	ImFont* Font::GetFont() const
	{
		return font;
	}

	ImFont* Font::GetLargeFont() const
	{
		return largeFont;
	}

	void Manager::LoadSettings()
	{
		Settings::GetSingleton()->SerializeFonts([this](auto& ini) {
			defaultFont.LoadSettings(ini, "Font");
			controllerButtonFont.LoadSettings(ini, "ControllerButtonFont");
		});
	}

	void Manager::LoadMCMSettings(const CSimpleIniA& a_ini)
	{
		buttonScheme = static_cast<BUTTON_SCHEME>(a_ini.GetLongValue("Controls", "iButtonScheme", std::to_underlying(buttonScheme)));
	}

	void Manager::ReloadFonts()
	{
		if (loadedFonts) {
			return;
		}

		auto& io = ImGui::GetIO();

		ImVector<ImWchar> ranges;

		ImFontGlyphRangesBuilder builder;
		builder.AddText(RE::BSScaleformManager::GetSingleton()->validNameChars.c_str());
		builder.AddChar(0xf083);  // RETRO CAMERA
		builder.AddChar(0xf017);  // CLOCK
		builder.AddChar(0xf183);  // PERSON
		builder.AddChar(0xf042);  // CONTRAST
		builder.AddChar(0xf03e);  // IMAGE
		builder.BuildRanges(&ranges);

		defaultFont.LoadFont(ranges, true);
		controllerButtonFont.LoadFont(ControllerButtons_Data, ControllerButtons_Size, ranges, false);

		io.Fonts->Build();

		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui_ImplDX11_CreateDeviceObjects();

		io.FontDefault = defaultFont.GetFont();

		loadedFonts = true;
	}

	ImFont* Manager::GetLargeFont() const
	{
		return defaultFont.GetLargeFont();
	}

	ImFont* Manager::GetControllerButtonFont() const
	{
		return controllerButtonFont.GetFont();
	}

	Button Manager::GetIcon(std::uint32_t key)
	{
		if (auto inputDevice = MANAGER(Input)->GetInputDevice(); inputDevice == Input::DEVICE::kKeyboard || inputDevice == Input::DEVICE::kMouse) {
			if (key >= F4SE::InputMap::kMacro_MouseButtonOffset) {
				if (const auto it = mouse.find(key); it != mouse.end()) {
					return { it->second, false };
				}
			} else if (const auto it = keyboard.find(static_cast<RE::BS_BUTTON_CODE>(key)); it != keyboard.end()) {
				return { it->second, false };
			}
		} else {
			if (const auto it = gamePad.find(key); it != gamePad.end()) {
				return GetGamePadIcon(it->second);
			}
			return { unknownKeyController, true };
		}
		return { unknownKey, false };
	}

	Button Manager::GetGamePadIcon(const GamePadButton& a_icons) const
	{
		std::string text;

		switch (buttonScheme) {
		case BUTTON_SCHEME::kAutoDetect:
			text = MANAGER(Input)->GetInputDevice() == Input::DEVICE::kGamepadOrbis ? a_icons.ps4 : a_icons.xbox;
			break;
		case BUTTON_SCHEME::kXbox:
			text = a_icons.xbox;
			break;
		case BUTTON_SCHEME::kPS4:
			text = a_icons.ps4;
			break;
		default:
			text = a_icons.xbox;
			break;
		}

		return { text, true };
	}
}

ImVec2 ImGui::ButtonIcon(std::uint32_t a_key)
{
	const auto& [icon, useControllerFont] = MANAGER(Font)->GetIcon(a_key);
	return ButtonIcon(icon, useControllerFont, false);
}

ImVec2 ImGui::ButtonIcon(const std::string& a_key, bool a_useControllerFont, bool a_centerIcon)
{
	auto size = ImGui::CalcTextSize(a_key.c_str());
	if (a_centerIcon) {
		const float height = ImGui::GetWindowSize().y;
		ImGui::SetCursorPosY((height - size.y) / 2);
	}
	if (a_useControllerFont) {
		ImGui::PushFont(MANAGER(Font)->GetControllerButtonFont());
	}
	ImGui::TextUnformatted(a_key.c_str());
	if (a_useControllerFont) {
		ImGui::PopFont();
	}
	return size + GetStyle().ItemSpacing;
}

void ImGui::ButtonIconWithLabel(const char* a_text, const Font::Button& a_key, bool a_centerIcon)
{
	auto& [icon, useControllerButtonFont] = a_key;
	ImGui::ButtonIcon(icon, useControllerButtonFont, a_centerIcon);
	ImGui::SameLine();
	ImGui::Text(a_text);
}
