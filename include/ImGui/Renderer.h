#pragma once

namespace ImGui::Renderer
{
	inline std::atomic initialized{ false };

	float GetResolutionScale();

	void Install();
}
