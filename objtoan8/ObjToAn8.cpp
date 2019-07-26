
// ObjToAn8.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ObjToAn8.h"
#include "ObjToAn8Dlg.h"
#include "StartupParameters.h"
#include "CustomCommandLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObjToAn8App

BEGIN_MESSAGE_MAP(CObjToAn8App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CObjToAn8App construction

CObjToAn8App::CObjToAn8App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	returnExitCode = 0;
}


// The one and only CObjToAn8App object

CObjToAn8App theApp;


// CObjToAn8App initialization

BOOL CObjToAn8App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ObjToAn8"));

	CCustomCommandLineInfo oInfo;
	ParseCommandLine(oInfo);

	CObjToAn8Dlg dlg;
	dlg.startupParameters = oInfo.startupParameters;

	if (oInfo.startupParameters.autoGenerate)
	{
		if (!dlg.PerformConversion(oInfo.startupParameters.specialKeywordMode, oInfo.startupParameters.mergeLikeMaterials, oInfo.startupParameters.renameMaterials, oInfo.startupParameters.renameGroups, oInfo.startupParameters.stripImagePaths, 
			oInfo.startupParameters.roundVertices, oInfo.startupParameters.roundVerticesTenths, oInfo.startupParameters.roundVerticesHundredths, oInfo.startupParameters.ignoreShading, oInfo.startupParameters.ignoreShadingPoint7, oInfo.startupParameters.recenterObjects, 
			oInfo.startupParameters.scaleVertices, oInfo.startupParameters.noGroups, oInfo.startupParameters.primarySecondaryGroups, oInfo.startupParameters.useReplacementFile, oInfo.startupParameters.fixKsKaNs, 
			oInfo.startupParameters.scaleDiffuse, oInfo.startupParameters.scaleAmbient, oInfo.startupParameters.scaleSpecular, oInfo.startupParameters.mergeHierarchicalGroupsToEnd, oInfo.startupParameters.moveCoords, 
			oInfo.startupParameters.scaleAmbientFactorAn8, oInfo.startupParameters.scaleDiffuseFactorAn8, oInfo.startupParameters.disableAutoshading, oInfo.startupParameters.defaultShading, 
			oInfo.startupParameters.recalculateUVs, oInfo.startupParameters.scaleUVs, oInfo.startupParameters.specifyUVTileSize, 
			oInfo.startupParameters.scaleDiffuseFactor, oInfo.startupParameters.scaleAmbientFactor, oInfo.startupParameters.scaleSpecularFactor, 
			oInfo.startupParameters.xMove, oInfo.startupParameters.yMove, oInfo.startupParameters.zMove, 
			oInfo.startupParameters.scaleVerticesFactor, oInfo.startupParameters.texelSizeU, oInfo.startupParameters.texelSizeV, oInfo.startupParameters.scaleUVsFactor, 
			oInfo.startupParameters.scaleAmbientFactorAn8Value, oInfo.startupParameters.scaleDiffuseFactorAn8Value, oInfo.startupParameters.regexFilterCheck, oInfo.startupParameters.regexFilter, 
			oInfo.startupParameters.inputFile, oInfo.startupParameters.outputFile, oInfo.startupParameters.replaceFile, oInfo.startupParameters.fbxExportType, oInfo.startupParameters.overrideSkeleton, oInfo.startupParameters.overrideSkeletonFile, oInfo.startupParameters.doMessageBoxes, oInfo.startupParameters.jointMode, oInfo.startupParameters.fps))
		{
			returnExitCode = 99;
			return FALSE;
		}
	}

	if (!oInfo.startupParameters.autoExit)
	{
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CObjToAn8App::ExitInstance()
{
	// NOte: exit code 2 = invalid value (clicking X in MFC)
	if (returnExitCode == 99)
		return returnExitCode;
	else
		return CWinApp::ExitInstance();
}
