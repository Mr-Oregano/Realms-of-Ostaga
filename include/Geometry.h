#pragma once

#include <Ostaga.h>
#include <Ref.h>

#include <Texture.h>

namespace Ostaga { namespace Graphics {

	class Geometry
	{
	public:
		Geometry() = default;
		virtual ~Geometry() = default;

		virtual void Bind() = 0;
		virtual void Draw() = 0;
	};

} }