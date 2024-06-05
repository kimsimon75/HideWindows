#pragma once
#include <Windows.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <iostream>



bool SetApplicationVolume(DWORD processId, float volumeLevel) {
	HRESULT hr = S_OK;
	IMMDeviceEnumerator* enumerator = nullptr;
	IMMDevice* device = nullptr;
	IAudioSessionManager2* sessionManager = nullptr;
	IAudioSessionEnumerator* sessionEnumerator = nullptr;

	// Initialize COM library
	hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		std::cerr << "Failed to initialize COM library" << std::endl;
		return false;
	}

	// Create multimedia device enumerator
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));
	if (FAILED(hr)) {
		std::cerr << "Failed to create device enumerator" << std::endl;
		CoUninitialize(); // Uninitialize COM library
		return false;
	}

	// Get default audio endpoint device
	hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	if (FAILED(hr)) {
		std::cerr << "Failed to get default audio endpoint device" << std::endl;
		enumerator->Release();
		CoUninitialize(); // Uninitialize COM library
		return false;
	}

	// Activate audio session manager
	hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&sessionManager));
	if (FAILED(hr)) {
		std::cerr << "Failed to activate audio session manager" << std::endl;
		device->Release();
		enumerator->Release();
		CoUninitialize(); // Uninitialize COM library
		return false;
	}

	// Get session enumerator
	hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
	if (FAILED(hr)) {
		std::cerr << "Failed to get session enumerator" << std::endl;
		sessionManager->Release();
		device->Release();
		enumerator->Release();
		CoUninitialize(); // Uninitialize COM library
		return false;
	}

	// Loop through each audio session
	int sessionCount = 0;
	hr = sessionEnumerator->GetCount(&sessionCount);
	if (SUCCEEDED(hr)) {
		for (int i = 0; i < sessionCount; ++i) {
			// Get audio session control
			IAudioSessionControl* sessionControl = nullptr;
			hr = sessionEnumerator->GetSession(i, &sessionControl);
			if (SUCCEEDED(hr)) {
				// Get process ID of the audio session
				DWORD sessionProcessId = 0;
				IAudioSessionControl2* sessionControl2 = nullptr;
				hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), reinterpret_cast<void**>(&sessionControl2));
				if (SUCCEEDED(hr)) {
					hr = sessionControl2->GetProcessId(&sessionProcessId);
					sessionControl2->Release();
				}
				if (SUCCEEDED(hr) && sessionProcessId == processId) {
					// Get simple audio volume
					ISimpleAudioVolume* simpleVolume = nullptr;
					hr = sessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), reinterpret_cast<void**>(&simpleVolume));
					if (SUCCEEDED(hr)) {
						// Set volume level
						hr = simpleVolume->SetMasterVolume(volumeLevel, nullptr);
						if (FAILED(hr)) {
							std::cerr << "Failed to set volume level for session" << std::endl;
						}
						else
							return true;
						simpleVolume->Release();
					}
				}
				sessionControl->Release();
			}
		}
	}

	// Release resources
	sessionEnumerator->Release();
	sessionManager->Release();
	device->Release();
	enumerator->Release();
	CoUninitialize(); // Uninitialize COM library
}