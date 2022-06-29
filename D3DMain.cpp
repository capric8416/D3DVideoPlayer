#include "D3DFileMemory.h"
#include "D3DCommand.h"

#include <windowsx.h>
#include <VersionHelpers.h>

extern "C" {
#include <libavcodec/avcodec.h>
}



//const bool IS_WINDOWS_7 = false;
const bool IS_WINDOWS_7 = IsWindows7OrGreater() && !IsWindows8OrGreater();



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		auto pPlayer = (D3DPlayer::D3DPlayerCommand *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pPlayer) {
			pPlayer->Resize(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}

		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}



wchar_t * ConvertToWideChars(const char *src)
{
	wchar_t *dst;
	int length = MultiByteToWideChar(CP_ACP, 0, src, (int)strlen(src), NULL, 0);
	dst = new wchar_t[length + 1];
	MultiByteToWideChar(CP_ACP, 0, src, (int)strlen(src), dst, length);
	dst[length] = '\0';
	return dst;
}



class ResourcesManager
{
public:
	ResourcesManager()
		: m_DebugMode(false)

		, m_Ways(1)
		, m_Fps(60)

		, m_Count(1)
		, m_Width(1920)
		, m_Height(1080)

		, m_hWnd(nullptr)

		, m_Player(IS_WINDOWS_7)
	{
		memset(m_Path, 0, sizeof(wchar_t) * MAX_PATH);
	}

	~ResourcesManager()
	{
		unload();
	}

	void Run(HINSTANCE hInstance)
	{
		// ���������в���
		parse();

		// ��ȡ264�ڴ�
		load();

		// ����������
		create(hInstance);

		// ������ʱ���߳�
		bool playing = true;
		std::thread timer([&]() {
			LONGLONG interval = 1000 / m_Fps;
			if (IS_WINDOWS_7) {
				interval /= 3;
			}

			while (playing) {
				m_Player.Walk([](D3DPlayer::D3DPlayerCommand *pPlayerCmd, D3DPlayer::D3DPlayerResource *pRes) {
					AVFrame *pFrame = pPlayerCmd->AcquireFrame(pRes);
					if (pFrame) {
						pPlayerCmd->Render(pRes, pFrame);
						pPlayerCmd->ReleaseFrame(pRes);
					}
				});

				SleepMilliseconds(interval);
			}
		});

		// ����Ϣѭ��:
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �ȴ�������ʱ���߳̽���
		playing = false;
		timer.join();
	}


private:
	// �����н���
	void parse()
	{
		for (UINT i = 1; i < static_cast<UINT>(__argc); ++i)
		{
			if (strcmp(__argv[i], "--path") == 0)
			{
				if (++i >= static_cast<UINT>(__argc))
				{
					return;
				}

				wchar_t *path = ConvertToWideChars(__argv[i]);
				wcscpy_s(m_Path, path);
				delete[] path;
			}
			else if (strcmp(__argv[i], "--debug") == 0)
			{
				if (++i >= static_cast<UINT>(__argc))
				{
					return;
				}

				m_DebugMode = strcmp(__argv[i], "true") == 0;
			}
			else if (strcmp(__argv[i], "--ways") == 0)
			{
				if (++i >= static_cast<UINT>(__argc))
				{
					return;
				}

				m_Ways = atoi(__argv[i]);

				if (m_Ways > 36) {
					m_Ways = 36;
				}
				else if (m_Ways < 1) {
					m_Ways = 1;
				}
			}
			else if (strcmp(__argv[i], "--fps") == 0)
			{
				if (++i >= static_cast<UINT>(__argc))
				{
					return;
				}

				m_Fps = atoi(__argv[i]);

				if (m_Fps > 60) {
					m_Fps = 60;
				}
				else if (m_Fps < 10) {
					m_Fps = 10;
				}
			}
		}

		// ����ģʽ��ֻ����һ��1080p 30fps������
		if (m_DebugMode) {
			m_Ways = 1;
			m_Fps = 30;
			m_Count = 1;
			m_Width = 1920;
			m_Height = 1080;
		}
		else {  // �з���Ļ - �������� n x n ��������
			m_Count = (int)sqrt(m_Ways);
			m_Width = GetSystemMetrics(SM_CXSCREEN) / m_Count;
			m_Height = GetSystemMetrics(SM_CYSCREEN) / m_Count;

		}
	}

	void load()
	{
		if (wcslen(m_Path) == 0) {
			swprintf_s(m_Path, MAX_PATH, L"D:/risley/Media Files/H.264 Videos/Overwatch_Alive_Short_%dx%d.264", m_Width, m_Height);
		}
		TRACE("playing %d video files '%s' at same time", m_Ways, m_Path);

		// ��264�ļ������ڴ�
		D3DPlayer::D3DFileMemory::Load(m_Path);
	}

	void unload()
	{
		// ����264�ڴ�
		D3DPlayer::D3DFileMemory::Unload();
	}

	// ����������
	void create(HINSTANCE hInstance)
	{
		wchar_t name[MAX_PATH >> 2];
		for (int i = 0; i < m_Count; i++) {
			for (int j = 0; j < m_Count; j++) {
				int index = i * m_Count + j;
				swprintf_s(name, 64, L"player_%d", index);

				WNDCLASSW wndClass = {};
				wndClass.hInstance = hInstance;
				wndClass.lpszClassName = name;
				wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
				wndClass.lpfnWndProc = WndProc;
				RegisterClass(&wndClass);

				m_hWnd = CreateWindow(name, name, WS_POPUP, i * m_Width, j * m_Height, m_Width, m_Height, NULL, NULL, hInstance, NULL);

				m_Player.Create(m_hWnd);

				SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)&m_Player);

				ShowWindow(m_hWnd, SW_SHOW);
			}
		}
	}



private:
	bool m_DebugMode;  // ����ģʽ

	int m_Ways;  // ��·������
	int m_Fps;  // ֡��

	int m_Count;  // ����/����ƽ�̼�·������
	int m_Width;  // ��·��������
	int m_Height;  // ��·��������

	wchar_t m_Path[MAX_PATH];  // ��Ƶ�ļ�·��

	D3DPlayer::D3DPlayerCommand m_Player;  // ��Ƶ���Ź�����

	HWND m_hWnd;  // ���һ���ھ��
};



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	CoInitializeEx(NULL, COINIT::COINIT_MULTITHREADED);

	SetProcessDPIAware();

	ResourcesManager manager;
	manager.Run(hInstance);

	CoUninitialize();

	return 0;
}
