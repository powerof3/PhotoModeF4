#include "Console.h"

#include "PhotoMode/Manager.h"

namespace Console
{
	namespace detail
	{
		constexpr auto LONG_NAME = "PhotoMode"sv;
		constexpr auto SHORT_NAME = ""sv;

		[[nodiscard]] const std::string& HelpString()
		{
			static auto help = []() {
				std::string buf;
				buf += "Start PhotoMode\n";
				return buf;
			}();
			return help;
		}

		static bool Execute(const RE::SCRIPT_PARAMETER*, const char*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, float&, std::uint32_t&)
		{
			MANAGER(PhotoMode)->ToggleActive();

			return true;
		}
	}

	void Install()
	{
		const auto functions = RE::SCRIPT_FUNCTION::GetConsoleFunctions();
		const auto it = std::find_if(
			functions.begin(),
			functions.end(),
			[&](auto&& a_elem) {
				return _stricmp(a_elem.functionName, "TogglePathLine") == 0;
			});
		if (it != functions.end()) {
			*it = RE::SCRIPT_FUNCTION{ detail::LONG_NAME.data(), detail::SHORT_NAME.data(), it->output };
			it->helpString = detail::HelpString().data();
			it->referenceFunction = false;
			it->executeFunction = &detail::Execute;
			it->conditionFunction = nullptr;

			logger::debug("installed {}", detail::LONG_NAME);
		} else {
			stl::report_and_fail("failed to find function"sv);
		}
	}
}
