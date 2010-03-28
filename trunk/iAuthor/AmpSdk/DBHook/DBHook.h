 
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DBHOOK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DBHOOK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DBHOOK_EXPORTS
#define DBHOOK_API __declspec(dllexport)
#else
#define DBHOOK_API __declspec(dllimport)
#endif

// This class is exported from the DBHook.dll
class DBHOOK_API CDBHook {
public:
	CDBHook(void);
	// TODO: add your methods here.
};


DBHOOK_API bool DBHookInstallCheck();
DBHOOK_API bool DBHookInstall(HWND hWndhInst, HCURSOR hCursor);
DBHOOK_API int  DBHookUninstall();
DBHOOK_API void DBHookSetCursor(HCURSOR hCursor);
DBHOOK_API void DBHSetHookEnable(bool bEnable);
DBHOOK_API void DBHSetHookFullWorkarea(bool bFullarea);
DBHOOK_API void DBHSetHookDrawParentarea(bool bDrawParentArea);
DBHOOK_API void DBHSetHookSelectRange(bool bSelectRange);

DBHOOK_API RECT DBHGetWorkarea();
