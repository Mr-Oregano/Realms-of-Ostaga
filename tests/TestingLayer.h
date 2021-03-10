#pragma once

#include <Ostaga.h>

#include <queue>
#include <vector>

#include <Layer.h>

#include <IAudio.h>
#include <IAudioPlayer.h>

#include <Ref.h>

namespace Ostaga {

	class TestingLayer : public Layer
	{
	public:
		Ref<Audio::IAudio> temptation_track;
		Ref<Audio::IAudioPlayer> musicPlayer;

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStartup() override;
		virtual void OnShutdown() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Event &e) override;

		virtual void OnGui() override;
	};
}
