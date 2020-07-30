
// Precompiled
#include "ospch.h"
//

#include "Texture.h"

#include <Ostaga.h>

#include <stb_image.h>

namespace Ostaga { namespace Graphics {

    Texture::Texture(unsigned char *data, int width, int height, int channels, const TextureProps &props)
        : m_Width(width), m_Height(height), m_Channels(channels), m_Props(props)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ContextID);
        LoadTexture(data);
    }

    Texture::~Texture()
    {
    }

    std::pair<GLenum, int> SelectFormat(int channels)
    {
        switch (channels)
        {
        case 1: return { GL_RED, 1 };
        case 2: return { GL_RG, 2 };
        case 3: return { GL_RGB, 1 };
        case 4: return { GL_RGBA, 4 };

        default: LOG_WARN("Unsupported texture format (channels = {0})", channels);
            return { GL_RGBA, 4 };
        }
    }
    GLenum SelectFilter(Filter filter, MipmapMode mipmap)
    {
        switch (filter)
        {
        case Filter::Linear:
            switch (mipmap)
            {
            case MipmapMode::None: return GL_LINEAR;
            case MipmapMode::Nearest: return GL_LINEAR_MIPMAP_NEAREST;
            case MipmapMode::Anisotropic:
            default: return GL_LINEAR_MIPMAP_LINEAR;
            }
        default:
            switch (mipmap)
            {
            case MipmapMode::None: return GL_NEAREST;
            case MipmapMode::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
            case MipmapMode::Anisotropic:
            default: return GL_NEAREST_MIPMAP_LINEAR;
            }
        }
    }
    GLenum SelectWrapMode(WrapMode wrap)
    {
        switch (wrap)
        {
        case WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        case WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case WrapMode::Mirror: return GL_MIRRORED_REPEAT;
        default: return GL_REPEAT;
        }
    }

    void Texture::PremultiplyAlpha(unsigned char *data)
    {
        for (int i = 0; i < m_Width * m_Height * 4; i += 4)
        {
            float alpha = data[i + 3] / 255.0f;
            data[i + 0] = (unsigned char)(data[i + 0] * alpha);
            data[i + 1] = (unsigned char)(data[i + 1] * alpha);
            data[i + 2] = (unsigned char)(data[i + 2] * alpha);
        }
    }

    void Texture::LoadTexture(unsigned char *data)
    {
        auto [format, alignment] = SelectFormat(m_Channels);

        if (format == GL_RGBA)
            PremultiplyAlpha(data);

        glBindTexture(GL_TEXTURE_2D, m_ContextID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SelectFilter(m_Props.filter, MipmapMode::None)); // Mag filter will never use mipmaps
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SelectFilter(m_Props.filter, m_Props.mipmap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SelectWrapMode(m_Props.wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, SelectWrapMode(m_Props.wrap));

        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

        if (m_Props.mipmap == MipmapMode::Anisotropic)
        {
            if (GLAD_GL_EXT_texture_filter_anisotropic)
            {
                float anisotropy_max = 0;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy_max);
                float amount = glm::max(4.0f, anisotropy_max);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
            }
            else
                LOG_WARN("GLAD_GL_EXT_texture_filter_anisotropic not supported! Using default mipmapping instead.");
        }

        if (m_Props.mipmap != MipmapMode::None)
            glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Ref<Texture> Texture::LoadFromFile(
        const std::string &path, 
        const TextureProps &props)
    {
        stbi_set_flip_vertically_on_load(true);

        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        Ref<Texture> texture = CreateRef<Texture>(data, width, height, channels, props);
        stbi_image_free(data);

        return texture;
    }

    Ref<Texture> Texture::LoadFromData(
        unsigned char *data, 
        int width, int height, int channels, 
        const TextureProps &props)
    {
        return CreateRef<Texture>(data, width, height, channels, props);
    }

} }


