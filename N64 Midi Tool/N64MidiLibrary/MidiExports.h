#pragma once

#ifdef DLL_CONFIG
	#ifdef MIDI_EXPORTS
		#define MIDI_API __declspec(dllexport)
	#else
		#define MIDI_API __declspec(dllimport)
	#endif
#else
	#define MIDI_API
#endif