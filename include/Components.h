#pragma once

#include <entt/entt.hpp>

#include <string>

#include <Maths.h>
#include <TextureAtlas.h>
#include <Geometry.h>

#include <Ref.h>

namespace Ostaga {

	struct CName
	{
		std::string name;
	};

	struct CTransform
	{
		glm::vec2 pos;
		glm::vec2 size;
		float rotation = 0;
	};

	struct CRenderable
	{
		Graphics::TextureAtlasEntry texture;
		glm::vec4 tint = glm::vec4{ 1.0f };
	};
}
