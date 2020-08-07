#pragma once

class CStartupParameters
{
public:
	CStartupParameters(void);
	~CStartupParameters(void);

	bool specialKeywordMode;
	bool mergeLikeMaterials;
	bool renameMaterials;
	bool renameGroups;
	bool stripImagePaths;
	bool roundVertices;
	bool roundVerticesTenths;
	bool roundVerticesHundredths;
	bool ignoreShading;
	bool ignoreShadingPoint7;
	bool recenterObjects;
	bool scaleVertices;
	bool noGroups;
	bool primarySecondaryGroups;
	bool useReplacementFile;
	bool fixKsKaNs;
	bool scaleDiffuse;
	bool scaleAmbient;
	bool scaleSpecular;
	bool mergeHierarchicalGroupsToEnd;
	bool moveCoords;
	bool scaleAmbientFactorAn8;
	bool scaleDiffuseFactorAn8;
	bool disableAutoshading;
	bool defaultShading;
	bool recalculateUVs;
	bool scaleUVs;
	bool specifyUVTileSize;
	bool regexFilterCheck;

	float scaleDiffuseFactor;
	float scaleAmbientFactor;
	float scaleSpecularFactor;
	float xMove;
	float yMove;
	float zMove;
	float scaleVerticesFactor;
	float texelSizeU;
	float texelSizeV;
	float scaleUVsFactor;

	float scaleAmbientFactorAn8Value;
	float scaleDiffuseFactorAn8Value;

	CString regexFilter;

	CString inputFile;
	CString outputFile;
	CString replaceFile;

	CString fbxExportType;

	bool overrideSkeleton;
	CString overrideSkeletonFile;

	bool autoGenerate;
	bool autoExit;

	bool doMessageBoxes;
	CString jointMode;
	float fps;

	bool sortTexturesWithinGroup;
};
