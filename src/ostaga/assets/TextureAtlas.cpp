
// Precompiled
#include "ospch.h"
//

#include "TextureAtlas.h"

namespace Ostaga { namespace Assets {
	
	TextureAtlas::TextureAtlas(const Ref<Texture>& atlas)
		: m_Atlas(atlas)
	{
	}

	TextureAtlasEntry& TextureAtlas::AddEntry(const glm::vec4 &rect)
	{
		TextureAtlasEntry entry;
		entry.x = rect.x / GetWidth();
		entry.y = 1.0f - rect.y / GetHeight();
		entry.width = rect.z / GetWidth();
		entry.height = rect.w / GetHeight();

		m_Entries.push_back(entry);
		return *(m_Entries.end() - 1);
	}

	Ref<TextureAtlas> TextureAtlas::Create(const std::string &path, const TextureProps &props)
	{
		return CreateRef<TextureAtlas>(Texture::LoadFromFile(path, props));
	}
} }

