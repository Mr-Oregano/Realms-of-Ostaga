#pragma once

#include <vector>

#include <Texture.h>

#include <Maths.h>
#include <Ref.h>

namespace Ostaga { namespace Graphics {

	struct TextureAtlasEntry
	{
		float x;
		float y;
		float width;
		float height;
	};

	// TODO: Manage texture coordinates bias
	class TextureAtlas
	{
	public:
		TextureAtlas(const Ref<Texture>& atlas);

		TextureAtlasEntry& AddEntry(const glm::vec4& rect);

		inline void Bind(GLuint unit = 0) const { m_Atlas->Bind(unit); }

		inline int GetWidth() const { return m_Atlas->GetWidth(); }
		inline int GetHeight() const { return m_Atlas->GetHeight(); }

		inline const Texture& GetAtlas() const { return *m_Atlas; }
		inline Texture& GetAtlas() { return *m_Atlas; }
		
		inline TextureAtlasEntry& GetEntry(int idx) { return m_Entries[idx]; }
		inline size_t GetEntryCount() const { return m_Entries.size(); }

	private:
		Ref<Texture> m_Atlas;
		std::vector<TextureAtlasEntry> m_Entries;

	public:
		static Ref<TextureAtlas> Create(
			const std::string& path, 
			const TextureProps &props = { Filter::Linear, WrapMode::ClampToEdge, MipmapMode::Linear });

	};
} }