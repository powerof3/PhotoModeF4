#pragma once

namespace Texture
{
	std::string Sanitize(std::string& a_path);

	void AlphaBlendImage(const DirectX::Image* a_baseImg, const DirectX::Image* a_overlayImg, DirectX::ScratchImage& resultImg, float a_intensity);

	void SaveToPNG(const DirectX::ScratchImage& a_inputImage, std::string_view a_path);
}
