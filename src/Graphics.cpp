#include "Graphics.h"

namespace Texture
{
	std::string Sanitize(std::string& a_path)
	{
		a_path = clib_util::string::tolower(a_path);

		a_path = srell::regex_replace(a_path, srell::regex("/+|\\\\+"), "\\");
		a_path = srell::regex_replace(a_path, srell::regex("^\\\\+"), "");
		a_path = srell::regex_replace(a_path, srell::regex(R"(.*?[^\s]textures\\|^textures\\)", srell::regex::icase), "");

		return a_path;
	}

	void AlphaBlendImage(const DirectX::Image* a_baseImg, const DirectX::Image* a_overlayImg, DirectX::ScratchImage& a_outImage, float a_intensity)
	{
		auto hr = a_outImage.InitializeFromImage(*a_baseImg);
		if (FAILED(hr)) {
			return;
		}

		const auto resultImage = a_outImage.GetImages();

		const std::size_t width = a_baseImg->width;
		const std::size_t height = a_baseImg->height;
		const std::size_t pixelSize = DirectX::BitsPerPixel(a_baseImg->format) / 8;

		auto processRows = [&](const std::size_t startRow, const std::size_t endRow) {
			for (std::size_t y = startRow; y < endRow; y++) {
				std::uint8_t*       resultPixel = resultImage->pixels + (y * resultImage->rowPitch);
				const std::uint8_t* basePixel = a_baseImg->pixels + (y * a_baseImg->rowPitch);
				const std::uint8_t* overlayPixel = a_overlayImg->pixels + (y * a_overlayImg->rowPitch);

				for (std::size_t x = 0; x < width; x++) {
					if (const float overlayAlpha = (overlayPixel[x * pixelSize + 3] / 255.0f) * a_intensity; overlayAlpha > 0.0f) {
						const float baseAlpha = 1.0f - overlayAlpha;

						for (std::size_t i = 0; i < pixelSize - 1; i++) {
							float blendedValue = (overlayPixel[x * pixelSize + i] * overlayAlpha) + (basePixel[x * pixelSize + i] * baseAlpha);
							resultPixel[x * pixelSize + i] = static_cast<std::uint8_t>(std::round(std::min(blendedValue, 255.0f)));
						}
					}
				}
			}
		};

		const auto numThreads = std::thread::hardware_concurrency();

		std::vector<std::jthread> threads;
		threads.reserve(numThreads);

		const std::size_t rowsPerThread = height / numThreads;

		for (std::size_t i = 0; i < numThreads; ++i) {
			std::size_t startRow = i * rowsPerThread;
			std::size_t endRow = (i + 1) * rowsPerThread;

			threads.emplace_back(std::jthread(processRows, startRow, endRow));
		}

		for (auto& thread : threads) {
			thread.join();
		}
	}

	void SaveToPNG(const DirectX::ScratchImage& a_inputImage, std::string_view a_path)
	{
		// Save texture
		const auto wPath = stl::utf8_to_utf16(a_path);
		auto       hr = DirectX::SaveToWICFile(*a_inputImage.GetImage(0, 0, 0), DirectX::WIC_FLAGS_FORCE_SRGB,
				  DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG), wPath->c_str());
		if (FAILED(hr)) {
			logger::info("Failed to save png");
		}
	}
}
