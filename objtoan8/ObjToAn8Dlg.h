
// ObjToAn8Dlg.h : header file
//
#pragma once
#if (_MSC_VER == 1500)
#include <vld.h> 
#endif

#include "afxwin.h"
#include <vector>
#include <map>
#include <algorithm>
#include "Normal.h"
#include "PolygonPoint.h"
#include "PolygonSet.h"
#include "UVCoordinate.h"
#include "VerticeColor.h"
#include "Vertice.h"
#include "Group.h"
#include "Material.h"
#include "MaterialFile.h"
#include "AnimationPart.h"
#include "Keyframe.h"
#include "Animation.h"
#include "Joint.h"
#include "JointWeight.h"
#include "Bone.h"
#include "Camera.h"
#include "StartupParameters.h"
#include <string>
#include "Liban8/An8Loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/Exporter.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#ifdef FBXSDK_NEW_API
#include <fbxsdk.h>
#include <math.h>
#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif
#endif

#define PI 3.14159265

#include "xImage.h"

using namespace std;

struct PolygonTextureGroup
{
	std::vector<CPolygon*> polygons;
	CString materialName;
};

// CObjToAn8Dlg dialog
class CObjToAn8Dlg : public CDialog
{
// Construction
public:
	CObjToAn8Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OBJTOAN8_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit mInputFilename;
	CEdit mOutputFilename;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	bool PerformConversion(bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool renameGroups, bool stripImagePaths, 
		bool roundVertices, bool roundVerticesTenths, bool roundVerticesHundredths, bool ignoreShading, bool ignoreShadingPoint7, bool recenterObjects, 
		bool scaleVertices, bool noGroups, bool primarySecondaryGroups, bool useReplacementFile, bool fixKsKaNs, 
		bool scaleDiffuse, bool scaleAmbient, bool scaleSpecular, bool mergeHierarchicalGroupsToEnd, bool moveCoords, 
		bool scaleAmbientFactorAn8, bool scaleDiffuseFactorAn8, bool disableAutoshading, bool defaultShading, 
		bool recalculateUVs, bool scaleUVs, bool specifyUVTileSize, 
		float scaleDiffuseFactor, float scaleAmbientFactor, float scaleSpecularFactor, float xMove, float yMove, float zMove, 
		float scaleVerticesFactor, float texelSizeU, float texelSizeV, float scaleUVsFactor, 
		float scaleAmbientFactorAn8Value, float scaleDiffuseFactorAn8Value, bool regexFilterCheck, CString regexFilter, 
		CString inputFile, CString outputFile, CString replaceFile, CString fbxExportType, bool overrideSkeleton, CString overrideSkeletonFile, bool doMessageBoxes, CString jointMode,
		float fps);

	CStartupParameters startupParameters;

	int GetSizeFile(CString filename);
	afx_msg void OnBnClickedOk();
	CString PullOutImageName(CString fullPath);
	unsigned char ColorFloatToHex(float color);
	CString PrintHexColorToString(float r, float g, float b);
	CButton mSpecialKeywordMode;
	bool IsSpecialKeyword(bool specialKeywordMode, CMaterial* material);
	bool IsSpecialKeywordEnvMapping(bool specialKeywordMode, CMaterial* material);
	CButton mMergeLikeMaterials;
	CButton mRenameMaterialsToTextureNames;
	bool ReadObjFile(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups);
	bool ReadAn8File(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups);
	bool ReadAssimpFile(CString extension, CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups);

	void WriteAn8File(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool roundVertices, bool roundVerticesTenths, bool roundVerticesHundredths, bool recenterObjects, bool scaleAmbientFactor, float scaleAmbientFactorValue, bool scaleDiffuseFactor, float scaleDiffuseFactorValue, bool disableAutoshading, bool defaultShading, bool regexFilterCheck, CString regexFilter);
	void WriteObjFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles,  std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter);
	void WriteBvhFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles,  std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter);
	void WriteAssimpFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles,  std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter, CString extensionWrite);

	CString GetRotationAndTruncateToEpsilonString(float rotation);
	CString GetTranslationToEpsilonString(float translation);

	void WriteBvhSkeleton(FILE* outFile, CJoint* joint, int indent);
	void WriteBvhMotionKeyframe(FILE* outFile, CKeyframe* keyframe, CJoint* joint);

	void WriteAssimpMesh(aiScene* scene, aiMesh* mesh, bool foundNormals, bool foundTextureUV, bool foundVerticeColors, std::vector<CPolygon*> polygonList, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CJoint*> joints);
	void WriteAssimpSkeleton(aiScene* scene, CJoint* joint, aiNode* parentNode);


	CString ReplaceStringInPlace(CString inStr, CString source, CString change);
	void RenameMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void RecalculateUVs(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, bool specifyTileSize, float tileU, float tileV);
	void RenameGroups(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void IgnoreShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void IgnoreShadingPoint7(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void MergeLikeMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void StripImagePaths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void RoundVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void RoundVerticesTenths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void RoundVerticesHundredths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void MoveVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float xMove, float yMove, float zMove);
	void ScaleVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float scaleFactor);
	void ScaleUVs(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float scaleFactor);
	void ReplaceMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, std::vector<CMaterial*> replacementMaterialOverrides);
	void FixNiKsKaNs(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors);
	void ScaleDiffuseShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float diffuseScaling);
	void ScaleSpecularShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float specularScaling);
	void ScaleAmbientShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float ambientScaling);
	void DisposeStructures(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CMaterial*>& replacementMaterialOverrides, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations);
	int GetAddUVIndex(std::vector<CUVCoordinate*>& uvCoordinates, float uvalue, float vvalue);
	int GetAddNormalIndex(std::vector<CNormal*>& normals, float nx, float ny, float nz);
	int GetAddVerticeColorIndex(std::vector<CVerticeColor*>& verticeColors, float r, float g, float b, float a);
	CButton mRenameGroups;
	CButton mStripMtlImagePaths;
	CButton mRound;
	float round(float before);
	float roundtenths(float before);
	float roundhundreths(float before);
	CButton mIgnoreShading;
	CButton mRoundTenths;

	std::vector<CJoint*> FindRootJoints(std::vector<CJoint*>& joints);

	#ifdef FBXSDK_NEW_API
	bool ReadFbxFile(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups, bool overrideSkeleton, CString overrideSkeletonFile, JointType jointType);
	void ParseFbxNodeRecursive(FbxNode* pNode, CGroup* currentGroup, CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints,  std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool noGroups, CString& errorString, JointType jointType);

	void GetNumberKeyframesFbxCameraRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, int& maxKeyframes, std::vector<CJoint*>& joints);
	void GetNumberKeyframesFbxAnimationRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, int& maxKeyframes, std::vector<CJoint*>& joints);
	std::vector<int> GetKeyframeIndexes(FbxAnimCurve* lAnimCurve);
	void ParseFbxAnimationRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CJoint*>& joints, CAnimation* animation, int numberKeyframes);
	void ParseFbxCameraRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations);
	void ParseFbxBlendShapeRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CJoint*>& joints, CAnimation* animation, bool noGroups, int& verticesOffset);
	void ContainsFbxBlendShapeRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool& containsBlendShape);

	void ParseFbxSkeletonRecursive(FbxNode* pNode, std::vector<CJoint*>& joints, float3 position, CJoint* parent, map<CString, float3> skeletalOverrides, map<CString, float3> skeletalOverridesRelativeScale, map<CString, float3> skeletalOverridesRelativeRotation, map<CString, float3> skeletalOverridesRelativePosition, JointType jointType);
	void WriteFbxSkeleton(std::map<CString, FbxCluster*>& jointCluster, std::map<CString, FbxNode*>& skeletonCluster, FbxScene* pScene, CJoint* joint, FbxNode* skeletonNode);
	void WriteFbxFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, CString inputPath, bool regexFilterCheck, CString regexFilter, CString fbxExportType, float fps);

	FbxDouble3 GetMaterialColor(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName);
	const char* GetMaterialTextureName(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName);
	FbxSurfacePhong* CreateMaterial(FbxManager* pSdkManager, CString textureName);
	FbxTexture* CreateTexture(FbxManager* pSdkManager, CString texturePath);
	#endif
	void WriteOwnDaeSkeleton(CString& libraryVisualScenes, CJoint* joint, int indent);
	void WriteOwnDaeFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles,  std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter);
	CKeyframe* GetAddKeyframe(int keyframeNumber, CAnimation* animation);
	CAnimationPart* GetAddPart(CString partName, CKeyframe* keyframe);
	void ReadMaterialOverridesFile(std::vector<CMaterial*>& replacementMaterialOverrides, CString replaceFile);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CButton mRecenterObjects;
	CButton mScaleVerticesCheck;
	CEdit mScaleVerticesEdit;
	CButton mCheckNoGroups;
	CEdit mReplacesFilename;
	afx_msg void OnBnClickedButtonchoosereplaces();
	CButton mDoReplaceFile;
	CButton mCheckFixNiKaKsNs;
	CButton mCheckScaleDiffuseShading;
	CEdit mScaleDiffuseShade;
	CButton mMergeHierarchicalGroupsToEnd;
	CButton mIgnorePoint7Shades;
	CEdit mXMove;
	CEdit mYMove;
	CEdit mZMove;
	CButton mCheckMovePoints;
	CButton mCheckScaleSpecularShading;
	CButton mCheckScaleAmbientShading;
	CEdit mScaleSpecularShade;
	CEdit mScaleAmbientShade;
	CEdit mScaleAmbientFactor;
	CEdit mScaleDiffuseFactor;
	CButton mCheckScaleAmbientFactor;
	CButton mCheckScaleDiffuseFactor;
	CButton mDisableAutoshading;
	CButton mDefaultShading;
	afx_msg void OnBnClickedCheckdisableautoshading();
	afx_msg void OnBnClickedCheckdefaultshading();
	afx_msg void OnBnClickedCheckignoreshading();
	afx_msg void OnBnClickedCheckignoreshading2();
	CButton mPrimarySecondaryGroups;
	CButton mRecalculateUVs;
	int FindCaseInsensitive(CString inputString, CString string, int start=0);
	CEdit mScaleUVsEdit;
	CButton mScaleUVs;
	CButton mSpecifyTileSizeCheck;
	CEdit mTileSizeU;
	CEdit mTileSizeV;

	void WriteAn8BoneDeclaration(CBone* parentBone, std::vector<CBone*>& bones, std::vector<CBone*>& rootBones, std::vector<CJoint*> joints, CJoint* parentJoint);
	void WriteAn8Bone(FILE* outFile, CBone* bone, int indent, Quat parentOrientation, CString parentName);
	void ParseAn8BoneRecursive(An8Bone bone, std::vector<CJoint*>& joints, float3 position, CJoint* parent, AN8XMATRIX* i_ParentAbsoluteMatrix);
	void CalculateAn8JointOrientation(CJoint* joint, CBone* rootBone, AN8XMATRIX& finalMatrix);
	void CalculateAn8OrientationRecursive(CBone* bone, AN8XMATRIX* i_ParentAbsoluteMatrix, AN8XMATRIX& finalMatrix, CJoint* endJoint);

	void ParseAssimpNodesRecursive(CString extension, const aiScene* scene, aiNode* node, std::vector<CJoint*>& joints, aiVector3t<float> position, CJoint* parent, CMaterialFile* currentMaterialFile, CGroup* currentGroup,
		std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, bool& foundNormals, bool& foundTextureUV, bool& foundVerticeColors, bool noGroups);

	bool doMessageBoxes;

	bool StringHasNumber(CString string);
	CButton mRegexGroupFilterCheck;
	CEdit mRegexGroupFilter;
	CButton mRoundHundredths;
	CStatic mFbxExporterGroup;
	CStatic mFbxExportTypeLabel;
	CComboBox mFbxExportType;
	CButton mOverrideSkeletonCheck;
	CButton mOverrideSkeletonChooser;
	CEdit mOverrideSkeletonFilename;
	afx_msg void OnBnClickedButtonchooseoverrideskeleton();
	static const float EPSILONVALUES;
	CStatic mJointModeLabel;
	CComboBox mJointMode;
	CStatic mFBXFPSLabel;
	CEdit mFBXFPS;
};
