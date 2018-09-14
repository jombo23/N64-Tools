#ifdef DLL_CONFIG
	#ifdef SOUNDTOOL_EXPORTS
		#define SOUNDTOOL_API __declspec(dllexport)
	#else
		#define SOUNDTOOL_API __declspec(dllimport)
	#endif
#else
	#define SOUNDTOOL_API
#endif