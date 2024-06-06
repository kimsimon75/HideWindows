#pragma once
#include <Windows.h>>
#include "숨기기, 보이기.h"

HHOOK keyboardHook;
HHOOK mouseHook;
NOTIFYICONDATA nid;

LRESULT __stdcall MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{

		MSLLHOOKSTRUCT* hookStruct = (MSLLHOOKSTRUCT*)lParam;

			if (wParam == WM_XBUTTONDOWN)
			{
				
				return 1;
			}
			if (wParam == WM_XBUTTONUP)
			{
				if (hookStruct->mouseData == 0x20000)
				{
					HideWindow();
				}
				else if (hookStruct->mouseData == 0x10000)
				{
					ShowWindow();
				}
				return 1;
			}
		
		
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam); // 이전으로 돌아가도록 변경
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;

		if (wParam == WM_KEYDOWN)
		{
			// F1 키를 누른 경우
			if (kbStruct->vkCode == VK_F1)
			{
				ShowWindowAll();
				return 1;
			}
			else if (kbStruct->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000) 
			{
				ShowWindow(GetConsoleWindow(), SW_SHOW);
				ShowWindowAll();
				Shell_NotifyIcon(NIM_DELETE, &nid);
				UnhookWindowsHookEx(keyboardHook);
				UnhookWindowsHookEx(mouseHook);
				PostMessage(GetConsoleWindow(), WM_CLOSE, NULL, NULL);
				return 1;
			}
			else if (kbStruct->vkCode == VK_F2)
			{
				HideWindow();
				return 1;
			}
		}
		else if (wParam == WM_KEYUP && kbStruct->vkCode == VK_F1)
		{
			return 1;
		}
	}
	// 후킹된 이벤트 처리를 계속합니다.
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}
