#include "Screenshots/Manager.h"

#include "Graphics.h"
#include "PhotoMode/Manager.h"
#include "Settings.h"

namespace Screenshot
{
	Image::Image(std::string_view a_path, std::uint32_t a_index) :
		index(a_index),
		path(std::format("{}/screenshot{}.dds", a_path, a_index))
	{}

	Image::Image(std::string& a_path) :
		path(Texture::Sanitize(a_path))
	{
		srell::smatch       matches;
		static srell::regex screenshotPattern{ R"(screenshot(\d+))" };
		if (srell::regex_search(path, matches, screenshotPattern)) {
			if (matches.size() > 1) {
				index = string::to_num<std::int32_t>(matches[1].str());
			}
		}
	}
	
	void Manager::LoadMCMSettings(const CSimpleIniA& a_ini)
	{
		useCustomFolderDirectory = a_ini.GetBoolValue("Screenshots", "bCustomPhotoFolder", useCustomFolderDirectory);
		autoHideMenus = a_ini.GetBoolValue("Screenshots", "bAutoHideMenus", autoHideMenus);
		allowMultiScreenshots = a_ini.GetBoolValue("Screenshots", "bMultiScreenshots", allowMultiScreenshots);
	}

	void Manager::LoadScreenshots()
	{
		logger::info("Loading screenshots...");

		if (auto directory = logger::log_directory()) {
			directory->remove_filename();
			*directory /= "Photos";
			photoDirectory = *directory;
		}

		if (!std::filesystem::exists(photoDirectory)) {
			std::filesystem::create_directory(photoDirectory);
		}

		logger::info("\tScreenshot directory : {}", photoDirectory.string());

		Settings::GetSingleton()->SerializeMCM([this](auto& ini) {
			index = ini.GetLongValue("Screenshots", "iScreenshotIndex", index);
			AssignHighestPossibleIndex();
			ini.SetLongValue("Screenshots", "iScreenshotIndex", index);
		});

		logger::info("\tscreenshot index : {}", index);
	}

	std::uint32_t Manager::GetIndex() const
	{
		return index;
	}

	void Manager::AssignHighestPossibleIndex()
	{
		const auto get_photos_index = [this]() {
			std::vector<Image> photos{};
			for (const auto& entry : std::filesystem::directory_iterator(photoDirectory)) {
				if (entry.is_regular_file()) {
					if (const auto& path = entry.path(); path.extension() == ".png") {
						auto pathStr = entry.path().string();
						photos.push_back(pathStr);
					}
				}
			}
			if (photos.empty()) {
				return -1;
			}
			std::sort(photos.begin(), photos.end());
			return photos.back().index + 1;
		};

		auto mcmIndex = index;
		auto photosIndex = get_photos_index();
		auto vanillaScreenshotIndex = RE::GetINISetting("iScreenShotIndex:Display")->GetInt();

		logger::info("\tAssigning highest screenshot index...");
		logger::info("\t\tmcm index: {}", mcmIndex);
		logger::info("\t\tphoto directory index: {}", photosIndex);
		logger::info("\t\tvanilla directory index: {}", vanillaScreenshotIndex);

		std::set<std::int32_t> indices;
		indices.insert({ mcmIndex, photosIndex, vanillaScreenshotIndex });

		index = *indices.rbegin();
	}

	void Manager::IncrementIndex()
	{
		index++;
		Settings::GetSingleton()->SerializeMCM([this](auto& ini) {
			ini.SetLongValue("Screenshots", "iScreenshotIndex", index);
		});
	}

	bool Manager::AllowMultiScreenshots() const
	{
		return allowMultiScreenshots;
	}

	bool Manager::CanAutoHideMenus() const
	{
		return autoHideMenus;
	}

	bool Manager::TakeScreenshot()
	{
		bool skipVanillaScreenshot = false;

		const auto renderer = RE::BSGraphics::RendererData::GetSingleton();
		if (!renderer) {
			return skipVanillaScreenshot;
		}

		// capture screenshot
		DirectX::ScratchImage inputImage{};

		const ComPtr<ID3D11Device>        device{ (ID3D11Device*)renderer->device };
		const ComPtr<ID3D11DeviceContext> deviceContext{ (ID3D11DeviceContext*)renderer->context };

		const auto       renderTarget = RE::BSGraphics::RenderTargetManager::GetSingleton().renderTargetID[63];
		ID3D11Texture2D* texture2D{ (ID3D11Texture2D*)renderer->renderTargets[renderTarget].texture };

		if (auto result = DirectX::CaptureTexture(device.Get(), deviceContext.Get(), texture2D, inputImage); result == S_OK) {
			skipVanillaScreenshot = true;

			std::string pngPath = useCustomFolderDirectory ? std::format("{}\\Screenshot{}.png", photoDirectory.string(), GetIndex()) :
			                                                 std::format("{}_{}.png", RE::GetINISetting("sScreenShotBaseName:Display")->GetString(), GetIndex());

			// apply overlay
			if (const auto [overlay, alpha] = MANAGER(PhotoMode)->GetOverlay(); overlay) {
				DirectX::ScratchImage overlayImage;
				DirectX::ScratchImage blendedImage;

				// Convert PNG B8G8R8 format to R8G8B8
				DirectX::Convert(overlay->image->GetImages(), 1,
					overlay->image->GetMetadata(),
					inputImage.GetMetadata().format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT,
					overlayImage);

				Texture::AlphaBlendImage(inputImage.GetImages(), overlayImage.GetImages(), blendedImage, alpha);
				Texture::SaveToPNG(blendedImage, pngPath);

				overlayImage.Release();
				blendedImage.Release();
			} else {
				Texture::SaveToPNG(inputImage, pngPath);
			}

			IncrementIndex();
		}

		inputImage.Release();

		return skipVanillaScreenshot;
	}
}
