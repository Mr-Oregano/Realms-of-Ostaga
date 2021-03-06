// Precompiled
#include "ospch.h"
//

#include <Editor.h>

#include <Application.h>
#include <Renderer.h>
#include <Debug.h>

#include <string>

namespace Ostaga {
	
	void Editor::OnStartup()
	{
	}

	void Editor::OnShutdown()
	{
	}

	void Editor::OnUpdate(TimeStep ts)
	{
	}

	void Editor::OnRender()
	{
	}

	void Editor::OnEvent(Event &e)
	{
	}

	#define SAMPLES 100
	static float frameTime_plot[SAMPLES] = {};
	static int current = 0;
	
	float Plotter(void *data, int idx)
	{
		return ((float*) data)[(current + idx) % SAMPLES];
	}

	void Analytics()
	{
		ApplicationAnalytics appStats = Application::Get().GetAnalytics();
		frameTime_plot[current] = (float)appStats.frameTime;
		current = (++current) % SAMPLES;

		ImGui::Begin("Analytics");
		if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Current FPS: %f", appStats.framesPerSecond);
			ImGui::Text("Current Frame time: %f", appStats.frameTime);
			ImGui::PushItemWidth(ImGui::GetWindowWidth());
			ImGui::PlotLines("", &Plotter, frameTime_plot, SAMPLES, 0, "Frame time", FLT_MAX, FLT_MAX, { 0, 150 });
		}

		using namespace Graphics;
		RendererAnalytics rendererStats = Renderer::GetAnalytics();

		if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Submissions: %d", rendererStats.submissions);
			ImGui::Text("Draw calls made: %d", rendererStats.drawCalls);

			ImGui::Text("Current Texture Atlas:");
			auto &atlas = rendererStats.currentAtlas->GetAtlas();
			float aspect = (float)atlas.GetWidth() / atlas.GetHeight();
			ImVec2 size = { ImGui::GetWindowSize().x * 0.8f, ImGui::GetWindowSize().x * 0.8f * aspect };
			void* contextID = (void*) ((long long) atlas.GetContextID());

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) * 0.5f);
			ImGui::Image(contextID, size, { 1, 1 }, { 0, 0 });
		}

		ImGui::End();
	}

	void Profiling()
	{
		static bool profiling = false;
		static char name[256] = "Ostaga-Runtime";
		
		ImGui::Begin("Profiling");
		ImGui::InputText("Output Name", name, sizeof(name));
		
		if (profiling)
		{
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.929f, 0.450f, 0.450f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.933f, 0.364f, 0.364f, 1.0f });
			if (ImGui::Button("End Session"))
			{
				Profiler::EndSession();
				LOG_INFO("Terminating profiling session: {0}", name);
				profiling = false;
			}
			ImGui::PopStyleColor(2);
		}
		else
		{
			if (ImGui::Button("Record Session"))
			{
				std::stringstream ss;
				ss << name << ".json";

				Profiler::BeginSession(name, ss.str());
				LOG_INFO("Start profiling session: {0}", name);
				profiling = true;
			}
		}

		ImGui::End();
	}

	void Editor::OnGui()
	{
		Analytics();
		Profiling();
	}
}


