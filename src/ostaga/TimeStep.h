#pragma once

namespace Ostaga {

	struct TimeStep
	{
	private:
		float deltaTime = 0.0f;

	public:
		TimeStep(float deltaTime) : deltaTime(deltaTime) {}

		inline operator float() const { return deltaTime; }
		inline float GetDeltaTime() const { return deltaTime; }

	};

}