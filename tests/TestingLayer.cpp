
// Precompiled
#include "ospch.h"
//

#include "TestingLayer.h"

#include <Input.h>
#include <KeyEvent.h>
#include <Camera.h>

#include <Renderer.h>
#include <Application.h>
#include <World.h>

#include <Debug.h>

namespace Ostaga {

	using namespace Graphics;
	using namespace Audio;

	Scope<World> world;

	void TestingLayer::OnStartup()
	{
		world = CreateScope<World>(20, 20, 0);

		temptation_track = IAudio::LoadFromFile("res/music/cyberbyte.wav");
		musicPlayer = IAudioPlayer::Create(temptation_track, { AudioMode::Loop });
		musicPlayer->SetGain(0.25f);
		OSTAGA_IF_DEBUG(, musicPlayer->Play());
	}

	void TestingLayer::OnShutdown()
	{
	}

	void TestingLayer::OnUpdate(TimeStep ts)
	{
		PROFILE_FUNCTION();
		world->OnUpdate(ts);
	}

	void TestingLayer::OnRender()
	{
		PROFILE_FUNCTION();
		world->OnRender();
	}

	void TestingLayer::OnEvent(Event &e)
	{
		world->OnEvent(e);
	}

	void TestingLayer::OnGui()
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Testing Layer");

		if (ImGui::CollapsingHeader("Music Player", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Play"))
				musicPlayer->Play();

			ImGui::SameLine();
			if (ImGui::Button("Stop"))
				musicPlayer->Stop();

			ImGui::SameLine();
			if (ImGui::Button("Pause"))
				musicPlayer->Pause();
		}

		ImGui::End();

		world->OnGui();
	}

}