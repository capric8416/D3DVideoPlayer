#include "D3DRender.h"


extern "C"
{
#include <libavcodec/avcodec.h>
}



D3DPlayer::D3DRender::D3DRender(HWND hWnd, int VideoWidth, int VideoHeight)
	: m_hWnd(hWnd)

	, m_VideoWidth(VideoWidth)
	, m_VideoHeight(VideoHeight)

	, m_ViewWidth(0)
	, m_ViewHeight(0)
	, m_OldViewWidth(0)
	, m_OldViewHeight(0)

	, m_NeedResize(false)
{
	UpdateClientRect();
}


D3DPlayer::D3DRender::~D3DRender()
{

}


HWND D3DPlayer::D3DRender::GetHWND()
{
	return m_hWnd;
}


void D3DPlayer::D3DRender::Resize(int width, int height)
{
	if (!m_NeedResize) {
		m_NeedResize = true;

		m_OldViewWidth = m_ViewWidth;
		m_OldViewHeight = m_ViewHeight;
	}

	m_ViewWidth = width;                                                                 
	m_ViewHeight = height;
}


void D3DPlayer::D3DRender::UpdateClientRect()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);

	m_OldViewWidth = m_ViewWidth;
	m_OldViewHeight = m_ViewHeight;

	m_ViewWidth = rect.right;
	m_ViewHeight = rect.bottom;

	if (m_OldViewWidth != m_ViewWidth || m_OldViewHeight != m_ViewHeight)
	{
		m_NeedResize = true;
	}
}


double D3DPlayer::D3DRender::GetVideoRatio()
{
	return (double)m_VideoHeight / m_VideoWidth;
}


double D3DPlayer::D3DRender::GetViewRatio()
{
	return (double)m_ViewHeight / m_ViewWidth;
}
