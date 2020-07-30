#pragma once

#include <string>

#include <Ref.h>

#include <glad/glad.h>

namespace Ostaga { namespace Graphics {

	enum class Filter
	{
		Nearest = 0,
		Linear
	};
	enum class WrapMode
	{
		ClampToEdge = 0,
		ClampToBorder,
		Repeat,
		Mirror
	};
	enum class MipmapMode
	{
		None = 0,
		Nearest,
		Linear,
		Anisotropic
	};

	struct TextureProps
	{
		Filter filter;
		WrapMode wrap;
		MipmapMode mipmap;
	};

	class Texture
	{
	public:
		Texture(unsigned char *data, int width, int height, int channels, const TextureProps &props);
		~Texture();

		inline GLuint GetContextID() const { return m_ContextID; }
		inline void Bind(GLuint unit = 0) const { glBindTextureUnit(unit, m_ContextID); }
		inline void Unbind(GLuint unit = 0) const { glBindTextureUnit(unit, 0); }

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline int GetChannels() const { return m_Channels; }

	private:
		GLuint m_ContextID = 0;

		int m_Width = 0;
		int m_Height = 0;
		int m_Channels = 0;

		TextureProps m_Props;

	private:
		void PremultiplyAlpha(unsigned char *data);
		void LoadTexture(unsigned char *data);

	public:
		static Ref<Texture> LoadFromFile(
			const std::string &path,
			const TextureProps &props = { Filter::Linear, WrapMode::ClampToEdge, MipmapMode::Linear });

		static Ref<Texture> LoadFromData(unsigned char *data, int width, int height, int channels,
			const TextureProps &props = { Filter::Linear, WrapMode::ClampToEdge, MipmapMode::Linear });
	};

} }