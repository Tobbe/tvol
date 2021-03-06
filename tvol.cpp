// Written by Tobbe
// 2006-10-19
// Latest update: 2009-03-01

#define STRICT
#define NOCRYPT

#include <windows.h>
#include "tvol.h"

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

// just initializing the functions
void BangVol(HWND caller, const char *bangName, const char *args);

// Actual main function
extern "C" int initModuleEx(HWND ParentWnd, HINSTANCE dllInst, LPCSTR szPath)
{
	// register the bangs with Litestep
	AddBangCommandEx("!tVolSet", BangVol);
	AddBangCommandEx("!tVolGet", BangVol);
	AddBangCommandEx("!tVolToggleMute", BangVol);

	return 0;
}

void BangVol(HWND caller, const char* bangName, const char* args)
{
	bool bToggleMute = false;
	bool bSet = false;
	bool bGet = false;

	if (bangName[5] == 'T' || bangName[5] == 't')
	{
		bToggleMute = true;
	}
	else if (bangName[5] == 'S' || bangName[5] == 's')
	{
		bSet = true;
	}
	else
	{
		bGet = true;
	}

	/**
	 *
	 * Open the mixer, set hMixer
	 *
	 */

	HMIXER hMixer;
	if (mixerOpen(&hMixer, 0, 0, 0, MIXER_OBJECTF_HMIXER) != MMSYSERR_NOERROR)
	{
		MessageBox(caller, "Couldn't execute the requested operation", "Error", MB_OK);
		return;
	}

	/**
	 *
	 * get dwLineID
	 *
	 */

	MIXERLINE ml = {0};
	ml.cbStruct = sizeof(MIXERLINE);
	ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (mixerGetLineInfo((HMIXEROBJ)hMixer, &ml, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
	{
		MessageBox(caller, "Couldn't execute the requested operation", "Error", MB_OK);
		return;
	}

	/**
	 *
	 * get dwControlID
	 *
	 */

	MIXERLINECONTROLS mlc = {0};
	MIXERCONTROL mc = {0};

	if (bToggleMute)
	{
		mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
	}
	else
	{
		mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	}

	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = ml.dwLineID;
	mlc.cControls = 1;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	mlc.pamxctrl = &mc;

	if (mixerGetLineControls((HMIXEROBJ)hMixer, &mlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		MessageBox(caller, "Couldn't execute the requested operation", "Error", MB_OK);
		return;
	}

	MIXERCONTROLDETAILS mcd = {0};
	MIXERCONTROLDETAILS_BOOLEAN mcdb = {0};
	MIXERCONTROLDETAILS_UNSIGNED mcdu = {0};

	if (bToggleMute)
	{
		mcd.paDetails = &mcdb;
		mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	}
	else
	{
		mcd.paDetails = &mcdu;
		mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	}

	mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mcd.dwControlID = mc.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;

	if (mixerGetControlDetails((HMIXEROBJ) hMixer, &mcd, MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
	{
		MessageBox(caller, "Couldn't execute the requested operation", "Error", MB_OK);
		return;
	}

	mcdb.fValue = !mcdb.fValue;

	if (bSet)
	{
		//full volume at 65535, max dwords can handle is 4294967295
		mcdu.dwValue = atol(args);
	}

	if (bGet)
	{
		char tmp[6];
		wsprintf(tmp, "%lu", mcdu.dwValue);

		LSSetVariable("tVolVolume", tmp);
	}
	else
	{
		if (mixerSetControlDetails((HMIXEROBJ)hMixer, &mcd, MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		{
			MessageBox(caller, "Couldn't execute the requested operation", "Error", MB_OK);
			return;
		}
	}

	mixerClose(hMixer);
}

// -------------------------------------------------------
// cleanup (opposite of init())
extern "C" void quitModule(HINSTANCE dllInst)
{
	RemoveBangCommand("!tVolSet");
	RemoveBangCommand("!tVolGet");
	RemoveBangCommand("!tVolToggleMute");
}