#include <Windows.h>
#include "후킹.h"
#include <thread>

//아이콘 출처 : <a href="https://www.flaticon.com/kr/free-icons/-" title="양자 컴퓨팅 아이콘">양자 컴퓨팅 아이콘 제작자: xnimrodx - Flaticon</a>

using namespace std;

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

int main()
{
	thread t1(CheckPID);

	HINSTANCE hInstance = GetModuleHandle(NULL);
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
	}
	UnhookWindowsHookEx(keyboardHook);
	UnhookWindowsHookEx(mouseHook);
}
  


