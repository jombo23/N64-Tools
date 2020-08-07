#include "StdAfx.h"
#include "StartupParameters.h"

CStartupParameters::CStartupParameters(void)
{
	specialKeywordMode = false;
	mergeLikeMaterials = true;
	renameMaterials = true;
	renameGroups = false;
	stripImagePaths = true;
	roundVertices = false;
	roundVerticesTenths = false;
	roundVerticesHundredths = false;
	ignoreShading = false;
	ignoreShadingPoint7 = false;
	recenterObjects = false;
	scaleVertices = false;
	noGroups = false;
	primarySecondaryGroups = false;
	useReplacementFile = false;
	fixKsKaNs = false;
	scaleDiffuse = false;
	scaleAmbient = false;
	scaleSpecular = false;
	mergeHierarchicalGroupsToEnd = false;
	moveCoords = false;
	scaleAmbientFactorAn8 = false;
	scaleDiffuseFactorAn8 = false;
	disableAutoshading = true;
	defaultShading = false;
	recalculateUVs = false;
	scaleUVs = false;
	specifyUVTileSize = false;
	doMessageBoxes = true;

	scaleDiffuseFactor = 1.0f;
	scaleAmbientFactor = 1.0f;
	scaleSpecularFactor = 1.0f;
	xMove = 0.0f;
	yMove = 0.0f;
	zMove = 0.0f;
	scaleVerticesFactor = 1.0f;
	texelSizeU = 10.0f;
	texelSizeV = 10.0f;
	scaleUVsFactor = 1.0f;

	scaleAmbientFactorAn8Value = 1.0f;
	scaleDiffuseFactorAn8Value = 0.001f;

	regexFilterCheck = false;
	regexFilter = ".*";

	inputFile = "";
	outputFile = "";
	replaceFile = "";

	fbxExportType = "ascii";

	autoGenerate = false;
	autoExit = false;

	overrideSkeleton = false;
	overrideSkeletonFile = "";

	jointMode = "absolute";

	fps = 30.0;
	sortTexturesWithinGroup = false;
}

CStartupParameters::~CStartupParameters(void)
{
}
