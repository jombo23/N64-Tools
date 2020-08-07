#include "StdAfx.h"
#include "CustomCommandLineInfo.h"

CCustomCommandLineInfo::CCustomCommandLineInfo(void)
{
	
}

CCustomCommandLineInfo::~CCustomCommandLineInfo(void)
{
}

void CCustomCommandLineInfo::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	CString paramString;
	paramString.Format("%s", pszParam);
	
	if (paramString.Find("+autoGenerate") == 0)
	{
		startupParameters.autoGenerate = true;
	}
	if (paramString.Find("-autoGenerate") == 0)
	{
		startupParameters.autoGenerate = false;
	}

	if (paramString.Find("+autoExit") == 0)
	{
		startupParameters.autoExit = true;
	}
	if (paramString.Find("-autoExit") == 0)
	{
		startupParameters.autoExit = false;
	}

	if (paramString.Find("+doMessageBoxes") == 0)
	{
		startupParameters.doMessageBoxes = true;
	}
	if (paramString.Find("-doMessageBoxes") == 0)
	{
		startupParameters.doMessageBoxes = false;
	}
	
	if (paramString.Find("scaleDiffuseFactor=") == 0)
	{
		startupParameters.scaleDiffuseFactor = atof(paramString.Mid(13));
	}

	if (paramString.Find("scaleAmbientFactor=") == 0)
	{
		startupParameters.scaleAmbientFactor = atof(paramString.Mid(13));
	}
	
	if (paramString.Find("scaleSpecularFactor=") == 0)
	{
		startupParameters.scaleSpecularFactor = atof(paramString.Mid(14));
	}

	if (paramString.Find("xMove=") == 0)
	{
		startupParameters.xMove = atof(paramString.Mid(6));
	}

	if (paramString.Find("yMove=") == 0)
	{
		startupParameters.yMove = atof(paramString.Mid(6));
	}

	if (paramString.Find("zMove=") == 0)
	{
		startupParameters.zMove = atof(paramString.Mid(6));
	}

	if (paramString.Find("scaleVerticesFactor=") == 0)
	{
		startupParameters.scaleVerticesFactor = atof(paramString.Mid(14));
	}

	if (paramString.Find("texelSizeU=") == 0)
	{
		startupParameters.texelSizeU = atof(paramString.Mid(11));
	}

	if (paramString.Find("texelSizeV=") == 0)
	{
		startupParameters.texelSizeV = atof(paramString.Mid(11));
	}

	if (paramString.Find("scaleUVsFactor=") == 0)
	{
		startupParameters.scaleUVsFactor = atof(paramString.Mid(9));
	}
	
	if (paramString.Find("scaleAmbientFactorAn8Value=") == 0)
	{
		startupParameters.scaleAmbientFactorAn8Value = atof(paramString.Mid(19));
	}

	if (paramString.Find("scaleDiffuseFactorAn8Value=") == 0)
	{
		startupParameters.scaleDiffuseFactorAn8Value = atof(paramString.Mid(19));
	}

	if (paramString.Find("inputFile=") == 0)
	{
		startupParameters.inputFile = paramString.Mid(10);
		startupParameters.inputFile.Replace("\"", "");
	}

	if (paramString.Find("outputFile=") == 0)
	{
		startupParameters.outputFile = paramString.Mid(11);
		startupParameters.outputFile.Replace("\"", "");
	}

	if (paramString.Find("replaceFile=") == 0)
	{
		startupParameters.replaceFile = paramString.Mid(12);
		startupParameters.replaceFile.Replace("\"", "");
	}

	if (paramString.Find("+specialKeywordMode") == 0)
	{
		startupParameters.specialKeywordMode = true;
	}
	if (paramString.Find("-specialKeywordMode") == 0)
	{
		startupParameters.specialKeywordMode = false;
	}

	if (paramString.Find("+mergeLikeMaterials") == 0)
	{
		startupParameters.mergeLikeMaterials = true;
	}
	if (paramString.Find("-mergeLikeMaterials") == 0)
	{
		startupParameters.mergeLikeMaterials = false;
	}

	if (paramString.Find("+renameMaterials") == 0)
	{
		startupParameters.renameMaterials = true;
	}
	if (paramString.Find("-renameMaterials") == 0)
	{
		startupParameters.renameMaterials = false;
	}

	if (paramString.Find("+renameGroups") == 0)
	{
		startupParameters.renameGroups = true;
	}
	if (paramString.Find("-renameGroups") == 0)
	{
		startupParameters.renameGroups = false;
	}

	if (paramString.Find("+stripImagePaths") == 0)
	{
		startupParameters.stripImagePaths = true;
	}
	if (paramString.Find("-stripImagePaths") == 0)
	{
		startupParameters.stripImagePaths = false;
	}

	if (paramString.Find("+roundVertices") == 0)
	{
		startupParameters.roundVertices = true;
	}
	if (paramString.Find("-roundVertices") == 0)
	{
		startupParameters.roundVertices = false;
	}

	if (paramString.Find("+roundVerticesTenths") == 0)
	{
		startupParameters.roundVerticesTenths = true;
	}
	if (paramString.Find("-roundVerticesTenths") == 0)
	{
		startupParameters.roundVerticesTenths = false;
	}

	if (paramString.Find("+roundVerticesHundredths") == 0)
	{
		startupParameters.roundVerticesHundredths = true;
	}
	if (paramString.Find("-roundVerticesHundredths") == 0)
	{
		startupParameters.roundVerticesHundredths = false;
	}

	if (paramString.Find("+ignoreShading") == 0)
	{
		startupParameters.ignoreShading = true;
	}
	if (paramString.Find("-ignoreShading") == 0)
	{
		startupParameters.ignoreShading = false;
	}

	if (paramString.Find("+ignoreShadingPoint7") == 0)
	{
		startupParameters.ignoreShadingPoint7 = true;
	}
	if (paramString.Find("-ignoreShadingPoint7") == 0)
	{
		startupParameters.ignoreShadingPoint7 = false;
	}

	if (paramString.Find("+recenterObjects") == 0)
	{
		startupParameters.recenterObjects = true;
	}
	if (paramString.Find("-recenterObjects") == 0)
	{
		startupParameters.recenterObjects = false;
	}

	if (paramString.Find("+scaleVertices") == 0)
	{
		startupParameters.scaleVertices = true;
	}
	if (paramString.Find("-scaleVertices") == 0)
	{
		startupParameters.scaleVertices = false;
	}

	if (paramString.Find("+noGroups") == 0)
	{
		startupParameters.noGroups = true;
	}
	if (paramString.Find("-noGroups") == 0)
	{
		startupParameters.noGroups = false;
	}

	if (paramString.Find("+primarySecondaryGroups") == 0)
	{
		startupParameters.primarySecondaryGroups = true;
	}
	if (paramString.Find("-primarySecondaryGroups") == 0)
	{
		startupParameters.primarySecondaryGroups = false;
	}

	if (paramString.Find("+useReplacementFile") == 0)
	{
		startupParameters.useReplacementFile = true;
	}
	if (paramString.Find("-useReplacementFile") == 0)
	{
		startupParameters.useReplacementFile = false;
	}

	if (paramString.Find("+fixKsKaNs") == 0)
	{
		startupParameters.fixKsKaNs = true;
	}
	if (paramString.Find("-fixKsKaNs") == 0)
	{
		startupParameters.fixKsKaNs = false;
	}

	if (paramString.Find("+scaleDiffuse") == 0)
	{
		startupParameters.scaleDiffuse = true;
	}
	if (paramString.Find("-scaleDiffuse") == 0)
	{
		startupParameters.scaleDiffuse = false;
	}

	if (paramString.Find("+scaleAmbient") == 0)
	{
		startupParameters.scaleAmbient = true;
	}
	if (paramString.Find("-scaleAmbient") == 0)
	{
		startupParameters.scaleAmbient = false;
	}

	if (paramString.Find("+scaleSpecular") == 0)
	{
		startupParameters.scaleSpecular = true;
	}
	if (paramString.Find("-scaleSpecular") == 0)
	{
		startupParameters.scaleSpecular = false;
	}

	if (paramString.Find("+mergeHierarchicalGroupsToEnd") == 0)
	{
		startupParameters.mergeHierarchicalGroupsToEnd = true;
	}
	if (paramString.Find("-mergeHierarchicalGroupsToEnd") == 0)
	{
		startupParameters.mergeHierarchicalGroupsToEnd = false;
	}

	if (paramString.Find("+moveCoords") == 0)
	{
		startupParameters.moveCoords = true;
	}
	if (paramString.Find("-moveCoords") == 0)
	{
		startupParameters.moveCoords = false;
	}

	if (paramString.Find("+scaleAmbientFactorAn8") == 0)
	{
		startupParameters.scaleAmbientFactorAn8 = true;
	}
	if (paramString.Find("-scaleAmbientFactorAn8") == 0)
	{
		startupParameters.scaleAmbientFactorAn8 = false;
	}

	if (paramString.Find("+scaleDiffuseFactorAn8") == 0)
	{
		startupParameters.scaleDiffuseFactorAn8 = true;
	}
	if (paramString.Find("-scaleDiffuseFactorAn8") == 0)
	{
		startupParameters.scaleDiffuseFactorAn8 = false;
	}

	if (paramString.Find("+disableAutoshading") == 0)
	{
		startupParameters.disableAutoshading = true;
	}
	if (paramString.Find("-disableAutoshading") == 0)
	{
		startupParameters.disableAutoshading = false;
	}

	if (paramString.Find("+defaultShading") == 0)
	{
		startupParameters.defaultShading = true;
	}
	if (paramString.Find("-defaultShading") == 0)
	{
		startupParameters.defaultShading = false;
	}

	if (paramString.Find("+recalculateUVs") == 0)
	{
		startupParameters.recalculateUVs = true;
	}
	if (paramString.Find("-recalculateUVs") == 0)
	{
		startupParameters.recalculateUVs = false;
	}

	if (paramString.Find("+scaleUVs") == 0)
	{
		startupParameters.scaleUVs = true;
	}
	if (paramString.Find("-scaleUVs") == 0)
	{
		startupParameters.scaleUVs = false;
	}

	if (paramString.Find("+specifyUVTileSize") == 0)
	{
		startupParameters.specifyUVTileSize = true;
	}
	if (paramString.Find("-specifyUVTileSize") == 0)
	{
		startupParameters.specifyUVTileSize = false;
	}	

	if (paramString.Find("+regexFilterCheck") == 0)
	{
		startupParameters.regexFilterCheck = true;
	}
	if (paramString.Find("-regexFilterCheck") == 0)
	{
		startupParameters.regexFilterCheck = false;
	}

	if (paramString.Find("regexFilter=") == 0)
	{
		startupParameters.regexFilter = paramString.Mid(12);
	}

	if (paramString.Find("fbxExportType=") == 0)
	{
		startupParameters.fbxExportType = paramString.Mid(14);
	}

	if (paramString.Find("+overrideSkeleton") == 0)
	{
		startupParameters.overrideSkeleton = true;
	}
	if (paramString.Find("-overrideSkeleton") == 0)
	{
		startupParameters.overrideSkeleton = false;
	}

	if (paramString.Find("overrideSkeletonFile=") == 0)
	{
		startupParameters.overrideSkeletonFile = paramString.Mid(21);
	}

	if (paramString.Find("jointMode=") == 0)
	{
		startupParameters.jointMode = paramString.Mid(10);

		startupParameters.jointMode.MakeLower();
		if (startupParameters.jointMode != "relative")
			startupParameters.jointMode = "absolute";
	}

	if (paramString.Find("fps=") == 0)
	{
		startupParameters.fps = atof(paramString.Mid(4));
		if (startupParameters.fps < 0)
			startupParameters.fps = 30;
	}

	if (paramString.Find("+sortTexturesWithinGroup") == 0)
	{
		startupParameters.sortTexturesWithinGroup = true;
	}
	if (paramString.Find("-sortTexturesWithinGroup") == 0)
	{
		startupParameters.sortTexturesWithinGroup = false;
	}
}