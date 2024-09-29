#pragma once

namespace Input
{
	enum class DEVICE
	{
		kKeyboard,
		kMouse,
		kGamepadDirectX,  // xbox
		kGamepadOrbis     // ps4
	};

	class Manager final :
		public ISingleton<Manager>,
		public RE::BSInputEventUser
	{
	public:
		static void Register();
		void        LoadMCMSettings(const CSimpleIniA& a_ini);

		DEVICE GetInputDevice() const;

		void HideMenu(bool a_hide);
		bool IsScreenshotQueued() const;
		void QueueScreenshot(bool a_forceQueue);
		void OnScreenshotFinish();

	private:
		static ImGuiKey ToImGuiKey(RE::BS_BUTTON_CODE a_key);
		static ImGuiKey ToImGuiKey_DirectX(RE::BS_BUTTON_CODE a_key);
		static ImGuiKey ToImGuiKey_Orbis(RE::BS_BUTTON_CODE a_key);

		void SendKeyEvent(RE::BS_BUTTON_CODE a_key, bool a_keyPressed) const;

		virtual bool ShouldHandleEvent(const RE::InputEvent*);             // 01
		virtual void OnCharacterEvent(const RE::CharacterEvent* a_event);  // 07
		virtual void OnButtonEvent(const RE::ButtonEvent* a_event);        // 08

		// members
		bool screenshotQueued{ false };
		bool menuHidden{ false };

		float keyHeldDuration{ 0.5 };

		RE::INPUT_DEVICE gameDevice;
		DEVICE           inputDevice{ DEVICE::kKeyboard };
	};
}
