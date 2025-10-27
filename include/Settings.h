#pragma once

enum class FileType
{
	kFonts,
	kMCM,
};

class Settings
{
public:
	using INIFunc = std::function<void(CSimpleIniA&)>;

	static Settings* GetSingleton()
	{
		return &instance;
	}

	void Load(FileType type, INIFunc a_func, bool a_generate = false) const;
	void Save(FileType type, INIFunc a_func, bool a_generate = false) const;

	void LoadMCMSettings() const;

private:
	static void LoadINI(const wchar_t* a_path, INIFunc a_func, bool a_generate = false);
	static void LoadINI(const wchar_t* a_defaultPath, const wchar_t* a_userPath, INIFunc a_func);

	// members
	const wchar_t* fontsPath{ L"Data/Interface/PhotoMode/fonts.ini" };

	const wchar_t* defaultMCMPath{ L"Data/MCM/Config/PhotoMode/settings.ini" };
	const wchar_t* userMCMPath{ L"Data/MCM/Settings/PhotoMode.ini" };

	static Settings instance;
};

inline constinit Settings Settings::instance;
