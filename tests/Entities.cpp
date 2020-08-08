
// Precompiled
#include "ospch.h"
//

#include "Entities.h"

namespace Ostaga {

	Ref<Audio::IAudio> monster_soundfx;

	Ref<Graphics::TextureAtlas> atlas;
	Graphics::TextureAtlasEntry forest_tile;
	Graphics::TextureAtlasEntry grass1;
	Graphics::TextureAtlasEntry grass2;
	Graphics::TextureAtlasEntry grass3;
	Graphics::TextureAtlasEntry grass4;
	Graphics::TextureAtlasEntry boulder;
	Graphics::TextureAtlasEntry oaktree;
	Graphics::TextureAtlasEntry pinetree;
	Graphics::TextureAtlasEntry player;
	Graphics::TextureAtlasEntry white;

	void LoadAssets()
	{
		monster_soundfx = Audio::IAudio::LoadFromFile("res/sounds/mnstr2.wav");

		Random::SetSeed(1);
		atlas = Graphics::TextureAtlas::Create("res/textures/atlas.png");

		forest_tile = atlas->AddEntry({ 1, 1, 16, 16 });
		oaktree = atlas->AddEntry({ 32, 0, 64, 64 });
		pinetree = atlas->AddEntry({ 96, 0, 64, 64 });
		grass1 = atlas->AddEntry({ 160, 0, 16, 16 });
		grass2 = atlas->AddEntry({ 160, 16, 16, 16 });
		grass3 = atlas->AddEntry({ 176, 0, 16, 16 });
		grass4 = atlas->AddEntry({ 176, 16, 16, 16 });
		boulder = atlas->AddEntry({ 192, 0, 64, 64 });
		player = atlas->AddEntry({ 0, 64, 32, 48 });
		white = atlas->AddEntry({ atlas->GetWidth() - 1, atlas->GetHeight() - 1, 1, 1 });

		Graphics::Renderer::SetTextureAtlas(atlas);
	}
}