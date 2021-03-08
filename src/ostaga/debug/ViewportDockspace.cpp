// Precompiled
#include "ospch.h"
//

#include "ViewportDockspace.h"

#include <Application.h>
#include <Debug.h>
#include <imgui_internal.h>

namespace Ostaga {

	using namespace Graphics;

	ViewportDockspace::ViewportDockspace(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
		FrameBufferProps props;
		props.width = width;
		props.height = height;
		m_FrameBuffer = FrameBuffer::Create(props);
	}

	void ViewportDockspace::Resize(unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;
		m_FrameBuffer->Resize(width, height);
	}

	void ViewportDockspace::OnBegin()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, { 0.0f, 0.0f }, dockspace_flags);

		// Retrieve the current window position (relative to desktop) of the dockspace.
		m_ViewportContentProps.x = ImGui::GetWindowPos().x;
		m_ViewportContentProps.y = ImGui::GetWindowPos().y;
	}

	void ViewportDockspace::OnEnd()
	{
		ImGui::End();
	}

	void ViewportDockspace::OnEvent(Event &e)
	{
		// TODO: Handle mouse events such that they only occur when the content area is focused.
		e.Handled = !m_Focused;
	}

	void ViewportDockspace::ShowViewportWindow()
	{
		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Always);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		m_Focused = ImGui::IsWindowFocused();

		size_t textureID = m_FrameBuffer->GetColorAttachmentID();
		ImVec2 drawRegion = ImGui::GetContentRegionAvail();
		drawRegion.x = drawRegion.y * (float) m_Width / (float) m_Height;

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - drawRegion.x) * 0.5f);

		// TODO:: Ideally use a child window containing the image (make mouse mapping/event handling easier).
		m_ViewportContentProps.width = drawRegion.x;
		m_ViewportContentProps.height = drawRegion.y;
		m_ViewportContentProps.x = (double) ImGui::GetWindowPos().x - m_ViewportContentProps.x + ImGui::GetCursorPosX();
		m_ViewportContentProps.y = (double) ImGui::GetWindowPos().y - m_ViewportContentProps.y + ImGui::GetCursorPosY();

		ImGui::Image((void*) textureID, drawRegion, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
}