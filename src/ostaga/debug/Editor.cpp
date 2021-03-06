
// Precompiled
#include "ospch.h"
//

#include <Editor.h>

#include <Debug.h>
#include <Application.h>

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

	void Editor::OnGui()
	{
		ApplicationAnalytics stats = Application::Get().GetAnalytics();
		frameTime_plot[current] = (float)stats.frameTime;
		current = (++current) % SAMPLES;

		ImGui::Begin("Analytics");
		if (!ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::End();
			return;
		}
		
		ImGui::Text("Current FPS: %f", stats.framesPerSecond);
		ImGui::Text("Current Frame time: %f", stats.frameTime);
		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::PlotLines("", &Plotter, frameTime_plot, SAMPLES, 0, 0, FLT_MAX, FLT_MAX, ImVec2{0, 150});
		
		ImGui::End();
	}
}


