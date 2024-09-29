#include "Translation.h"

namespace Translation
{
	std::string Manager::GetGameLanguage()
	{
		const auto setting = RE::GetINISetting("sLanguage:General");
		return (setting && setting->GetType() == RE::Setting::SETTING_TYPE::kString) ? string::toupper(setting->GetString()) : "EN"s;
	}

	void Manager::BuildTranslationMap()
	{
		std::filesystem::path path{ std::format(R"(Data\Interface\Translations\PhotoMode_{}.txt)", GetGameLanguage()) };

		if (!LoadTranslation(path)) {
			logger::info("Failed to load translation file in {}, loading default EN file...", path.string());
			LoadTranslation(R"(Data\Interface\Translations\PhotoMode_EN.txt)"sv);
		}

		AddGameTranslationStrings();
	}

	bool Manager::LoadTranslation(const std::filesystem::path& a_path)
	{
		if (!std::filesystem::exists(a_path)) {
			return false;
		}

		std::wfstream filestream(a_path, std::wfstream::in | std::wfstream::binary);
		if (!filestream.good()) {
			return false;
		} else {
			logger::info("Reading translations from {}...", a_path.string());
		}

		filestream.imbue(std::locale(filestream.getloc(), new std::codecvt_utf16<wchar_t, 0x10FFFF, std::little_endian>));

		// check if the BOM is UTF-16
		constexpr wchar_t BOM_UTF16LE = 0xFEFF;
		if (filestream.get() != BOM_UTF16LE) {
			logger::info("\tBOM Error, file must be encoded in UCS-2 LE.");
			return false;
		}

		std::wstring line, key, value;

		while (std::getline(filestream, line)) {
			std::wstringstream ss(line);
			ss >> key;
			// remove leading whitespace
			std::getline(ss >> std::ws, value);
			// remove space/new line at end
			if (std::isspace(value.back())) {
				value.pop_back();
			}
			translationMap.emplace(*stl::utf16_to_utf8(key), *stl::utf16_to_utf8(value));
		}

		return true;
	}

	void Manager::AddGameTranslationStrings()
	{
		const auto scaleformManager = RE::BSScaleformManager::GetSingleton();
		const auto loader = scaleformManager ? scaleformManager->loader : nullptr;
		const auto translator = loader ? static_cast<RE::BSScaleformTranslator*>(loader->GetStateAddRef(RE::Scaleform::GFx::State::StateType::kTranslator)) : nullptr;

		auto FetchTranslation = [this](RE::BSScaleformTranslator* a_trns, const wchar_t* a_key) {
			auto it = a_trns->translator.translationMap.find(a_key);
			if (it != a_trns->translator.translationMap.end()) {
				translationMap.emplace(*stl::utf16_to_utf8(a_key), *stl::utf16_to_utf8(it->second.data()));
			}
		};

		FetchTranslation(translator, L"$ON");
		FetchTranslation(translator, L"$OFF");
		FetchTranslation(translator, L"$EXIT");
		FetchTranslation(translator, L"$YES");
		FetchTranslation(translator, L"$NO");
		FetchTranslation(translator, L"$RESET");
		FetchTranslation(translator, L"$NONE");
	}
}
