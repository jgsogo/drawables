#include "texture_loader.h"

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/ImageView.h>


namespace drawables {

    struct TextureLoader::Impl {
        Impl() {
            pngImporter = manager.loadAndInstantiate("PngImporter");
        }

        ~Impl() {
            pngImporter->close();
            pngImporter.release();
            textures.clear();
        }

        std::map<std::filesystem::path, std::tuple<Magnum::GL::Texture2D, int, int>> textures;
        Corrade::Containers::Pointer <Magnum::Trade::AbstractImporter> pngImporter;
        Magnum::PluginManager::Manager <Magnum::Trade::AbstractImporter> manager;
    };

    TextureLoader::TextureLoader() : pImpl(std::make_unique<Impl>()) {
    }

    TextureLoader::~TextureLoader() = default;

    Magnum::GL::Texture2D &TextureLoader::loadTexture(const std::filesystem::path &filename, int &width, int &height) {
        auto[it, inserted] = pImpl->textures.insert(std::make_pair(filename, std::make_tuple(Magnum::GL::Texture2D{}, 0, 0)));
        if (inserted) {
            auto r = pImpl->pngImporter->openFile(filename);
            Magnum::Containers::Optional <Magnum::Trade::ImageData2D> image = pImpl->pngImporter->image2D(0);

            std::get<1>(it->second) = image->size().x();
            std::get<2>(it->second) = image->size().y();
            std::get<0>(it->second).setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
                    .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                    .setMinificationFilter(Magnum::GL::SamplerFilter::Linear)
                    .setStorage(1, Magnum::GL::textureFormat(image->format()), image->size())
                    .setSubImage(0, {}, *image);
            pImpl->pngImporter->close();
        }
        width = std::get<1>(it->second);
        height = std::get<2>(it->second);
        return std::get<0>(it->second);
    }

}
