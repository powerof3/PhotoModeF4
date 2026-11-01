#include "Graphics.h"
#include "ImageMath.h"

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

    bool AlphaBlendImage(const DirectX::Image* base, const DirectX::Image* overlay,
        DirectX::ScratchImage& output, float intensity)
    {
        if (!base || !overlay || base->width != overlay->width || base->height != overlay->height ||
            base->format != DXGI_FORMAT_R8G8B8A8_UNORM || overlay->format != base->format) {
            return false;
        }
        if (FAILED(output.InitializeFromImage(*base))) return false;
        const auto* result = output.GetImage(0, 0, 0);
        return ImageMath::CompositeOpaque(result->pixels, result->rowPitch,
            overlay->pixels, overlay->rowPitch, base->width, base->height, intensity);
    }

    bool SaveToPNG(const DirectX::ScratchImage& input, std::string_view path)
    {
        const auto* source = input.GetImage(0, 0, 0);
        if (!source) return false;
        DirectX::ScratchImage rgba;
        HRESULT hr = source->format == DXGI_FORMAT_R8G8B8A8_UNORM ?
            rgba.InitializeFromImage(*source) :
            DirectX::Convert(*source, DXGI_FORMAT_R8G8B8A8_UNORM,
                DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, rgba);
        if (FAILED(hr)) return false;
        const auto* image = rgba.GetImage(0, 0, 0);
        for (std::size_t y = 0; y < image->height; ++y) {
            auto* row = image->pixels + y * image->rowPitch;
            for (std::size_t x = 0; x < image->width; ++x) row[x * 4 + 3] = 255;
        }
        const auto wide = stl::utf8_to_utf16(path);
        if (!wide) return false;
        hr = DirectX::SaveToWICFile(*image, DirectX::WIC_FLAGS_FORCE_SRGB,
            DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG), wide->c_str());
        if (FAILED(hr)) logger::error("Failed to save PNG {}: HRESULT {}", path, static_cast<unsigned long>(hr));
        return SUCCEEDED(hr);
    }
}
