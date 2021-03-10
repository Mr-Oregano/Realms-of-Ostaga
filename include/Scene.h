#pragma once

#include <Components.h>
#include <TextureAtlas.h>
#include <WorldChunk.h>
#include <Camera.h>

namespace Ostaga {

	class Scene
	{
	public:
		Scene (OrthoCamera camera) : camera(camera) {};

	public:
		Ref<Graphics::TextureAtlas> atlas;
		
		std::vector<WorldChunk> chunks;
		entt::registry entities;

		OrthoCamera camera;
	};
}
