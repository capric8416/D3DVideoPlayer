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
		// 解析命令行参数
		parse();

		// 读取264内存
		load();

		// 创建播放器
		create(hInstance);

		// 启动定时器线程
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

		// 主消息循环:
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 等待启动定时器线程结束
		playing = false;
		timer.join();
	}


private:
	// 命令行解析
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

		// 测试模式，只启动一个1080p 30fps播放器
		if (m_DebugMode) {
			m_Ways = 1;
			m_Fps = 30;
			m_Count = 1;
			m_Width = 1920;
			m_Height = 1080;
		}
		else {  // 切分屏幕 - 决定启动 n x n 个播放器
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

		// 将264文件载入内存
		D3DPlayer::D3DFileMemory::Load(m_Path);
	}

	void unload()
	{
		// 清理264内存
		D3DPlayer::D3DFileMemory::Unload();
	}

	// 创建播放器
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
	bool m_DebugMode;  // 测试模式

	int m_Ways;  // 几路播放器
	int m_Fps;  // 帧率

	int m_Count;  // 横向/纵向平铺几路播放器
	int m_Width;  // 单路播放器宽
	int m_Height;  // 单路播放器高

	wchar_t m_Path[MAX_PATH];  // 视频文件路径

	D3DPlayer::D3DPlayerCommand m_Player;  // 视频播放管理器

	HWND m_hWnd;  // 最后一窗口句柄
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
