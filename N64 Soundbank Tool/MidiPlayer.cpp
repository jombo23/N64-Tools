#include "stdafx.h"
#include "MidiPlayer.h"

#include "N64AIFCAudio.h"
#include "SDKSound.h"
#include <math.h>
#include <algorithm>

using namespace std;

float Log(float base, float value)
{
	return log(value) / log(base);
}

float Log2(float value)
{
	return Log(2.f, value);
}

CMidiPlayer::CMidiPlayer()
{
	mMusic8 = NULL;
	mLoader = NULL;
	mPerformance = NULL;
	mSegment = NULL;
	mMusicPort = NULL;
	mSegmentState8 = NULL;
	mCollection = NULL;
	mPortDownload = NULL;

	StartedSegment = false;

	CoInitialize(NULL);

	EADGameReleaseTimeSeconds[0x00] = 1000;
	EADGameReleaseTimeSeconds[0x01] = 22.52571429;
	EADGameReleaseTimeSeconds[0x02] = 21.51428571;
	EADGameReleaseTimeSeconds[0x03] = 20.48571429;
	EADGameReleaseTimeSeconds[0x04] = 19.45714286;
	EADGameReleaseTimeSeconds[0x05] = 18.44571429;
	EADGameReleaseTimeSeconds[0x06] = 17.41714286;
	EADGameReleaseTimeSeconds[0x07] = 16.40571429;
	EADGameReleaseTimeSeconds[0x08] = 15.37714286;
	EADGameReleaseTimeSeconds[0x09] = 14.34857143;
	EADGameReleaseTimeSeconds[0x0A] = 13.32;
	EADGameReleaseTimeSeconds[0x0B] = 12.30857143;
	EADGameReleaseTimeSeconds[0x0C] = 11.28;
	EADGameReleaseTimeSeconds[0x0D] = 10.26857143;
	EADGameReleaseTimeSeconds[0x0E] = 9.24;
	EADGameReleaseTimeSeconds[0x0F] = 8.211428571;
	EADGameReleaseTimeSeconds[0x10] = 8.691428571;
	EADGameReleaseTimeSeconds[0x11] = 8.622857143;
	EADGameReleaseTimeSeconds[0x12] = 8.554285714;
	EADGameReleaseTimeSeconds[0x13] = 8.485714286;
	EADGameReleaseTimeSeconds[0x14] = 8.417142857;
	EADGameReleaseTimeSeconds[0x15] = 8.348571429;
	EADGameReleaseTimeSeconds[0x16] = 8.28;
	EADGameReleaseTimeSeconds[0x17] = 8.211428571;
	EADGameReleaseTimeSeconds[0x18] = 8.142857143;
	EADGameReleaseTimeSeconds[0x19] = 8.074285714;
	EADGameReleaseTimeSeconds[0x1A] = 8.005714286;
	EADGameReleaseTimeSeconds[0x1B] = 7.937142857;
	EADGameReleaseTimeSeconds[0x1C] = 7.868571429;
	EADGameReleaseTimeSeconds[0x1D] = 7.8;
	EADGameReleaseTimeSeconds[0x1E] = 7.731428571;
	EADGameReleaseTimeSeconds[0x1F] = 7.8;
	EADGameReleaseTimeSeconds[0x20] = 7.594285714;
	EADGameReleaseTimeSeconds[0x21] = 7.525714286;
	EADGameReleaseTimeSeconds[0x22] = 7.457142857;
	EADGameReleaseTimeSeconds[0x23] = 7.388571429;
	EADGameReleaseTimeSeconds[0x24] = 7.32;
	EADGameReleaseTimeSeconds[0x25] = 7.251428571;
	EADGameReleaseTimeSeconds[0x26] = 7.182857143;
	EADGameReleaseTimeSeconds[0x27] = 7.114285714;
	EADGameReleaseTimeSeconds[0x28] = 7.234285714;
	EADGameReleaseTimeSeconds[0x29] = 7.045714286;
	EADGameReleaseTimeSeconds[0x2A] = 6.908571429;
	EADGameReleaseTimeSeconds[0x2B] = 6.857142857;
	EADGameReleaseTimeSeconds[0x2C] = 6.788571429;
	EADGameReleaseTimeSeconds[0x2D] = 6.72;
	EADGameReleaseTimeSeconds[0x2E] = 6.651428571;
	EADGameReleaseTimeSeconds[0x2F] = 6.582857143;
	EADGameReleaseTimeSeconds[0x30] = 6.514285714;
	EADGameReleaseTimeSeconds[0x31] = 6.445714286;
	EADGameReleaseTimeSeconds[0x32] = 6.377142857;
	EADGameReleaseTimeSeconds[0x33] = 6.308571429;
	EADGameReleaseTimeSeconds[0x34] = 6.24;
	EADGameReleaseTimeSeconds[0x35] = 6.171428571;
	EADGameReleaseTimeSeconds[0x36] = 6.102857143;
	EADGameReleaseTimeSeconds[0x37] = 6.034285714;
	EADGameReleaseTimeSeconds[0x38] = 5.965714286;
	EADGameReleaseTimeSeconds[0x39] = 5.897142857;
	EADGameReleaseTimeSeconds[0x3A] = 5.828571429;
	EADGameReleaseTimeSeconds[0x3B] = 5.76;
	EADGameReleaseTimeSeconds[0x3C] = 5.691428572;
	EADGameReleaseTimeSeconds[0x3D] = 5.622857143;
	EADGameReleaseTimeSeconds[0x3E] = 5.554285714;
	EADGameReleaseTimeSeconds[0x3F] = 5.485714286;
	EADGameReleaseTimeSeconds[0x40] = 5.417142857;
	EADGameReleaseTimeSeconds[0x41] = 5.348571429;
	EADGameReleaseTimeSeconds[0x42] = 5.28;
	EADGameReleaseTimeSeconds[0x43] = 5.211428571;
	EADGameReleaseTimeSeconds[0x44] = 5.142857143;
	EADGameReleaseTimeSeconds[0x45] = 5.074285714;
	EADGameReleaseTimeSeconds[0x46] = 5.005714286;
	EADGameReleaseTimeSeconds[0x47] = 4.937142857;
	EADGameReleaseTimeSeconds[0x48] = 4.868571429;
	EADGameReleaseTimeSeconds[0x49] = 4.8;
	EADGameReleaseTimeSeconds[0x4A] = 4.731428572;
	EADGameReleaseTimeSeconds[0x4B] = 4.662857143;
	EADGameReleaseTimeSeconds[0x4C] = 4.594285714;
	EADGameReleaseTimeSeconds[0x4D] = 4.525714286;
	EADGameReleaseTimeSeconds[0x4E] = 4.457142857;
	EADGameReleaseTimeSeconds[0x4F] = 4.388571429;
	EADGameReleaseTimeSeconds[0x50] = 4.32;
	EADGameReleaseTimeSeconds[0x51] = 4.251428571;
	EADGameReleaseTimeSeconds[0x52] = 4.182857143;
	EADGameReleaseTimeSeconds[0x53] = 4.114285714;
	EADGameReleaseTimeSeconds[0x54] = 4.045714286;
	EADGameReleaseTimeSeconds[0x55] = 3.977142857;
	EADGameReleaseTimeSeconds[0x56] = 3.908571429;
	EADGameReleaseTimeSeconds[0x57] = 3.84;
	EADGameReleaseTimeSeconds[0x58] = 3.771428572;
	EADGameReleaseTimeSeconds[0x59] = 3.702857143;
	EADGameReleaseTimeSeconds[0x5A] = 3.634285714;
	EADGameReleaseTimeSeconds[0x5B] = 3.565714286;
	EADGameReleaseTimeSeconds[0x5C] = 3.497142857;
	EADGameReleaseTimeSeconds[0x5D] = 3.428571429;
	EADGameReleaseTimeSeconds[0x5E] = 3.36;
	EADGameReleaseTimeSeconds[0x5F] = 3.291428572;
	EADGameReleaseTimeSeconds[0x60] = 3.222857143;
	EADGameReleaseTimeSeconds[0x61] = 3.155357143;
	EADGameReleaseTimeSeconds[0x62] = 3.087857143;
	EADGameReleaseTimeSeconds[0x63] = 3.020357143;
	EADGameReleaseTimeSeconds[0x64] = 2.952857143;
	EADGameReleaseTimeSeconds[0x65] = 2.885357143;
	EADGameReleaseTimeSeconds[0x66] = 2.817857143;
	EADGameReleaseTimeSeconds[0x67] = 2.750357143;
	EADGameReleaseTimeSeconds[0x68] = 2.682857143;
	EADGameReleaseTimeSeconds[0x69] = 2.615357143;
	EADGameReleaseTimeSeconds[0x6A] = 2.547857143;
	EADGameReleaseTimeSeconds[0x6B] = 2.480357143;
	EADGameReleaseTimeSeconds[0x6C] = 2.412857143;
	EADGameReleaseTimeSeconds[0x6D] = 2.345357143;
	EADGameReleaseTimeSeconds[0x6E] = 2.277857143;
	EADGameReleaseTimeSeconds[0x6F] = 2.210357143;
	EADGameReleaseTimeSeconds[0x70] = 2.142857143;
	EADGameReleaseTimeSeconds[0x71] = 2.141785714;
	EADGameReleaseTimeSeconds[0x72] = 2.140714286;
	EADGameReleaseTimeSeconds[0x73] = 2.139642857;
	EADGameReleaseTimeSeconds[0x74] = 2.138571429;
	EADGameReleaseTimeSeconds[0x75] = 2.1375;
	EADGameReleaseTimeSeconds[0x76] = 2.136428571;
	EADGameReleaseTimeSeconds[0x77] = 2.135357143;
	EADGameReleaseTimeSeconds[0x78] = 2.134285714;
	EADGameReleaseTimeSeconds[0x79] = 2.133214286;
	EADGameReleaseTimeSeconds[0x7A] = 2.132142857;
	EADGameReleaseTimeSeconds[0x7B] = 2.131071429;
	EADGameReleaseTimeSeconds[0x7C] = 2.13;
	EADGameReleaseTimeSeconds[0x7D] = 2.128928572;
	EADGameReleaseTimeSeconds[0x7E] = 2.127857143;
	EADGameReleaseTimeSeconds[0x7F] = 2.126785714;
	EADGameReleaseTimeSeconds[0x80] = 2.125714286;
	EADGameReleaseTimeSeconds[0x81] = 2.108571429;
	EADGameReleaseTimeSeconds[0x82] = 2.091428571;
	EADGameReleaseTimeSeconds[0x83] = 2.074285714;
	EADGameReleaseTimeSeconds[0x84] = 2.057142857;
	EADGameReleaseTimeSeconds[0x85] = 2.04;
	EADGameReleaseTimeSeconds[0x86] = 2.022857143;
	EADGameReleaseTimeSeconds[0x87] = 2.005714285;
	EADGameReleaseTimeSeconds[0x88] = 1.988571428;
	EADGameReleaseTimeSeconds[0x89] = 1.971428571;
	EADGameReleaseTimeSeconds[0x8A] = 1.954285714;
	EADGameReleaseTimeSeconds[0x8B] = 1.937142857;
	EADGameReleaseTimeSeconds[0x8C] = 1.919999999;
	EADGameReleaseTimeSeconds[0x8D] = 1.902857142;
	EADGameReleaseTimeSeconds[0x8E] = 1.885714285;
	EADGameReleaseTimeSeconds[0x8F] = 1.868571428;
	EADGameReleaseTimeSeconds[0x90] = 1.851428571;
	EADGameReleaseTimeSeconds[0x91] = 1.834285714;
	EADGameReleaseTimeSeconds[0x92] = 1.817142857;
	EADGameReleaseTimeSeconds[0x93] = 1.8;
	EADGameReleaseTimeSeconds[0x94] = 1.782857143;
	EADGameReleaseTimeSeconds[0x95] = 1.765714286;
	EADGameReleaseTimeSeconds[0x96] = 1.748571429;
	EADGameReleaseTimeSeconds[0x97] = 1.731428572;
	EADGameReleaseTimeSeconds[0x98] = 1.714285714;
	EADGameReleaseTimeSeconds[0x99] = 1.697142857;
	EADGameReleaseTimeSeconds[0x9A] = 1.68;
	EADGameReleaseTimeSeconds[0x9B] = 1.662857143;
	EADGameReleaseTimeSeconds[0x9C] = 1.645714286;
	EADGameReleaseTimeSeconds[0x9D] = 1.628571429;
	EADGameReleaseTimeSeconds[0x9E] = 1.611428572;
	EADGameReleaseTimeSeconds[0x9F] = 1.594285715;
	EADGameReleaseTimeSeconds[0xA0] = 1.577142857;
	EADGameReleaseTimeSeconds[0xA1] = 1.56;
	EADGameReleaseTimeSeconds[0xA2] = 1.542857143;
	EADGameReleaseTimeSeconds[0xA3] = 1.525714286;
	EADGameReleaseTimeSeconds[0xA4] = 1.508571429;
	EADGameReleaseTimeSeconds[0xA5] = 1.491428572;
	EADGameReleaseTimeSeconds[0xA6] = 1.474285714;
	EADGameReleaseTimeSeconds[0xA7] = 1.457142857;
	EADGameReleaseTimeSeconds[0xA8] = 1.44;
	EADGameReleaseTimeSeconds[0xA9] = 1.422857143;
	EADGameReleaseTimeSeconds[0xAA] = 1.405714286;
	EADGameReleaseTimeSeconds[0xAB] = 1.388571429;
	EADGameReleaseTimeSeconds[0xAC] = 1.371428572;
	EADGameReleaseTimeSeconds[0xAD] = 1.354285715;
	EADGameReleaseTimeSeconds[0xAE] = 1.337142857;
	EADGameReleaseTimeSeconds[0xAF] = 1.32;
	EADGameReleaseTimeSeconds[0xB0] = 1.302857143;
	EADGameReleaseTimeSeconds[0xB1] = 1.285714286;
	EADGameReleaseTimeSeconds[0xB2] = 1.268571429;
	EADGameReleaseTimeSeconds[0xB3] = 1.251428571;
	EADGameReleaseTimeSeconds[0xB4] = 1.234285714;
	EADGameReleaseTimeSeconds[0xB5] = 1.217142857;
	EADGameReleaseTimeSeconds[0xB6] = 1.2;
	EADGameReleaseTimeSeconds[0xB7] = 1.182857143;
	EADGameReleaseTimeSeconds[0xB8] = 1.165714286;
	EADGameReleaseTimeSeconds[0xB9] = 1.148571429;
	EADGameReleaseTimeSeconds[0xBA] = 1.131428572;
	EADGameReleaseTimeSeconds[0xBB] = 1.114285714;
	EADGameReleaseTimeSeconds[0xBC] = 1.097142857;
	EADGameReleaseTimeSeconds[0xBD] = 1.08;
	EADGameReleaseTimeSeconds[0xBE] = 1.062857143;
	EADGameReleaseTimeSeconds[0xBF] = 1.045714286;
	EADGameReleaseTimeSeconds[0xC0] = 1.028571429;
	EADGameReleaseTimeSeconds[0xC1] = 1.011428571;
	EADGameReleaseTimeSeconds[0xC2] = 0.994285714;
	EADGameReleaseTimeSeconds[0xC3] = 0.977142857;
	EADGameReleaseTimeSeconds[0xC4] = 0.96;
	EADGameReleaseTimeSeconds[0xC5] = 0.942857143;
	EADGameReleaseTimeSeconds[0xC6] = 0.925714285;
	EADGameReleaseTimeSeconds[0xC7] = 0.908571428;
	EADGameReleaseTimeSeconds[0xC8] = 0.891428571;
	EADGameReleaseTimeSeconds[0xC9] = 0.874285714;
	EADGameReleaseTimeSeconds[0xCA] = 0.857142857;
	EADGameReleaseTimeSeconds[0xCB] = 0.84;
	EADGameReleaseTimeSeconds[0xCC] = 0.822857142;
	EADGameReleaseTimeSeconds[0xCD] = 0.805714285;
	EADGameReleaseTimeSeconds[0xCE] = 0.788571428;
	EADGameReleaseTimeSeconds[0xCF] = 0.771428571;
	EADGameReleaseTimeSeconds[0xD0] = 0.754285714;
	EADGameReleaseTimeSeconds[0xD1] = 0.737142857;
	EADGameReleaseTimeSeconds[0xD2] = 0.72;
	EADGameReleaseTimeSeconds[0xD3] = 0.702857143;
	EADGameReleaseTimeSeconds[0xD4] = 0.685714286;
	EADGameReleaseTimeSeconds[0xD5] = 0.668571429;
	EADGameReleaseTimeSeconds[0xD6] = 0.651428572;
	EADGameReleaseTimeSeconds[0xD7] = 0.634285714;
	EADGameReleaseTimeSeconds[0xD8] = 0.617142857;
	EADGameReleaseTimeSeconds[0xD9] = 0.6;
	EADGameReleaseTimeSeconds[0xDA] = 0.582857143;
	EADGameReleaseTimeSeconds[0xDB] = 0.565714286;
	EADGameReleaseTimeSeconds[0xDC] = 0.548571429;
	EADGameReleaseTimeSeconds[0xDD] = 0.531428572;
	EADGameReleaseTimeSeconds[0xDE] = 0.514285715;
	EADGameReleaseTimeSeconds[0xDF] = 0.497142857;
	EADGameReleaseTimeSeconds[0xE0] = 0.48;
	EADGameReleaseTimeSeconds[0xE1] = 0.462857143;
	EADGameReleaseTimeSeconds[0xE2] = 0.445714286;
	EADGameReleaseTimeSeconds[0xE3] = 0.428571429;
	EADGameReleaseTimeSeconds[0xE4] = 0.411428571;
	EADGameReleaseTimeSeconds[0xE5] = 0.394285714;
	EADGameReleaseTimeSeconds[0xE6] = 0.377142857;
	EADGameReleaseTimeSeconds[0xE7] = 0.36;
	EADGameReleaseTimeSeconds[0xE8] = 0.342857143;
	EADGameReleaseTimeSeconds[0xE9] = 0.325714286;
	EADGameReleaseTimeSeconds[0xEA] = 0.308571429;
	EADGameReleaseTimeSeconds[0xEB] = 0.291428571;
	EADGameReleaseTimeSeconds[0xEC] = 0.274285714;
	EADGameReleaseTimeSeconds[0xED] = 0.257142857;
	EADGameReleaseTimeSeconds[0xEE] = 0.24;
	EADGameReleaseTimeSeconds[0xEF] = 0.222857143;
	EADGameReleaseTimeSeconds[0xF0] = 0.205714286;
	EADGameReleaseTimeSeconds[0xF1] = 0.188571429;
	EADGameReleaseTimeSeconds[0xF2] = 0.171428571;
	EADGameReleaseTimeSeconds[0xF3] = 0.154285714;
	EADGameReleaseTimeSeconds[0xF4] = 0.137142857;
	EADGameReleaseTimeSeconds[0xF5] = 0.12;
	EADGameReleaseTimeSeconds[0xF6] = 0.102857143;
	EADGameReleaseTimeSeconds[0xF7] = 0.171428571;
	EADGameReleaseTimeSeconds[0xF8] = 0.068571429;
	EADGameReleaseTimeSeconds[0xF9] = 0.051428571;
	EADGameReleaseTimeSeconds[0xFA] = 0.034285714;
	EADGameReleaseTimeSeconds[0xFB] = 0.025714286;
	EADGameReleaseTimeSeconds[0xFC] = 0.017142857;
	EADGameReleaseTimeSeconds[0xFD] = 0.008571429;
	EADGameReleaseTimeSeconds[0xFE] = 0.0042855;
	EADGameReleaseTimeSeconds[0xFF] = 0;
}

CMidiPlayer::~CMidiPlayer()
{
	UnloadAllLoadedInstruments();

	if (mSegmentState8 != NULL)
		mSegmentState8->Release();

	UnloadMidi();

	if (mCollection != NULL)
		mCollection->Release();

	if (mPerformance != NULL)
	{
		if (mMusicPort != NULL)
			mPerformance->RemovePort(mMusicPort);
		mPerformance->CloseDown();
		if (mMusicPort != NULL)
			mMusicPort->Release();
	}

	if (mLoader != NULL)
		mLoader->Release();

	if (mMusic8 != NULL)
		mMusic8->Release();
	if (mMusicPort != NULL)
		mMusicPort->Release();

	CoUninitialize();
}

HRESULT CMidiPlayer::Startup(CString mainFolder, HWND hwnd)
{
	this->mainFolder = mainFolder;

	HRESULT hr = S_OK;

	if (FAILED(hr = CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (LPVOID*)&mLoader)))
		return hr;

	if (FAILED(hr = CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (LPVOID*)&mPerformance)))
		return hr;


	IDirectMusic* music = NULL;
	if (FAILED(hr = mPerformance->Init(&music, NULL, NULL)))
		return hr;

	if (FAILED(hr = music->QueryInterface(IID_IDirectMusic8, (LPVOID*)&mMusic8)))
	{
		if (music != NULL)
			music->Release();

		return hr;
	}

	if (music != NULL)
		music->Release();

	return S_OK;
}

HRESULT CMidiPlayer::GetNextPort(unsigned long portIndex, MIDIINFO& midiPort)
{
	HRESULT hr = S_OK;
	DMUS_PORTCAPS portCaps;

	ZeroMemory(&portCaps, sizeof(portCaps));
	portCaps.dwSize = sizeof(DMUS_PORTCAPS);

	if (FAILED(hr = mMusic8->EnumPort(portIndex, &portCaps)))
		return hr;

	midiPort.description = CString(portCaps.wszDescription);

	midiPort.guidPort = portCaps.guidPort;
	midiPort.dwClass = portCaps.dwClass;
	midiPort.dwFlags = portCaps.dwFlags;
	midiPort.dwMaxChannelGroups = portCaps.dwMaxChannelGroups;

	return hr;
}

HRESULT CMidiPlayer::ChoosePort(MIDIINFO& midiPort)
{
	HRESULT hr = S_OK;
	DMUS_PORTPARAMS portParams;

	ZeroMemory(&portParams, sizeof(DMUS_PORTPARAMS));
	portParams.dwSize = sizeof(DMUS_PORTPARAMS);
	portParams.dwChannelGroups = 1;
	portParams.dwValidParams = DMUS_PORTPARAMS_CHANNELGROUPS;	

	if (midiPort.dwFlags & DMUS_PC_AUDIOPATH)
		portParams.dwFeatures = DMUS_PORT_FEATURE_AUDIOPATH;

	portParams.dwEffectFlags = DMUS_EFFECT_REVERB;

	if (FAILED(hr = mMusic8->CreatePort(midiPort.guidPort, &portParams, &mMusicPort, NULL)))
		return hr;

	if (FAILED(hr = mMusicPort->Activate(TRUE)))
	{
		mMusicPort->Release();
		mMusicPort = NULL;
		return hr;
	}

	if (FAILED(hr = mPerformance->AddPort(mMusicPort)))
		return hr;

	if (FAILED(hr = mPerformance->AssignPChannelBlock(0, mMusicPort, 1)))
		return hr;

	if (FAILED(hr = mMusicPort->QueryInterface(IID_IDirectMusicPortDownload8, (LPVOID*)&mPortDownload)))
		return hr;

	return S_OK;
}

HRESULT CMidiPlayer::LoadMidi(LPCSTR midiName)
{
	HRESULT hr = S_OK;

	UnloadMidi();

	CStringW midiCString(midiName);
	WCHAR* wideChar = midiCString.GetBuffer(0);

	if (FAILED(hr = mLoader->LoadObjectFromFile(CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, wideChar, (LPVOID*)&mSegment)))
		return hr;

	midiCString.ReleaseBuffer();

	if (FAILED(hr = mSegment->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL)))
		return hr;

	if (FAILED(hr = mSegment->Download(mPerformance)))
		return hr;

	StartedSegment = false;

	GUID guid = GUID_NOTIFICATION_SEGMENT;
	if (FAILED(hr = mSegment->AddNotificationType(guid)))
		return hr;

	return S_OK;
}

HRESULT CMidiPlayer::UnloadMidi()
{
	if (mSegment != NULL)
	{
		HRESULT hr = mLoader->ReleaseObjectByUnknown(mSegment);

		GUID guid = GUID_NOTIFICATION_SEGMENT;
		mSegment->RemoveNotificationType(guid);

		mSegment->Release();
		mSegment = NULL;
	}

	return S_OK;
}

HRESULT CMidiPlayer::Play()
{
	if (mSegment == NULL)
		return S_FALSE;

	HRESULT hr = S_OK;

	IDirectMusicSegmentState* segmentState = NULL;
	if (FAILED(hr = mPerformance->PlaySegment(mSegment, 0, 0, &segmentState)))
		return hr;

	if (FAILED(hr = segmentState->QueryInterface(IID_IDirectMusicSegmentState8, (LPVOID*)&mSegmentState8)))
	{
		if (segmentState != NULL)
			segmentState->Release();

		return hr;
	}

	if (segmentState != NULL)
		segmentState->Release();

	return hr;
}

HRESULT CMidiPlayer::SetRepeat(bool repeat)
{
	if (mSegment == NULL)
		return E_INVALIDARG;

	HRESULT hr = S_OK;

	if (repeat)
		hr = mSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	else
		hr = mSegment->SetRepeats(0);

	return hr;
}

HRESULT CMidiPlayer::SetLoopSpot(int startLoop, int endLoop)
{
	if (mSegment == NULL)
		return E_INVALIDARG;

	HRESULT hr = S_OK;

	mSegment->SetLoopPoints(startLoop, endLoop);

	return hr;
}

HRESULT CMidiPlayer::Stop()
{
	HRESULT hr = S_OK;

	if (mPerformance == NULL)
		return S_OK;

	if (FAILED(hr = mPerformance->Stop(NULL, NULL, 0, 0)))
		return hr;

	if (mSegment != NULL)
	{
		if (FAILED(hr = mSegment->SetStartPoint(0))) 
			return hr;
	}

	return S_OK;
}

HRESULT CMidiPlayer::Pause()
{
	MUSIC_TIME musicTime;
	HRESULT hr = S_OK;

	if (FAILED(hr = mPerformance->Stop(NULL, NULL, 0, 0)))
		return hr;

	if (FAILED(hr = mSegmentState8->GetSeek(&musicTime)))
		return hr;

	if (FAILED(hr = mSegment->SetStartPoint(musicTime)))
		return hr;

	return S_OK;

}

HRESULT CMidiPlayer::Resume()
{
	HRESULT hr = S_OK;

	if (IsPlaying() == S_FALSE)
	{
		hr = Play();
		return hr;
	}
	return S_FALSE;
}

HRESULT CMidiPlayer::IsPlaying()
{
	HRESULT hr = S_OK;

	if (FAILED(hr = mPerformance->IsPlaying(mSegment, NULL)))
		return hr;

	return hr;
}

HRESULT CMidiPlayer::UnloadAllLoadedInstruments()
{
	for (int x = 0; x < mInstrumentsLoaded.size(); x++)
	{
		mInstrumentsLoaded[x]->Release();
	}

	mInstrumentsLoaded.clear();

	HRESULT hr = S_OK;

	for (int x = 0; x < mInstrumentGeneratedLoaded.size(); x++)
	{
		hr = mPortDownload->Unload(mInstrumentGeneratedLoaded[x]);
		mInstrumentGeneratedLoaded[x]->Release();
	}

	mInstrumentGeneratedLoaded.clear();

	return hr;
}

HRESULT CMidiPlayer::LoadDLS(LPTSTR dlsFilename)
{
	UnloadAllLoadedInstruments();

	HRESULT hr = S_OK;
	DMUS_OBJECTDESC musObjectDesc;

	int sizeofObjectDesc = sizeof(DMUS_OBJECTDESC);

	ZeroMemory(&musObjectDesc, sizeofObjectDesc);
	musObjectDesc.dwSize = sizeofObjectDesc;
	musObjectDesc.guidClass = CLSID_DirectMusicCollection;

	if (dlsFilename == NULL)
	{
		// use general midi
		musObjectDesc.guidObject = GUID_DefaultGMCollection;
		musObjectDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_OBJECT;
	}
	else
	{
		CStringW midiCString(dlsFilename);
		WCHAR* wideChar = midiCString.GetBuffer(0);
		wcsncpy(musObjectDesc.wszFileName, wideChar, DMUS_MAX_FILENAME);
		midiCString.ReleaseBuffer();

		musObjectDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME | DMUS_OBJ_FULLPATH;
	}

	if ((mLoader != NULL) && (mCollection != NULL))
	{
		if (FAILED(hr = mLoader->ReleaseObjectByUnknown(mCollection)))
		{
			if (mCollection != NULL)
			{
				mCollection->Release();
				mCollection = NULL;
			}
			return hr;
		}
	}

	if (mLoader)
	{
		if (FAILED(hr = mLoader->GetObject(&musObjectDesc, IID_IDirectMusicCollection, (LPVOID*)&mCollection)))
			return hr;

		if (mCollection != NULL)
		{
			unsigned long patchNumber = 0;
			WCHAR wszInstName[MAX_PATH];

			unsigned long instrumentIndex = 0;

			while (mCollection->EnumInstrument(instrumentIndex, &patchNumber, wszInstName, MAX_PATH) == S_OK)
			{
				IDirectMusicInstrument8* musicInstrument = NULL;

				if (FAILED(hr = mCollection->GetInstrument(patchNumber, &musicInstrument)))
					return hr;

				IDirectMusicDownloadedInstrument* mDownloadedInstrument = NULL;

				DMUS_NOTERANGE noteRange;
				noteRange.dwHighNote = 127;
				noteRange.dwLowNote = 0;

				unsigned long channel = 0;
				unsigned long group = 0;
				if (FAILED(hr = mPerformance->DownloadInstrument(musicInstrument, 0, &mDownloadedInstrument, &noteRange, 1, &mMusicPort, &group, &channel)))
					return hr;
				instrumentIndex++;

				if (mDownloadedInstrument != NULL)
				{
					mInstrumentsLoaded.push_back(mDownloadedInstrument);
				}
			}
		}
	}

	return S_OK;
}

TCENT CMidiPlayer::TimeSecondsToCents(double timeSeconds)
{
	if (timeSeconds == 0)
		return (TCENT)0x80000000;
	else
		return (TCENT)ceil((Log2(timeSeconds)) * 1200.0 * 65536.0);
}

PCENT CMidiPlayer::PitchHzToCents(double frequencyHz)
{
	return (PCENT)(((Log2(frequencyHz / 440.0)) * 1200.0 + 6900.0) * 65536.0);
}

PCENT CMidiPlayer::PercentToUnits(double percent)
{
	return (PCENT)(percent * 10.0 * 65536.0);
}

unsigned short CMidiPlayer::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned short CMidiPlayer::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned long CMidiPlayer::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CMidiPlayer::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

void CMidiPlayer::ParseSoundMacroList(std::vector<Factor5SoundMacro> soundMacroList, std::map<int, Factor5ADSR> poolTables, int& sampleId, unsigned long& attackTime, unsigned long& decayTime, float& sustainPercentage, unsigned long& releaseTime, unsigned char& macroPan)
{
	for (int macroNumber = 0; macroNumber < soundMacroList.size(); macroNumber++)
	{
		if (soundMacroList[macroNumber].commandData[0] == 0xC) // SETADSR
		{
			unsigned short tableLookupId = (soundMacroList[macroNumber].commandData[2] << 8) | soundMacroList[macroNumber].commandData[1];
			Factor5ADSR adsrLookedUp = poolTables[tableLookupId];
			unsigned char dlsMode = soundMacroList[macroNumber].commandData[3];

			attackTime = adsrLookedUp.attackTime;
			decayTime = adsrLookedUp.decayTime;
			sustainPercentage = adsrLookedUp.sustainPercentage;
			releaseTime = adsrLookedUp.releaseTime;
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0xE) // Pan
		{
			macroPan = soundMacroList[macroNumber].commandData[1];
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0x10) // Start Sound
		{
			sampleId = (soundMacroList[macroNumber].commandData[2] << 8) | soundMacroList[macroNumber].commandData[1];
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0x16) // SETADSRCTRL
		{
			attackTime = soundMacroList[macroNumber].commandData[1];
			decayTime = soundMacroList[macroNumber].commandData[2];
			sustainPercentage = (float)soundMacroList[macroNumber].commandData[3] / 256.0f;
			releaseTime = soundMacroList[macroNumber].commandData[4];
		}
	}
}

HRESULT CMidiPlayer::SetupMidiSoundBankFactor5(int matchSongId, unsigned char* buffer, int romSize, unsigned long projOffset, unsigned long poolOffset, int projSize, int poolSize, ALBank* alBankInstr, float timeMultiplier, bool halfSamplingRate, bool overrideSamplingRate, int samplingRate, CString gameSoundType)
{ 
	// THIS DOESN'T YET WORK, plus has lookup...
	HRESULT hr = S_OK;

	UnloadAllLoadedInstruments();

	CN64AIFCAudio n64Audio;

	unsigned char* proj = NULL;
	unsigned char* pool = NULL;

	bool isCompressed = false;
	int zlibGame = 0;
	if (buffer[poolOffset] == 0x78)
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameSoundType == "Musyx")
	{
		isCompressed = false;
	}
	else if (gameSoundType == "ZLibMusyx")
	{
		isCompressed = true;
		zlibGame = GAUNTLETLEGENDS;
	}
	else if (gameSoundType == "ZLib78DAMusyx")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameSoundType == "MusyxREZLib")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameSoundType == "MusyxSmallZlib")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameSoundType == "LzMusyx")
	{
		isCompressed = true;
	}
	if (!isCompressed)
	{
		proj = new unsigned char[projSize];
		memcpy(proj, &buffer[projOffset], projSize);
		pool = new unsigned char[poolSize];
		memcpy(pool, &buffer[poolOffset], poolSize);
	}
	else
	{
		projSize = 0;
		poolSize = 0;

		if (gameSoundType == "LzMusyx")
		{
			proj = new unsigned char[0x100000];
			pool = new unsigned char[0x100000];
			MidwayLZ midwayLz;
			projSize = midwayLz.dec(&buffer[projOffset], romSize - projOffset, proj);
			poolSize = midwayLz.dec(&buffer[poolOffset], romSize - poolOffset, pool);
			/*FILE* projDebugFile = fopen("C:\\temp\\proj.bin", "wb");
			if (projDebugFile)
			{
				fwrite(proj, 1, projSize, projDebugFile);
				fflush(projDebugFile);
				fclose(projDebugFile);
			}
			FILE* poolDebugFile = fopen("C:\\temp\\pool.bin", "wb");
			if (poolDebugFile)
			{
				fwrite(pool, 1, poolSize, poolDebugFile);
				fflush(poolDebugFile);
				fclose(poolDebugFile);
			}*/
		}
		else
		{
			GECompression compression;
			compression.SetPath(mainFolder);
			if ((buffer[poolOffset] == 0x78) && (buffer[poolOffset+1] == 0xDA))
			{
				compression.SetGame(STUNTRACER64);
			}
			else
			{
				compression.SetGame(zlibGame);
			}

			compression.SetCompressedBuffer(&buffer[projOffset], romSize - projOffset);
			projSize = 0;
			int projCompressedSize = 0;
			proj = compression.OutputDecompressedBuffer(projSize, projCompressedSize);

			compression.SetCompressedBuffer(&buffer[poolOffset], romSize - poolOffset);
			poolSize = 0;
			int poolCompressedSize = 0;
			pool = compression.OutputDecompressedBuffer(poolSize, poolCompressedSize);
		}
	}
	

	unsigned int soundMacrosOffset = CharArrayToLong(&pool[0]);
    unsigned int tablesOffset = CharArrayToLong(&pool[4]);
    unsigned int keymapsOffset = CharArrayToLong(&pool[8]);
    unsigned int layersOffset = CharArrayToLong(&pool[0xC]);

	std::map<int, Factor5SoundMacroList> poolMacroList; // By ObjectId
	std::map<int, Factor5ADSR> poolTables; // By ObjectId
	std::map<int, Factor5KeymapGroup> poolKeyGroups; // By ObjectId
	std::map<int, Factor5LayerGroup> poolLayers; // By ObjectId

	CString commandType[0xFF];
	commandType[0x1] = "STOP";
	commandType[0x2] = "SPLITKEY";
	commandType[0x3] = "SPLITVEL";
	commandType[0x4] = "WAIT_TICKS";
	commandType[0x5] = "LOOP";
	commandType[0x6] = "GOTO";
	commandType[0x7] = "WAIT_MS";
	commandType[0x8] = "PLAYMACRO";
	commandType[0x9] = "SENDKEYOFF";
	commandType[0xA] = "SPLITMOD";
	commandType[0xB] = "PIANOPAN";
	commandType[0xC] = "SETADSR";
	commandType[0xD] = "SCALEVOLUME";
	commandType[0xE] = "PANNING";
	commandType[0xF] = "ENVELOPE";
	commandType[0x10] = "STARTSAMPLE";
	commandType[0x11] = "STOPSAMPLE";
	commandType[0x12] = "KEYOFF";
	commandType[0x13] = "SPLITRND";
	commandType[0x14] = "FADE-IN";
	commandType[0x15] = "SPANNING";
	commandType[0x16] = "SETADSRCTRL";
	commandType[0x17] = "RNDNOTE";
	commandType[0x18] = "ADDNOTE";
	commandType[0x19] = "SETNOTE";
	commandType[0x1A] = "LASTNOTE";
	commandType[0x1B] = "PORTAMENTO";
	commandType[0x1C] = "VIBRATO";
	commandType[0x1D] = "PITCHSWEEP1";
	commandType[0x1E] = "PITCHSWEEP2";
	commandType[0x1F] = "SETPITCH";
	commandType[0x20] = "SETPITCHADSR";
	commandType[0x21] = "SCALEVOLUME DLS";
	commandType[0x22] = "MOD2VIBRANGE";
	commandType[0x23] = "SETUP TREMOLO";
	commandType[0x24] = "RETURN";
	commandType[0x25] = "GOSUB";
	commandType[0x28] = "TRAP_EVENT";
	commandType[0x29] = "UNTRAP_EVENT";
	commandType[0x2A] = "SEND_MESSAGE";
	commandType[0x2B] = "GET_MESSAGE";
	commandType[0x2C] = "GET_VID";
	commandType[0x30] = "ADDAGECOUNT";
	commandType[0x31] = "SETAGECOUNT";
	commandType[0x32] = "SENDFLAG";
	commandType[0x33] = "PITCHWHEELR";
	commandType[0x34] = "SETPRIORITY";
	commandType[0x35] = "ADDPRIORITY";
	commandType[0x36] = "AGECNTSPEED";
	commandType[0x37] = "AGECNTVEL";
	commandType[0x40] = "VOL_SELECT";
	commandType[0x41] = "PAN_SELECT";
	commandType[0x42] = "PitchW_SELECT";
	commandType[0x43] = "ModW_SELECT";
	commandType[0x44] = "PEDAL_SELECT";
	commandType[0x45] = "PORTA_SELECT";
	commandType[0x46] = "REVERB_SELECT";
	commandType[0x47] = "SPAN_SELECT";
	commandType[0x48] = "DOPPLER_SELECT";
	commandType[0x49] = "TREMOLO_SELECT";
	commandType[0x4A] = "PREA_SELECT";
	commandType[0x4B] = "PREB_SELECT";
	commandType[0x4C] = "POSTB_SELECT";
	commandType[0x4D] = "AUXAFX_SELECT";
	commandType[0x4E] = "AUXBFX_SELECT";
	commandType[0x50] = "SETUP_LFO";
	commandType[0x58] = "MODE_SELECT";
	commandType[0x59] = "SET_KEYGROUP";
	commandType[0x5A] = "SRCMODE_SELECT";
	commandType[0x60] = "ADD_VARS";
	commandType[0x61] = "SUB_VARS";
	commandType[0x62] = "MUL_VARS";
	commandType[0x63] = "DIV_VARS";
	commandType[0x64] = "ADDI_VARS";
	commandType[0x70] = "IF_EQUAL";
	commandType[0x71] = "IF_LESS";
	commandType[0x0] = "END";


	if (soundMacrosOffset != NULL)
	{
		//fprintf(outPool, "Sound Macros Offset\n");

		unsigned long tempSoundMacrosOffset = soundMacrosOffset;
		while (CharArrayToLong(&pool[tempSoundMacrosOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempSoundMacrosOffset]);
            int id = CharArrayToShort(&pool[tempSoundMacrosOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X\n", tempSoundMacrosOffset, id, size);

			Factor5SoundMacroList soundMacroList;

			unsigned long tempCommandOffset = tempSoundMacrosOffset + 8;
			while (pool[tempCommandOffset + 3] != 0x00)
			{
				Factor5SoundMacro soundMacro;

				soundMacro.commandData[0] = pool[tempCommandOffset + 3];
				soundMacro.commandData[1] = pool[tempCommandOffset + 2];
				soundMacro.commandData[2] = pool[tempCommandOffset + 1];
				soundMacro.commandData[3] = pool[tempCommandOffset + 0];

				soundMacro.commandData[4] = pool[tempCommandOffset + 7];
				soundMacro.commandData[5] = pool[tempCommandOffset + 6];
				soundMacro.commandData[6] = pool[tempCommandOffset + 5];
				soundMacro.commandData[7] = pool[tempCommandOffset + 4];

				soundMacroList.soundMacros.push_back(soundMacro);

				//fprintf(outPool, "	%08X: Command %02X %s - %02X %02X %02X %02X %02X %02X %02X\n", tempCommandOffset, commandData[0], commandType[commandData[0]], commandData[1], commandData[2], commandData[3], commandData[4], commandData[5], commandData[6], commandData[7]);
				

				tempCommandOffset += 8;
			}

			poolMacroList[id] = soundMacroList;

			tempSoundMacrosOffset += size;

		}
	}

	if (tablesOffset != NULL)
	{
		//fprintf(outPool, "\nTables Offset\n");

		unsigned long tempTablesOffset = tablesOffset;
		while (CharArrayToLong(&pool[tempTablesOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempTablesOffset]);
            int id = CharArrayToShort(&pool[tempTablesOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X", tempTablesOffset, id, size);

			/*0x0	1	Attack time (0-255 milliseconds); no multiplication is done to the value
			0x1	1	Attack time (0-65280 milliseconds); multiply value by 256
			0x2	1	Decay time (0-255 milliseconds); no multiplication is done to the value
			0x3	1	Decay time (0-65280 milliseconds); multiply value by 256
			0x4	1	Sustain (percentage); multiply value by 0.0244
			0x5	1	Sustain (percentage); multiply value by 6.25
			0x6	1	Release time (0-255 milliseconds); no multiplication is done to the value
			0x7	1	Release time (0-65280 milliseconds); multiply value by 256*/

			Factor5ADSR adsr;
			adsr.objectId = id;
			adsr.attackTime = (pool[tempTablesOffset + 8] + (pool[tempTablesOffset + 9] << 8));
			adsr.decayTime = (pool[tempTablesOffset + 0xA] + (pool[tempTablesOffset + 0xB] << 8));
			adsr.sustainPercentage = (((float)pool[tempTablesOffset + 0xC] * 0.0244) + ((float)pool[tempTablesOffset + 0xD] * 6.25));
			adsr.releaseTime = (pool[tempTablesOffset + 0xE] + (pool[tempTablesOffset + 0xF] << 8));

			//fprintf(outPool, " Attack Time %04X Decay Time %04X Sustain %f Release Time %04X\n", attackTime, decayTime, sustainPercentage, releaseTime);

			poolTables[id] = adsr;

			tempTablesOffset += size;
		}
	}
	
	if (keymapsOffset != NULL)
	{
		//fprintf(outPool, "\nKeymaps Offset\n");

		unsigned long tempKeymapsOffset = keymapsOffset;
		while (CharArrayToLong(&pool[tempKeymapsOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempKeymapsOffset]);
            int id = CharArrayToShort(&pool[tempKeymapsOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X\n", tempKeymapsOffset, id, size);

			Factor5KeymapGroup keymapGroup;
			keymapGroup.objectId = id;
			for (int y = 0; y < 128; y++)
			{
				keymapGroup.keys[y].objectId = CharArrayToShort(&pool[tempKeymapsOffset + 8 + (y * 8)]);
				keymapGroup.keys[y].transpose = pool[tempKeymapsOffset + 8 + (y * 8) + 1];
				keymapGroup.keys[y].pan = pool[tempKeymapsOffset + 8 + (y * 8) + 2];
				keymapGroup.keys[y].priorityOffset = pool[tempKeymapsOffset + 8 + (y * 8) + 3];
				//fprintf(outPool, "	%08X: Key #%02X ObjectId %04X Transpose %02X Pan %02X Priority Offset %02X\n", tempKeymapsOffset + 8 + (y * 8), y, objectId, transpose, pan, priorityOffset);
			}

			poolKeyGroups[id] = keymapGroup;
			tempKeymapsOffset += size;
		}
	}

	if (layersOffset != NULL)
	{
		//fprintf(outPool, "\nLayers Offset\n");

		unsigned long tempLayersOffset = layersOffset;
		/*if (CharArrayToLong(&pool[tempLayersOffset]) == 0xFFFFFFFF)
		{
			delete [] pool;
			delete [] proj;
			return S_FALSE;
		}*/

		while (CharArrayToLong(&pool[tempLayersOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempLayersOffset]);
            int id = CharArrayToShort(&pool[tempLayersOffset + 4]);

			unsigned long count = CharArrayToLong(&pool[tempLayersOffset + 8]);
			//fprintf(outPool, "%08X: Id %04X Size %08X Count %08X\n", tempLayersOffset, id, size, count);

			Factor5LayerGroup layerGroup;
			layerGroup.objectId = id;

			unsigned long subTempLayersOffset = tempLayersOffset + 12;
			for (int y = 0; y < count ; y++)
			{
				Factor5Layer layer;

				layer.objectId = CharArrayToShort(&pool[subTempLayersOffset]);
				layer.keyLo = pool[subTempLayersOffset + 2];
				layer.keyHi = pool[subTempLayersOffset + 3];
				layer.transpose = pool[subTempLayersOffset + 4];
				layer.volume = pool[subTempLayersOffset + 5];
				layer.priorityOffset = pool[subTempLayersOffset + 6];
				layer.span = pool[subTempLayersOffset + 7];
				layer.pan = pool[subTempLayersOffset + 8];

				layerGroup.factor5Layers.push_back(layer);

				//fprintf(outPool, "%08X: #%02X Object Id %04X Key Low %02X Key Hi %02X Transpose %02X Volume %02X Priority Offset %02X Span %02X Pan %02X\n", subTempLayersOffset, y, objectId, keyLo, keyHi, transpose, volume, priorityOffset, span, pan);
				subTempLayersOffset += 12;
			}

			poolLayers[id] = layerGroup;
			//fprintf(outPool, "\n");

			tempLayersOffset += size;
		}
	}


	int groupOffset = 0;
	int groupCounter = 0;

	while (groupOffset < projSize)
	{
		unsigned int groupEndOff = CharArrayToLong(&proj[groupOffset + 0]);

		if (groupEndOff == 0xFFFFFFFF)
			break;

		unsigned short groupId = CharArrayToShort(&proj[groupOffset + 4]);
		int type = CharArrayToShort(&proj[groupOffset + 6]);
		unsigned int soundMacroIdsOff = CharArrayToLong(&proj[groupOffset + 8]);
		unsigned int samplIdsOff = CharArrayToLong(&proj[groupOffset + 0xC]);
		unsigned int tableIdsOff = CharArrayToLong(&proj[groupOffset + 0x10]);
		unsigned int keymapIdsOff = CharArrayToLong(&proj[groupOffset + 0x14]);
		unsigned int layerIdsOff = CharArrayToLong(&proj[groupOffset + 0x18]);
		unsigned int pageTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
		unsigned int sfxTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
		unsigned int drumTableOff = CharArrayToLong(&proj[groupOffset + 0x20]);
		unsigned int midiSetupsOff = CharArrayToLong(&proj[groupOffset + 0x24]);

		bool absOffs = false;
		unsigned long subDataOff = absOffs ? groupOffset : groupOffset + 8;

		CString typeStr;
		if (type == 0)
		{
			typeStr = "SONG";
			//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X Page Table %08X Drum Page Table %08X Midi Setups %08X\n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff, subDataOff + drumTableOff, subDataOff + midiSetupsOff);
		}
		else
		{
			typeStr = "SFX ";
			//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X SFX Table Offset %08X \n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff);
		}
		
		if (type == 0) // Song
        {
			std::map<int, Factor5Page> instrumentPages; // by program number
			std::map<int, Factor5Page> drumPages; // by program number
			std::map<int, Factor5SongArray> songInfo; // by Song id

			unsigned int tempPageTableOff = subDataOff + pageTableOff;

			//fprintf(outProj, "\nPage Info\n");

			while (CharArrayToShort(&proj[tempPageTableOff]) != 0xFFFF)
			{
				Factor5Page page;
				page.objId = CharArrayToShort(&proj[tempPageTableOff]);
				page.priority = proj[tempPageTableOff + 2];
				page.maxVoices = proj[tempPageTableOff + 3];
				page.unk = proj[tempPageTableOff + 4];
				page.programNo = proj[tempPageTableOff + 5];
				page.pad = CharArrayToShort(&proj[tempPageTableOff + 6]);
				instrumentPages[page.programNo] = page;

				//fprintf(outProj, "%08X: Page: ObjId %04X Priority %02X Max Voices %02X Unknown %02X Program Number %02X\n", tempPageTableOff, objId, priority, maxVoices, unk, programNo);
		
				tempPageTableOff += 8;
			}

			tempPageTableOff = subDataOff + drumTableOff;

			//fprintf(outProj, "\nDrum Page Info\n");

			while (CharArrayToShort(&proj[tempPageTableOff]) != 0xFFFF)
			{
				Factor5Page page;
				page.objId = CharArrayToShort(&proj[tempPageTableOff]);
				page.priority = proj[tempPageTableOff + 2];
				page.maxVoices = proj[tempPageTableOff + 3];
				page.unk = proj[tempPageTableOff + 4];
				page.programNo = proj[tempPageTableOff + 5];
				page.pad = CharArrayToShort(&proj[tempPageTableOff + 6]);
				drumPages[page.programNo] = page;

				//fprintf(outProj, "%08X: Drum Page: ObjId %04X Priority %02X Max Voices %02X Unknown %02X Program Number %02X\n", tempPageTableOff, objId, priority, maxVoices, unk, programNo);

				tempPageTableOff += 8;
			}

			unsigned long tempMidiSetupData = subDataOff + midiSetupsOff;

			//fprintf(outProj, "\nSong Info\n");
			while (CharArrayToLong(&proj[tempMidiSetupData]) != 0xFFFFFFFF)
            {
				Factor5SongArray songArray;

				songArray.songId = CharArrayToShort(&proj[tempMidiSetupData]);

				//fprintf(outProj, "%08X: Song Id %04X\n", tempMidiSetupData, songId);

				for (int y = 0; y < 16; y++)
				{
					songArray.songInfo[y].programNo = proj[tempMidiSetupData + 4 + (y * 8)];
					songArray.songInfo[y].volume = proj[tempMidiSetupData + 4 + (y * 8) + 1];
					songArray.songInfo[y].panning = proj[tempMidiSetupData + 4 + (y * 8) + 2];
					songArray.songInfo[y].reverb = proj[tempMidiSetupData + 4 + (y * 8) + 3];
					songArray.songInfo[y].chorus = proj[tempMidiSetupData + 4 + (y * 8) + 4];
					// pad 3

					//fprintf(outProj, "#%02X Program Number %02X Volume %02X Panning %02X Reverb %02X Chorus %02X\n", y, programNo, volume, panning, reverb, chorus);
				}


				songInfo[songArray.songId] = songArray;

                tempMidiSetupData += 8 * 16 + 4;
            }

			for (map<int, Factor5SongArray>::iterator iter = songInfo.begin(); iter != songInfo.end(); iter++)
			{
				int songId = iter->first;
				if (songId == matchSongId)
				{
					if (alBankInstr == NULL)
						return E_POINTER;

					CWaveFile waveFile;

					int writeBank = 0;
					Factor5SongArray songArray = iter->second;

					CString tempStr;

					for (int channel = 0; channel < 0x10; channel++)
					{
						int programNumber = songArray.songInfo[channel].programNo;

						//int bankNumber = 0;
						Factor5Page* page;
						if (channel == 9)
						{
							if (drumPages.find(programNumber) != drumPages.end())
							{
								//bankNumber = F_INSTRUMENT_DRUMS;
								page = &drumPages[programNumber];
							}
							else
							{
								continue;
							}
						}
						else
						{
							if (instrumentPages.find(programNumber) != instrumentPages.end())
							{
								page = &instrumentPages[programNumber];
							}
							else
							{
								continue;
							}
						}
						
						int objectId = page->objId;

						Factor5ADSR* adsr = NULL;
						Factor5KeymapGroup* keymapGroup = NULL;
						Factor5LayerGroup* layerGroup = NULL;
						Factor5SoundMacroList* soundMacroList = NULL;

						if (poolTables.find(objectId) != poolTables.end())
						{
							adsr = &poolTables[objectId];
						}

						if (poolKeyGroups.find(objectId) != poolKeyGroups.end())
						{
							keymapGroup = &poolKeyGroups[objectId];
						}

						if (poolLayers.find(objectId) != poolLayers.end())
						{
							layerGroup = &poolLayers[objectId];
						}

						if (poolMacroList.find(objectId) != poolMacroList.end())
						{
							soundMacroList = &poolMacroList[objectId];
						}

						int trueSoundCount = 0;
						if (layerGroup != NULL)
						{
							for (int layer = (layerGroup->factor5Layers.size() - 1); layer >= 0; layer--)
							{
								if (layerGroup->factor5Layers[layer].objectId != 0xFFFF)
								{
									Factor5Layer layerData = layerGroup->factor5Layers[layer];
									
									std::vector<Factor5SoundMacro> soundMacros = poolMacroList[layerData.objectId].soundMacros;

									int sampleId = -1;
									unsigned long attackTime = 0;
									unsigned long decayTime = 0;
									float sustainPercentage = 0.0;
									unsigned long releaseTime = 0;
									unsigned char macroPan = 0x40;

									ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

									int foundSampleId = -1;
									for (int s = 0; s < alBankInstr->count; s++)
									{
										if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
										{
											// sfx id check
											if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
											{
												foundSampleId = s;
											}
										}
									}

									if (foundSampleId == -1)
									{
									}
									else
									{
										trueSoundCount++;
									}
								}
							}
						}
						else if (keymapGroup != NULL)
						{
							for (int keyNumber = 0; keyNumber < 128; keyNumber++)
							{
								if (keymapGroup->keys[keyNumber].objectId != 0xFFFF)
								{
									Factor5Keymap keyMap = keymapGroup->keys[keyNumber];
									
									std::vector<Factor5SoundMacro> soundMacros = poolMacroList[keyMap.objectId].soundMacros;

									int sampleId = -1;
									unsigned long attackTime = 0;
									unsigned long decayTime = 0;
									float sustainPercentage = 0.0;
									unsigned long releaseTime = 0;
									unsigned char macroPan = 0x40;

									ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

									int foundSampleId = -1;
									for (int s = 0; s < alBankInstr->count; s++)
									{
										if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
										{
											// sfx id check
											if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
											{
												foundSampleId = s;
											}
										}
									}

									if (foundSampleId == -1)
									{
									}
									else
									{
										trueSoundCount++;
									}
								}
							}
						}
						else if (soundMacroList != NULL)
						{
							int sampleId = -1;
							unsigned long attackTime = 0;
							unsigned long decayTime = 0;
							float sustainPercentage = 0.0;
							unsigned long releaseTime = 0;
							unsigned char macroPan = 0x40;

							ParseSoundMacroList(soundMacroList->soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

							int foundSampleId = -1;
							for (int s = 0; s < alBankInstr->count; s++)
							{
								if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
								{
									// sfx id check
									if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
									{
										foundSampleId = s;
									}
								}
							}

							if (foundSampleId == -1)
							{
							}
							else
							{
								trueSoundCount++;
							}
						}

						if (trueSoundCount == 0)
							continue;

						int bankNumber = floor((float)channel / (float)0x80);
						unsigned long patchNumber = channel % 0x80;

						unsigned long dlIdInstrument = 0;
						if (FAILED(hr = mPortDownload->GetDLId(&dlIdInstrument, 1)))
						{
							return hr;
						}

						void *dataInstrument = NULL;
						unsigned long sizeDataInstrument = 0;

						unsigned long sizeofInstrument = sizeof(DMUS_DOWNLOADINFO) + (sizeof(unsigned long) * (1 + trueSoundCount * 3)) + sizeof(DMUS_INSTRUMENT) + ((sizeof(DMUS_REGION) + sizeof(DMUS_ARTICULATION) + sizeof(DMUS_ARTICPARAMS)) * trueSoundCount);

						IDirectMusicDownload8* instrumentMusicDownload = NULL;
						if (FAILED(hr = mPortDownload->AllocateBuffer(sizeofInstrument,
							&instrumentMusicDownload)))
						{
							return hr;
						}

						if (FAILED(hr = instrumentMusicDownload->GetBuffer(&dataInstrument, &sizeDataInstrument)))
						{
							if (instrumentMusicDownload != NULL)
								instrumentMusicDownload->Release();
							return hr;
						}

						unsigned char* dataInstrumentByteBuffer = (unsigned char*)dataInstrument;
						ZeroMemory(dataInstrumentByteBuffer, sizeofInstrument);

						int currentDataInstBufferOffset = 0;

						DMUS_DOWNLOADINFO* dlInfo = (DMUS_DOWNLOADINFO*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
						dlInfo->dwDLType = DMUS_DOWNLOADINFO_INSTRUMENT;
						dlInfo->cbSize = sizeofInstrument;
						dlInfo->dwDLId = dlIdInstrument;

						dlInfo->dwNumOffsetTableEntries = 1 + (trueSoundCount * 3);

						currentDataInstBufferOffset += sizeof(DMUS_DOWNLOADINFO);


						unsigned long* ulOffsetTable = (unsigned long*)(&(dataInstrumentByteBuffer[currentDataInstBufferOffset]));

						currentDataInstBufferOffset += (sizeof(unsigned long) * (1 + (trueSoundCount * 3)));

						ulOffsetTable[0] = currentDataInstBufferOffset;

						DMUS_INSTRUMENT* instrument = (DMUS_INSTRUMENT*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);

						currentDataInstBufferOffset += sizeof(DMUS_INSTRUMENT);

						instrument->ulPatch = ((bankNumber << 8) | patchNumber);
						instrument->ulFirstRegionIdx = 1;
						instrument->ulGlobalArtIdx = 0; // No global

						int trueSound = 0;

						if (layerGroup != NULL)
						{
							for (int layer = (layerGroup->factor5Layers.size() - 1); layer >= 0; layer--)
							{
								if (layerGroup->factor5Layers[layer].objectId != 0xFFFF)
								{
									Factor5Layer layerData = layerGroup->factor5Layers[layer];
									
									std::vector<Factor5SoundMacro> soundMacros = poolMacroList[layerData.objectId].soundMacros;

									int sampleId = -1;
									unsigned long attackTime = 0;
									unsigned long decayTime = 0;
									float sustainPercentage = 0.0;
									unsigned long releaseTime = 0;
									unsigned char macroPan = 0x40;

									ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

									int foundSampleId = -1;
									for (int s = 0; s < alBankInstr->count; s++)
									{
										if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
										{
											// sfx id check
											if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
											{
												foundSampleId = s;
											}
										}
									}

									if (foundSampleId == -1)
									{
										continue;
									}
									else
									{
										sampleId = foundSampleId;
									}

									CString wavPath;
									wavPath.Format("%sSnd%02X_%04X_%04X.wav", mainFolder, writeBank, channel, trueSound);

									float sampleRate = (float)alBankInstr->samplerate;

									sampleRate = alBankInstr->inst[sampleId]->sounds[0]->wav.sampleRateNotInDefaultNintendoSpec;

									if (overrideSamplingRate)
										sampleRate = samplingRate;

									if (halfSamplingRate)
										sampleRate /= 2.0;
									bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, sampleId, 0, wavPath, sampleRate, PRIMARY, false);

									CStringW wavPathW(wavPath);
									LPWSTR wavPathStr = wavPathW.GetBuffer(0);

									WAVEFORMATEX waveFormatEx;

									if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
									{
										continue;
									}

									wavPathW.ReleaseBuffer();

									unsigned long append = 0;
									if (FAILED(hr = mPortDownload->GetAppend(&append)))
									{
										instrumentMusicDownload->Release();

										return hr;
									}

									IDirectMusicDownload8* waveMusicDownload = NULL;
									if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
										append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
									{
										instrumentMusicDownload->Release();
										if (waveMusicDownload != NULL)
											waveMusicDownload->Release();
										return hr;
									}

									void *waveDataBuffer = NULL;
									unsigned long sizeWaveBuffer = 0;

									if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
									ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

									unsigned long dlIdWave = 0;
									if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
									waveDownload->dlInfo.cbSize = sizeWaveBuffer;
									waveDownload->dlInfo.dwDLId = dlIdWave;
									waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

									waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
									waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
									waveDownload->dmWave.ulWaveDataIdx = 1;

									waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

									if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									waveFile.Close();

									if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
									{
										mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
									}
									else
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									::DeleteFile(wavPath);



									ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
									DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_REGION);

									ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
									DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

									ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
									DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

									if (trueSound == (trueSoundCount - 1))
										region->ulNextRegionIdx = 0;
									else
										region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

									region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

									long tcAttack = 0;
									long ptSustain = 0;
									long tcRelease = 0;
									long tcDecay = 0;

									float attackPercentage = 1.0f;
									float decayPercentage = sustainPercentage / 100.0f;

									tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
									
									if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
										tcDecay = 0x7FFFFFFF;
									else
										tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

									ptSustain = PercentToUnits(decayPercentage * 100.0);
									tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

									articParams->PitchEG.tcAttack = tcAttack;
									articParams->PitchEG.tcDecay = tcDecay;
									articParams->PitchEG.ptSustain = ptSustain;
									articParams->PitchEG.tcRelease = tcRelease;

									articParams->VolEG.tcAttack = tcAttack;
									articParams->VolEG.tcDecay = tcDecay;
									articParams->VolEG.ptSustain = ptSustain;
									articParams->VolEG.tcRelease = tcRelease;



									signed long pan = 0;
									macroPan = layerData.pan;

									double percentPan;
									if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x40)
										percentPan = 0;
									else if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x7F)
										percentPan = 50;
									else if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x00)
										percentPan = -50;
									else
									{
										float panFloat = ((float)macroPan / (float)0x7F) * 100.0;
										percentPan = (panFloat - 50.0);
									}
									articParams->Misc.ptDefaultPan = PercentToUnits(percentPan);



									char cSampleLoops = 0;
									ULONG ulLoopStart = 0;
									ULONG ulLoopLength = 0;
									ULONG ulLoopType = 0;

									
									if (alBankInstr->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
									{
										if ((alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
											&& (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
										{
											if (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
											{
												region->WSMP.cSampleLoops = 1;
												region->WLOOP[0].ulStart = alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
												region->WLOOP[0].ulLength = (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);

											}
										}
									}

									region->WLOOP[0].cbSize = sizeof(WLOOP);
									region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;

									articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

									long volumeAttenuation = 0;
									unsigned char keyBase = 0x3C;
									signed short fineTune = 0;

									int volume = layerData.volume;


									float volumePercentage = (float)volume / (float)0x7F;
									if (volume >= 0x7F)
									{
										volumeAttenuation = 0x00000000;
									}
									else if (volumePercentage != 0)
									{
										double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
										volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
									}
									else
									{
										volumeAttenuation = 0x7FFFFF00;
									}

									region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
									region->WaveLink.ulChannel = 1;
									region->WaveLink.ulTableIndex = dlIdWave;
									region->WaveLink.usPhaseGroup = 0;
									region->WSMP.cbSize = sizeof(WSMPL);
									region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
									region->WSMP.sFineTune = 0;

									region->WSMP.usUnityNote = alBankInstr->inst[sampleId]->sounds[0]->key.keybase;
									region->WSMP.usUnityNote -= layerData.transpose;
									region->WSMP.sFineTune = 0;

									region->RangeKey.usHigh = layerData.keyHi;
									region->RangeKey.usLow = layerData.keyLo;

									if (layer != 0)
									{
										if (layerGroup->factor5Layers[layer-1].keyLo == layerData.keyHi)
											region->RangeKey.usHigh--;
									}

									region->RangeVelocity.usHigh = 0x7F;
									region->RangeVelocity.usLow = 0;

									trueSound++;
								}
							}
						}
						else if (keymapGroup != NULL)
						{
							for (int keyNumber = 0; keyNumber < 128; keyNumber++)
							{
								if (keymapGroup->keys[keyNumber].objectId != 0xFFFF)
								{
									Factor5Keymap keyMap = keymapGroup->keys[keyNumber];
									
									std::vector<Factor5SoundMacro> soundMacros = poolMacroList[keyMap.objectId].soundMacros;

									int sampleId = -1;
									unsigned long attackTime = 0;
									unsigned long decayTime = 0;
									float sustainPercentage = 0.0;
									unsigned long releaseTime = 0;
									unsigned char macroPan = 0x40;

									ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

									int foundSampleId = -1;
									for (int s = 0; s < alBankInstr->count; s++)
									{
										if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
										{
											// sfx id check
											if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
											{
												foundSampleId = s;
											}
										}
									}

									if (foundSampleId == -1)
									{
										continue;
									}
									else
									{
										sampleId = foundSampleId;
									}

									CString wavPath;
									wavPath.Format("%sSnd%02X_%04X_%04X.wav", mainFolder, writeBank, channel, trueSound);

									float sampleRate = (float)alBankInstr->samplerate;

									sampleRate = alBankInstr->inst[sampleId]->sounds[0]->wav.sampleRateNotInDefaultNintendoSpec;

									if (overrideSamplingRate)
										sampleRate = samplingRate;

									if (halfSamplingRate)
										sampleRate /= 2.0;
									bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, sampleId, 0, wavPath, sampleRate, PRIMARY, false);

									CStringW wavPathW(wavPath);
									LPWSTR wavPathStr = wavPathW.GetBuffer(0);

									WAVEFORMATEX waveFormatEx;

									if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
									{
										continue;
									}

									wavPathW.ReleaseBuffer();

									unsigned long append = 0;
									if (FAILED(hr = mPortDownload->GetAppend(&append)))
									{
										instrumentMusicDownload->Release();

										return hr;
									}

									IDirectMusicDownload8* waveMusicDownload = NULL;
									if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
										append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
									{
										instrumentMusicDownload->Release();
										if (waveMusicDownload != NULL)
											waveMusicDownload->Release();
										return hr;
									}

									void *waveDataBuffer = NULL;
									unsigned long sizeWaveBuffer = 0;

									if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
									ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

									unsigned long dlIdWave = 0;
									if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
									waveDownload->dlInfo.cbSize = sizeWaveBuffer;
									waveDownload->dlInfo.dwDLId = dlIdWave;
									waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

									waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
									waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
									waveDownload->dmWave.ulWaveDataIdx = 1;

									waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

									if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									waveFile.Close();

									if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
									{
										mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
									}
									else
									{
										instrumentMusicDownload->Release();
										waveMusicDownload->Release();
										return hr;
									}

									::DeleteFile(wavPath);



									ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
									DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_REGION);

									ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
									DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

									ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
									DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
									currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

									if (trueSound == (trueSoundCount - 1))
										region->ulNextRegionIdx = 0;
									else
										region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

									region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

									long tcAttack = 0;
									long ptSustain = 0;
									long tcRelease = 0;
									long tcDecay = 0;

									float attackPercentage = 1.0f;
									float decayPercentage = sustainPercentage / 100.0f;

									tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
									
									if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
										tcDecay = 0x7FFFFFFF;
									else
										tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

									ptSustain = PercentToUnits(decayPercentage * 100.0);
									tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

									articParams->PitchEG.tcAttack = tcAttack;
									articParams->PitchEG.tcDecay = tcDecay;
									articParams->PitchEG.ptSustain = ptSustain;
									articParams->PitchEG.tcRelease = tcRelease;

									articParams->VolEG.tcAttack = tcAttack;
									articParams->VolEG.tcDecay = tcDecay;
									articParams->VolEG.ptSustain = ptSustain;
									articParams->VolEG.tcRelease = tcRelease;



									signed long pan = 0;
									macroPan = keyMap.pan;

									double percentPan;
									if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x40)
										percentPan = 0;
									else if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x7F)
										percentPan = 50;
									else if (alBankInstr->inst[sampleId]->sounds[0]->samplePan == 0x00)
										percentPan = -50;
									else
									{
										float panFloat = ((float)macroPan / (float)0x7F) * 100.0;
										percentPan = (panFloat - 50.0);
									}
									articParams->Misc.ptDefaultPan = PercentToUnits(percentPan);



									char cSampleLoops = 0;
									ULONG ulLoopStart = 0;
									ULONG ulLoopLength = 0;
									ULONG ulLoopType = 0;

									
									if (alBankInstr->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
									{
										if ((alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
											&& (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
										{
											if (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
											{
												region->WSMP.cSampleLoops = 1;
												region->WLOOP[0].ulStart = alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
												region->WLOOP[0].ulLength = (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);

											}
										}
									}

									region->WLOOP[0].cbSize = sizeof(WLOOP);
									region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;

									articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

									long volumeAttenuation = 0;
									unsigned char keyBase = 0x3C;
									signed short fineTune = 0;

									int volume = 0x7F;


									float volumePercentage = (float)volume / (float)0x7F;
									if (volume >= 0x7F)
									{
										volumeAttenuation = 0x00000000;
									}
									else if (volumePercentage != 0)
									{
										double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
										volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
									}
									else
									{
										volumeAttenuation = 0x7FFFFF00;
									}

									region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
									region->WaveLink.ulChannel = 1;
									region->WaveLink.ulTableIndex = dlIdWave;
									region->WaveLink.usPhaseGroup = 0;
									region->WSMP.cbSize = sizeof(WSMPL);
									region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
									region->WSMP.sFineTune = 0;

									region->WSMP.usUnityNote = alBankInstr->inst[sampleId]->sounds[0]->key.keybase;
									region->WSMP.usUnityNote -= keyMap.transpose;
									region->WSMP.sFineTune = 0;

									region->RangeKey.usHigh = keyNumber;
									region->RangeKey.usLow = keyNumber;

									region->RangeVelocity.usHigh = 0x7F;
									region->RangeVelocity.usLow = 0;

									trueSound++;
								}
							}
						}
						else if (soundMacroList != NULL)
						{
							int sampleId = -1;
							unsigned long attackTime = 0;
							unsigned long decayTime = 0;
							float sustainPercentage = 0.0;
							unsigned long releaseTime = 0;
							unsigned char macroPan = 0x40;

							ParseSoundMacroList(soundMacroList->soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

							int foundSampleId = -1;
							for (int s = 0; s < alBankInstr->count; s++)
							{
								if ((alBankInstr->inst[s] != NULL) && (alBankInstr->inst[s]->sounds[0] != NULL))
								{
									// sfx id check
									if (alBankInstr->inst[s]->sounds[0]->wav.unknown1 == sampleId)
									{
										foundSampleId = s;
									}
								}
							}

							if (foundSampleId == -1)
							{
								continue;
							}
							else
							{
								sampleId = foundSampleId;
							}

							CString wavPath;
							wavPath.Format("%sSnd%02X_%04X_%04X.wav", mainFolder, writeBank, channel, trueSound);

							float sampleRate = (float)alBankInstr->samplerate;

							sampleRate = alBankInstr->inst[sampleId]->sounds[0]->wav.sampleRateNotInDefaultNintendoSpec;

							if (overrideSamplingRate)
								sampleRate = samplingRate;

							if (halfSamplingRate)
								sampleRate /= 2.0;
							bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, sampleId, 0, wavPath, sampleRate, PRIMARY, false);

							CStringW wavPathW(wavPath);
							LPWSTR wavPathStr = wavPathW.GetBuffer(0);

							WAVEFORMATEX waveFormatEx;

							if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
							{
								continue;
							}

							wavPathW.ReleaseBuffer();

							unsigned long append = 0;
							if (FAILED(hr = mPortDownload->GetAppend(&append)))
							{
								instrumentMusicDownload->Release();

								return hr;
							}

							IDirectMusicDownload8* waveMusicDownload = NULL;
							if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
								append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
							{
								instrumentMusicDownload->Release();
								if (waveMusicDownload != NULL)
									waveMusicDownload->Release();
								return hr;
							}

							void *waveDataBuffer = NULL;
							unsigned long sizeWaveBuffer = 0;

							if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
							{
								instrumentMusicDownload->Release();
								waveMusicDownload->Release();
								return hr;
							}

							WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
							ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

							unsigned long dlIdWave = 0;
							if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
							{
								instrumentMusicDownload->Release();
								waveMusicDownload->Release();
								return hr;
							}

							waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
							waveDownload->dlInfo.cbSize = sizeWaveBuffer;
							waveDownload->dlInfo.dwDLId = dlIdWave;
							waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

							waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
							waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
							waveDownload->dmWave.ulWaveDataIdx = 1;

							waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

							if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
							{
								instrumentMusicDownload->Release();
								waveMusicDownload->Release();
								return hr;
							}

							waveFile.Close();

							if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
							{
								mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
							}
							else
							{
								instrumentMusicDownload->Release();
								waveMusicDownload->Release();
								return hr;
							}

							::DeleteFile(wavPath);



							ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
							DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
							currentDataInstBufferOffset += sizeof(DMUS_REGION);

							ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
							DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
							currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

							ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
							DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
							currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

							if (trueSound == (trueSoundCount - 1))
								region->ulNextRegionIdx = 0;
							else
								region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

							region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

							long tcAttack = 0;
							long ptSustain = 0;
							long tcRelease = 0;
							long tcDecay = 0;

							float attackPercentage = 1.0f;
							float decayPercentage = sustainPercentage / 100.0f;

							tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
							
							if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
								tcDecay = 0x7FFFFFFF;
							else
								tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

							ptSustain = PercentToUnits(decayPercentage * 100.0);
							tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

							articParams->PitchEG.tcAttack = tcAttack;
							articParams->PitchEG.tcDecay = tcDecay;
							articParams->PitchEG.ptSustain = ptSustain;
							articParams->PitchEG.tcRelease = tcRelease;

							articParams->VolEG.tcAttack = tcAttack;
							articParams->VolEG.tcDecay = tcDecay;
							articParams->VolEG.ptSustain = ptSustain;
							articParams->VolEG.tcRelease = tcRelease;

							double percentPan = 0;
							articParams->Misc.ptDefaultPan = PercentToUnits(percentPan);



							char cSampleLoops = 0;
							ULONG ulLoopStart = 0;
							ULONG ulLoopLength = 0;
							ULONG ulLoopType = 0;

							
							if (alBankInstr->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
							{
								if ((alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
									&& (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
								{
									if (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
									{
										region->WSMP.cSampleLoops = 1;
										region->WLOOP[0].ulStart = alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
										region->WLOOP[0].ulLength = (alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankInstr->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);

									}
								}
							}

							region->WLOOP[0].cbSize = sizeof(WLOOP);
							region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;

							articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

							long volumeAttenuation = 0;
							unsigned char keyBase = 0x3C;
							signed short fineTune = 0;

							int volume = 0x7F;


							float volumePercentage = (float)volume / (float)0x7F;
							if (volume >= 0x7F)
							{
								volumeAttenuation = 0x00000000;
							}
							else if (volumePercentage != 0)
							{
								double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
								volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
							}
							else
							{
								volumeAttenuation = 0x7FFFFF00;
							}

							region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
							region->WaveLink.ulChannel = 1;
							region->WaveLink.ulTableIndex = dlIdWave;
							region->WaveLink.usPhaseGroup = 0;
							region->WSMP.cbSize = sizeof(WSMPL);
							region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
							region->WSMP.sFineTune = 0;

							region->WSMP.usUnityNote = alBankInstr->inst[sampleId]->sounds[0]->key.keybase;
							region->WSMP.sFineTune = 0;

							region->RangeKey.usHigh = 0x7F;
							region->RangeKey.usLow = 0;

							region->RangeVelocity.usHigh = 0x7F;
							region->RangeVelocity.usLow = 0;

							trueSound++;
						}

						if (trueSound > 0)
						{
							if (SUCCEEDED(hr = mPortDownload->Download(instrumentMusicDownload)))
							{
								mInstrumentGeneratedLoaded.push_back(instrumentMusicDownload);
							}
							else
							{
								instrumentMusicDownload->Release();
							}
						}
					}
				}
			}
		}
		else // SFX
		{
			//fprintf(outProj, "\SFX\n");
			unsigned int tempSfxTableOff = subDataOff + sfxTableOff;

			unsigned short count = CharArrayToShort(&proj[tempSfxTableOff]);

			for (int y = 0; y < count; y++)
			{
				unsigned short defineId = CharArrayToShort(&proj[tempSfxTableOff + 4 + (y * 0xC)]);
				unsigned short objectId = CharArrayToShort(&proj[tempSfxTableOff + 4 + (y * 0xC) + 2]);
				unsigned char priority = proj[tempSfxTableOff + 4 + (y * 0xC) + 4];
				unsigned char maxVoices = proj[tempSfxTableOff + 4 + (y * 0xC) + 5];
				unsigned char volume = proj[tempSfxTableOff + 4 + (y * 0xC) + 6];
				unsigned char pan = proj[tempSfxTableOff + 4 + (y * 0xC) + 7];
				unsigned char key = proj[tempSfxTableOff + 4 + (y * 0xC) + 8];
				unsigned char unk = proj[tempSfxTableOff + 4 + (y * 0xC) + 9];

				//fprintf(outProj, "%08X: Define %04X ObjectId %04X Priority %02X MaxVoices %02X Volume %02X Pan %02X Key %02X Unkn %02X\n", tempSfxTableOff + 4 + (y * 0xC), defineId, objectId, priority, maxVoices, volume, pan, key, unk);
			}
		}

		//fprintf(outProj, "\nSound Macros Ids\n");
		unsigned long tempSoundMacroIdsOff = subDataOff + soundMacroIdsOff;
		while (CharArrayToShort(&proj[tempSoundMacroIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempSoundMacroIdsOff]));
			tempSoundMacroIdsOff += 4;
		}

		//fprintf(outProj, "\nTables Ids\n");
		unsigned long tempTablesIdsOff = subDataOff + tableIdsOff;
		while (CharArrayToShort(&proj[tempTablesIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempTablesIdsOff]));
			tempTablesIdsOff += 4;
		}

		//fprintf(outProj, "\nKeymap Ids\n");
		unsigned long tempKeymapsIdsOff = subDataOff + keymapIdsOff;
		while (CharArrayToShort(&proj[tempKeymapsIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempKeymapsIdsOff]));
			tempKeymapsIdsOff += 4;
		}

		//fprintf(outProj, "\nLayers Macros Ids\n");
		unsigned long tempLayersIdsOff = subDataOff + layerIdsOff;
		while (CharArrayToShort(&proj[tempLayersIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempLayersIdsOff]));
			tempLayersIdsOff += 4;
		}

		//fprintf(outProj, "\n");

		groupOffset += groupEndOff;
		groupCounter++;
	}





	delete [] proj;
	delete [] pool;

	return hr;
}

HRESULT CMidiPlayer::SetupMidiSoundBank(std::vector<ALBank*> alBanks, float timeMultiplier, bool halfSamplingRate, bool overrideSamplingRate, int samplingRate, std::vector<int> skipInstruments, bool combineBanks, CString gameMidiType, bool useT1Bank, int soundbankNumber, std::vector<t1Result> t1Results)
{
	HRESULT hr = S_OK;

	UnloadAllLoadedInstruments();

	CN64AIFCAudio n64Audio;

	for (int writeBank = 0; writeBank < alBanks.size(); writeBank++)
	{
		ALBank* alBankInstr = alBanks[writeBank];
	
		if (alBankInstr == NULL)
			return E_POINTER;

		CWaveFile waveFile;

		//int x = 0x3A;
		for (int x = 0; x < alBankInstr->count; x++)
		{
			int bankNumber = floor((float)x / (float)0x80);

			if (combineBanks && (gameMidiType == "Konami"))
				bankNumber += (writeBank * 2);

			if (std::find(skipInstruments.begin(), skipInstruments.end(), x) != skipInstruments.end())
				continue;

			int trueSoundCount = 0;
			for (int y = 0; y < alBankInstr->inst[x]->soundCount; y++)
			{
				if (alBankInstr->inst[x]->sounds[y] != NULL)
				{
					trueSoundCount++;

					if (alBankInstr->inst[x]->sounds[y]->hasWavePrevious)
					{
						trueSoundCount++;
					}
					
					if (alBankInstr->inst[x]->sounds[y]->hasWaveSecondary)
					{
						trueSoundCount++;
					}
				}
			}

			if (trueSoundCount == 0)
				continue;

			unsigned long dlIdInstrument = 0;
			if (FAILED(hr = mPortDownload->GetDLId(&dlIdInstrument, 1)))
			{
				return hr;
			}

			unsigned long patchNumber = x % 0x80;

			void *dataInstrument = NULL;
			unsigned long sizeDataInstrument = 0;

			unsigned long sizeofInstrument = sizeof(DMUS_DOWNLOADINFO) + (sizeof(unsigned long) * (1 + trueSoundCount * 3)) + sizeof(DMUS_INSTRUMENT) + ((sizeof(DMUS_REGION) + sizeof(DMUS_ARTICULATION) + sizeof(DMUS_ARTICPARAMS)) * trueSoundCount);

			IDirectMusicDownload8* instrumentMusicDownload = NULL;
			if (FAILED(hr = mPortDownload->AllocateBuffer(sizeofInstrument,
				&instrumentMusicDownload)))
			{
				return hr;
			}

			if (FAILED(hr = instrumentMusicDownload->GetBuffer(&dataInstrument, &sizeDataInstrument)))
			{
				if (instrumentMusicDownload != NULL)
					instrumentMusicDownload->Release();
				return hr;
			}

			unsigned char* dataInstrumentByteBuffer = (unsigned char*)dataInstrument;
			ZeroMemory(dataInstrumentByteBuffer, sizeofInstrument);

			int currentDataInstBufferOffset = 0;

			DMUS_DOWNLOADINFO* dlInfo = (DMUS_DOWNLOADINFO*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
			dlInfo->dwDLType = DMUS_DOWNLOADINFO_INSTRUMENT;
			dlInfo->cbSize = sizeofInstrument;
			dlInfo->dwDLId = dlIdInstrument;

			dlInfo->dwNumOffsetTableEntries = 1 + (trueSoundCount * 3);

			currentDataInstBufferOffset += sizeof(DMUS_DOWNLOADINFO);


			unsigned long* ulOffsetTable = (unsigned long*)(&(dataInstrumentByteBuffer[currentDataInstBufferOffset]));

			currentDataInstBufferOffset += (sizeof(unsigned long) * (1 + (trueSoundCount * 3)));

			ulOffsetTable[0] = currentDataInstBufferOffset;

			DMUS_INSTRUMENT* instrument = (DMUS_INSTRUMENT*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);

			currentDataInstBufferOffset += sizeof(DMUS_INSTRUMENT);

			instrument->ulPatch = ((bankNumber << 8) | patchNumber);
			instrument->ulFirstRegionIdx = 1;
			instrument->ulGlobalArtIdx = 0; // No global

			int trueSound = 0;

			for (int y = 0; y < alBankInstr->inst[x]->soundCount; y++)
			{
				if (alBankInstr->inst[x]->sounds[y] == NULL)
					continue;

				{
					CString wavPath;
					wavPath.Format("%sSnd%02X_%04X_%04X.wav", mainFolder, writeBank, x, y);

					float sampleRate = (float)alBankInstr->samplerate;

					if (alBankInstr->soundBankFormat == N64DD)
					{
						sampleRate = alBankInstr->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;
					}

					if (overrideSamplingRate)
						sampleRate = samplingRate;
					else if (useT1Bank)
					{
						for (int b = 0; b < t1Results.size(); b++)
						{
							for (int t = 0; t < t1Results[b].bank->t1Entries.size(); t++)
							{
								if (
									(t1Results[b].bank->t1Entries[t].bankNumber == soundbankNumber)
									&& (t1Results[b].bank->t1Entries[t].instrumentNumber == x)
									&& (t1Results[b].bank->t1Entries[t].soundNumber == y)
									)
								{
									sampleRate = t1Results[b].bank->t1Entries[t].frequency;
									break;
								}
							}
						}
					}

					if (halfSamplingRate)
						sampleRate /= 2.0;
					bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, x, y, wavPath, sampleRate, PRIMARY, false);

					CStringW wavPathW(wavPath);
					LPWSTR wavPathStr = wavPathW.GetBuffer(0);

					WAVEFORMATEX waveFormatEx;

					if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
					{
						continue;
					}

					wavPathW.ReleaseBuffer();

					unsigned long append = 0;
					if (FAILED(hr = mPortDownload->GetAppend(&append)))
					{
						instrumentMusicDownload->Release();

						return hr;
					}

					IDirectMusicDownload8* waveMusicDownload = NULL;
					if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
						append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
					{
						instrumentMusicDownload->Release();
						if (waveMusicDownload != NULL)
							waveMusicDownload->Release();
						return hr;
					}

					void *waveDataBuffer = NULL;
					unsigned long sizeWaveBuffer = 0;

					if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
					ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

					unsigned long dlIdWave = 0;
					if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
					waveDownload->dlInfo.cbSize = sizeWaveBuffer;
					waveDownload->dlInfo.dwDLId = dlIdWave;
					waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

					waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
					waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
					waveDownload->dmWave.ulWaveDataIdx = 1;

					waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

					if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveFile.Close();

					if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
					{
						mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
					}
					else
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					::DeleteFile(wavPath);



					ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
					DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_REGION);

					ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
					DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

					ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
					DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

					if (trueSound == (trueSoundCount - 1))
						region->ulNextRegionIdx = 0;
					else
						region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

					region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

					if (
							(alBankInstr->soundBankFormat == STANDARDFORMAT)
							|| (alBankInstr->soundBankFormat == CONKERFORMAT)
							|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
							|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
							|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
							|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
							|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
							|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
							|| (alBankInstr->soundBankFormat == TUROKFORMAT)
							|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
							|| (alBankInstr->soundBankFormat == TITUS)
							|| (alBankInstr->soundBankFormat == NINDEC)
							|| (alBankInstr->soundBankFormat == KONAMICTL)
							|| (alBankInstr->soundBankFormat == KONAMI8CTL)
							)
					{
						region->RangeKey.usHigh = alBankInstr->inst[x]->sounds[y]->key.keymax;
						region->RangeKey.usLow = alBankInstr->inst[x]->sounds[y]->key.keymin;
						region->RangeVelocity.usHigh = alBankInstr->inst[x]->sounds[y]->key.velocitymax;
						region->RangeVelocity.usLow = alBankInstr->inst[x]->sounds[y]->key.velocitymin;
					}
					else if (
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
									)
					{
						unsigned char checkPrevious = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
						unsigned char checkSecondary = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);

						region->RangeKey.usHigh = checkSecondary + 0x15;
						region->RangeKey.usLow = checkPrevious + 0x15;

						region->RangeVelocity.usHigh = 127;
						region->RangeVelocity.usLow = 0;
					}
					else if (
								(alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
								|| (alBankInstr->soundBankFormat == RNCCOMPRESSEDN64PTR)
								|| (alBankInstr->soundBankFormat == SNOW)
								|| (alBankInstr->soundBankFormat == SYDNEY)
								|| (alBankInstr->soundBankFormat == ARMYMENFORMAT)
								|| (alBankInstr->soundBankFormat == MEGAMAN64PTRV2)
								|| (alBankInstr->soundBankFormat == TAZHUFFMAN)
								)
					{
						region->RangeKey.usHigh = 127;
						region->RangeKey.usLow = 0;

						region->RangeVelocity.usHigh = 127;
						region->RangeVelocity.usLow = 0;
					}

					region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
					region->WaveLink.ulChannel = 1;
					region->WaveLink.ulTableIndex = dlIdWave;
					region->WaveLink.usPhaseGroup = 0;
					region->WSMP.cbSize = sizeof(WSMPL);
					region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
					region->WSMP.sFineTune = 0;

					if (
							(alBankInstr->soundBankFormat == STANDARDFORMAT)
							|| (alBankInstr->soundBankFormat == CONKERFORMAT)
							|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
							|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
							|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
							|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
							|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
							|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
							|| (alBankInstr->soundBankFormat == TUROKFORMAT)
							|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
							|| (alBankInstr->soundBankFormat == TITUS)
							|| (alBankInstr->soundBankFormat == NINDEC)
							|| (alBankInstr->soundBankFormat == KONAMICTL)
							|| (alBankInstr->soundBankFormat == KONAMI8CTL)
							)
					{
						region->WSMP.usUnityNote = alBankInstr->inst[x]->sounds[y]->key.keybase;
						region->WSMP.sFineTune = (signed short)alBankInstr->inst[x]->sounds[y]->key.detune;

						if ((alBankInstr->soundBankFormat == KONAMICTL) || (alBankInstr->soundBankFormat == KONAMI8CTL))
						{
							bool foundDrum = false;
							if (alBankInstr->konamiDrumsADSR.size() > 0)
							{
								for (int z = 0; z < alBankInstr->konamiDrumsADSR.size(); z++)
								{
									if (alBankInstr->konamiDrumsADSR[z].instrumentLookup == x)
									{
										region->WSMP.usUnityNote -= (signed char)alBankInstr->konamiDrumsADSR[z].coarseTune;
										region->WSMP.sFineTune = (signed char)alBankInstr->konamiDrumsADSR[z].fineTune;
										foundDrum = true;
										break;
									}
								}
							}
							if (!foundDrum)
							{
								if (alBankInstr->konamiADSR.size() > 0)
								{
									if (x < alBankInstr->konamiADSR.size())
									{
										region->WSMP.usUnityNote -= (signed char)alBankInstr->konamiADSR[x].coarseTune;
										region->WSMP.sFineTune = (signed char)alBankInstr->konamiADSR[x].fineTune;
									}
								}
							}
						}
						//region->WSMP.lAttenuation = 0;
					}
					else if (
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
							)
					{
						float keybaseFloat = *reinterpret_cast<float*> (&alBankInstr->inst[x]->sounds[y]->floatKeyBase);
						region->WSMP.usUnityNote = CN64AIFCAudio::ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (
								(alBankInstr->soundBankFormat == MUSYX)
								|| (alBankInstr->soundBankFormat == MUSYXLZ)
								|| (alBankInstr->soundBankFormat == MUSYXZLIB)
								|| (alBankInstr->soundBankFormat == MUSYXSMALLZLIB)
								)
					{
						region->WSMP.usUnityNote = alBankInstr->inst[x]->sounds[y]->key.keybase;
					}
					else if (
								(alBankInstr->soundBankFormat == N64DD)
								)
					{
						region->WSMP.usUnityNote = alBankInstr->inst[x]->sounds[y]->key.keybase;
						region->WSMP.sFineTune = (signed short)alBankInstr->inst[x]->sounds[y]->key.detune;
					}
					else if (
								((alBankInstr->soundBankFormat == SN64) || (alBankInstr->soundBankFormat == ZLIBSN64))
								)
					{
						region->WSMP.usUnityNote = alBankInstr->inst[x]->sounds[y]->key.keybase;
					}
					else if (
								(alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
								|| (alBankInstr->soundBankFormat == RNCCOMPRESSEDN64PTR)
								|| (alBankInstr->soundBankFormat == SNOW)
								|| (alBankInstr->soundBankFormat == SYDNEY)
								|| (alBankInstr->soundBankFormat == ARMYMENFORMAT)
								|| (alBankInstr->soundBankFormat == MEGAMAN64PTRV2)
								|| (alBankInstr->soundBankFormat == TAZHUFFMAN)
								)
					{
						region->WSMP.usUnityNote = 0x3C - (signed char)alBankInstr->inst[x]->volume;
						//region->WSMP.sFineTune = (signed char)alBankInstr->inst[x]->pan;
					}

					if ((alBankInstr->inst[x]->sounds[y]->wav.adpcmWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wav.adpcmWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wav.adpcmWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wav.adpcmWave->loop->end - alBankInstr->inst[x]->sounds[y]->wav.adpcmWave->loop->start + 1);
						}
					}
					else if ((alBankInstr->inst[x]->sounds[y]->wav.rawWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wav.rawWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wav.rawWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wav.rawWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wav.rawWave->loop->end - alBankInstr->inst[x]->sounds[y]->wav.rawWave->loop->start + 1);
						}
					}

					region->WLOOP[0].cbSize = sizeof(WLOOP);
					region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;


					articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

					//articParams->LFO.pcFrequency = PitchHzToCents((float)alBankInstr->samplerate);
					//articParams->LFO.tcDelay = TimeSecondsToCents(10 * timeMultiplier);
					//articParams->LFO.pcFrequency = PitchHzToCents(8.176);

					if (
							(alBankInstr->soundBankFormat == STANDARDFORMAT)
							|| (alBankInstr->soundBankFormat == CONKERFORMAT)
							|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
							|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
							|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
							|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
							|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
							|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
							|| (alBankInstr->soundBankFormat == TUROKFORMAT)
							|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
							|| (alBankInstr->soundBankFormat == TITUS)
							|| (alBankInstr->soundBankFormat == NINDEC)
							|| (alBankInstr->soundBankFormat == KONAMICTL)
							|| (alBankInstr->soundBankFormat == KONAMI8CTL)
							)
					{
						float decayPercentage = (float)(alBankInstr->inst[x]->sounds[y]->env.decayVolume) / (float)alBankInstr->inst[x]->sounds[y]->env.attackVolume;
						if (decayPercentage >= 0.99)
							decayPercentage = 1;

						articParams->PitchEG.tcAttack = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.attackTime * timeMultiplier);
						if (((signed long)alBankInstr->inst[x]->sounds[y]->env.decayTime == -1) || ((signed long)alBankInstr->inst[x]->sounds[y]->env.decayTime == 0))
							articParams->PitchEG.tcDecay = 0x7FFFFFFF;
						else
							articParams->PitchEG.tcDecay = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.decayTime * timeMultiplier);
						articParams->PitchEG.ptSustain = PercentToUnits(decayPercentage * 100.0);
						articParams->PitchEG.tcRelease = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.releaseTime * timeMultiplier);

						double percentPan;
						if (alBankInstr->inst[x]->sounds[y]->samplePan == 0x40)
							percentPan = 0;
						else if (alBankInstr->inst[x]->sounds[y]->samplePan == 0x7F)
							percentPan = 50;
						else if (alBankInstr->inst[x]->sounds[y]->samplePan == 0x00)
							percentPan = -50;
						else
						{
							float panFloat = ((float)alBankInstr->inst[x]->sounds[y]->samplePan / (float)0x7F) * 100.0;
							percentPan = (panFloat - 50.0);
						}

						articParams->Misc.ptDefaultPan = PercentToUnits(percentPan);

						articParams->VolEG.tcAttack = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.attackTime * timeMultiplier);
						if ((signed long)alBankInstr->inst[x]->sounds[y]->env.decayTime == -1)
							articParams->VolEG.tcDecay = 0x7FFFFFFF;
						else
							articParams->VolEG.tcDecay = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.decayTime * timeMultiplier);

						articParams->VolEG.ptSustain = PercentToUnits(decayPercentage * 100.0);
						articParams->VolEG.tcRelease = TimeSecondsToCents((signed long)alBankInstr->inst[x]->sounds[y]->env.releaseTime * timeMultiplier);
					}
					else if (	
								(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									/*|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)*/
							)
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] < 0))
							articParams->PitchEG.tcAttack = TimeSecondsToCents(0);
						else
							articParams->PitchEG.tcAttack = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));
						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] < 0))
						{
							articParams->PitchEG.tcDecay = 0x7FFFFFFF;
							articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						}
						else
						{
							articParams->PitchEG.tcDecay = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
							articParams->PitchEG.ptSustain = PercentToUnits(0.0);
						}
						if (alBankInstr->soundBankFormat == ZELDAFORMAT)
							articParams->PitchEG.tcRelease = TimeSecondsToCents(EADGameReleaseTimeSeconds[alBankInstr->inst[x]->sounds[y]->unknown1 & 0xFF]);
						else
							articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);
					}
					else // Not yet known, such as Zelda, N64WavPtr, etc
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						articParams->PitchEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->PitchEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);

						articParams->VolEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->VolEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->VolEG.ptSustain = PercentToUnits(100.0);
						articParams->VolEG.tcRelease = TimeSecondsToCents(0.5);
					}

					trueSound++;
				}






				if (alBankInstr->inst[x]->sounds[y]->hasWavePrevious)
				{
					CString wavPath;
					wavPath.Format("%sSnd%02X_%04X_%04X_Prev.wav", mainFolder, writeBank, x, y);

					float sampleRate = (float)alBankInstr->samplerate;
					if (overrideSamplingRate)
						sampleRate = samplingRate;

					if (halfSamplingRate)
						sampleRate /= 2.0;
					bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, x, y, wavPath, sampleRate, PREVIOUS, false);

					CStringW wavPathW(wavPath);
					LPWSTR wavPathStr = wavPathW.GetBuffer(0);

					WAVEFORMATEX waveFormatEx;

					if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
					{
						continue;
					}

					wavPathW.ReleaseBuffer();

					unsigned long append = 0;
					if (FAILED(hr = mPortDownload->GetAppend(&append)))
					{
						instrumentMusicDownload->Release();

						return hr;
					}

					IDirectMusicDownload8* waveMusicDownload = NULL;
					if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
						append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
					{
						instrumentMusicDownload->Release();
						if (waveMusicDownload != NULL)
							waveMusicDownload->Release();
						return hr;
					}

					void *waveDataBuffer = NULL;
					unsigned long sizeWaveBuffer = 0;

					if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
					ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

					unsigned long dlIdWave = 0;
					if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
					waveDownload->dlInfo.cbSize = sizeWaveBuffer;
					waveDownload->dlInfo.dwDLId = dlIdWave;
					waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

					waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
					waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
					waveDownload->dmWave.ulWaveDataIdx = 1;

					waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

					if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveFile.Close();

					if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
					{
						mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
					}
					else
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					::DeleteFile(wavPath);



					ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
					DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_REGION);

					ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
					DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

					ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
					DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

					if (trueSound == (trueSoundCount - 1))
						region->ulNextRegionIdx = 0;
					else
						region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

					region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

					if (
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
									)
					{
						unsigned char checkPrevious = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
						unsigned char checkSecondary = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);
						
						if (checkPrevious != 0x00)
						{
							region->RangeKey.usHigh = checkPrevious - 1 + 0x15;
							region->RangeKey.usLow = 0;
						}
						else
						{
							region->RangeKey.usHigh = 0;
							region->RangeKey.usLow = 0;
						}

						region->RangeVelocity.usHigh = 127;
						region->RangeVelocity.usLow = 0;
					}

					region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
					region->WaveLink.ulChannel = 1;
					region->WaveLink.ulTableIndex = dlIdWave;
					region->WaveLink.usPhaseGroup = 0;
					region->WSMP.cbSize = sizeof(WSMPL);
					region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
					region->WSMP.sFineTune = 0;

					if (
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
							)
					{
						float keybaseFloat = *reinterpret_cast<float*> (&alBankInstr->inst[x]->sounds[y]->floatKeyBase);
						region->WSMP.usUnityNote = CN64AIFCAudio::ConvertEADGameValueToKeyBase(keybaseFloat);
					}

					if ((alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end - alBankInstr->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start + 1);
						}
					}
					else if ((alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave->loop->end - alBankInstr->inst[x]->sounds[y]->wavPrevious.rawWave->loop->start + 1);
						}
					}

					region->WLOOP[0].cbSize = sizeof(WLOOP);
					region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;


					articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

					//articParams->LFO.pcFrequency = PitchHzToCents((float)alBankInstr->samplerate);
					//articParams->LFO.tcDelay = TimeSecondsToCents(10 * timeMultiplier);
					//articParams->LFO.pcFrequency = PitchHzToCents(8.176);

					if (
							(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									/*|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)*/
						)
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] < 0))
							articParams->PitchEG.tcAttack = TimeSecondsToCents(0);
						else
							articParams->PitchEG.tcAttack = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));
						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] < 0))
						{
							articParams->PitchEG.tcDecay = 0x7FFFFFFF;
							articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						}
						else
						{
							articParams->PitchEG.tcDecay = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
							articParams->PitchEG.ptSustain = PercentToUnits(0.0);
						}
						if (alBankInstr->soundBankFormat == ZELDAFORMAT)
							articParams->PitchEG.tcRelease = TimeSecondsToCents(EADGameReleaseTimeSeconds[alBankInstr->inst[x]->sounds[y]->unknown1 & 0xFF]);
						else
							articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);
					}
					else // Not yet known, such as Zelda, N64WavPtr, etc
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						articParams->PitchEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->PitchEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);

						articParams->VolEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->VolEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->VolEG.ptSustain = PercentToUnits(100.0);
						articParams->VolEG.tcRelease = TimeSecondsToCents(0.5);
					}

					trueSound++;
				}

				if (alBankInstr->inst[x]->sounds[y]->hasWaveSecondary)
				{
					CString wavPath;
					wavPath.Format("%sSnd%02X_%04X_%04X_Sec.wav", mainFolder, writeBank, x, y);

					float sampleRate = (float)alBankInstr->samplerate;
					if (overrideSamplingRate)
						sampleRate = samplingRate;

					if (halfSamplingRate)
						sampleRate /= 2.0;
					bool extractResult = n64Audio.ExtractRawSound(mainFolder, alBankInstr, x, y, wavPath, sampleRate, SECONDARY, false);

					CStringW wavPathW(wavPath);
					LPWSTR wavPathStr = wavPathW.GetBuffer(0);

					WAVEFORMATEX waveFormatEx;

					if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
					{
						continue;
					}

					wavPathW.ReleaseBuffer();

					unsigned long append = 0;
					if (FAILED(hr = mPortDownload->GetAppend(&append)))
					{
						instrumentMusicDownload->Release();

						return hr;
					}

					IDirectMusicDownload8* waveMusicDownload = NULL;
					if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
						append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
					{
						instrumentMusicDownload->Release();
						if (waveMusicDownload != NULL)
							waveMusicDownload->Release();
						return hr;
					}

					void *waveDataBuffer = NULL;
					unsigned long sizeWaveBuffer = 0;

					if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
					ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

					unsigned long dlIdWave = 0;
					if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
					waveDownload->dlInfo.cbSize = sizeWaveBuffer;
					waveDownload->dlInfo.dwDLId = dlIdWave;
					waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

					waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
					waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
					waveDownload->dmWave.ulWaveDataIdx = 1;

					waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

					if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					waveFile.Close();

					if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
					{
						mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
					}
					else
					{
						instrumentMusicDownload->Release();
						waveMusicDownload->Release();
						return hr;
					}

					::DeleteFile(wavPath);



					ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
					DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_REGION);

					ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
					DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

					ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
					DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
					currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

					if (trueSound == (trueSoundCount - 1))
						region->ulNextRegionIdx = 0;
					else
						region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

					region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

					if (
							(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
							|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
							|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
						)
					{
						unsigned char checkPrevious = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
						unsigned char checkSecondary = ((alBankInstr->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);
						
						if (checkSecondary != 0x7F)
						{
							region->RangeKey.usHigh = 127;
							region->RangeKey.usLow = checkSecondary + 1 + 0x15;
						}
						else
						{
							region->RangeKey.usHigh = 127;
							region->RangeKey.usLow = 127;
						}

						region->RangeVelocity.usHigh = 127;
						region->RangeVelocity.usLow = 0;
					}

					region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
					region->WaveLink.ulChannel = 1;
					region->WaveLink.ulTableIndex = dlIdWave;
					region->WaveLink.usPhaseGroup = 0;
					region->WSMP.cbSize = sizeof(WSMPL);
					region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
					region->WSMP.sFineTune = 0;

					if (
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
							)
					{
						float keybaseFloat = *reinterpret_cast<float*> (&alBankInstr->inst[x]->sounds[y]->floatKeyBase);
						region->WSMP.usUnityNote = CN64AIFCAudio::ConvertEADGameValueToKeyBase(keybaseFloat);
					}

					if ((alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end - alBankInstr->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start + 1);
						}
					}
					else if ((alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave != NULL)
						&& (alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave->loop != NULL))
					{
						if (alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave->loop->count > 0)
						{
							region->WSMP.cSampleLoops = 1;
							region->WLOOP[0].ulStart = alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave->loop->start;
							region->WLOOP[0].ulLength = (alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave->loop->end - alBankInstr->inst[x]->sounds[y]->wavSecondary.rawWave->loop->start + 1);
						}
					}

					region->WLOOP[0].cbSize = sizeof(WLOOP);
					region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;

					articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

					//articParams->LFO.pcFrequency = PitchHzToCents((float)alBankInstr->samplerate);
					//articParams->LFO.tcDelay = TimeSecondsToCents(10 * timeMultiplier);
					//articParams->LFO.pcFrequency = PitchHzToCents(8.176);

					if (
							(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
									/*|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)*/
						)
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] < 0))
							articParams->PitchEG.tcAttack = TimeSecondsToCents(0);
						else
							articParams->PitchEG.tcAttack = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));
						if ((alBankInstr->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] < 0))
						{
							articParams->PitchEG.tcDecay = 0x7FFFFFFF;
							articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						}
						else
						{
							articParams->PitchEG.tcDecay = TimeSecondsToCents(((float)alBankInstr->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
							articParams->PitchEG.ptSustain = PercentToUnits(0.0);
						}
						if (alBankInstr->soundBankFormat == ZELDAFORMAT)
							articParams->PitchEG.tcRelease = TimeSecondsToCents(EADGameReleaseTimeSeconds[alBankInstr->inst[x]->sounds[y]->unknown1 & 0xFF]);
						else
							articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);
					}
					else // Not yet known, such as Zelda, N64WavPtr, etc
					{
						articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

						articParams->PitchEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->PitchEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->PitchEG.ptSustain = PercentToUnits(100.0);
						articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);

						articParams->VolEG.tcAttack = TimeSecondsToCents(0.001);
						articParams->VolEG.tcDecay = TimeSecondsToCents(0.001);
						articParams->VolEG.ptSustain = PercentToUnits(100.0);
						articParams->VolEG.tcRelease = TimeSecondsToCents(0.5);
					}

					trueSound++;
				}
			}

			/*if (trueSoundCount == 1)
			{
				int sizeofInstrDownload = sizeof(INSTRUMENT_DOWNLOAD);

				unsigned long offsetOfMusDownloadInfo = offsetof(INSTRUMENT_DOWNLOAD, dlInfo);
				unsigned long offsetOfOffsetTable = offsetof(INSTRUMENT_DOWNLOAD, ulOffsetTable);
				unsigned long offsetOfMusicInstrument = offsetof(INSTRUMENT_DOWNLOAD, dmInstrument);
				unsigned long offsetOfRegion = offsetof(INSTRUMENT_DOWNLOAD, dmRegion);
				unsigned long offsetOfArticulation = offsetof(INSTRUMENT_DOWNLOAD, dmArticulation);
				unsigned long offsetOfArticParams = offsetof(INSTRUMENT_DOWNLOAD, dmArticParams);

				
				INSTRUMENT_DOWNLOAD* instrDownloadTest = (INSTRUMENT_DOWNLOAD*)dataInstrumentByteBuffer;
				instrDownloadTest = instrDownloadTest;
			}
			else if (trueSoundCount == 4)
			{
				int sizeofInstrDownload = sizeof(INSTRUMENT_DOWNLOAD4);

				unsigned long offsetOfMusDownloadInfo = offsetof(INSTRUMENT_DOWNLOAD4, dlInfo);
				unsigned long offsetOfOffsetTable = offsetof(INSTRUMENT_DOWNLOAD4, ulOffsetTable);
				unsigned long offsetOfMusicInstrument = offsetof(INSTRUMENT_DOWNLOAD4, dmInstrument);
				unsigned long offsetOfRegion = offsetof(INSTRUMENT_DOWNLOAD4, dmRegion);
				unsigned long offsetOfArticulation = offsetof(INSTRUMENT_DOWNLOAD4, dmArticulation);
				unsigned long offsetOfArticParams = offsetof(INSTRUMENT_DOWNLOAD4, dmArticParams);

				INSTRUMENT_DOWNLOAD4* instrDownloadTest = (INSTRUMENT_DOWNLOAD4*)dataInstrumentByteBuffer;
				instrDownloadTest = instrDownloadTest;
			}*/

			if (SUCCEEDED(hr = mPortDownload->Download(instrumentMusicDownload)))
			{
				mInstrumentGeneratedLoaded.push_back(instrumentMusicDownload);
			}
			else
			{
				instrumentMusicDownload->Release();
			}
		}

		if (alBankInstr->percussion != NULL)
		{
			int trueSoundCount = 0;
			for (int y = 0; y < alBankInstr->percussion->soundCount; y++)
			{
				if (alBankInstr->percussion->sounds[y] != NULL)
				{
					trueSoundCount++;

					if (alBankInstr->percussion->sounds[y]->hasWavePrevious)
					{
						trueSoundCount++;
					}
					
					if (alBankInstr->percussion->sounds[y]->hasWaveSecondary)
					{
						trueSoundCount++;
					}
				}
			}

			if (trueSoundCount != 0)
			{
				unsigned long dlIdInstrument = 0;
				if (FAILED(hr = mPortDownload->GetDLId(&dlIdInstrument, 1)))
				{
					return hr;
				}

				int bankNumber = 0;
				unsigned long patchNumber = 0;

				void *dataInstrument = NULL;
				unsigned long sizeDataInstrument = 0;

				unsigned long sizeofInstrument = sizeof(DMUS_DOWNLOADINFO) + (sizeof(unsigned long) * (1 + trueSoundCount * 3)) + sizeof(DMUS_INSTRUMENT) + ((sizeof(DMUS_REGION) + sizeof(DMUS_ARTICULATION) + sizeof(DMUS_ARTICPARAMS)) * trueSoundCount);

				IDirectMusicDownload8* instrumentMusicDownload = NULL;
				if (FAILED(hr = mPortDownload->AllocateBuffer(sizeofInstrument,
					&instrumentMusicDownload)))
				{
					return hr;
				}

				if (FAILED(hr = instrumentMusicDownload->GetBuffer(&dataInstrument, &sizeDataInstrument)))
				{
					if (instrumentMusicDownload != NULL)
						instrumentMusicDownload->Release();
					return hr;
				}

				unsigned char* dataInstrumentByteBuffer = (unsigned char*)dataInstrument;
				ZeroMemory(dataInstrumentByteBuffer, sizeofInstrument);

				int currentDataInstBufferOffset = 0;

				DMUS_DOWNLOADINFO* dlInfo = (DMUS_DOWNLOADINFO*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
				dlInfo->dwDLType = DMUS_DOWNLOADINFO_INSTRUMENT;
				dlInfo->cbSize = sizeofInstrument;
				dlInfo->dwDLId = dlIdInstrument;

				dlInfo->dwNumOffsetTableEntries = 1 + (trueSoundCount * 3);

				currentDataInstBufferOffset += sizeof(DMUS_DOWNLOADINFO);


				unsigned long* ulOffsetTable = (unsigned long*)(&(dataInstrumentByteBuffer[currentDataInstBufferOffset]));

				currentDataInstBufferOffset += (sizeof(unsigned long) * (1 + (trueSoundCount * 3)));

				ulOffsetTable[0] = currentDataInstBufferOffset;

				DMUS_INSTRUMENT* instrument = (DMUS_INSTRUMENT*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);

				currentDataInstBufferOffset += sizeof(DMUS_INSTRUMENT);

				instrument->ulPatch = F_INSTRUMENT_DRUMS | ((bankNumber << 8) | patchNumber);
				instrument->ulFirstRegionIdx = 1;
				instrument->ulGlobalArtIdx = 0; // No global

				int trueSound = 0;

				for (int y = 0; y < alBankInstr->percussion->soundCount; y++)
				{
					if (alBankInstr->percussion->sounds[y] == NULL)
						continue;

					{
						CString wavPath;
						wavPath.Format("%sSnd%02X_%04X_Perc.wav", mainFolder, writeBank, y);

						float sampleRate = (float)alBankInstr->samplerate;

						if (alBankInstr->soundBankFormat == N64DD)
						{
							sampleRate = alBankInstr->percussion->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;
						}

						if (overrideSamplingRate)
							sampleRate = samplingRate;

						if (halfSamplingRate)
							sampleRate /= 2.0;
						bool extractResult = n64Audio.ExtractPercussion(mainFolder, alBankInstr, y, wavPath, sampleRate, false);

						CStringW wavPathW(wavPath);
						LPWSTR wavPathStr = wavPathW.GetBuffer(0);

						WAVEFORMATEX waveFormatEx;

						if (FAILED(hr = waveFile.Open(wavPathStr, &waveFormatEx, WAVEFILE_READ)))
						{
							continue;
						}

						wavPathW.ReleaseBuffer();

						unsigned long append = 0;
						if (FAILED(hr = mPortDownload->GetAppend(&append)))
						{
							instrumentMusicDownload->Release();

							return hr;
						}

						IDirectMusicDownload8* waveMusicDownload = NULL;
						if (FAILED(hr = mPortDownload->AllocateBuffer(sizeof(WAVE_DOWNLOAD) +
							append * waveFile.GetFormat()->nBlockAlign + waveFile.GetSize(), &waveMusicDownload)))
						{
							instrumentMusicDownload->Release();
							if (waveMusicDownload != NULL)
								waveMusicDownload->Release();
							return hr;
						}

						void *waveDataBuffer = NULL;
						unsigned long sizeWaveBuffer = 0;

						if (FAILED(hr = waveMusicDownload->GetBuffer(&waveDataBuffer, &sizeWaveBuffer)))
						{
							instrumentMusicDownload->Release();
							waveMusicDownload->Release();
							return hr;
						}

						WAVE_DOWNLOAD* waveDownload = (WAVE_DOWNLOAD*)waveDataBuffer;
						ZeroMemory(waveDownload, sizeof(WAVE_DOWNLOAD));

						unsigned long dlIdWave = 0;
						if (FAILED(hr = mPortDownload->GetDLId(&dlIdWave, 1)))
						{
							instrumentMusicDownload->Release();
							waveMusicDownload->Release();
							return hr;
						}

						waveDownload->dlInfo.dwDLType = DMUS_DOWNLOADINFO_WAVE;
						waveDownload->dlInfo.cbSize = sizeWaveBuffer;
						waveDownload->dlInfo.dwDLId = dlIdWave;
						waveDownload->dlInfo.dwNumOffsetTableEntries = 2;

						waveDownload->ulOffsetTable[0] = offsetof(WAVE_DOWNLOAD, dmWave);
						waveDownload->ulOffsetTable[1] = offsetof(WAVE_DOWNLOAD, dmWaveData);
						waveDownload->dmWave.ulWaveDataIdx = 1;

						waveDownload->dmWave.WaveformatEx = *(waveFile.GetFormat());

						if (FAILED(hr = waveFile.Read(((WAVE_DOWNLOAD*)waveDataBuffer)->dmWaveData.byData, waveFile.GetSize(), &waveDownload->dmWaveData.cbSize)))
						{
							instrumentMusicDownload->Release();
							waveMusicDownload->Release();
							return hr;
						}

						waveFile.Close();

						if (SUCCEEDED(hr = mPortDownload->Download(waveMusicDownload)))
						{
							mInstrumentGeneratedLoaded.push_back(waveMusicDownload);
						}
						else
						{
							instrumentMusicDownload->Release();
							waveMusicDownload->Release();
							return hr;
						}

						::DeleteFile(wavPath);



						ulOffsetTable[1 + (trueSound * 3)] = currentDataInstBufferOffset;
						DMUS_REGION* region = (DMUS_REGION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
						currentDataInstBufferOffset += sizeof(DMUS_REGION);

						ulOffsetTable[1 + (trueSound * 3) + 1] = currentDataInstBufferOffset;
						DMUS_ARTICULATION* articulation = (DMUS_ARTICULATION*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
						currentDataInstBufferOffset += sizeof(DMUS_ARTICULATION);

						ulOffsetTable[1 + (trueSound * 3) + 2] = currentDataInstBufferOffset;
						DMUS_ARTICPARAMS* articParams = (DMUS_ARTICPARAMS*)(&dataInstrumentByteBuffer[currentDataInstBufferOffset]);
						currentDataInstBufferOffset += sizeof(DMUS_ARTICPARAMS);

						if (trueSound == (trueSoundCount - 1))
							region->ulNextRegionIdx = 0;
						else
							region->ulNextRegionIdx = 1 + ((trueSound + 1) * 3);

						region->ulRegionArtIdx = 1 + (trueSound * 3) + 1;

						if (
								(alBankInstr->soundBankFormat == STANDARDFORMAT)
								|| (alBankInstr->soundBankFormat == CONKERFORMAT)
								|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
								|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
								|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
								|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
								|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
								|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
								|| (alBankInstr->soundBankFormat == TUROKFORMAT)
								|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
								|| (alBankInstr->soundBankFormat == TITUS)
								|| (alBankInstr->soundBankFormat == NINDEC)
								|| (alBankInstr->soundBankFormat == KONAMICTL)
								|| (alBankInstr->soundBankFormat == KONAMI8CTL)
								)
						{
							region->RangeKey.usHigh = alBankInstr->percussion->sounds[y]->key.keymax;
							region->RangeKey.usLow = alBankInstr->percussion->sounds[y]->key.keymin;
							region->RangeVelocity.usHigh = alBankInstr->percussion->sounds[y]->key.velocitymax;
							region->RangeVelocity.usLow = alBankInstr->percussion->sounds[y]->key.velocitymin;
						}
						else if (
										(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
										|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
										|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
										)
						{
							unsigned char checkPrevious = ((alBankInstr->percussion->sounds[y]->unknown1 >> 16) & 0xFF);
							unsigned char checkSecondary = ((alBankInstr->percussion->sounds[y]->unknown1 >> 8) & 0xFF);

							region->RangeKey.usHigh = checkSecondary + 0x15;
							region->RangeKey.usLow = checkPrevious + 0x15;

							region->RangeVelocity.usHigh = 127;
							region->RangeVelocity.usLow = 0;
						}
						else if (
								(alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
								|| (alBankInstr->soundBankFormat == RNCCOMPRESSEDN64PTR)
								|| (alBankInstr->soundBankFormat == SNOW)
								|| (alBankInstr->soundBankFormat == SYDNEY)
								|| (alBankInstr->soundBankFormat == ARMYMENFORMAT)
								|| (alBankInstr->soundBankFormat == MEGAMAN64PTRV2)
								|| (alBankInstr->soundBankFormat == TAZHUFFMAN)
								)
						{
							region->RangeKey.usHigh = 127;
							region->RangeKey.usLow = 0;

							region->RangeVelocity.usHigh = 127;
							region->RangeVelocity.usLow = 0;
						}

						region->fusOptions = F_RGN_OPTION_SELFNONEXCLUSIVE;
						region->WaveLink.ulChannel = 1;
						region->WaveLink.ulTableIndex = dlIdWave;
						region->WaveLink.usPhaseGroup = 0;
						region->WSMP.cbSize = sizeof(WSMPL);
						region->WSMP.fulOptions = F_WSMP_NO_TRUNCATION;
						region->WSMP.sFineTune = 0;

						if (
								(alBankInstr->soundBankFormat == STANDARDFORMAT)
								|| (alBankInstr->soundBankFormat == CONKERFORMAT)
								|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
								|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
								|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
								|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
								|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
								|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
								|| (alBankInstr->soundBankFormat == TUROKFORMAT)
								|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
								|| (alBankInstr->soundBankFormat == TITUS)
								|| (alBankInstr->soundBankFormat == NINDEC)
								|| (alBankInstr->soundBankFormat == KONAMICTL)
								|| (alBankInstr->soundBankFormat == KONAMI8CTL)
								)
						{
							region->WSMP.usUnityNote = alBankInstr->percussion->sounds[y]->key.keybase;
							region->WSMP.sFineTune = (signed short)alBankInstr->percussion->sounds[y]->key.detune;

							if ((alBankInstr->soundBankFormat == KONAMICTL) || (alBankInstr->soundBankFormat == KONAMI8CTL))
							{
								if (alBankInstr->konamiDrumsADSR.size() > 0)
								{
									if (y < alBankInstr->konamiDrumsADSR.size())
									{
										region->WSMP.usUnityNote -= (signed char)alBankInstr->konamiDrumsADSR[y].coarseTune;
										region->WSMP.sFineTune = (signed char)alBankInstr->konamiDrumsADSR[y].fineTune;
									}
								}
							}
							//region->WSMP.lAttenuation = 0;
						}
						else if (
										(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
										|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
										|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)
								)
						{
							float keybaseFloat = *reinterpret_cast<float*> (&alBankInstr->percussion->sounds[y]->floatKeyBase);
							region->WSMP.usUnityNote = CN64AIFCAudio::ConvertEADGameValueToKeyBase(keybaseFloat);
						}
						else if (
									(alBankInstr->soundBankFormat == MUSYX)
									|| (alBankInstr->soundBankFormat == MUSYXLZ)
									|| (alBankInstr->soundBankFormat == MUSYXZLIB)
									|| (alBankInstr->soundBankFormat == MUSYXSMALLZLIB)
									)
						{
							region->WSMP.usUnityNote = alBankInstr->percussion->sounds[y]->key.keybase;
						}
						else if (
									(alBankInstr->soundBankFormat == N64DD)
									)
						{
							region->WSMP.usUnityNote = alBankInstr->percussion->sounds[y]->key.keybase;
							region->WSMP.sFineTune = (signed short)alBankInstr->percussion->sounds[y]->key.detune;
						}
						else if (
								((alBankInstr->soundBankFormat == SN64) || (alBankInstr->soundBankFormat == ZLIBSN64))
									)
						{
							region->WSMP.usUnityNote = alBankInstr->percussion->sounds[y]->key.keybase;
						}
						else if (
								(alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
								|| (alBankInstr->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
								|| (alBankInstr->soundBankFormat == RNCCOMPRESSEDN64PTR)
								|| (alBankInstr->soundBankFormat == SNOW)
								|| (alBankInstr->soundBankFormat == SYDNEY)
								|| (alBankInstr->soundBankFormat == ARMYMENFORMAT)
								|| (alBankInstr->soundBankFormat == MEGAMAN64PTRV2)
								|| (alBankInstr->soundBankFormat == TAZHUFFMAN)
								)
						{
							region->WSMP.usUnityNote = 0x3C;
							region->WSMP.sFineTune = 0;
						}

						if ((alBankInstr->percussion->sounds[y]->wav.adpcmWave != NULL)
							&& (alBankInstr->percussion->sounds[y]->wav.adpcmWave->loop != NULL))
						{
							if (alBankInstr->percussion->sounds[y]->wav.adpcmWave->loop->count > 0)
							{
								region->WSMP.cSampleLoops = 1;
								region->WLOOP[0].ulStart = alBankInstr->percussion->sounds[y]->wav.adpcmWave->loop->start;
								region->WLOOP[0].ulLength = (alBankInstr->percussion->sounds[y]->wav.adpcmWave->loop->end - alBankInstr->percussion->sounds[y]->wav.adpcmWave->loop->start + 1);
							}
						}
						else if ((alBankInstr->percussion->sounds[y]->wav.rawWave != NULL)
							&& (alBankInstr->percussion->sounds[y]->wav.rawWave->loop != NULL))
						{
							if (alBankInstr->percussion->sounds[y]->wav.rawWave->loop->count > 0)
							{
								region->WSMP.cSampleLoops = 1;
								region->WLOOP[0].ulStart = alBankInstr->percussion->sounds[y]->wav.rawWave->loop->start;
								region->WLOOP[0].ulLength = (alBankInstr->percussion->sounds[y]->wav.rawWave->loop->end - alBankInstr->percussion->sounds[y]->wav.rawWave->loop->start + 1);
							}
						}

						region->WLOOP[0].cbSize = sizeof(WLOOP);
						region->WLOOP[0].ulType = WLOOP_TYPE_FORWARD;


						articulation->ulArt1Idx = 1 + (trueSound * 3) + 2;

						//articParams->LFO.pcFrequency = PitchHzToCents((float)alBankInstr->samplerate);
						//articParams->LFO.tcDelay = TimeSecondsToCents(10 * timeMultiplier);
						//articParams->LFO.pcFrequency = PitchHzToCents(8.176);

						if (
								(alBankInstr->soundBankFormat == STANDARDFORMAT)
								|| (alBankInstr->soundBankFormat == CONKERFORMAT)
								|| (alBankInstr->soundBankFormat == STANDARDRNCCOMPRESSED)
								|| (alBankInstr->soundBankFormat == STANDARDFORMATRAWALLOWED)
								|| (alBankInstr->soundBankFormat == STANDARDRNXCOMPRESSED)
								|| (alBankInstr->soundBankFormat == ZLIBSTANDARD)
								|| (alBankInstr->soundBankFormat == BLASTCORPSZLBSTANDARD)
								|| (alBankInstr->soundBankFormat == BANJOTOOIEAUDIO)
								|| (alBankInstr->soundBankFormat == TUROKFORMAT)
								|| (alBankInstr->soundBankFormat == MKMYTHOLOGIES)
								|| (alBankInstr->soundBankFormat == TITUS)
								|| (alBankInstr->soundBankFormat == NINDEC)
								|| (alBankInstr->soundBankFormat == KONAMICTL)
								|| (alBankInstr->soundBankFormat == KONAMI8CTL)
								)
						{
							float decayPercentage = (float)(alBankInstr->percussion->sounds[y]->env.decayVolume) / (float)alBankInstr->percussion->sounds[y]->env.attackVolume;
							if (decayPercentage >= 0.99)
								decayPercentage = 1;

							articParams->PitchEG.tcAttack = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.attackTime * timeMultiplier);
							if (((signed long)alBankInstr->percussion->sounds[y]->env.decayTime == -1) || ((signed long)alBankInstr->percussion->sounds[y]->env.decayTime == 0))
								articParams->PitchEG.tcDecay = 0x7FFFFFFF;
							else
								articParams->PitchEG.tcDecay = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.decayTime * timeMultiplier);
							articParams->PitchEG.ptSustain = PercentToUnits(decayPercentage * 100.0);
							articParams->PitchEG.tcRelease = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.releaseTime * timeMultiplier);

							double percentPan;
							if (alBankInstr->percussion->sounds[y]->samplePan == 0x40)
								percentPan = 0;
							else if (alBankInstr->percussion->sounds[y]->samplePan == 0x7F)
								percentPan = 50;
							else if (alBankInstr->percussion->sounds[y]->samplePan == 0x00)
								percentPan = -50;
							else
							{
								float panFloat = ((float)alBankInstr->percussion->sounds[y]->samplePan / (float)0x7F) * 100.0;
								percentPan = (panFloat - 50.0);
							}

							articParams->Misc.ptDefaultPan = PercentToUnits(percentPan);

							articParams->VolEG.tcAttack = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.attackTime * timeMultiplier);
							if ((signed long)alBankInstr->percussion->sounds[y]->env.decayTime == -1)
								articParams->VolEG.tcDecay = 0x7FFFFFFF;
							else
								articParams->VolEG.tcDecay = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.decayTime * timeMultiplier);

							articParams->VolEG.ptSustain = PercentToUnits(decayPercentage * 100.0);
							articParams->VolEG.tcRelease = TimeSecondsToCents((signed long)alBankInstr->percussion->sounds[y]->env.releaseTime * timeMultiplier);
						}
						else if (	
									(alBankInstr->soundBankFormat == SUPERMARIO64FORMAT)
										|| (alBankInstr->soundBankFormat == ZELDAFORMAT)
										/*|| (alBankInstr->soundBankFormat == STARFOX64FORMAT)*/
								)
						{
							articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

							if ((alBankInstr->percussion->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankInstr->percussion->sounds[y]->adsrEAD[0] < 0))
								articParams->PitchEG.tcAttack = TimeSecondsToCents(0);
							else
								articParams->PitchEG.tcAttack = TimeSecondsToCents(((float)alBankInstr->percussion->sounds[y]->adsrEAD[0] / 1000 * 7.5));
							if ((alBankInstr->percussion->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankInstr->percussion->sounds[y]->adsrEAD[2] < 0))
							{
								articParams->PitchEG.tcDecay = 0x7FFFFFFF;
								articParams->PitchEG.ptSustain = PercentToUnits(100.0);
							}
							else
							{
								articParams->PitchEG.tcDecay = TimeSecondsToCents(((float)alBankInstr->percussion->sounds[y]->adsrEAD[2] / 1000 * 7.5));
								articParams->PitchEG.ptSustain = PercentToUnits(0.0);
							}
							if (alBankInstr->soundBankFormat == ZELDAFORMAT)
								articParams->PitchEG.tcRelease = TimeSecondsToCents(EADGameReleaseTimeSeconds[alBankInstr->percussion->sounds[y]->unknown1 & 0xFF]);
							else
								articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);
						}
						else // Not yet known, such as Zelda, N64WavPtr, etc
						{
							articParams->Misc.ptDefaultPan = PercentToUnits(0.0);

							articParams->PitchEG.tcAttack = TimeSecondsToCents(0.001);
							articParams->PitchEG.tcDecay = TimeSecondsToCents(0.001);
							articParams->PitchEG.ptSustain = PercentToUnits(100.0);
							articParams->PitchEG.tcRelease = TimeSecondsToCents(0.5);

							articParams->VolEG.tcAttack = TimeSecondsToCents(0.001);
							articParams->VolEG.tcDecay = TimeSecondsToCents(0.001);
							articParams->VolEG.ptSustain = PercentToUnits(100.0);
							articParams->VolEG.tcRelease = TimeSecondsToCents(0.5);
						}

						trueSound++;
					}
				}

				if (SUCCEEDED(hr = mPortDownload->Download(instrumentMusicDownload)))
				{
					mInstrumentGeneratedLoaded.push_back(instrumentMusicDownload);
				}
				else
				{
					instrumentMusicDownload->Release();
				}
			}
		}

		if ((alBankInstr->countEADPercussion > 0) && (alBankInstr->eadPercussion != NULL))
		{
			// TODO
		}
	}

	return hr;
}

HRESULT CMidiPlayer::GetNextEvent(DMUS_NOTIFICATION_PMSG& pPmsg)
{
	if (mPerformance == NULL)
		return S_FALSE;

	DMUS_NOTIFICATION_PMSG* newMsg = NULL;
	HRESULT hr = mPerformance->GetNotificationPMsg(&newMsg);

	if (hr == S_OK)
	{
		pPmsg = *newMsg;

		// CANNOT FIGURE OUT WHY, only seg start firing, seg end never firing
		if (pPmsg.dwNotificationOption == DMUS_NOTIFICATION_SEGSTART)
			StartedSegment = true;

		mPerformance->FreePMsg((DMUS_PMSG*)newMsg);
	}
	return hr;
}

