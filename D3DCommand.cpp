#include "D3DCommand.h"
#include "D3DDecoder.h"
#include "D3D9Render.h"
#include "D3D11Render.h"

extern "C"
{
#include <libavutil/avutil.h>
}



D3DPlayer::D3DPlayerCommand::D3DPlayerCommand(bool UseD3D9)
	: m_No(0)
	, m_UseD3D9(UseD3D9)
{
}


D3DPlayer::D3DPlayerCommand::~D3DPlayerCommand()
{
	std::lock_guard<std::mutex> locker(m_mutexOperation);

	for (auto iter = m_listResources.begin(); iter != m_listResources.end(); iter++) {
		D3DPlayerResource *p = *iter;

		SafeDelete(p->Decoder);

		SafeDelete(p->Render);

		PostMessage(p->Wnd, WM_QUIT, 0, 0);
	}

	m_listResources.clear();
}


D3DPlayer::D3DPlayerResource * D3DPlayer::D3DPlayerCommand::Create(HWND hWnd)
{
	std::lock_guard<std::mutex> locker(m_mutexOperation);

	m_No++;

	D3DDecoder *pDecoder = new D3DDecoder(m_UseD3D9);

	D3DRender *pRender;
	if (m_UseD3D9) {
		pRender = new D3D9Render(hWnd, pDecoder->GetWidth(), pDecoder->GetHeight());
	}
	else {
		pRender = new D3D11Render(hWnd, pDecoder->GetWidth(), pDecoder->GetHeight());
	}

	D3DPlayerResource *pRes = new D3DPlayerResource(m_No, hWnd, pDecoder, pRender);

	m_listResources.push_back(pRes);

	return pRes;
}


void D3DPlayer::D3DPlayerCommand::Destroy(D3DPlayerResource * pRes)
{
	std::lock_guard<std::mutex> locker(m_mutexOperation);

	for (auto iter = m_listResources.begin(); iter != m_listResources.end(); iter++) {
		D3DPlayerResource *p = *iter;

		if (p == pRes)
		{
			SafeDelete(pRes->Decoder);

			SafeDelete(pRes->Render);

			m_listResources.erase(iter++);

			break;
		}
		else
		{
			++iter;
		}
	}
}


int D3DPlayer::D3DPlayerCommand::GetWidth(D3DPlayerResource *pRes)
{
	if (pRes == nullptr) {
		return 0;
	}

	return pRes->Decoder->GetWidth();
}


int D3DPlayer::D3DPlayerCommand::GetHeight(D3DPlayerResource *pRes)
{
	if (pRes == nullptr) {
		return 0;
	}

	return pRes->Decoder->GetHeight();
}


AVFrame * D3DPlayer::D3DPlayerCommand::AcquireFrame(D3DPlayerResource * pRes)
{
	if (pRes == nullptr) {
		return nullptr;
	}

	return pRes->Decoder->AcquireFrame();
}


void D3DPlayer::D3DPlayerCommand::ReleaseFrame(D3DPlayerResource * pRes)
{
	if (pRes == nullptr) {
		return;
	}

	pRes->Decoder->ReleaseFrame();
}


void D3DPlayer::D3DPlayerCommand::Render(D3DPlayerResource * pRes, AVFrame *pFrame)
{
	if (pRes == nullptr) {
		return;
	}

	if (m_UseD3D9) {
		D3D9Render *pRender = (D3D9Render *)pRes->Render;
		pRender->Draw(pFrame);
		pRender->Present();
	}
	else {
		D3D11Render *pRender = (D3D11Render *)pRes->Render;
		pRender->Draw(pFrame);
		pRender->Present();
	}
}


void D3DPlayer::D3DPlayerCommand::Resize(HWND hWnd, int width, int height)
{
	for (auto iter = m_listResources.begin(); iter != m_listResources.end(); iter++) {
		D3DPlayerResource *p = *iter;
		if (p->Wnd == hWnd) {
			p->Render->Resize(width, height);
			break;
		}
	}
}


void D3DPlayer::D3DPlayerCommand::Walk(WalkProc proc)
{
	for (auto iter = m_listResources.begin(); iter != m_listResources.end(); iter++) {
		proc(this, *iter);
	}
}
