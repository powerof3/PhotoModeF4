#include "Input.h"

#include "PhotoMode/Hotkeys.h"
#include "PhotoMode/Manager.h"
#include "Screenshots/Manager.h"

namespace Input
{
	DEVICE Manager::GetInputDevice() const
	{
		return inputDevice;
	}

	void Manager::Register()
	{
		RE::MenuControls::GetSingleton()->handlers.emplace_back(GetSingleton());
		logger::info("Registered hotkey handler");
	}

	void Manager::LoadMCMSettings(const CSimpleIniA& a_ini)
	{
		keyHeldDuration = static_cast<float>(a_ini.GetDoubleValue("Controls", "iKeyHeldDuration", keyHeldDuration));
	}

	bool Manager::IsScreenshotQueued() const
	{
		return screenshotQueued;
	}

	void Manager::QueueScreenshot(bool a_forceQueue)
	{
		screenshotQueued = true;

		if (MANAGER(Screenshot)->CanAutoHideMenus()) {
			HideMenu(true);
		}

		if (inputDevice != DEVICE::kKeyboard || a_forceQueue) {
			RE::MenuControls::GetSingleton()->QueueScreenshot();
		}
	}

	void Manager::OnScreenshotFinish()
	{
		if (screenshotQueued) {
			screenshotQueued = false;
			HideMenu(false);
		}
	}

	ImGuiKey Manager::ToImGuiKey(RE::BS_BUTTON_CODE a_key)
	{
		switch (a_key) {
		case RE::BS_BUTTON_CODE::kTab:
			return ImGuiKey_Tab;
		case RE::BS_BUTTON_CODE::kLeft:
			return ImGuiKey_LeftArrow;
		case RE::BS_BUTTON_CODE::kRight:
			return ImGuiKey_RightArrow;
		case RE::BS_BUTTON_CODE::kUp:
			return ImGuiKey_UpArrow;
		case RE::BS_BUTTON_CODE::kDown:
			return ImGuiKey_DownArrow;
		case RE::BS_BUTTON_CODE::kPageUp:
			return ImGuiKey_PageUp;
		case RE::BS_BUTTON_CODE::kPageDown:
			return ImGuiKey_PageDown;
		case RE::BS_BUTTON_CODE::kHome:
			return ImGuiKey_Home;
		case RE::BS_BUTTON_CODE::kEnd:
			return ImGuiKey_End;
		case RE::BS_BUTTON_CODE::kInsert:
			return ImGuiKey_Insert;
		case RE::BS_BUTTON_CODE::kDelete:
			return ImGuiKey_Delete;
		case RE::BS_BUTTON_CODE::kBackspace:
			return ImGuiKey_Backspace;
		case RE::BS_BUTTON_CODE::kSpace:
			return ImGuiKey_Space;
		case RE::BS_BUTTON_CODE::kEnter:
			return ImGuiKey_Enter;
		case RE::BS_BUTTON_CODE::kEscape:
			return ImGuiKey_Escape;
		case RE::BS_BUTTON_CODE::kApostrophe:
			return ImGuiKey_Apostrophe;
		case RE::BS_BUTTON_CODE::kComma:
			return ImGuiKey_Comma;
		case RE::BS_BUTTON_CODE::kMinus:
			return ImGuiKey_Minus;
		case RE::BS_BUTTON_CODE::kPeriod:
			return ImGuiKey_Period;
		/*case RE::BS_BUTTON_CODE::kSlash:
			return ImGuiKey_Slash;*/
		case RE::BS_BUTTON_CODE::kSemicolon:
			return ImGuiKey_Semicolon;
		case RE::BS_BUTTON_CODE::kEquals:
			return ImGuiKey_Equal;
		case RE::BS_BUTTON_CODE::kLBracket:
			return ImGuiKey_LeftBracket;
		case RE::BS_BUTTON_CODE::kBackslash:
			return ImGuiKey_Backslash;
		case RE::BS_BUTTON_CODE::kRBracket:
			return ImGuiKey_RightBracket;
		/*case RE::BS_BUTTON_CODE::kTilde:
			return ImGuiKey_GraveAccent;*/
		case RE::BS_BUTTON_CODE::kCapsLock:
			return ImGuiKey_CapsLock;
		case RE::BS_BUTTON_CODE::kScrollLock:
			return ImGuiKey_ScrollLock;
		case RE::BS_BUTTON_CODE::kNumLock:
			return ImGuiKey_NumLock;
		case RE::BS_BUTTON_CODE::kPrintScreen:
			return ImGuiKey_PrintScreen;
		case RE::BS_BUTTON_CODE::kPause:
			return ImGuiKey_Pause;
		case RE::BS_BUTTON_CODE::kNumpad_0:
			return ImGuiKey_Keypad0;
		case RE::BS_BUTTON_CODE::kNumpad_1:
			return ImGuiKey_Keypad1;
		case RE::BS_BUTTON_CODE::kNumpad_2:
			return ImGuiKey_Keypad2;
		case RE::BS_BUTTON_CODE::kNumpad_3:
			return ImGuiKey_Keypad3;
		case RE::BS_BUTTON_CODE::kNumpad_4:
			return ImGuiKey_Keypad4;
		case RE::BS_BUTTON_CODE::kNumpad_5:
			return ImGuiKey_Keypad5;
		case RE::BS_BUTTON_CODE::kNumpad_6:
			return ImGuiKey_Keypad6;
		case RE::BS_BUTTON_CODE::kNumpad_7:
			return ImGuiKey_Keypad7;
		case RE::BS_BUTTON_CODE::kNumpad_8:
			return ImGuiKey_Keypad8;
		case RE::BS_BUTTON_CODE::kNumpad_9:
			return ImGuiKey_Keypad9;
		case RE::BS_BUTTON_CODE::kNumpad_Period:
			return ImGuiKey_KeypadDecimal;
		case RE::BS_BUTTON_CODE::kNumpad_Divide:
			return ImGuiKey_KeypadDivide;
		case RE::BS_BUTTON_CODE::kNumpad_Multiply:
			return ImGuiKey_KeypadMultiply;
		case RE::BS_BUTTON_CODE::kNumpad_Minus:
			return ImGuiKey_KeypadSubtract;
		case RE::BS_BUTTON_CODE::kNumpad_Plus:
			return ImGuiKey_KeypadAdd;
		/*case RE::BS_BUTTON_CODE::kNumpad_Enter:
			return ImGuiKey_KeypadEnter;*/
		case RE::BS_BUTTON_CODE::kLShift:
			return ImGuiKey_LeftShift;
		case RE::BS_BUTTON_CODE::kLControl:
			return ImGuiKey_LeftCtrl;
		case RE::BS_BUTTON_CODE::kLAlt:
			return ImGuiKey_LeftAlt;
		/*case RE::BS_BUTTON_CODE::kLeftWin:
			return ImGuiKey_LeftSuper;*/
		case RE::BS_BUTTON_CODE::kRShift:
			return ImGuiKey_RightShift;
		case RE::BS_BUTTON_CODE::kRControl:
			return ImGuiKey_RightCtrl;
		case RE::BS_BUTTON_CODE::kRAlt:
			return ImGuiKey_RightAlt;
		/*case RE::BS_BUTTON_CODE::kRightWin:
			return ImGuiKey_RightSuper;*/
		case RE::BS_BUTTON_CODE::kApps:
			return ImGuiKey_Menu;
		case RE::BS_BUTTON_CODE::k0:
			return ImGuiKey_0;
		case RE::BS_BUTTON_CODE::k1:
			return ImGuiKey_1;
		case RE::BS_BUTTON_CODE::k2:
			return ImGuiKey_2;
		case RE::BS_BUTTON_CODE::k3:
			return ImGuiKey_3;
		case RE::BS_BUTTON_CODE::k4:
			return ImGuiKey_4;
		case RE::BS_BUTTON_CODE::k5:
			return ImGuiKey_5;
		case RE::BS_BUTTON_CODE::k6:
			return ImGuiKey_6;
		case RE::BS_BUTTON_CODE::k7:
			return ImGuiKey_7;
		case RE::BS_BUTTON_CODE::k8:
			return ImGuiKey_8;
		case RE::BS_BUTTON_CODE::k9:
			return ImGuiKey_9;
		case RE::BS_BUTTON_CODE::kA:
			return ImGuiKey_A;
		case RE::BS_BUTTON_CODE::kB:
			return ImGuiKey_B;
		case RE::BS_BUTTON_CODE::kC:
			return ImGuiKey_C;
		case RE::BS_BUTTON_CODE::kD:
			return ImGuiKey_D;
		case RE::BS_BUTTON_CODE::kE:
			return ImGuiKey_E;
		case RE::BS_BUTTON_CODE::kF:
			return ImGuiKey_F;
		case RE::BS_BUTTON_CODE::kG:
			return ImGuiKey_G;
		case RE::BS_BUTTON_CODE::kH:
			return ImGuiKey_H;
		case RE::BS_BUTTON_CODE::kI:
			return ImGuiKey_I;
		case RE::BS_BUTTON_CODE::kJ:
			return ImGuiKey_J;
		case RE::BS_BUTTON_CODE::kK:
			return ImGuiKey_K;
		case RE::BS_BUTTON_CODE::kL:
			return ImGuiKey_L;
		case RE::BS_BUTTON_CODE::kM:
			return ImGuiKey_M;
		case RE::BS_BUTTON_CODE::kN:
			return ImGuiKey_N;
		case RE::BS_BUTTON_CODE::kO:
			return ImGuiKey_O;
		case RE::BS_BUTTON_CODE::kP:
			return ImGuiKey_P;
		case RE::BS_BUTTON_CODE::kQ:
			return ImGuiKey_Q;
		case RE::BS_BUTTON_CODE::kR:
			return ImGuiKey_R;
		case RE::BS_BUTTON_CODE::kS:
			return ImGuiKey_S;
		case RE::BS_BUTTON_CODE::kT:
			return ImGuiKey_T;
		case RE::BS_BUTTON_CODE::kU:
			return ImGuiKey_U;
		case RE::BS_BUTTON_CODE::kV:
			return ImGuiKey_V;
		case RE::BS_BUTTON_CODE::kW:
			return ImGuiKey_W;
		case RE::BS_BUTTON_CODE::kX:
			return ImGuiKey_X;
		case RE::BS_BUTTON_CODE::kY:
			return ImGuiKey_Y;
		case RE::BS_BUTTON_CODE::kZ:
			return ImGuiKey_Z;
		case RE::BS_BUTTON_CODE::kF1:
			return ImGuiKey_F1;
		case RE::BS_BUTTON_CODE::kF2:
			return ImGuiKey_F2;
		case RE::BS_BUTTON_CODE::kF3:
			return ImGuiKey_F3;
		case RE::BS_BUTTON_CODE::kF4:
			return ImGuiKey_F4;
		case RE::BS_BUTTON_CODE::kF5:
			return ImGuiKey_F5;
		case RE::BS_BUTTON_CODE::kF6:
			return ImGuiKey_F6;
		case RE::BS_BUTTON_CODE::kF7:
			return ImGuiKey_F7;
		case RE::BS_BUTTON_CODE::kF8:
			return ImGuiKey_F8;
		case RE::BS_BUTTON_CODE::kF9:
			return ImGuiKey_F9;
		case RE::BS_BUTTON_CODE::kF10:
			return ImGuiKey_F10;
		case RE::BS_BUTTON_CODE::kF11:
			return ImGuiKey_F11;
		case RE::BS_BUTTON_CODE::kF12:
			return ImGuiKey_F12;
		default:
			return ImGuiKey_None;
		}
	}

	ImGuiKey Manager::ToImGuiKey_DirectX(RE::BS_BUTTON_CODE a_key)
	{
		switch (a_key) {
		case RE::BS_BUTTON_CODE::kDPAD_Up:
			return ImGuiKey_GamepadDpadUp;
		case RE::BS_BUTTON_CODE::kDPAD_Down:
			return ImGuiKey_GamepadDpadDown;
		case RE::BS_BUTTON_CODE::kDPAD_Left:
			return ImGuiKey_GamepadDpadLeft;
		case RE::BS_BUTTON_CODE::kDPAD_Right:
			return ImGuiKey_GamepadDpadRight;
		case RE::BS_BUTTON_CODE::kStart:
			return ImGuiKey_GamepadStart;
		case RE::BS_BUTTON_CODE::kBack:
			return ImGuiKey_GamepadBack;
		case RE::BS_BUTTON_CODE::kLStick:
			return ImGuiKey_GamepadL3;
		case RE::BS_BUTTON_CODE::kRStick:
			return ImGuiKey_GamepadR3;
		case RE::BS_BUTTON_CODE::kLShoulder:
			return ImGuiKey_GamepadL1;
		case RE::BS_BUTTON_CODE::kRShoulder:
			return ImGuiKey_GamepadR1;
		case RE::BS_BUTTON_CODE::kAButton:
			return ImGuiKey_GamepadFaceDown;
		case RE::BS_BUTTON_CODE::kBButton:
			return ImGuiKey_GamepadFaceRight;
		case RE::BS_BUTTON_CODE::kXButton:
			return ImGuiKey_GamepadFaceLeft;
		case RE::BS_BUTTON_CODE::kYButton:
			return ImGuiKey_GamepadFaceUp;
		default:
			return ImGuiKey_None;
		}
	}

	// faking this with keyboard inputs, since ImGUI doesn't support DirectInput
	ImGuiKey Manager::ToImGuiKey_Orbis(RE::BS_BUTTON_CODE a_key)
	{
		switch (a_key) {
		// Move / Tweak / Resize Window (in Windowing mode)
		case RE::BS_BUTTON_CODE::kDPAD_Up:
			return ImGuiKey_UpArrow;
		// Move / Tweak / Resize Window (in Windowing mode)
		case RE::BS_BUTTON_CODE::kDPAD_Down:
			return ImGuiKey_DownArrow;
		// Move / Tweak / Resize Window (in Windowing mode)
		case RE::BS_BUTTON_CODE::kDPAD_Left:
			return ImGuiKey_LeftArrow;
		// Move / Tweak / Resize Window (in Windowing mode)
		case RE::BS_BUTTON_CODE::kDPAD_Right:
			return ImGuiKey_RightArrow;

		case RE::BS_BUTTON_CODE::kStart:
			return ImGuiKey_GamepadStart;
		case RE::BS_BUTTON_CODE::kBack:
			return ImGuiKey_GamepadBack;
		case RE::BS_BUTTON_CODE::kLStick:
			return ImGuiKey_GamepadL3;
		case RE::BS_BUTTON_CODE::kRStick:
			return ImGuiKey_GamepadR3;

		// Tweak Slower / Focus Previous (in Windowing mode)
		case RE::BS_BUTTON_CODE::kLTrigger:
			return ImGuiKey_NavKeyboardTweakSlow;
		// Tweak Faster / Focus Next (in Windowing mode)
		case RE::BS_BUTTON_CODE::kRTrigger:
			return ImGuiKey_NavKeyboardTweakFast;
		// Activate / Open / Toggle / Tweak
		case RE::BS_BUTTON_CODE::kAButton:
			return ImGuiKey_Enter;
		// Cancel / Close / Exit
		case RE::BS_BUTTON_CODE::kBButton:
			return ImGuiKey_Escape;

		case RE::BS_BUTTON_CODE::kXButton:
			return ImGuiKey_GamepadFaceLeft;
		case RE::BS_BUTTON_CODE::kYButton:
			return ImGuiKey_GamepadFaceUp;
		default:
			return ImGuiKey_None;
		}
	}

	void Manager::SendKeyEvent(RE::BS_BUTTON_CODE a_key, bool a_keyPressed) const
	{
		auto& io = ImGui::GetIO();

		ImGuiKey key{ ImGuiKey_None };
		switch (inputDevice) {
		case DEVICE::kKeyboard:
			key = ToImGuiKey(a_key);
			break;
		case DEVICE::kGamepadDirectX:
			key = ToImGuiKey_DirectX(a_key);
			break;
		case DEVICE::kGamepadOrbis:
			key = ToImGuiKey_Orbis(a_key);
			break;
		default:
			break;
		}

		io.AddKeyEvent(key, a_keyPressed);
	}

	void Manager::HideMenu(bool a_hide)
	{
		if (a_hide && RE::UI::GetSingleton()->menuSystemVisible) {
			RE::UI::GetSingleton()->menuSystemVisible = false;
			RE::Interface3D::Renderer::DisableAll(true);
			menuHidden = true;
		}
		if (!a_hide && menuHidden) {
			RE::UI::GetSingleton()->menuSystemVisible = true;
			RE::Interface3D::Renderer::DisableAll(false);
			menuHidden = false;
		}
	}

	bool Manager::ShouldHandleEvent(const RE::InputEvent*)
	{
		if (const auto photoMode = MANAGER(PhotoMode); photoMode->IsActive()) {
			if (!photoMode->ShouldBlockInput() && RE::Main::GetSingleton()->gameActive) {
				return true;
			}
		}
		return false;
	}

	void Manager::OnCharacterEvent(const RE::CharacterEvent* a_event)
	{
		ImGui::GetIO().AddInputCharacter(a_event->charCode);
	}

	void Manager::OnButtonEvent(const RE::ButtonEvent* a_event)
	{
		const auto key = a_event->QIDCode();
		const auto device = a_event->device.get();
		auto       hotKey = key;

		// get input type
		switch (device) {
		case RE::INPUT_DEVICE::kKeyboard:
			inputDevice = DEVICE::kKeyboard;
			break;
		case RE::INPUT_DEVICE::kMouse:
			{
				hotKey += F4SE::InputMap::kMacro_MouseButtonOffset;
				inputDevice = DEVICE::kMouse;
			}
			break;
		case RE::INPUT_DEVICE::kGamepad:
			{
				hotKey = F4SE::InputMap::GamepadMaskToKeycode(hotKey);
				if (RE::ControlMap::GetSingleton()->pcGamePadMapType == RE::PC_GAMEPAD_TYPE::kOrbis) {
					inputDevice = DEVICE::kGamepadOrbis;
				} else {
					inputDevice = DEVICE::kGamepadDirectX;
				}
			}
			break;
		default:
			return;
		}

		// recreate vertical pan event for mouse
		if (inputDevice == DEVICE::kMouse) {
			if (auto freeCameraState = static_cast<RE::FreeCameraState*>(RE::PlayerCamera::GetSingleton()->currentState.get())) {
				if (key == 0) {  // LeftButton
					std::uint16_t value = (value & 0x00ff) | (0 << 8);
					bool          released = true;
					if (a_event->value != 0.0 || a_event->heldDownSecs < 0.0) {
						released = false;
					}
					value = (value & 0xff00) | (released == false);
					freeCameraState->worldZDirection = value;
					return;
				} else if (key == 1) {  // RightButton
					if (a_event->value == 0.0 && a_event->heldDownSecs >= 0.0) {
						freeCameraState->worldZDirection = 0;
					} else {
						freeCameraState->worldZDirection = -1;
					}
					return;
				}
			}
		}

		const auto photoMode = MANAGER(PhotoMode);
		const auto hotKeys = MANAGER(PhotoMode::Hotkeys);
		auto&      io = ImGui::GetIO();

		if (!io.WantTextInput) {
			if (hotKey == hotKeys->ToggleMenusKey() && a_event->QJustPressed()) {
				photoMode->ToggleUI();
			} else if (hotKey == hotKeys->TakePhotoKey()) {  // only fires for non-prtscreen keys
				if (a_event->QJustPressed() || MANAGER(Screenshot)->AllowMultiScreenshots() && a_event->QHeldDownSecs() > keyHeldDuration) {
					QueueScreenshot(true);
				}
			} else if (!photoMode->IsHidden()) {
				if (hotKey == hotKeys->NextTabKey() && a_event->QJustPressed()) {
					photoMode->NavigateTab(false);
				} else if (hotKey == hotKeys->PreviousTabKey() && a_event->QJustPressed()) {
					photoMode->NavigateTab(true);
				} else if (hotKey == hotKeys->FreezeTimeKey() && a_event->QJustPressed()) {
					RE::Main::GetSingleton()->freezeTime = !RE::Main::GetSingleton()->freezeTime;
				} else if (hotKey == hotKeys->ResetKey()) {
					if (a_event->QReleased()) {
						photoMode->Revert(false);
					} else if (a_event->QHeldDownSecs() > keyHeldDuration) {
						photoMode->DoResetAll();
					}
				}
			}
		}

		if (!photoMode->IsHidden() || hotKey == hotKeys->EscapeKey()) {
			SendKeyEvent(static_cast<RE::BS_BUTTON_CODE>(key), a_event->QPressed());
		}
	}
}
