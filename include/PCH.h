#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800
#define IMGUI_DEFINE_MATH_OPERATORS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#define MANAGER(T) T::Manager::GetSingleton()

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <codecvt>
#include <fstream>
#include <set>
#include <wrl/client.h>

#include <DirectXMath.h>
#include <DirectXTex.h>

#include <ankerl/unordered_dense.h>
#include <freetype/freetype.h>
#include <rapidfuzz/rapidfuzz_all.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <srell.hpp>
#include <xbyak/xbyak.h>

#include "imgui_internal.h"
#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <ClibUtil/RNG.hpp>
#include <ClibUtil/hash.hpp>
#include <ClibUtil/simpleINI.hpp>
#include <ClibUtil/singleton.hpp>
#include <ClibUtil/string.hpp>

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

using EventResult = RE::BSEventNotifyControl;

using namespace std::literals;
using namespace clib_util;
using namespace string::literals;
using namespace clib_util::singleton;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

template <class K, class D>
using Map = ankerl::unordered_dense::map<K, D>;

struct string_hash
{
	using is_transparent = void;  // enable heterogeneous overloads
	using is_avalanching = void;  // mark class as high quality avalanching hash

	[[nodiscard]] std::uint64_t operator()(std::string_view str) const noexcept
	{
		return ankerl::unordered_dense::hash<std::string_view>{}(str);
	}
};

template <class D>
using StringMap = ankerl::unordered_dense::map<std::string, D, string_hash, std::equal_to<>>;

namespace stl
{
	using namespace F4SE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		F4SE::AllocTrampoline(14);

		auto& trampoline = F4SE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class T>
	void write_thunk_jmp(std::uintptr_t a_src)
	{
		F4SE::AllocTrampoline(14);

		auto& trampoline = F4SE::GetTrampoline();
		T::func = trampoline.write_branch<5>(a_src, T::thunk);
	}

	template <class F, size_t index, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[index] };
		T::func = vtbl.write_vfunc(T::idx, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		write_vfunc<F, 0, T>();
	}
}

#include "RE.h"
#include "Translation.h"
#include "Version.h"
