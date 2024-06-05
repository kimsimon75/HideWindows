#include <Windows.h>
#include "후킹.h"
#include <thread>

//아이콘 출처 : <a href="https://www.flaticon.com/kr/free-icons/-" title="양자 컴퓨팅 아이콘">양자 컴퓨팅 아이콘 제작자: xnimrodx - Flaticon</a>

using namespace std;


NOTIFYICONDATA nid;
template <class T>


void SafeRelease(T** ppT) {

	if (*ppT) {

		(*ppT)->Release();

		*ppT = NULL;

	}

}

void CheckPID()
{
	while (1)
	{
		CoInitialize(NULL);



		IMMDeviceEnumerator* pEnumerator = NULL;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnumerator));



		IMMDevice* pDevice = NULL;

		pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);



		IAudioSessionManager2* pSessionManager = NULL;

		pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);



		IAudioSessionEnumerator* pSessionList = NULL;

		pSessionManager->GetSessionEnumerator(&pSessionList);



		int sessionCount = -1;

		pSessionList->GetCount(&sessionCount);



		for (int i = 0; i < sessionCount; i++) {

			IAudioSessionControl* pSessionControl = NULL;

			pSessionList->GetSession(i, &pSessionControl);



			IAudioSessionControl2* pSessionControl2 = NULL;

			HRESULT hr = pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);

			if (SUCCEEDED(hr)) {

				DWORD processId = -1;

				pSessionControl2->GetProcessId(&processId);

				std::wstring processName = GetProcessNameByPID(processId);



				// 여기서 할 일 하면 됨

				std::wcout << L"PID: " << processId << ", Name: " << processName << std::endl;

				if (m.find(processName) == m.end())
				{
					m.insert(pair<wstring, DWORD>(processName, processId));
				}

				pSessionControl2->Release();

			}

			pSessionControl->Release();

		}



		SafeRelease(&pSessionList);

		SafeRelease(&pSessionManager);

		SafeRelease(&pDevice);

		SafeRelease(&pEnumerator);



		CoUninitialize();
		Sleep(100);
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_USER + 1: // 트레이 아이콘 메시지 처리
		switch (lParam) {
		case WM_RBUTTONUP: // 오른쪽 마우스 버튼 클릭

			POINT p;
			GetCursorPos(&p);
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, 1, L"닫기");
			SetForegroundWindow(hwnd);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, p.x, p.y, 0, hwnd, NULL);
			PostMessage(hwnd, WM_NULL, 0, 0); // 메뉴가 닫히도록 함
			DestroyMenu(hMenu);
			break;

		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			cout << "hello"<< endl;
			Shell_NotifyIcon(NIM_DELETE, &nid); // 트레이 아이콘 제거
			DestroyWindow(hwnd);
			UnhookWindowsHookEx(keyboardHook);
			UnhookWindowsHookEx(mouseHook);
			PostMessage(GetConsoleWindow(), WM_CLOSE, NULL, NULL);
			break;
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

int main()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// 윈도우 클래스 등록
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"TrayWindowClass";

	RegisterClass(&wc);

	// 윈도우 생성
	HWND hwnd = CreateWindow(
		L"TrayWindowClass", L"Tray Example",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
		NULL, NULL, hInstance, NULL);

	// 트레이 아이콘 설정
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER + 1;
	nid.hIcon = (HICON)LoadImage(hInstance, L"south_korea_19280.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (nid.hIcon == NULL)
	{
		cout << "failed" << endl;
		return 1;
	}
	wcscpy_s(nid.szTip, L"Tray Example");

	Shell_NotifyIcon(NIM_ADD, &nid);

	thread t1(CheckPID);

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);

	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);
	if (mouseHook == NULL) {
		std::cerr << "Failed to install mouse hook!" << std::endl;
		return 1;
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(keyboardHook);
	UnhookWindowsHookEx(mouseHook);
}
  


