#pragma once

#include <map>
#include <filesystem>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Texture.h>


namespace drawables {

    class TextureLoader {
    public:
        TextureLoader();

        ~TextureLoader();

        Magnum::GL::Texture2D &loadTexture(const std::filesystem::path &filename, int &width, int &height);

    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };

}
