#pragma once

#include <Layer.h>
#include <Ref.h>

#include <FrameBuffer.h>

namespace Ostaga {

	struct ViewportContentProps
	{
		double width;
		double height;
		double x;
		double y;
	};

	class ViewportDockspace : public Ostaga::Layer
	{
	public:
		ViewportDockspace(unsigned int width, unsigned int height);

		inline void BeginCapture() const { m_FrameBuffer->Bind(); }
		inline void EndCapture() const { m_FrameBuffer->Unbind(); }
		inline bool IsViewportFocused() const { return m_Focused; }
		inline unsigned long TextureID() const { return m_FrameBuffer->GetColorAttachmentID(); }

		inline const ViewportContentProps& GetViewportContentProps() const { return m_ViewportContentProps; }

		void Resize(unsigned int width, unsigned int height);
		
		void OnBegin() override;
		void OnEnd() override;
		void OnEvent(Event &e) override;

		void ShowViewportWindow();

	private:
		Ref<Graphics::FrameBuffer> m_FrameBuffer;
		bool m_Focused = false;
		unsigned int m_Width;
		unsigned int m_Height;

		ViewportContentProps m_ViewportContentProps;
	};
}
