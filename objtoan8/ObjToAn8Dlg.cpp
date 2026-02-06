
// ObjToAn8Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjToAn8.h"
#include "ObjToAn8Dlg.h"
#include "MathGeoLib.h"
#include <regex>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObjToAn8Dlg dialog



CObjToAn8Dlg::CObjToAn8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjToAn8Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	doMessageBoxes = true;
}

void CObjToAn8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mInputFilename);
	DDX_Control(pDX, IDC_EDIT2, mOutputFilename);
	DDX_Control(pDX, IDC_CHECKSPECIALKEYWORD, mSpecialKeywordMode);
	DDX_Control(pDX, IDC_CHECKMERGELIKEMATERIALS, mMergeLikeMaterials);
	DDX_Control(pDX, IDC_CHECKMERGELIKEMATERIALS2, mRenameMaterialsToTextureNames);
	DDX_Control(pDX, IDC_CHECKMERGELIKEMATERIALS3, mRenameGroups);
	DDX_Control(pDX, IDC_CHECKMERGELIKEMATERIALS4, mStripMtlImagePaths);
	DDX_Control(pDX, IDC_CHECKMERGELIKEMATERIALS5, mRound);
	DDX_Control(pDX, IDC_CHECKIGNORESHADING, mIgnoreShading);
	DDX_Control(pDX, IDC_CHECKROUNDVERTICESTENTHS, mRoundTenths);
	DDX_Control(pDX, IDC_CHECKRECENTEROBJECTS, mRecenterObjects);
	DDX_Control(pDX, IDC_CHECKSCALEVERTICES, mScaleVerticesCheck);
	DDX_Control(pDX, IDC_EDITSCALEVERTICES, mScaleVerticesEdit);
	DDX_Control(pDX, IDC_CHECKNOGROUPS, mCheckNoGroups);
	DDX_Control(pDX, IDC_EDITMATERIALREPLACES, mReplacesFilename);
	DDX_Control(pDX, IDC_CHECKREPLACE, mDoReplaceFile);
	DDX_Control(pDX, IDC_CHECKFIXNIKAKS, mCheckFixNiKaKsNs);
	DDX_Control(pDX, IDC_CHECKSCALEDIFFUSESHADING, mCheckScaleDiffuseShading);
	DDX_Control(pDX, IDC_EDITSCALEDIFFUSESHADING, mScaleDiffuseShade);
	DDX_Control(pDX, IDC_CHECKSCALEDIFFUSESHADING2, mMergeHierarchicalGroupsToEnd);
	DDX_Control(pDX, IDC_CHECKIGNORESHADING2, mIgnorePoint7Shades);
	DDX_Control(pDX, IDC_EDITX, mXMove);
	DDX_Control(pDX, IDC_EDITX2, mYMove);
	DDX_Control(pDX, IDC_EDITX3, mZMove);
	DDX_Control(pDX, IDC_CHECKMOVEPOINTS, mCheckMovePoints);
	DDX_Control(pDX, IDC_CHECKSCALESPECULARSHADING, mCheckScaleSpecularShading);
	DDX_Control(pDX, IDC_CHECKAMBIENTSHADING, mCheckScaleAmbientShading);
	DDX_Control(pDX, IDC_EDITSPECULARSHADING, mScaleSpecularShade);
	DDX_Control(pDX, IDC_EDITAMBIENTSHADING, mScaleAmbientShade);
	DDX_Control(pDX, IDC_EDITSCALEAMBIENTFACTOR, mScaleAmbientFactor);
	DDX_Control(pDX, IDC_EDITSCALEDIFFUSEFACTOR, mScaleDiffuseFactor);
	DDX_Control(pDX, IDC_CHECKSCALEAMBIENTFACTOR, mCheckScaleAmbientFactor);
	DDX_Control(pDX, IDC_CHECKSCALEDIFFUSEFACTOR, mCheckScaleDiffuseFactor);
	DDX_Control(pDX, IDC_CHECKDISABLEAUTOSHADING, mDisableAutoshading);
	DDX_Control(pDX, IDC_CHECKDEFAULTSHADING, mDefaultShading);
	DDX_Control(pDX, IDC_CHECKNOGROUPS2, mPrimarySecondaryGroups);
	DDX_Control(pDX, IDC_CHECKAUTOCALCUVS, mRecalculateUVs);
	DDX_Control(pDX, IDC_EDITSCALEVERTICES2, mScaleUVsEdit);
	DDX_Control(pDX, IDC_CHECKSCALEUVS, mScaleUVs);
	DDX_Control(pDX, IDC_CHECKSPECIFYTILESIZE, mSpecifyTileSizeCheck);
	DDX_Control(pDX, IDC_EDITSCALEVERTICES3, mTileSizeU);
	DDX_Control(pDX, IDC_EDITSCALEVERTICES4, mTileSizeV);
	DDX_Control(pDX, IDC_CHECKREGEXGROUPFILTER, mRegexGroupFilterCheck);
	DDX_Control(pDX, IDC_EDITGROUPFILTER, mRegexGroupFilter);
	DDX_Control(pDX, IDC_CHECKROUNDVERTICESHUNDREDTHS, mRoundHundredths);
	DDX_Control(pDX, IDC_GROUPFBXEXPORTER, mFbxExporterGroup);
	DDX_Control(pDX, IDC_LABELFBXEXPORTTYPE, mFbxExportTypeLabel);
	DDX_Control(pDX, IDC_COMBOFBXEXPORTER, mFbxExportType);
	DDX_Control(pDX, IDC_CHECKOVERRIDESKELETON, mOverrideSkeletonCheck);
	DDX_Control(pDX, IDC_BUTTONCHOOSEOVERRIDESKELETON, mOverrideSkeletonChooser);
	DDX_Control(pDX, IDC_EDITOVERRIDESKELETON, mOverrideSkeletonFilename);
	DDX_Control(pDX, IDC_JOINTMODELABEL, mJointModeLabel);
	DDX_Control(pDX, IDC_COMBOJOINTSRT, mJointMode);
	DDX_Control(pDX, IDC_LABELFBXEXPORTTYPE2, mFBXFPSLabel);
	DDX_Control(pDX, IDC_EDIT3, mFBXFPS);
	DDX_Control(pDX, IDC_CHECKSORTTEXTURESWITHINGROUP, mSortTexturesWithinGroup);
	DDX_Control(pDX, IDC_CHECKOBJOUTPUTVCOLORS, mCheckOutputObjVColors);
}

BEGIN_MESSAGE_MAP(CObjToAn8Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CObjToAn8Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjToAn8Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjToAn8Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CObjToAn8Dlg::OnBnClickedOk)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTONCHOOSEREPLACES, &CObjToAn8Dlg::OnBnClickedButtonchoosereplaces)
	ON_BN_CLICKED(IDC_CHECKDISABLEAUTOSHADING, &CObjToAn8Dlg::OnBnClickedCheckdisableautoshading)
	ON_BN_CLICKED(IDC_CHECKDEFAULTSHADING, &CObjToAn8Dlg::OnBnClickedCheckdefaultshading)
	ON_BN_CLICKED(IDC_CHECKIGNORESHADING, &CObjToAn8Dlg::OnBnClickedCheckignoreshading)
	ON_BN_CLICKED(IDC_CHECKIGNORESHADING2, &CObjToAn8Dlg::OnBnClickedCheckignoreshading2)
	ON_BN_CLICKED(IDC_BUTTONCHOOSEOVERRIDESKELETON, &CObjToAn8Dlg::OnBnClickedButtonchooseoverrideskeleton)
END_MESSAGE_MAP()

struct less_than_CAnimationPart
{
	inline bool operator() (const CAnimationPart* struct1, const CAnimationPart* struct2)
	{
		return (atoi(struct1->name) < atoi(struct2->name));
	}
};

struct less_than_CJoint
{
	inline bool operator() (const CJoint* struct1, const CJoint* struct2)
	{
		return (atoi(struct1->name) < atoi(struct2->name));
	}
};

struct less_than_PolygonMaterial
{
	inline bool operator() (const CPolygon* polygon1, const CPolygon* polygon2)
	{
		if (polygon1->materialName.Find("Untextured") == 0)
		{
			if (polygon2->materialName.Find("Untextured") == 0)
			{
				return (polygon1->materialName < polygon2->materialName);
			}
			else
			{
				return true;
			}
		}
		else if (polygon2->materialName.Find("Untextured") == 0)
		{
			return false;
		}
		else if ((polygon1->materialName.Find("Transparent") != -1) || (polygon1->materialName.Find("TopFlag") != -1) || (polygon1->materialName.Find("Decal") != -1))
		{
			if ((polygon2->materialName.Find("Transparent") != -1) || (polygon2->materialName.Find("TopFlag") != -1) || (polygon2->materialName.Find("Decal") != -1))
			{
				return (polygon1->materialName < polygon2->materialName);
			}
			else
			{
				return false;
			}
		}
		else if ((polygon2->materialName.Find("Transparent") != -1) || (polygon2->materialName.Find("TopFlag") != -1) || (polygon2->materialName.Find("Decal") != -1))
		{
			return true;
		}
		else
		{
			return (polygon1->materialName < polygon2->materialName);
		}
	}
};

// CObjToAn8Dlg message handlers
const float CObjToAn8Dlg::EPSILONVALUES = 0.001;

BOOL CObjToAn8Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText("ObjToAn8 " __DATE__);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mSpecialKeywordMode.SetCheck(startupParameters.specialKeywordMode);
	mMergeLikeMaterials.SetCheck(startupParameters.mergeLikeMaterials);
	mRenameMaterialsToTextureNames.SetCheck(startupParameters.renameMaterials);
	mRenameGroups.SetCheck(startupParameters.renameGroups);
	mStripMtlImagePaths.SetCheck(startupParameters.stripImagePaths);
	mRound.SetCheck(startupParameters.roundVertices);
	mRoundTenths.SetCheck(startupParameters.roundVerticesTenths);
	mRoundHundredths.SetCheck(startupParameters.roundVerticesHundredths);
	mIgnoreShading.SetCheck(startupParameters.ignoreShading);
	mIgnorePoint7Shades.SetCheck(startupParameters.ignoreShadingPoint7);
	mRecenterObjects.SetCheck(startupParameters.recenterObjects);
	mScaleVerticesCheck.SetCheck(startupParameters.scaleVertices);
	mCheckNoGroups.SetCheck(startupParameters.noGroups);
	mPrimarySecondaryGroups.SetCheck(startupParameters.primarySecondaryGroups);
	mDoReplaceFile.SetCheck(startupParameters.useReplacementFile);
	mCheckFixNiKaKsNs.SetCheck(startupParameters.fixKsKaNs);
	mCheckScaleDiffuseShading.SetCheck(startupParameters.scaleDiffuse);
	mCheckScaleAmbientShading.SetCheck(startupParameters.scaleAmbient);
	mCheckScaleSpecularShading.SetCheck(startupParameters.scaleSpecular);
	mMergeHierarchicalGroupsToEnd.SetCheck(startupParameters.mergeHierarchicalGroupsToEnd);
	mCheckMovePoints.SetCheck(startupParameters.moveCoords);
	mCheckScaleAmbientFactor.SetCheck(startupParameters.scaleAmbientFactorAn8);
	mCheckScaleDiffuseFactor.SetCheck(startupParameters.scaleDiffuseFactorAn8);
	mDisableAutoshading.SetCheck(startupParameters.disableAutoshading);
	mDefaultShading.SetCheck(startupParameters.defaultShading);
	mRecalculateUVs.SetCheck(startupParameters.recalculateUVs);
	mScaleUVs.SetCheck(startupParameters.scaleUVs);
	mSpecifyTileSizeCheck.SetCheck(startupParameters.specifyUVTileSize);
	mRegexGroupFilterCheck.SetCheck(startupParameters.specifyUVTileSize);

	CString tempStr;
	tempStr.Format("%.3f", startupParameters.scaleVerticesFactor);
	mScaleVerticesEdit.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleDiffuseFactor);
	mScaleDiffuseShade.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleSpecularFactor);
	mScaleSpecularShade.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleAmbientFactor);
	mScaleAmbientShade.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleAmbientFactorAn8Value);
	mScaleAmbientFactor.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleDiffuseFactorAn8Value);
	mScaleDiffuseFactor.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.scaleUVsFactor);
	mScaleUVsEdit.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.xMove);
	mXMove.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.yMove);
	mYMove.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.zMove);
	mZMove.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.texelSizeU);
	mTileSizeU.SetWindowText(tempStr);

	tempStr.Format("%.3f", startupParameters.texelSizeV);
	mTileSizeV.SetWindowText(tempStr);

	mRegexGroupFilter.SetWindowText(startupParameters.regexFilter);
	mJointMode.SelectString(0, "Absolute");

	mFBXFPS.SetWindowText("30");

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);

	char iniFile[8020];
	strcpy(iniFile, tempFolder);
	strcat(iniFile, "\\obj2an8.ini");

	char returnStr[40000];
	if (startupParameters.inputFile == "")
	{
		GetPrivateProfileString("Settings", "inputFilename", "C:\\temp\\test.obj", returnStr, 40000, iniFile);
		mInputFilename.SetWindowText(returnStr);
	}
	else
	{
		mInputFilename.SetWindowText(startupParameters.inputFile);
	}

	if (startupParameters.outputFile == "")
	{
		GetPrivateProfileString("Settings", "outputFilename", "C:\\temp\\test.an8", returnStr, 40000, iniFile);
		mOutputFilename.SetWindowText(returnStr);
	}
	else
	{
		mOutputFilename.SetWindowText(startupParameters.outputFile);
	}

	if (startupParameters.replaceFile == "")
	{
		GetPrivateProfileString("Settings", "replaceFile", "C:\\temp\\test.rep", returnStr, 40000, iniFile);
		mReplacesFilename.SetWindowText(returnStr);
	}
	else
	{
		mReplacesFilename.SetWindowText(startupParameters.replaceFile);
	}

	if (startupParameters.overrideSkeletonFile == "")
	{
		GetPrivateProfileString("Settings", "overrideSkeletonFile", "C:\\temp\\skeleton.obj", returnStr, 40000, iniFile);
		mOverrideSkeletonFilename.SetWindowText(returnStr);
	}
	else
	{
		mOverrideSkeletonFilename.SetWindowText(startupParameters.replaceFile);
	}

	#ifdef FBXSDK_NEW_API
	mFbxExporterGroup.ShowWindow(SW_SHOW);
	mFbxExportTypeLabel.ShowWindow(SW_SHOW);
	mFbxExportType.ShowWindow(SW_SHOW);
	mOverrideSkeletonCheck.ShowWindow(SW_SHOW);
	mOverrideSkeletonChooser.ShowWindow(SW_SHOW);
	mOverrideSkeletonFilename.ShowWindow(SW_SHOW);
	mJointModeLabel.ShowWindow(SW_SHOW);
	mJointMode.ShowWindow(SW_SHOW);
	mFBXFPSLabel.ShowWindow(SW_SHOW);
	mFBXFPS.ShowWindow(SW_SHOW);

	mFbxExportType.ResetContent();

	FbxManager* pSdkManager = FbxManager::Create();
	FbxExporter* lExporter = FbxExporter::Create(pSdkManager, "");

	int pFileFormat = -1;
    if( pFileFormat < 0 || pFileFormat >= pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
    {
        // Write in fall back format if pEmbedMedia is true
        pFileFormat = pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();
            
        int lFormatIndex, lFormatCount = pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

        for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
        {
            if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
            {
                FbxString lDesc = pSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
                
				if (lDesc.Find("encrypt") == -1)
				{
					mFbxExportType.AddString(lDesc);
				}
            }
        }
    }

	lExporter->Destroy();
	pSdkManager->Destroy();

	CString fbxExportType;
	GetPrivateProfileString("Settings", "fbxExportType", "FBX ascii", returnStr, 40000, iniFile);
	fbxExportType = returnStr;

	if (mFbxExportType.FindString(0, fbxExportType) == -1)
		fbxExportType = "FBX ascii";

	mFbxExportType.SelectString(0, fbxExportType);
	#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CObjToAn8Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CObjToAn8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CObjToAn8Dlg::OnBnClickedButton1()
{
	#ifdef FBXSDK_NEW_API
	CFileDialog m_ldFile(TRUE, NULL, "test", OFN_HIDEREADONLY, "Wavefront Obj Format (*.obj)|*.obj|FBX Format(*.fbx)|*.fbx|Anim8or Format(*.an8)|*.an8|Collada Format(*.dae)|*.dae|Biovision Hierarchy(*.bvh)|*.bvh|", this);
	#else
	CFileDialog m_ldFile(TRUE, NULL, "test", OFN_HIDEREADONLY, "Wavefront Obj Format (*.obj)|*.obj|Anim8or Format(*.an8)|*.an8|Collada Format(*.dae)|*.dae|Biovision Hierarchy(*.bvh)|*.bvh|", this);
	#endif

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	if (statusFileOpen == IDOK)
	{
		mInputFilename.SetWindowText(m_ldFile.GetPathName());
	}
}

void CObjToAn8Dlg::OnBnClickedButton2()
{
	CFileDialog m_svFile(FALSE, NULL, "test", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		#ifdef FBXSDK_NEW_API
		"Wavefront Obj Format (*.obj)|*.obj|FBX Format(*.fbx)|*.fbx|Anim8or Format(*.an8)|*.an8|Biovision Hierarchy(*.bvh)|*.bvh|", this);
		#else
		"Wavefront Obj Format (*.obj)|*.obj|Anim8or Format(*.an8)|*.an8|Biovision Hierarchy(*.bvh)|*.bvh|", this);
		#endif

	int statusFileOpen = (int) m_svFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_svFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	if (statusFileOpen == IDOK)
	{
		mOutputFilename.SetWindowText(m_svFile.GetPathName());
	}
}

int CObjToAn8Dlg::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

int CObjToAn8Dlg::GetAddVerticeColorIndex(std::vector<CVerticeColor*>& verticeColors, float r, float g, float b, float a)
{
	std::vector<CVerticeColor*>::iterator	iterVerticeColor;
	int verticeColorIndex = 0;
	for (iterVerticeColor = verticeColors.begin(); iterVerticeColor != verticeColors.end(); iterVerticeColor++)
	{
		CVerticeColor* verticeColor = (CVerticeColor*)*iterVerticeColor;

		if ((verticeColor->color.r == r) && (verticeColor->color.g == g) && (verticeColor->color.b == b) && (verticeColor->color.a == a))
		{
			return verticeColorIndex;
		}
		verticeColorIndex++;
	}

	CVerticeColor* newVerticeColor = new CVerticeColor();
	newVerticeColor->color.r = Round(r * 255.0f);
	newVerticeColor->color.g = Round(g * 255.0f);
	newVerticeColor->color.b = Round(b * 255.0f);
	newVerticeColor->color.a = Round(a * 255.0f);
	newVerticeColor->contains = true;
	verticeColors.push_back(newVerticeColor);
	return (verticeColors.size() - 1);
}

int CObjToAn8Dlg::GetAddNormalIndex(std::vector<CNormal*>& normals, float nx, float ny, float nz)
{
	std::vector<CNormal*>::iterator	iterNormal;
	int normalIndex = 0;
	for (iterNormal = normals.begin(); iterNormal != normals.end(); iterNormal++)
	{
		CNormal* normal = (CNormal*)*iterNormal;

		if ((normal->nx == nx) && (normal->ny == ny) && (normal->nz == nz))
		{
			return normalIndex;
		}
		normalIndex++;
	}

	CNormal* newNormal = new CNormal();
	newNormal->nx = nx;
	newNormal->ny = ny;
	newNormal->nz = nz;
	normals.push_back(newNormal);
	return (normals.size() - 1);
}

int CObjToAn8Dlg::GetAddUVIndex(std::vector<CUVCoordinate*>& uvCoordinates, float uvalue, float vvalue)
{
	std::vector<CUVCoordinate*>::iterator	iterUVList;
	int uvIndex = 0;
	for (iterUVList = uvCoordinates.begin(); iterUVList != uvCoordinates.end(); iterUVList++)
	{
		CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;

		if ((uv->u == uvalue) && (uv->v == vvalue))
		{
			return uvIndex;
		}
		uvIndex++;
	}

	CUVCoordinate* newUVCoordinate = new CUVCoordinate();
	newUVCoordinate->u = uvalue;
	newUVCoordinate->v = vvalue;
	uvCoordinates.push_back(newUVCoordinate);
	return (uvCoordinates.size() - 1);
}

bool CObjToAn8Dlg::ReadObjFile(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
							   bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors,
							   bool noGroups)
{
	CString currentObjFolder = inputFile.Mid(0, ((inputFile.ReverseFind('\\')+1)));

	FILE* inFile = fopen(inputFile, "r");
	if (inFile == NULL)
	{
		if (doMessageBoxes)
			MessageBox("Bad input file, can't open");
		return false;
	}

	CGroup* defaultGroup = new CGroup();
	defaultGroup->name = "DefaultUnassigned";

	groups.push_back(defaultGroup);

	CGroup* currentGroup = defaultGroup;

	char currentLine[40000];

	CMaterialFile* defaultMaterialFile = new CMaterialFile();
	defaultMaterialFile->name = "DefaultUnassigned";
	materialFiles.push_back(defaultMaterialFile);

	CMaterialFile* currentMaterialFile = defaultMaterialFile;

	CString errorString = "";

	CString currentMaterialName = "";

	CPolygon* lastPolygon = NULL;
	CJoint* lastJoint = NULL;

	CAnimation* lastAnimation = NULL;
	CKeyframe* lastKeyframe = NULL;
	CAnimationPart* lastAnimationPart = NULL;

	JointType jointType = Absolute;

	// generate precounts
	while (!feof(inFile))
	{
		for (int rz = 0; rz < 40000; rz++)
			currentLine[rz] = 0;

		fgets(currentLine, 40000, inFile);

		for (int x = 0; x < 40000; x++)
		{
			if (currentLine[x] == 0xA)
				currentLine[x] = 0;
		}

		CString lineString;
		lineString.Format("%s", currentLine);

		lineString = lineString.Trim();

		if (lineString.Find("#jointmode ") == 0)
		{
			CString jointModeRead = lineString.Mid(11).Trim().MakeLower();
			if (jointModeRead == "relative")
			{
				jointType = Relative;							
			}
		}
		else if ((lineString.Find("o ") == 0) || (lineString.Find("g ") == 0))
		{
			if (!noGroups)
			{
				CGroup* newGroup = new CGroup();
				newGroup->name = lineString.Mid(2);

				groups.push_back(newGroup);

				currentGroup = newGroup;
			}
		}
		else if (lineString.Find("mtllib ") == 0)
		{
			CString materialFileName = lineString.Mid(7);

			std::vector<CMaterialFile*>::iterator	iter;
			for (iter = materialFiles.begin(); iter != materialFiles.end(); iter++)
			{
				if (((CMaterialFile*)(*iter))->name == materialFileName)
				{
					currentMaterialFile = (CMaterialFile*)*iter;
				}
			}

			CMaterialFile* newMaterialFile = new CMaterialFile();
			newMaterialFile->name = materialFileName;

			materialFiles.push_back(newMaterialFile);

			currentMaterialFile = newMaterialFile;


			FILE* tempMaterial;
			if (materialFileName.Find("\\") == -1)
				tempMaterial = fopen(currentObjFolder+materialFileName, "r");
			else
				tempMaterial = fopen(materialFileName, "r");
			if (tempMaterial == NULL)
			{
				errorString += ("Missing Material " + materialFileName);
				continue;
			}

			CMaterial* defaultMaterialAdd = new CMaterial();
			defaultMaterialAdd->name = "DefaultMaterial";
			newMaterialFile->materials.push_back(defaultMaterialAdd);

			CMaterial* currentMaterial = defaultMaterialAdd;

			char currentCharLineTempSub[40000];

			while (!feof(tempMaterial))
			{
				for (int rz = 0; rz < 40000; rz++)
					currentCharLineTempSub[rz] = 0;
				fgets(currentCharLineTempSub, 40000, tempMaterial);
				for (int x = 0; x < 40000; x++)
				{
					if (currentCharLineTempSub[x] == 0xA)
						currentCharLineTempSub[x] = 0;
				}

				CString tempLineStrTempSub;
				tempLineStrTempSub.Format("%s", currentCharLineTempSub);

				if ((currentCharLineTempSub[0] == 'n') && (currentCharLineTempSub[1] == 'e') && (currentCharLineTempSub[2] == 'w') && (currentCharLineTempSub[3] == 'm') && (currentCharLineTempSub[4] == 't') && (currentCharLineTempSub[5] == 'l'))
				{
					CMaterial* newMaterial = new CMaterial();
					newMaterial->name = tempLineStrTempSub.Mid(7);

					currentMaterialFile->materials.push_back(newMaterial);

					currentMaterial = newMaterial;
				}
				else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 'a'))
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					currentMaterial->ambientColor.r = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->ambientColor.g = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->ambientColor.b = atof(pch);
					currentMaterial->ambientColor.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 'd')) // diffuse
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					currentMaterial->diffuseColor.r = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->diffuseColor.g = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->diffuseColor.b = atof(pch);
					currentMaterial->diffuseColor.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 's'))
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					currentMaterial->spectralColor.r = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->spectralColor.g = atof(pch);
					pch = strtok (NULL, " ");
					currentMaterial->spectralColor.b = atof(pch);
					currentMaterial->spectralColor.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'd') || ((currentCharLineTempSub[0] == 'T') && (currentCharLineTempSub[1] == 'r')))
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					if (currentCharLineTempSub[0] == 'd')
						currentMaterial->alpha.floatValue = (atof(pch));
					else if ((currentCharLineTempSub[0] == 'T') && (currentCharLineTempSub[1] == 'r'))
						currentMaterial->alpha.floatValue = 1.0 - (atof(pch));
					currentMaterial->alpha.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'N') && (currentCharLineTempSub[1] == 's'))
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					currentMaterial->phongSpectral.floatValue = (atof(pch));
					currentMaterial->phongSpectral.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'i') && (currentCharLineTempSub[1] == 'l') && (currentCharLineTempSub[2] == 'l') && (currentCharLineTempSub[3] == 'u') && (currentCharLineTempSub[4] == 'm'))
				{
					char* pch = strtok (currentCharLineTempSub," ");
					pch = strtok (NULL, " ");
					currentMaterial->illum.floatValue = (atoi(pch));
					currentMaterial->illum.contains = true;
				}
				else if ((currentCharLineTempSub[0] == 'm') && (currentCharLineTempSub[1] == 'a') && (currentCharLineTempSub[2] == 'p') && (currentCharLineTempSub[3] == '_') && (currentCharLineTempSub[4] == 'K') && (currentCharLineTempSub[5] == 'd'))
				{
					currentMaterial->diffuseTexture.stringValue = tempLineStrTempSub.Mid(7);
					currentMaterial->diffuseTexture.contains = true;
				}
			}

			fclose(tempMaterial);
		}
		else if (lineString.Find("usemtl ") == 0)
		{
			currentMaterialName = lineString.Mid(7);


			bool found = false;

			std::vector<CMaterial*>::iterator	iter;
			for (iter = currentMaterialFile->materials.begin(); iter != currentMaterialFile->materials.end(); iter++)
			{
				if (((CMaterial*)(*iter))->name == currentMaterialName)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				errorString += "Material " + currentMaterialName + " not found in " + currentMaterialFile->name + " Group: " + currentGroup->name + "\r\n";
				currentMaterialName = "DefaultMaterial";
			}
		}
		else if (lineString.Find("v ") == 0)
		{
			CVertice* newVertice = new CVertice();
			
			char* pch = strtok(currentLine, " ");
			pch = strtok(NULL, " ");
			newVertice->vertex.x = (atof(pch));
			pch = strtok(NULL, " ");
			newVertice->vertex.y = (atof(pch));
			pch = strtok(NULL, " ");
			newVertice->vertex.z = (atof(pch));

			pch = strtok(NULL, " ");
			// included color in extended spec http://paulbourke.net/dataformats/obj/colour.html
			if (pch != NULL)
			{
				newVertice->vertexColor.x = (atof(pch));
				if (newVertice->vertexColor.x > 1)
					newVertice->vertexColor.x = 1;
				if (newVertice->vertexColor.x < 0)
					newVertice->vertexColor.x = 0;
				pch = strtok(NULL, " ");
				if (pch != NULL)
				{
					newVertice->vertexColor.y = (atof(pch));
					if (newVertice->vertexColor.y > 1)
						newVertice->vertexColor.y = 1;
					if (newVertice->vertexColor.y < 0)
						newVertice->vertexColor.y = 0;
					pch = strtok(NULL, " ");
					if (pch != NULL)
					{
						newVertice->vertexColor.z = (atof(pch));
						if (newVertice->vertexColor.z > 1)
							newVertice->vertexColor.z = 1;
						if (newVertice->vertexColor.z < 0)
							newVertice->vertexColor.z = 0;
						newVertice->hasVertexColor = true;
					}
				}
			}

			vertices.push_back(newVertice);
		}
		else if (lineString.Find("vt ") == 0)
		{
			CUVCoordinate* newUv = new CUVCoordinate();

			char* pch = strtok (currentLine," ");
			pch = strtok (NULL, " ");
			newUv->u = (((atof(pch))));
			pch = strtok (NULL, " ");
			newUv->v = (((atof(pch))));

			uvCoordinates.push_back(newUv);
		}
		else if (lineString.Find("#vcolor ") == 0)
		{
			CVerticeColor* newVerticeColor = new CVerticeColor();

			if (lineString.Find("null") != -1)
			{
				newVerticeColor->contains = false;
				verticeColors.push_back(newVerticeColor);
			}
			else
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				newVerticeColor->color.r = (((atof(pch))));
				pch = strtok (NULL, " ");
				newVerticeColor->color.g = (((atof(pch))));
				pch = strtok (NULL, " ");
				newVerticeColor->color.b = (((atof(pch))));

				pch = strtok (NULL, " ");
				if (pch != NULL)
					newVerticeColor->color.a = (((atof(pch))));
				else
					newVerticeColor->color.a = 255.0f;

				newVerticeColor->contains = true;

				verticeColors.push_back(newVerticeColor);
			}
		}
		else if (lineString.Find("f ") == 0)
		{
			CPolygon* newPolygon = new CPolygon();
			newPolygon->materialName = currentMaterialName;

			char* pch = strtok (currentLine," ");
			pch = strtok (NULL, " ");
			
			while (pch != NULL)
			{
				CPolygonPoint* polygonPoint = new CPolygonPoint();

				CString tempCurrent;
				tempCurrent.Format("%s", pch);

				if (tempCurrent == 0xA)
				{
					pch = strtok (NULL, " ");
					continue;
				}

				polygonPoint->verticeIndex = (atoi(pch)-1);

				bool didTexture = false;

				for (int x = 0; x < tempCurrent.GetLength(); x++)
				{
					if (pch[x] == '/')
					{
						if (pch[x+1] == '/')
						{
							if (!didTexture)
							{
								didTexture = true;
							}
							else
							{
								// normal
							}
						}
						else
						{
							if (!didTexture)
							{
								polygonPoint->uvIndex = (atoi(&pch[x+1])-1);
								foundTextureUV = true;
								didTexture = true;
							}
							else
							{
								foundNormals = true;
								polygonPoint->normalIndex = (atoi(&pch[x+1])-1);
							}
						}
					}
				}

				pch = strtok (NULL, " ");

				newPolygon->polygonPoints.push_back(polygonPoint);
			}

			std::vector<int> duplicatePoints;

			int sourceSpot = 0;
			std::vector<CPolygonPoint*>::iterator	itertestpolygonPoint;
			for (itertestpolygonPoint = newPolygon->polygonPoints.begin(); itertestpolygonPoint != newPolygon->polygonPoints.end(); itertestpolygonPoint++)
			{
				CPolygonPoint* orig = (CPolygonPoint*)*itertestpolygonPoint;

				int matchSpot = 0;
				std::vector<CPolygonPoint*>::iterator	itertestpolygonPointMatch;
				for (itertestpolygonPointMatch = newPolygon->polygonPoints.begin(); itertestpolygonPointMatch != newPolygon->polygonPoints.end(); itertestpolygonPointMatch++)
				{
					CPolygonPoint* match = (CPolygonPoint*)*itertestpolygonPointMatch;

					if (orig != match)
					{
						if (
							(vertices[orig->verticeIndex]->vertex.x == vertices[match->verticeIndex]->vertex.x)
							&& (vertices[orig->verticeIndex]->vertex.y == vertices[match->verticeIndex]->vertex.y)
							&& (vertices[orig->verticeIndex]->vertex.z == vertices[match->verticeIndex]->vertex.z))
						{
							if (std::find(duplicatePoints.begin(), duplicatePoints.end(), sourceSpot) == duplicatePoints.end())
							{
								duplicatePoints.push_back(matchSpot);
							}
						}
					}

					matchSpot++;
				}

				sourceSpot++;
			}

			// remove non-tris, caused fbx errors importing into xsi
			// maybe move to checkbox
			
			if (duplicatePoints.size()> 0)
			{
				std::sort(duplicatePoints.begin(), duplicatePoints.end(), std::greater<int>());

				for (int r = 0; r < duplicatePoints.size(); r++)
				{
					delete newPolygon->polygonPoints[duplicatePoints[r]];
					newPolygon->polygonPoints.erase(newPolygon->polygonPoints.begin() + duplicatePoints[r]);
					
				}
			}

			if (newPolygon->polygonPoints.size() < 3)
			{
				// remove < 3 point tris, invalid
				std::vector<CPolygonPoint*>::iterator	itertestpolygonPoint;
				for (itertestpolygonPoint = newPolygon->polygonPoints.begin(); itertestpolygonPoint != newPolygon->polygonPoints.end(); itertestpolygonPoint++)
				{
					CVertice* vertice = vertices[(*itertestpolygonPoint)->verticeIndex];
					delete *itertestpolygonPoint;
				}

				delete newPolygon;
			}
			else
			{
				lastPolygon = newPolygon;
				currentGroup->polygons.push_back(newPolygon);
			}
		}
		else if (lineString.Find("#fvcolorindex ") == 0)
		{
			char* pch = strtok (currentLine," ");
			pch = strtok (NULL, " ");

			int vertColorIndex = 0;

			while (pch != NULL)
			{
				CString tempCurrent;
				tempCurrent.Format("%s", pch);

				if (tempCurrent == 0xA)
				{
					pch = strtok (NULL, " ");
					continue;
				}

				CString tempPchStr;
				tempPchStr.Format("%s", pch);

				int verticeColorIndex;
				if (tempPchStr.Find("null") != -1)
				{
					verticeColorIndex = -1;
				}
				else
				{
					verticeColorIndex = (atoi(pch)-1);
				}

				
	
				pch = strtok (NULL, " ");

				if (lastPolygon != NULL)
				{
					if (vertColorIndex < lastPolygon->polygonPoints.size())
					{
						foundVerticeColors = true;
						lastPolygon->polygonPoints[vertColorIndex]->verticeColorIndex = verticeColorIndex;
					}
				}

				vertColorIndex++;
			}
		}
		else if (lineString.Find("#joint ") == 0)
		{
			CString newJointName = lineString.Mid(7);
			lastJoint = NULL;
			for (int x = 0; x < joints.size(); x++)
			{
				if (joints[x]->name == newJointName)
				{
					lastJoint = joints[x];
					break;
				}
			}

			if (lastJoint == NULL)
			{
				lastJoint = new CJoint();
				lastJoint->name = newJointName;
				lastJoint->jointType = jointType;
				joints.push_back(lastJoint);
			}
		}
		else if (lineString.Find("#jointposition ") == 0)
		{
			if (lastJoint->jointType == Absolute)
			{
				if (lastJoint != NULL)
				{
					char* pch = strtok (currentLine," ");
					pch = strtok (NULL, " ");
					lastJoint->positionAbsolute.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->positionAbsolute.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->positionAbsolute.z = (((atof(pch))));
				}
			}
		}
		else if (lineString.Find("#jointsrt ") == 0)
		{
			if (lastJoint->jointType == Relative)
			{
				if (lastJoint != NULL)
				{
					char* pch = strtok (currentLine," ");
					pch = strtok (NULL, " ");
					lastJoint->scaleRelative.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->scaleRelative.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->scaleRelative.z = (((atof(pch))));

					pch = strtok (NULL, " ");
					lastJoint->rotationRelative.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->rotationRelative.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->rotationRelative.z = (((atof(pch))));

					pch = strtok (NULL, " ");
					lastJoint->positionRelative.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->positionRelative.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastJoint->positionRelative.z = (((atof(pch))));
				}
			}
		}
		else if (lineString.Find("#connection ") == 0)
		{
			CString tempString = lineString.Mid(12);

			int nTokenPos = 0;
			CString strToken = tempString.Tokenize(_T(" "), nTokenPos);

			CString parent;
			CString child;

			int counter = 0;
			while (!strToken.IsEmpty())
			{
				if (counter == 0)
				{
					parent = strToken;
					counter++;
				}
				else
				{
					child = strToken;
					counter++;
					break;
				}
				
				strToken = tempString.Tokenize(_T(" "), nTokenPos);
			}

			if (counter == 2)
			{
				CJoint* foundParent = NULL;
				CJoint* foundChild = NULL;

				for (int x = 0; x < joints.size(); x++)
				{
					if (joints[x]->name == parent)
					{
						foundParent = joints[x];
					}

					if (joints[x]->name == child)
					{
						foundChild = joints[x];
					}

					if (foundParent && foundChild)
						break;
				}

				if (foundParent && foundChild)
				{
					if(std::find(foundParent->children.begin(), foundParent->children.end(), foundChild) == foundParent->children.end())
					{
						foundParent->children.push_back(foundChild);
					}

					foundChild->parent = foundParent;
				}
			}
		}
		else if (lineString.Find("#vjoint ") == 0)
		{
			if (lastJoint != NULL)
			{
				CString tempString = lineString.Mid(8);

				int nTokenPos = 0;
				CString strToken = tempString.Tokenize(_T(" "), nTokenPos);

				while (!strToken.IsEmpty())
				{
					int vertexNumber = atoi(strToken)-1;
					lastJoint->controlPoints.push_back(vertexNumber);

					strToken = tempString.Tokenize(_T(" "), nTokenPos);
				}
			}
		}
		else if (lineString.Find("#animation ") == 0)
		{
			CString newAnimationName = lineString.Mid(11);
			lastAnimation = NULL;
			for (int x = 0; x < animations.size(); x++)
			{
				if (animations[x]->name == newAnimationName)
				{
					lastAnimation = animations[x];
					break;
				}
			}

			if (lastAnimation == NULL)
			{
				lastAnimation = new CAnimation();
				lastAnimation->name = newAnimationName;
				lastAnimation->treatAsBlendShape = false;
				animations.push_back(lastAnimation);
			}
		}
		else if (lineString.Find("#shapeanimation ") == 0)
		{
			CString newAnimationName = lineString.Mid(16);
			lastAnimation = NULL;
			for (int x = 0; x < animations.size(); x++)
			{
				if (animations[x]->name == newAnimationName)
				{
					lastAnimation = animations[x];
					break;
				}
			}

			if (lastAnimation == NULL)
			{
				lastAnimation = new CAnimation();
				lastAnimation->name = newAnimationName;
				lastAnimation->treatAsBlendShape = true;
				animations.push_back(lastAnimation);
			}
		}
		else if (lineString.Find("#camera ") == 0)
		{
			CString newAnimationName = lineString.Mid(8);
			lastAnimation = NULL;
			for (int x = 0; x < animations.size(); x++)
			{
				if (animations[x]->name == newAnimationName)
				{
					lastAnimation = animations[x];
					break;
				}
			}

			if (lastAnimation == NULL)
			{
				lastAnimation = new CCamera();
				lastAnimation->name = newAnimationName;
				animations.push_back(lastAnimation);
			}
		}
		else if (lineString.Find("#keyframe ") == 0)
		{
			if (lastAnimation != NULL)
			{
				int newKeyframeNumber = atoi(lineString.Mid(10));
				lastKeyframe = NULL;
				for (int x = 0; x < lastAnimation->keyframes.size(); x++)
				{
					if (lastAnimation->keyframes[x]->number == newKeyframeNumber)
					{
						lastKeyframe = lastAnimation->keyframes[x];
						break;
					}
				}

				if (lastKeyframe == NULL)
				{
					lastKeyframe = new CKeyframe();
					lastKeyframe->number = newKeyframeNumber;
					lastAnimation->keyframes.push_back(lastKeyframe);
				}
			}
		}
		else if (lineString.Find("#keyframetranslation ") == 0)
		{
			if (lastAnimation != NULL)
			{
				if (lastKeyframe != NULL)
				{
					char* pch = strtok (currentLine," ");
					pch = strtok (NULL, " ");
					lastKeyframe->translation.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastKeyframe->translation.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastKeyframe->translation.z = (((atof(pch))));
				}
			}
		}
		else if (lineString.Find("#keyframeisinterpolatedtranslation ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastKeyframe->translationIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->translationIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->translationIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("#keyframerotation ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				lastKeyframe->rotation.x = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastKeyframe->rotation.y = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastKeyframe->rotation.z = (((atof(pch))));
			}
		}
		else if (lineString.Find("#keyframeisinterpolatedrotation ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastKeyframe->rotationIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->rotationIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->rotationIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("#keyframescale ") == 0)
		{
			if (lastAnimation != NULL)
			{
				if (lastKeyframe != NULL)
				{
					char* pch = strtok (currentLine," ");
					pch = strtok (NULL, " ");
					lastKeyframe->scale.x = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastKeyframe->scale.y = (((atof(pch))));
					pch = strtok (NULL, " ");
					lastKeyframe->scale.z = (((atof(pch))));
				}
			}
		}
		else if (lineString.Find("#keyframeisinterpolatedscale ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastKeyframe->scaleIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->scaleIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastKeyframe->scaleIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("#keyframecolor ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				lastKeyframe->envColor.r = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastKeyframe->envColor.g = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastKeyframe->envColor.b = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastKeyframe->envColor.a = (((atof(pch))));
				lastKeyframe->envColor.contains = true;
			}
		}
		else if (lineString.Find("#keyframefieldofview ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				lastKeyframe->fieldOfView.floatValue = atof(lineString.Mid(21));
				lastKeyframe->fieldOfView.contains = true;
			}
		}
		else if (lineString.Find("#part ") == 0)
		{
			if (lastKeyframe != NULL)
			{
				CString newAnimationPartName = lineString.Mid(6);
				lastAnimationPart = NULL;

				for (int x = 0; x < lastKeyframe->animationParts.size(); x++)
				{
					if (lastKeyframe->animationParts[x]->name == newAnimationPartName)
					{
						lastAnimationPart = lastKeyframe->animationParts[x];
						break;
					}
				}

				if (lastAnimationPart == NULL)
				{
					lastAnimationPart = new CAnimationPart();
					lastAnimationPart->name = newAnimationPartName;
					lastKeyframe->animationParts.push_back(lastAnimationPart);
				}
			}
		}
		else if (lineString.Find("#jointrotationorder ") == 0)
		{
			CString rotationOrder = lineString.Mid(20).Trim().MakeUpper();
			if (lastJoint != NULL)
			{
				if ((rotationOrder == "XYZ") || (rotationOrder == "XZY") || (rotationOrder == "YZX")
					|| (rotationOrder == "YXZ") || (rotationOrder == "ZXY") || (rotationOrder == "ZYX"))
				{
					lastJoint->rotationOrder = rotationOrder;
				}
			}
		}
		else if (lineString.Find("#partrotation ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				lastAnimationPart->rotation.x = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->rotation.y = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->rotation.z = (((atof(pch))));
			}
		}
		else if (lineString.Find("#partisinterpolatedrotation ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastAnimationPart->rotationIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->rotationIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->rotationIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("#partscale ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				lastAnimationPart->scale.x = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->scale.y = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->scale.z = (((atof(pch))));
			}
		}
		else if (lineString.Find("#partisinterpolatedscale ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastAnimationPart->scaleIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->scaleIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->scaleIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("#parttranslation ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok (currentLine," ");
				pch = strtok (NULL, " ");
				lastAnimationPart->translation.x = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->translation.y = (((atof(pch))));
				pch = strtok (NULL, " ");
				lastAnimationPart->translation.z = (((atof(pch))));
			}
		}
		else if (lineString.Find("#partshapetranslation ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				CAnimationShapeVertex animationShapeVertex;
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				animationShapeVertex.vertexIndex = (((atoi(pch)))) - 1;
				pch = strtok(NULL, " ");
				animationShapeVertex.translation.x = (((atof(pch))));
				pch = strtok(NULL, " ");
				animationShapeVertex.translation.y = (((atof(pch))));
				pch = strtok(NULL, " ");
				animationShapeVertex.translation.z = (((atof(pch))));

				lastAnimationPart->shapeVertices.push_back(animationShapeVertex);
			}
		}
		else if (lineString.Find("#partisinterpolatedtranslation ") == 0)
		{
			if (lastAnimationPart != NULL)
			{
				char* pch = strtok(currentLine, " ");
				pch = strtok(NULL, " ");
				lastAnimationPart->translationIsInterpolated[0] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->translationIsInterpolated[1] = (bool)atoi(pch);
				pch = strtok(NULL, " ");
				lastAnimationPart->translationIsInterpolated[2] = (bool)atoi(pch);
			}
		}
		else if (lineString.Find("vn ") == 0)
		{
			CNormal* newNormal = new CNormal();

			char* pch = strtok (currentLine," ");
			pch = strtok (NULL, " ");
			newNormal->nx = (((atof(pch))));
			pch = strtok (NULL, " ");
			newNormal->ny = (((atof(pch))));
			pch = strtok (NULL, " ");
			newNormal->nz = (((atof(pch))));

			normals.push_back(newNormal);
		}
	}
	
	fclose(inFile);

	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			bool allAssigned = true;
			int countAdded = 0;
			std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
			for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
			{
				CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);

				if (polygonPoint->verticeColorIndex == -1)
				{
					if (polygonPoint->verticeIndex < vertices.size())
					{
						if (vertices[polygonPoint->verticeIndex]->hasVertexColor)
						{
							CVerticeColor* newVerticeColor = new CVerticeColor();
							newVerticeColor->color.r = vertices[polygonPoint->verticeIndex]->vertexColor.x * 255.0;
							newVerticeColor->color.g = vertices[polygonPoint->verticeIndex]->vertexColor.y * 255.0;
							newVerticeColor->color.b = vertices[polygonPoint->verticeIndex]->vertexColor.z * 255.0;
							newVerticeColor->color.a = 255.0;
							newVerticeColor->contains = true;
							polygonPoint->verticeColorIndex = verticeColors.size();
							verticeColors.push_back(newVerticeColor);
							countAdded++;
						}
						else
						{
							allAssigned = false;
							break;
						}
					}
				}
			}

			if ((countAdded > 0) && !allAssigned) // Can't just do partial
			{
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);
					polygonPoint->verticeColorIndex = -1;
				}

				for (int x = 0; x < countAdded; x++)
					verticeColors.pop_back();
			}
		}
	}

	if (errorString != "")
	{
		FILE* outFileErr = fopen(inputFile + "errors.txt", "w");
		if (outFileErr != NULL)
		{
			fprintf(outFileErr, "%s", errorString);
			fclose(outFileErr);
		}
		if (doMessageBoxes)
			MessageBox(errorString);
	}

	return true;
}

void CObjToAn8Dlg::ParseAssimpNodesRecursive(CString extension, const aiScene* scene, aiNode* node, std::vector<CJoint*>& joints, aiVector3t<float> position, CJoint* parent, CMaterialFile* currentMaterialFile, CGroup* currentGroup, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, bool& foundNormals, bool& foundTextureUV, bool& foundVerticeColors, bool noGroups)
{
	CString nodeName = node->mName.data;

	if ((extension == "bvh") && (nodeName.Find("EndSite") == 0))
		return;

	if (node->mNumMeshes == 0) // assume animation
	{
		if (nodeName.Find("TopJoint") == -1)
		{
			CJoint* joint = new CJoint();
			joint->name = nodeName;

			aiVector3t<float> positionLocal(0, 0, 0);
			aiVector3t<float> scaling(0, 0, 0);
			aiQuaterniont<float> rotation(0, 0, 0, 1);
			node->mTransformation.Decompose(scaling, rotation, positionLocal);

			position.x += positionLocal.x;
			position.y += positionLocal.y;
			position.z += positionLocal.z;

			joint->positionAbsolute.x = position.x;
			joint->positionAbsolute.y = position.y;
			joint->positionAbsolute.z = position.z;

			joint->parent = parent;

			if (parent != NULL)
			{
				parent->children.push_back(joint);
			}

			parent = joint;

			joints.push_back(joint);
		}
	}
	else
	{
		if (!noGroups)
		{
			if (nodeName != "")
			{
				CGroup* newGroup = new CGroup();
				newGroup->name = nodeName;
				groups.push_back(newGroup);
				currentGroup = newGroup;
			}
		}

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (int x = 0 ; x < node->mNumMeshes; x++) 
		{
			const aiMesh* paiMesh = scene->mMeshes[node->mMeshes[x]];

			bool hasBones = paiMesh->HasBones();

			int meshVerticeOffset = vertices.size();
			int meshUvOffset = uvCoordinates.size();
			int meshNormalsOffset = normals.size();
			int meshVerticeColorsOffset = verticeColors.size();

			if (hasBones)
			{
				for (int y = 0; y < paiMesh->mNumBones; y++)
				{
					CString boneName = paiMesh->mBones[y]->mName.data;

					if (boneName.Find(":") != -1)
					{
						boneName = boneName.Mid(boneName.Find(":") + 1);
					}

					if (boneName.Find("_") == 0)
						boneName = boneName.Mid(1);

					CJoint* joint = NULL;
					for (int z = 0; z < joints.size(); z++)
					{
						if (joints[z]->name == boneName)
						{
							joint = joints[z];
							break;
						}
					}

					if (joint != NULL)
					{
						for (int z = 0; z < paiMesh->mBones[y]->mNumWeights; z++)
						{
							joint->controlPoints.push_back(meshVerticeOffset + paiMesh->mBones[y]->mWeights[z].mVertexId);
						}
					}
				}
			}

			for (unsigned int x = 0; x < paiMesh->mNumVertices; x++) 
			{
				const aiVector3D* pPos = &(paiMesh->mVertices[x]);

				CVertice* newVertice = new CVertice();
					
				newVertice->vertex.x = pPos->x;
				newVertice->vertex.y = pPos->y;
				newVertice->vertex.z = pPos->z;

				for (int r = 0; r < joints.size(); r++)
				{
					if ((std::find(joints[r]->controlPoints.begin(), joints[r]->controlPoints.end(), (meshVerticeOffset+x)) != joints[r]->controlPoints.end()))
					{
						newVertice->vertex.x += joints[r]->positionAbsolute.x;
						newVertice->vertex.y += joints[r]->positionAbsolute.y;
						newVertice->vertex.z += joints[r]->positionAbsolute.z;
						break;
					}
				}

				vertices.push_back(newVertice);
			}

			if (paiMesh->HasTextureCoords(0))
			{
				foundTextureUV = true;
				for (unsigned int x = 0; x < paiMesh->mNumVertices; x++) 
				{
					const aiVector3D* pTexCoord = &(paiMesh->mTextureCoords[0][x]);

					if (pTexCoord == NULL)
					{
						pTexCoord = &Zero3D;
					}

					CUVCoordinate* newUv = new CUVCoordinate();

					newUv->u = pTexCoord->x;
					newUv->v = pTexCoord->y;
					
					uvCoordinates.push_back(newUv);
				}
			}

			if (paiMesh->HasNormals())
			{
				foundNormals = true;

				for (unsigned int x = 0; x < paiMesh->mNumVertices; x++) 
				{
					const aiVector3D* pNormal = &(paiMesh->mNormals[x]);
					if (pNormal == NULL)
						pNormal = &Zero3D;

					CNormal* newNormal = new CNormal();

					newNormal->nx = pNormal->x;
					newNormal->ny = pNormal->y;
					newNormal->nz = pNormal->z;

					normals.push_back(newNormal);	
				}
			}

			if (paiMesh->HasVertexColors(0))
			{
				foundVerticeColors = true;

				for (unsigned int x = 0; x < paiMesh->mNumVertices; x++) 
				{
					CVerticeColor* newVerticeColor = new CVerticeColor();

					newVerticeColor->color.r = Round(paiMesh->mColors[0][x].r * 255.0f);
					newVerticeColor->color.g = Round(paiMesh->mColors[0][x].g * 255.0f);
					newVerticeColor->color.b = Round(paiMesh->mColors[0][x].b * 255.0f);
					newVerticeColor->color.a = Round(paiMesh->mColors[0][x].a * 255.0f);
					newVerticeColor->contains = true;

					verticeColors.push_back(newVerticeColor);
				}
			}

			if (paiMesh->HasFaces())
			{
				for (unsigned int x = 0; x < paiMesh->mNumFaces; x++) 
				{
					const aiFace& Face = paiMesh->mFaces[x];

					CPolygon* newPolygon = new CPolygon();

					int materialNumber = paiMesh->mMaterialIndex;

					newPolygon->materialName = currentMaterialFile->materials[materialNumber]->name;

					for (int w = 0; w < Face.mNumIndices; w++)
					{
						CPolygonPoint* polygonPoint = new CPolygonPoint();

						polygonPoint->verticeIndex = meshVerticeOffset + Face.mIndices[w];

						if (paiMesh->HasNormals())
						{
							polygonPoint->normalIndex = meshNormalsOffset + Face.mIndices[w];
						}

						if (paiMesh->HasTextureCoords(0))
						{
							polygonPoint->uvIndex = meshUvOffset + Face.mIndices[w];
						}

						if (paiMesh->HasVertexColors(0))
						{
							polygonPoint->verticeColorIndex = meshVerticeColorsOffset + Face.mIndices[w];
						}


						newPolygon->polygonPoints.push_back(polygonPoint);
					}

					currentGroup->polygons.push_back(newPolygon);
				}
			}

			if (paiMesh->HasNormals())
			{
				meshNormalsOffset += paiMesh->mNumVertices;
			}

			if (paiMesh->HasTextureCoords(0))
			{
				meshUvOffset += paiMesh->mNumVertices;
			}

			if (paiMesh->HasVertexColors(0))
			{
				meshVerticeColorsOffset += paiMesh->mNumVertices;
			}

			meshVerticeOffset += paiMesh->mNumVertices;
		}
	}

	for (int x = 0; x < node->mNumChildren; x++)
	{
		ParseAssimpNodesRecursive(extension, scene, node->mChildren[x], joints, position, parent, currentMaterialFile, currentGroup, verticeColors, normals, uvCoordinates, vertices, groups, foundNormals, foundTextureUV, foundVerticeColors, noGroups);
	}
}

bool CObjToAn8Dlg::ReadAssimpFile(CString extension, CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups)
{
	CString inputFileData;

	std::ifstream inFile(inputFile);
	if (inFile) 
	{
		inputFileData = std::string( ( std::istreambuf_iterator<char> (inFile) ) ,std::istreambuf_iterator<char>()).c_str();
		inFile.close();
	}
	else 
	{
		return false;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(inputFile, 0);

	if (scene == NULL)
	{	
		CString errorString = importer.GetErrorString();
		if (doMessageBoxes)
			MessageBox(errorString);
		return false;
	}

	CGroup* defaultGroup = new CGroup();
	defaultGroup->name = "DefaultUnassigned";

	groups.push_back(defaultGroup);

	CGroup* currentGroup = defaultGroup;

	CMaterialFile* currentMaterialFile;
	if (materialFiles.size() == 0)
	{
		currentMaterialFile = new CMaterialFile();
		currentMaterialFile->name = "DefaultMaterialFile";
		materialFiles.push_back(currentMaterialFile);
	}
	else
	{
		currentMaterialFile = materialFiles[0];
	}

	for (int x = 0 ; x < scene->mNumMaterials; x++) 
	{
        const aiMaterial* pMaterial = scene->mMaterials[x];

		CMaterial* newMaterial = new CMaterial();

		aiString name;
		if (pMaterial->Get(AI_MATKEY_NAME, name) == AI_SUCCESS)
		{
			newMaterial->name = name.data;
		}

		aiColor3D color(0.f,0.f,0.f);

		if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			newMaterial->diffuseColor.contains = true;
			newMaterial->diffuseColor.r = color.r;
			newMaterial->diffuseColor.g = color.g;
			newMaterial->diffuseColor.b = color.b;
		}

		if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		{
			newMaterial->ambientColor.contains = true;
			newMaterial->ambientColor.r = color.r;
			newMaterial->ambientColor.g = color.g;
			newMaterial->ambientColor.b = color.b;
		}

		if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		{
			newMaterial->spectralColor.contains = true;
			newMaterial->spectralColor.r = color.r;
			newMaterial->spectralColor.g = color.g;
			newMaterial->spectralColor.b = color.b;
		}

		float alpha;
		if (pMaterial->Get(AI_MATKEY_OPACITY, alpha) == AI_SUCCESS)
		{
			newMaterial->alpha.contains = true;
			newMaterial->alpha.floatValue = alpha;
		}
		

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		{
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				newMaterial->diffuseTexture.contains = true;
				newMaterial->diffuseTexture.stringValue = Path.data;
			}
		}

		currentMaterialFile->materials.push_back(newMaterial);
	}

	for (int x = 0; x < scene->mRootNode->mNumChildren; x++)
	{
		aiVector3t<float> position(0, 0, 0);
		ParseAssimpNodesRecursive(extension, scene, scene->mRootNode->mChildren[x], joints, position, NULL, currentMaterialFile, currentGroup, verticeColors, normals, uvCoordinates, vertices, groups, foundNormals, foundTextureUV, foundVerticeColors, noGroups);
	}

	for (int x = 0; x < scene->mNumAnimations; x++)
	{
		CAnimation* animation;
		if (joints.size() == 0)
		{
			// assume camera
			animation = new CCamera();
		}
		else
		{
			animation = new CAnimation();
		}

		aiAnimation* animationScene = scene->mAnimations[x];
		
		int maxKeyframes = 0;
		for (int y = 0; y < animationScene->mNumChannels; y++)
		{
			aiNodeAnim* nodeAnim = animationScene->mChannels[y];

			if (nodeAnim->mNumPositionKeys > maxKeyframes)
				maxKeyframes = nodeAnim->mNumPositionKeys;

			if (nodeAnim->mNumRotationKeys > maxKeyframes)
				maxKeyframes = nodeAnim->mNumRotationKeys;

			if (nodeAnim->mNumScalingKeys > maxKeyframes)
				maxKeyframes = nodeAnim->mNumScalingKeys;
		}

		float fps = 30;

		if (extension == "dae")
		{
			CString techniqueMaxFrameRate = "<technique profile=\"MAX3D\"><frame_rate>";

			if (inputFileData.Find(techniqueMaxFrameRate) != -1)
			{
				fps = atof(inputFileData.Mid(inputFileData.Find(techniqueMaxFrameRate) + techniqueMaxFrameRate.GetLength()));
			}

			if ((maxKeyframes > 0) && (animationScene->mTicksPerSecond > 0))
				fps = round(1 / ((animationScene->mDuration / animationScene->mTicksPerSecond) / (float)maxKeyframes));
		}
		else if (extension == "bvh")
		{
			fps = 1;

			maxKeyframes = animationScene->mDuration + 1;
		}

		for (int y = 0; y < animationScene->mNumChannels; y++)
		{
			aiNodeAnim* nodeAnim = animationScene->mChannels[y];

			CString affectedNode = nodeAnim->mNodeName.data;

			if (affectedNode.Find(":") != -1)
			{
				affectedNode = affectedNode.Mid(affectedNode.Find(":") + 1);
			}

			if (affectedNode.Find("_") == 0)
				affectedNode = affectedNode.Mid(1);

			if ((affectedNode == "TopJoint") || (joints.size() == 0)) // assume camera	 or top
			{
				CKeyframe defaultKeyframe;
				defaultKeyframe.translation.x = 0;
				defaultKeyframe.translation.y = 0;
				defaultKeyframe.translation.z = 0;
				defaultKeyframe.rotation.x = 0;
				defaultKeyframe.rotation.y = 0;
				defaultKeyframe.rotation.z = 0;
				defaultKeyframe.scale.x = 0;
				defaultKeyframe.scale.y = 0;
				defaultKeyframe.scale.z = 0;

				CKeyframe* previousKeyframe = &defaultKeyframe;

				int previousKeyframeNumber = -1;
				for(int lCount = 0; lCount < nodeAnim->mNumPositionKeys; lCount++)
				{
					int keyframeNumber = round(nodeAnim->mPositionKeys[lCount].mTime * fps);
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->translation.x = nodeAnim->mPositionKeys[lCount].mValue.x;
						keyframe->translation.y = nodeAnim->mPositionKeys[lCount].mValue.y;
						keyframe->translation.z = nodeAnim->mPositionKeys[lCount].mValue.z;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->translation.x = nodeAnim->mPositionKeys[lCount].mValue.x;
								sliceKeyframe->translation.y = nodeAnim->mPositionKeys[lCount].mValue.y;
								sliceKeyframe->translation.z = nodeAnim->mPositionKeys[lCount].mValue.z;
							}
							keyframe->translation.x = nodeAnim->mPositionKeys[lCount].mValue.x;
							keyframe->translation.y = nodeAnim->mPositionKeys[lCount].mValue.y;
							keyframe->translation.z = nodeAnim->mPositionKeys[lCount].mValue.z;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float3 deltaSlice(
								(nodeAnim->mPositionKeys[lCount].mValue.x - previousKeyframe->translation.x) / (slices + 1),
								(nodeAnim->mPositionKeys[lCount].mValue.y - previousKeyframe->translation.y) / (slices + 1),
								(nodeAnim->mPositionKeys[lCount].mValue.z - previousKeyframe->translation.z) / (slices + 1)
								);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->translation.x = previousKeyframe->translation.x + (deltaSlice.x * (y + 1));
								sliceKeyframe->translation.y = previousKeyframe->translation.y + (deltaSlice.y * (y + 1));
								sliceKeyframe->translation.z = previousKeyframe->translation.z + (deltaSlice.z * (y + 1));
								sliceKeyframe->translationIsInterpolated[0] = true;
								sliceKeyframe->translationIsInterpolated[1] = true;
								sliceKeyframe->translationIsInterpolated[2] = true;
							}

							keyframe->translation.x = nodeAnim->mPositionKeys[lCount].mValue.x;
							keyframe->translation.y = nodeAnim->mPositionKeys[lCount].mValue.y;
							keyframe->translation.z = nodeAnim->mPositionKeys[lCount].mValue.z;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->translation.x = previousKeyframe->translation.x;
					sliceKeyframe->translation.y = previousKeyframe->translation.y;
					sliceKeyframe->translation.z = previousKeyframe->translation.z;
				}


				previousKeyframe = &defaultKeyframe;
				previousKeyframeNumber = -1;
				for(int lCount = 0; lCount < nodeAnim->mNumRotationKeys; lCount++)
				{
					int keyframeNumber = round(nodeAnim->mRotationKeys[lCount].mTime * fps);
					Quat quat(nodeAnim->mRotationKeys[lCount].mValue.x, nodeAnim->mRotationKeys[lCount].mValue.y, nodeAnim->mRotationKeys[lCount].mValue.z, nodeAnim->mRotationKeys[lCount].mValue.w); 

					float3 keyValueOrig = quat.ToEulerZYX();
					float3 keyValue;

					keyValue.z = RadToDeg(keyValueOrig.x);
					keyValue.y = RadToDeg(keyValueOrig.y);
					keyValue.x = RadToDeg(keyValueOrig.z);

					if ((fabs(keyValue.x) < EPSILONVALUES) || (fabs(360 - keyValue.x) < EPSILONVALUES))
						keyValue.x = 0;
					if ((fabs(keyValue.y) < EPSILONVALUES) || (fabs(360 - keyValue.y) < EPSILONVALUES))
						keyValue.y = 0;
					if ((fabs(keyValue.z) < EPSILONVALUES) || (fabs(360 - keyValue.z) < EPSILONVALUES))
						keyValue.z = 0;

					while (keyValue.x >= 360.0f)
						keyValue.x -= 360.0f;
					while (keyValue.x < 0.0f)
						keyValue.x += 360.0f;

					while (keyValue.y >= 360.0f)
						keyValue.y -= 360.0f;
					while (keyValue.y < 0.0f)
						keyValue.y += 360.0f;

					while (keyValue.z >= 360.0f)
						keyValue.z -= 360.0f;
					while (keyValue.z < 0.0f)
						keyValue.z += 360.0f;

					if ((fabs(keyValue.x) < EPSILONVALUES) || (fabs(360 - keyValue.x) < EPSILONVALUES))
						keyValue.x = 0;
					if ((fabs(keyValue.y) < EPSILONVALUES) || (fabs(360 - keyValue.y) < EPSILONVALUES))
						keyValue.y = 0;
					if ((fabs(keyValue.z) < EPSILONVALUES) || (fabs(360 - keyValue.z) < EPSILONVALUES))
						keyValue.z = 0;
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->rotation.x = keyValue.x;
						keyframe->rotation.y = keyValue.y;
						keyframe->rotation.z = keyValue.z;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->rotation.x = keyValue.x;
								sliceKeyframe->rotation.y = keyValue.y;
								sliceKeyframe->rotation.z = keyValue.z;
							}
							keyframe->rotation.x = keyValue.x;
							keyframe->rotation.y = keyValue.y;
							keyframe->rotation.z = keyValue.z;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);

							float3 span (
								(keyValue.x - previousKeyframe->rotation.x),
								(keyValue.y - previousKeyframe->rotation.y),
								(keyValue.z - previousKeyframe->rotation.z)
								);

							while (span.x >= 360.0f)
								span.x -= 360.0f;
							while (span.x < 0.0f)
								span.x += 360.0f;

							while (span.y >= 360.0f)
								span.y -= 360.0f;
							while (span.y < 0.0f)
								span.y += 360.0f;

							while (span.z >= 360.0f)
								span.z -= 360.0f;
							while (span.z < 0.0f)
								span.z += 360.0f;

							if (fabs(span.x) > 180.0f)
								span.x -= 360.0f;
							if (fabs(span.y) > 180.0f)
								span.y -= 360.0f;
							if (fabs(span.z) > 180.0f)
								span.z -= 360.0f;

							float3 deltaSlice(
								span.x / (slices + 1),
								span.y / (slices + 1),
								span.z / (slices + 1)
								);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->rotation.x = previousKeyframe->rotation.x + (deltaSlice.x * (y + 1));
								sliceKeyframe->rotation.y = previousKeyframe->rotation.y + (deltaSlice.y * (y + 1));
								sliceKeyframe->rotation.z = previousKeyframe->rotation.z + (deltaSlice.z * (y + 1));
								sliceKeyframe->rotationIsInterpolated[0] = true;
								sliceKeyframe->rotationIsInterpolated[1] = true;
								sliceKeyframe->rotationIsInterpolated[2] = true;
							}

							if (Sign(span.x) != Sign(keyValue.x))
								keyframe->rotation.x = keyValue.x - 360.0f;
							else
								keyframe->rotation.x = keyValue.x;

							if (Sign(span.y) != Sign(keyValue.y))
								keyframe->rotation.y = keyValue.y - 360.0f;
							else
								keyframe->rotation.y = keyValue.y;

							if (Sign(span.z) != Sign(keyValue.z))
								keyframe->rotation.z = keyValue.z - 360.0f;
							else
								keyframe->rotation.z = keyValue.z;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->rotation.x = previousKeyframe->rotation.x;
					sliceKeyframe->rotation.y = previousKeyframe->rotation.y;
					sliceKeyframe->rotation.z = previousKeyframe->rotation.z;
				}

				previousKeyframe = &defaultKeyframe;
				previousKeyframeNumber = -1;
				for(int lCount = 0; lCount < nodeAnim->mNumScalingKeys; lCount++)
				{
					int keyframeNumber = round(nodeAnim->mScalingKeys[lCount].mTime * fps);

					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
						keyframe->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
						keyframe->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
								sliceKeyframe->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
								sliceKeyframe->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
							}
							keyframe->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
							keyframe->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
							keyframe->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.x - previousKeyframe->scale.x) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.x = previousKeyframe->scale.x + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[0] = true;
							}

							deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.y - previousKeyframe->scale.y) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.y = previousKeyframe->scale.y + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[1] = true;
							}

							deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.z - previousKeyframe->scale.z) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.z = previousKeyframe->scale.z + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[2] = true;
							}

							keyframe->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
							keyframe->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
							keyframe->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->scale.x = previousKeyframe->scale.x;
					sliceKeyframe->scale.y = previousKeyframe->scale.y;
					sliceKeyframe->scale.z = previousKeyframe->scale.z;
				}
			}
			else// if (StringHasNumber(affectedNode))
			{
				CJoint* joint = NULL;
				for (int z = 0; z < joints.size(); z++)
				{
					if (joints[z]->name == affectedNode)
					{
						joint = joints[z];
						break;
					}
				}

				if (joint != NULL)
				{
					CAnimationPart defaultPart;
					defaultPart.translation.x = 0;
					defaultPart.translation.y = 0;
					defaultPart.translation.z = 0;
					defaultPart.rotation.x = 0;
					defaultPart.rotation.y = 0;
					defaultPart.rotation.z = 0;
					defaultPart.scale.x = 0;
					defaultPart.scale.y = 0;
					defaultPart.scale.z = 0;

					CAnimationPart* previousAnimationPart = &defaultPart;
					int previousKeyframeNumber = -1;

					for(int lCount = 0; lCount < nodeAnim->mNumRotationKeys; lCount++)
					{
						int keyframeNumber = round(nodeAnim->mRotationKeys[lCount].mTime * fps);
						
						Quat quat(nodeAnim->mRotationKeys[lCount].mValue.x, nodeAnim->mRotationKeys[lCount].mValue.y, nodeAnim->mRotationKeys[lCount].mValue.z, nodeAnim->mRotationKeys[lCount].mValue.w);

						/*aiMatrix4x4 res;
						aiMatrix4x4 rot;
						
						aiVector3D axis;

						float angleX = 60 * float( AI_MATH_PI) / 180.0f;
						axis = aiVector3D( 0.0f, 0.0f, 1.0f);
						aiMatrix4x4::Rotation( angleX, axis, rot);
						res *= rot;

						float angleY = 45 * float( AI_MATH_PI) / 180.0f;
						axis = aiVector3D( 0.0f, 1.0f, 0.0f);
						aiMatrix4x4::Rotation( angleY, axis, rot);
						res *= rot;

						float angleZ = 30 * float( AI_MATH_PI) / 180.0f;
						axis = aiVector3D( 1.0f, 0.0f, 0.0f);
						aiMatrix4x4::Rotation( angleZ, axis, rot);
						res *= rot;

						aiVector3D scale;
						aiQuaternion rotation;
						aiVector3D translation;
						res.Decompose(scale, rotation, translation);

						float4x4 tryA(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
						tryA = tryA;

						float3 a = tryA.ToEulerXYZ();
						a.x = RadToDeg(keyValue.x);
						a.y = RadToDeg(keyValue.y);
						a.z = RadToDeg(keyValue.z);
						
						float3 b = tryA.ToEulerZYX();
						b.x = RadToDeg(keyValue.x);
						b.y = RadToDeg(keyValue.y);
						b.z = RadToDeg(keyValue.z);*/

						float3 keyValueOrig = quat.ToEulerZYX();
						float3 keyValue;
						keyValue.z = RadToDeg(keyValueOrig.x);
						keyValue.y = RadToDeg(keyValueOrig.y);
						keyValue.x = RadToDeg(keyValueOrig.z);

						if ((fabs(keyValue.x) < EPSILONVALUES) || (fabs(360 - keyValue.x) < EPSILONVALUES))
							keyValue.x = 0;
						if ((fabs(keyValue.y) < EPSILONVALUES) || (fabs(360 - keyValue.y) < EPSILONVALUES))
							keyValue.y = 0;
						if ((fabs(keyValue.z) < EPSILONVALUES) || (fabs(360 - keyValue.z) < EPSILONVALUES))
							keyValue.z = 0;

						while (keyValue.x >= 360.0f)
							keyValue.x -= 360.0f;
						while (keyValue.x < 0.0f)
							keyValue.x += 360.0f;

						while (keyValue.y >= 360.0f)
							keyValue.y -= 360.0f;
						while (keyValue.y < 0.0f)
							keyValue.y += 360.0f;

						while (keyValue.z >= 360.0f)
							keyValue.z -= 360.0f;
						while (keyValue.z < 0.0f)
							keyValue.z += 360.0f;

						if ((fabs(keyValue.x) < EPSILONVALUES) || (fabs(360 - keyValue.x) < EPSILONVALUES))
							keyValue.x = 0;
						if ((fabs(keyValue.y) < EPSILONVALUES) || (fabs(360 - keyValue.y) < EPSILONVALUES))
							keyValue.y = 0;
						if ((fabs(keyValue.z) < EPSILONVALUES) || (fabs(360 - keyValue.z) < EPSILONVALUES))
							keyValue.z = 0;
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
						CAnimationPart* animationPart = GetAddPart(joint->name, keyframe);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							animationPart->rotation.x = keyValue.x;
							animationPart->rotation.y = keyValue.y;
							animationPart->rotation.z = keyValue.z;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->rotation.x = keyValue.x;
									sliceAnimationPart->rotation.y = keyValue.y;
									sliceAnimationPart->rotation.z = keyValue.z;
								}
								animationPart->rotation.x = keyValue.x;
								animationPart->rotation.y = keyValue.y;
								animationPart->rotation.z = keyValue.z;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);

								float3 span (
									(keyValue.x - previousAnimationPart->rotation.x),
									(keyValue.y - previousAnimationPart->rotation.y),
									(keyValue.z - previousAnimationPart->rotation.z)
									);

								while (span.x >= 360.0f)
									span.x -= 360.0f;
								while (span.x < 0.0f)
									span.x += 360.0f;

								while (span.y >= 360.0f)
									span.y -= 360.0f;
								while (span.y < 0.0f)
									span.y += 360.0f;

								while (span.z >= 360.0f)
									span.z -= 360.0f;
								while (span.z < 0.0f)
									span.z += 360.0f;

								if (fabs(span.x) > 180.0f)
									span.x -= 360.0f;
								if (fabs(span.y) > 180.0f)
									span.y -= 360.0f;
								if (fabs(span.z) > 180.0f)
									span.z -= 360.0f;

								float3 deltaSlice(
									span.x / (slices + 1),
									span.y / (slices + 1),
									span.z / (slices + 1)
									);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x + (deltaSlice.x * (y + 1));
									sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y + (deltaSlice.y * (y + 1));
									sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z + (deltaSlice.z * (y + 1));
									sliceAnimationPart->rotationIsInterpolated[0] = true;
									sliceAnimationPart->rotationIsInterpolated[1] = true;
									sliceAnimationPart->rotationIsInterpolated[2] = true;
								}

								if (Sign(span.x) != Sign(keyValue.x))
									animationPart->rotation.x = keyValue.x - 360.0f;
								else
									animationPart->rotation.x = keyValue.x;

								if (Sign(span.y) != Sign(keyValue.y))
									animationPart->rotation.y = keyValue.y - 360.0f;
								else
									animationPart->rotation.y = keyValue.y;

								if (Sign(span.z) != Sign(keyValue.z))
									animationPart->rotation.z = keyValue.z - 360.0f;
								else
									animationPart->rotation.z = keyValue.z;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousAnimationPart = animationPart;
					}

					for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
						CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

						sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x;
						sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y;
						sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z;
					}

					defaultPart.translation.x = 0;
					defaultPart.translation.y = 0;
					defaultPart.translation.z = 0;
					defaultPart.rotation.x = 0;
					defaultPart.rotation.y = 0;
					defaultPart.rotation.z = 0;
					defaultPart.scale.x = 0;
					defaultPart.scale.y = 0;
					defaultPart.scale.z = 0;

					previousAnimationPart = &defaultPart;
					previousKeyframeNumber = -1;

					for(int lCount = 0; lCount < nodeAnim->mNumPositionKeys; lCount++)
					{
						int keyframeNumber = round(nodeAnim->mPositionKeys[lCount].mTime * fps);

						float3 keyframeAdjustedPosition(0,0,0);

						float deltaPartPos = 0;
						if (extension == "dae")
						{
							deltaPartPos = joint->positionAbsolute.x;
							if (joint->parent != NULL)
								deltaPartPos = (deltaPartPos - joint->parent->positionAbsolute.x);
						}

						keyframeAdjustedPosition.x = nodeAnim->mPositionKeys[lCount].mValue.x - deltaPartPos;

						deltaPartPos = 0;
						if (extension == "dae")
						{
							deltaPartPos = joint->positionAbsolute.y;
							if (joint->parent != NULL)
								deltaPartPos = (deltaPartPos - joint->parent->positionAbsolute.y);
						}

						keyframeAdjustedPosition.y = nodeAnim->mPositionKeys[lCount].mValue.y - deltaPartPos;

						deltaPartPos = 0;
						if (extension == "dae")
						{
							deltaPartPos = joint->positionAbsolute.z;
							if (joint->parent != NULL)
								deltaPartPos = (deltaPartPos - joint->parent->positionAbsolute.z);
						}

						keyframeAdjustedPosition.z = nodeAnim->mPositionKeys[lCount].mValue.z - deltaPartPos;
						

						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
						CAnimationPart* animationPart = GetAddPart(joint->name, keyframe);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							animationPart->translation.x = keyframeAdjustedPosition.x;
							animationPart->translation.y = keyframeAdjustedPosition.y;
							animationPart->translation.z = keyframeAdjustedPosition.z;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->translation.x = keyframeAdjustedPosition.x;
									sliceAnimationPart->translation.y = keyframeAdjustedPosition.y;
									sliceAnimationPart->translation.z = keyframeAdjustedPosition.z;
								}
								animationPart->translation.x = keyframeAdjustedPosition.x;
								animationPart->translation.y = keyframeAdjustedPosition.y;
								animationPart->translation.z = keyframeAdjustedPosition.z;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								
								float deltaSlice = (keyframeAdjustedPosition.x - previousAnimationPart->translation.x) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->translation.x = previousAnimationPart->translation.x + (deltaSlice * (y + 1));
									sliceAnimationPart->translationIsInterpolated[0] = true;
								}

								deltaSlice = (keyframeAdjustedPosition.y - previousAnimationPart->translation.y) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->translation.y = previousAnimationPart->translation.y + (deltaSlice * (y + 1));
									sliceAnimationPart->translationIsInterpolated[1] = true;
								}

								deltaSlice = (keyframeAdjustedPosition.z - previousAnimationPart->translation.z) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->translation.z = previousAnimationPart->translation.z + (deltaSlice * (y + 1));
									sliceAnimationPart->translationIsInterpolated[2] = true;
								}

								animationPart->translation.x = keyframeAdjustedPosition.x;
								animationPart->translation.y = keyframeAdjustedPosition.y;
								animationPart->translation.z = keyframeAdjustedPosition.z;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousAnimationPart = animationPart;
					}

					for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
						CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

						sliceAnimationPart->translation.x = previousAnimationPart->translation.x;
						sliceAnimationPart->translation.y = previousAnimationPart->translation.y;
						sliceAnimationPart->translation.z = previousAnimationPart->translation.z;
					}

					defaultPart.translation.x = 0;
					defaultPart.translation.y = 0;
					defaultPart.translation.z = 0;
					defaultPart.rotation.x = 0;
					defaultPart.rotation.y = 0;
					defaultPart.rotation.z = 0;
					defaultPart.scale.x = 0;
					defaultPart.scale.y = 0;
					defaultPart.scale.z = 0;

					previousAnimationPart = &defaultPart;
					previousKeyframeNumber = -1;

					for(int lCount = 0; lCount < nodeAnim->mNumScalingKeys; lCount++)
					{
						int keyframeNumber = round(nodeAnim->mScalingKeys[lCount].mTime * fps);

						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
						CAnimationPart* animationPart = GetAddPart(joint->name, keyframe);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							animationPart->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
							animationPart->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
							animationPart->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
									sliceAnimationPart->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
									sliceAnimationPart->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
								}
								animationPart->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;
								animationPart->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;
								animationPart->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.x - previousAnimationPart->scale.x) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->scale.x = previousAnimationPart->scale.x + (deltaSlice * (y + 1));
									sliceAnimationPart->scaleIsInterpolated[0] = true;
								}

								animationPart->scale.x = nodeAnim->mScalingKeys[lCount].mValue.x;

								deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.y - previousAnimationPart->scale.y) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->scale.y = previousAnimationPart->scale.y + (deltaSlice * (y + 1));
									sliceAnimationPart->scaleIsInterpolated[1] = true;
								}

								animationPart->scale.y = nodeAnim->mScalingKeys[lCount].mValue.y;

								deltaSlice = (nodeAnim->mScalingKeys[lCount].mValue.z - previousAnimationPart->scale.z) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
									CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

									sliceAnimationPart->scale.z = previousAnimationPart->scale.z + (deltaSlice * (y + 1));
									sliceAnimationPart->scaleIsInterpolated[2] = true;
								}

								animationPart->scale.z = nodeAnim->mScalingKeys[lCount].mValue.z;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousAnimationPart = animationPart;
					}

					for (int y = (previousKeyframeNumber + 1); y < maxKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
						CAnimationPart* sliceAnimationPart = GetAddPart(joint->name, sliceKeyframe);

						sliceAnimationPart->scale.x = previousAnimationPart->scale.x;
						sliceAnimationPart->scale.y = previousAnimationPart->scale.y;
						sliceAnimationPart->scale.z = previousAnimationPart->scale.z;
					}
				}
			}
		}

		animations.push_back(animation);
	}

	return true;
}

bool CObjToAn8Dlg::ReadAn8File(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, 
		bool noGroups)
{
	An8File an8File;

	if( an8File.LoadFile(string((LPCTSTR)(inputFile))) == false)
	{
		return false;
	}

	CGroup* defaultGroup = new CGroup();
	defaultGroup->name = "DefaultUnassigned";

	groups.push_back(defaultGroup);

	CGroup* currentGroup = defaultGroup;

	CString namedMainObject = "";

	std::map<CString, int> meshVertexOffsets;

	//for (int x = 0; x < an8File.NbrObject; x++)
	// later let you pick
	if (an8File.NbrObject > 0)
	{
		int x = 0;

		namedMainObject = an8File.vObjects[x].Name.c_str();
		{
			if(an8File.vObjects[x].NbrMesh > 0)
			{
				CMaterialFile* currentMaterialFile = new CMaterialFile();
				currentMaterialFile->name.Format("%s", an8File.vObjects[x].Name.c_str());
				materialFiles.push_back(currentMaterialFile);

				for (int y = 0; y < an8File.vObjects[x].NbrMaterial; y++)
				{
					CMaterial* newMaterial = new CMaterial();
					newMaterial->name.Format("%s", an8File.vObjects[x].vMaterials[y].Name.c_str());

					newMaterial->alpha.floatValue = (float)an8File.vObjects[x].vMaterials[y].Alpha / 255.05f;
					newMaterial->alpha.contains = true;

					if (!an8File.vObjects[x].vMaterials[y].DiffuseTexName.empty())
					{
						CString diffuseTextureName;
						diffuseTextureName.Format("%s", an8File.vObjects[x].vMaterials[y].DiffuseTexName.c_str());

						for (int z = 0; z < an8File.NbrTexture; z++)
						{
							CString textureName;
							textureName.Format("%s", an8File.vTextures[z].Name.c_str());
							if (textureName == diffuseTextureName)
							{
								newMaterial->diffuseTexture.stringValue.Format("%s", an8File.vTextures[z].FileName.c_str());
								newMaterial->diffuseTexture.contains = true;
								break;
							}
						}
					}

					newMaterial->diffuseColor.contains = true;
					newMaterial->diffuseColor.r = (float)an8File.vObjects[x].vMaterials[y].DiffuseRGB[0] / 255.0f;
					newMaterial->diffuseColor.g = (float)an8File.vObjects[x].vMaterials[y].DiffuseRGB[1] / 255.0f;
					newMaterial->diffuseColor.b = (float)an8File.vObjects[x].vMaterials[y].DiffuseRGB[2] / 255.0f;

					newMaterial->ambientColor.contains = true;
					newMaterial->ambientColor.r = (float)an8File.vObjects[x].vMaterials[y].AmbiantRGB[0] / 255.0f;
					newMaterial->ambientColor.g = (float)an8File.vObjects[x].vMaterials[y].AmbiantRGB[1] / 255.0f;
					newMaterial->ambientColor.b = (float)an8File.vObjects[x].vMaterials[y].AmbiantRGB[2] / 255.0f;

					newMaterial->spectralColor.contains = true;
					newMaterial->spectralColor.r = (float)an8File.vObjects[x].vMaterials[y].SpecularRGB[0] / 255.0f;
					newMaterial->spectralColor.g = (float)an8File.vObjects[x].vMaterials[y].SpecularRGB[1] / 255.0f;
					newMaterial->spectralColor.b = (float)an8File.vObjects[x].vMaterials[y].SpecularRGB[2] / 255.0f;

					currentMaterialFile->materials.push_back(newMaterial);
				}

				for (int y = 0; y < an8File.vObjects[x].NbrMesh; y++)
				{
					if (!noGroups)
					{
						CGroup* newGroup = new CGroup();
						newGroup->name.Format("%s", an8File.vObjects[x].vMeshes[y].Name.c_str());
						groups.push_back(newGroup);
						currentGroup = newGroup;
					}

					int meshVerticeOffset = vertices.size();
					int meshUvOffset = uvCoordinates.size();
					int meshNormalsOffset = normals.size();

					meshVertexOffsets[an8File.vObjects[x].vMeshes[y].Name.c_str()] = meshVerticeOffset;

					for (int z = 0; z < an8File.vObjects[x].vMeshes[y].NbrPoint; z++)
					{
						CVertice* newVertice = new CVertice();
				
						newVertice->vertex.x = an8File.vObjects[x].vMeshes[y].vPoints[z].x;
						newVertice->vertex.y = an8File.vObjects[x].vMeshes[y].vPoints[z].y;
						newVertice->vertex.z = an8File.vObjects[x].vMeshes[y].vPoints[z].z;

						vertices.push_back(newVertice);
					}

					if (an8File.vObjects[x].vMeshes[y].NbrNormal > 0)
					{
						foundNormals = true;
					}

					for (int z = 0; z < an8File.vObjects[x].vMeshes[y].NbrNormal; z++)
					{
						CNormal* newNormal = new CNormal();

						newNormal->nx = an8File.vObjects[x].vMeshes[y].vNormals[z].x;
						newNormal->ny = an8File.vObjects[x].vMeshes[y].vNormals[z].y;
						newNormal->nz = an8File.vObjects[x].vMeshes[y].vNormals[z].z;

						normals.push_back(newNormal);	
					}

					if (an8File.vObjects[x].vMeshes[y].NbrUVCoord > 0)
					{
						foundTextureUV = true;
					}

					for (int z = 0; z < an8File.vObjects[x].vMeshes[y].NbrUVCoord; z++)
					{
						CUVCoordinate* newUv = new CUVCoordinate();

						newUv->u = an8File.vObjects[x].vMeshes[y].vTexCoords[z].x;
						newUv->v = an8File.vObjects[x].vMeshes[y].vTexCoords[z].y;
						
						uvCoordinates.push_back(newUv);
					}

					for (int z = 0; z < an8File.vObjects[x].vMeshes[y].NbrFace; z++)
					{
						CPolygon* newPolygon = new CPolygon();

						int materialNumber = an8File.vObjects[x].vMeshes[y].vFaces[z].Material;

						materialNumber = an8File.vObjects[x].vMeshes[y].vMatNumber[materialNumber];

						newPolygon->materialName = an8File.vObjects[x].vMaterials[materialNumber].Name.c_str();


						for (int w = 0; w < an8File.vObjects[x].vMeshes[y].vFaces[z].NbrVertex; w++)
						{
							CPolygonPoint* polygonPoint = new CPolygonPoint();

							polygonPoint->verticeIndex = meshVerticeOffset + an8File.vObjects[x].vMeshes[y].vFaces[z].vIndices[w].Point;

							if (foundNormals)
							{
								if (an8File.vObjects[x].vMeshes[y].vFaces[z].vIndices[w].Normal >= 0)
								{
									polygonPoint->normalIndex = meshNormalsOffset + an8File.vObjects[x].vMeshes[y].vFaces[z].vIndices[w].Normal;
								}
							}

							if (foundTextureUV)
							{
								if (an8File.vObjects[x].vMeshes[y].vFaces[z].vIndices[w].TexCoord >= 0)
								{
									polygonPoint->uvIndex = meshUvOffset + an8File.vObjects[x].vMeshes[y].vFaces[z].vIndices[w].TexCoord;
								}
							}

							polygonPoint->verticeColorIndex = -1;


							newPolygon->polygonPoints.push_back(polygonPoint);
						}

						currentGroup->polygons.push_back(newPolygon);
					}
				}
			}
		}
	}

	//for (int x = 0; x < an8File.NbrFigure; x++)
	// later let you pick
	if (an8File.NbrFigure > 0)
	{
		int x = 0;
		{
			AN8XMATRIX parentMatrix;
			ParseAn8BoneRecursive(an8File.vFigures[x].Root, joints, float3(0,0,0), NULL, &parentMatrix);



			if (an8File.vFigures[x].Root.NbrNamedObject > 0)
			{
				if (namedMainObject == an8File.vFigures[x].Root.vNamedObjects[0].ObjectName.c_str())
				{
					An8NamedObject* vNamedObject = &(an8File.vFigures[x].Root.vNamedObjects[0]);
					for (int z = 0; z < vNamedObject->NbrWeights; z++)
					{
						CString meshWeightName = vNamedObject->vWeights[z].MeshName.c_str();
					
						int vertexOffset = 0;

						std::map<CString, int>::iterator itermeshVertexOffsets;
						for (itermeshVertexOffsets = meshVertexOffsets.begin(); itermeshVertexOffsets != meshVertexOffsets.end(); itermeshVertexOffsets++ )
						{
							CString meshName = (CString)((*itermeshVertexOffsets).first);
							int meshVertexOffset = (int)((*itermeshVertexOffsets).second);
						
							if (meshName == meshWeightName)
							{
								vertexOffset = meshVertexOffset;
								break;
							}
						}

						for (int w = 0; w < vNamedObject->vWeights[z].NbrOfVertexWeight; w++)
						{
							if (vNamedObject->vWeights[z].vVertexWeights[w].NbrOfWeightValue > 0)
							{
								int jointIndex = vNamedObject->vWeights[z].vVertexWeights[w].vWeightValues[0].BoneIndex;
								CString weightedBy = vNamedObject->vWeightedBy[jointIndex].c_str();

								for (int ww = 0; ww < joints.size(); ww++)
								{
									if (joints[ww]->name == weightedBy)
									{
										joints[ww]->controlPoints.push_back(vertexOffset + w);
										break;
									}
								}
							}
						}
					}
				}
			}

			// now get control points
			for (int y = 0; y < an8File.vFigures[x].Root.NbrBone; y++)
			{
				if (an8File.vFigures[x].Root.vBones[y].NbrNamedObject > 0)
				{
					if (namedMainObject == an8File.vFigures[x].Root.vBones[y].vNamedObjects[0].ObjectName.c_str())
					{
						An8NamedObject* vNamedObject = &(an8File.vFigures[x].Root.vBones[y].vNamedObjects[0]);
						for (int z = 0; z < vNamedObject->NbrWeights; z++)
						{
							CString meshWeightName = vNamedObject->vWeights[z].MeshName.c_str();
						
							int vertexOffset = 0;

							std::map<CString, int>::iterator itermeshVertexOffsets;
							for (itermeshVertexOffsets = meshVertexOffsets.begin(); itermeshVertexOffsets != meshVertexOffsets.end(); itermeshVertexOffsets++ )
							{
								CString meshName = (CString)((*itermeshVertexOffsets).first);
								int meshVertexOffset = (int)((*itermeshVertexOffsets).second);
							
								if (meshName == meshWeightName)
								{
									vertexOffset = meshVertexOffset;
									break;
								}
							}

							for (int w = 0; w < vNamedObject->vWeights[z].NbrOfVertexWeight; w++)
							{
								if (vNamedObject->vWeights[z].vVertexWeights[w].NbrOfWeightValue > 0)
								{
									int jointIndex = vNamedObject->vWeights[z].vVertexWeights[w].vWeightValues[0].BoneIndex;
									CString weightedBy = vNamedObject->vWeightedBy[jointIndex].c_str();

									for (int ww = 0; ww < joints.size(); ww++)
									{
										if (joints[ww]->name == weightedBy)
										{
											joints[ww]->controlPoints.push_back(vertexOffset + w);
											break;
										}
									}
								}
							}
						}
					}
					break;
				}
			}

		}
	}

	int numberKeyframes = 0;

	//for (int x = 0; x < an8File.NbrSequence; x++)
	// later let you pick
	if (an8File.NbrSequence > 0)
	{
		int x = 0;
		{
			if (an8File.vSequences[x].NbrFrame > 0)
			{
				CAnimation* animation = new CAnimation();
				animation->isCamera = false;
				animation->name = an8File.vSequences[x].Name.c_str();

				for (int y = 0; y < an8File.vSequences[x].NbrJointAngle; y++)
				{
					if (an8File.vSequences[x].vJointAngles[y].HasTrack)
					{
						for (int z = 0; z < an8File.vSequences[x].vJointAngles[y].Track.NbrFloatKey; z++)
						{
							int keyframeNumber = an8File.vSequences[x].vJointAngles[y].Track.vFloatKeys[z].FrameNumber;
							if ((keyframeNumber + 1) > numberKeyframes)
								numberKeyframes = (keyframeNumber + 1);
						}
					}
				}

				for (int y = 0; y < an8File.vSequences[x].NbrJointAngle; y++)
				{
					if (an8File.vSequences[x].vJointAngles[y].HasTrack)
					{
						CString boneNameStr;
						boneNameStr.Format("%s", an8File.vSequences[x].vJointAngles[y].BoneName.c_str());
						if (boneNameStr == "TopJoint")
						{
							CKeyframe defaultKeyframe;
							defaultKeyframe.rotation.x = 0;
							defaultKeyframe.rotation.y = 0;
							defaultKeyframe.rotation.z = 0;

							int previousKeyframeNumber = -1;
							CKeyframe* previousKeyframe = &defaultKeyframe;

							for (int z = 0; z < an8File.vSequences[x].vJointAngles[y].Track.NbrFloatKey; z++)
							{
								int keyframeNumber = an8File.vSequences[x].vJointAngles[y].Track.vFloatKeys[z].FrameNumber;

								CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
								
								float lKeyValue = an8File.vSequences[x].vJointAngles[y].Track.vFloatKeys[z].vFloats[0];

								while (lKeyValue >= 360.0f)
									lKeyValue -= 360.0f;
								while (lKeyValue < 0.0f)
									lKeyValue += 360.0f;

								if (an8File.vSequences[x].vJointAngles[y].Axe == 1) // X
								{
									if (keyframeNumber == (previousKeyframeNumber + 1))
									{
										keyframe->rotation.x = lKeyValue;
									}
									else
									{
										if (z == 0)
										{
											for (int rr = 0; rr < keyframeNumber; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
												
												sliceKeyframe->rotation.x = lKeyValue;
											}
											keyframe->rotation.x = lKeyValue;
										}
										else
										{
											int slices = keyframeNumber - (previousKeyframeNumber+1);
											float span = (lKeyValue - previousKeyframe->rotation.x);

											while (span >= 360.0f)
												span -= 360.0f;
											while (span < 0.0f)
												span += 360.0f;

											if (fabs(span) > 180.0f)
												span -= 360.0f;

											float deltaSlice = span / (slices + 1);
											for (int rr = 0; rr < slices; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
												
												sliceKeyframe->rotation.x = previousKeyframe->rotation.x + (deltaSlice * (rr + 1));
												sliceKeyframe->rotationIsInterpolated[0] = true;
											}

											if (Sign(span) != Sign(lKeyValue))
												keyframe->rotation.x = lKeyValue - 360.0f;
											else
												keyframe->rotation.x = lKeyValue;
										}
									}

									previousKeyframeNumber = keyframeNumber;
									previousKeyframe = keyframe;
								}
								else if (an8File.vSequences[x].vJointAngles[y].Axe == 2) // Y
								{
									if (keyframeNumber == (previousKeyframeNumber + 1))
									{
										keyframe->rotation.y = lKeyValue;
									}
									else
									{
										if (z == 0)
										{
											for (int rr = 0; rr < keyframeNumber; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
												
												sliceKeyframe->rotation.y = lKeyValue;
											}
											keyframe->rotation.y = lKeyValue;
										}
										else
										{
											int slices = keyframeNumber - (previousKeyframeNumber+1);
											float span = (lKeyValue - previousKeyframe->rotation.y);

											while (span >= 360.0f)
												span -= 360.0f;
											while (span < 0.0f)
												span += 360.0f;

											if (fabs(span) > 180.0f)
												span -= 360.0f;

											float deltaSlice = span / (slices + 1);
											for (int rr = 0; rr < slices; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
												
												sliceKeyframe->rotation.y = previousKeyframe->rotation.y + (deltaSlice * (rr + 1));
												sliceKeyframe->rotationIsInterpolated[1] = true;
											}

											if (Sign(span) != Sign(lKeyValue))
												keyframe->rotation.y = lKeyValue - 360.0f;
											else
												keyframe->rotation.y = lKeyValue;
										}
									}

									previousKeyframeNumber = keyframeNumber;
									previousKeyframe = keyframe;
								}
								else if (an8File.vSequences[x].vJointAngles[y].Axe == 3) // Z
								{
									if (keyframeNumber == (previousKeyframeNumber + 1))
									{
										keyframe->rotation.z = lKeyValue;
									}
									else
									{
										if (z == 0)
										{
											for (int rr = 0; rr < keyframeNumber; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
												
												sliceKeyframe->rotation.z = lKeyValue;
											}
											keyframe->rotation.z = lKeyValue;
										}
										else
										{
											int slices = keyframeNumber - (previousKeyframeNumber+1);
											float span = (lKeyValue - previousKeyframe->rotation.z);

											while (span >= 360.0f)
												span -= 360.0f;
											while (span < 0.0f)
												span += 360.0f;

											if (fabs(span) > 180.0f)
												span -= 360.0f;

											float deltaSlice = span / (slices + 1);
											for (int rr = 0; rr < slices; rr++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
												
												sliceKeyframe->rotation.z = previousKeyframe->rotation.z + (deltaSlice * (rr + 1));
												sliceKeyframe->rotationIsInterpolated[2] = true;
											}

											if (Sign(span) != Sign(lKeyValue))
												keyframe->rotation.z = lKeyValue - 360.0f;
											else
												keyframe->rotation.z = lKeyValue;
										}
									}

									previousKeyframeNumber = keyframeNumber;
									previousKeyframe = keyframe;
								}
							}

							for (int rr = (previousKeyframeNumber + 1); rr < numberKeyframes; rr++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
								
								if (an8File.vSequences[x].vJointAngles[y].Axe == 1) // X
								{
									sliceKeyframe->rotation.x = previousKeyframe->rotation.x;
								}
								else if (an8File.vSequences[x].vJointAngles[y].Axe == 2) // Y
								{
									sliceKeyframe->rotation.y = previousKeyframe->rotation.y;
								}
								else if (an8File.vSequences[x].vJointAngles[y].Axe == 3) // Z
								{
									sliceKeyframe->rotation.z = previousKeyframe->rotation.z;
								}
							}
						}
						else
						{
							CAnimationPart defaultPart;
							defaultPart.rotation.x = 0;
							defaultPart.rotation.y = 0;
							defaultPart.rotation.z = 0;

							int previousKeyframeNumber = -1;
							CAnimationPart* previousAnimationPart = &defaultPart;

							for (int z = 0; z < an8File.vSequences[x].vJointAngles[y].Track.NbrFloatKey; z++)
							{
								for (int w = 0; w < joints.size(); w++)
								{
									boneNameStr.Format("%s", an8File.vSequences[x].vJointAngles[y].BoneName.c_str());

									if (boneNameStr == joints[w]->name)
									{
										int keyframeNumber = an8File.vSequences[x].vJointAngles[y].Track.vFloatKeys[z].FrameNumber;

										CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
										CAnimationPart* animationPart = GetAddPart(joints[w]->name, keyframe);

										float lKeyValue = an8File.vSequences[x].vJointAngles[y].Track.vFloatKeys[z].vFloats[0];

										while (lKeyValue >= 360.0f)
											lKeyValue -= 360.0f;
										while (lKeyValue < 0.0f)
											lKeyValue += 360.0f;

										if (an8File.vSequences[x].vJointAngles[y].Axe == 1) // X
										{
											if (keyframeNumber == (previousKeyframeNumber + 1))
											{
												animationPart->rotation.x = lKeyValue;
											}
											else
											{
												if (z == 0)
												{
													for (int rr = 0; rr < keyframeNumber; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.x = lKeyValue;
													}
													animationPart->rotation.x = lKeyValue;
												}
												else
												{
													int slices = keyframeNumber - (previousKeyframeNumber+1);
													float span = (lKeyValue - previousAnimationPart->rotation.x);

													while (span >= 360.0f)
														span -= 360.0f;
													while (span < 0.0f)
														span += 360.0f;

													if (fabs(span) > 180.0f)
														span -= 360.0f;

													float deltaSlice = span / (slices + 1);
													for (int rr = 0; rr < slices; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x + (deltaSlice * (rr + 1));
														sliceAnimationPart->rotationIsInterpolated[0] = true;
													}

													if (Sign(span) != Sign(lKeyValue))
														animationPart->rotation.x = lKeyValue - 360.0f;
													else
														animationPart->rotation.x = lKeyValue;
												}
											}

											previousKeyframeNumber = keyframeNumber;
											previousAnimationPart = animationPart;
										}
										else if (an8File.vSequences[x].vJointAngles[y].Axe == 2) // Y
										{
											if (keyframeNumber == (previousKeyframeNumber + 1))
											{
												animationPart->rotation.y = lKeyValue;
											}
											else
											{
												if (z == 0)
												{
													for (int rr = 0; rr < keyframeNumber; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.y = lKeyValue;
													}
													animationPart->rotation.y = lKeyValue;
												}
												else
												{
													int slices = keyframeNumber - (previousKeyframeNumber+1);
													float span = (lKeyValue - previousAnimationPart->rotation.y);
													
													while (span >= 360.0f)
														span -= 360.0f;
													while (span < 0.0f)
														span += 360.0f;

													if (fabs(span) > 180.0f)
														span -= 360.0f;

													float deltaSlice = span / (slices + 1);
													for (int rr = 0; rr < slices; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y + (deltaSlice * (rr + 1));
														sliceAnimationPart->rotationIsInterpolated[1] = true;
													}

													if (Sign(span) != Sign(lKeyValue))
														animationPart->rotation.y = lKeyValue - 360.0f;
													else
														animationPart->rotation.y = lKeyValue;
												}
											}

											previousKeyframeNumber = keyframeNumber;
											previousAnimationPart = animationPart;
										}
										else if (an8File.vSequences[x].vJointAngles[y].Axe == 3) // Z
										{
											if (keyframeNumber == (previousKeyframeNumber + 1))
											{
												animationPart->rotation.z = lKeyValue;
											}
											else
											{
												if (z == 0)
												{
													for (int rr = 0; rr < keyframeNumber; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.z = lKeyValue;
													}
													animationPart->rotation.z = lKeyValue;
												}
												else
												{
													int slices = keyframeNumber - (previousKeyframeNumber+1);
													float span = (lKeyValue - previousAnimationPart->rotation.z);
													
													while (span >= 360.0f)
														span -= 360.0f;
													while (span < 0.0f)
														span += 360.0f;

													if (fabs(span) > 180.0f)
														span -= 360.0f;

													float deltaSlice = span / (slices + 1);
													for (int rr = 0; rr < slices; rr++)
													{
														CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (rr + 1), animation);
														CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

														sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z + (deltaSlice * (rr + 1));
														sliceAnimationPart->rotationIsInterpolated[2] = true;
													}

													if (Sign(span) != Sign(lKeyValue))
														animationPart->rotation.z = lKeyValue - 360.0f;
													else
														animationPart->rotation.z = lKeyValue;
												}
											}

											previousKeyframeNumber = keyframeNumber;
											previousAnimationPart = animationPart;
										}
										break;
									}
								}	
							}

							for (int w = 0; w < joints.size(); w++)
							{
								boneNameStr.Format("%s", an8File.vSequences[x].vJointAngles[y].BoneName.c_str());

								if (boneNameStr == joints[w]->name)
								{
									for (int rr = (previousKeyframeNumber + 1); rr < numberKeyframes; rr++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(rr, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[w]->name, sliceKeyframe);

										if (an8File.vSequences[x].vJointAngles[y].Axe == 1) // X
										{
											sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x;
										}
										else if (an8File.vSequences[x].vJointAngles[y].Axe == 2) // Y
										{
											sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y;
										}
										else if (an8File.vSequences[x].vJointAngles[y].Axe == 3) // Z
										{
											sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z;
										}
									}
								}
							}
						}
					}
				}

				for (int x = 0; x < animation->keyframes.size(); x++)
				{
					float3 rotation = animation->keyframes[x]->rotation;

					animation->keyframes[x]->rotation.z = rotation.x;
					animation->keyframes[x]->rotation.x = rotation.y;
					animation->keyframes[x]->rotation.y = rotation.z;
				}

				for (int w = 0; w < joints.size(); w++)
				{
					for (int x = 0; x < animation->keyframes.size(); x++)
					{
						for (int y = 0; y < animation->keyframes[x]->animationParts.size(); y++)
						{
							if (animation->keyframes[x]->animationParts[y]->name == joints[w]->name)
							{
								/*float3 axisX = float3(joints[w]->orientationVectorX.x, joints[w]->orientationVectorX.y, joints[w]->orientationVectorX.z);
								float3 axisY = float3(joints[w]->orientationVectorY.x, joints[w]->orientationVectorY.y, joints[w]->orientationVectorY.z);
								float3 axisZ = float3(joints[w]->orientationVectorZ.x, joints[w]->orientationVectorZ.y, joints[w]->orientationVectorZ.z);

								float3 rotation = animation->keyframes[x]->animationParts[y]->rotation;

								Quat quatRotation = Quat::identity;
								quatRotation = quatRotation * Quat::RotateAxisAngle(axisX, AN8XToRadian(rotation.x));
								quatRotation = quatRotation * Quat::RotateAxisAngle(axisY, AN8XToRadian(rotation.y));
								quatRotation = quatRotation * Quat::RotateAxisAngle(axisZ, AN8XToRadian(rotation.z));

								float4x4 matrix = float4x4(quatRotation);

								float3 eulerAngles = quatRotation.ToEulerXYZ();
								eulerAngles = float3(AN8XToDegree(eulerAngles.x), AN8XToDegree(eulerAngles.y), AN8XToDegree(eulerAngles.z));

								animation->keyframes[x]->animationParts[y]->rotation.x = eulerAngles.x;
								animation->keyframes[x]->animationParts[y]->rotation.y = eulerAngles.y;
								animation->keyframes[x]->animationParts[y]->rotation.z = eulerAngles.z;*/

								float3 rotation = animation->keyframes[x]->animationParts[y]->rotation;

								animation->keyframes[x]->animationParts[y]->rotation.z = rotation.x;
								animation->keyframes[x]->animationParts[y]->rotation.x = rotation.y;
								animation->keyframes[x]->animationParts[y]->rotation.y = rotation.z;
							}
						}
					}
				}

				animations.push_back(animation);
			}
		}
	}

	if ((an8File.NbrScene > 0) && (animations.size() > 0))
	{
		CAnimation* animation = animations[0];

		int x = 0;
		std::vector<An8FigureElement*> figureElements = an8File.vScenes[x].GetFigureElements();

		int y = 0;
		if (figureElements.size() > 0)
		{
			for (int w = 0; w < figureElements[y]->NbrController; w++)
			{
				if (figureElements[y]->vControllers[w].Name == "position") // TopJoint
				{
					if (figureElements[y]->vControllers[w].HasTrack)
					{
						if (figureElements[y]->vControllers[w].Track.NbrPointKey > 0)
						{
							CKeyframe defaultKeyframe;
							defaultKeyframe.translation.x = 0;
							defaultKeyframe.translation.y = 0;
							defaultKeyframe.translation.z = 0;

							CKeyframe* previousKeyframe = &defaultKeyframe;
							int previousKeyframeNumber = -1;

							for (int z = 0; z < figureElements[y]->vControllers[w].Track.NbrPointKey; z++)
							{
								int keyframeNumber = figureElements[y]->vControllers[w].Track.vPointKeys[z].FrameNumber;
								
								CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

								if (keyframeNumber == (previousKeyframeNumber + 1))
								{
									keyframe->translation.x = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.x;
									keyframe->translation.y = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.y;
									keyframe->translation.z = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.z;
								}
								else
								{
									if (z == 0)
									{
										for (int y = 0; y < keyframeNumber; y++)
										{
											CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

											sliceKeyframe->translation.x = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.x;
											sliceKeyframe->translation.y = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.y;
											sliceKeyframe->translation.z = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.z;
										}
										keyframe->translation.x = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.x;
										keyframe->translation.y = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.y;
										keyframe->translation.z = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.z;
									}
									else
									{
										int slices = keyframeNumber - (previousKeyframeNumber+1);
										float3 deltaSlice(
											(figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.x - previousKeyframe->translation.x) / (slices + 1),
											(figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.y - previousKeyframe->translation.y) / (slices + 1),
											(figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.z - previousKeyframe->translation.z) / (slices + 1)
											);
										for (int y = 0; y < slices; y++)
										{
											CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

											sliceKeyframe->translation.x = previousKeyframe->translation.x + (deltaSlice.x * (y + 1));
											sliceKeyframe->translation.y = previousKeyframe->translation.y + (deltaSlice.y * (y + 1));
											sliceKeyframe->translation.z = previousKeyframe->translation.z + (deltaSlice.z * (y + 1));
											sliceKeyframe->translationIsInterpolated[0] = true;
											sliceKeyframe->translationIsInterpolated[1] = true;
											sliceKeyframe->translationIsInterpolated[2] = true;
										}

										keyframe->translation.x = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.x;
										keyframe->translation.y = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.y;
										keyframe->translation.z = figureElements[y]->vControllers[w].Track.vPointKeys[z].Value.z;
									}
								}

								previousKeyframeNumber = keyframeNumber;
								previousKeyframe = keyframe;
							}

							for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->translation.x = previousKeyframe->translation.x;
								sliceKeyframe->translation.y = previousKeyframe->translation.y;
								sliceKeyframe->translation.z = previousKeyframe->translation.z;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void CObjToAn8Dlg::ParseAn8BoneRecursive(An8Bone bone, std::vector<CJoint*>& joints, float3 position, CJoint* parent, AN8XMATRIX* i_ParentAbsoluteMatrix)
{
	CString nodeName = bone.Name.c_str();

	AN8XMATRIX absoluteMatrix;

	if ((nodeName.Trim() != "TopJoint") && (nodeName.Find("root") == -1))
	{
		CJoint* joint = NULL;

		// skip bones, only use joints
		if (nodeName.Find("-") == -1)
		{
			joint = new CJoint();
			joint->name = nodeName;
			joint->positionAbsolute.x = position.y;
			joint->positionAbsolute.y = position.z;
			joint->positionAbsolute.z = position.x;

			joint->parent = parent;

			if (parent != NULL)
			{
				parent->children.push_back(joint);
			}

			parent = joint;

			joints.push_back(joint);
		}

		// this little bit from anipov
		AN8XBone newBone;
		newBone.Name = bone.Name;
		newBone.Orientation.x = bone.Orientation.x;
		newBone.Orientation.y = bone.Orientation.y;
		newBone.Orientation.z = bone.Orientation.z; 
		newBone.Orientation.w = bone.Orientation.w;
		newBone.ParentName = bone.ParentName;
		newBone.Length = bone.Length;

		// Compute Absolute matrix /////////////////////////////////////
		AN8XMATRIX boneMatrix;
		AN8XMatrixRotationQuaternion( &boneMatrix , &bone.Orientation );
		AN8XMATRIX boneTranslationMatrix;
		boneTranslationMatrix._41 = 0.0f; // x
		boneTranslationMatrix._42 = bone.Length; // y
		boneTranslationMatrix._43 = 0.0f; // z
		AN8XMatrixMultiply(&boneMatrix,&boneMatrix,&boneTranslationMatrix);
		
		AN8XMatrixMultiply(&absoluteMatrix,i_ParentAbsoluteMatrix,&boneMatrix);

		// Compute Bone Bind Position ////////////////////////////
		newBone.BoneStartPosition = AN8XVECTOR3(0.0f,0.0f,0.0f);
		newBone.BoneEndPosition = AN8XVECTOR3(0.0f,bone.Length,0.0f);
		AN8XVECTOR4 vec4;
		AN8XVec3Transform(&vec4,&newBone.BoneStartPosition,i_ParentAbsoluteMatrix);
		newBone.BoneStartPosition.x = vec4.x;
		newBone.BoneStartPosition.y = vec4.y;
		newBone.BoneStartPosition.z = vec4.z;

		AN8XVECTOR4 vec4OriginNew;
		AN8XVECTOR3 vec3Origin(0.0f,0.0f,0.0f);
		AN8XVec3Transform(&vec4OriginNew,&vec3Origin,&absoluteMatrix);
		newBone.BoneEndPosition.x = vec4OriginNew.x;
		newBone.BoneEndPosition.y = vec4OriginNew.y;
		newBone.BoneEndPosition.z = vec4OriginNew.z;
		
		position.x = newBone.BoneEndPosition.x;
		position.y = newBone.BoneEndPosition.y;
		position.z = newBone.BoneEndPosition.z;

		AN8XVECTOR3 vec3X(1.0f,0.0f,0.0f);
		AN8XVECTOR3 vec3Y(0.0f,1.0f,0.0f);
		AN8XVECTOR3 vec3Z(0.0f,0.0f,1.0f);

		AN8XVECTOR4 vec4XNew;
		AN8XVECTOR4 vec4YNew;
		AN8XVECTOR4 vec4ZNew;
		AN8XVec3Transform(&vec4XNew,&vec3X,&absoluteMatrix);
		AN8XVec3Transform(&vec4YNew,&vec3Y,&absoluteMatrix);
		AN8XVec3Transform(&vec4ZNew,&vec3Z,&absoluteMatrix);

		AN8XVECTOR3 new3X = AN8XVECTOR3(vec4XNew.x - vec4OriginNew.x, vec4XNew.y - vec4OriginNew.y, vec4XNew.z - vec4OriginNew.z);
		AN8XVECTOR3 new3Y = AN8XVECTOR3(vec4YNew.x - vec4OriginNew.x, vec4YNew.y - vec4OriginNew.y, vec4YNew.z - vec4OriginNew.z);
		AN8XVECTOR3 new3Z = AN8XVECTOR3(vec4ZNew.x - vec4OriginNew.x, vec4ZNew.y - vec4OriginNew.y, vec4ZNew.z - vec4OriginNew.z);

		AN8XVec3Normalize(&new3X, &new3X);
		AN8XVec3Normalize(&new3Y, &new3Y);
		AN8XVec3Normalize(&new3Z, &new3Z);

		if (joint != NULL)
		{
			joint->orientationVectorX = float3(new3X.x, new3X.y, new3X.z);
			joint->orientationVectorY = float3(new3Y.x, new3Y.y, new3Y.z);
			joint->orientationVectorZ = float3(new3Z.x, new3Z.y, new3Z.z);
		}
	}

	for(int i = 0; i < bone.NbrBone; i++)
    {
        // recursively call this
		ParseAn8BoneRecursive(bone.vBones[i], joints, position, parent, &absoluteMatrix);
    }
}

int CObjToAn8Dlg::FindCaseInsensitive(CString inputString, CString string, int start)
{
	return inputString.MakeUpper().Find(string.MakeUpper(), start);
}

void CObjToAn8Dlg::RecalculateUVs(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, bool specifyTileSize, float tileU, float tileV)
{
	CString inputFolder = inputFile.Mid(0, ((inputFile.ReverseFind('\\')+1)));

	if (foundTextureUV)
	{
		std::vector<CUVCoordinate*>::iterator	iteruvCoordinates;
		for (iteruvCoordinates = uvCoordinates.begin(); iteruvCoordinates != uvCoordinates.end(); iteruvCoordinates++)
		{
			delete *iteruvCoordinates;
		}

		uvCoordinates.clear();

		std::vector<CGroup*>::iterator	itergroups;
			for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;
			
			int width = 32;
			int height = 32;

			std::vector<CPolygon*>::iterator	iterpolygon;
			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));

				if (specifyTileSize)
				{
					width = tileU;
					height = tileV;
				}
				else
				{
					CString materialName = polygon->materialName;

					std::vector<CMaterialFile*>::iterator	itermaterialFiles;
					for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
					{
						CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

						std::vector<CMaterial*>::iterator	itermaterial;
						for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
						{
							CMaterial* material = (CMaterial*)*itermaterial;

							if (material->name == materialName)
							{
								if (material->diffuseTexture.contains)
								{
									if (material->diffuseTexture.stringValue.GetLength() > 4)
									{
										CString imageFilename = material->diffuseTexture.stringValue;
										if (imageFilename.Find("\\") == -1)
										{
											imageFilename = inputFolder + imageFilename;
										}
										if (FindCaseInsensitive(material->diffuseTexture.stringValue, ".gif", (material->diffuseTexture.stringValue.GetLength()-4)) == (material->diffuseTexture.stringValue.GetLength()-4))
										{
											CxImage image(imageFilename, CXIMAGE_FORMAT_GIF);
											width = image.GetWidth();
											height = image.GetHeight();
											image.Destroy();
										}
										else if (FindCaseInsensitive(material->diffuseTexture.stringValue, ".bmp", (material->diffuseTexture.stringValue.GetLength()-4)) == (material->diffuseTexture.stringValue.GetLength()-4))
										{
											CxImage image(imageFilename, CXIMAGE_FORMAT_BMP);
											width = image.GetWidth();
											height = image.GetHeight();
											image.Destroy();
										}
									}
								}
							}
						}
					}
				}

				if (width < 0)
					width = 32;
				if (height < 0)
					height = 32;

				int numPolyPointsExpected = polygon->polygonPoints.size();

				MATH_NAMESPACE_NAME::Polygon mathGeoLibPolygon;

				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
				
				float3 minimum(0,0,0);
				float3 maximum(0,0,0);

				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);
					CVertice* vertice = vertices[polygonPoint->verticeIndex];
					if (vertice->vertex.x > maximum.x)
						maximum.x = vertice->vertex.x;
					if (vertice->vertex.y > maximum.y)
						maximum.y = vertice->vertex.y;
					if (vertice->vertex.z > maximum.z)
						maximum.z = vertice->vertex.z;

					if (vertice->vertex.x < minimum.x)
						minimum.x = vertice->vertex.x;
					if (vertice->vertex.y < minimum.y)
						minimum.y = vertice->vertex.y;
					if (vertice->vertex.z < minimum.z)
						minimum.z = vertice->vertex.z;

					mathGeoLibPolygon.p.push_back(float3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z));
				}

				float3 center((minimum.x + maximum.x)/2.0f, (minimum.y + maximum.y)/2.0f, (minimum.z + maximum.z)/2.0f);

				math::float3 normal = float3(0,1,0);
				for (int x = 0; x < (polygon->polygonPoints.size()-2); x++)
				{
					if (
						(!(Line::AreCollinear(mathGeoLibPolygon.p[x], mathGeoLibPolygon.p[x+1], mathGeoLibPolygon.p[x+2])))
						&&
						(mathGeoLibPolygon.p[x].Distance(mathGeoLibPolygon.p[x+1]) > 0.001)
						&&
						(mathGeoLibPolygon.p[x].Distance(mathGeoLibPolygon.p[x+2]) > 0.001)
						)
					{
						Triangle tri(mathGeoLibPolygon.p[x], mathGeoLibPolygon.p[x+1], mathGeoLibPolygon.p[x+2]);
						normal = tri.NormalCCW();
						break;
					}
				}

				/*float nx = normal.x;
				float ny = normal.y;
				float nz = normal.z;

				float norm=1;
				if ((nx<0) && (nx<ny) && (nx<nz))
					norm=-1;
				else if ((ny>0) && (nx<ny) && (ny>nz))
					norm=-1;
				else if ((nz<0) && (nz<ny) && (nx>nz))
					norm=-1;

				std::vector<float3> vvv;
				int countRows;
				if ((fabs(ny)<=(fabs(nz)+.00001)) && (fabs(ny)<=(abs(nx)+.00001)))
				{
					for (int r = 0; r < mathGeoLibPolygon.p.size(); r++)
					{
						vvv.push_back(float3(mathGeoLibPolygon.p[r].x,mathGeoLibPolygon.p[r].y,mathGeoLibPolygon.p[r].z));
					}
				}
				else if (fabs(nz)<=abs(ny)+.00001 && fabs(nz)<=fabs(nx)+.00001)
				{
					for (int r = 0; r < mathGeoLibPolygon.p.size(); r++)
					{
						vvv.push_back(float3(mathGeoLibPolygon.p[r].x,mathGeoLibPolygon.p[r].z,mathGeoLibPolygon.p[r].y));
					}
				}
				else
				{
					for (int r = 0; r < mathGeoLibPolygon.p.size(); r++)
					{
						vvv.push_back(float3(mathGeoLibPolygon.p[r].y,mathGeoLibPolygon.p[r].z,mathGeoLibPolygon.p[r].x));
					}
				}

				if (vvv.size() < 3)
					continue;

				float3 v1 = float3(vvv[0].x, vvv[0].y, vvv[0].z);
				float3 v2 = float3(vvv[1].x, vvv[1].y, vvv[1].z);
				float3 v3 = float3(vvv[2].x, vvv[2].y, vvv[2].z);

				float xd, yd;
				if ((fabs(v1.x-v2.x)<.001) && (fabs(v1.z-v2.z)<.001))
				{
					xd=fabs(v1.x-v3.x);
					yd=fabs(v1.z-v3.z);
				}
				else
				{
					xd=fabs(v1.x-v2.x);
					yd=fabs(v1.z-v2.z);
				}
				float hyp=sqrt(pow(xd,2) + pow(yd,2));

				for (int p = 0; p < vvv.size(); p++)
				{
					float tu, tv, x;

					if ((xd == 0) && (yd == 0))
					{
						x = 0.0f;
					}
					else
					{
						if (xd>yd)
							x=vvv[p].x*hyp/xd;
						else
							x=vvv[p].z*hyp/yd;
					}
					//if (flip==0)
					tu = x/(float)width*norm;
					tv = vvv[p].y/(float)height;
					//else
						//face.uv[p]=vvv[p][2]*pxls/imagex,x*pxls/imagey*norm*-1

					int foundUv = -1;
					int foundUvCounter = 0;
					for (iteruvCoordinates = uvCoordinates.begin(); iteruvCoordinates != uvCoordinates.end(); iteruvCoordinates++)
					{
						CUVCoordinate* uvCoordinate = ((CUVCoordinate*)*iteruvCoordinates);
						if ((uvCoordinate->u == tu) && (uvCoordinate->v == tv))
						{
							foundUv = foundUvCounter;
							break;
						}
						foundUvCounter++;
					}	

					if (foundUv == -1)
					{
						foundUv = uvCoordinates.size();

						CUVCoordinate* uvCoordinate = new CUVCoordinate();
						uvCoordinate->u = tu;
						uvCoordinate->v = tv;
						uvCoordinates.push_back(uvCoordinate);
					}

					polygon->polygonPoints[p]->uvIndex = foundUvCounter;
				}*/

				std::vector<float3> totalUniquePoints;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);
					CVertice* vertice = vertices[polygonPoint->verticeIndex];

					bool found = false;
					for (int x = 0; x < totalUniquePoints.size(); x++)
					{
						if ((totalUniquePoints[x].x == vertice->vertex.x) && (totalUniquePoints[x].y == vertice->vertex.y) && (totalUniquePoints[x].z == vertice->vertex.z))
						{
							found = true;
						}
					}

					if (!found)
					{
						totalUniquePoints.push_back(float3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z));
					}
				}
				

				std::vector<float2> uvTempCoordinates;
				std::vector<float2> uvTempCoordinatesOriginal;
				std::vector<float2> uvTempCoordinatesOriginalProject;
				
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);
					CVertice* vertice = vertices[polygonPoint->verticeIndex];

					float tu;
					float tv;

					// check if all really same
					if ((fabs(fabs(normal.x) - fabs(normal.y)) < 0.001) && (fabs(fabs(normal.x) - fabs(normal.z)) < 0.001))
					{
						if (Sign(normal.x) == Sign(normal.y))
						{
							// X
							// so just pick one, so X I guess to match
							float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


							tu = - verticeFloat3.z / width * Sign(normal.x);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginal.push_back(float2(tu, tv));

							

							float3 planeVector(Sign(normal.x), 0.0f, 0.0f);

							float4x4 matrix = matrix.identity;
							matrix = matrix.RotateFromTo(normal, planeVector, center);

							float4 tempVerticeFloat4(verticeFloat3, 0);
							tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

							verticeFloat3.x = tempVerticeFloat4.x;
							verticeFloat3.y = tempVerticeFloat4.y;
							verticeFloat3.z = tempVerticeFloat4.z;

							tu = - verticeFloat3.z / width * Sign(normal.x);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
						}
						else if (Sign(normal.x) == Sign(normal.z))
						{
							// X
							// so just pick one, so X I guess to match
							float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


							tu = - verticeFloat3.z / width * Sign(normal.x);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginal.push_back(float2(tu, tv));

							

							float3 planeVector(Sign(normal.x), 0.0f, 0.0f);

							float4x4 matrix = matrix.identity;
							matrix = matrix.RotateFromTo(normal, planeVector, center);

							float4 tempVerticeFloat4(verticeFloat3, 0);
							tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

							verticeFloat3.x = tempVerticeFloat4.x;
							verticeFloat3.y = tempVerticeFloat4.y;
							verticeFloat3.z = tempVerticeFloat4.z;

							tu = - verticeFloat3.z / width * Sign(normal.x);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
						}
						else
						{
							// Z
							float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


							tu = verticeFloat3.x / width * Sign(normal.z);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginal.push_back(float2(tu, tv));


							float3 planeVector(0.0f, 0.0f, Sign(normal.z));

							float4x4 matrix = matrix.identity;
							matrix = matrix.RotateFromTo(normal, planeVector, center);

							float4 tempVerticeFloat4(verticeFloat3, 0);
							tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

							verticeFloat3.x = tempVerticeFloat4.x;
							verticeFloat3.y = tempVerticeFloat4.y;
							verticeFloat3.z = tempVerticeFloat4.z;

							tu = verticeFloat3.x / width * Sign(normal.z);
							tv = -(-verticeFloat3.y / height);

							uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
						}
					}
					// now if 2 are equal, and one less
					else if ((fabs(fabs(normal.x) - fabs(normal.y)) < 0.001) && (fabs(normal.z) < fabs(normal.x)))
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


						tu = - verticeFloat3.z / width * Sign(normal.x);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));

						

						float3 planeVector(Sign(normal.x), 0.0f, 0.0f);

						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = - verticeFloat3.z / width * Sign(normal.x);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}
					else if ((fabs(fabs(normal.x) - fabs(normal.z)) < 0.001) && (fabs(normal.y) < fabs(normal.x)))
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


						tu = - verticeFloat3.z / width * Sign(normal.x);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));

						

						float3 planeVector(Sign(normal.x), 0.0f, 0.0f);

						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = - verticeFloat3.z / width * Sign(normal.x);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}
					else if ((fabs(fabs(normal.y) - fabs(normal.z)) < 0.001) && (fabs(normal.x) < fabs(normal.y)))
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);


						tu = verticeFloat3.x / width * Sign(normal.z);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));


						float3 planeVector(0.0f, 0.0f, Sign(normal.z));

						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = verticeFloat3.x / width * Sign(normal.z);
						tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}
					// else pick the highest component
 					else if ((fabs(normal.x) >= fabs(normal.y)) && (fabs(normal.x) >= fabs(normal.z)))
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

						tu = - verticeFloat3.z / width * Sign(normal.x);
                        tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));



						float3 planeVector(Sign(normal.x), 0.0f, 0.0f);

						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = - verticeFloat3.z / width * Sign(normal.x);
                        tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}
					else if ((fabs(normal.z) >= fabs(normal.x)) && (fabs(normal.z) >= fabs(normal.y)))
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

						tu = verticeFloat3.x / width * Sign(normal.z);
                        tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));


						float3 planeVector(0.0f, 0.0f, Sign(normal.z));

						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = verticeFloat3.x / width * Sign(normal.z);
                        tv = -(-verticeFloat3.y / height);

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}
					else
					{
						float3 verticeFloat3(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

						tu = verticeFloat3.x / width;
                        tv = -(verticeFloat3.z / height * Sign(normal.y));

						uvTempCoordinatesOriginal.push_back(float2(tu, tv));



						float3 planeVector(0.0f, Sign(normal.y), 0.0f);
						
						float4x4 matrix = matrix.identity;
						matrix = matrix.RotateFromTo(normal, planeVector, center);

						float4 tempVerticeFloat4(verticeFloat3, 0);
						tempVerticeFloat4 = matrix.Mul(tempVerticeFloat4);

						verticeFloat3.x = tempVerticeFloat4.x;
						verticeFloat3.y = tempVerticeFloat4.y;
						verticeFloat3.z = tempVerticeFloat4.z;

						tu = verticeFloat3.x / width;
                        tv = -(verticeFloat3.z / height * Sign(normal.y));

						uvTempCoordinatesOriginalProject.push_back(float2(tu, tv));
					}

					uvTempCoordinates.push_back(float2(tu, tv));
				}

				if (totalUniquePoints.size() == 4)
				{
					float2 uDimension(9999999999999,-9999999999999);
					float2 vDimension(9999999999999,-9999999999999);

					int longestSideA = -1;
					int longestSideB = -1;
					float longestDimension = 0.0;
					for (int counter = 0; counter < uvTempCoordinates.size(); counter++)
					{
						if (uvTempCoordinates[counter].x < uDimension.x)
						{
							uDimension.x = uvTempCoordinates[counter].x;
						}
						if (uvTempCoordinates[counter].x > uDimension.y)
						{
							uDimension.y = uvTempCoordinates[counter].x;
						}

						if (uvTempCoordinates[counter].y < vDimension.x)
						{
							vDimension.x = uvTempCoordinates[counter].y;
						}
						if (uvTempCoordinates[counter].y > vDimension.y)
						{
							vDimension.y = uvTempCoordinates[counter].y;
						}

						if (counter == (uvTempCoordinates.size()-1))
						{
							if (Distance(uvTempCoordinates[counter], uvTempCoordinates[0]) > longestDimension)
							{
								longestDimension = Distance(uvTempCoordinates[counter], uvTempCoordinates[0]);
								longestSideA = counter;
								longestSideB = 0;
							}
						}
						else
						{
							if (Distance(uvTempCoordinates[counter], uvTempCoordinates[counter+1]) > longestDimension)
							{
								longestDimension = Distance(uvTempCoordinates[counter], uvTempCoordinates[counter+1]);
								longestSideA = counter;
								longestSideB = counter+1;
							}
						}

					}

					float2 center2D(0,0);
					center2D.x = ((uDimension.x + uDimension.y) / 2);
					center2D.y = ((vDimension.x + vDimension.y) / 2);

					float widthAcross = fabs(uDimension.y - uDimension.x);
					float heightAcross = fabs(vDimension.y - vDimension.x);

					float2 vector = uvTempCoordinates[longestSideB] - uvTempCoordinates[longestSideA];
					vector = vector.Normalized();
					float2 vectorX(Sign(vector.x), 0.0);
					float2 vectorY(0.0, Sign(vector.y));

					float angleBetweenX = RadToDeg(vector.AngleBetween(vectorX));
					if (angleBetweenX > 90)
					{
						angleBetweenX = 180.0f - angleBetweenX;
					}

					float angleBetweenY = RadToDeg(vector.AngleBetween(vectorY));
					if (angleBetweenY > 90)
					{
						angleBetweenY = 180.0f - angleBetweenY;
					}

					// flip if positive, should go opposite direction
					if (
						(vector.x < 0) && (vector.y > 0)
						|| 
						(vector.x > 0) && (vector.y < 0)
						)
					{
						angleBetweenX = fabs(angleBetweenX);
						angleBetweenY = fabs(angleBetweenY);
					}
					else
					{
						angleBetweenX = -fabs(angleBetweenX);
						angleBetweenY = -fabs(angleBetweenY);
					}

					if (fabs(angleBetweenX) < 89.9f)
					{
						angleBetweenX = DegToRad(angleBetweenX);
						angleBetweenY = DegToRad(angleBetweenY);
					
						for (int counter = 0; counter < uvTempCoordinates.size(); counter++)
						{
							if (
							(vector.x < 0) && (vector.y > 0)
							|| 
							(vector.x > 0) && (vector.y < 0)
							)
							{
								float inputX = uvTempCoordinates[counter].x - center2D.x;
								float inputY = uvTempCoordinates[counter].y - center2D.y;

								uvTempCoordinates[counter].x = inputX*cos(angleBetweenX) - inputY*sin(angleBetweenX);
								uvTempCoordinates[counter].y = inputX*sin(angleBetweenX) + inputY*cos(angleBetweenX);

								uvTempCoordinates[counter].x = uvTempCoordinates[counter].x + center2D.x;
								uvTempCoordinates[counter].y = uvTempCoordinates[counter].y + center2D.y;
							}
							else
							{
								float inputX = uvTempCoordinates[counter].x - center2D.x;
								float inputY = uvTempCoordinates[counter].y - center2D.y;

								uvTempCoordinates[counter].x = inputX*cos(angleBetweenX) - inputY*sin(angleBetweenX);
								uvTempCoordinates[counter].y = inputX*sin(angleBetweenX) + inputY*cos(angleBetweenX);

								uvTempCoordinates[counter].x = uvTempCoordinates[counter].x + center2D.x;
								uvTempCoordinates[counter].y = uvTempCoordinates[counter].y + center2D.y;
							}
						}
					}
				}
				
				
				

				int uvCounter = 0;

				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);
					CVertice* vertice = vertices[polygonPoint->verticeIndex];

					int foundUv = -1;
					int foundUvCounter = 0;
					for (iteruvCoordinates = uvCoordinates.begin(); iteruvCoordinates != uvCoordinates.end(); iteruvCoordinates++)
					{
						CUVCoordinate* uvCoordinate = ((CUVCoordinate*)*iteruvCoordinates);
						if ((uvCoordinate->u == uvTempCoordinates[uvCounter].x) && (uvCoordinate->v == uvTempCoordinates[uvCounter].y))
						{
							foundUv = foundUvCounter;
							break;
						}
						foundUvCounter++;
					}	

					if (foundUv == -1)
					{
						foundUv = uvCoordinates.size();

						CUVCoordinate* uvCoordinate = new CUVCoordinate();
						uvCoordinate->u = uvTempCoordinates[uvCounter].x;
						uvCoordinate->v = uvTempCoordinates[uvCounter].y;
						uvCoordinates.push_back(uvCoordinate);
					}

					polygonPoint->uvIndex = foundUvCounter;

					uvCounter++;
				}
			}
		}
	}
}

CString CObjToAn8Dlg::ReplaceStringInPlace(CString inStr, CString source, CString change)
{
	inStr.Replace(source, change);
	return inStr;
}

void CObjToAn8Dlg::RenameMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;

		std::vector<CMaterial*>::iterator	itermaterial;
		std::vector<CMaterial*> doneMaterials;
		std::vector<CMaterial*> dupeMaterials;

		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			CString originalName = material->name;
			if (material->diffuseTexture.contains)
			{
				int extensionSpot = material->diffuseTexture.stringValue.ReverseFind('.');
				material->name = material->diffuseTexture.stringValue.Mid(0, extensionSpot);
				if (specialKeywordMode)
				{
					if (FindCaseInsensitive(originalName, "Transparent") != -1)
					{
						if (FindCaseInsensitive(material->name, "Transparent") == -1) // handle if in bmp name
							material->name += "Transparent";
					}
					if (FindCaseInsensitive(originalName, "EnvMapping") != -1)
					{
						if (FindCaseInsensitive(material->name, "EnvMapping") == -1) // handle if in bmp name
							material->name += "EnvMapping";
					}
					if (FindCaseInsensitive(originalName, "TopFlag") != -1)
					{
						if (FindCaseInsensitive(material->name, "TopFlag") == -1) // handle if in bmp name
							material->name += "TopFlag";
					}
					if (FindCaseInsensitive(originalName, "Decal") != -1)
					{
						if (FindCaseInsensitive(material->name, "Decal") == -1) // handle if in bmp name
							material->name += "Decal";
					}
					if (FindCaseInsensitive(originalName, "CullBoth") != -1)
					{
						if (FindCaseInsensitive(material->name, "CullBoth") == -1) // handle if in bmp name
							material->name += "CullBoth";
					}
					if (FindCaseInsensitive(originalName, "CullFront") != -1)
					{
						if (FindCaseInsensitive(material->name, "CullFront") == -1) // handle if in bmp name
							material->name += "CullFront";
					}
					if (FindCaseInsensitive(originalName, "MirrorS") != -1)
					{
						if (FindCaseInsensitive(material->name, "MirrorS") == -1) // handle if in bmp name
							material->name += "MirrorS";
					}
					if (FindCaseInsensitive(originalName, "ClampS") != -1)
					{
						if (FindCaseInsensitive(material->name, "ClampS") == -1) // handle if in bmp name
							material->name += "ClampS";
					}
					if (FindCaseInsensitive(originalName, "MirrorT") != -1)
					{
						if (FindCaseInsensitive(material->name, "MirrorT") == -1) // handle if in bmp name
							material->name += "MirrorT";
					}
					if (FindCaseInsensitive(originalName, "ClampT") != -1)
					{
						if (FindCaseInsensitive(material->name, "ClampT") == -1) // handle if in bmp name
							material->name += "ClampT";
					}
					if (FindCaseInsensitive(originalName, "TextureLOD") != -1)
					{
						if (FindCaseInsensitive(material->name, "TextureLOD") == -1) // handle if in bmp name
							material->name += "TextureLOD";
					}
					if (FindCaseInsensitive(originalName, "TextureNoMipmap") != -1)
					{
						if (FindCaseInsensitive(material->name, "TextureNoMipmap") == -1) // handle if in bmp name
							material->name += "TextureNoMipmap";
					}
					if (FindCaseInsensitive(originalName, "TexturePass") != -1)
					{
						if (FindCaseInsensitive(material->name, "TexturePass") == -1) // handle if in bmp name
							material->name += "TexturePass";
					}
					int texScaleSPosition = FindCaseInsensitive(originalName, "TexScaleS", 0);
					if ((texScaleSPosition != -1))
					{
						float tempSFloatValue = atof(ReplaceStringInPlace(originalName.Mid(texScaleSPosition + 9), "_", "."));
						CString tempStr;
						tempStr.Format("TexScaleS%.6f", tempSFloatValue);
						if (FindCaseInsensitive(material->name, tempStr) == -1) // handle if in bmp name
							material->name += tempStr;
					}

					int texScaleTPosition = FindCaseInsensitive(originalName, "TexScaleT", 0);
					if ((texScaleTPosition != -1))
					{
						float tempTFloatValue = atof(ReplaceStringInPlace(originalName.Mid(texScaleTPosition + 9), "_", "."));
						CString tempStr;
						tempStr.Format("TexScaleT%.6f", tempTFloatValue);
						if (FindCaseInsensitive(material->name, tempStr) == -1) // handle if in bmp name
							material->name += tempStr;
					}
				}
			}
			else
			{
				material->name = "Untextured";
				if (FindCaseInsensitive(originalName, "CullBoth") != -1)
				{
					if (FindCaseInsensitive(material->name, "CullBoth") == -1) // handle if in bmp name
						material->name += "CullBoth";
				}
				if (FindCaseInsensitive(originalName, "CullFront") != -1)
				{
					if (FindCaseInsensitive(material->name, "CullFront") == -1) // handle if in bmp name
						material->name += "CullFront";
				}
			}
			if ((material->diffuseColor.contains) && ((material->diffuseColor.r < 1.0) || (material->diffuseColor.g < 1.0) || (material->diffuseColor.b < 1.0)))
			{
				CString colorStr;
				colorStr.Format("RGB%02X%02X%02X", ColorFloatToHex(material->diffuseColor.r), ColorFloatToHex(material->diffuseColor.g), ColorFloatToHex(material->diffuseColor.b));
				material->name += colorStr;
			}
			if (material->alpha.contains)
			{
				if (material->alpha.floatValue != 1.0)
				{
					CString tempStr;
					tempStr.Format("Alpha%.3f", material->alpha.floatValue);
					material->name += tempStr;
				}
			}

			bool dupe = false;
			for (int m = 0; m < doneMaterials.size(); m++)
			{
				if (doneMaterials[m]->name == material->name)
				{
					dupe = true;
					break;
				}
			}

			replacementStrings[originalName] = material->name;

			if (dupe)
			{
				dupeMaterials.push_back(material);
			}
			else
			{
				doneMaterials.push_back(material);
			}
		}

		for (int m = 0; m < dupeMaterials.size(); m++)
		{
			materialFile->materials.erase(std::remove(materialFile->materials.begin(), materialFile->materials.end(), dupeMaterials[m]), materialFile->materials.end());
		}

		std::vector<CGroup*>::iterator	itergroups;
		for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;

			std::vector<CPolygon*>::iterator	iterpolygon;
			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));

				std::map<CString,CString>::iterator iterreplacements;
				for (iterreplacements = replacementStrings.begin(); iterreplacements != replacementStrings.end(); iterreplacements++ )
				{
					CString name = (CString)((*iterreplacements).first);
					CString nameReplacement = (CString)((*iterreplacements).second);

					if (polygon->materialName == name)
					{
						polygon->materialName = nameReplacement;
					}

				}
			}
		}
	}
}

void CObjToAn8Dlg::RenameGroups(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	int groupCounter = 1;
	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;
	
		CString tempStr;
		tempStr.Format("%d", groupCounter);
		group->name = "Group" + tempStr;
		groupCounter++;
	}
}

float CObjToAn8Dlg::round(float value)
{
   return (value < 0 ? ceil((value)-0.5) : floor((value)+0.5));
}

float CObjToAn8Dlg::roundtenths(float value)
{
	return 0.1f * round(10.0f * value);
}

float CObjToAn8Dlg::roundhundreths(float value)
{
	return 0.01f * round(100.0f * value);
}

void CObjToAn8Dlg::MoveVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float xMove, float yMove, float zMove)
{
	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = vertice->vertex.x + xMove;
		vertice->vertex.y = vertice->vertex.y + yMove;
		vertice->vertex.z = vertice->vertex.z + zMove;
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x += xMove;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y += yMove;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z += zMove;
				}
			}
		}
	}
	
	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x += xMove;
				joints[x]->positionAbsolute.y += yMove;
				joints[x]->positionAbsolute.z += zMove;
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x += xMove;
						animations[x]->keyframes[y]->animationParts[z]->translation.y += yMove;
						animations[x]->keyframes[y]->animationParts[z]->translation.z += zMove;
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::RecenterVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	float centerX = 0;
	float centerY = 0;
	float centerZ = 0;

	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		centerX += (*iterPointList)->vertex.x;
		centerY += (*iterPointList)->vertex.y;
		centerZ += (*iterPointList)->vertex.z;
	}

	centerX = centerX / vertices.size();
	centerY = centerY / vertices.size();
	centerZ = centerZ / vertices.size();

	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = vertice->vertex.x - centerX;
		vertice->vertex.y = vertice->vertex.y - centerY;
		vertice->vertex.z = vertice->vertex.z - centerZ;
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x -= centerX;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y -= centerY;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z -= centerZ;
				}
			}
		}
	}

	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x -=centerX;
				joints[x]->positionAbsolute.y -= centerY;
				joints[x]->positionAbsolute.z -= centerZ;
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x -= centerX;
						animations[x]->keyframes[y]->animationParts[z]->translation.y -= centerY;
						animations[x]->keyframes[y]->animationParts[z]->translation.z -= centerZ;
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::RoundVerticesTenths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = roundtenths(vertice->vertex.x);
		vertice->vertex.y = roundtenths(vertice->vertex.y);
		vertice->vertex.z = roundtenths(vertice->vertex.z);
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x = roundtenths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y = roundtenths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z = roundtenths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z);
				}
			}
		}
	}

	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x = roundtenths(joints[x]->positionAbsolute.x);
				joints[x]->positionAbsolute.y = roundtenths(joints[x]->positionAbsolute.y);
				joints[x]->positionAbsolute.z = roundtenths(joints[x]->positionAbsolute.z);
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x = roundtenths(animations[x]->keyframes[y]->animationParts[z]->translation.x);
						animations[x]->keyframes[y]->animationParts[z]->translation.y = roundtenths(animations[x]->keyframes[y]->animationParts[z]->translation.y);
						animations[x]->keyframes[y]->animationParts[z]->translation.z = roundtenths(animations[x]->keyframes[y]->animationParts[z]->translation.z);
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::RoundVerticesHundredths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = roundhundreths(vertice->vertex.x);
		vertice->vertex.y = roundhundreths(vertice->vertex.y);
		vertice->vertex.z = roundhundreths(vertice->vertex.z);
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z);
				}
			}
		}
	}

	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x = roundhundreths(joints[x]->positionAbsolute.x);
				joints[x]->positionAbsolute.y = roundhundreths(joints[x]->positionAbsolute.y);
				joints[x]->positionAbsolute.z = roundhundreths(joints[x]->positionAbsolute.z);
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->translation.x);
						animations[x]->keyframes[y]->animationParts[z]->translation.y = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->translation.y);
						animations[x]->keyframes[y]->animationParts[z]->translation.z = roundhundreths(animations[x]->keyframes[y]->animationParts[z]->translation.z);
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::ScaleUVs(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float scaleFactor)
{
	if (scaleFactor == 0)
		return;

	std::vector<CUVCoordinate*>::iterator	iterUVList;
	int uvIndex = 0;
	for (iterUVList = uvCoordinates.begin(); iterUVList != uvCoordinates.end(); iterUVList++)
	{
		CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;
		uv->u *= scaleFactor;
		uv->v *= scaleFactor;
	}
}

void CObjToAn8Dlg::ScaleVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float scaleFactor)
{
	if (scaleFactor == 0)
		return;

	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = (vertice->vertex.x * scaleFactor);
		vertice->vertex.y = (vertice->vertex.y * scaleFactor);
		vertice->vertex.z = (vertice->vertex.z * scaleFactor);
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x *= scaleFactor;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y *= scaleFactor;
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z *= scaleFactor;
				}
			}
		}
	}

	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x *= scaleFactor;
				joints[x]->positionAbsolute.y *= scaleFactor;
				joints[x]->positionAbsolute.z *= scaleFactor;
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x *= scaleFactor;
						animations[x]->keyframes[y]->animationParts[z]->translation.y *= scaleFactor;
						animations[x]->keyframes[y]->animationParts[z]->translation.z *= scaleFactor;
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::RoundVertices(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles,  std::vector<CJoint*> joints, std::vector<CAnimation*> animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CVertice*>::iterator	iterPointList;
	for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
	{
		CVertice* vertice = (CVertice*)*iterPointList;

		CString formatVertice;
		vertice->vertex.x = round(vertice->vertex.x);
		vertice->vertex.y = round(vertice->vertex.y);
		vertice->vertex.z = round(vertice->vertex.z);
	}

	for (int x = 0; x < animations.size(); x++)
	{
		for (int y = 0; y < animations[x]->keyframes.size(); y++)
		{
			for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
			{
				for (int b = 0; b < animations[x]->keyframes[y]->animationParts[z]->shapeVertices.size(); b++)
				{
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x = round(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.x);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y = round(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.y);
					animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z = round(animations[x]->keyframes[y]->animationParts[z]->shapeVertices[b].translation.z);
				}
			}
		}
	}

	if (animations.size() > 0)
	{
		bool allBlendShape = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (animations[x]->treatAsBlendShape == false)
			{
				allBlendShape = false;
				break;
			}
		}

		if (allBlendShape)
		{
			for (int x = 0; x < joints.size(); x++)
			{
				joints[x]->positionAbsolute.x = round(joints[x]->positionAbsolute.x);
				joints[x]->positionAbsolute.y = round(joints[x]->positionAbsolute.y);
				joints[x]->positionAbsolute.z = round(joints[x]->positionAbsolute.z);
			}

			for (int x = 0; x < animations.size(); x++)
			{
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					for (int z = 0; z < animations[x]->keyframes[y]->animationParts.size(); z++)
					{
						animations[x]->keyframes[y]->animationParts[z]->translation.x = round(animations[x]->keyframes[y]->animationParts[z]->translation.x);
						animations[x]->keyframes[y]->animationParts[z]->translation.y = round(animations[x]->keyframes[y]->animationParts[z]->translation.y);
						animations[x]->keyframes[y]->animationParts[z]->translation.z = round(animations[x]->keyframes[y]->animationParts[z]->translation.z);
					}
				}
			}
		}
	}
}

void CObjToAn8Dlg::StripImagePaths(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			if (material->diffuseTexture.contains)
			{
				if (material->diffuseTexture.stringValue.ReverseFind('\\') != -1)
				{
					material->diffuseTexture.stringValue = material->diffuseTexture.stringValue.Mid(material->diffuseTexture.stringValue.ReverseFind('\\') + 1);
				}

				if (material->diffuseTexture.stringValue.ReverseFind('/') != -1)
				{
					material->diffuseTexture.stringValue = material->diffuseTexture.stringValue.Mid(material->diffuseTexture.stringValue.ReverseFind('/') + 1);
				}
			}
		}
	}
}

void CObjToAn8Dlg::IgnoreShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;
			material->diffuseColor.contains = true;
			material->diffuseColor.r = 1.0;
			material->diffuseColor.g = 1.0;
			material->diffuseColor.b = 1.0;
			material->ambientColor.contains = false;
			material->illum.contains = true;
			material->illum.floatValue = 0.0;
			material->alpha.contains = true;
			material->alpha.floatValue = 1.0;
			material->phongSpectral.contains = false;
			material->spectralColor.contains = false;
		}
	}
}

void CObjToAn8Dlg::IgnoreShadingPoint7(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
				CMaterial* material = (CMaterial*)*itermaterial;
				if ((material->diffuseColor.contains) && (material->diffuseColor.r == 0.7) && (material->diffuseColor.g == 0.7) && (material->diffuseColor.b == 0.7))
				{
					material->diffuseColor.contains = true;
					material->diffuseColor.r = 1.0;
					material->diffuseColor.g = 1.0;
					material->diffuseColor.b = 1.0;
					material->ambientColor.contains = false;
					material->illum.contains = true;
					material->illum.floatValue = 0.0;
					material->alpha.contains = true;
					material->alpha.floatValue = 1.0;
					material->phongSpectral.contains = false;
					material->spectralColor.contains = false;
				}
		}
	}
}

void CObjToAn8Dlg::MergeLikeMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			std::vector<CMaterial*>::iterator	itermaterialcheck;
			for (itermaterialcheck = materialFile->materials.begin(); itermaterialcheck != materialFile->materials.end(); itermaterialcheck++)
			{
				CMaterial* materialCheck = (CMaterial*)*itermaterialcheck;

				if (material == materialCheck) // only test against ones before it
					break;

				if (material->alpha.contains != materialCheck->alpha.contains)
					continue;

				if (material->alpha.contains)
				{
					if (material->alpha.floatValue != materialCheck->alpha.floatValue)
						continue;
				}

				if (material->ambientColor.contains != materialCheck->ambientColor.contains)
					continue;

				if (material->ambientColor.contains)
				{
					if (material->ambientColor.r != materialCheck->ambientColor.r)
						continue;
					if (material->ambientColor.g != materialCheck->ambientColor.g)
						continue;
					if (material->ambientColor.b != materialCheck->ambientColor.b)
						continue;
				}

				if (material->diffuseColor.contains != materialCheck->diffuseColor.contains)
					continue;

				if (material->diffuseColor.contains)
				{
					if (material->diffuseColor.r != materialCheck->diffuseColor.r)
						continue;
					if (material->diffuseColor.g != materialCheck->diffuseColor.g)
						continue;
					if (material->diffuseColor.b != materialCheck->diffuseColor.b)
						continue;
				}

				if (material->diffuseTexture.contains != materialCheck->diffuseTexture.contains)
					continue;

				if (material->diffuseTexture.contains)
				{
					if (material->diffuseTexture.stringValue != materialCheck->diffuseTexture.stringValue)
						continue;
				}

				if (material->illum.contains != materialCheck->illum.contains)
					continue;

				if (material->illum.contains)
				{
					if (material->illum.floatValue != materialCheck->illum.floatValue)
						continue;
				}

				if (material->phongSpectral.contains != materialCheck->phongSpectral.contains)
					continue;

				if (material->phongSpectral.contains)
				{
					if (material->phongSpectral.floatValue != materialCheck->phongSpectral.floatValue)
						continue;
				}

				if (material->spectralColor.contains != materialCheck->spectralColor.contains)
					continue;

				if (material->spectralColor.contains)
				{
					if (material->spectralColor.r != materialCheck->spectralColor.r)
						continue;
					if (material->spectralColor.g != materialCheck->spectralColor.g)
						continue;
					if (material->spectralColor.b != materialCheck->spectralColor.b)
						continue;
				}

				if (specialKeywordMode)
				{
					if ((FindCaseInsensitive(material->name, "Transparent") >= 0) != (FindCaseInsensitive(materialCheck->name, "Transparent") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "EnvMapping") >= 0) != (FindCaseInsensitive(materialCheck->name, "EnvMapping") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TexScaleS") >= 0) != (FindCaseInsensitive(materialCheck->name, "TexScaleS") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TexScaleT") >= 0) != (FindCaseInsensitive(materialCheck->name, "TexScaleT") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "Decal") >= 0) != (FindCaseInsensitive(materialCheck->name, "Decal") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TopFlag") >= 0) != (FindCaseInsensitive(materialCheck->name, "TopFlag") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "CullBoth") >= 0) != (FindCaseInsensitive(materialCheck->name, "CullBoth") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "CullFront") >= 0) != (FindCaseInsensitive(materialCheck->name, "CullFront") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "MirrorS") >= 0) != (FindCaseInsensitive(materialCheck->name, "MirrorS") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "ClampS") >= 0) != (FindCaseInsensitive(materialCheck->name, "ClampS") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "MirrorT") >= 0) != (FindCaseInsensitive(materialCheck->name, "MirrorT") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "ClampT") >= 0) != (FindCaseInsensitive(materialCheck->name, "ClampT") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TextureLOD") >= 0) != (FindCaseInsensitive(materialCheck->name, "TextureLOD") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TextureNoMipmap") >= 0) != (materialCheck->name.Find("TextureNoMipmap") >= 0))
						break;

					if ((FindCaseInsensitive(material->name, "TexturePass") >= 0) != (FindCaseInsensitive(materialCheck->name, "TexturePass") >= 0))
						break;
				}

				// passed, merge
				removals.push_back(material);
				replacementStrings[material->name] = materialCheck->name;
				break;
			}
		}

		std::vector<CMaterial*>::iterator	iterdeletes;
		for (iterdeletes = removals.begin(); iterdeletes != removals.end(); iterdeletes++)
		{
			CMaterial* material = *iterdeletes;

			// remove
			materialFile->materials.erase(std::remove(materialFile->materials.begin(), materialFile->materials.end(), material), materialFile->materials.end());

			delete material;
		}

		std::vector<CGroup*>::iterator	itergroups;
		for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;

			std::vector<CPolygon*>::iterator	iterpolygon;
			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));

				std::map<CString,CString>::iterator iterreplacements;
				for (iterreplacements = replacementStrings.begin(); iterreplacements != replacementStrings.end(); iterreplacements++ )
				{
					CString name = (CString)((*iterreplacements).first);
					CString nameReplacement = (CString)((*iterreplacements).second);

					if (polygon->materialName == name)
					{
						polygon->materialName = nameReplacement;
					}

				}
			}
		}
	}
}

void CObjToAn8Dlg::WriteAssimpSkeleton(aiScene* scene, CJoint* joint, aiNode* parentNode)
{
	aiNode* node = new aiNode();
	node->mNumMeshes = 0;
	node->mName = aiString(joint->name.GetBuffer(0));
	joint->name.ReleaseBuffer();

	node->mNumChildren = 0;

	aiVector3D translation(0,0,0);
	if (joint->parent != NULL)
	{
		translation = aiVector3D((joint->positionAbsolute.x - joint->parent->positionAbsolute.x), (joint->positionAbsolute.y - joint->parent->positionAbsolute.y), (joint->positionAbsolute.z - joint->parent->positionAbsolute.z));
	}
	else
	{
		translation = aiVector3D(joint->positionAbsolute.x, joint->positionAbsolute.y, joint->positionAbsolute.z);
	}

	node->mTransformation = aiMatrix4x4::Translation(translation, node->mTransformation);
	
	if (parentNode->mNumChildren == 0)
	{
		parentNode->mChildren = new aiNode*[1];
		parentNode->mChildren[0] = node;
	}
	else
	{
		aiNode** newNodes = new aiNode*[parentNode->mNumChildren+1];
		for (int x = 0; x < parentNode->mNumChildren; x++)
		{
			newNodes[x] = parentNode->mChildren[x];
		}

		newNodes[parentNode->mNumChildren] = node;
		delete [] parentNode->mChildren;
		parentNode->mChildren = newNodes;
	}

	parentNode->mNumChildren++;

	for (int x = 0; x < joint->children.size(); x++)
	{
		WriteAssimpSkeleton(scene, joint->children[x], node);
	}
}

void CObjToAn8Dlg::WriteAssimpMesh(aiScene* scene, aiMesh* mesh, bool foundNormals, bool foundTextureUV, bool foundVerticeColors, std::vector<CPolygon*> polygonList, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CJoint*> joints)
{
	if (mesh != NULL)
	{
		int totalVertices = 0;

		for (int x = 0; x < polygonList.size(); x++)
		{
			totalVertices += polygonList[x]->polygonPoints.size();
		}

		mesh->mNumVertices = totalVertices;
		mesh->mVertices = new aiVector3D[mesh->mNumVertices];
		if (foundNormals)
			mesh->mNormals = new aiVector3D[mesh->mNumVertices];
		if (foundTextureUV)
		{
			mesh->mNumUVComponents[ 0 ] = mesh->mNumVertices;
			mesh->mTextureCoords[ 0 ] = new aiVector3D[ mesh->mNumUVComponents[ 0 ] ];
		}
		if (foundVerticeColors)
		{
			mesh->mColors[0] = new aiColor4D[mesh->mNumVertices];
		}

		mesh->mNumFaces = polygonList.size();
		mesh->mFaces = new aiFace[ mesh->mNumFaces ];

		int countVertices = 0;
		for (int x = 0; x < polygonList.size(); x++)
		{
			mesh->mFaces[x].mIndices = new unsigned int[polygonList[x]->polygonPoints.size()];
			mesh->mFaces[x].mNumIndices = polygonList[x]->polygonPoints.size();

			for (int y = 0; y < polygonList[x]->polygonPoints.size(); y++)
			{
				CPolygonPoint* polygonPoint = polygonList[x]->polygonPoints[y];
				
				CVertice* vertice = NULL;
				aiVector3D verticeValue(0,0,0);
				if (polygonPoint->verticeIndex != -1)
				{
					vertice = vertices[polygonPoint->verticeIndex];
					verticeValue = aiVector3D(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);
				}
				mesh->mVertices[countVertices] = verticeValue;

				CUVCoordinate* uv = NULL;
				aiVector3D uvValue(0,0,0);
				if (polygonPoint->uvIndex != -1)
				{
					uv = uvCoordinates[polygonPoint->uvIndex];
					uvValue = aiVector3D(uv->u, uv->v, 0);
				}
				if (foundTextureUV)
				{
					mesh->mTextureCoords[ 0 ][countVertices] = uvValue;
				}

				CNormal* normal = NULL;
				aiVector3D normalValue(0,0,0);
				if (polygonPoint->normalIndex != -1)
				{
					normal = normals[polygonPoint->normalIndex];
					normalValue = aiVector3D(normal->nx, normal->ny, normal->nz);
				}
				if (foundNormals)
				{
					mesh->mNormals[countVertices] = normalValue;
				}

				CVerticeColor* verticeColor = NULL;
				aiColor4D verticeColorValue(1,1,1,1);

				if (polygonPoint->verticeColorIndex != -1)
				{
					if (verticeColors[polygonPoint->verticeColorIndex]->contains)
					{
						verticeColor = verticeColors[polygonPoint->verticeColorIndex];
						verticeColorValue.r = verticeColor->color.r / 255.0f;
						verticeColorValue.g = verticeColor->color.g / 255.0f;
						verticeColorValue.b = verticeColor->color.b / 255.0f;
						verticeColorValue.a = verticeColor->color.a / 255.0f;
					}
				}
				if (foundVerticeColors)
				{
					mesh->mColors[0][countVertices] = verticeColorValue;
				}


				int found = 0;

				for (int x = 0; x < joints.size(); x++)
				{
					for (int y = 0; y < joints[x]->controlPoints.size(); y++)
					{
						CVertice* controlVertice = vertices[joints[x]->controlPoints[y]];
						if (controlVertice == vertice)
						{
							aiBone* bone = NULL;
							for (int r = 0; r < mesh->mNumBones; r++)
							{
								CString boneName = mesh->mBones[r]->mName.C_Str();
								if (boneName == joints[x]->name)
								{
									bone = mesh->mBones[r];
									break;
								}
							}

							if (bone == NULL)
							{
								bone = new aiBone();
								bone->mName = joints[x]->name.GetBuffer(0);
								joints[x]->name.ReleaseBuffer();

								if (mesh->mNumBones == 0)
								{
									mesh->mBones = new aiBone*[1];
									mesh->mBones[0] = bone;
								}
								else
								{
									aiBone** newBones = new aiBone*[mesh->mNumBones+1];
									for (int x = 0; x < mesh->mNumBones; x++)
									{
										newBones[x] = mesh->mBones[x];
									}

									newBones[mesh->mNumBones] = bone;
									delete [] mesh->mBones;

									mesh->mBones = newBones;
								}
								mesh->mNumBones++;
							}

							if (bone->mNumWeights == 0)
							{
								bone->mWeights = new aiVertexWeight[1];
							}
							else
							{
								aiVertexWeight* newWeights = new aiVertexWeight[bone->mNumWeights+1];

								for (int r = 0; r < bone->mNumWeights; r++)
								{
									newWeights[r] = bone->mWeights[r];
								}

								delete [] bone->mWeights;
								bone->mWeights = newWeights;
							}

							bone->mWeights[bone->mNumWeights].mVertexId = countVertices;
							bone->mWeights[bone->mNumWeights].mWeight = 1.0f;

							bone->mNumWeights++;


							found++;
							break;
						}
					}

					if (found > 0)
						break;
				}

				mesh->mFaces[x].mIndices[y] = countVertices;

				countVertices++;
			}
		}
	}
}

void CObjToAn8Dlg::WriteAssimpFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter, CString extensionWrite, bool sortByTextureWithinGroup)
{
	Assimp::Exporter exporter;

	int exporterCount = exporter.GetExportFormatCount();
	int exportType = -1;

	const aiExportFormatDesc* exportFormatDesc = NULL;

	for (int x = 0; x < exporterCount; x++)
	{
		exportFormatDesc = exporter.GetExportFormatDescription(x);
		CString extension;
		extension = exportFormatDesc->fileExtension;

		if (extension.MakeLower() == extensionWrite)
		{
			break;
		}
	}

	aiScene scene;
	
	scene.mRootNode = new aiNode();

	int countMaterials = 0;

	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		countMaterials += materialFile->materials.size();
	}

	scene.mNumMaterials = countMaterials;
	scene.mMaterials = new aiMaterial*[ scene.mNumMaterials ];
	
	std::map<CString, int> materialLookupByNumber;

	countMaterials = 0;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			aiMaterial* newAiMaterial = new aiMaterial();
			scene.mMaterials[ countMaterials ] = newAiMaterial;

			materialLookupByNumber[material->name] = countMaterials;

			aiString materialNameString(std::string(material->name.GetBuffer(0)));
			material->name.ReleaseBuffer();

			newAiMaterial->AddProperty(&materialNameString, AI_MATKEY_NAME);

			aiColor3D color(0.f,0.f,0.f);

			if (material->ambientColor.contains)
			{
				color.r = material->ambientColor.r;
				color.g = material->ambientColor.g;
				color.b = material->ambientColor.b;
				newAiMaterial->AddProperty(&color, 1, AI_MATKEY_COLOR_AMBIENT);
			}

			if (material->diffuseColor.contains)
			{
				color.r = material->diffuseColor.r;
				color.g = material->diffuseColor.g;
				color.b = material->diffuseColor.b;
				newAiMaterial->AddProperty(&color, 1, AI_MATKEY_COLOR_DIFFUSE);
			}

			if (material->spectralColor.contains)
			{
				color.r = material->spectralColor.r;
				color.g = material->spectralColor.g;
				color.b = material->spectralColor.b;
				newAiMaterial->AddProperty(&color, 1, AI_MATKEY_COLOR_SPECULAR);
			}

			if (material->alpha.contains)
			{
				newAiMaterial->AddProperty(&material->alpha.floatValue, 1, AI_MATKEY_OPACITY);
			}

			if (material->diffuseTexture.contains)
			{
				aiString materialTextureNameString(std::string(material->diffuseTexture.stringValue.GetBuffer(0)));
				material->diffuseTexture.stringValue.ReleaseBuffer();

				newAiMaterial->AddProperty(&materialTextureNameString, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0));
			}

			countMaterials++;
		}
	}

	aiNode* rootNodeAnimation = NULL;

	if (joints.size() > 0)
	{
		bool allCamera = true;

		for (int x = 0; x < animations.size(); x++)
		{
			if (!animations[x]->isCamera)
			{
				allCamera = false;
				break;
			}
		}

		if (animations.size() == 0)
		{
			allCamera = false;
		}

		if (!allCamera)
		{
			std::vector<CJoint*> rootJoints = FindRootJoints(joints);

			if (rootJoints.size() == 0)
			{
				if (doMessageBoxes)
					MessageBox("Error, no root nodes");
			}
			else if (rootJoints.size() == 1)
			{
				aiNode* topJointNode = new aiNode();
				rootNodeAnimation = topJointNode;

				topJointNode->mName = "TopJoint";
				topJointNode->mNumChildren = 0;

				aiNode* skeletonRootNode;

				// just one, if not present, add top
				CJoint* rootJoint = rootJoints[0];
				if (rootJoint->name != "TopJoint")
				{
					topJointNode->mNumChildren = 1;
					topJointNode->mChildren = new aiNode*[1];

					skeletonRootNode = new aiNode();
					topJointNode->mChildren[0] = skeletonRootNode;

					skeletonRootNode->mName = rootJoint->name.GetBuffer(0);
					rootJoint->name.ReleaseBuffer();

					skeletonRootNode->mTransformation = aiMatrix4x4::Translation(aiVector3D(rootJoint->positionAbsolute.x, rootJoint->positionAbsolute.y, rootJoint->positionAbsolute.z), skeletonRootNode->mTransformation);
				}
				else
				{
					skeletonRootNode = topJointNode;
				}

				for (int y = 0; y < rootJoint->children.size(); y++)
				{
					WriteAssimpSkeleton(&scene, rootJoint->children[y], skeletonRootNode);
				}
			}
			else
			{
				// multiple, need to write top level one
				aiNode* topJointNode = new aiNode();
				rootNodeAnimation = topJointNode;

				topJointNode->mName = "TopJoint";
				topJointNode->mNumChildren = 0;

				for (int x = 0; x < rootJoints.size(); x++)
				{
					CJoint* rootJoint = rootJoints[x];

					aiNode* skeletonRootNode = new aiNode();
					skeletonRootNode->mName = rootJoint->name.GetBuffer(0);
					rootJoint->name.ReleaseBuffer();

					skeletonRootNode->mTransformation = aiMatrix4x4::Translation(aiVector3D(rootJoints[x]->positionAbsolute.x, rootJoints[x]->positionAbsolute.y, rootJoints[x]->positionAbsolute.z), skeletonRootNode->mTransformation);

					for (int y = 0; y < rootJoints[x]->children.size(); y++)
					{
						WriteAssimpSkeleton(&scene, rootJoint->children[y], skeletonRootNode);
					}
				}
			}
		}
	}

	if (rootNodeAnimation != NULL)
	{
		scene.mRootNode->mNumChildren = 1;
		scene.mRootNode->mChildren = new aiNode*[1];
		scene.mRootNode->mChildren[0] = rootNodeAnimation;
	}

	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;

		if (regexFilterCheck)
		{
			std::tr1::regex rx(regexFilter.GetBuffer());
			std::string stdString = std::string(group->name.GetBuffer());
			regexFilter.ReleaseBuffer();

			bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
			group->name.ReleaseBuffer();

			if (!isMatch)
			{
				continue;
			}
		}

		aiNode* node = new aiNode();

		if (scene.mRootNode->mNumChildren == 0)
		{
			scene.mRootNode->mChildren = new aiNode*[1];
			scene.mRootNode->mChildren[0] = node;
		}
		else
		{
			aiNode** newNodes = new aiNode*[scene.mRootNode->mNumChildren+1];
			for (int x = 0; x < scene.mRootNode->mNumChildren; x++)
			{
				newNodes[x] = scene.mRootNode->mChildren[x];
			}

			newNodes[scene.mRootNode->mNumChildren] = node;
			delete [] scene.mRootNode->mChildren;

			scene.mRootNode->mChildren = newNodes;
		}
		scene.mRootNode->mNumChildren++;

		node->mName = group->name.GetBuffer(0);
		group->name.ReleaseBuffer();

		std::vector<CPolygon*> polygonList;

		int materialIndex = -2;
		aiMesh* mesh = NULL;

		if (sortByTextureWithinGroup)
		{
			SortPolygonGroupByTexture(group);
		}

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			int newMaterialIndex = -1;

			if (materialLookupByNumber.find(polygon->materialName) != materialLookupByNumber.end() ) 
			{
				newMaterialIndex = materialLookupByNumber[polygon->materialName];
			}

			if (materialIndex != newMaterialIndex)
			{
				WriteAssimpMesh(&scene, mesh, foundNormals, foundTextureUV, foundVerticeColors, polygonList, verticeColors, normals, uvCoordinates, vertices, joints);

				polygonList.clear();

				mesh = new aiMesh();
				mesh->mMaterialIndex = newMaterialIndex;
				if (mesh->mMaterialIndex < 0)
					mesh->mMaterialIndex = 0;

				if (scene.mNumMeshes == 0)
				{
					scene.mMeshes = new aiMesh*[1];
					scene.mMeshes[0] = mesh;
				}
				else
				{
					aiMesh** newMeshes = new aiMesh*[scene.mNumMeshes+1];
					for (int x = 0; x < scene.mNumMeshes; x++)
					{
						newMeshes[x] = scene.mMeshes[x];
					}

					newMeshes[scene.mNumMeshes] = mesh;
					delete [] scene.mMeshes;

					scene.mMeshes = newMeshes;
				}
				scene.mNumMeshes++;

				if (node->mNumMeshes == 0)
				{
					node->mMeshes = new unsigned int[1];
					node->mMeshes[0] = scene.mNumMeshes - 1;
				}
				else
				{
					unsigned int* newMeshes = new unsigned int[node->mNumMeshes+1];
					for (int x = 0; x < node->mNumMeshes; x++)
					{
						newMeshes[x] = node->mMeshes[x];
					}

					newMeshes[node->mNumMeshes] = scene.mNumMeshes - 1;
					delete [] node->mMeshes;

					node->mMeshes = newMeshes;
				}
				node->mNumMeshes++;

				polygonList.clear();

				materialIndex = newMaterialIndex;
			}

			polygonList.push_back(polygon);
		}

		if (polygonList.size() > 0)
		{
			WriteAssimpMesh(&scene, mesh, foundNormals, foundTextureUV, foundVerticeColors, polygonList, verticeColors, normals, uvCoordinates, vertices, joints);
		}
	}

	scene.mNumAnimations = animations.size();
	scene.mAnimations = new aiAnimation*[scene.mNumAnimations];

	for (int x = 0; x < animations.size(); x++)
	{
		aiAnimation* animationScene = new aiAnimation();
		scene.mAnimations[x] = animationScene;

		animationScene->mName = animations[x]->name.GetBuffer(0);
		animations[x]->name.ReleaseBuffer();
		
		// TODO
		/*if (animations[x]->isCamera)
		{
			lCamera = FbxCamera::Create(pScene, animations[x]->name);
			lCamera->SetAspect( FbxCamera::eFixedResolution, 320, 240);
			lCamera->SetApertureMode(FbxCamera::eVertical);
			
			if (animations[x]->keyframes.size() > 0)
			{
				lCamera->Position.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));
				lCamera->InterestPosition.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));

				if (animations[x]->keyframes[0]->fieldOfView.contains)
				{
					lCamera->FieldOfView.Set(animations[x]->keyframes[0]->fieldOfView.floatValue);
				}
			}

			cameraNode = FbxNode::Create(pScene, animations[x]->name);

			if (animations[x]->keyframes.size() > 0)
			{
				cameraNode->LclTranslation.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));
				cameraNode->LclRotation.Set(FbxDouble3(animations[x]->keyframes[0]->rotation.x, animations[x]->keyframes[0]->rotation.y, animations[x]->keyframes[0]->rotation.z));


				cameraNode->SetRotationActive(true);
				cameraNode->SetPostRotation(FbxNode::eSourcePivot, FbxVector4(0, -90, 0));
			}


			cameraNode->SetNodeAttribute(lCamera);
			lMainNode->AddChild(cameraNode);

			pScene->GetGlobalSettings().SetDefaultCamera(PRODUCER_PERSPECTIVE);
		}*/

		if (!(animations[x]->isCamera))
		{
			int countChannels = 0;

			// Add TopJoint
			animationScene->mNumChannels = joints.size() + 1;
			animationScene->mChannels = new aiNodeAnim*[animationScene->mNumChannels];
			animationScene->mDuration = (animations[x]->keyframes.size() / 30.0);
			animationScene->mTicksPerSecond = 1.0f;

			aiNodeAnim* aiNodeAnimTopJoint = new aiNodeAnim();
			// TopJointFirst
			animationScene->mChannels[countChannels] = aiNodeAnimTopJoint;

			aiNodeAnimTopJoint->mNodeName = "TopJoint";
			
			aiNodeAnimTopJoint->mNumPositionKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mPositionKeys = new aiVectorKey[aiNodeAnimTopJoint->mNumPositionKeys];

			aiNodeAnimTopJoint->mNumRotationKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mRotationKeys = new aiQuatKey[aiNodeAnimTopJoint->mNumPositionKeys];

			aiNodeAnimTopJoint->mNumScalingKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mScalingKeys = new aiVectorKey[aiNodeAnimTopJoint->mNumPositionKeys];

			for (int y = 0; y < animations[x]->keyframes.size(); y++)
			{
				aiNodeAnimTopJoint->mPositionKeys[y].mTime = (y / 30.0);
				aiNodeAnimTopJoint->mPositionKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->translation.x, animations[x]->keyframes[y]->translation.y, animations[x]->keyframes[y]->translation.z);

				aiNodeAnimTopJoint->mRotationKeys[y].mTime = (y / 30.0);
				Quat quat = Quat::FromEulerZYX(DegToRad(animations[x]->keyframes[y]->rotation.z), DegToRad(animations[x]->keyframes[y]->rotation.y), DegToRad(animations[x]->keyframes[y]->rotation.x));
				aiNodeAnimTopJoint->mRotationKeys[y].mValue = aiQuaternion(quat.w, quat.x, quat.y, quat.z);

				aiNodeAnimTopJoint->mScalingKeys[y].mTime = (y / 30.0);
				aiNodeAnimTopJoint->mScalingKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->scale.x, animations[x]->keyframes[y]->scale.y, animations[x]->keyframes[y]->scale.z);
			}

			countChannels++;

			for (int z = 0; z < joints.size(); z++)
			{
				aiNodeAnim* aiNodeAnimJoint = new aiNodeAnim();
				animationScene->mChannels[countChannels] = aiNodeAnimJoint;

				aiNodeAnimJoint->mNodeName = joints[z]->name.GetBuffer(0);
				joints[z]->name.ReleaseBuffer();

				countChannels++;

				aiNodeAnimJoint->mNumPositionKeys = animations[x]->keyframes.size();
				aiNodeAnimJoint->mPositionKeys = new aiVectorKey[aiNodeAnimJoint->mNumPositionKeys];

				aiNodeAnimJoint->mNumRotationKeys = animations[x]->keyframes.size();
				aiNodeAnimJoint->mRotationKeys = new aiQuatKey[aiNodeAnimJoint->mNumPositionKeys];

				aiNodeAnimJoint->mNumScalingKeys = animations[x]->keyframes.size();
				aiNodeAnimJoint->mScalingKeys = new aiVectorKey[aiNodeAnimJoint->mNumPositionKeys];

				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					bool foundRotationKey = false;
					bool foundScaleKey = false;
					bool foundTranslationKey = false;

					aiNodeAnimJoint->mPositionKeys[y].mTime = (y / 30.0);
					aiNodeAnimJoint->mRotationKeys[y].mTime = (y / 30.0);
					aiNodeAnimJoint->mScalingKeys[y].mTime = (y / 30.0);

					for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
					{
						if (animations[x]->keyframes[y]->animationParts[w]->name == joints[z]->name)
						{
							aiNodeAnimJoint->mPositionKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->animationParts[w]->translation.x, animations[x]->keyframes[y]->animationParts[w]->translation.y, animations[x]->keyframes[y]->animationParts[w]->translation.z);

							Quat quat = Quat::FromEulerZYX(DegToRad(animations[x]->keyframes[y]->animationParts[w]->rotation.z), DegToRad(animations[x]->keyframes[y]->animationParts[w]->rotation.y), DegToRad(animations[x]->keyframes[y]->animationParts[w]->rotation.x));
							aiNodeAnimJoint->mRotationKeys[y].mValue = aiQuaternion(quat.w, quat.x, quat.y, quat.z);

							aiNodeAnimJoint->mScalingKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->animationParts[w]->scale.x, animations[x]->keyframes[y]->animationParts[w]->scale.y, animations[x]->keyframes[y]->animationParts[w]->scale.z);

							foundRotationKey = true;
							foundScaleKey = true;
							foundTranslationKey = true;
							break;
						}
					}

					if (!foundRotationKey)
					{
						aiNodeAnimJoint->mRotationKeys[y].mValue = aiQuaternion(1, 0, 0, 0);
						foundRotationKey = true;
					}

					if (!foundScaleKey)
					{
						aiNodeAnimJoint->mScalingKeys[y].mValue = aiVector3D(1, 1, 1);
						foundScaleKey = true;
					}

					if (!foundTranslationKey)
					{
						aiNodeAnimJoint->mPositionKeys[y].mValue = aiVector3D(0, 0, 0);
						foundTranslationKey = true;
					}
				}
			}
		}
		else
		{
			// Add TopJoint
			animationScene->mNumChannels = 1;
			animationScene->mChannels = new aiNodeAnim*[animationScene->mNumChannels];
			animationScene->mDuration = (animations[x]->keyframes.size() / 30.0);
			animationScene->mTicksPerSecond = 1.0f;

			aiNodeAnim* aiNodeAnimTopJoint = new aiNodeAnim();
			// TopJointFirst
			animationScene->mChannels[0] = aiNodeAnimTopJoint;

			aiNodeAnimTopJoint->mNodeName = "Camera";
			
			aiNodeAnimTopJoint->mNumPositionKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mPositionKeys = new aiVectorKey[aiNodeAnimTopJoint->mNumPositionKeys];

			aiNodeAnimTopJoint->mNumRotationKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mRotationKeys = new aiQuatKey[aiNodeAnimTopJoint->mNumPositionKeys];

			aiNodeAnimTopJoint->mNumScalingKeys = animations[x]->keyframes.size();
			aiNodeAnimTopJoint->mScalingKeys = new aiVectorKey[aiNodeAnimTopJoint->mNumPositionKeys];

			for (int y = 0; y < animations[x]->keyframes.size(); y++)
			{
				aiNodeAnimTopJoint->mPositionKeys[y].mTime = (y / 30.0);
				aiNodeAnimTopJoint->mPositionKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->translation.x, animations[x]->keyframes[y]->translation.y, animations[x]->keyframes[y]->translation.z);

				aiNodeAnimTopJoint->mRotationKeys[y].mTime = (y / 30.0);
				Quat quat = Quat::FromEulerZYX(DegToRad(animations[x]->keyframes[y]->rotation.z), DegToRad(animations[x]->keyframes[y]->rotation.y), DegToRad(animations[x]->keyframes[y]->rotation.x));
				aiNodeAnimTopJoint->mRotationKeys[y].mValue = aiQuaternion(quat.w, quat.x, quat.y, quat.z);

				aiNodeAnimTopJoint->mScalingKeys[y].mTime = (y / 30.0);
				aiNodeAnimTopJoint->mScalingKeys[y].mValue = aiVector3D(animations[x]->keyframes[y]->scale.x, animations[x]->keyframes[y]->scale.y, animations[x]->keyframes[y]->scale.z);
			}
		}
	}

	exporter.Export(&scene, exportFormatDesc->id, outputFile, 0);
}

void CObjToAn8Dlg::WriteObjFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter, bool sortByTextureWithinGroup, bool outputObjVColors)
{
	CString lastGroup = "NOT A REAL GROUP";
	FILE* outFile = fopen(outputFile, "w");
	if (outFile != NULL)
	{

	}
	else
	{
		if (doMessageBoxes)
			MessageBox("Can't open output file " + outputFile);
		return;
	}
	CString mtlFilename = outputFile.Mid(0, outputFile.GetLength()-3) + "mtl";
	FILE* outFileMtl = fopen(mtlFilename, "w");
	if (outFileMtl != NULL)
	{
		std::vector<CMaterialFile*>::iterator	itermaterialFiles;
		for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
		{
			CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

			std::vector<CMaterial*>::iterator	itermaterial;
			for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
			{
				CMaterial* material = (CMaterial*)*itermaterial;

				fprintf(outFileMtl, "newmtl %s\n", material->name);
				
				if (material->ambientColor.contains)
				{
					fprintf(outFileMtl, "Ka %f %f %f\n", material->ambientColor.r, material->ambientColor.g, material->ambientColor.b);
				}

				if ((material->diffuseColor.contains) || (material->diffuseTexture.contains))
				{
					fprintf(outFileMtl, "Kd %f %f %f\n", material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b);
				}

				if (material->spectralColor.contains)
				{
					fprintf(outFileMtl, "Ks %f %f %f\n", material->spectralColor.r, material->spectralColor.g, material->spectralColor.b);
				}

				if (material->alpha.contains)
				{

					fprintf(outFileMtl, "d %f\n", material->alpha.floatValue);
				}
				if ((!ignoreShading) || (ignoreShadingPoint7 && (material->diffuseColor.contains && material->diffuseColor.r == 1.0) && (material->diffuseColor.g == 1.0) && (material->diffuseColor.b == 0.7)))
				{
					fprintf(outFileMtl, "Ns 752.6\n");
				}
				if (material->illum.contains)
				{
					fprintf(outFileMtl, "illum %d\n", (int)material->illum.floatValue);
				}
				if (material->diffuseTexture.contains)
				{
					fprintf(outFileMtl, "map_Kd %s\n", material->diffuseTexture.stringValue);
				}
				fprintf(outFileMtl, "\n");
			}
		}

		fprintf(outFile, "mtllib %s\n", PullOutImageName(outputFile) + ".mtl");

		JointType jointType = Absolute;
		for (int x = 0; x < joints.size(); x++)
		{
			if (joints[x]->jointType == Relative)
			{
				jointType = Relative;
				fprintf(outFile, "#jointmode relative\n");
				break;
			}
		}

		std::sort(joints.begin(), joints.end(), less_than_CJoint());

		for (int x = 0; x < joints.size(); x++)
		{
			fprintf(outFile, "#joint %s\n", joints[x]->name);
			if (jointType == Absolute)
				fprintf(outFile, "#jointposition %f %f %f\n", joints[x]->positionAbsolute.x, joints[x]->positionAbsolute.y, joints[x]->positionAbsolute.z);
			else if (jointType == Relative)
				fprintf(outFile, "#jointsrt %f %f %f %f %f %f %f %f %f\n", joints[x]->scaleRelative.x, joints[x]->scaleRelative.y, joints[x]->scaleRelative.z, joints[x]->rotationRelative.x, joints[x]->rotationRelative.y, joints[x]->rotationRelative.z, joints[x]->positionRelative.x, joints[x]->positionRelative.y, joints[x]->positionRelative.z);
			if (joints[x]->rotationOrder != "XYZ")
				fprintf(outFile, "#jointrotationorder %s\n", joints[x]->rotationOrder);
		}

		for (int x = 0; x < joints.size(); x++)
		{
			for (int y = 0; y < joints[x]->children.size(); y++)
			{
				fprintf(outFile, "#connection %s %s\n", joints[x]->name, joints[x]->children[y]->name);
			}
		}
		
		std::vector<CVertice*>::iterator	iterPointList;
		int vertexCounter = 0;
		for (iterPointList = vertices.begin(); iterPointList != vertices.end(); iterPointList++)
		{
			CVertice* vertice = (CVertice*)*iterPointList;

			if (vertice->hasVertexColor && outputObjVColors)
			{
				fprintf(outFile, "v %f %f %f %f %f %f\n", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z, vertice->vertexColor.x, vertice->vertexColor.y, vertice->vertexColor.z);
			}
			else
			{
				if (outputObjVColors)
				{
					bool wroteColor = false;
					std::vector<CGroup*>::iterator	itergroups;
					for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
					{
						CGroup* group = ((CGroup*)(*itergroups));

						if (group->polygons.size() == 0)
							continue;

						std::vector<CPolygon*>::iterator	iterpolygon;
						for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
						{
							CPolygon* polygon = ((CPolygon*)(*iterpolygon));

							int numPolyPointsExpected = polygon->polygonPoints.size();

							if (foundVerticeColors)
							{
								std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
								for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
								{
									CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);

									if ((polygonPoint->verticeColorIndex != -1) && (polygonPoint->verticeIndex == vertexCounter))
									{
										if (!wroteColor)
										{
											fprintf(outFile, "v %f %f %f %f %f %f\n", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z, verticeColors[polygonPoint->verticeColorIndex]->color.r / 255.0, verticeColors[polygonPoint->verticeColorIndex]->color.g / 255.0, verticeColors[polygonPoint->verticeColorIndex]->color.b / 255.0);
											wroteColor = true;
										}
									}
								}
							}
						}
					}

					if (!wroteColor)
					{
						fprintf(outFile, "v %f %f %f\n", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);
					}
				}
				else
				{
					fprintf(outFile, "v %f %f %f\n", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);
				}
			}
			vertexCounter++;
		}

		for (int x = 0; x < joints.size(); x++)
		{
			if (joints[x]->controlPoints.size() > 0)
			{
				fprintf(outFile, "#joint %s\n", joints[x]->name);
				fprintf(outFile, "#vjoint");
				for (int y = 0; y < joints[x]->controlPoints.size(); y++)
				{
					fprintf(outFile, " %d", (joints[x]->controlPoints[y] + 1));
				}
				fprintf(outFile, "\n");
			}
		}

		std::vector<CUVCoordinate*>::iterator	iterUVList;
		for (iterUVList = uvCoordinates.begin(); iterUVList != uvCoordinates.end(); iterUVList++)
		{
			CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;

			fprintf(outFile, "vt %f %f\n", uv->u, uv->v);
		}

		std::vector<CNormal*>::iterator	iterNormals;
		for (iterNormals = normals.begin(); iterNormals != normals.end(); iterNormals++)
		{
			CNormal* normal = (CNormal*)*iterNormals;

			fprintf(outFile, "vn %f %f %f\n", normal->nx, normal->ny, normal->nz);
		}

		if (!outputObjVColors)
		{
			std::vector<CVerticeColor*>::iterator	iterVerticeColors;
			for (iterVerticeColors = verticeColors.begin(); iterVerticeColors != verticeColors.end(); iterVerticeColors++)
			{
				CVerticeColor* verticeColor = (CVerticeColor*)*iterVerticeColors;
				if (verticeColor->contains)
				{
					fprintf(outFile, "#vcolor %f %f %f %f\n", verticeColor->color.r, verticeColor->color.g, verticeColor->color.b, verticeColor->color.a);
				}
				else
				{
					fprintf(outFile, "#vcolor null\n");
				}
			}
		}

		CString prevPrimarySecondary = "";

		std::vector<CGroup*>::iterator	itergroups;
		for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;
			
			if ((!noGroups) && (!primarySecondaryGroups))
			{
				if (regexFilterCheck)
				{
					std::tr1::regex rx(regexFilter.GetBuffer());
					std::string stdString = std::string(group->name.GetBuffer());
					regexFilter.ReleaseBuffer();

					bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
					group->name.ReleaseBuffer();

					if (!isMatch)
					{
						continue;
					}
				}

				if (mergeHierarchicalGroups)
				{
					CString tempGroupName =  group->name;
					tempGroupName.Replace(" Model", "");

					if (tempGroupName.ReverseFind(' ') != -1)
					{
						tempGroupName = tempGroupName.Mid(tempGroupName.ReverseFind(' ') + 1);
					}

					if (tempGroupName != lastGroup)
					{
						fprintf(outFile, "g %s\n", tempGroupName);

						lastGroup = tempGroupName;
					}
				}
				else
				{
					fprintf(outFile, "g %s\n", group->name);
				}
			}


			if (sortByTextureWithinGroup)
			{
				SortPolygonGroupByTexture(group);
			}

			bool initialMtl = false;
			CString prevMtlName = "";

			std::vector<CPolygon*>::iterator	iterpolygon;
			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));


				if (primarySecondaryGroups)
				{
					CString currentPrimarySecondary = "primary";
					if ((polygon->materialName.Find("Transparent") != -1) || (polygon->materialName.Find("TopFlag") != -1) || (polygon->materialName.Find("Decal") != -1))
					{
						currentPrimarySecondary = "secondary";
					}

					std::vector<CMaterialFile*>::iterator	itermaterialFiles;
					for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
					{
						CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

						std::vector<CMaterial*>::iterator	itermaterial;
						for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
						{
							CMaterial* material = (CMaterial*)*itermaterial;

							if (polygon->materialName == material->name)
							{
								if (material->diffuseTexture.contains)
								{
									if ((material->diffuseTexture.stringValue.Find("Transparent") != -1) || (material->diffuseTexture.stringValue.Find("TopFlag") != -1) || (material->diffuseTexture.stringValue.Find("Decal") != -1))
									{
										currentPrimarySecondary = "secondary";
									}
								}
							}
						}
					}

					if (currentPrimarySecondary != prevPrimarySecondary)
					{
						fprintf(outFile, "g %s\n", currentPrimarySecondary);

						prevPrimarySecondary = currentPrimarySecondary;
					}
				}

				if ((!initialMtl) || (prevMtlName != polygon->materialName))
				{
					fprintf(outFile, "usemtl %s\n", polygon->materialName);

					initialMtl = true;
					prevMtlName = polygon->materialName;
				}


				int numPolyPointsExpected = polygon->polygonPoints.size();

				fprintf(outFile, "f ");
				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);

					if (foundTextureUV && foundNormals && (polygonPoint->uvIndex != -1) && (polygonPoint->normalIndex != -1))
					{
						fprintf(outFile, "%d/%d/%d ", polygonPoint->verticeIndex+1, polygonPoint->uvIndex+1, polygonPoint->normalIndex+1);
					}
					else if (foundTextureUV && (polygonPoint->uvIndex != -1))
					{
						fprintf(outFile, "%d/%d ", polygonPoint->verticeIndex+1, polygonPoint->uvIndex+1);
					}
					else if (foundNormals && (polygonPoint->normalIndex != -1))
					{
						fprintf(outFile, "%d//%d ", polygonPoint->verticeIndex+1, polygonPoint->normalIndex+1);
					}
					else
					{
						fprintf(outFile, "%d ", polygonPoint->verticeIndex+1);
					}
				}
				fprintf(outFile, "\n");

				if (foundVerticeColors && !outputObjVColors)
				{
					bool wroteOnce = false;
					
					for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
					{
						CPolygonPoint* polygonPoint = ((CPolygonPoint*)*iterpolygonpoint);

						 if (polygonPoint->verticeColorIndex != -1)
						 {
							 if (!wroteOnce)
							 {
								fprintf(outFile, "#fvcolorindex ");
								wroteOnce = true;
							 }
							 if (polygonPoint->verticeColorIndex == -1)
								fprintf(outFile, "(null) ");
							 else
								fprintf(outFile, "%d ", polygonPoint->verticeColorIndex+1);
						 }
					}
					fprintf(outFile, "\n");
				}
			}
		}

		for (int x = 0; x < animations.size(); x++)
		{
			CAnimation* animation = animations[x];
			if (animation->isCamera)
			{
				fprintf(outFile, "#camera %s\n", animation->name);
			}
			else if (animation->treatAsBlendShape)
			{
				fprintf(outFile, "#shapeanimation %s\n", animation->name);
			}
			else
			{
				fprintf(outFile, "#animation %s\n", animation->name);
			}
			for (int y = 0; y < animation->keyframes.size(); y++)
			{
				CKeyframe* keyframe = animation->keyframes[y];
				fprintf(outFile, "#keyframe %d\n", animation->keyframes[y]->number);

				if ((fabs(keyframe->rotation.x) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.x) < EPSILONVALUES))
					keyframe->rotation.x = 0;
				if ((fabs(keyframe->rotation.y) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.y) < EPSILONVALUES))
					keyframe->rotation.y = 0;
				if ((fabs(keyframe->rotation.z) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.z) < EPSILONVALUES))
					keyframe->rotation.z = 0;

				while (keyframe->rotation.x >= 360.0f)
					keyframe->rotation.x -= 360.0f;
				while (keyframe->rotation.x < 0.0f)
					keyframe->rotation.x += 360.0f;

				while (keyframe->rotation.y >= 360.0f)
					keyframe->rotation.y -= 360.0f;
				while (keyframe->rotation.y < 0.0f)
					keyframe->rotation.y += 360.0f;

				while (keyframe->rotation.z >= 360.0f)
					keyframe->rotation.z -= 360.0f;
				while (keyframe->rotation.z < 0.0f)
					keyframe->rotation.z += 360.0f;

				if ((fabs(keyframe->rotation.x) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.x) < EPSILONVALUES))
					keyframe->rotation.x = 0;
				if ((fabs(keyframe->rotation.y) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.y) < EPSILONVALUES))
					keyframe->rotation.y = 0;
				if ((fabs(keyframe->rotation.z) < EPSILONVALUES) || (fabs(360 - keyframe->rotation.z) < EPSILONVALUES))
					keyframe->rotation.z = 0;

				fprintf(outFile, "#keyframetranslation %f %f %f\n", keyframe->translation.x, keyframe->translation.y, keyframe->translation.z);
				if ((keyframe->translationIsInterpolated[0]) || (keyframe->translationIsInterpolated[1]) || (keyframe->translationIsInterpolated[2]))
					fprintf(outFile, "#keyframeisinterpolatedtranslation %d %d %d\n", keyframe->translationIsInterpolated[0], keyframe->translationIsInterpolated[1], keyframe->translationIsInterpolated[2]);
				fprintf(outFile, "#keyframerotation %f %f %f\n", keyframe->rotation.x, keyframe->rotation.y, keyframe->rotation.z);
				if ((keyframe->rotationIsInterpolated[0]) || (keyframe->rotationIsInterpolated[1]) || (keyframe->rotationIsInterpolated[2]))
					fprintf(outFile, "#keyframeisinterpolatedrotation %d %d %d\n", keyframe->rotationIsInterpolated[0], keyframe->rotationIsInterpolated[1], keyframe->rotationIsInterpolated[2]);
				fprintf(outFile, "#keyframescale %f %f %f\n", keyframe->scale.x, keyframe->scale.y, keyframe->scale.z);
				if ((keyframe->scaleIsInterpolated[0]) || (keyframe->scaleIsInterpolated[1]) || (keyframe->scaleIsInterpolated[2]))
					fprintf(outFile, "#keyframeisinterpolatedscale %d %d %d\n", keyframe->scaleIsInterpolated[0], keyframe->scaleIsInterpolated[1], keyframe->scaleIsInterpolated[2]);

				/*if ((keyframe->rotationHasDerivative[0]) || (keyframe->rotationHasDerivative[1]) || (keyframe->rotationHasDerivative[2]))
					fprintf(outFile, "#keyframederivativerotation %f %f %f\n", keyframe->rotationDerivativeValue.x, keyframe->rotationDerivativeValue.y, keyframe->rotationDerivativeValue.z);
				if ((keyframe->translationHasDerivative[0]) || (keyframe->translationHasDerivative[1]) || (keyframe->translationHasDerivative[2]))
					fprintf(outFile, "#keyframederivativetranslation %f %f %f\n", keyframe->translationDerivativeValue.x, keyframe->translationDerivativeValue.y, keyframe->translationDerivativeValue.z);
				if ((keyframe->scaleHasDerivative[0]) || (keyframe->scaleHasDerivative[1]) || (keyframe->scaleHasDerivative[2]))
					fprintf(outFile, "#keyframederivativescale %f %f %f\n", keyframe->scaleDerivativeValue.x, keyframe->scaleDerivativeValue.y, keyframe->scaleDerivativeValue.z);*/

				if (keyframe->fieldOfView.contains)
				{
					fprintf(outFile, "#keyframefieldofview %f\n", keyframe->fieldOfView.floatValue);
				}

				if (keyframe->envColor.contains)
				{
					fprintf(outFile, "#keyframecolor %f %f %f %f\n", keyframe->envColor.r, keyframe->envColor.g, keyframe->envColor.b, keyframe->envColor.a);
				}
				
				std::sort(keyframe->animationParts.begin(), keyframe->animationParts.end(), less_than_CAnimationPart());
				
				for (int z = 0; z < keyframe->animationParts.size(); z++)
				{
					CAnimationPart* animationPart = keyframe->animationParts[z];
					fprintf(outFile, "#part %s\n", animationPart->name);
					
					if (animationPart->shapeVertices.size() == 0)
					{
						//if ((fabs(animationPart->translation.x) > EPSILONVALUES) || (fabs(animationPart->translation.y) > EPSILONVALUES) || (fabs(animationPart->translation.z) > EPSILONVALUES))
						fprintf(outFile, "#parttranslation %f %f %f\n", animationPart->translation.x, animationPart->translation.y, animationPart->translation.z);
						if ((animationPart->translationIsInterpolated[0]) || (animationPart->translationIsInterpolated[1]) || (animationPart->translationIsInterpolated[2]))
							fprintf(outFile, "#partisinterpolatedtranslation %d %d %d\n", animationPart->translationIsInterpolated[0], animationPart->translationIsInterpolated[1], animationPart->translationIsInterpolated[2]);

						if ((fabs(animationPart->rotation.x) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.x) < EPSILONVALUES))
							animationPart->rotation.x = 0;
						if ((fabs(animationPart->rotation.y) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.y) < EPSILONVALUES))
							animationPart->rotation.y = 0;
						if ((fabs(animationPart->rotation.z) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.z) < EPSILONVALUES))
							animationPart->rotation.z = 0;

						while (animationPart->rotation.x >= 360.0f)
							animationPart->rotation.x -= 360.0f;
						while (animationPart->rotation.x < 0.0f)
							animationPart->rotation.x += 360.0f;

						while (animationPart->rotation.y >= 360.0f)
							animationPart->rotation.y -= 360.0f;
						while (animationPart->rotation.y < 0.0f)
							animationPart->rotation.y += 360.0f;

						while (animationPart->rotation.z >= 360.0f)
							animationPart->rotation.z -= 360.0f;
						while (animationPart->rotation.z < 0.0f)
							animationPart->rotation.z += 360.0f;

						if ((fabs(animationPart->rotation.x) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.x) < EPSILONVALUES))
							animationPart->rotation.x = 0;
						if ((fabs(animationPart->rotation.y) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.y) < EPSILONVALUES))
							animationPart->rotation.y = 0;
						if ((fabs(animationPart->rotation.z) < EPSILONVALUES) || (fabs(360 - animationPart->rotation.z) < EPSILONVALUES))
							animationPart->rotation.z = 0;

						fprintf(outFile, "#partrotation %f %f %f\n", animationPart->rotation.x, animationPart->rotation.y, animationPart->rotation.z);
						if ((animationPart->rotationIsInterpolated[0]) || (animationPart->rotationIsInterpolated[1]) || (animationPart->rotationIsInterpolated[2]))
							fprintf(outFile, "#partisinterpolatedrotation %d %d %d\n", animationPart->rotationIsInterpolated[0], animationPart->rotationIsInterpolated[1], animationPart->rotationIsInterpolated[2]);

						fprintf(outFile, "#partscale %f %f %f\n", animationPart->scale.x, animationPart->scale.y, animationPart->scale.z);
						if ((animationPart->scaleIsInterpolated[0]) || (animationPart->scaleIsInterpolated[1]) || (animationPart->scaleIsInterpolated[2]))
							fprintf(outFile, "#partisinterpolatedscale %d %d %d\n", animationPart->scaleIsInterpolated[0], animationPart->scaleIsInterpolated[1], animationPart->scaleIsInterpolated[2]);

						/*if ((animationPart->rotationHasDerivative[0]) || (animationPart->rotationHasDerivative[1]) || (animationPart->rotationHasDerivative[2]))
							fprintf(outFile, "#partderivativerotation %f %f %f\n", animationPart->rotationDerivativeValue.x, animationPart->rotationDerivativeValue.y, animationPart->rotationDerivativeValue.z);
							if ((animationPart->translationHasDerivative[0]) || (animationPart->translationHasDerivative[1]) || (animationPart->translationHasDerivative[2]))
							fprintf(outFile, "#partderivativetranslation %f %f %f\n", animationPart->translationDerivativeValue.x, animationPart->translationDerivativeValue.y, animationPart->translationDerivativeValue.z);
							if ((animationPart->scaleHasDerivative[0]) || (animationPart->scaleHasDerivative[1]) || (animationPart->scaleHasDerivative[2]))
							fprintf(outFile, "#partderivativescale %f %f %f\n", animationPart->scaleDerivativeValue.x, animationPart->scaleDerivativeValue.y, animationPart->scaleDerivativeValue.z);*/
					}
					else
					{
						for (int p = 0; p < animationPart->shapeVertices.size(); p++)
						{
							fprintf(outFile, "#partshapetranslation %d %f %f %f\n", animationPart->shapeVertices[p].vertexIndex, animationPart->shapeVertices[p].translation.x, animationPart->shapeVertices[p].translation.y, animationPart->shapeVertices[p].translation.z);
						}
					}
				}
			}
		}

		fclose(outFileMtl);
		fclose(outFile);
	}
	else
	{
		fclose(outFile);
		if (doMessageBoxes)
			MessageBox("Can't open output mtl file " + outputFile);
		return;
	}
}

CString CObjToAn8Dlg::GetBvhRotationOrderChannelString(CString rotationOrder)
{
	if (rotationOrder == "XZY")
		return "Xposition Yposition Zposition Xrotation Zrotation Yrotation";
	else if (rotationOrder == "YZX")
		return "Xposition Yposition Zposition Yrotation Zrotation Xrotation";
	else if (rotationOrder == "YXZ")
		return "Xposition Yposition Zposition Yrotation Xrotation Zrotation";
	else if (rotationOrder == "ZXY")
		return "Xposition Yposition Zposition Zrotation Xrotation Yrotation";
	else if (rotationOrder == "ZYX")
		return "Xposition Yposition Zposition Xrotation Yrotation Zrotation";
	else // XYZ
		return "Xposition Yposition Zposition Zrotation Yrotation Xrotation";
}

void CObjToAn8Dlg::WriteBvhSkeleton(FILE* outFile, CJoint* joint, int indent)
{
	for (int x = 0; x < indent; x++)
		fprintf(outFile, "	");

	fprintf(outFile, "JOINT %s\n", joint->name);

	for (int x = 0; x < indent; x++)
		fprintf(outFile, "	");

	fprintf(outFile, "{\n");

	for (int x = 0; x < indent+1; x++)
		fprintf(outFile, "	");
	
	if (joint->parent != NULL)
	{
		fprintf(outFile, "OFFSET %f %f %f\n", (joint->positionAbsolute.x - joint->parent->positionAbsolute.x), (joint->positionAbsolute.y - joint->parent->positionAbsolute.y), (joint->positionAbsolute.z - joint->parent->positionAbsolute.z));
	}
	else
	{
		fprintf(outFile, "OFFSET %f %f %f\n", joint->positionAbsolute.x, joint->positionAbsolute.y, joint->positionAbsolute.z);
	}
	
	for (int x = 0; x < indent+1; x++)
		fprintf(outFile, "	");

	fprintf(outFile, "CHANNELS 6 %s\n", GetBvhRotationOrderChannelString(joint->rotationOrder));

	if (joint->children.size() == 0)
	{
		for (int x = 0; x < indent+1; x++)
			fprintf(outFile, "	");
		fprintf(outFile, "End Site\n");

		for (int x = 0; x < indent+1; x++)
			fprintf(outFile, "	");
		fprintf(outFile, "{\n");

		for (int x = 0; x < indent+2; x++)
			fprintf(outFile, "	");
		fprintf(outFile, "OFFSET 0 0 0\n");
		for (int x = 0; x < indent+1; x++)
			fprintf(outFile, "	");
		fprintf(outFile, "}\n");
	}
	else
	{
		for (int x = 0; x < joint->children.size(); x++)
		{
			WriteBvhSkeleton(outFile, joint->children[x], indent + 1);
		}
	}

	for (int x = 0; x < indent; x++)
		fprintf(outFile, "	");

	fprintf(outFile, "}\n");
}

void CObjToAn8Dlg::WriteBvhMotionKeyframe(FILE* outFile, CKeyframe* keyframe, CJoint* joint)
{
	bool foundPart = false;
	for (int x = 0; x < keyframe->animationParts.size(); x++)
	{
		if (keyframe->animationParts[x]->name == joint->name)
		{
			if (joint->rotationOrder == "XZY")
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y));
			else if (joint->rotationOrder == "YZX")
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x));
			else if (joint->rotationOrder == "YXZ")
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y));
			else if (joint->rotationOrder == "ZXY")
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z));
			else if (joint->rotationOrder == "ZYX")
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z));
			else // XYZ
				fprintf(outFile, " %s %s %s %s %s %s", GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.x), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.y), GetTranslationToEpsilonString(keyframe->animationParts[x]->translation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->animationParts[x]->rotation.x));
			foundPart = true;
			break;
		}
	}

	if (!foundPart)
	{
		fprintf(outFile, " 0 0 0 0 0 0");
	}

	for (int x = 0; x < joint->children.size(); x++)
	{
		WriteBvhMotionKeyframe(outFile, keyframe, joint->children[x]);
	}
}

CString CObjToAn8Dlg::GetRotationAndTruncateToEpsilonString(float rotation)
{
	if ((fabs(rotation) < EPSILONVALUES) || (fabs(360 - rotation) < EPSILONVALUES))
	{
		return "0";
	}
	else
	{
		CString tempStr;
		tempStr.Format("%f", rotation);
		return tempStr;
	}
}

CString CObjToAn8Dlg::GetTranslationToEpsilonString(float translation)
{
	if (fabs(translation) < EPSILONVALUES)
	{
		return "0";
	}
	else
	{
		CString tempStr;
		tempStr.Format("%f", translation);
		return tempStr;
	}
}

void CObjToAn8Dlg::WriteBvhFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter)
{
	FILE* outFile = fopen(outputFile, "w");
	if (outFile != NULL)
	{

	}
	else
	{
		if (doMessageBoxes)
			MessageBox("Can't open output file " + outputFile);
		return;
	}

	fprintf(outFile, "HIERARCHY\n");

	for (int x = 0; x < animations.size(); x++)
	{
		CAnimation* animation = animations[x];
	}

	std::vector<CJoint*> rootJoints = FindRootJoints(joints);

	if (rootJoints.size() == 0)
	{
		// add one instead
		fclose(outFile);
		if (doMessageBoxes)
			MessageBox("Error, no root nodes");
		return;
	}
	else if (rootJoints.size() == 1)
	{
		fprintf(outFile, "ROOT TopJoint\n");
		fprintf(outFile, "{\n");

		// just one, if not present, add top
		CJoint* rootJoint = rootJoints[0];
		if (rootJoint->name != "TopJoint")
		{
			fprintf(outFile, "	OFFSET 0.000000 0.000000 0.000000\n");
			fprintf(outFile, "	CHANNELS 6 %s\n", GetBvhRotationOrderChannelString(rootJoint->rotationOrder));

			WriteBvhSkeleton(outFile, rootJoint, 1);
		}
		else
		{
			fprintf(outFile, "	OFFSET %f %f %f\n", rootJoint->positionAbsolute.x, rootJoint->positionAbsolute.y, rootJoint->positionAbsolute.z);
			fprintf(outFile, "	CHANNELS 6 %s\n", GetBvhRotationOrderChannelString(rootJoint->rotationOrder));

			for (int y = 0; y < rootJoint->children.size(); y++)
			{
				WriteBvhSkeleton(outFile, rootJoint->children[y], 1);
			}
		}

		fprintf(outFile, "}\n");
	}
	else
	{
		// multiple, need to write top level one
		fprintf(outFile, "ROOT TopJoint\n");
		fprintf(outFile, "{\n");
		fprintf(outFile, "	OFFSET 0.000000 0.000000 0.000000\n");
		fprintf(outFile, "	CHANNELS 6 %s\n", GetBvhRotationOrderChannelString("XYZ"));

		for (int x = 0; x < rootJoints.size(); x++)
		{
			CJoint* rootJoint = rootJoints[x];

			WriteBvhSkeleton(outFile, rootJoint, 1);
		}

		fprintf(outFile, "}\n");
	}

	for (int x = 0; x < animations.size(); x++)
	{
		CAnimation* animation = animations[x];
		if (!animation->isCamera)
		{	
			fprintf(outFile, "MOTION\n");
			fprintf(outFile, "Frames: %d\n", (int)animation->keyframes.size());
			fprintf(outFile, "Frame Time: %f\n", 1.0 / 30.0);

			for (int y = 0; y < animation->keyframes.size(); y++)
			{
				CKeyframe* keyframe = animation->keyframes[y];
				if (rootJoints.size() == 1)
				{
					// just one, if not present, add top
					CJoint* rootJoint = rootJoints[0];
					if (rootJoint->name != "TopJoint")
					{
						fprintf(outFile, "%s %s %s", GetTranslationToEpsilonString(keyframe->translation.x), GetTranslationToEpsilonString(keyframe->translation.y), GetTranslationToEpsilonString(keyframe->translation.z));
						fprintf(outFile, " %s %s %s ", GetRotationAndTruncateToEpsilonString(keyframe->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->rotation.x));

						WriteBvhMotionKeyframe(outFile, keyframe, rootJoint);
					}
					else
					{
						fprintf(outFile, "%s %s %s", GetTranslationToEpsilonString(keyframe->translation.x), GetTranslationToEpsilonString(keyframe->translation.y), GetTranslationToEpsilonString(keyframe->translation.z));
						fprintf(outFile, " %s %s %s ", GetRotationAndTruncateToEpsilonString(keyframe->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->rotation.x));

						for (int y = 0; y < rootJoint->children.size(); y++)
						{
							WriteBvhMotionKeyframe(outFile, keyframe, rootJoint->children[y]);
						}
					}

					fprintf(outFile, "\n");
				}
				else
				{
					fprintf(outFile, "%s %s %s", GetTranslationToEpsilonString(keyframe->translation.x), GetTranslationToEpsilonString(keyframe->translation.y), GetTranslationToEpsilonString(keyframe->translation.z));
					fprintf(outFile, " %s %s %s ", GetRotationAndTruncateToEpsilonString(keyframe->rotation.z), GetRotationAndTruncateToEpsilonString(keyframe->rotation.y), GetRotationAndTruncateToEpsilonString(keyframe->rotation.x));

					// multiple, need to write top level one
					for (int x = 0; x < rootJoints.size(); x++)
					{
						CJoint* rootJoint = rootJoints[x];

						WriteBvhMotionKeyframe(outFile, keyframe, rootJoint);
					}

					fprintf(outFile, "\n");
				}
			}
		}
	}

	fclose(outFile);
}

void CObjToAn8Dlg::WriteAn8BoneDeclaration(CBone* parentBone, std::vector<CBone*>& bones, std::vector<CBone*>& rootBones, std::vector<CJoint*> joints, CJoint* parentJoint)
{
	if (parentJoint != NULL)
	{
		CBone* leafBone = new CBone();
		leafBone->boneStart = parentJoint;
		leafBone->boneEnd = parentJoint;
		leafBone->name = parentJoint->name;
		leafBone->parentBone = parentBone;
		leafBone->length = 0;
		bones.push_back(leafBone);

		parentBone->boneConnections.push_back(leafBone);

		if (parentJoint->children.size() > 0)
		{
			for (int y = 0; y < parentJoint->children.size(); y++)
			{
				CBone* bone = new CBone();
				bone->boneStart = parentJoint;
				bone->boneEnd = parentJoint->children[y];
				bone->name.Format("%s-%s", parentJoint->name, parentJoint->children[y]->name);
				bone->parentBone = leafBone;
				bone->length = 0;
				bones.push_back(bone);

				leafBone->boneConnections.push_back(bone);

				WriteAn8BoneDeclaration(bone, bones, rootBones, joints, parentJoint->children[y]);
			}
		}
	}
	else
	{
		for (int x = 0; x < joints.size(); x++)
		{
			if (joints[x]->parent == NULL)
			{
				CBone* bone = new CBone();
				bone->boneStart = NULL;
				bone->boneEnd = joints[x];
				bone->name.Format("TopJoint-%s", joints[x]->name);
				bone->parentBone = NULL;
				bone->length = 0;
				bones.push_back(bone);

				rootBones.push_back(bone);

				WriteAn8BoneDeclaration(bone, bones, rootBones, joints, joints[x]);
			}
		}
	}
}

void CObjToAn8Dlg::WriteAn8File(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool roundVertices, bool roundVerticesTenths, bool roundVerticesHundredths, bool recenterObjects, bool scaleAmbientFactor, float scaleAmbientFactorValue, bool scaleDiffuseFactor, float scaleDiffuseFactorValue, bool disableAutoshading, bool defaultShading, bool regexFilterCheck, CString regexFilter, bool sortByTextureWithinGroup)
{
	// Write an8
	FILE* outFile = fopen(outputFile, "w");
	if (outFile != NULL)
	{
		fprintf(outFile, "header {\n");
		fprintf(outFile, "  version { \"0.97d beta\" }\n");
		fprintf(outFile, "  build { \"2008.09.21\" }\n");
		fprintf(outFile, "}\n");
		fprintf(outFile, "environment {\n");
		fprintf(outFile, "  grid { 1 10 50 50 }\n");
		fprintf(outFile, "  lighting {\n");
		fprintf(outFile, "    intensity { 1 }\n");
		fprintf(outFile, "    ambientintensity { 0.20000 }\n");
		fprintf(outFile, "  }\n");
		fprintf(outFile, "  framerate { 30 }\n");
		fprintf(outFile, "  limitplayback { }\n");
		fprintf(outFile, "  film {\n");
		fprintf(outFile, "    size { 400 300 }\n");
		fprintf(outFile, "    ratio { 4 3 }\n");
		fprintf(outFile, "  }\n");
		fprintf(outFile, "}\n");

		std::map<CString,CString> flatTextureList;

		std::vector<CMaterialFile*>::iterator	itermaterialFiles;
		for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
		{
			CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

			std::vector<CMaterial*>::iterator	itermaterial;
			for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
			{
				CMaterial* material = (CMaterial*)*itermaterial;

				if (material->diffuseTexture.contains)
				{
					CString smallerFilename = PullOutImageName(material->diffuseTexture.stringValue);			

					if (flatTextureList.count(smallerFilename) == 0)
					{
						flatTextureList[smallerFilename] = material->diffuseTexture.stringValue;
					}
				}
			}
		}

		std::map<CString,CString>::iterator iterFlatTextureMap;
		for (iterFlatTextureMap = flatTextureList.begin(); iterFlatTextureMap != flatTextureList.end(); iterFlatTextureMap++ )
		{
			CString name = (CString)((*iterFlatTextureMap).first);
			CString texture = (CString)((*iterFlatTextureMap).second);
			texture.Replace("\\", "\\\\");

			fprintf(outFile, "texture { \"" + name + "\"\n");
			fprintf(outFile, "  file { \"" + texture + "\" }\n");
			fprintf(outFile, "}\n");
		}

		flatTextureList.clear();


		fprintf(outFile, "object { \"obj\"\n");

		for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
		{
			CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

			std::vector<CMaterial*>::iterator	itermaterial;
			for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
			{
				CMaterial* material = (CMaterial*)*itermaterial;

				fprintf(outFile, "  material { \"" + material->name + "\"\n");
				fprintf(outFile, "    surface {\n");
				if (material->diffuseColor.contains)
				{
					fprintf(outFile, "      /* RGB chunk no longer used */\n");
					fprintf(outFile, "      rgb { %s }\n", PrintHexColorToString(material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b) );
				}

				//if (specialKeywordMode && material->diffuseColor.contains)
					fprintf(outFile, "      lockambiantdiffuse { }\n");

				if (ignoreShading || ignoreShadingPoint7 || (!material->diffuseColor.contains))
				{
					material->ambientColor.contains = true;
					material->ambientColor.r = 1;
					material->ambientColor.g = 1;
					material->ambientColor.b = 1;

					material->diffuseColor.contains = true;
					material->diffuseColor.r = 1;
					material->diffuseColor.g = 1;
					material->diffuseColor.b = 1;
				}

				if (material->ambientColor.contains)
				{
					fprintf(outFile, "      ambiant {\n");
					if (specialKeywordMode && material->diffuseColor.contains)
						fprintf(outFile, "        rgb { %s }\n", PrintHexColorToString(material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b));
					else
						fprintf(outFile, "        rgb { %s }\n", PrintHexColorToString(material->ambientColor.r, material->ambientColor.g, material->ambientColor.b));
					if (specialKeywordMode)
					{
						if ((material->name.Find("EnvMapping") != -1) || ((material->diffuseTexture.contains) && (material->diffuseTexture.stringValue.Find("EnvMapping") != -1)))
						{
							fprintf(outFile, "        factor { 0 }\n");
						}
						else
						{
							if (disableAutoshading)
								fprintf(outFile, "        factor { 1 }\n");
							else if (defaultShading)
								fprintf(outFile, "        factor { 0.3 }\n");
							if (scaleAmbientFactor)
								fprintf(outFile, "        factor { %f }\n", scaleAmbientFactorValue);
							else
								fprintf(outFile, "        factor { 1 }\n");
						}
					}
					else
					{
						if (disableAutoshading)
							fprintf(outFile, "        factor { 1 }\n");
						else if (defaultShading)
							fprintf(outFile, "        factor { 0.3 }\n");
						else if (scaleAmbientFactor)
							fprintf(outFile, "        factor { %f }\n", scaleAmbientFactorValue);
						else
							fprintf(outFile, "        factor { 0.3 }\n");
					}
					fprintf(outFile, "      }\n");
				}
				if ((material->diffuseColor.contains) || (material->diffuseTexture.contains))
				{
					fprintf(outFile, "      diffuse {\n");
					if ((material->diffuseColor.contains))
					{
						fprintf(outFile, "        rgb { %s }\n", PrintHexColorToString(material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b));
						if ((material->diffuseTexture.contains) || specialKeywordMode)
						{
							if (disableAutoshading)
								fprintf(outFile, "        factor { 0.00100 }\n");
							else if (defaultShading)
								fprintf(outFile, "        factor { 0.7 }\n");
							else if (scaleDiffuseFactor)
								fprintf(outFile, "        factor { %f }\n", scaleDiffuseFactorValue);
							else
								fprintf(outFile, "        factor { 0.00100 }\n");
						}
						else
						{
							if (disableAutoshading)
								fprintf(outFile, "        factor { 0.00100 }\n");
							else if (defaultShading)
								fprintf(outFile, "        factor { 0.7 }\n");
							else if (scaleDiffuseFactor)
								fprintf(outFile, "        factor { %f }\n", scaleDiffuseFactorValue);
							else
								fprintf(outFile, "        factor { 0.70000 }\n");
						}
					}

					if (material->diffuseTexture.contains)
					{
						fprintf(outFile, "        texturename { \"" + PullOutImageName(material->diffuseTexture.stringValue) + "\" }\n");
						fprintf(outFile, "        textureparams {\n");
						if ((material->name.Find("EnvMapping") != -1) || ((material->diffuseTexture.contains) && (material->diffuseTexture.stringValue.Find("EnvMapping") != -1)))
						{
							fprintf(outFile, "          blendmode { decal }\n");
						}
						else
						{
							fprintf(outFile, "          blendmode { darken }\n");
						}
						fprintf(outFile, "          alphamode { none }\n");
						fprintf(outFile, "        }\n");
					}
					fprintf(outFile, "      }\n");
				}

				if (material->spectralColor.contains)
				{
					fprintf(outFile, "      specular {\n");
					fprintf(outFile, "        rgb { %s }\n", PrintHexColorToString(material->spectralColor.r, material->spectralColor.g, material->spectralColor.b));
					fprintf(outFile, "        factor { 0.00000 }\n");
					fprintf(outFile, "      }\n");
				}
				if ((material->alpha.contains) && (material->alpha.floatValue < 1) && (material->alpha.floatValue > 0))
				{
					fprintf(outFile, "      alpha { %d }\n", ColorFloatToHex(material->alpha.floatValue));
				}
				fprintf(outFile, "      phongsize { 32.004 }\n");
				if ((((material->alpha.contains) && (material->alpha.floatValue < 1) && (material->alpha.floatValue > 0)) && (material->diffuseTexture.contains) || (IsSpecialKeyword(specialKeywordMode, material))))
				{
					fprintf(outFile, "      map {\n");
					fprintf(outFile, "        kind { \"transparency\" }\n");
					fprintf(outFile, "        texturename { \"%s\" }\n", PullOutImageName(material->diffuseTexture.stringValue));
					fprintf(outFile, "        textureparams {\n");
					fprintf(outFile, "          blendmode { decal }\n");
					fprintf(outFile, "          alphamode { none }\n");
					// was causing to not read, not sure why
					/*if ((material->alpha.contains) && (material->alpha.floatValue < 1) && (material->alpha.floatValue > 0))
					{
						fprintf(outFile, "          percent { %f }\n", (material->alpha.floatValue * 100));
					}*/
					fprintf(outFile, "        }\n");
					fprintf(outFile, "      }\n");
				}
				if (IsSpecialKeywordEnvMapping(specialKeywordMode, material))
				{
					fprintf(outFile, "      map {\n");
					fprintf(outFile, "        kind { \"environment\" }\n");
					fprintf(outFile, "        texturename { \"%s\" }\n", PullOutImageName(material->diffuseTexture.stringValue));
					fprintf(outFile, "        textureparams {\n");
					fprintf(outFile, "          blendmode { decal }\n");
					fprintf(outFile, "          alphamode { none }\n");
					fprintf(outFile, "        }\n");
					fprintf(outFile, "      }\n");
				}
				fprintf(outFile, "    }\n");
				fprintf(outFile, "  }\n");
			}
		}

		std::map<CString, std::vector<CJointWeight*>> partToControlJointPointMapping;
		std::vector<CJointWeight*> totalJointWeights;

		std::vector<CGroup*>::iterator	itergroups;

		std::map<CString, int> duplicateNameCounter;

		// Correct duplicate names, because not valid for an8
		for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;

			map<CString, int>::iterator it = duplicateNameCounter.find(group->name);
			
			if (it != duplicateNameCounter.end())
			{
				CString tempStr;
				tempStr.Format("%d", duplicateNameCounter[group->name]);
				duplicateNameCounter[group->name] = duplicateNameCounter[group->name] + 1;

				group->name += tempStr;
			}
			else
			{
				duplicateNameCounter[group->name] = 1;
			}
		}

		for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
		{
			CGroup* group = ((CGroup*)(*itergroups));

			if (group->polygons.size() == 0)
				continue;

			if (regexFilterCheck)
			{
				std::tr1::regex rx(regexFilter.GetBuffer());
				std::string stdString = std::string(group->name.GetBuffer());
				regexFilter.ReleaseBuffer();

				bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
				group->name.ReleaseBuffer();

				if (!isMatch)
				{
					continue;
				}
			}

			CString defaultMaterial = "DefaultMaterial";

			std::vector<CPolygon*>::iterator	iterpolygon;
			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));
				defaultMaterial = polygon->materialName;
				break;
			}

			if (sortByTextureWithinGroup)
			{
				SortPolygonGroupByTexture(group);
			}

			std::vector<CVertice*> subVerticeList;
			std::vector<CUVCoordinate*> subUVList;
			std::vector<CNormal*> subNormalList;
			std::vector<CString> subMaterialList;
			std::vector<CJointWeight*> jointPointMapping;

			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));

				if (!(std::find(subMaterialList.begin(), subMaterialList.end(), polygon->materialName)!=subMaterialList.end()))
				{
					subMaterialList.push_back(polygon->materialName);
				}

				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					CPolygonPoint* polygonPoint = (CPolygonPoint*)*iterpolygonpoint;

					CVertice* vertice = NULL;
					if (polygonPoint->verticeIndex != -1)
						vertice = vertices[polygonPoint->verticeIndex];
					CUVCoordinate* uv = NULL;
					if (polygonPoint->uvIndex != -1)
						uv = uvCoordinates[polygonPoint->uvIndex];
					CNormal* normal = NULL;
					if (polygonPoint->normalIndex != -1)
						normal = normals[polygonPoint->normalIndex];

					if (vertice != NULL)
					{
						if (!(std::find(subVerticeList.begin(), subVerticeList.end(), vertice)!=subVerticeList.end()))
						{
							subVerticeList.push_back(vertice);

							CJointWeight* jointWeight = NULL;
							for (int x = 0; x < joints.size(); x++)
							{
								for (int y = 0; y < joints[x]->controlPoints.size(); y++)
								{
									CVertice* controlVertice = vertices[joints[x]->controlPoints[y]];
									if (controlVertice == vertice)
									{
										jointWeight = new CJointWeight();
										jointWeight->joint = joints[x];
										jointWeight->subIndexNumber = subVerticeList.size()-1;
										jointWeight->weight = 1.0f; // TODO
										totalJointWeights.push_back(jointWeight);
										break;
									}
								}

								if (jointWeight != NULL)
									break;
							}

							jointPointMapping.push_back(jointWeight);
						}
					}

					if (uv != NULL)
					{
						if (!(std::find(subUVList.begin(), subUVList.end(), uv)!=subUVList.end()))
						{
							subUVList.push_back(uv);
						}
					}

					if (normal != NULL)
					{
						if (!(std::find(subNormalList.begin(), subNormalList.end(), normal)!=subNormalList.end()))
						{
							subNormalList.push_back(normal);
						}
					}
				}
			}

			// center based on points
			// probably should be by triangle midpoint, but this is fine I imagine

			float centerX = 0;
			float centerY = 0;
			float centerZ = 0;

			if (recenterObjects)
			{
				std::vector<CVertice*>::iterator	iterPointList;
				for (iterPointList = subVerticeList.begin(); iterPointList != subVerticeList.end(); iterPointList++)
				{
					centerX += (*iterPointList)->vertex.x;
					centerY += (*iterPointList)->vertex.y;
					centerZ += (*iterPointList)->vertex.z;
				}

				centerX = centerX / subVerticeList.size();
				centerY = centerY / subVerticeList.size();
				centerZ = centerZ / subVerticeList.size();

				if (roundVerticesTenths)
				{
					centerX = roundtenths(centerX);
					centerY = roundtenths(centerY);
					centerZ = roundtenths(centerZ);
				}

				if (roundVerticesHundredths)
				{
					centerX = roundhundreths(centerX);
					centerY = roundhundreths(centerY);
					centerZ = roundhundreths(centerZ);
				}

				if (roundVertices)
				{
					centerX = round(centerX);
					centerY = round(centerY);
					centerZ = round(centerZ);
				}

				for (iterPointList = subVerticeList.begin(); iterPointList != subVerticeList.end(); iterPointList++)
				{
					(*iterPointList)->vertex.x = (*iterPointList)->vertex.x - centerX;
					(*iterPointList)->vertex.y = (*iterPointList)->vertex.y - centerY;
					(*iterPointList)->vertex.z = (*iterPointList)->vertex.z - centerZ;
				}
			}

			fprintf(outFile, "  mesh {\n");
			fprintf(outFile, "    name { \"%s\" }\n", group->name);
			fprintf(outFile, "    base {\n");
			fprintf(outFile, "      origin { (%f %f %f) }\n", centerX, centerY, centerZ);
			fprintf(outFile, "    }\n");
			fprintf(outFile, "    material { \"%s\" }\n", defaultMaterial);
			fprintf(outFile, "    smoothangle { 45 }\n");
			fprintf(outFile, "    /* %d points, %d faces, %d uvCoords */\n", (int)subVerticeList.size(), (int)group->polygons.size(), (int)subUVList.size());
			fprintf(outFile, "    materiallist {\n");

			

			std::vector<CString>::iterator	iterMaterialList;
			for (iterMaterialList = subMaterialList.begin(); iterMaterialList != subMaterialList.end(); iterMaterialList++)
			{
				fprintf(outFile, "      materialname { \"%s\" }\n", ((CString)*iterMaterialList));
			}
			fprintf(outFile, "    }\n");

			fprintf(outFile, "    points {\n");
			
			CString curWriteLine = "      ";
			std::vector<CVertice*>::iterator	iterPointList;
			for (iterPointList = subVerticeList.begin(); iterPointList != subVerticeList.end(); iterPointList++)
			{
				CVertice* vertice = (CVertice*)*iterPointList;

				CString formatVertice;
				formatVertice.Format("(%f %f %f) ", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

				if ((curWriteLine + formatVertice).GetLength() > 73)
				{
					fprintf(outFile, "%s\n", curWriteLine);
					curWriteLine = "      ";
				}

				curWriteLine += formatVertice;
			}

			// last one
			if (curWriteLine != "      ")
			{
				fprintf(outFile, "%s\n", curWriteLine);
			}
			
			fprintf(outFile, "    }\n");

			fprintf(outFile, "    texcoords {\n");

			curWriteLine = "      ";
			std::vector<CUVCoordinate*>::iterator	iterUVList;
			for (iterUVList = subUVList.begin(); iterUVList != subUVList.end(); iterUVList++)
			{
				CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;

				CString formatUV;
				formatUV.Format("(%f %f) ", uv->u, uv->v);

				if ((curWriteLine + formatUV).GetLength() > 73)
				{
					fprintf(outFile, "%s\n", curWriteLine);
					curWriteLine = "      ";
				}

				curWriteLine += formatUV;
			}

			// last one
			if (curWriteLine != "      ")
			{
				fprintf(outFile, "%s\n", curWriteLine);
			}

			fprintf(outFile, "    }\n");

			fprintf(outFile, "    faces {\n");

			for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
			{
				CPolygon* polygon = ((CPolygon*)(*iterpolygon));

				int materialIndex = 0;
				std::vector<CString>::iterator	iterMaterialList;
				for (iterMaterialList = subMaterialList.begin(); iterMaterialList != subMaterialList.end(); iterMaterialList++)
				{
					CString material = (CString)(*iterMaterialList);
					if (material == polygon->materialName)
					{
						break;
					}
					materialIndex++;
				}

				if (materialIndex >= subMaterialList.size())
					materialIndex = 0; // error

				if (foundTextureUV)
					curWriteLine.Format("      %d 4 %d -1 ( ", polygon->polygonPoints.size(), materialIndex);
				else
					curWriteLine.Format("      %d 0 %d -1 ( ", polygon->polygonPoints.size(), materialIndex);

				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;

				int pointNum = 0;
				int numPolyPointsExpected = polygon->polygonPoints.size();

				CPolygonPoint** resultingPoints = new CPolygonPoint*[polygon->polygonPoints.size()];

				int resultCounter = 0;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					resultingPoints[polygon->polygonPoints.size() - resultCounter - 1] = (CPolygonPoint*)*iterpolygonpoint;
					resultCounter++;
				}

				for (int x = 0; x < resultCounter; x++)
				{
					CPolygonPoint* polygonPoint = resultingPoints[x];
					CVertice* vertice = ((CVertice*)vertices[polygonPoint->verticeIndex]);

					int foundPoint = 0;
					std::vector<CVertice*>::iterator	iterVertice;
					for (iterVertice = subVerticeList.begin(); iterVertice != subVerticeList.end(); iterVertice++)
					{
						CVertice* verticeTest = (CVertice*)(*iterVertice);
						if (vertice == verticeTest)
						{
							break;
						}
						foundPoint++;
					}

					if (foundPoint >= subVerticeList.size())
						foundPoint = 0; // error

					CString formatIndexStr;
					if (foundTextureUV)
					{
						if (polygonPoint->uvIndex != -1)
						{
							CUVCoordinate* uv = ((CUVCoordinate*)uvCoordinates[polygonPoint->uvIndex]);

							int foundUV = 0;
							std::vector<CUVCoordinate*>::iterator	iterUV;
							for (iterUV = subUVList.begin(); iterUV != subUVList.end(); iterUV++)
							{
								CUVCoordinate* uvTest = (CUVCoordinate*)(*iterUV);
								if (uv == uvTest)
								{
									break;
								}
								foundUV++;
							}

							if (foundUV >= subUVList.size())
								foundUV = 0; // error

							formatIndexStr.Format("(%d %d) ", foundPoint, foundUV);
						}
						else
						{
							formatIndexStr.Format("(%d %d) ", foundPoint, 0);
						}
					}
					else
					{
						formatIndexStr.Format("(%d) ", foundPoint);
					}

					if ((curWriteLine + formatIndexStr).GetLength() > 73)
					{
						fprintf(outFile, "%s\n", curWriteLine);
						curWriteLine = "      ";
					}

					curWriteLine += formatIndexStr;

					pointNum++;

					if (pointNum == (numPolyPointsExpected))
					{
						curWriteLine += ")";
					}
				}

				delete [] resultingPoints;

				if (curWriteLine != "      ")
				{
					fprintf(outFile, "%s\n", curWriteLine);
				}
			}

			// end faces
			fprintf(outFile, "    }\n");

			// end mesh
			fprintf(outFile, "  }\n");


			partToControlJointPointMapping[group->name] = jointPointMapping;
		}

		// end object
		fprintf(outFile, "}\n");

		std::vector<CJoint*> rootJoints = FindRootJoints(joints);

		if (rootJoints.size() > 0)
		{
			fprintf(outFile, "figure { \"figure01\"\n");
			fprintf(outFile, "  bone { \"root\"\n");
			fprintf(outFile, "    length { 0 }\n");

			fprintf(outFile, "    bone { \"Object-Root\"\n");
			fprintf(outFile, "      length { 0 }\n");

			std::vector<CBone*> bones;
			std::vector<CBone*> rootBones;

			WriteAn8BoneDeclaration(NULL, bones, rootBones, joints, NULL);

			fprintf(outFile, "      namedobject { \"obj\"\n");
			
			fprintf(outFile, "        name { \"namedobjectObject-Root\" }\n");
			fprintf(outFile, "        material { \" -- default --\" }\n");

			for (int x = 0; x < bones.size(); x++)
			{
				fprintf(outFile, "        weightedby { \"" + bones[x]->name + "\" }\n");
			}
			
			std::map<CString, std::vector<CJointWeight*>>::iterator iterJointPointMapping;
			for (iterJointPointMapping = partToControlJointPointMapping.begin(); iterJointPointMapping != partToControlJointPointMapping.end(); iterJointPointMapping++ )
			{
				fprintf(outFile, "        weights { \"" + iterJointPointMapping->first + "\"\n");
				CString curWriteLine = "          ";

				for (int x = 0; x < iterJointPointMapping->second.size(); x++)
				{
					CJointWeight* jointWeight = iterJointPointMapping->second[x];

					int boneNumber = 0;
					if (jointWeight != NULL)
					{
						for (int y = 0; y < bones.size(); y++)
						{
							// joints only
							if (bones[y]->name == jointWeight->joint->name)
							{
								boneNumber = y;
								break;
							}
						}
					}

					CString formatJointWeight;
					if ((jointWeight != NULL) && (jointWeight->weight == 1.0f))
						formatJointWeight.Format("(1 (%d 1))", boneNumber);
					else if (jointWeight != NULL)
						formatJointWeight.Format("(1 (%d %f))", boneNumber, jointWeight->weight);
					else
						formatJointWeight.Format("(1 (0 0))");

					if ((curWriteLine + formatJointWeight).GetLength() > 73)
					{
						fprintf(outFile, "%s\n", curWriteLine);

						curWriteLine = "          ";
					}

					curWriteLine += formatJointWeight;
				}

				if (curWriteLine != "          ")
				{
					fprintf(outFile, "%s\n", curWriteLine);
				}
				fprintf(outFile, "        }\n");
			}

			// end named object
			fprintf(outFile, "      }\n");

			// Write first special
			CString boneName;
			boneName.Format("%s", "TopJoint");
			fprintf(outFile, "      bone { \"TopJoint\"\n");
			fprintf(outFile, "        length { 0 }\n");
			fprintf(outFile, "        diameter { 10 }\n");
			fprintf(outFile, "        dof { \"X\" -179  0 180  }\n");
			fprintf(outFile, "        dof { \"Y\" -179  0 180  }\n");
			fprintf(outFile, "        dof { \"Z\" -179  0 180  }\n");
			fprintf(outFile, "        orientation { (-0.500005 -0.500005 -0.500005 0.500005) }\n");

			for (int x = 0; x < rootBones.size(); x++)
			{
				WriteAn8Bone(outFile, rootBones[x], 1, Quat(0,0,0,1), "TopJoint");
			}

			// end bone first one
			fprintf(outFile, "      }\n");

			// end Object-Root bone
			fprintf(outFile, "    }\n");

			// end root bone
			fprintf(outFile, "  }\n");

			// end figure
			fprintf(outFile, "}\n");

			for (int x = 0; x < animations.size(); x++)
			{
				CAnimation* animation = animations[x];
				fprintf(outFile, "sequence { \"" + animation->name + "\"\n");
				fprintf(outFile, "  figure { \"figure01\" }\n");

				CString tempStr;
				tempStr.Format("%d", animation->keyframes.size());
				fprintf(outFile, "  frames { " + tempStr + " }\n");

				fprintf(outFile, "  jointangle { \"TopJoint\" \"X\"\n");
				fprintf(outFile, "    track {\n");

				for (int z = 0; z < animation->keyframes.size(); z++)
				{
					if (animation->keyframes[z]->rotation.z == 0)
						fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
					else
						fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, animation->keyframes[z]->rotation.z);
				}

				fprintf(outFile, "    }\n");
				fprintf(outFile, "  }\n");

				fprintf(outFile, "  jointangle { \"TopJoint\" \"Y\"\n");
				fprintf(outFile, "    track {\n");

				for (int z = 0; z < animation->keyframes.size(); z++)
				{
					if (animation->keyframes[z]->rotation.x == 0)
						fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
					else
						fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, animation->keyframes[z]->rotation.x);
				}

				fprintf(outFile, "    }\n");
				fprintf(outFile, "  }\n");

				fprintf(outFile, "  jointangle { \"TopJoint\" \"Z\"\n");
				fprintf(outFile, "    track {\n");

				for (int z = 0; z < animation->keyframes.size(); z++)
				{
					if (animation->keyframes[z]->rotation.y == 0)
						fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
					else
						fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, animation->keyframes[z]->rotation.y);
				}

				fprintf(outFile, "    }\n");
				fprintf(outFile, "  }\n");

				for (int y = 0; y < joints.size(); y++)
				{
					CJoint* joint = joints[y];

					fprintf(outFile, "  jointangle { \"" + joint->name + "\" \"X\"\n");
					fprintf(outFile, "    track {\n");

					for (int z = 0; z < animation->keyframes.size(); z++)
					{
						CKeyframe* keyframe = animation->keyframes[z];
						
						for (int r = 0; r < keyframe->animationParts.size(); r++)
						{
							CAnimationPart* animationPart = keyframe->animationParts[r];
							if (animationPart->name == joint->name)
							{
								float rotation = 0;

								if ((animationPart->rotation.x != 0) || (animationPart->rotation.y != 0) || (animationPart->rotation.z != 0))
								{
									float4x4 coordinateChangeMatrix = float4x4::identity;
									for (int r = 0; r < bones.size(); r++)
									{
										if ((bones[r]->boneStart != NULL) && (bones[r]->boneEnd != NULL))
										{
											if ((bones[r]->boneStart->name == animationPart->name) && (bones[r]->boneEnd->name == animationPart->name))
											{
												coordinateChangeMatrix = float4x4(bones[r]->overallOrientation);
											}
										}
									}

									float3 translate;
									float4x4 rotationMatrix;
									float3 scale;
									coordinateChangeMatrix.Decompose(translate, rotationMatrix, scale);

									float4x4 coordinateChangeMatrixInverse = rotationMatrix;
									coordinateChangeMatrixInverse.InverseOrthonormal();

									float4x4 matrixRotation = float4x4::FromEulerXYZ(DegToRad(animationPart->rotation.x), DegToRad(animationPart->rotation.y), DegToRad(animationPart->rotation.z));

									float4x4 matrixInNewCoordinateSystem = coordinateChangeMatrix * matrixRotation * coordinateChangeMatrixInverse;

									float3 relativeRotation = matrixInNewCoordinateSystem.ToEulerXYZ();
									
									rotation = RadToDeg(relativeRotation.z);
								}

								while (rotation > 180.0f)
									rotation -= 360.0f;
								while (rotation < -180.0f)
									rotation += 360.0f;

								if (rotation == 0)
									fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
								else
									fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, rotation);
								break;
							}
						}
					}

					fprintf(outFile, "    }\n");
					fprintf(outFile, "  }\n");


					fprintf(outFile, "  jointangle { \"" + joint->name + "\" \"Y\"\n");
					fprintf(outFile, "    track {\n");

					for (int z = 0; z < animation->keyframes.size(); z++)
					{
						CKeyframe* keyframe = animation->keyframes[z];
						
						for (int r = 0; r < keyframe->animationParts.size(); r++)
						{
							CAnimationPart* animationPart = keyframe->animationParts[r];
							if (animationPart->name == joint->name)
							{
								float rotation = 0;

								if ((animationPart->rotation.x != 0) || (animationPart->rotation.y != 0) || (animationPart->rotation.z != 0))
								{
									float4x4 coordinateChangeMatrix = float4x4::identity;
									for (int r = 0; r < bones.size(); r++)
									{
										if ((bones[r]->boneStart != NULL) && (bones[r]->boneEnd != NULL))
										{
											if ((bones[r]->boneStart->name == animationPart->name) && (bones[r]->boneEnd->name == animationPart->name))
											{
												coordinateChangeMatrix = float4x4(bones[r]->overallOrientation);
											}
										}
									}

									float3 translate;
									float4x4 rotationMatrix;
									float3 scale;
									coordinateChangeMatrix.Decompose(translate, rotationMatrix, scale);

									float4x4 coordinateChangeMatrixInverse = rotationMatrix;
									coordinateChangeMatrixInverse.InverseOrthonormal();

									float4x4 matrixRotation = float4x4::FromEulerXYZ(DegToRad(animationPart->rotation.x), DegToRad(animationPart->rotation.y), DegToRad(animationPart->rotation.z));

									float4x4 matrixInNewCoordinateSystem = coordinateChangeMatrix * matrixRotation * coordinateChangeMatrixInverse;

									float3 relativeRotation = matrixInNewCoordinateSystem.ToEulerXYZ();
									
									rotation = RadToDeg(relativeRotation.x);
								}

								while (rotation > 180.0f)
									rotation -= 360.0f;
								while (rotation < -180.0f)
									rotation += 360.0f;

								if (rotation == 0)
									fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
								else
									fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, rotation);
								break;
							}
						}
					}

					fprintf(outFile, "    }\n");
					fprintf(outFile, "  }\n");


					fprintf(outFile, "  jointangle { \"" + joint->name + "\" \"Z\"\n");
					fprintf(outFile, "    track {\n");

					for (int z = 0; z < animation->keyframes.size(); z++)
					{
						CKeyframe* keyframe = animation->keyframes[z];
						
						for (int r = 0; r < keyframe->animationParts.size(); r++)
						{
							CAnimationPart* animationPart = keyframe->animationParts[r];
							if (animationPart->name == joint->name)
							{
								float rotation = 0;

								if ((animationPart->rotation.x != 0) || (animationPart->rotation.y != 0) || (animationPart->rotation.z != 0))
								{
									float4x4 coordinateChangeMatrix = float4x4::identity;
									for (int r = 0; r < bones.size(); r++)
									{
										if ((bones[r]->boneStart != NULL) && (bones[r]->boneEnd != NULL))
										{
											if ((bones[r]->boneStart->name == animationPart->name) && (bones[r]->boneEnd->name == animationPart->name))
											{
												coordinateChangeMatrix = float4x4(bones[r]->overallOrientation);
											}
										}
									}

									float3 translate;
									float4x4 rotationMatrix;
									float3 scale;
									coordinateChangeMatrix.Decompose(translate, rotationMatrix, scale);

									float4x4 coordinateChangeMatrixInverse = rotationMatrix;
									coordinateChangeMatrixInverse.InverseOrthonormal();

									float4x4 matrixRotation = float4x4::FromEulerXYZ(DegToRad(animationPart->rotation.x), DegToRad(animationPart->rotation.y), DegToRad(animationPart->rotation.z));

									float4x4 matrixInNewCoordinateSystem = coordinateChangeMatrix * matrixRotation * coordinateChangeMatrixInverse;

									float3 relativeRotation = matrixInNewCoordinateSystem.ToEulerXYZ();
									
									rotation = RadToDeg(relativeRotation.y);
								}

								while (rotation > 180.0f)
									rotation -= 360.0f;
								while (rotation < -180.0f)
									rotation += 360.0f;

								if (rotation == 0)
									fprintf(outFile, "      floatkey { %d 0 0 0 \"C\" }\n", z);
								else
									fprintf(outFile, "      floatkey { %d %f 0 0 \"C\" }\n", z, rotation);
								break;
							}
						}
					}

					fprintf(outFile, "    }\n");
					fprintf(outFile, "  }\n");
				}

				fprintf(outFile, "}\n");
			}

			for (int x = 0; x < bones.size(); x++)
			{
				delete bones[x];
			}

			// TODO scene
			for (int x = 0; x < animations.size(); x++)
			{
				CAnimation* animation = animations[x];
	
				CString tempStr;
				fprintf(outFile, "scene { \"scene%d\"\n", x);
				fprintf(outFile, "  frames { %d }\n", (int)animation->keyframes.size());
				fprintf(outFile, "  groundgrid { 0 }\n");
				fprintf(outFile, "  shadowbias { 0.00100 }\n");
				fprintf(outFile, "  background { 153 153 153 }\n");
				fprintf(outFile, "  figureelement { \"efigure01\" \"figure01\"\n");
				fprintf(outFile, "	loc { (0 0 0) }\n");
				fprintf(outFile, "	namedsequence { \"" + animation->name + "\" 0 }\n");
				fprintf(outFile, "	castshadow { }\n");
				fprintf(outFile, "	receiveshadows { }\n");
				fprintf(outFile, "	controller { \"position\"\n");
				fprintf(outFile, "	  track {\n");
				for (int y = 0; y < animation->keyframes.size(); y++)
				{
					fprintf(outFile, "		pointkey { %d (%f %f %f) (-1 0 0) (1 0 0) \"C\" }\n", y, animation->keyframes[y]->translation.x, animation->keyframes[y]->translation.y, animation->keyframes[y]->translation.z);
				}
				fprintf(outFile, "	  }\n");
				fprintf(outFile, "	}\n");
				fprintf(outFile, "  }\n");
				fprintf(outFile, "}\n");
			}
		}

		std::map<CString, std::vector<CJointWeight*>>::iterator iterJointPointMapping;
		for (iterJointPointMapping = partToControlJointPointMapping.begin(); iterJointPointMapping != partToControlJointPointMapping.end(); iterJointPointMapping++ )
		{
			for (int x = 0; x < iterJointPointMapping->second.size(); x++)
			{
				if (iterJointPointMapping->second[x] != NULL)
				{
					std::vector<CJointWeight*>::iterator it = std::find(totalJointWeights.begin(), totalJointWeights.end(), iterJointPointMapping->second[x]);
					if(it != totalJointWeights.end())
						totalJointWeights.erase(it);

					delete iterJointPointMapping->second[x];
				}
			}
		}

		for (int x = 0; x < totalJointWeights.size(); x++)
		{
			delete totalJointWeights[x];
		}

		fclose(outFile);
	}
	else
	{
		if (doMessageBoxes)
			MessageBox("Can't open output file " + outputFile);
		return;
	}
}

void CObjToAn8Dlg::CalculateAn8JointOrientation(CJoint* joint, CBone* rootBone, AN8XMATRIX& finalMatrix)
{
	AN8XMATRIX i_ParentAbsoluteMatrix;
	CalculateAn8OrientationRecursive(rootBone, &i_ParentAbsoluteMatrix, finalMatrix, joint);
}

void CObjToAn8Dlg::CalculateAn8OrientationRecursive(CBone* bone, AN8XMATRIX* i_ParentAbsoluteMatrix, AN8XMATRIX& finalMatrix, CJoint* endJoint)
{
	AN8XMATRIX absoluteMatrix;

	CString nodeName = bone->name;

	if (nodeName == "root")
	{
		nodeName = "TopJoint";
	}

	if (nodeName.Find("TopJoint") == -1)
	{
		AN8XBone newBone;
		newBone.Name = bone->name;
		newBone.Orientation.x = bone->orientation.x;
		newBone.Orientation.y = bone->orientation.y;
		newBone.Orientation.z = bone->orientation.z; 
		newBone.Orientation.w = bone->orientation.w;
		if (bone->parentBone != NULL)
			newBone.ParentName = bone->parentBone->name;
		else
			newBone.ParentName = "";
		newBone.Length = bone->length;

		double yaw,pitch,roll;
		yaw = pitch = roll = 0.0f;
		
		/*AN8XYawPitchRollQuaternion(&yaw,&pitch,&roll,&bone.Orientation);
		//newBone.Rotation = AN8XVECTOR3(float(yaw),float(pitch),float(roll));
		newBone.Rotation = AN8XVECTOR3(float(pitch),float(yaw),float(roll)); // XXX*/

		// Compute Absolute matrix /////////////////////////////////////
		AN8XMATRIX boneMatrix;
		AN8XMatrixRotationQuaternion( &boneMatrix , &bone->orientation );
		AN8XMATRIX boneTranslationMatrix;
		boneTranslationMatrix._41 = 0.0f; // x
		boneTranslationMatrix._42 = bone->length; // y
		boneTranslationMatrix._43 = 0.0f; // z
		AN8XMatrixMultiply(&boneMatrix,&boneMatrix,&boneTranslationMatrix);
		
		AN8XMatrixMultiply(&absoluteMatrix,i_ParentAbsoluteMatrix,&boneMatrix);

		if ((bone->boneStart == endJoint) && (bone->boneEnd == endJoint))
		{
			finalMatrix = absoluteMatrix;
			return;
		}
	}

	for (int x = 0; x < bone->boneConnections.size(); x++)
	{
		CalculateAn8OrientationRecursive(bone->boneConnections[x], &absoluteMatrix, finalMatrix, endJoint);
	}
}

// This bit taken from bvh2anim8or
Quat Vector_to_Quat(float3 V)
{
    float3 vUp(0, 1, 0);
	float3 vRight = vUp.Cross(V);
    vUp = V.Cross(vRight);
	Quat returnQuat;
    returnQuat.w = sqrt(1.0 + vRight.x + vUp.y + V.z) / 2.0;
    float WScale = returnQuat.w * 4.0;
    if (fabs(WScale) > 0.00001)
	{
        returnQuat.x = ((V.y - vUp.z) / WScale);
        returnQuat.y = ((vRight.z - V.x) / WScale);
        returnQuat.z = ((vUp.x - vRight.y) / WScale);
	}
	else
	{
        returnQuat.x = 0.0;
        returnQuat.y = 0.0;
        returnQuat.z = 0.0;
        returnQuat.w = 1.0;
	}
   return returnQuat;
}


void CObjToAn8Dlg::WriteAn8Bone(FILE* outFile, CBone* bone, int indent, Quat parentOrientation, CString parentName)
{
	CString initialIndentStr;

	for (int x = 0; x < indent; x++)
		initialIndentStr += "  ";

	float length = 0;
	Quat quat = Quat::identity;
	if (bone->boneStart == bone->boneEnd)
	{
		quat = Quat(0, 0, 0, 1);
	}
	else
	{
		float3 orientation = float3((bone->boneEnd->positionAbsolute.x), (bone->boneEnd->positionAbsolute.y), (bone->boneEnd->positionAbsolute.z));
		if (bone->boneStart != NULL)
			orientation = float3(orientation.x - bone->boneStart->positionAbsolute.x, orientation.y - bone->boneStart->positionAbsolute.y, orientation.z - bone->boneStart->positionAbsolute.z);

		length = orientation.Length();

		if (length > 0)
		{
			CBone* realParent = NULL;

			CBone* tempBone = bone->parentBone;

			while (tempBone != NULL)
			{
				if (tempBone->boneStart != tempBone->boneEnd)
				{
					float3 orientation = float3((tempBone->boneEnd->positionAbsolute.x), (tempBone->boneEnd->positionAbsolute.y), (tempBone->boneEnd->positionAbsolute.z));
					if (tempBone->boneStart != NULL)
						orientation = float3(orientation.x - tempBone->boneStart->positionAbsolute.x, orientation.y - tempBone->boneStart->positionAbsolute.y, orientation.z - tempBone->boneStart->positionAbsolute.z);

					length = orientation.Length();

					if (length > 0)
					{
						realParent = tempBone;
						break;
					}
				}

				tempBone = tempBone->parentBone;
			}

			CBone* realGrandParent = NULL;
			if (realParent != NULL)
			{
				tempBone = realParent->parentBone;

				while (tempBone != NULL)
				{
					if (tempBone->boneStart != tempBone->boneEnd)
					{
						float3 orientation = float3((tempBone->boneEnd->positionAbsolute.x), (tempBone->boneEnd->positionAbsolute.y), (tempBone->boneEnd->positionAbsolute.z));
						if (tempBone->boneStart != NULL)
							orientation = float3(orientation.x - tempBone->boneStart->positionAbsolute.x, orientation.y - tempBone->boneStart->positionAbsolute.y, orientation.z - tempBone->boneStart->positionAbsolute.z);

						length = orientation.Length();

						if (length > 0)
						{
							realGrandParent = tempBone;
							break;
						}
					}

					tempBone = tempBone->parentBone;
				}
			}

			if (realParent == NULL) // From TopJoint
			{
				quat = Quat::RotateFromTo(float3(0,1,0), bone->boneEnd->positionAbsolute.Normalized());

				length = bone->boneEnd->positionAbsolute.Length();

				float3 startLink = float3(0,1,0);
				float3 endLink = bone->boneEnd->positionAbsolute;

				// This bit taken from bvh2anim8or
				Quat qRef = (Vector_to_Quat((float3(0,1,0))));
                Quat qParent = (Vector_to_Quat((startLink.Normalized())));
                Quat qBone = (Vector_to_Quat((endLink.Normalized())));

				qRef.Normalize();
				qParent.Normalize();
				qBone.Normalize();

				Quat qRot = (qParent.Conjugated().Normalized() * qRef).Normalized();
				Quat qrParent = qParent * qRot;
				Quat qrBone = qBone * qRot;

				Quat qOr = (qrBone.Conjugated().Normalized() * qrParent).Normalized();

				if (abs(qOr.w) < 0.000001)
				{
                    qOr.x = 0.0;
                    qOr.y = 0.0;
                    qOr.z = 0.0;
                    qOr.w = 1.0;
				}

                quat = qOr;
			}
			else
			{
				float3 startLink = realParent->boneEnd->positionAbsolute - ((realParent->boneStart == 0) ? float3(0,0,0) : realParent->boneStart->positionAbsolute);
				float3 endLink = bone->boneEnd->positionAbsolute - bone->boneStart->positionAbsolute;
				
				quat = Quat::RotateFromTo( startLink.Normalized(), endLink.Normalized() );

				length = (bone->boneEnd->positionAbsolute - bone->boneStart->positionAbsolute).Length();

				// This bit taken from bvh2anim8or
				Quat qRef = (Vector_to_Quat((float3(0,1,0))));
                Quat qParent = (Vector_to_Quat((startLink.Normalized())));
                Quat qBone = (Vector_to_Quat((endLink.Normalized())));

				qRef.Normalize();
				qParent.Normalize();
				qBone.Normalize();

				Quat qRot = (qParent.Conjugated().Normalized() * qRef).Normalized();
				Quat qrParent = qParent * qRot;
				Quat qrBone = qBone * qRot;

				Quat qOr = (qrBone.Conjugated().Normalized() * qrParent).Normalized();

				if (abs(qOr.w) < 0.000001)
				{
                    qOr.x = 0.0;
                    qOr.y = 0.0;
                    qOr.z = 0.0;
                    qOr.w = 1.0;
				}

                quat = qOr;
			}
		}
		else
		{
			quat = Quat(0, 0, 0, 1);
		}
	}

	bone->orientation.x = quat.x;
	bone->orientation.y = quat.y;
	bone->orientation.z = quat.z;
	bone->orientation.w = quat.w;
	bone->length = length;

	CString tempStr;

	bone->overallOrientation.x = parentOrientation.x;
	bone->overallOrientation.y = parentOrientation.y;
	bone->overallOrientation.z = parentOrientation.z;
	bone->overallOrientation.w = parentOrientation.w;

	Quat orientationAdjusted = Quat(0, 0.70711, 0, 0.70711) * Quat(0, 0, 0.70711, 0.70711) * parentOrientation;

	Quat parentInverse = parentOrientation;
	parentInverse.Inverse();

	parentInverse = parentInverse * Quat(0, 0, -0.70711, 0.70711) * Quat(0, -0.70711, 0, 0.70711);

	bool isReverseRotationJoint = false;

	if (length == 0)
	{
		// From TopJoint
		if (bone->boneStart == NULL)
		{
			fprintf(outFile, initialIndentStr + "      bone { \"" + bone->name + "\"\n");

			tempStr.Format("%f", length);
			fprintf(outFile, initialIndentStr + "        length { 0 }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 0.01000 }\n");

			fprintf(outFile, initialIndentStr + "        dof { \"X\" -179  0 180  }\n");
			fprintf(outFile, initialIndentStr + "        dof { \"Y\" -179  0 180  }\n");
			fprintf(outFile, initialIndentStr + "        dof { \"Z\" -179  0 180  }\n");
			fprintf(outFile, initialIndentStr + "        orientation { (0.500005 0.500005 0.500005 0.500005) }\n");
		}
		else
		{
			bone->overallOrientation.x = 0;
			bone->overallOrientation.y = 0;
			bone->overallOrientation.z = 0;
			bone->overallOrientation.w = 1;

			isReverseRotationJoint = true;

			fprintf(outFile, initialIndentStr + "      bone { \"" + parentName + "A" + "\"\n");
			fprintf(outFile, initialIndentStr + "        length { 0 }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 0.01000 }\n");

			tempStr.Format("%f %f %f %f", parentInverse.x, parentInverse.y, parentInverse.z, parentInverse.w);
			fprintf(outFile, initialIndentStr + "        orientation { (" + tempStr + ") }\n");

			fprintf(outFile, initialIndentStr + "        locked { }\n");



			indent++;
			initialIndentStr = "";
			for (int x = 0; x < indent; x++)
				initialIndentStr += "  ";



			fprintf(outFile, initialIndentStr + "      bone { \"" + bone->name + "\"\n");

			tempStr.Format("%f", length);
			fprintf(outFile, initialIndentStr + "        length { " + ((length == 0) ? "0" : tempStr) + " }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 10 }\n");
			fprintf(outFile, initialIndentStr + "        dof { \"X\" -179  0 180  }\n");
			fprintf(outFile, initialIndentStr + "        dof { \"Y\" -179  0 180  }\n");
			fprintf(outFile, initialIndentStr + "        dof { \"Z\" -179  0 180  }\n");


			indent++;
			initialIndentStr = "";
			for (int x = 0; x < indent; x++)
				initialIndentStr += "  ";



			fprintf(outFile, initialIndentStr + "      bone { \"" + parentName + "B" + "\"\n");
			fprintf(outFile, initialIndentStr + "        length { 0 }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 0.01000 }\n");

			tempStr.Format("%f %f %f %f", orientationAdjusted.x, orientationAdjusted.y, orientationAdjusted.z, orientationAdjusted.w);
			fprintf(outFile, initialIndentStr + "        orientation { (" + tempStr + ") }\n");

			fprintf(outFile, initialIndentStr + "        locked { }\n");
		}
	}
	else
	{
		// From TopJoint
		if (bone->boneStart == NULL)
		{
			fprintf(outFile, initialIndentStr + "      bone { \"" + bone->name + "\"\n");

			tempStr.Format("%f", length);
			fprintf(outFile, initialIndentStr + "        length { " + ((length == 0) ? "0" : tempStr) + " }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 2 }\n");

			Quat backFromTopJointQuat = Quat(0.500005, 0.500005, 0.500005, 0.500005) * quat;

			tempStr.Format("%f %f %f %f", backFromTopJointQuat.x, backFromTopJointQuat.y, backFromTopJointQuat.z, backFromTopJointQuat.w);
			fprintf(outFile, initialIndentStr + "        orientation { (" + tempStr + ") }\n");

			fprintf(outFile, initialIndentStr + "        locked { }\n");
		}
		else
		{
			fprintf(outFile, initialIndentStr + "      bone { \"" + bone->name + "\"\n");

			tempStr.Format("%f", length);
			fprintf(outFile, initialIndentStr + "        length { " + ((length == 0) ? "0" : tempStr) + " }\n");

			fprintf(outFile, initialIndentStr + "        diameter { 2 }\n");

			tempStr.Format("%f %f %f %f", quat.x, quat.y, quat.z, quat.w);
			fprintf(outFile, initialIndentStr + "        orientation { (" + tempStr + ") }\n");

			fprintf(outFile, initialIndentStr + "        locked { }\n");
		}
	}

	
	for (int x = 0; x < bone->boneConnections.size(); x++)
	{
		WriteAn8Bone(outFile, bone->boneConnections[x], indent+1, parentOrientation * quat, bone->name);
	}

	fprintf(outFile, initialIndentStr + "      }\n");
	if (isReverseRotationJoint)
	{
		indent--;
		initialIndentStr = "";
		for (int x = 0; x < indent; x++)
			initialIndentStr += "  ";

		fprintf(outFile, initialIndentStr + "      }\n");

		indent--;
		initialIndentStr = "";
		for (int x = 0; x < indent; x++)
			initialIndentStr += "  ";

		fprintf(outFile, initialIndentStr + "      }\n");
	}
}


bool CObjToAn8Dlg::PerformConversion(bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool renameGroups, bool stripImagePaths, 
		bool roundVertices, bool roundVerticesTenths, bool roundVerticesHundredths, bool ignoreShading, bool ignoreShadingPoint7, bool recenterObjects, 
		bool scaleVertices, bool noGroups, bool primarySecondaryGroups, bool useReplacementFile, bool fixKsKaNs, 
		bool scaleDiffuse, bool scaleAmbient, bool scaleSpecular, bool mergeHierarchicalGroupsToEnd, bool moveCoords, 
		bool scaleAmbientFactorAn8, bool scaleDiffuseFactorAn8, bool disableAutoshading, bool defaultShading, 
		bool recalculateUVs, bool scaleUVs, bool specifyUVTileSize, 
		float scaleDiffuseFactor, float scaleAmbientFactor, float scaleSpecularFactor, float xMove, float yMove, float zMove, 
		float scaleVerticesFactor, float texelSizeU, float texelSizeV, float scaleUVsFactor, 
		float scaleAmbientFactorAn8Value, float scaleDiffuseFactorAn8Value, bool regexFilterCheck, CString regexFilter, 
		CString inputFile, CString outputFile, CString replaceFile, CString fbxExportType, bool overrideSkeleton, CString overrideSkeletonFile, bool doMessageBoxes, CString jointMode, float fps, bool sortByTextureWithinGroup,
		bool outputObjVColors)
{
	JointType jointType = Absolute;
	jointMode = jointMode.MakeLower();
	if (jointMode == "relative")
		jointType = Relative;

	if (fps < 0)
		fps = 30;

	this->doMessageBoxes = doMessageBoxes;

	if ((inputFile.GetLength() < 3) || (inputFile.Find(".") == -1))
	{
		if (doMessageBoxes)
			MessageBox("Invalid input filename");
		return false;
	}

	int lastDot = inputFile.ReverseFind('.');
	CString extensionInput = inputFile.Mid(lastDot + 1);

	CString inputPath = inputFile.Mid(0, (inputFile.ReverseFind('\\')+1));

	if ((extensionInput.MakeLower() != "obj")
		&& (extensionInput.MakeLower() != "an8")
		&& (extensionInput.MakeLower() != "dae")
		&& (extensionInput.MakeLower() != "bvh")
		#ifdef FBXSDK_NEW_API
		&& (extensionInput.MakeLower() != "fbx")
		#endif
		)
	{
		if (doMessageBoxes)
			MessageBox("Not supported input type");
		return false;
	}

	if ((outputFile.GetLength() < 3) || (outputFile.Find(".") == -1))
	{
		if (doMessageBoxes)
			MessageBox("Invalid output filename");
		return false;
	}

	lastDot = outputFile.ReverseFind('.');
	CString extensionWrite = outputFile.Mid(lastDot + 1);

	if ((extensionWrite != "obj") && (extensionWrite != "an8") && (extensionWrite != "bvh") && (extensionWrite != "dae")
		#ifdef FBXSDK_NEW_API
		 && (extensionWrite != "fbx")
		#endif
		)
	{
		if (doMessageBoxes)
			MessageBox("Not supported output type");
		return false;
	}

	if (GetSizeFile(inputFile) == 0)
	{
		if (doMessageBoxes)
			MessageBox("Bad input file");
		return false;
	}

	std::vector<CVerticeColor*> verticeColors;
	std::vector<CNormal*> normals;
	std::vector<CUVCoordinate*> uvCoordinates;
	std::vector<CVertice*> vertices;
	std::vector<CGroup*> groups;
	std::vector<CMaterialFile*> materialFiles;
	std::vector<CMaterial*> replacementMaterialOverrides;

	std::vector<CJoint*> joints;
	std::vector<CAnimation*> animations;

	bool foundTextureUV = false;
	bool foundNormals = false;
	bool foundVerticeColors = false;

	if (extensionInput.MakeLower() == "obj")
	{
		if (!ReadObjFile(inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations
			, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors,
			noGroups))
		{
			DisposeStructures(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, replacementMaterialOverrides, joints, animations);
			return false;
		}
	}
	else if ((extensionInput.MakeLower() == "dae") || (extensionInput.MakeLower() == "bvh"))
	{
		if (!ReadAssimpFile(extensionInput.MakeLower(), inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations
			, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors,
			noGroups))
		{
			DisposeStructures(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, replacementMaterialOverrides, joints, animations);
			return false;
		}
	}
	else if (extensionInput.MakeLower() == "an8")
	{
		if (!ReadAn8File(inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations
			, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors,
			noGroups))
		{
			DisposeStructures(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, replacementMaterialOverrides, joints, animations);
			return false;
		}
	}
	#ifdef FBXSDK_NEW_API
	else if (extensionInput.MakeLower() == "fbx")
	{
		if (!ReadFbxFile(inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors,
			noGroups, overrideSkeleton, overrideSkeletonFile, jointType))
		{
			DisposeStructures(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, replacementMaterialOverrides, joints, animations);
			return false;
		}
	}
	#endif

	if (useReplacementFile)
	{
		ReadMaterialOverridesFile(replacementMaterialOverrides, replaceFile);
	}

	if (scaleDiffuse)
	{
		ScaleDiffuseShading(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, scaleDiffuseFactor);
	}

	if (scaleAmbient)
	{
		ScaleAmbientShading(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, scaleAmbientFactor);
	}

	if (scaleSpecular)
	{
		ScaleSpecularShading(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, scaleSpecularFactor);
	}

	if (fixKsKaNs)
	{
		FixNiKsKaNs(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);		
	}

	if (ignoreShading)
	{
		IgnoreShading(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (ignoreShadingPoint7)
	{
		IgnoreShadingPoint7(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (moveCoords)
	{
		MoveVertices(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, xMove, yMove, zMove);
	}
	
	if (roundVerticesHundredths)
	{
		RoundVerticesHundredths(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (roundVerticesTenths)
	{
		RoundVerticesTenths(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (roundVertices)
	{
		RoundVertices(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (scaleVertices)
	{
		ScaleVertices(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, scaleVerticesFactor);
	}

	if (recenterObjects)
	{
		RecenterVertices(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if ((stripImagePaths)
		|| (extensionWrite.MakeLower() == "dae")
		#ifdef FBXSDK_NEW_API
		|| (extensionWrite.MakeLower() == "fbx")
		#endif
		) // fbx will be auto-appended
	{
		StripImagePaths(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (useReplacementFile)
	{
		ReplaceMaterials(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, replacementMaterialOverrides);
	}

	if (mergeLikeMaterials)
	{
		MergeLikeMaterials(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (renameMaterials)
	{
		RenameMaterials(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (recalculateUVs)
	{
		RecalculateUVs(inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, specifyUVTileSize, texelSizeU, texelSizeV);
	}

	if (scaleUVs)
	{
		ScaleUVs(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, 
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, scaleUVsFactor);
	}

	if (renameGroups)
	{
		RenameGroups(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors);
	}

	if (extensionWrite.MakeLower() == "an8")
	{
		WriteAn8File(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, ignoreShading, ignoreShadingPoint7, roundVertices, roundVerticesTenths, roundVerticesHundredths, recenterObjects, scaleAmbientFactorAn8, scaleAmbientFactorAn8Value, scaleDiffuseFactorAn8, scaleDiffuseFactorAn8Value, disableAutoshading, defaultShading, regexFilterCheck, regexFilter, sortByTextureWithinGroup);
	}
	else if (extensionWrite.MakeLower() == "obj")
	{
		WriteObjFile(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, ignoreShading, ignoreShadingPoint7, noGroups, primarySecondaryGroups, mergeHierarchicalGroupsToEnd, regexFilterCheck, regexFilter, sortByTextureWithinGroup, outputObjVColors);
	}
	else if (extensionWrite.MakeLower() == "bvh")
	{
		WriteBvhFile(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, ignoreShading, ignoreShadingPoint7, noGroups, primarySecondaryGroups, mergeHierarchicalGroupsToEnd, regexFilterCheck, regexFilter);
	}
	else if (extensionWrite.MakeLower() == "dae")
	{
		WriteOwnDaeFile(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, ignoreShading, ignoreShadingPoint7, noGroups, primarySecondaryGroups, mergeHierarchicalGroupsToEnd, regexFilterCheck, regexFilter, sortByTextureWithinGroup);
		//WriteAssimpFile(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			//specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, ignoreShading, ignoreShadingPoint7, noGroups, primarySecondaryGroups, mergeHierarchicalGroupsToEnd, regexFilterCheck, regexFilter, extensionWrite.MakeLower(), sortByTextureWithinGroup);
	}
	#ifdef FBXSDK_NEW_API
	else if (extensionWrite.MakeLower() == "fbx")
	{
		WriteFbxFile(outputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, inputPath, regexFilterCheck, regexFilter, fbxExportType, fps, sortByTextureWithinGroup);
	}
	#endif


	DisposeStructures(verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, replacementMaterialOverrides, joints, animations);

	return true;
}

void CObjToAn8Dlg::OnBnClickedButton3()
{
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);

	char iniFile[8020];
	strcpy(iniFile, tempFolder);
	strcat(iniFile, "\\obj2an8.ini");

	CString inputFile;
	mInputFilename.GetWindowText(inputFile);
	WritePrivateProfileString("Settings", "inputFilename", inputFile, iniFile);

	CString outputFile;
	mOutputFilename.GetWindowText(outputFile);
	WritePrivateProfileString("Settings", "outputFilename", outputFile, iniFile);

	CString replaceFile;
	mReplacesFilename.GetWindowText(replaceFile);
	WritePrivateProfileString("Settings", "replaceFile", replaceFile, iniFile);

	CString fbxExportType = "ascii";
	bool overrideSkeleton = false;
	CString overrideSkeletonFile = "";

	#ifdef FBXSDK_NEW_API
	
	if (mFbxExportType.GetCurSel() != -1)
	{
		mFbxExportType.GetWindowText(fbxExportType);
		WritePrivateProfileString("Settings", "fbxExportType", fbxExportType, iniFile);

		overrideSkeleton = mOverrideSkeletonCheck.GetCheck();

		mOverrideSkeletonFilename.GetWindowText(overrideSkeletonFile);
		WritePrivateProfileString("Settings", "overrideSkeletonFile", overrideSkeletonFile, iniFile);
	}
	#endif

	

	bool specialKeywordMode = mSpecialKeywordMode.GetCheck();
	bool mergeLikeMaterials = mMergeLikeMaterials.GetCheck();
	bool renameMaterials = mRenameMaterialsToTextureNames.GetCheck();
	bool renameGroups = mRenameGroups.GetCheck();
	bool stripImagePaths = mStripMtlImagePaths.GetCheck();
	bool roundVertices = mRound.GetCheck();
	bool roundVerticesTenths = mRoundTenths.GetCheck();
	bool roundVerticesHundredths = mRoundHundredths.GetCheck();
	bool ignoreShading = mIgnoreShading.GetCheck();
	bool ignoreShadingPoint7 = mIgnorePoint7Shades.GetCheck();
	bool recenterObjects = mRecenterObjects.GetCheck();
	bool scaleVertices = mScaleVerticesCheck.GetCheck();
	bool noGroups = mCheckNoGroups.GetCheck();
	bool primarySecondaryGroups = mPrimarySecondaryGroups.GetCheck();
	bool useReplacementFile = mDoReplaceFile.GetCheck();
	bool fixKsKaNs = mCheckFixNiKaKsNs.GetCheck();
	bool scaleDiffuse = mCheckScaleDiffuseShading.GetCheck();
	bool scaleAmbient = mCheckScaleAmbientShading.GetCheck();
	bool scaleSpecular = mCheckScaleSpecularShading.GetCheck();
	bool mergeHierarchicalGroupsToEnd = mMergeHierarchicalGroupsToEnd.GetCheck();
	bool moveCoords = mCheckMovePoints.GetCheck();
	bool scaleAmbientFactorAn8 = mCheckScaleAmbientFactor.GetCheck();
	bool scaleDiffuseFactorAn8 = mCheckScaleDiffuseFactor.GetCheck();
	bool disableAutoshading = mDisableAutoshading.GetCheck();
	bool defaultShading = mDefaultShading.GetCheck();
	bool recalculateUVs = mRecalculateUVs.GetCheck();
	bool scaleUVs = mScaleUVs.GetCheck();
	bool specifyUVTileSize = mSpecifyTileSizeCheck.GetCheck();
	bool regexFilterCheck = mRegexGroupFilterCheck.GetCheck();
	bool sortTexturesWithinGroup = mSortTexturesWithinGroup.GetCheck();

	CString tempStr;
	mScaleDiffuseShade.GetWindowText(tempStr);
	float scaleDiffuseFactor = atof(tempStr);
	mScaleAmbientShade.GetWindowText(tempStr);
	float scaleAmbientFactor = atof(tempStr);
	mScaleSpecularShade.GetWindowText(tempStr);
	float scaleSpecularFactor = atof(tempStr);

	mXMove.GetWindowText(tempStr);
	float xMove = atof(tempStr);

	mYMove.GetWindowText(tempStr);
	float yMove = atof(tempStr);

	mZMove.GetWindowText(tempStr);
	float zMove = atof(tempStr);

	mScaleVerticesEdit.GetWindowText(tempStr);
	float scaleVerticesFactor = atof(tempStr);

	mTileSizeU.GetWindowText(tempStr);
	float texelSizeU = atof(tempStr);

	mTileSizeV.GetWindowText(tempStr);
	float texelSizeV = atof(tempStr);

	mScaleAmbientFactor.GetWindowText(tempStr);
	float scaleAmbientFactorAn8Value = atof(tempStr);
	mScaleDiffuseFactor.GetWindowText(tempStr);
	float scaleDiffuseFactorAn8Value = atof(tempStr);

	mScaleUVsEdit.GetWindowText(tempStr);
	float scaleUVsFactor = atof(tempStr);

	
	CString regexFilter;
	mRegexGroupFilter.GetWindowText(regexFilter);

	CString jointMode;
	mJointMode.GetWindowText(jointMode);
	jointMode.MakeLower();

	CString fpsString;
	mFBXFPS.GetWindowText(fpsString);
	float fps = atof(fpsString);
	if (fps < 0)
		fps = 30;

	bool outputObjVColors = mCheckOutputObjVColors.GetCheck();

	PerformConversion(specialKeywordMode, mergeLikeMaterials, renameMaterials, renameGroups, stripImagePaths, 
		roundVertices, roundVerticesTenths, roundVerticesHundredths, ignoreShading, ignoreShadingPoint7, recenterObjects, 
		scaleVertices, noGroups, primarySecondaryGroups, useReplacementFile, fixKsKaNs, 
		scaleDiffuse, scaleAmbient, scaleSpecular, mergeHierarchicalGroupsToEnd, moveCoords, 
		scaleAmbientFactorAn8, scaleDiffuseFactorAn8, disableAutoshading, defaultShading, 
		recalculateUVs, scaleUVs, specifyUVTileSize, 
		scaleDiffuseFactor, scaleAmbientFactor, scaleSpecularFactor, xMove, yMove, zMove, 
		scaleVerticesFactor, texelSizeU, texelSizeV, scaleUVsFactor, 
		scaleAmbientFactorAn8Value, scaleDiffuseFactorAn8Value, regexFilterCheck, regexFilter,
		inputFile, outputFile, replaceFile, fbxExportType, overrideSkeleton, overrideSkeletonFile, true, jointMode, fps, sortTexturesWithinGroup, outputObjVColors);
}

void CObjToAn8Dlg::ReadMaterialOverridesFile(std::vector<CMaterial*>& replacementMaterialOverrides, CString replaceFile)
{
	FILE* tempMaterial = fopen(replaceFile, "r");
	if (tempMaterial == NULL)
	{
		if (doMessageBoxes)
			MessageBox("Bad input file, can't open");
		return;
	}

	CString errorString = "";

	CMaterial* defaultMaterialAdd = new CMaterial();
	defaultMaterialAdd->name = "DefaultMaterial";
	replacementMaterialOverrides.push_back(defaultMaterialAdd);

	CMaterial* currentMaterial = defaultMaterialAdd;

	char currentCharLineTempSub[40000];

	while (!feof(tempMaterial))
	{
		for (int rz = 0; rz < 40000; rz++)
			currentCharLineTempSub[rz] = 0;
		fgets(currentCharLineTempSub, 40000, tempMaterial);
		for (int x = 0; x < 40000; x++)
		{
			if (currentCharLineTempSub[x] == 0xA)
				currentCharLineTempSub[x] = 0;
		}

		CString tempLineStrTempSub;
		tempLineStrTempSub.Format("%s", currentCharLineTempSub);

		if ((currentCharLineTempSub[0] == 'n') && (currentCharLineTempSub[1] == 'e') && (currentCharLineTempSub[2] == 'w') && (currentCharLineTempSub[3] == 'm') && (currentCharLineTempSub[4] == 't') && (currentCharLineTempSub[5] == 'l'))
		{
			CMaterial* newMaterial = new CMaterial();
			newMaterial->name = tempLineStrTempSub.Mid(7);

			replacementMaterialOverrides.push_back(newMaterial);

			currentMaterial = newMaterial;
		}
		else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 'a'))
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			currentMaterial->ambientColor.r = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->ambientColor.g = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->ambientColor.b = atof(pch);
			currentMaterial->ambientColor.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 'd')) // diffuse
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			currentMaterial->diffuseColor.r = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->diffuseColor.g = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->diffuseColor.b = atof(pch);
			currentMaterial->diffuseColor.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'K') && (currentCharLineTempSub[1] == 's'))
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			currentMaterial->spectralColor.r = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->spectralColor.g = atof(pch);
			pch = strtok (NULL, " ");
			currentMaterial->spectralColor.b = atof(pch);
			currentMaterial->spectralColor.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'd') || ((currentCharLineTempSub[0] == 'T') && (currentCharLineTempSub[1] == 'r')))
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			if (currentCharLineTempSub[0] == 'd')
				currentMaterial->alpha.floatValue = (atof(pch));
			else if ((currentCharLineTempSub[0] == 'T') && (currentCharLineTempSub[1] == 'r'))
				currentMaterial->alpha.floatValue = 1.0 - (atof(pch));
			currentMaterial->alpha.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'N') && (currentCharLineTempSub[1] == 's'))
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			currentMaterial->phongSpectral.floatValue = (atof(pch));
			currentMaterial->phongSpectral.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'i') && (currentCharLineTempSub[1] == 'l') && (currentCharLineTempSub[2] == 'l') && (currentCharLineTempSub[3] == 'u') && (currentCharLineTempSub[4] == 'm'))
		{
			char* pch = strtok (currentCharLineTempSub," ");
			pch = strtok (NULL, " ");
			currentMaterial->illum.floatValue = (atoi(pch));
			currentMaterial->illum.contains = true;
		}
		else if ((currentCharLineTempSub[0] == 'm') && (currentCharLineTempSub[1] == 'a') && (currentCharLineTempSub[2] == 'p') && (currentCharLineTempSub[3] == '_') && (currentCharLineTempSub[4] == 'K') && (currentCharLineTempSub[5] == 'd'))
		{
			currentMaterial->diffuseTexture.stringValue = tempLineStrTempSub.Mid(7);
			currentMaterial->diffuseTexture.contains = true;
		}
	}

	fclose(tempMaterial);

	if (errorString != "")
	{
		if (doMessageBoxes)
			MessageBox(errorString);
	}
}

void CObjToAn8Dlg::FixNiKsKaNs(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;
			material->spectralColor.contains = true;
			material->spectralColor.r = 1.0;
			material->spectralColor.g = 1.0;
			material->spectralColor.b = 1.0;
			material->ambientColor.contains = true;
			material->ambientColor.r = 1.0;
			material->ambientColor.g = 1.0;
			material->ambientColor.b = 1.0;
			material->phongSpectral.contains = true;
			material->phongSpectral.floatValue = 752.6;
			if (material->alpha.contains)
			{
				if (material->alpha.floatValue == 0.00)
					material->alpha.floatValue = 1.0;
			}
		}
	}
}

void CObjToAn8Dlg::ScaleDiffuseShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float diffuseScaling)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;
			if (material->diffuseColor.contains)
			{
				material->diffuseColor.r *= diffuseScaling;
				material->diffuseColor.g *= diffuseScaling;
				material->diffuseColor.b *= diffuseScaling;
			}
		}
	}
}

void CObjToAn8Dlg::ScaleSpecularShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float specularScaling)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;
			if (material->spectralColor.contains)
			{
				material->spectralColor.r *= specularScaling;
				material->spectralColor.g *= specularScaling;
				material->spectralColor.b *= specularScaling;
			}
		}
	}
}

void CObjToAn8Dlg::ScaleAmbientShading(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, float ambientScaling)
{
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::map<CString, CString> replacementStrings;
		std::vector<CMaterial*> removals;

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;
			if (material->ambientColor.contains)
			{
				material->ambientColor.r *= ambientScaling;
				material->ambientColor.g *= ambientScaling;
				material->ambientColor.b *= ambientScaling;
			}
		}
	}
}

void CObjToAn8Dlg::ReplaceMaterials(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, 
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool foundTextureUV, bool foundNormals, bool foundVerticeColors, std::vector<CMaterial*> replacementMaterialOverrides)
{
	CMaterial* defaultMaterial = NULL;

	std::vector<CMaterial*>::iterator	itermaterialFindAllRepl;
	for (itermaterialFindAllRepl = replacementMaterialOverrides.begin(); itermaterialFindAllRepl != replacementMaterialOverrides.end(); itermaterialFindAllRepl++)
	{
		CMaterial* materialRepl = (CMaterial*)*itermaterialFindAllRepl;
		if (materialRepl->name == "Default")
		{
			defaultMaterial = materialRepl;
			break;
		}
	}

	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			if (defaultMaterial != NULL)
			{
				if (defaultMaterial->alpha.contains)
				{
					material->alpha.contains = true;
					material->alpha.floatValue = defaultMaterial->alpha.floatValue;
				}

				if (defaultMaterial->ambientColor.contains)
				{
					material->ambientColor.contains = true;
					material->ambientColor.r = defaultMaterial->ambientColor.r;
					material->ambientColor.g = defaultMaterial->ambientColor.g;
					material->ambientColor.b = defaultMaterial->ambientColor.b;
				}

				if (defaultMaterial->diffuseColor.contains)
				{
					material->diffuseColor.contains = true;
					material->diffuseColor.r = defaultMaterial->diffuseColor.r;
					material->diffuseColor.g = defaultMaterial->diffuseColor.g;
					material->diffuseColor.b = defaultMaterial->diffuseColor.b;
				}

				if (defaultMaterial->spectralColor.contains)
				{
					material->spectralColor.contains = true;
					material->spectralColor.r = defaultMaterial->spectralColor.r;
					material->spectralColor.g = defaultMaterial->spectralColor.g;
					material->spectralColor.b = defaultMaterial->spectralColor.b;
				}

				if (defaultMaterial->phongSpectral.contains)
				{
					material->phongSpectral.contains = true;
					material->phongSpectral.floatValue = defaultMaterial->phongSpectral.floatValue;
				}

				if (defaultMaterial->illum.contains)
				{
					material->illum.contains = true;
					material->illum.floatValue = defaultMaterial->illum.floatValue;
				}

				if (defaultMaterial->diffuseTexture.contains)
				{
					material->diffuseTexture.contains = true;
					material->diffuseTexture.stringValue = defaultMaterial->diffuseTexture.stringValue;
				}
			}
			

			std::vector<CMaterial*>::iterator	itermaterialRepl;
			for (itermaterialRepl = replacementMaterialOverrides.begin(); itermaterialRepl != replacementMaterialOverrides.end(); itermaterialRepl++)
			{
				CMaterial* materialRepl = (CMaterial*)*itermaterialRepl;

				if (
					((material->diffuseTexture.contains) && (material->diffuseTexture.stringValue == materialRepl->name))
					|| 
					((!material->diffuseTexture.contains) && (material->name == materialRepl->name))
					|| 
					((!material->diffuseTexture.contains) && (materialRepl->name == "UNTEXTUREDALL"))
					)
				{

					if ((!material->diffuseTexture.contains) && (materialRepl->name == "UNTEXTUREDALL"))
					{
						materialRepl->name = materialRepl->name;
					}
					if (materialRepl->alpha.contains)
					{
						material->alpha.contains = true;
						material->alpha.floatValue = materialRepl->alpha.floatValue;
					}

					if (materialRepl->ambientColor.contains)
					{
						material->ambientColor.contains = true;
						material->ambientColor.r = materialRepl->ambientColor.r;
						material->ambientColor.g = materialRepl->ambientColor.g;
						material->ambientColor.b = materialRepl->ambientColor.b;
					}

					if (materialRepl->diffuseColor.contains)
					{
						material->diffuseColor.contains = true;
						material->diffuseColor.r = materialRepl->diffuseColor.r;
						material->diffuseColor.g = materialRepl->diffuseColor.g;
						material->diffuseColor.b = materialRepl->diffuseColor.b;
					}

					if (materialRepl->spectralColor.contains)
					{
						material->spectralColor.contains = true;
						material->spectralColor.r = materialRepl->spectralColor.r;
						material->spectralColor.g = materialRepl->spectralColor.g;
						material->spectralColor.b = materialRepl->spectralColor.b;
					}

					if (materialRepl->phongSpectral.contains)
					{
						material->phongSpectral.contains = true;
						material->phongSpectral.floatValue = materialRepl->phongSpectral.floatValue;
					}

					if (materialRepl->illum.contains)
					{
						material->illum.contains = true;
						material->illum.floatValue = materialRepl->illum.floatValue;
					}

					if (materialRepl->diffuseTexture.contains)
					{
						material->diffuseTexture.contains = true;
						material->diffuseTexture.stringValue = materialRepl->diffuseTexture.stringValue;
					}
					break;
				}
			}
		}
	}
}

void CObjToAn8Dlg::DisposeStructures(std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CMaterial*>& replacementMaterialOverrides, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations)
{
	// Cleanup

	std::vector<CVerticeColor*>::iterator	iterVerticeColor;
	for (iterVerticeColor = verticeColors.begin(); iterVerticeColor != verticeColors.end(); iterVerticeColor++)
	{
		delete *iterVerticeColor;
	}

	std::vector<CNormal*>::iterator	iterNormal;
	for (iterNormal = normals.begin(); iterNormal != normals.end(); iterNormal++)
	{
		delete *iterNormal;
	}

	std::vector<CUVCoordinate*>::iterator	iteruvCoordinates;
	for (iteruvCoordinates = uvCoordinates.begin(); iteruvCoordinates != uvCoordinates.end(); iteruvCoordinates++)
	{
		delete *iteruvCoordinates;
	}

	std::vector<CVertice*>::iterator	itervertices;
	for (itervertices = vertices.begin(); itervertices != vertices.end(); itervertices++)
	{
		delete *itervertices;
	}

	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
			for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
			{
				delete *iterpolygonpoint;
			}
			delete polygon;
		}
		delete group;
	}


	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			delete *itermaterial;
		}

		delete materialFile;
	}

	std::vector<CMaterial*>::iterator	itermaterialRepl;
	for (itermaterialRepl = replacementMaterialOverrides.begin(); itermaterialRepl != replacementMaterialOverrides.end(); itermaterialRepl++)
	{
		delete *itermaterialRepl;
	}

	std::vector<CJoint*>::iterator	iterjoint;
	for (iterjoint = joints.begin(); iterjoint != joints.end(); iterjoint++)
	{
		delete *iterjoint;
	}

	std::vector<CAnimation*>::iterator	iteranim;
	for (iteranim = animations.begin(); iteranim != animations.end(); iteranim++)
	{
		CAnimation* animation = ((CAnimation*)(*iteranim));

		std::vector<CKeyframe*>::iterator	iterkeyframe;
		for (iterkeyframe = animation->keyframes.begin(); iterkeyframe != animation->keyframes.end(); iterkeyframe++)
		{
			CKeyframe* keyframe = ((CKeyframe*)(*iterkeyframe));
			
			std::vector<CAnimationPart*>::iterator	iteranimationPart;
			for (iteranimationPart = keyframe->animationParts.begin(); iteranimationPart != keyframe->animationParts.end(); iteranimationPart++)
			{
				CAnimationPart* animationPart = ((CAnimationPart*)(*iteranimationPart));

				delete *iteranimationPart;
			}

			delete *iterkeyframe;
		}

		delete *iteranim;
	}
}
					
CString CObjToAn8Dlg::PullOutImageName(CString fullPath)
{
	int startSpot = 0;
	if (fullPath.ReverseFind('\\') != -1)
		startSpot = fullPath.ReverseFind('\\') + 1;

	int endSpot = fullPath.GetLength();
	if (fullPath.ReverseFind('.') != -1)
		endSpot = fullPath.ReverseFind('.');

	return (fullPath.Mid(startSpot, (endSpot - startSpot)));
}

unsigned char CObjToAn8Dlg::ColorFloatToHex(float color)
{
	float result = color * 255.0f;
	if (result >= 255.0f)
		return 0xFF;
	else if (result <= 0)
		return 0;
	else return ((unsigned char)(Round(color * 255.0f)));
}

bool CObjToAn8Dlg::IsSpecialKeywordEnvMapping(bool specialKeywordMode, CMaterial* material)
{
	if (material->diffuseTexture.contains)
	{
		if (material->diffuseTexture.stringValue.Find("EnvMapping") != -1)
			return true;
	}
	if (material->name.Find("EnvMapping") != -1)
		return true;

	return false;
}

bool CObjToAn8Dlg::IsSpecialKeyword(bool specialKeywordMode, CMaterial* material)
{
	if ((material->alpha.contains) && (material->alpha.floatValue < 1) && (material->alpha.floatValue > 0))
		return true;
	if (material->diffuseTexture.contains)
	{
		if (material->diffuseTexture.stringValue.Find("Transparent") != -1)
			return true;
		if (material->diffuseTexture.stringValue.Find("Decal") != -1)
			return true;
	}
	if (material->name.Find("Transparent") != -1)
		return true;
	if (material->name.Find("Decal") != -1)
		return true;

	return false;
}

CString CObjToAn8Dlg::PrintHexColorToString(float r, float g, float b)
{
	CString tempStr;
	tempStr.Format("%d %d %d", ColorFloatToHex(r), ColorFloatToHex(g), ColorFloatToHex(b));
	return tempStr;
}

void CObjToAn8Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

std::vector<CJoint*> CObjToAn8Dlg::FindRootJoints(std::vector<CJoint*>& joints)
{
	std::vector<CJoint*> rootJoints;
	for (int x = 0; x < joints.size(); x++)
	{
		if (joints[x]->parent == NULL)
		{
			if (!joints[x]->isShapeAnimationJoint)
			{
				//if ((StringHasNumber(joints[x]->name)) || (joints[x]->name.Find("TopJoint") != -1))
				rootJoints.push_back(joints[x]);
			}
		}
	}

	return rootJoints;
}


#ifdef FBXSDK_NEW_API
const char* CObjToAn8Dlg::GetMaterialTextureName(const FbxSurfaceMaterial * pMaterial,
        const char * pPropertyName)
    {
		const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
        if (lProperty.IsValid())
        {
            const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
            if (lTextureCount)
            {
                const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* fileName = lTexture->GetFileName();
				return fileName;
            }
        }
		return NULL;
    }

FbxDouble3 CObjToAn8Dlg::GetMaterialColor(const FbxSurfaceMaterial * pMaterial,
        const char * pPropertyName,
        const char * pFactorPropertyName)
    {
        FbxDouble3 lResult(0, 0, 0);
        const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
        const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
        if (lProperty.IsValid() && lFactorProperty.IsValid())
        {
            lResult = lProperty.Get<FbxDouble3>();
            double lFactor = lFactorProperty.Get<FbxDouble>();
            if (lFactor != 1)
            {
                lResult[0] *= lFactor;
                lResult[1] *= lFactor;
                lResult[2] *= lFactor;
            }
        }

        return lResult;
    }

bool CObjToAn8Dlg::ReadFbxFile(CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
		bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors,
		bool noGroups, bool overrideSkeleton, CString overrideSkeletonFile, JointType jointType)
{
	CString currentFolder = inputFile.Mid(0, ((inputFile.ReverseFind('\\')+1)));

	char  pFbxFilePath[1000];
	strcpy(pFbxFilePath,inputFile);

	FbxManager* pSdkManager = NULL;
	FbxScene*      pScene      = NULL;

	// Create the FBX SDK memory manager object.
    // The SDK Manager allocates and frees memory
    // for almost all the classes in the SDK.
    pSdkManager = FbxManager::Create();

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(pSdkManager, IOSROOT );
	pSdkManager->SetIOSettings(ios);

	pScene = FbxScene::Create(pSdkManager,"");

	// Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pSdkManager,"");

    // Initialize the importer by providing a filename.
    bool lImportStatus = lImporter->Initialize(pFbxFilePath, -1, pSdkManager->GetIOSettings() );

    if( !lImportStatus )
    {
        // Destroy the importer
        lImporter->Destroy();
        return false;
    }

    if (lImporter->IsFBX())
    {
        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene
    bool lStatus = lImporter->Import(pScene);
	
	try
	{
		FbxAxisSystem lAxisSystemReference = pScene->GetGlobalSettings().GetAxisSystem();
		int lUpVectorSign = 1;
		int lFrontVectorSign = 1;

		FbxAxisSystem::EUpVector lUpVector = lAxisSystemReference.GetUpVector( lUpVectorSign );
		//get FrontVector and its sign.
		FbxAxisSystem::EFrontVector lFrontVector = lAxisSystemReference.GetFrontVector( lFrontVectorSign );
		//get uCoorSystem. 
		FbxAxisSystem::ECoordSystem lCoorSystem = lAxisSystemReference.GetCoorSystem();

		if (
			(lUpVector != FbxAxisSystem::EUpVector::eYAxis)
			|| (lFrontVector != FbxAxisSystem::EFrontVector::eParityOdd)
			|| (lCoorSystem != FbxAxisSystem::ECoordSystem::eRightHanded))
		{
			MessageBox("WARNING: FBX Format is not Y-up, Z-forward, and Right Handed.  It must be these axes for correct conversion.");
		}

		FbxNode* rootNode = pScene->GetRootNode();

		std::string rootName = rootNode->GetName();

		map<CString, float3> skeletalOverrides;
		map<CString, float3> skeletalOverridesRelativeScale;
		map<CString, float3> skeletalOverridesRelativeRotation;
		map<CString, float3> skeletalOverridesRelativePosition;

		if (overrideSkeleton)
		{
			JointType jointType = Absolute;
			FILE* inSkeletonFile = fopen(overrideSkeletonFile, "r");
			if (inSkeletonFile == NULL)
			{
				if (doMessageBoxes)
					MessageBox("Can't open skeletal override file");
			}
			else
			{
				char currentLine[40000];

				CString lastJointName = "";
				while (!feof(inSkeletonFile))
				{
					for (int rz = 0; rz < 40000; rz++)
						currentLine[rz] = 0;

					fgets(currentLine, 40000, inSkeletonFile);

					for (int x = 0; x < 40000; x++)
					{
						if (currentLine[x] == 0xA)
							currentLine[x] = 0;
					}

					CString lineString;
					lineString.Format("%s", currentLine);

					lineString = lineString.Trim();
					if (lineString.Find("#jointmode ") == 0)
					{
						CString jointModeRead = lineString.Mid(11).Trim().MakeLower();
						if (jointModeRead == "relative")
						{
							jointType = Relative;							
						}
					}
					else if (lineString.Find("#joint ") == 0)
					{
						lastJointName = lineString.Mid(7);

						if (lastJointName.Find(":") != -1)
						{
							lastJointName = lastJointName.Mid(lastJointName.Find(":") + 1);
						}

						if (lastJointName.Find("_") == 0)
							lastJointName = lastJointName.Mid(1);
					}
					else if (lineString.Find("#jointposition ") == 0)
					{
						if (jointType == Absolute)
						{
							if (lastJointName != "")
							{
								float3 jointPositionOverride(0,0,0);

								char* pch = strtok (currentLine," ");
								pch = strtok (NULL, " ");
								jointPositionOverride.x = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointPositionOverride.y = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointPositionOverride.z = (((atof(pch))));

								skeletalOverrides[lastJointName] = jointPositionOverride;
							}
						}
					}
					else if (lineString.Find("#jointsrt ") == 0)
					{
						if (jointType == Relative)
						{
							if (lastJointName != "")
							{
								float3 jointRelativeScaleOverride(0,0,0);
								float3 jointRelativeRotationOverride(0,0,0);
								float3 jointRelativePositionOverride(0,0,0);

								char* pch = strtok (currentLine," ");
								pch = strtok (NULL, " ");
								jointRelativeScaleOverride.x = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativeScaleOverride.y = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativeScaleOverride.z = (((atof(pch))));

								skeletalOverridesRelativeScale[lastJointName] = jointRelativeScaleOverride;

								pch = strtok (NULL, " ");
								jointRelativeRotationOverride.x = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativeRotationOverride.y = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativeRotationOverride.z = (((atof(pch))));

								skeletalOverridesRelativeRotation[lastJointName] = jointRelativeRotationOverride;

								pch = strtok (NULL, " ");
								jointRelativePositionOverride.x = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativePositionOverride.y = (((atof(pch))));
								pch = strtok (NULL, " ");
								jointRelativePositionOverride.z = (((atof(pch))));

								skeletalOverridesRelativePosition[lastJointName] = jointRelativePositionOverride;
							}
						}
					}
				}

				fclose(inSkeletonFile);
			}
		}

		ParseFbxSkeletonRecursive(rootNode, joints, float3(0,0,0), NULL, skeletalOverrides, skeletalOverridesRelativeScale, skeletalOverridesRelativeRotation, skeletalOverridesRelativePosition, jointType);

		CGroup* defaultGroup = new CGroup();
		defaultGroup->name = "DefaultUnassigned";

		groups.push_back(defaultGroup);

		CGroup* currentGroup = defaultGroup;

		CString errorString = "";

		ParseFbxNodeRecursive(rootNode, currentGroup, inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, noGroups, errorString, jointType);

		if (errorString != "")
		{
			if (doMessageBoxes)
				MessageBox(errorString);
		}

		int animationStackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
		//for (int i = 0; i < animationStackCount; i++)
		if (animationStackCount > 0)
		{
			for (int stack = 0; stack < animationStackCount; stack++)
			{
				FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(stack);

				CString name = lAnimStack->GetName();

				int nbAnimLayers = lAnimStack->GetMemberCount<FbxAnimLayer>();

				if (nbAnimLayers > 0)
				{
					// TODO maybe later do multi-layer
					FbxAnimLayer* lAnimLayer = lAnimStack->GetMember<FbxAnimLayer>(0);
					CString animLayerName = lAnimLayer->GetName();
					
					ParseFbxCameraRecursive(lAnimLayer, pScene->GetRootNode(), joints, animations);

					if (joints.size() > 0)
					{
						int numberKeyframes = 0;
						GetNumberKeyframesFbxAnimationRecursive(lAnimLayer, pScene->GetRootNode(), numberKeyframes, joints);

						if (numberKeyframes > 0)
						{
							CAnimation* animation;
							if (joints.size() == 0)
							{
								// assume camera
								animation = new CCamera();
							}
							else
							{
								animation = new CAnimation();
							}
							animation->name = name;

							// add initial in order
							for (int x = 0; x < numberKeyframes; x++)
							{
								GetAddKeyframe(x, animation);
							}

							ParseFbxAnimationRecursive(lAnimLayer, pScene->GetRootNode(), joints, animation, numberKeyframes);

							bool containsBlendShape = false;
							ContainsFbxBlendShapeRecursive(lAnimLayer, pScene->GetRootNode(), containsBlendShape);

							if (containsBlendShape)
							{
								int verticesOffset = 0;
								ParseFbxBlendShapePartRecursive(lAnimLayer, pScene->GetRootNode(), vertices, groups, joints, animation, noGroups, verticesOffset);
							}

							animations.push_back(animation);
						}
					}
					else
					{
						bool containsBlendShape = false;
						ContainsFbxBlendShapeRecursive(lAnimLayer, pScene->GetRootNode(), containsBlendShape);

						if (containsBlendShape)
						{
							CAnimation* animation;
							animation = new CAnimation();
							animation->name = name;
							animation->isCamera = false;
							animation->treatAsBlendShape = true;
							animations.push_back(animation);

							// Add all the joints for each vertex
							for (int x = 0; x < vertices.size(); x++)
							{
								CJoint* joint = new CJoint();
								joint->name.Format("%d", x + 1);
								joint->controlPoints.push_back(x);
								joint->parent = NULL;
								joint->positionAbsolute = vertices[x]->vertex;
								joints.push_back(joint);
							}

							int verticesOffset = 0;
							ParseFbxBlendShapeRecursive(lAnimLayer, pScene->GetRootNode(), vertices, groups, joints, animation, noGroups, verticesOffset);
						}
					}
				}
			}
		}

		// Destroy the importer
		lImporter->Destroy();

		pScene->Destroy();

		if (pSdkManager) pSdkManager->Destroy();

		return true;
	} catch (...)
	{
	    // Destroy the importer
		lImporter->Destroy();

		pScene->Destroy();

		if (pSdkManager) pSdkManager->Destroy();

		if (doMessageBoxes)
			MessageBox("Error converting", "Error");
		return false;
	}
}


// Create material.
// FBX scene must connect materials FbxNode, otherwise materials will not be exported.
// FBX document don't need connect materials to FbxNode, it can export standalone materials.
FbxSurfacePhong* CObjToAn8Dlg::CreateMaterial(FbxManager* pSdkManager, CString textureName)
{
    FbxString lMaterialName = "material";
    FbxString lShadingName  = "Phong";
    FbxDouble3 lBlack(0.0, 0.0, 0.0);
    FbxDouble3 lRed(1.0, 0.0, 0.0);
    FbxDouble3 lDiffuseColor(0.75, 0.75, 0.0);
    FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(pSdkManager, lMaterialName.Buffer());

    // Generate primary and secondary colors.
    lMaterial->Emissive      .Set(lBlack);
    lMaterial->Ambient       .Set(lRed);
    lMaterial->AmbientFactor .Set(1.);
    // Add texture for diffuse channel
    lMaterial->Diffuse       .ConnectSrcObject(CreateTexture(pSdkManager, textureName));
    lMaterial->DiffuseFactor .Set(1.);
    lMaterial->TransparencyFactor  .Set(0.4);
    lMaterial->ShadingModel        .Set(lShadingName);
    lMaterial->Shininess           .Set(0.5);
    lMaterial->Specular            .Set(lBlack);
    lMaterial->SpecularFactor      .Set(0.3);

    return lMaterial;
}


// Create a texture
FbxTexture*  CObjToAn8Dlg::CreateTexture(FbxManager* pSdkManager, CString texturePath)
{
    FbxFileTexture* lTexture = FbxFileTexture::Create(pSdkManager,"");

    FbxString lTexPath = texturePath;

    // Set texture properties.
    lTexture->SetFileName(lTexPath.Buffer());
    lTexture->SetName("Diffuse Texture");
    lTexture->SetTextureUse(FbxTexture::eStandard);
    lTexture->SetMappingType(FbxTexture::eUV);
    lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    lTexture->SetSwapUV(false);
    lTexture->SetAlphaSource (FbxTexture::eNone);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);

    return lTexture;
}



void CObjToAn8Dlg::WriteFbxSkeleton(std::map<CString, FbxCluster*>& jointCluster, std::map<CString, FbxNode*>& skeletonCluster, FbxScene* pScene, CJoint* joint, FbxNode* skeletonNode)
{
	FbxSkeleton* lSkeletonLimbNodeAttribute = FbxSkeleton::Create(pScene, joint->name);
    lSkeletonLimbNodeAttribute->SetSkeletonType(FbxSkeleton::eLimb);
    lSkeletonLimbNodeAttribute->Size.Set(1.0);
    FbxNode* lSkeletonLimbNode = FbxNode::Create(pScene, joint->name);
    lSkeletonLimbNode->SetNodeAttribute(lSkeletonLimbNodeAttribute);    
	if (joint->rotationOrder == "XZY")
		lSkeletonLimbNode->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderXZY);
	else if (joint->rotationOrder == "YZX")
		lSkeletonLimbNode->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYZX);
	else if (joint->rotationOrder == "YXZ")
		lSkeletonLimbNode->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYXZ);
	else if (joint->rotationOrder == "ZXY")
		lSkeletonLimbNode->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZXY);
	else if (joint->rotationOrder == "ZYX")
		lSkeletonLimbNode->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZYX);
	// XYZ default

	if (joint->jointType == Absolute)
	{
		if (joint->parent != NULL)
		{
			lSkeletonLimbNode->LclTranslation.Set(FbxVector4((joint->positionAbsolute.x - joint->parent->positionAbsolute.x), (joint->positionAbsolute.y - joint->parent->positionAbsolute.y), (joint->positionAbsolute.z - joint->parent->positionAbsolute.z)));
		}
		else
		{
			lSkeletonLimbNode->LclTranslation.Set(FbxVector4(joint->positionAbsolute.x, joint->positionAbsolute.y, joint->positionAbsolute.z));
		}
	}
	else if (joint->jointType == Relative)
	{
		lSkeletonLimbNode->LclScaling.Set(FbxVector4(joint->scaleRelative.x, joint->scaleRelative.y, joint->scaleRelative.z));
		lSkeletonLimbNode->LclRotation.Set(FbxVector4(joint->rotationRelative.x, joint->rotationRelative.y, joint->rotationRelative.z));
		lSkeletonLimbNode->LclTranslation.Set(FbxVector4(joint->positionRelative.x, joint->positionRelative.y, joint->positionRelative.z));
	}

	skeletonNode->AddChild(lSkeletonLimbNode);

	skeletonCluster[joint->name] = lSkeletonLimbNode;

	for (int x = 0; x < joint->children.size(); x++)
	{
		WriteFbxSkeleton(jointCluster, skeletonCluster, pScene, joint->children[x], lSkeletonLimbNode);
	}
}

void CObjToAn8Dlg::WriteFbxFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, CString inputPath, bool regexFilterCheck, CString regexFilter, CString fbxExportType, float fps, bool sortByTextureWithinGroup)
{
	FbxManager* pSdkManager = FbxManager::Create();

    // create an IOSettings object
    FbxIOSettings * ios = FbxIOSettings::Create(pSdkManager, IOSROOT );
    pSdkManager->SetIOSettings(ios);

	// create the main document
    FbxScene* pScene = FbxScene::Create(pSdkManager,"RootDoc");

	int lCount;

    // create document info
    FbxDocumentInfo* lDocInfo = FbxDocumentInfo::Create(pSdkManager,"DocInfo");
    lDocInfo->mTitle = "Converted Fbx";
    lDocInfo->mSubject = "Converted Fbx using Obj2An8";
    lDocInfo->mAuthor = "SubDrag";
    lDocInfo->mRevision = "Rev. 1.0";
    lDocInfo->mKeywords = "Fbx document";
    lDocInfo->mComment = "";

    // add the documentInfo
    pScene->SetDocumentInfo(lDocInfo);

	if (fps == 24.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames24);
	else if (fps == 30.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames30);
	else if (fps == 48)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames48);
	else if (fps == 50.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames50);
	else if (fps == 59.94)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames59dot94);
	else if (fps == 60.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames60);
	else if (fps == 72.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames72);
	else if (fps == 96)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames96);
	else if (fps == 100.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames100);
	else if (fps == 120.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames120);
	else if (fps == 1000.0)
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames1000);
	else
	{
		pScene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eCustom);
		pScene->GetGlobalSettings().SetCustomFrameRate(fps);
	}

	int x = 0;
	if (animations[x]->keyframes.size() > 0)
	{
		int y = 0;
		{
			for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
			{
				for (int j = 0; j < joints.size(); j++)
				{
					if (animations[x]->keyframes[y]->animationParts[w]->name == joints[j]->name)
					{
						if (animations[x]->keyframes[y]->animationParts[w]->shapeVertices.size() > 0)
						{
							joints[j]->isShapeAnimationJoint = true;
							break;
						}
					}
				}
			}
		}
	}
			
	FbxNode* lMainNode = pScene->GetRootNode();

	std::map<CString, FbxCluster*> jointCluster;
	std::map<CString, FbxNode*> jointSkeleton;

	FbxNode* lTopJointNode = NULL;
	FbxNode* lSkeletonRoot = NULL;

	bool allCamera = false;
	bool allBlendShape = false;

	if (joints.size() > 0)
	{
		if (animations.size() > 0)
		{
			allCamera = true;
			allBlendShape = true;

			for (int x = 0; x < animations.size(); x++)
			{
				FbxNode* cameraNode = NULL;
				if (!animations[x]->isCamera)
				{
					allCamera = false;
				}
				if (!animations[x]->treatAsBlendShape)
				{
					allBlendShape = false;
				}
			}
		}

		if (animations.size() == 0)
		{
			allCamera = false;
		}

		if (!allCamera && !allBlendShape)
		{
			std::vector<CJoint*> rootJoints = FindRootJoints(joints);

			if (rootJoints.size() == 0)
			{
				// add one instead
				lSkeletonRoot = NULL;
				if (doMessageBoxes)
					MessageBox("Error, no root nodes");
			}
			else if (rootJoints.size() == 1)
			{
				FbxSkeleton* lSkeletonRootAttributeTopJoint = FbxSkeleton::Create(pScene, "TopJoint");
				lSkeletonRootAttributeTopJoint->SetSkeletonType(FbxSkeleton::eLimb);
				FbxNode* lSkeletonRootTopJoint = FbxNode::Create(pScene, "TopJoint");
				lSkeletonRootTopJoint->SetNodeAttribute(lSkeletonRootAttributeTopJoint);
				if (rootJoints[0]->rotationOrder == "XZY")
					lSkeletonRootTopJoint->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderXZY);
				else if (rootJoints[0]->rotationOrder == "YZX")
					lSkeletonRootTopJoint->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYZX);
				else if (rootJoints[0]->rotationOrder == "YXZ")
					lSkeletonRootTopJoint->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYXZ);
				else if (rootJoints[0]->rotationOrder == "ZXY")
					lSkeletonRootTopJoint->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZXY);
				else if (rootJoints[0]->rotationOrder == "ZYX")
					lSkeletonRootTopJoint->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZYX);
				// XYZ default
				lMainNode->AddChild(lSkeletonRootTopJoint);

				lTopJointNode = lSkeletonRootTopJoint;

				jointSkeleton["TopJoint"] = lSkeletonRootTopJoint;

				// just one, if not present, add top
				CJoint* rootJoint = rootJoints[0];
				if (rootJoint->name != "TopJoint")
				{
					FbxSkeleton* lSkeletonRootAttribute = FbxSkeleton::Create(pScene, rootJoint->name);
					lSkeletonRootAttribute->SetSkeletonType(FbxSkeleton::eLimb);
					lSkeletonRootAttribute->Size.Set(1.0);
					lSkeletonRoot = FbxNode::Create(pScene, rootJoint->name);
					lSkeletonRoot->SetNodeAttribute(lSkeletonRootAttribute);
					lSkeletonRootTopJoint->AddChild(lSkeletonRoot);

					jointSkeleton[rootJoint->name] = lSkeletonRoot;

					if (rootJoint->jointType == Absolute)
					{
						lSkeletonRoot->LclTranslation.Set(FbxVector4(rootJoint->positionAbsolute.x, rootJoint->positionAbsolute.y, rootJoint->positionAbsolute.z));
					}
					else if (rootJoint->jointType == Relative)
					{
						lSkeletonRoot->LclScaling.Set(FbxVector4(rootJoint->scaleRelative.x, rootJoint->scaleRelative.y, rootJoint->scaleRelative.z));
						lSkeletonRoot->LclRotation.Set(FbxVector4(rootJoint->rotationRelative.x, rootJoint->rotationRelative.y, rootJoint->rotationRelative.z));
						lSkeletonRoot->LclTranslation.Set(FbxVector4(rootJoint->positionRelative.x, rootJoint->positionRelative.y, rootJoint->positionRelative.z));
					}
				}
				else
				{
					lSkeletonRoot = lSkeletonRootTopJoint;
				}

				for (int y = 0; y < rootJoint->children.size(); y++)
				{
					WriteFbxSkeleton(jointCluster, jointSkeleton, pScene, rootJoint->children[y], lSkeletonRoot);
				}
			}
			else
			{
				// multiple, need to write top level one
				FbxSkeleton* lSkeletonRootAttributeTopJoint = FbxSkeleton::Create(pScene, "TopJoint");
				lSkeletonRootAttributeTopJoint->SetSkeletonType(FbxSkeleton::eLimb);
				lSkeletonRootAttributeTopJoint->Size.Set(1.0);
				FbxNode* lSkeletonRootTopJoint = FbxNode::Create(pScene, "TopJoint");
				lSkeletonRootTopJoint->SetNodeAttribute(lSkeletonRootAttributeTopJoint);
				// XYZ default
				lMainNode->AddChild(lSkeletonRootTopJoint);

				lTopJointNode = lSkeletonRootTopJoint;

				jointSkeleton["TopJoint"] = lSkeletonRootTopJoint;

				for (int x = 0; x < rootJoints.size(); x++)
				{
					CJoint* rootJoint = rootJoints[x];

					FbxSkeleton* lSkeletonRootAttribute = FbxSkeleton::Create(pScene, rootJoint->name);
					lSkeletonRootAttribute->SetSkeletonType(FbxSkeleton::eLimb);
					lSkeletonRootAttribute->Size.Set(1.0);
					lSkeletonRoot = FbxNode::Create(pScene, rootJoint->name);
					lSkeletonRoot->SetNodeAttribute(lSkeletonRootAttribute);
					if (rootJoint->rotationOrder == "XZY")
						lSkeletonRoot->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderXZY);
					else if (rootJoint->rotationOrder == "YZX")
						lSkeletonRoot->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYZX);
					else if (rootJoint->rotationOrder == "YXZ")
						lSkeletonRoot->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderYXZ);
					else if (rootJoint->rotationOrder == "ZXY")
						lSkeletonRoot->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZXY);
					else if (rootJoint->rotationOrder == "ZYX")
						lSkeletonRoot->SetRotationOrder(FbxNode::eSourcePivot, FbxEuler::eOrderZYX);
					// XYZ default
					lSkeletonRootTopJoint->AddChild(lSkeletonRoot);

					jointSkeleton[rootJoints[x]->name] = lSkeletonRoot;

					if (rootJoint->jointType == Absolute)
					{
						lSkeletonRoot->LclTranslation.Set(FbxVector4(rootJoints[x]->positionAbsolute.x, rootJoints[x]->positionAbsolute.y, rootJoints[x]->positionAbsolute.z));
					}
					else if (rootJoint->jointType == Relative)
					{
						lSkeletonRoot->LclScaling.Set(FbxVector4(rootJoints[x]->scaleRelative.x, rootJoints[x]->scaleRelative.y, rootJoints[x]->scaleRelative.z));
						lSkeletonRoot->LclRotation.Set(FbxVector4(rootJoints[x]->rotationRelative.x, rootJoints[x]->rotationRelative.y, rootJoints[x]->rotationRelative.z));
						lSkeletonRoot->LclTranslation.Set(FbxVector4(rootJoints[x]->positionRelative.x, rootJoints[x]->positionRelative.y, rootJoints[x]->positionRelative.z));
					}

					for (int y = 0; y < rootJoints[x]->children.size(); y++)
					{
						WriteFbxSkeleton(jointCluster, jointSkeleton, pScene, rootJoints[x]->children[y], lSkeletonRoot);
					}
				}
			}
		}
	}


	std::vector<CMaterialFile*>::iterator	itermaterialFiles;

	FbxString lMaterialNameDefault = "DefaultMaterial";
	FbxSurfacePhong* defaultMaterialDefinition = FbxSurfacePhong::Create(pSdkManager, lMaterialNameDefault.Buffer());

	defaultMaterialDefinition->Diffuse.Set(FbxDouble3(1.0, 1.0, 1.0));
	defaultMaterialDefinition->DiffuseFactor.Set(1.0);
	defaultMaterialDefinition->Ambient.Set(FbxDouble3(1.0, 1.0, 1.0));
	defaultMaterialDefinition->AmbientFactor.Set(1.0);
	defaultMaterialDefinition->Specular.Set(FbxDouble3(1.0, 1.0, 1.0));
	defaultMaterialDefinition->SpecularFactor.Set(1.0);
	
	FbxPose* lPose = FbxPose::Create(pScene,"BindPose");
	lPose->SetIsBindPose(true);

	std::vector<FbxNode*> addedNodes;

	map<CString, FbxNode*>::iterator iteratorJointSkeleton;
	for (iteratorJointSkeleton = jointSkeleton.begin(); iteratorJointSkeleton != jointSkeleton.end(); iteratorJointSkeleton++)
	{
		FbxNode* parentNode = iteratorJointSkeleton->second;
		while (parentNode != NULL)
		{
			bool foundAlready = false;
			for (int r = 0; r < addedNodes.size(); r++)
			{
				if (parentNode == addedNodes[r])
				{
					foundAlready = true;
					break;
				}
			}

			if (!foundAlready)
			{
				FbxMatrix lTransformMatrix = parentNode->EvaluateGlobalTransform();
				lPose->Add(parentNode, lTransformMatrix, false);

				addedNodes.push_back(parentNode);
			}

			//parentNode = parentNode->GetParent();
			break;
		}
	}

	std::map<CString, FbxSurfacePhong*> usedMaterialList;

	std::map<CGroup*, FbxMesh*> meshGrouplookup;
	std::map<CGroup*, std::map<int, int>> originalToSubVerticeControlPointMapping;

	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;

		if (regexFilterCheck)
		{
			std::tr1::regex rx(regexFilter.GetBuffer());
			std::string stdString = std::string(group->name.GetBuffer());
			regexFilter.ReleaseBuffer();

			bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
			group->name.ReleaseBuffer();

			if (!isMatch)
			{
				continue;
			}
		}

		for (int x = 0; x < animations.size(); x++)
		{
			if (!animations[x]->treatAsBlendShape)
			{
				int jointMatch = -1;
				std::vector<CPolygon*>::iterator	iterpolygon;
				for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
				{
					CPolygon* polygon = ((CPolygon*)(*iterpolygon));

					std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
					for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
					{
						CPolygonPoint* polygonPoint = (CPolygonPoint*)*iterpolygonpoint;

						CVertice* vertice = NULL;
						if ((polygonPoint->verticeIndex != -1) && (polygonPoint->verticeIndex < vertices.size()))
							vertice = vertices[polygonPoint->verticeIndex];

						if (vertice != NULL)
						{
							for (int z = 0; z < joints.size(); z++)
							{
								if (joints[z]->isShapeAnimationJoint)
								{
									for (int y = 0; y < joints[z]->controlPoints.size(); y++)
									{
										CVertice* controlVertice = vertices[joints[z]->controlPoints[y]];
										if (controlVertice == vertice)
										{
											jointMatch = z;
											break;
										}
									}
								}
							}
						}
					}
				}

				if (jointMatch != -1)
				{
					CString tempFormatStr;
					tempFormatStr.Format("_JointAssociation%s", joints[jointMatch]->name);
					group->name += tempFormatStr;
					break;
				}
			}
		}

		CString defaultMaterial = "DefaultMaterial";

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			defaultMaterial = polygon->materialName;
			break;
		}

		if (sortByTextureWithinGroup)
		{
			SortPolygonGroupByTexture(group);
		}

		FbxMesh* lMesh = FbxMesh::Create(pSdkManager, group->name);
		meshGrouplookup[group] = lMesh;

		FbxSkin* lSkin = NULL;
		if (!allBlendShape)
		{
			lSkin = FbxSkin::Create(pScene, "");

			for (int j = 0; j < joints.size(); j++)
			{
				if (joints[j]->isShapeAnimationJoint)
					continue;

				FbxCluster* cluster = FbxCluster::Create(pScene, "");
				cluster->SetLink(jointSkeleton[joints[j]->name]);
				cluster->SetLinkMode(FbxCluster::eNormalize);

				lSkin->AddCluster(cluster);

				jointCluster[joints[j]->name] = cluster;
			}

			lMesh->AddDeformer(lSkin);
		}

		// create a FbxNode
		FbxNode* lNode = FbxNode::Create(pSdkManager, group->name);

		// set the node attribute
		lNode->SetNodeAttribute(lMesh);

		// set the shading mode to view texture
		lNode->SetShadingMode(FbxNode::eTextureShading);

		std::vector<CVertice*> subVerticeList;
		std::vector<CUVCoordinate*> subUVList;
		std::vector<CVerticeColor*> subVertexColorList;
		std::vector<CNormal*> subNormalList;
		std::vector<CString> subMaterialList;

		lNode->AddMaterial(defaultMaterialDefinition);

		subMaterialList.push_back("DefaultMaterial");

		int totalCountPolygonIndexes = 0;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			totalCountPolygonIndexes += polygon->polygonPoints.size();
		}

		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));


			if (!(std::find(subMaterialList.begin(), subMaterialList.end(), polygon->materialName)!=subMaterialList.end()))
			{
				CMaterial* materialFound = NULL;

				for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
				{
					CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

					std::vector<CMaterial*>::iterator	itermaterial;
					for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
					{
						CMaterial* material = (CMaterial*)*itermaterial;

						if (material->name == polygon->materialName)
						{
							materialFound = material;
						}
					}
				}

				if (materialFound != NULL)
				{
					FbxSurfacePhong* lMaterial = NULL;

					std::map<CString,FbxSurfacePhong*>::iterator iterusedmaterials;
					for (iterusedmaterials = usedMaterialList.begin(); iterusedmaterials != usedMaterialList.end(); iterusedmaterials++ )
					{
						CString name = (CString)((*iterusedmaterials).first);
						FbxSurfacePhong* phongSurface = (FbxSurfacePhong*)((*iterusedmaterials).second);

						if (name == polygon->materialName)
						{
							lMaterial = phongSurface;
							break;
						}
					}

					subMaterialList.push_back(polygon->materialName);

					if (lMaterial == NULL)
					{
						// add material to the node. 
						// the material can't in different document with the geometry node or in sub-document
						// we create a simple material here which belong to main document
						FbxString lMaterialName = materialFound->name;
						lMaterial = FbxSurfacePhong::Create(pSdkManager, lMaterialName.Buffer());

						if (materialFound->diffuseColor.contains)
						{
							lMaterial->Diffuse.Set(FbxDouble3(materialFound->diffuseColor.r, materialFound->diffuseColor.g, materialFound->diffuseColor.b));
							lMaterial->DiffuseFactor.Set(1.0);
						}

						if (materialFound->diffuseTexture.contains)
						{
							FbxFileTexture* gTexture = FbxFileTexture::Create(pScene,"Diffuse Texture");

							// Resource file must be in the application's directory.
							FbxString lTexPath = inputPath + materialFound->diffuseTexture.stringValue;

							// Set texture properties.
							gTexture->SetFileName(lTexPath.Buffer()); 
							gTexture->SetTextureUse(FbxTexture::eStandard);
							gTexture->SetMappingType(FbxTexture::eUV);
							gTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
							gTexture->SetSwapUV(false);
							gTexture->SetTranslation(0.0, 0.0);
							gTexture->SetScale(1.0, 1.0);
							gTexture->SetRotation(0.0, 0.0);
							lMaterial->Diffuse.ConnectSrcObject(gTexture);
						}

						if (materialFound->ambientColor.contains)
						{
							lMaterial->Ambient.Set(FbxDouble3(materialFound->ambientColor.r, materialFound->ambientColor.g, materialFound->ambientColor.b));
							lMaterial->AmbientFactor.Set(1.0);
						}

						if (materialFound->spectralColor.contains)
						{
							lMaterial->Specular.Set(FbxDouble3(materialFound->spectralColor.r, materialFound->spectralColor.g, materialFound->spectralColor.b));
							lMaterial->SpecularFactor.Set(1.0);
						}

						if (materialFound->alpha.contains)
						{
							lMaterial->TransparencyFactor = materialFound->alpha.floatValue;
						}

						/*if (materialFound->phongSpectral.contains)
						{
							lMaterial->Shininess = materialFound->alpha.floatValue;
						}*/

						if (materialFound->illum.contains)
						{
							lMaterial->ReflectionFactor = materialFound->illum.floatValue;
						}
					}

					lNode->AddMaterial(lMaterial);
				}
			}

			int totalUVs = uvCoordinates.size();

			std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
			for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
			{
				CPolygonPoint* polygonPoint = (CPolygonPoint*)*iterpolygonpoint;

				CVertice* vertice = NULL;
				if ((polygonPoint->verticeIndex != -1) && (polygonPoint->verticeIndex < vertices.size()))
					vertice = vertices[polygonPoint->verticeIndex];
				CUVCoordinate* uv = NULL;
				if ((polygonPoint->uvIndex != -1) && (polygonPoint->uvIndex < uvCoordinates.size()))
					uv = uvCoordinates[polygonPoint->uvIndex];
				CNormal* normal = NULL;
				if ((polygonPoint->normalIndex != -1) && (polygonPoint->normalIndex < normals.size()))
					normal = normals[polygonPoint->normalIndex];
				CVerticeColor* verticeColor = NULL;
				if (polygonPoint->verticeColorIndex != -1)
				{
					if (verticeColors[polygonPoint->verticeColorIndex]->contains)
						verticeColor = verticeColors[polygonPoint->verticeColorIndex];
				}

				if (vertice != NULL)
				{
					if (!(std::find(subVerticeList.begin(), subVerticeList.end(), vertice)!=subVerticeList.end()))
					{
						subVerticeList.push_back(vertice);
					}
				}

				if (uv != NULL)
				{
					if (!(std::find(subUVList.begin(), subUVList.end(), uv)!=subUVList.end()))
					{
						subUVList.push_back(uv);
					}
				}

				if (normal != NULL)
				{
					if (!(std::find(subNormalList.begin(), subNormalList.end(), normal)!=subNormalList.end()))
					{
						subNormalList.push_back(normal);
					}
				}

				if (verticeColor != NULL)
				{
					if (!(std::find(subVertexColorList.begin(), subVertexColorList.end(), verticeColor)!=subVertexColorList.end()))
					{
						subVertexColorList.push_back(verticeColor);
					}
				}
			}
		}

		lMesh->InitControlPoints(subVerticeList.size());
		FbxVector4* lControlPoints = lMesh->GetControlPoints();

		int subVerticeCounter = 0;
		std::vector<CVertice*>::iterator	iterPointList;
		for (iterPointList = subVerticeList.begin(); iterPointList != subVerticeList.end(); iterPointList++)
		{
			CVertice* vertice = (CVertice*)*iterPointList;

			lControlPoints[subVerticeCounter] = FbxVector4(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

			for (int x = 0; x < joints.size(); x++)
			{
				for (int y = 0; y < joints[x]->controlPoints.size(); y++)
				{
					CVertice* controlVertice = vertices[joints[x]->controlPoints[y]];
					if (controlVertice == vertice)
					{
						originalToSubVerticeControlPointMapping[group][joints[x]->controlPoints[y]] = subVerticeCounter;
						break;
					}
				}
			}

			if (!allBlendShape)
			{
				int found = 0;

				for (int x = 0; x < joints.size(); x++)
				{
					if (!joints[x]->isShapeAnimationJoint)
					{
						FbxCluster* cluster = jointCluster[joints[x]->name];

						for (int y = 0; y < joints[x]->controlPoints.size(); y++)
						{
							CVertice* controlVertice = vertices[joints[x]->controlPoints[y]];
							if (controlVertice == vertice)
							{
								cluster->AddControlPointIndex(subVerticeCounter, 1.0f);

								if (joints[x]->jointType == Absolute)
								{
									lControlPoints[subVerticeCounter] = FbxVector4(vertice->vertex.x - joints[x]->positionAbsolute.x, vertice->vertex.y - joints[x]->positionAbsolute.y, vertice->vertex.z - joints[x]->positionAbsolute.z);
								}
								else if (joints[x]->jointType == Relative)
								{
									lControlPoints[subVerticeCounter] = FbxVector4(vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);
								}
								found++;
								break;
							}
						}

						if (found > 0)
							break;
					}
				}
			}

			subVerticeCounter++;
		}

		FbxGeometryElementNormal* lElementNormal= lMesh->CreateElementNormal();
		lElementNormal->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		lElementNormal->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

		std::vector<CNormal*>::iterator	iterNormal;
		for (iterNormal = subNormalList.begin(); iterNormal != subNormalList.end(); iterNormal++)
		{
			CNormal* normal = (CNormal*)*iterNormal;

			lElementNormal->GetDirectArray().Add(FbxVector4(normal->nx, normal->ny, normal->nz));
		}

		//Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
		//we must update the size of the index array.
		lElementNormal->GetIndexArray().SetCount(totalCountPolygonIndexes);


		FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("default");
		lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

		std::vector<CUVCoordinate*>::iterator	iterUVList;
		for (iterUVList = subUVList.begin(); iterUVList != subUVList.end(); iterUVList++)
		{
			CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;

			lUVDiffuseElement->GetDirectArray().Add(FbxVector2(uv->u, uv->v));
		}

		//Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
		//we must update the size of the index array.
		lUVDiffuseElement->GetIndexArray().SetCount(totalCountPolygonIndexes);

		FbxGeometryElementVertexColor* lVertexColorElement = NULL;
		if (foundVerticeColors)
		{
			lVertexColorElement = lMesh->CreateElementVertexColor();
			lVertexColorElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
			lVertexColorElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

			std::vector<CVerticeColor*>::iterator	iterVertexColorList;
			for (iterVertexColorList = subVertexColorList.begin(); iterVertexColorList != subVertexColorList.end(); iterVertexColorList++)
			{
				CVerticeColor* verticeColor = (CVerticeColor*)*iterVertexColorList;

				lVertexColorElement->GetDirectArray().Add(FbxVector4(verticeColor->color.r / 255.0f, verticeColor->color.g / 255.0f, verticeColor->color.b / 255.0f, verticeColor->color.a / 255.0f));
			}

			lVertexColorElement->GetIndexArray().SetCount(totalCountPolygonIndexes);
		}

		FbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
		lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
		lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

		lMaterialElement->GetIndexArray().SetCount(group->polygons.size());

		int polygonCount = 0;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			
			int materialIndex = 0;
			std::vector<CString>::iterator	iterMaterialList;
			for (iterMaterialList = subMaterialList.begin(); iterMaterialList != subMaterialList.end(); iterMaterialList++)
			{
				CString material = (CString)(*iterMaterialList);
				if (material == polygon->materialName)
				{
					break;
				}
				materialIndex++;
			}

			if (materialIndex >= subMaterialList.size())
				materialIndex = 0; // error

			lMaterialElement->GetIndexArray().SetAt(polygonCount, materialIndex);
		}

		int pointNum = 0;

		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			
			int materialIndex = 0;
			std::vector<CString>::iterator	iterMaterialList;
			for (iterMaterialList = subMaterialList.begin(); iterMaterialList != subMaterialList.end(); iterMaterialList++)
			{
				CString material = (CString)(*iterMaterialList);
				if (material == polygon->materialName)
				{
					break;
				}
				materialIndex++;
			}

			if (materialIndex >= subMaterialList.size())
				materialIndex = 0; // error

			lMesh->BeginPolygon(materialIndex);

			std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;

			int numPolyPointsExpected = polygon->polygonPoints.size();

			CPolygonPoint** resultingPoints = new CPolygonPoint*[polygon->polygonPoints.size()];

			int resultCounter = 0;
			for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
			{
				resultingPoints[resultCounter] = (CPolygonPoint*)*iterpolygonpoint;
				resultCounter++;
			}

			for (int x = 0; x < resultCounter; x++)
			{
				CPolygonPoint* polygonPoint = resultingPoints[x];
				CVertice* vertice = ((CVertice*)vertices[polygonPoint->verticeIndex]);

				int foundPoint = 0;
				std::vector<CVertice*>::iterator	iterVertice;
				for (iterVertice = subVerticeList.begin(); iterVertice != subVerticeList.end(); iterVertice++)
				{
					CVertice* verticeTest = (CVertice*)(*iterVertice);
					/*if (
						(vertice->vertex.x == verticeTest->vertex.x)
						&&
						(vertice->vertex.y == verticeTest->vertex.y)
						&& 
						(vertice->vertex.z == verticeTest->vertex.z)
						)
					{
						break;
					}*/
					if (vertice == verticeTest)
						break;
					foundPoint++;
				}

				if (foundPoint >= subVerticeList.size())
					foundPoint = 0; // error

				CString formatIndexStr;
				if (foundTextureUV)
				{
					if ((polygonPoint->uvIndex != -1) && (polygonPoint->uvIndex < uvCoordinates.size()))
					{
						CUVCoordinate* uv = ((CUVCoordinate*)uvCoordinates[polygonPoint->uvIndex]);

						int foundUV = 0;
						std::vector<CUVCoordinate*>::iterator	iterUV;
						for (iterUV = subUVList.begin(); iterUV != subUVList.end(); iterUV++)
						{
							CUVCoordinate* uvTest = (CUVCoordinate*)(*iterUV);
							/*if (
								(uv->u == uvTest->u)
								&&
								(uv->v == uvTest->v))*/
							if (uvTest == uv)
							{
								break;
							}
							foundUV++;
						}

						if (foundUV >= subUVList.size())
							foundUV = 0; // error

						lUVDiffuseElement->GetIndexArray().SetAt(pointNum, foundUV);
						lMesh->AddPolygon(foundPoint, foundUV);
					}
					else
					{
						lUVDiffuseElement->GetIndexArray().SetAt(pointNum, 0);
						lMesh->AddPolygon(foundPoint, 0);
					}
				}
				else
				{
					lUVDiffuseElement->GetIndexArray().SetAt(pointNum, -1);
					lMesh->AddPolygon(foundPoint, -1);
				}

				if (foundNormals)
				{
					if ((polygonPoint->normalIndex != -1) && (polygonPoint->normalIndex < normals.size()))
					{
						CNormal* normal = ((CNormal*)normals[polygonPoint->normalIndex]);

						int foundNormal = 0;
						std::vector<CNormal*>::iterator	iterNormal;
						for (iterNormal = subNormalList.begin(); iterNormal != subNormalList.end(); iterNormal++)
						{
							CNormal* normalTest = (CNormal*)(*iterNormal);
							/*if (
								(normal->nx == normalTest->nx)
								&&
								(normal->ny == normalTest->ny)
								&&
								(normal->nz == normalTest->nz)
								)*/
							if (normal == normalTest)
							{
								break;
							}
							foundNormal++;
						}

						if (foundNormal >= subNormalList.size())
							foundNormal = 0; // error

						lElementNormal->GetIndexArray().SetAt(pointNum, foundNormal);
					}
					else
					{
						lElementNormal->GetIndexArray().SetAt(pointNum, 0);
					}
				}
				else
				{
					lElementNormal->GetIndexArray().SetAt(pointNum, -1);
				}

				if ((polygonPoint->verticeColorIndex != -1) && (polygonPoint->verticeColorIndex < verticeColors.size()))
				{
					CVerticeColor* verticeColor = ((CVerticeColor*)verticeColors[polygonPoint->verticeColorIndex]);
					if (verticeColor->contains)
					{
					
						int foundVertColor = 0;
						std::vector<CVerticeColor*>::iterator	iterVertColor;
						for (iterVertColor = subVertexColorList.begin(); iterVertColor != subVertexColorList.end(); iterVertColor++)
						{
							CVerticeColor* vertColorTest = (CVerticeColor*)(*iterVertColor);
							/*if (
								(verticeColor->color.r == vertColorTest->color.r)
								&&
								(verticeColor->color.g == vertColorTest->color.g)
								&&
								(verticeColor->color.b == vertColorTest->color.b)
								)*/
							if (vertColorTest == verticeColor)
							{
								break;
							}
							foundVertColor++;
						}

						if (lVertexColorElement != NULL)
						{
							if (foundVertColor < subVertexColorList.size())
								lVertexColorElement->GetIndexArray().SetAt(pointNum, foundVertColor);
							else
								lVertexColorElement->GetIndexArray().SetAt(pointNum, 0);
						}
					}
					else
					{
						if (lVertexColorElement != NULL)
						{
							lVertexColorElement->GetIndexArray().SetAt(pointNum, -1);
						}
					}
				}

				pointNum++;
			}

			delete [] resultingPoints;
		}

		lMesh->GetLayer(0)->SetUVs(lUVDiffuseElement);
		lMesh->GetLayer(0)->SetMaterials(lMaterialElement);
		lMesh->GetLayer(0)->SetNormals(lElementNormal);
		lMesh->GetLayer(0)->SetVertexColors(lVertexColorElement);

		lMainNode->AddChild(lNode);

		if (!allBlendShape)
		{
			int numOfClusters = lSkin->GetClusterCount();
			for (int clusterIndex = (numOfClusters - 1); clusterIndex >= 0; clusterIndex--)
			{
				FbxCluster* lCluster = lSkin->GetCluster(clusterIndex);

				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

				if (lVertexIndexCount == 0)
				{
					lSkin->RemoveCluster(lCluster);
				}
			}
		}

		FbxMatrix lTransformMatrix = lNode->EvaluateGlobalTransform();
		lPose->Add(lNode, lTransformMatrix, false);

		addedNodes.push_back(lNode);
	}

	pScene->AddPose(lPose);

	FbxAnimStack* lAnimStack = NULL;
	FbxAnimLayer* lAnimLayer = NULL;

	if (animations.size() > 0)
	{
		lAnimStack = FbxAnimStack::Create(pScene, "Take 001");
		//lAnimStack->Description = "Animation"; // TODO read in this

		lAnimLayer = FbxAnimLayer::Create(pScene, "Base Layer");
		lAnimStack->AddMember(lAnimLayer);
	}

	for (int x = 0; x < animations.size(); x++)
	{
		FbxNode* cameraNode = NULL;
		FbxCamera* lCamera = NULL;
		if (animations[x]->treatAsBlendShape)
		{
			
		}
		else if (animations[x]->isCamera)
		{
			lCamera = FbxCamera::Create(pScene, animations[x]->name);
			lCamera->SetAspect( FbxCamera::eFixedResolution, 320, 240);
			lCamera->SetApertureMode(FbxCamera::eVertical);
			lCamera->SetNearPlane(0.001000);
			lCamera->SetFarPlane(600000.000000);
			
			if (animations[x]->keyframes.size() > 0)
			{
				lCamera->Position.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));
				lCamera->InterestPosition.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));

				if (animations[x]->keyframes[0]->fieldOfView.contains)
				{
					lCamera->FieldOfView.Set(animations[x]->keyframes[0]->fieldOfView.floatValue);
				}
			}

			cameraNode = FbxNode::Create(pScene, animations[x]->name);

			if (animations[x]->keyframes.size() > 0)
			{
				cameraNode->LclTranslation.Set(FbxDouble3(animations[x]->keyframes[0]->translation.x, animations[x]->keyframes[0]->translation.y, animations[x]->keyframes[0]->translation.z));
				cameraNode->LclRotation.Set(FbxDouble3(animations[x]->keyframes[0]->rotation.x, animations[x]->keyframes[0]->rotation.y, animations[x]->keyframes[0]->rotation.z));


				cameraNode->SetRotationActive(true);
				cameraNode->SetPostRotation(FbxNode::eSourcePivot, FbxVector4(0, -90, 0));
			}


			cameraNode->SetNodeAttribute(lCamera);
			lMainNode->AddChild(cameraNode);

			pScene->GetGlobalSettings().SetDefaultCamera(FBXSDK_CAMERA_PERSPECTIVE);
		}

		FbxTime lTime;
		FbxAnimCurveKey key;

		// TODO Cannot have more than one animation right now, because they share same skeleton tracks...not sure what to do, for now just hack if more than one, to not set to empty
		if (animations[x]->treatAsBlendShape)
		{
			for (std::map<CGroup*, FbxMesh*>::iterator itMeshes = meshGrouplookup.begin(); itMeshes != meshGrouplookup.end(); itMeshes++ )
			{
				FbxMesh* lMesh = itMeshes->second;
				FbxAnimCurve* lCurveShape = lMesh->GetShapeChannel(0, 0, lAnimLayer);

				std::vector<FbxShape*> fbxBlendShapes;
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					CString shapeStr;
					shapeStr.Format("%s_%s_%d", animations[x]->name, itMeshes->first->name, y);

					FbxBlendShape* lBlendShape = FbxBlendShape::Create(pScene, shapeStr);
					FbxBlendShapeChannel* lBlendShapeChannel = FbxBlendShapeChannel::Create(pScene, itMeshes->first->name);
					lBlendShape->AddBlendShapeChannel(lBlendShapeChannel);

					FbxShape* lShape = FbxShape::Create(pScene, shapeStr);
					int numberMeshPoints = lMesh->GetControlPointsCount();
					lShape->InitControlPoints(numberMeshPoints);
					
					FbxVector4* lMeshControlPoints = lMesh->GetControlPoints();
					FbxVector4* lShapeControlPoints = lShape->GetControlPoints();
					for (int m = 0; m < numberMeshPoints; m++)
					{
						lShapeControlPoints[m] = lMeshControlPoints[m];
					}

					for (int z = 0; z < joints.size(); z++)
					{
						for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
						{
							if (animations[x]->keyframes[y]->animationParts[w]->name == joints[z]->name)
							{
								for (int c = 0; c < joints[z]->controlPoints.size(); c++)
								{
									int controlPointIndex = joints[z]->controlPoints[c];
									if (originalToSubVerticeControlPointMapping[itMeshes->first].find(controlPointIndex) != originalToSubVerticeControlPointMapping[itMeshes->first].end())
									{
										controlPointIndex = originalToSubVerticeControlPointMapping[itMeshes->first][controlPointIndex];

										lShapeControlPoints[controlPointIndex][0] = lMeshControlPoints[controlPointIndex][0] + animations[x]->keyframes[y]->animationParts[w]->translation.x;
										lShapeControlPoints[controlPointIndex][1] = lMeshControlPoints[controlPointIndex][1] + animations[x]->keyframes[y]->animationParts[w]->translation.y;
										lShapeControlPoints[controlPointIndex][2] = lMeshControlPoints[controlPointIndex][2] + animations[x]->keyframes[y]->animationParts[w]->translation.z;
									}
								}
							}
						}
					}
		
					fbxBlendShapes.push_back(lShape);
					lBlendShapeChannel->AddTargetShape(lShape);
					lMesh->AddDeformer(lBlendShape);
				}

				int lKeyIndex;
				for (int y = 0; y < animations[x]->keyframes.size(); y++)
				{
					FbxShape* lShape = fbxBlendShapes[y];
					FbxBlendShapeChannel* lBlendShapeChannel = lShape->GetBlendShapeChannel();

					FbxAnimCurve* lCurveShape = lBlendShapeChannel->DeformPercent.GetCurve(lAnimLayer, true);
					lCurveShape->KeyModifyBegin();
					for (int yy = 0; yy < animations[x]->keyframes.size(); yy++)
					{
						if (y != yy)
						{
							if ((yy == 0) || (yy == (y - 1)) || (yy == (y + 1)))
							{
								lTime.SetFrame((float)animations[x]->keyframes[yy]->number);
								key.Set(lTime, 0.0);
								lKeyIndex = lCurveShape->KeyAdd(lTime, key);
								lCurveShape->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
							}
						}
						else
						{
							lTime.SetFrame((float)animations[x]->keyframes[yy]->number);
							key.Set(lTime, 100.0);
							lKeyIndex = lCurveShape->KeyAdd(lTime, key);
							lCurveShape->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
						}
					}
					lCurveShape->KeyModifyEnd();
				}
			}
		}
		else if (cameraNode == NULL)
		{
			FbxAnimCurve* lCurveTopRotationX = lTopJointNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			FbxAnimCurve* lCurveTopRotationY = lTopJointNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			FbxAnimCurve* lCurveTopRotationZ = lTopJointNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
			FbxAnimCurve* lCurveTopTranslationX = lTopJointNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			FbxAnimCurve* lCurveTopTranslationY = lTopJointNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			FbxAnimCurve* lCurveTopTranslationZ = lTopJointNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

			lCurveTopRotationX->KeyModifyBegin();
			lCurveTopRotationY->KeyModifyBegin();
			lCurveTopRotationZ->KeyModifyBegin();
			lCurveTopTranslationX->KeyModifyBegin();
			lCurveTopTranslationY->KeyModifyBegin();
			lCurveTopTranslationZ->KeyModifyBegin();

			for (int y = 0; y < animations[x]->keyframes.size(); y++)
			{
				lTime.SetFrame((float)animations[x]->keyframes[y]->number);

				int lKeyIndex;
				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.x != animations[x]->keyframes[y-1]->rotation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.x == animations[x]->keyframes[y-1]->rotation.x) && (animations[x]->keyframes[y]->rotation.x != animations[x]->keyframes[y+1]->rotation.x)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.x);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopRotationX->KeyAdd(lTime, key);
					lCurveTopRotationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.y != animations[x]->keyframes[y-1]->rotation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.y == animations[x]->keyframes[y-1]->rotation.y) && (animations[x]->keyframes[y]->rotation.y != animations[x]->keyframes[y+1]->rotation.y)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.y);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopRotationY->KeyAdd(lTime, key);
					lCurveTopRotationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.z != animations[x]->keyframes[y-1]->rotation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.z == animations[x]->keyframes[y-1]->rotation.z) && (animations[x]->keyframes[y]->rotation.z != animations[x]->keyframes[y+1]->rotation.z)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.z);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopRotationZ->KeyAdd(lTime, key);
					lCurveTopRotationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}


				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.x != animations[x]->keyframes[y-1]->translation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.x == animations[x]->keyframes[y-1]->translation.x) && (animations[x]->keyframes[y]->translation.x != animations[x]->keyframes[y+1]->translation.x)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.x);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopTranslationX->KeyAdd(lTime, key);
					lCurveTopTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.y != animations[x]->keyframes[y-1]->translation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.y == animations[x]->keyframes[y-1]->translation.y) && (animations[x]->keyframes[y]->translation.y != animations[x]->keyframes[y+1]->translation.y)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.y);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopTranslationY->KeyAdd(lTime, key);
					lCurveTopTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.z != animations[x]->keyframes[y-1]->translation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.z == animations[x]->keyframes[y-1]->translation.z) && (animations[x]->keyframes[y]->translation.z != animations[x]->keyframes[y+1]->translation.z)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.z);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTopTranslationZ->KeyAdd(lTime, key);
					lCurveTopTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}
			}

			lCurveTopRotationX->KeyModifyEnd();
			lCurveTopRotationY->KeyModifyEnd();
			lCurveTopRotationZ->KeyModifyEnd();
			lCurveTopTranslationX->KeyModifyEnd();
			lCurveTopTranslationY->KeyModifyEnd();
			lCurveTopTranslationZ->KeyModifyEnd();
			for (int z = 0; z < joints.size(); z++)
			{
				if (joints[z]->isShapeAnimationJoint)
				{
					for (std::map<CGroup*, FbxMesh*>::iterator itMeshes = meshGrouplookup.begin(); itMeshes != meshGrouplookup.end(); itMeshes++)
					{
						FbxMesh* lMesh = itMeshes->second;

						bool isMeshAShapeAnimationPart = false;
						//for (int y = 0; y < animations[x]->keyframes.size(); y++)
						if (animations[x]->keyframes.size() > 0)
						{
							int y = 0;
							{
								for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
								{
									if (animations[x]->keyframes[y]->animationParts[w]->name == joints[z]->name)
									{
										if (animations[x]->keyframes[y]->animationParts[w]->shapeVertices.size() > 0)
										{
											for (int c = 0; c < joints[z]->controlPoints.size(); c++)
											{
												int controlPointIndex = joints[z]->controlPoints[c];
												if (originalToSubVerticeControlPointMapping[itMeshes->first].find(controlPointIndex) != originalToSubVerticeControlPointMapping[itMeshes->first].end())
												{
													int controlPointIndexSub = originalToSubVerticeControlPointMapping[itMeshes->first][controlPointIndex];

													for (int s = 0; s < animations[x]->keyframes[y]->animationParts[w]->shapeVertices.size(); s++)
													{
														if (animations[x]->keyframes[y]->animationParts[w]->shapeVertices[s].vertexIndex == controlPointIndex)
														{
															isMeshAShapeAnimationPart = true;

															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						if (!isMeshAShapeAnimationPart)
							continue;

						FbxAnimCurve* lCurveShape = lMesh->GetShapeChannel(0, 0, lAnimLayer);

						std::vector<FbxShape*> fbxBlendShapes;
						for (int y = 0; y < animations[x]->keyframes.size(); y++)
						{
							CString shapeStr;
							shapeStr.Format("%s_%s_%d", animations[x]->name, itMeshes->first->name, y);

							FbxBlendShape* lBlendShape = FbxBlendShape::Create(pScene, shapeStr);
							FbxBlendShapeChannel* lBlendShapeChannel = FbxBlendShapeChannel::Create(pScene, itMeshes->first->name);
							lBlendShape->AddBlendShapeChannel(lBlendShapeChannel);

							FbxShape* lShape = FbxShape::Create(pScene, shapeStr);
							int numberMeshPoints = lMesh->GetControlPointsCount();
							lShape->InitControlPoints(numberMeshPoints);

							FbxVector4* lMeshControlPoints = lMesh->GetControlPoints();
							FbxVector4* lShapeControlPoints = lShape->GetControlPoints();
							for (int m = 0; m < numberMeshPoints; m++)
							{
								lShapeControlPoints[m] = lMeshControlPoints[m];
							}

							for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
							{
								if (animations[x]->keyframes[y]->animationParts[w]->name == joints[z]->name)
								{
									for (int c = 0; c < joints[z]->controlPoints.size(); c++)
									{
										int controlPointIndex = joints[z]->controlPoints[c];
										if (originalToSubVerticeControlPointMapping[itMeshes->first].find(controlPointIndex) != originalToSubVerticeControlPointMapping[itMeshes->first].end())
										{
											int controlPointIndexSub = originalToSubVerticeControlPointMapping[itMeshes->first][controlPointIndex];

											for (int s = 0; s < animations[x]->keyframes[y]->animationParts[w]->shapeVertices.size(); s++)
											{
												if (animations[x]->keyframes[y]->animationParts[w]->shapeVertices[s].vertexIndex == controlPointIndex)
												{
													lShapeControlPoints[controlPointIndexSub][0] = lMeshControlPoints[controlPointIndexSub][0] + animations[x]->keyframes[y]->animationParts[w]->shapeVertices[s].translation.x;
													lShapeControlPoints[controlPointIndexSub][1] = lMeshControlPoints[controlPointIndexSub][1] + animations[x]->keyframes[y]->animationParts[w]->shapeVertices[s].translation.y;
													lShapeControlPoints[controlPointIndexSub][2] = lMeshControlPoints[controlPointIndexSub][2] + animations[x]->keyframes[y]->animationParts[w]->shapeVertices[s].translation.z;

													break;
												}
											}
										}
									}
								}
							}

							fbxBlendShapes.push_back(lShape);
							lBlendShapeChannel->AddTargetShape(lShape);
							lMesh->AddDeformer(lBlendShape);
						}

						int lKeyIndex;
						for (int y = 0; y < animations[x]->keyframes.size(); y++)
						{
							FbxShape* lShape = fbxBlendShapes[y];
							FbxBlendShapeChannel* lBlendShapeChannel = lShape->GetBlendShapeChannel();

							FbxAnimCurve* lCurveShape = lBlendShapeChannel->DeformPercent.GetCurve(lAnimLayer, true);
							lCurveShape->KeyModifyBegin();
							for (int yy = 0; yy < animations[x]->keyframes.size(); yy++)
							{
								if (y != yy)
								{
									if ((yy == 0) || (yy == (y - 1)) || (yy == (y + 1)))
									{
										lTime.SetFrame((float)animations[x]->keyframes[yy]->number);
										key.Set(lTime, 0.0);
										lKeyIndex = lCurveShape->KeyAdd(lTime, key);
										lCurveShape->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
									}
								}
								else
								{
									lTime.SetFrame((float)animations[x]->keyframes[yy]->number);
									key.Set(lTime, 100.0);
									lKeyIndex = lCurveShape->KeyAdd(lTime, key);
									lCurveShape->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
								}
							}
							lCurveShape->KeyModifyEnd();
						}
					}
				}
				else
				{
					std::map<CString,FbxNode*>::iterator iterJointSkeleton = jointSkeleton.find(joints[z]->name);
					if (iterJointSkeleton != jointSkeleton.end())
					{
						FbxNode* node = iterJointSkeleton->second;
						if (node != NULL)
						{
							std::string name = node->GetName();

							FbxAnimCurve* lCurveRotationX = node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
							FbxAnimCurve* lCurveRotationY = node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
							FbxAnimCurve* lCurveRotationZ = node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
							FbxAnimCurve* lCurveScaleX = node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
							FbxAnimCurve* lCurveScaleY = node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
							FbxAnimCurve* lCurveScaleZ = node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
							FbxAnimCurve* lCurveTranslationX = node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
							FbxAnimCurve* lCurveTranslationY = node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
							FbxAnimCurve* lCurveTranslationZ = node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

							lCurveRotationX->KeyModifyBegin();
							lCurveRotationY->KeyModifyBegin();
							lCurveRotationZ->KeyModifyBegin();
							lCurveScaleX->KeyModifyBegin();
							lCurveScaleY->KeyModifyBegin();
							lCurveScaleZ->KeyModifyBegin();
							lCurveTranslationX->KeyModifyBegin();
							lCurveTranslationY->KeyModifyBegin();
							lCurveTranslationZ->KeyModifyBegin();

							/*if (joints[z]->parent != NULL)
							{
							node->LclTranslation.Set(FbxVector4((joints[z]->positionAbsolute.x - joints[z]->parent->positionAbsolute.x), (joints[z]->positionAbsolute.y - joints[z]->parent->positionAbsolute.y), (joints[z]->positionAbsolute.z - joints[z]->parent->positionAbsolute.z)));
							}
							else
							{
							node->LclTranslation.Set(FbxVector4(joints[z]->positionAbsolute.x, joints[z]->positionAbsolute.y, joints[z]->positionAbsolute.z));
							}*/

							for (int y = 0; y < animations[x]->keyframes.size(); y++)
							{
								lTime.SetFrame((float)animations[x]->keyframes[y]->number);

								bool foundRotationKey = false;
								bool foundScaleKey = false;
								bool foundTranslationKey = false;

								for (int w = 0; w < animations[x]->keyframes[y]->animationParts.size(); w++)
								{
									if (animations[x]->keyframes[y]->animationParts[w]->name == joints[z]->name)
									{
										int lKeyIndex;
										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->rotation.x != animations[x]->keyframes[y - 1]->animationParts[w]->rotation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->rotation.x == animations[x]->keyframes[y - 1]->animationParts[w]->rotation.x) && (animations[x]->keyframes[y]->animationParts[w]->rotation.x != animations[x]->keyframes[y + 1]->animationParts[w]->rotation.x)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->rotation.x);
											//key.Set(lTime, 0.0f);
											lKeyIndex = lCurveRotationX->KeyAdd(lTime, key);
											lCurveRotationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}

										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->rotation.y != animations[x]->keyframes[y - 1]->animationParts[w]->rotation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->rotation.y == animations[x]->keyframes[y - 1]->animationParts[w]->rotation.y) && (animations[x]->keyframes[y]->animationParts[w]->rotation.y != animations[x]->keyframes[y + 1]->animationParts[w]->rotation.y)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->rotation.y);
											//key.Set(lTime, 0.0f);
											lKeyIndex = lCurveRotationY->KeyAdd(lTime, key);
											lCurveRotationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}

										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->rotation.z != animations[x]->keyframes[y - 1]->animationParts[w]->rotation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->rotation.z == animations[x]->keyframes[y - 1]->animationParts[w]->rotation.z) && (animations[x]->keyframes[y]->animationParts[w]->rotation.z != animations[x]->keyframes[y + 1]->animationParts[w]->rotation.z)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->rotation.z);
											//key.Set(lTime, 0.0f);
											lKeyIndex = lCurveRotationZ->KeyAdd(lTime, key);
											lCurveRotationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}


										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->scale.x != animations[x]->keyframes[y - 1]->animationParts[w]->scale.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->scale.x == animations[x]->keyframes[y - 1]->animationParts[w]->scale.x) && (animations[x]->keyframes[y]->animationParts[w]->scale.x != animations[x]->keyframes[y + 1]->animationParts[w]->scale.x)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->scale.x);
											//key.Set(lTime, 1.0f);
											lKeyIndex = lCurveScaleX->KeyAdd(lTime, key);
											lCurveScaleX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}

										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->scale.y != animations[x]->keyframes[y - 1]->animationParts[w]->scale.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->scale.y == animations[x]->keyframes[y - 1]->animationParts[w]->scale.y) && (animations[x]->keyframes[y]->animationParts[w]->scale.y != animations[x]->keyframes[y + 1]->animationParts[w]->scale.y)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->scale.y);
											//key.Set(lTime, 1.0f);
											lKeyIndex = lCurveScaleY->KeyAdd(lTime, key);
											lCurveScaleY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}

										if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->scale.z != animations[x]->keyframes[y - 1]->animationParts[w]->scale.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->scale.z == animations[x]->keyframes[y - 1]->animationParts[w]->scale.z) && (animations[x]->keyframes[y]->animationParts[w]->scale.z != animations[x]->keyframes[y + 1]->animationParts[w]->scale.z)))
										{
											key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->scale.z);
											//key.Set(lTime, 1.0f);
											lKeyIndex = lCurveScaleZ->KeyAdd(lTime, key);
											lCurveScaleZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
										}


										if (joints[z]->name == "10")
										{
											joints[z]->name = joints[z]->name;
										}


										if (joints[z]->parent == NULL)
										{
											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.x != animations[x]->keyframes[y - 1]->animationParts[w]->translation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.x == animations[x]->keyframes[y - 1]->animationParts[w]->translation.x) && (animations[x]->keyframes[y]->animationParts[w]->translation.x != animations[x]->keyframes[y + 1]->animationParts[w]->translation.x)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.x + joints[z]->positionAbsolute.x);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationX->KeyAdd(lTime, key);
												lCurveTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}

											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.y != animations[x]->keyframes[y - 1]->animationParts[w]->translation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.y == animations[x]->keyframes[y - 1]->animationParts[w]->translation.y) && (animations[x]->keyframes[y]->animationParts[w]->translation.y != animations[x]->keyframes[y + 1]->animationParts[w]->translation.y)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.y + joints[z]->positionAbsolute.y);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationY->KeyAdd(lTime, key);
												lCurveTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}

											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.z != animations[x]->keyframes[y - 1]->animationParts[w]->translation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.z == animations[x]->keyframes[y - 1]->animationParts[w]->translation.z) && (animations[x]->keyframes[y]->animationParts[w]->translation.z != animations[x]->keyframes[y + 1]->animationParts[w]->translation.z)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.z + joints[z]->positionAbsolute.z);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationZ->KeyAdd(lTime, key);
												lCurveTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}
										}
										else
										{
											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.x != animations[x]->keyframes[y - 1]->animationParts[w]->translation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.x == animations[x]->keyframes[y - 1]->animationParts[w]->translation.x) && (animations[x]->keyframes[y]->animationParts[w]->translation.x != animations[x]->keyframes[y + 1]->animationParts[w]->translation.x)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.x + (joints[z]->positionAbsolute.x - joints[z]->parent->positionAbsolute.x));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationX->KeyAdd(lTime, key);
												lCurveTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}

											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.y != animations[x]->keyframes[y - 1]->animationParts[w]->translation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.y == animations[x]->keyframes[y - 1]->animationParts[w]->translation.y) && (animations[x]->keyframes[y]->animationParts[w]->translation.y != animations[x]->keyframes[y + 1]->animationParts[w]->translation.y)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.y + (joints[z]->positionAbsolute.y - joints[z]->parent->positionAbsolute.y));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationY->KeyAdd(lTime, key);
												lCurveTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}

											if ((y == 0) || (animations[x]->keyframes[y]->animationParts[w]->translation.z != animations[x]->keyframes[y - 1]->animationParts[w]->translation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->animationParts[w]->translation.z == animations[x]->keyframes[y - 1]->animationParts[w]->translation.z) && (animations[x]->keyframes[y]->animationParts[w]->translation.z != animations[x]->keyframes[y + 1]->animationParts[w]->translation.z)))
											{
												key.Set(lTime, animations[x]->keyframes[y]->animationParts[w]->translation.z + (joints[z]->positionAbsolute.z - joints[z]->parent->positionAbsolute.z));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationZ->KeyAdd(lTime, key);
												lCurveTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}
										}

										foundRotationKey = true;
										foundScaleKey = true;
										foundTranslationKey = true;
										break;
									}
								}

								if (animations.size() == 1)
								{
									if (!foundRotationKey)
									{
										if ((y == 0))
										{
											FbxAnimCurveKey key;

											key.Set(lTime, 0.0f);
											int lKeyIndex = lCurveRotationX->KeyAdd(lTime, key);
											lCurveRotationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

											key.Set(lTime, 0.0f);
											lKeyIndex = lCurveRotationY->KeyAdd(lTime, key);
											lCurveRotationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

											key.Set(lTime, 0.0f);
											lKeyIndex = lCurveRotationZ->KeyAdd(lTime, key);
											lCurveRotationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											foundRotationKey = true;
										}
									}

									if (!foundScaleKey)
									{
										if ((y == 0))
										{
											FbxAnimCurveKey key;

											key.Set(lTime, 1.0f);
											int lKeyIndex = lCurveScaleX->KeyAdd(lTime, key);
											lCurveScaleX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

											key.Set(lTime, 1.0f);
											lKeyIndex = lCurveScaleY->KeyAdd(lTime, key);
											lCurveScaleY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

											key.Set(lTime, 1.0f);
											lKeyIndex = lCurveScaleZ->KeyAdd(lTime, key);
											lCurveScaleZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											foundScaleKey = true;
										}
									}

									if (!foundTranslationKey)
									{
										FbxAnimCurveKey key;
										int lKeyIndex;

										if (joints[z]->parent == NULL)
										{
											if ((y == 0))
											{
												key.Set(lTime, joints[z]->positionAbsolute.x);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationX->KeyAdd(lTime, key);
												lCurveTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

												key.Set(lTime, joints[z]->positionAbsolute.y);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationY->KeyAdd(lTime, key);
												lCurveTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

												key.Set(lTime, joints[z]->positionAbsolute.z);
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationZ->KeyAdd(lTime, key);
												lCurveTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}
										}
										else
										{
											if ((y == 0))
											{
												key.Set(lTime, (joints[z]->positionAbsolute.x - joints[z]->parent->positionAbsolute.x));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationX->KeyAdd(lTime, key);
												lCurveTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

												key.Set(lTime, (joints[z]->positionAbsolute.y - joints[z]->parent->positionAbsolute.y));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationY->KeyAdd(lTime, key);
												lCurveTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

												key.Set(lTime, (joints[z]->positionAbsolute.z - joints[z]->parent->positionAbsolute.z));
												//key.Set(lTime, 0.0f);
												lKeyIndex = lCurveTranslationZ->KeyAdd(lTime, key);
												lCurveTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
											}
										}
										foundTranslationKey = true;
									}
								}
							}

							lCurveRotationX->KeyModifyEnd();
							lCurveRotationY->KeyModifyEnd();
							lCurveRotationZ->KeyModifyEnd();
							lCurveScaleX->KeyModifyEnd();
							lCurveScaleY->KeyModifyEnd();
							lCurveScaleZ->KeyModifyEnd();
							lCurveTranslationX->KeyModifyEnd();
							lCurveTranslationY->KeyModifyEnd();
							lCurveTranslationZ->KeyModifyEnd();
						}
					}
				}
			}
		}
		else
		{
			FbxAnimCurve* lCurveRotationX = cameraNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			FbxAnimCurve* lCurveRotationY = cameraNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			FbxAnimCurve* lCurveRotationZ = cameraNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
			FbxAnimCurve* lCurveScaleX = cameraNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			FbxAnimCurve* lCurveScaleY = cameraNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			FbxAnimCurve* lCurveScaleZ = cameraNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
			FbxAnimCurve* lCurveTranslationX = cameraNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			FbxAnimCurve* lCurveTranslationY = cameraNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			FbxAnimCurve* lCurveTranslationZ = cameraNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
		
			FbxAnimCurve* lCurveFieldOfView = lCamera->FieldOfView.GetCurve(lAnimLayer, true);

			lCurveRotationX->KeyModifyBegin();
			lCurveRotationY->KeyModifyBegin();
			lCurveRotationZ->KeyModifyBegin();
			lCurveScaleX->KeyModifyBegin();
			lCurveScaleY->KeyModifyBegin();
			lCurveScaleZ->KeyModifyBegin();
			lCurveTranslationX->KeyModifyBegin();
			lCurveTranslationY->KeyModifyBegin();
			lCurveTranslationZ->KeyModifyBegin();

			lCurveFieldOfView->KeyModifyBegin();

			for (int y = 0; y < animations[x]->keyframes.size(); y++)
			{
				lTime.SetFrame((float)animations[x]->keyframes[y]->number);

				int lKeyIndex;
				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.x != animations[x]->keyframes[y-1]->rotation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.x == animations[x]->keyframes[y-1]->rotation.x) && (animations[x]->keyframes[y]->rotation.x != animations[x]->keyframes[y+1]->rotation.x)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.x);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveRotationX->KeyAdd(lTime, key);
					lCurveRotationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.y != animations[x]->keyframes[y-1]->rotation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.y == animations[x]->keyframes[y-1]->rotation.y) && (animations[x]->keyframes[y]->rotation.y != animations[x]->keyframes[y+1]->rotation.y)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.y);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveRotationY->KeyAdd(lTime, key);
					lCurveRotationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->rotation.z != animations[x]->keyframes[y-1]->rotation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->rotation.z == animations[x]->keyframes[y-1]->rotation.z) && (animations[x]->keyframes[y]->rotation.z != animations[x]->keyframes[y+1]->rotation.z)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->rotation.z);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveRotationZ->KeyAdd(lTime, key);
					lCurveRotationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}


				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->scale.x != animations[x]->keyframes[y-1]->scale.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->scale.x == animations[x]->keyframes[y-1]->scale.x) && (animations[x]->keyframes[y]->scale.x != animations[x]->keyframes[y+1]->scale.x)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->scale.x);
					//key.Set(lTime, 1.0f);
					lKeyIndex = lCurveScaleX->KeyAdd(lTime, key);
					lCurveScaleX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->scale.y != animations[x]->keyframes[y-1]->scale.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->scale.y == animations[x]->keyframes[y-1]->scale.y) && (animations[x]->keyframes[y]->scale.y != animations[x]->keyframes[y+1]->scale.y)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->scale.y);
					//key.Set(lTime, 1.0f);
					lKeyIndex = lCurveScaleY->KeyAdd(lTime, key);
					lCurveScaleY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->scale.z != animations[x]->keyframes[y-1]->scale.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->scale.z == animations[x]->keyframes[y-1]->scale.z) && (animations[x]->keyframes[y]->scale.z != animations[x]->keyframes[y+1]->scale.z)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->scale.z);
					//key.Set(lTime, 1.0f);
					lKeyIndex = lCurveScaleZ->KeyAdd(lTime, key);
					lCurveScaleZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}


				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.x != animations[x]->keyframes[y-1]->translation.x) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.x == animations[x]->keyframes[y-1]->translation.x) && (animations[x]->keyframes[y]->translation.x != animations[x]->keyframes[y+1]->translation.x)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.x);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTranslationX->KeyAdd(lTime, key);
					lCurveTranslationX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.y != animations[x]->keyframes[y-1]->translation.y) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.y == animations[x]->keyframes[y-1]->translation.y) && (animations[x]->keyframes[y]->translation.y != animations[x]->keyframes[y+1]->translation.y)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.y);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTranslationY->KeyAdd(lTime, key);
					lCurveTranslationY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->translation.z != animations[x]->keyframes[y-1]->translation.z) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->translation.z == animations[x]->keyframes[y-1]->translation.z) && (animations[x]->keyframes[y]->translation.z != animations[x]->keyframes[y+1]->translation.z)))
				{
					key.Set(lTime, animations[x]->keyframes[y]->translation.z);
					//key.Set(lTime, 0.0f);
					lKeyIndex = lCurveTranslationZ->KeyAdd(lTime, key);
					lCurveTranslationZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}


				if (animations[x]->keyframes[y]->fieldOfView.contains)
				{
					if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)) || (animations[x]->keyframes[y]->fieldOfView.floatValue != animations[x]->keyframes[y-1]->fieldOfView.floatValue) || ((y != (animations[x]->keyframes.size() - 1)) && (animations[x]->keyframes[y]->fieldOfView.floatValue == animations[x]->keyframes[y-1]->fieldOfView.floatValue) && (animations[x]->keyframes[y]->fieldOfView.floatValue != animations[x]->keyframes[y+1]->fieldOfView.floatValue)))
					{
						key.Set(lTime, animations[x]->keyframes[y]->fieldOfView.floatValue);
						//key.Set(lTime, 0.0f);
						lKeyIndex = lCurveFieldOfView->KeyAdd(lTime, key);
						lCurveFieldOfView->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
					}
				}
				else
				{
					if ((y == 0) || (y == (animations[x]->keyframes.size() - 1)))
					{
						FbxAnimCurveKey key;

						key.Set(lTime, 60.0f);
						//key.Set(lTime, 0.0f);
						int lKeyIndex = lCurveFieldOfView->KeyAdd(lTime, key);
						lCurveFieldOfView->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
					}
				}
			}

			lCurveRotationX->KeyModifyEnd();
			lCurveRotationY->KeyModifyEnd();
			lCurveRotationZ->KeyModifyEnd();
			lCurveScaleX->KeyModifyEnd();
			lCurveScaleY->KeyModifyEnd();
			lCurveScaleZ->KeyModifyEnd();
			lCurveTranslationX->KeyModifyEnd();
			lCurveTranslationY->KeyModifyEnd();
			lCurveTranslationZ->KeyModifyEnd();

			lCurveFieldOfView->KeyModifyEnd();
		}
	}

	int lMajor, lMinor, lRevision;
    bool lStatus = true;
	int pFileFormat = -1;

    // Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(pSdkManager, "");

    if( pFileFormat < 0 || pFileFormat >= pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
    {
        // Write in fall back format if pEmbedMedia is true
        pFileFormat = pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();
            
	//Try to export in ASCII if possible
        int lFormatIndex, lFormatCount = pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

        for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
        {
            if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
            {
                FbxString lDesc = pSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);

				string fbxExportTypeStr;

				if (fbxExportType == "")
					fbxExportTypeStr = "ascii";
				else
					fbxExportTypeStr = fbxExportType;
				
                if (lDesc.Find(fbxExportTypeStr.c_str())>=0)
                {
                    pFileFormat = lFormatIndex;
                }
            }
        }
    }

    // Set the export states. By default, the export states are always set to 
    // true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
    // shows how to change these states.
    IOS_REF.SetBoolProp(EXP_FBX_MATERIAL,        true);
    IOS_REF.SetBoolProp(EXP_FBX_TEXTURE,         true);
    IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED,        false);
    IOS_REF.SetBoolProp(EXP_FBX_SHAPE,           true);
    IOS_REF.SetBoolProp(EXP_FBX_GOBO,            false);
    IOS_REF.SetBoolProp(EXP_FBX_ANIMATION,       true);
    IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

    // Initialize the exporter by providing a filename.
    if(lExporter->Initialize(outputFile, pFileFormat, pSdkManager->GetIOSettings()) == false)
    {
		if (doMessageBoxes)
			MessageBox("Call to FbxExporter::Initialize() failed.\n");
    }

    FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);

    // Export the scene.
    lStatus = lExporter->Export(pScene); 

    // Destroy the exporter.
    lExporter->Destroy();

	if (pSdkManager) pSdkManager->Destroy();
	pSdkManager = NULL;
}

void CObjToAn8Dlg::GetNumberKeyframesFbxCameraRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, int& maxKeyframes, std::vector<CJoint*>& joints)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	if (pNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

		FbxNode* parentNode = pNode->GetParent();
		if (attributeType == FbxNodeAttribute::eCamera) // assume camera or top
		{
			FbxAnimCurve* lAnimCurve = NULL;

			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
				
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}    
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}

			// camera get field of view
			if (pNode->GetNodeAttribute() != NULL)
			{
				FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();
				if (attributeType == FbxNodeAttribute::eCamera) 
				{
					FbxCamera* lCamera = (FbxCamera*) pNode->GetNodeAttribute();
					lAnimCurve = lCamera->FieldOfView.GetCurve(pAnimLayer, true);

					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
				}
			}
		}
		else if ((attributeType == FbxNodeAttribute::eNull) || (attributeType == FbxNodeAttribute::eSkeleton))
		{
			for (int x = 0; x < joints.size(); x++)
			{
				if (joints[x]->name == name)
				{
					FbxAnimCurve* lAnimCurve = NULL;

					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}

					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}

					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}    
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					break;
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		GetNumberKeyframesFbxCameraRecursive(pAnimLayer, pNode->GetChild(lModelCount), maxKeyframes, joints);
    }
}

void CObjToAn8Dlg::GetNumberKeyframesFbxAnimationRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, int& maxKeyframes, std::vector<CJoint*>& joints)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eNull;
	if (pNode->GetNodeAttribute() != NULL)
		attributeType = pNode->GetNodeAttribute()->GetAttributeType();

	FbxNode* parentNode = pNode->GetParent();
	if ((name == "TopJoint") || (parentNode != NULL))
	{
		if ((name == "TopJoint") || (((joints.size() == 0)) && (attributeType == FbxNodeAttribute::eCamera))) // assume camera or top
		{
			FbxAnimCurve* lAnimCurve = NULL;

			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
				
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					if ((keyframeNumber+1) > maxKeyframes)
						maxKeyframes = (keyframeNumber+1);
				}
			}

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}    
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				int lKeyCount = lAnimCurve->KeyGetCount();

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					//if ((keyframeNumber+1) > maxKeyframes)
						//maxKeyframes = (keyframeNumber+1);
				}
			}

			if (joints.size() == 0) // camera get field of view
			{
				if (pNode->GetNodeAttribute() != NULL)
				{
					FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();
					if (attributeType == FbxNodeAttribute::eCamera) 
					{
						FbxCamera* lCamera = (FbxCamera*) pNode->GetNodeAttribute();
						lAnimCurve = lCamera->FieldOfView.GetCurve(pAnimLayer, true);

						if (lAnimCurve)
						{
							int lKeyCount = lAnimCurve->KeyGetCount();

							for(int lCount = 0; lCount < lKeyCount; lCount++)
							{
								float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
								FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
								int keyframeNumber = lKeyTime.GetFrameCount();
								
								if ((keyframeNumber+1) > maxKeyframes)
									maxKeyframes = (keyframeNumber+1);
							}
						}
					}
				}
			}
		}
		else if ((attributeType == FbxNodeAttribute::eNull) || (attributeType == FbxNodeAttribute::eSkeleton))
		{
			for (int x = 0; x < joints.size(); x++)
			{
				if (joints[x]->name == name)
				{
					FbxAnimCurve* lAnimCurve = NULL;

					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}

					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}

					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}    
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						int lKeyCount = lAnimCurve->KeyGetCount();

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							if ((keyframeNumber+1) > maxKeyframes)
								maxKeyframes = (keyframeNumber+1);
						}
					}
					break;
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		GetNumberKeyframesFbxAnimationRecursive(pAnimLayer, pNode->GetChild(lModelCount), maxKeyframes, joints);
    }
}

std::vector<int> CObjToAn8Dlg::GetKeyframeIndexes(FbxAnimCurve* lAnimCurve)
{
	int lKeyCount = lAnimCurve->KeyGetCount();

	std::vector<int> keyIndexes;
	for(int lCount = 0; lCount < lKeyCount; lCount++)
	{
		float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
		FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
		int frameIndex = lKeyTime.GetFrameCount();
		keyIndexes.push_back(frameIndex);
	}

	return keyIndexes;
}

float CObjToAn8Dlg::CalculateDerivative(int lCount, int numberKeyframes, FbxAnimCurve* lAnimCurve)
{
	float derivateValue = 0;
	if ((lCount > 1) && ((numberKeyframes - lCount - 1) > 1))
	{
		// 2d derivative
		FbxTime fbxTimePrev;
		fbxTimePrev.SetFrame(lCount - 1);
		float prevValue = lAnimCurve->Evaluate(fbxTimePrev);

		FbxTime fbxTimeNext;
		fbxTimeNext.SetFrame(lCount + 1);
		float nextValue = lAnimCurve->Evaluate(fbxTimeNext);

		
		FbxTime fbxTimePrev2;
		fbxTimePrev.SetFrame(lCount - 2);
		float prevValue2 = lAnimCurve->Evaluate(fbxTimePrev2);

		FbxTime fbxTimeNext2;
		fbxTimeNext.SetFrame(lCount + 2);
		float nextValue2 = lAnimCurve->Evaluate(fbxTimeNext2);

		derivateValue = ((4.0/3.0) * ((nextValue - prevValue) / (2 * lCount))) - ((1/3) * ((nextValue2 - prevValue2) / (4 * lCount)));
	}
	else if ((lCount > 0) && ((numberKeyframes - lCount - 1) > 0))
	{
		// 1d derivative
		FbxTime fbxTimePrev;
		fbxTimePrev.SetFrame(lCount - 1);
		float prevValue = lAnimCurve->Evaluate(fbxTimePrev);

		FbxTime fbxTimeNext;
		fbxTimeNext.SetFrame(lCount + 1);
		float nextValue = lAnimCurve->Evaluate(fbxTimeNext);

		derivateValue = (nextValue - prevValue) / (2 * lCount);
	}
	else if ((lCount == 0) && (numberKeyframes > 1))
	{
		FbxTime fbxTimeCurrent;
		fbxTimeCurrent.SetFrame(lCount);
		float currentValue = lAnimCurve->Evaluate(fbxTimeCurrent);

		FbxTime fbxTimeNext;
		fbxTimeNext.SetFrame(lCount + 1);
		float nextValue = lAnimCurve->Evaluate(fbxTimeNext);

		derivateValue = (nextValue - currentValue);
	}
	else if ((lCount == (numberKeyframes - 1)) && (numberKeyframes > 1))
	{
		FbxTime fbxTimeCurrent;
		fbxTimeCurrent.SetFrame(lCount);
		float currentValue = lAnimCurve->Evaluate(fbxTimeCurrent);

		FbxTime fbxTimePrev;
		fbxTimePrev.SetFrame(lCount - 1);
		float prevValue = lAnimCurve->Evaluate(fbxTimePrev);

		derivateValue = (currentValue - prevValue);
	}
	else
	{
		derivateValue = 0;
	}

	return derivateValue;
}

void CObjToAn8Dlg::ParseFbxAnimationRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CJoint*>& joints, CAnimation* animation, int numberKeyframes)
{
	if (pNode == NULL)
		return;

	FbxEuler::EOrder rotationOrder;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eNull;
	if (pNode->GetNodeAttribute() != NULL)
		attributeType = pNode->GetNodeAttribute()->GetAttributeType();

	FbxNode* parentNode = pNode->GetParent();
	if ((name == "TopJoint") || (parentNode != NULL))
	{
		if (name == "TopJoint")
		{
			for (int x = 0; x < joints.size(); x++)
			{
				if (joints[x]->name == "TopJoint")
				{
					pNode->GetRotationOrder(FbxNode::eSourcePivot, rotationOrder);
					if (rotationOrder == FbxEuler::EOrder::eOrderXZY)
						joints[x]->rotationOrder = "XZY";
					else if (rotationOrder == FbxEuler::EOrder::eOrderYZX)
						joints[x]->rotationOrder = "YZX";
					else if (rotationOrder == FbxEuler::EOrder::eOrderYXZ)
						joints[x]->rotationOrder = "YXZ";
					else if (rotationOrder == FbxEuler::EOrder::eOrderZXY)
						joints[x]->rotationOrder = "ZXY";
					else if (rotationOrder == FbxEuler::EOrder::eOrderZYX)
						joints[x]->rotationOrder = "ZYX";
					break;
				}
			}

			FbxAnimCurve* lAnimCurve = NULL;

			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->translation.x = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->translationIsInterpolated[0] = true;
					
					keyframe->translationHasDerivative[0] = true;
					keyframe->translationDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.translation.x = pNode->LclTranslation.Get().mData[0];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->translation.x = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->translation.x = lKeyValue;
							}
							keyframe->translation.x = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->translation.x) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->translation.x = previousKeyframe->translation.x + (deltaSlice * (y + 1));
								sliceKeyframe->translationIsInterpolated[0] = true;
							}

							keyframe->translation.x = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->translation.x = previousKeyframe->translation.x;
				}*/
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->translation.y = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->translationIsInterpolated[1] = true;

					keyframe->translationHasDerivative[1] = true;
					keyframe->translationDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.translation.y = pNode->LclTranslation.Get().mData[1];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->translation.y = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->translation.y = lKeyValue;
							}
							keyframe->translation.y = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->translation.y) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->translation.y = previousKeyframe->translation.y + (deltaSlice * (y + 1));
								sliceKeyframe->translationIsInterpolated[1] = true;
							}

							keyframe->translation.y = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->translation.y = previousKeyframe->translation.y;
				}*/
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->translation.z = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->translationIsInterpolated[2] = true;

					keyframe->translationHasDerivative[2] = true;
					keyframe->translationDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.translation.z = pNode->LclTranslation.Get().mData[2];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->translation.z = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->translation.z = lKeyValue;
							}
							keyframe->translation.z = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->translation.z) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->translation.z = previousKeyframe->translation.z + (deltaSlice * (y + 1));
								sliceKeyframe->translationIsInterpolated[2] = true;
							}

							keyframe->translation.z = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->translation.z = previousKeyframe->translation.z;
				}*/
			}

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->rotation.x = lAnimCurve->Evaluate(fbxTime);

					while (keyframe->rotation.x >= 360.0f)
						keyframe->rotation.x -= 360.0f;
					while (keyframe->rotation.x < 0.0f)
						keyframe->rotation.x += 360.0f;

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->rotationIsInterpolated[0] = true;

					keyframe->rotationHasDerivative[0] = true;
					keyframe->rotationDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.rotation.x = pNode->LclRotation.Get().mData[0];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					while (lKeyValue >= 360.0f)
						lKeyValue -= 360.0f;
					while (lKeyValue < 0.0f)
						lKeyValue += 360.0f;

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->rotation.x = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->rotation.x = lKeyValue;
							}
							keyframe->rotation.x = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float span = (lKeyValue - previousKeyframe->rotation.x);
							
							while (span >= 360.0f)
								span -= 360.0f;
							while (span < 0.0f)
								span += 360.0f;

							if (fabs(span) > 180.0f)
								span -= 360.0f;

							float deltaSlice = span / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->rotation.x = previousKeyframe->rotation.x + (deltaSlice * (y + 1));
								sliceKeyframe->rotationIsInterpolated[0] = true;
							}

							if (Sign(span) != Sign(lKeyValue))
								keyframe->rotation.x = lKeyValue - 360.0f;
							else
								keyframe->rotation.x = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->rotation.x = previousKeyframe->rotation.x;
				}*/
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->rotation.y = lAnimCurve->Evaluate(fbxTime);

					while (keyframe->rotation.y >= 360.0f)
						keyframe->rotation.y -= 360.0f;
					while (keyframe->rotation.y < 0.0f)
						keyframe->rotation.y += 360.0f;

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->rotationIsInterpolated[1] = true;

					keyframe->rotationHasDerivative[1] = true;
					keyframe->rotationDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}
				/*CKeyframe defaultKeyframe;
				defaultKeyframe.rotation.y = pNode->LclRotation.Get().mData[1];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					while (lKeyValue >= 360.0f)
						lKeyValue -= 360.0f;
					while (lKeyValue < 0.0f)
						lKeyValue += 360.0f;

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->rotation.y = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->rotation.y = lKeyValue;
							}
							keyframe->rotation.y = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float span = (lKeyValue - previousKeyframe->rotation.y);
							
							while (span >= 360.0f)
								span -= 360.0f;
							while (span < 0.0f)
								span += 360.0f;

							if (fabs(span) > 180.0f)
								span -= 360.0f;

							float deltaSlice = span / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->rotation.y = previousKeyframe->rotation.y + (deltaSlice * (y + 1));
								sliceKeyframe->rotationIsInterpolated[1] = true;
							}

							if (Sign(span) != Sign(lKeyValue))
								keyframe->rotation.y = lKeyValue - 360.0f;
							else
								keyframe->rotation.y = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->rotation.y = previousKeyframe->rotation.y;
				}*/
			}
			lAnimCurve = pNode->LclRotation.
				GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->rotation.z = lAnimCurve->Evaluate(fbxTime);

					while (keyframe->rotation.z >= 360.0f)
						keyframe->rotation.z -= 360.0f;
					while (keyframe->rotation.z < 0.0f)
						keyframe->rotation.z += 360.0f;

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->rotationIsInterpolated[2] = true;

					keyframe->rotationHasDerivative[2] = true;
					keyframe->rotationDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.rotation.z = pNode->LclRotation.Get().mData[2];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					while (lKeyValue >= 360.0f)
						lKeyValue -= 360.0f;
					while (lKeyValue < 0.0f)
						lKeyValue += 360.0f;

					if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
						lKeyValue = 0;

					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->rotation.z = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->rotation.z = lKeyValue;
							}
							keyframe->rotation.z = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float span = (lKeyValue - previousKeyframe->rotation.z);
							
							while (span >= 360.0f)
								span -= 360.0f;
							while (span < 0.0f)
								span += 360.0f;

							if (fabs(span) > 180.0f)
								span -= 360.0f;

							float deltaSlice = span / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->rotation.z = previousKeyframe->rotation.z + (deltaSlice * (y + 1));
								sliceKeyframe->rotationIsInterpolated[2] = true;
							}

							if (Sign(span) != Sign(lKeyValue))
								keyframe->rotation.z = lKeyValue - 360.0f;
							else
								keyframe->rotation.z = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->rotation.z = previousKeyframe->rotation.z;
				}*/
			}

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->scale.x = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->scaleIsInterpolated[0] = true;

					keyframe->scaleHasDerivative[0] = true;
					keyframe->scaleDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.scale.x = pNode->LclScaling.Get().mData[0];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->scale.x = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->scale.x = lKeyValue;
							}
							keyframe->scale.x = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->scale.x) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.x = previousKeyframe->scale.x + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[0] = true;
							}

							keyframe->scale.x = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->scale.x = previousKeyframe->scale.x;
				}*/
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->scale.y = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->scaleIsInterpolated[1] = true;

					keyframe->scaleHasDerivative[1] = true;
					keyframe->scaleDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.scale.y = pNode->LclScaling.Get().mData[1];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->scale.y = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->scale.y = lKeyValue;
							}
							keyframe->scale.y = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->scale.y) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.y = previousKeyframe->scale.y + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[1] = true;
							}

							keyframe->scale.y = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->scale.y = previousKeyframe->scale.y;
				}*/
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

				for (int lCount = 0; lCount < numberKeyframes; lCount++)
				{
					FbxTime fbxTime;
					fbxTime.SetFrame(lCount);
					CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
					keyframe->scale.z = lAnimCurve->Evaluate(fbxTime);

					if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
						keyframe->scaleIsInterpolated[2] = true;

					keyframe->scaleHasDerivative[2] = true;
					keyframe->scaleDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
				}

				/*CKeyframe defaultKeyframe;
				defaultKeyframe.scale.z = pNode->LclScaling.Get().mData[2];

				int lKeyCount = lAnimCurve->KeyGetCount();

				int previousKeyframeNumber = -1;
				CKeyframe* previousKeyframe = &defaultKeyframe;

				for(int lCount = 0; lCount < lKeyCount; lCount++)
				{
					float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
					FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
					int keyframeNumber = lKeyTime.GetFrameCount();
					
					CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

					if (keyframeNumber == (previousKeyframeNumber + 1))
					{
						keyframe->scale.z = lKeyValue;
					}
					else
					{
						if (lCount == 0)
						{
							for (int y = 0; y < keyframeNumber; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->scale.z = lKeyValue;
							}
							keyframe->scale.z = lKeyValue;
						}
						else
						{
							int slices = keyframeNumber - (previousKeyframeNumber+1);
							float deltaSlice = (lKeyValue - previousKeyframe->scale.z) / (slices + 1);
							for (int y = 0; y < slices; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

								sliceKeyframe->scale.z = previousKeyframe->scale.z + (deltaSlice * (y + 1));
								sliceKeyframe->scaleIsInterpolated[2] = true;
							}

							keyframe->scale.z = lKeyValue;
						}
					}

					previousKeyframeNumber = keyframeNumber;
					previousKeyframe = keyframe;
				}

				for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
				{
					CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

					sliceKeyframe->scale.z = previousKeyframe->scale.z;
				}*/
			}

			if (joints.size() == 0) // camera get field of view
			{
				if (pNode->GetNodeAttribute() != NULL)
				{
					std::string cameraNodeName = pNode->GetName();
					FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();
					if (attributeType == FbxNodeAttribute::eCamera) 
					{
						FbxCamera* lCamera = (FbxCamera*) pNode->GetNodeAttribute();
						lAnimCurve = lCamera->FieldOfView.GetCurve(pAnimLayer, true);

						CKeyframe defaultKeyframe;
						defaultKeyframe.fieldOfView.contains = true;
						defaultKeyframe.fieldOfView.floatValue = lCamera->FieldOfView.Get();

						if (lAnimCurve)
						{
							for (int lCount = 0; lCount < numberKeyframes; lCount++)
							{
								FbxTime fbxTime;
								fbxTime.SetFrame(lCount);
								CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
								keyframe->fieldOfView.contains = true;
								keyframe->fieldOfView.floatValue = lAnimCurve->Evaluate(fbxTime);
							}

							/*int lKeyCount = lAnimCurve->KeyGetCount();

							int previousKeyframeNumber = -1;
							CKeyframe* previousKeyframe = &defaultKeyframe;

							for(int lCount = 0; lCount < lKeyCount; lCount++)
							{
								float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
								FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
								int keyframeNumber = lKeyTime.GetFrameCount();
								
								CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

								if (keyframeNumber == (previousKeyframeNumber + 1))
								{
									keyframe->fieldOfView.contains = true;
									keyframe->fieldOfView.floatValue = lKeyValue;
								}
								else
								{
									if (lCount == 0)
									{
										for (int y = 0; y < keyframeNumber; y++)
										{
											CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

											sliceKeyframe->fieldOfView.contains = true;
											sliceKeyframe->fieldOfView.floatValue = lKeyValue;
										}
										keyframe->fieldOfView.contains = true;
										keyframe->fieldOfView.floatValue = lKeyValue;
									}
									else
									{
										int slices = keyframeNumber - (previousKeyframeNumber+1);
										float deltaSlice = (lKeyValue - previousKeyframe->translation.z) / (slices + 1);
										for (int y = 0; y < slices; y++)
										{
											CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

											sliceKeyframe->fieldOfView.contains = true;
											sliceKeyframe->fieldOfView.floatValue = previousKeyframe->fieldOfView.floatValue + (deltaSlice * (y + 1));
										}

										keyframe->fieldOfView.contains = true;
										keyframe->fieldOfView.floatValue = lKeyValue;
									}
								}

								previousKeyframeNumber = keyframeNumber;
								previousKeyframe = keyframe;
							}

							for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
							{
								CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

								sliceKeyframe->fieldOfView.contains = true;
								sliceKeyframe->fieldOfView.floatValue = previousKeyframe->fieldOfView.floatValue;
							}*/
						}
					}
				}
			}
		}
		else if ((attributeType == FbxNodeAttribute::eNull) || (attributeType == FbxNodeAttribute::eSkeleton))
		{
			for (int x = 0; x < joints.size(); x++)
			{
				if (joints[x]->name == name)
				{
					pNode->GetRotationOrder(FbxNode::eSourcePivot, rotationOrder);
					if (rotationOrder == FbxEuler::EOrder::eOrderXZY)
						joints[x]->rotationOrder = "XZY";
					else if (rotationOrder == FbxEuler::EOrder::eOrderYZX)
						joints[x]->rotationOrder = "YZX";
					else if (rotationOrder == FbxEuler::EOrder::eOrderYXZ)
						joints[x]->rotationOrder = "YXZ";
					else if (rotationOrder == FbxEuler::EOrder::eOrderZXY)
						joints[x]->rotationOrder = "ZXY";
					else if (rotationOrder == FbxEuler::EOrder::eOrderZYX)
						joints[x]->rotationOrder = "ZYX";

					FbxAnimCurve* lAnimCurve = NULL;

					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->translation.x = lAnimCurve->Evaluate(fbxTime);

							float deltaPartPos = joints[x]->positionAbsolute.x;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.x);
							animationPart->translation.x = animationPart->translation.x - deltaPartPos;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->translationIsInterpolated[0] = true;

							animationPart->translationHasDerivative[0] = true;
							animationPart->translationDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.translation.x = pNode->LclTranslation.Get().mData[0];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							float deltaPartPos = joints[x]->positionAbsolute.x;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.x);
							lKeyValue = lKeyValue - deltaPartPos;
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->translation.x = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.x = lKeyValue;
									}
									animationPart->translation.x = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->translation.x) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.x = previousAnimationPart->translation.x + (deltaSlice * (y + 1));
										sliceAnimationPart->translationIsInterpolated[0] = true;
									}

									animationPart->translation.x = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->translation.x = previousAnimationPart->translation.x;
						}*/
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->translation.y = lAnimCurve->Evaluate(fbxTime);

							float deltaPartPos = joints[x]->positionAbsolute.y;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.y);
							animationPart->translation.y = animationPart->translation.y - deltaPartPos;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->translationIsInterpolated[1] = true;

							animationPart->translationHasDerivative[1] = true;
							animationPart->translationDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.translation.y = pNode->LclTranslation.Get().mData[1];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							float deltaPartPos = joints[x]->positionAbsolute.y;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.y);
							lKeyValue = lKeyValue - deltaPartPos;
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->translation.y = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.y = lKeyValue;
									}
									animationPart->translation.y = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->translation.y) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.y = previousAnimationPart->translation.y + (deltaSlice * (y + 1));
										sliceAnimationPart->translationIsInterpolated[1] = true;
									}

									animationPart->translation.y = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->translation.y = previousAnimationPart->translation.y;
						}*/
					}
					lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->translation.z = lAnimCurve->Evaluate(fbxTime);

							float deltaPartPos = joints[x]->positionAbsolute.z;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.z);
							animationPart->translation.z = animationPart->translation.z - deltaPartPos;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->translationIsInterpolated[2] = true;

							animationPart->translationHasDerivative[2] = true;
							animationPart->translationDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.translation.z = pNode->LclTranslation.Get().mData[2];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							float deltaPartPos = joints[x]->positionAbsolute.z;
							if (joints[x]->parent != NULL)
								deltaPartPos = (deltaPartPos - joints[x]->parent->positionAbsolute.z);
							lKeyValue = lKeyValue - deltaPartPos;
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->translation.z = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.z = lKeyValue;
									}
									animationPart->translation.z = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->translation.z) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->translation.z = previousAnimationPart->translation.z + (deltaSlice * (y + 1));
										sliceAnimationPart->translationIsInterpolated[2] = true;
									}

									animationPart->translation.z = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->translation.z = previousAnimationPart->translation.z;
						}*/
					}

					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->rotation.x = lAnimCurve->Evaluate(fbxTime);

							while (animationPart->rotation.x >= 360.0f)
								animationPart->rotation.x -= 360.0f;
							while (animationPart->rotation.x < 0.0f)
								animationPart->rotation.x += 360.0f;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->rotationIsInterpolated[0] = true;

							animationPart->rotationHasDerivative[0] = true;
							animationPart->rotationDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultPart;
						defaultPart.rotation.x = pNode->LclRotation.Get().mData[0];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							while (lKeyValue >= 360.0f)
								lKeyValue -= 360.0f;
							while (lKeyValue < 0.0f)
								lKeyValue += 360.0f;

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->rotation.x = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.x = lKeyValue;
									}
									animationPart->rotation.x = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float span = (lKeyValue - previousAnimationPart->rotation.x);

									while (span >= 360.0f)
										span -= 360.0f;
									while (span < 0.0f)
										span += 360.0f;

									if (fabs(span) > 180.0f)
										span -= 360.0f;

									float deltaSlice = span / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x + (deltaSlice * (y + 1));
										sliceAnimationPart->rotationIsInterpolated[0] = true;
									}

									if (Sign(span) != Sign(lKeyValue))
										animationPart->rotation.x = lKeyValue - 360.0f;
									else
										animationPart->rotation.x = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->rotation.x = previousAnimationPart->rotation.x;
						}*/
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->rotation.y = lAnimCurve->Evaluate(fbxTime);

							while (animationPart->rotation.y >= 360.0f)
								animationPart->rotation.y -= 360.0f;
							while (animationPart->rotation.y < 0.0f)
								animationPart->rotation.y += 360.0f;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->rotationIsInterpolated[1] = true;

							animationPart->rotationHasDerivative[1] = true;
							animationPart->rotationDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}

						/*CAnimationPart defaultPart;
						defaultPart.rotation.y = pNode->LclRotation.Get().mData[1];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							while (lKeyValue >= 360.0f)
								lKeyValue -= 360.0f;
							while (lKeyValue < 0.0f)
								lKeyValue += 360.0f;

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->rotation.y = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.y = lKeyValue;
									}
									animationPart->rotation.y = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float span = (lKeyValue - previousAnimationPart->rotation.y);
									
									while (span >= 360.0f)
										span -= 360.0f;
									while (span < 0.0f)
										span += 360.0f;

									if (fabs(span) > 180.0f)
										span -= 360.0f;

									float deltaSlice = span / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y + (deltaSlice * (y + 1));
										sliceAnimationPart->rotationIsInterpolated[1] = true;
									}

									if (Sign(span) != Sign(lKeyValue))
										animationPart->rotation.y = lKeyValue - 360.0f;
									else
										animationPart->rotation.y = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->rotation.y = previousAnimationPart->rotation.y;
						}*/
					}
					lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->rotation.z = lAnimCurve->Evaluate(fbxTime);

							while (animationPart->rotation.z >= 360.0f)
								animationPart->rotation.z -= 360.0f;
							while (animationPart->rotation.z < 0.0f)
								animationPart->rotation.z += 360.0f;

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->rotationIsInterpolated[2] = true;

							animationPart->rotationHasDerivative[2] = true;
							animationPart->rotationDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultPart;
						defaultPart.rotation.z = pNode->LclRotation.Get().mData[2];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							while (lKeyValue >= 360.0f)
								lKeyValue -= 360.0f;
							while (lKeyValue < 0.0f)
								lKeyValue += 360.0f;

							if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
								lKeyValue = 0;

							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();
							
							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->rotation.z = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.z = lKeyValue;
									}
									animationPart->rotation.z = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float span = (lKeyValue - previousAnimationPart->rotation.z);
									
									while (span >= 360.0f)
										span -= 360.0f;
									while (span < 0.0f)
										span += 360.0f;

									if (fabs(span) > 180.0f)
										span -= 360.0f;

									float deltaSlice = span / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z + (deltaSlice * (y + 1));
										sliceAnimationPart->rotationIsInterpolated[2] = true;
									}

									if (Sign(span) != Sign(lKeyValue))
										animationPart->rotation.z = lKeyValue - 360.0f;
									else
										animationPart->rotation.z = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->rotation.z = previousAnimationPart->rotation.z;
						}*/
					}
	  
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->scale.x = lAnimCurve->Evaluate(fbxTime);

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->scaleIsInterpolated[0] = true;

							animationPart->scaleHasDerivative[0] = true;
							animationPart->scaleDerivativeValue.x = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.scale.x = pNode->LclScaling.Get().mData[0];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->scale.x = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.x = lKeyValue;
									}
									animationPart->scale.x = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->scale.x) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.x = previousAnimationPart->scale.x + (deltaSlice * (y + 1));
										sliceAnimationPart->scaleIsInterpolated[0] = true;
									}

									animationPart->scale.x = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->scale.x = previousAnimationPart->scale.x;
						}*/
					}
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->scale.y = lAnimCurve->Evaluate(fbxTime);

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->scaleIsInterpolated[1] = true;

							animationPart->scaleHasDerivative[1] = true;
							animationPart->scaleDerivativeValue.y = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.scale.y = pNode->LclScaling.Get().mData[1];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->scale.y = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.y = lKeyValue;
									}
									animationPart->scale.y = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->scale.y) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.y = previousAnimationPart->scale.y + (deltaSlice * (y + 1));
										sliceAnimationPart->scaleIsInterpolated[1] = true;
									}

									animationPart->scale.y = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->scale.y = previousAnimationPart->scale.y;
						}*/
					}
					lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					if (lAnimCurve)
					{
						std::vector<int> keyIndexes = GetKeyframeIndexes(lAnimCurve);

						for (int lCount = 0; lCount < numberKeyframes; lCount++)
						{
							FbxTime fbxTime;
							fbxTime.SetFrame(lCount);
							CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);
							animationPart->scale.z = lAnimCurve->Evaluate(fbxTime);

							if (std::find(keyIndexes.begin(), keyIndexes.end(), lCount) == keyIndexes.end())
								animationPart->scaleIsInterpolated[2] = true;

							animationPart->scaleHasDerivative[2] = true;
							animationPart->scaleDerivativeValue.z = CalculateDerivative(lCount, numberKeyframes, lAnimCurve);
						}
						/*CAnimationPart defaultAnimationPart;
						defaultAnimationPart.scale.z = pNode->LclScaling.Get().mData[2];

						int lKeyCount = lAnimCurve->KeyGetCount();

						int previousKeyframeNumber = -1;
						CAnimationPart* previousAnimationPart = &defaultAnimationPart;

						for(int lCount = 0; lCount < lKeyCount; lCount++)
						{
							float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
							FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
							int keyframeNumber = lKeyTime.GetFrameCount();

							CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);
							CAnimationPart* animationPart = GetAddPart(joints[x]->name, keyframe);

							if (keyframeNumber == (previousKeyframeNumber + 1))
							{
								animationPart->scale.z = lKeyValue;
							}
							else
							{
								if (lCount == 0)
								{
									for (int y = 0; y < keyframeNumber; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.z = lKeyValue;
									}
									animationPart->scale.z = lKeyValue;
								}
								else
								{
									int slices = keyframeNumber - (previousKeyframeNumber+1);
									float deltaSlice = (lKeyValue - previousAnimationPart->scale.z) / (slices + 1);
									for (int y = 0; y < slices; y++)
									{
										CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);
										CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

										sliceAnimationPart->scale.z = previousAnimationPart->scale.z + (deltaSlice * (y + 1));
										sliceAnimationPart->scaleIsInterpolated[2] = true;
									}

									animationPart->scale.z = lKeyValue;
								}
							}

							previousKeyframeNumber = keyframeNumber;
							previousAnimationPart = animationPart;
						}

						for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
						{
							CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);
							CAnimationPart* sliceAnimationPart = GetAddPart(joints[x]->name, sliceKeyframe);

							sliceAnimationPart->scale.z = previousAnimationPart->scale.z;
						}*/
					}
					break;
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		ParseFbxAnimationRecursive(pAnimLayer, pNode->GetChild(lModelCount), joints, animation, numberKeyframes);
    }
}

void CObjToAn8Dlg::ParseFbxCameraRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	if (pNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

		FbxNode* parentNode = pNode->GetParent();
		if (attributeType == FbxNodeAttribute::eCamera)
		{
			int numberKeyframes = 0;
			GetNumberKeyframesFbxCameraRecursive(pAnimLayer, pNode, numberKeyframes, joints);

			if (numberKeyframes > 0)
			{
				CAnimation* animation;
				animation = new CCamera();
				animation->name = name;
				animations.push_back(animation);

				// add initial in order
				for (int x = 0; x < numberKeyframes; x++)
				{
					GetAddKeyframe(x, animation);
				}
				
				FbxAnimCurve* lAnimCurve = NULL;

				lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->translation.x = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.translation.x = pNode->LclTranslation.Get().mData[0];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->translation.x = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->translation.x = lKeyValue;
								}
								keyframe->translation.x = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->translation.x) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->translation.x = previousKeyframe->translation.x + (deltaSlice * (y + 1));
									sliceKeyframe->translationIsInterpolated[0] = true;
								}

								keyframe->translation.x = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->translation.x = previousKeyframe->translation.x;
					}*/
				}
				
				lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->translation.y = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.translation.y = pNode->LclTranslation.Get().mData[1];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->translation.y = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->translation.y = lKeyValue;
								}
								keyframe->translation.y = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->translation.y) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->translation.y = previousKeyframe->translation.y + (deltaSlice * (y + 1));
									sliceKeyframe->translationIsInterpolated[1] = true;
								}

								keyframe->translation.y = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->translation.y = previousKeyframe->translation.y;
					}*/
				}
				lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->translation.z = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.translation.z = pNode->LclTranslation.Get().mData[2];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->translation.z = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->translation.z = lKeyValue;
								}
								keyframe->translation.z = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->translation.z) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->translation.z = previousKeyframe->translation.z + (deltaSlice * (y + 1));
									sliceKeyframe->translationIsInterpolated[2] = true;
								}

								keyframe->translation.z = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->translation.z = previousKeyframe->translation.z;
					}*/
				}

				lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->rotation.x = lAnimCurve->Evaluate(fbxTime);

						while (keyframe->rotation.x >= 360.0f)
							keyframe->rotation.x -= 360.0f;
						while (keyframe->rotation.x < 0.0f)
							keyframe->rotation.x += 360.0f;
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.rotation.x = pNode->LclRotation.Get().mData[0];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						while (lKeyValue >= 360.0f)
							lKeyValue -= 360.0f;
						while (lKeyValue < 0.0f)
							lKeyValue += 360.0f;

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->rotation.x = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->rotation.x = lKeyValue;
								}
								keyframe->rotation.x = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float span = (lKeyValue - previousKeyframe->rotation.x);
								
								while (span >= 360.0f)
									span -= 360.0f;
								while (span < 0.0f)
									span += 360.0f;

								if (fabs(span) > 180.0f)
									span -= 360.0f;

								float deltaSlice = span / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->rotation.x = previousKeyframe->rotation.x + (deltaSlice * (y + 1));
									sliceKeyframe->rotationIsInterpolated[0] = true;
								}

								if (Sign(span) != Sign(lKeyValue))
									keyframe->rotation.x = lKeyValue - 360.0f;
								else
									keyframe->rotation.x = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->rotation.x = previousKeyframe->rotation.x;
					}*/
				}
				lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->rotation.y = lAnimCurve->Evaluate(fbxTime);

						while (keyframe->rotation.y >= 360.0f)
							keyframe->rotation.y -= 360.0f;
						while (keyframe->rotation.y < 0.0f)
							keyframe->rotation.y += 360.0f;
					}
					/*CKeyframe defaultKeyframe;
					defaultKeyframe.rotation.y = pNode->LclRotation.Get().mData[1];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						while (lKeyValue >= 360.0f)
							lKeyValue -= 360.0f;
						while (lKeyValue < 0.0f)
							lKeyValue += 360.0f;

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->rotation.y = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->rotation.y = lKeyValue;
								}
								keyframe->rotation.y = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float span = (lKeyValue - previousKeyframe->rotation.y);
								
								while (span >= 360.0f)
									span -= 360.0f;
								while (span < 0.0f)
									span += 360.0f;

								if (fabs(span) > 180.0f)
									span -= 360.0f;

								float deltaSlice = span / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->rotation.y = previousKeyframe->rotation.y + (deltaSlice * (y + 1));
									sliceKeyframe->rotationIsInterpolated[1] = true;
								}

								if (Sign(span) != Sign(lKeyValue))
									keyframe->rotation.y = lKeyValue - 360.0f;
								else
									keyframe->rotation.y = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->rotation.y = previousKeyframe->rotation.y;
					}*/
				}
				lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->rotation.z = lAnimCurve->Evaluate(fbxTime);

						while (keyframe->rotation.z >= 360.0f)
							keyframe->rotation.z -= 360.0f;
						while (keyframe->rotation.z < 0.0f)
							keyframe->rotation.z += 360.0f;
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.rotation.z = pNode->LclRotation.Get().mData[2];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						while (lKeyValue >= 360.0f)
							lKeyValue -= 360.0f;
						while (lKeyValue < 0.0f)
							lKeyValue += 360.0f;

						if ((fabs(lKeyValue) < EPSILONVALUES) || (fabs(360 - lKeyValue) < EPSILONVALUES))
							lKeyValue = 0;

						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->rotation.z = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->rotation.z = lKeyValue;
								}
								keyframe->rotation.z = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float span = (lKeyValue - previousKeyframe->rotation.z);
								
								while (span >= 360.0f)
									span -= 360.0f;
								while (span < 0.0f)
									span += 360.0f;

								if (fabs(span) > 180.0f)
									span -= 360.0f;

								float deltaSlice = span / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->rotation.z = previousKeyframe->rotation.z + (deltaSlice * (y + 1));
									sliceKeyframe->rotationIsInterpolated[2] = true;
								}

								if (Sign(span) != Sign(lKeyValue))
									keyframe->rotation.z = lKeyValue - 360.0f;
								else
									keyframe->rotation.z = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->rotation.z = previousKeyframe->rotation.z;
					}*/
				}

				lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->scale.x = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.scale.x = pNode->LclScaling.Get().mData[0];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->scale.x = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->scale.x = lKeyValue;
								}
								keyframe->scale.x = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->scale.x) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->scale.x = previousKeyframe->scale.x + (deltaSlice * (y + 1));
									sliceKeyframe->scaleIsInterpolated[0] = true;
								}

								keyframe->scale.x = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->scale.x = previousKeyframe->scale.x;
					}*/
				}
				lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->scale.y = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.scale.y = pNode->LclScaling.Get().mData[1];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->scale.y = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->scale.y = lKeyValue;
								}
								keyframe->scale.y = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->scale.y) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->scale.y = previousKeyframe->scale.y + (deltaSlice * (y + 1));
									sliceKeyframe->scaleIsInterpolated[1] = true;
								}

								keyframe->scale.y = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->scale.y = previousKeyframe->scale.y;
					}*/
				}
				lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				if (lAnimCurve)
				{
					for (int lCount = 0; lCount < numberKeyframes; lCount++)
					{
						FbxTime fbxTime;
						fbxTime.SetFrame(lCount);
						CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
						keyframe->scale.z = lAnimCurve->Evaluate(fbxTime);
					}

					/*CKeyframe defaultKeyframe;
					defaultKeyframe.scale.z = pNode->LclScaling.Get().mData[2];

					int lKeyCount = lAnimCurve->KeyGetCount();

					int previousKeyframeNumber = -1;
					CKeyframe* previousKeyframe = &defaultKeyframe;

					for(int lCount = 0; lCount < lKeyCount; lCount++)
					{
						float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
						FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
						int keyframeNumber = lKeyTime.GetFrameCount();
						
						CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

						if (keyframeNumber == (previousKeyframeNumber + 1))
						{
							keyframe->scale.z = lKeyValue;
						}
						else
						{
							if (lCount == 0)
							{
								for (int y = 0; y < keyframeNumber; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->scale.z = lKeyValue;
								}
								keyframe->scale.z = lKeyValue;
							}
							else
							{
								int slices = keyframeNumber - (previousKeyframeNumber+1);
								float deltaSlice = (lKeyValue - previousKeyframe->scale.z) / (slices + 1);
								for (int y = 0; y < slices; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

									sliceKeyframe->scale.z = previousKeyframe->scale.z + (deltaSlice * (y + 1));
									sliceKeyframe->scaleIsInterpolated[2] = true;
								}

								keyframe->scale.z = lKeyValue;
							}
						}

						previousKeyframeNumber = keyframeNumber;
						previousKeyframe = keyframe;
					}

					for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
					{
						CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

						sliceKeyframe->scale.z = previousKeyframe->scale.z;
					}*/
				}

				if (joints.size() == 0) // camera get field of view
				{
					if (pNode->GetNodeAttribute() != NULL)
					{
						std::string cameraNodeName = pNode->GetName();
						FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();
						if (attributeType == FbxNodeAttribute::eCamera) 
						{
							FbxCamera* lCamera = (FbxCamera*) pNode->GetNodeAttribute();
							lAnimCurve = lCamera->FieldOfView.GetCurve(pAnimLayer, true);

							CKeyframe defaultKeyframe;
							defaultKeyframe.fieldOfView.contains = true;
							defaultKeyframe.fieldOfView.floatValue = lCamera->FieldOfView.Get();

							if (lAnimCurve)
							{
								for (int lCount = 0; lCount < numberKeyframes; lCount++)
								{
									FbxTime fbxTime;
									fbxTime.SetFrame(lCount);
									CKeyframe* keyframe = GetAddKeyframe(lCount, animation);
									keyframe->fieldOfView.contains = true;
									keyframe->fieldOfView.floatValue = lAnimCurve->Evaluate(fbxTime);
								}

								/*int lKeyCount = lAnimCurve->KeyGetCount();

								int previousKeyframeNumber = -1;
								CKeyframe* previousKeyframe = &defaultKeyframe;

								for(int lCount = 0; lCount < lKeyCount; lCount++)
								{
									float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(lCount));
									FbxTime lKeyTime  = lAnimCurve->KeyGetTime(lCount);
									int keyframeNumber = lKeyTime.GetFrameCount();
									
									CKeyframe* keyframe = GetAddKeyframe(keyframeNumber, animation);

									if (keyframeNumber == (previousKeyframeNumber + 1))
									{
										keyframe->fieldOfView.contains = true;
										keyframe->fieldOfView.floatValue = lKeyValue;
									}
									else
									{
										if (lCount == 0)
										{
											for (int y = 0; y < keyframeNumber; y++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

												sliceKeyframe->fieldOfView.contains = true;
												sliceKeyframe->fieldOfView.floatValue = lKeyValue;
											}
											keyframe->fieldOfView.contains = true;
											keyframe->fieldOfView.floatValue = lKeyValue;
										}
										else
										{
											int slices = keyframeNumber - (previousKeyframeNumber+1);
											float deltaSlice = (lKeyValue - previousKeyframe->translation.z) / (slices + 1);
											for (int y = 0; y < slices; y++)
											{
												CKeyframe* sliceKeyframe = GetAddKeyframe(previousKeyframeNumber + (y + 1), animation);

												sliceKeyframe->fieldOfView.contains = true;
												sliceKeyframe->fieldOfView.floatValue = previousKeyframe->fieldOfView.floatValue + (deltaSlice * (y + 1));
											}

											keyframe->fieldOfView.contains = true;
											keyframe->fieldOfView.floatValue = lKeyValue;
										}
									}

									previousKeyframeNumber = keyframeNumber;
									previousKeyframe = keyframe;
								}

								for (int y = (previousKeyframeNumber + 1); y < numberKeyframes; y++)
								{
									CKeyframe* sliceKeyframe = GetAddKeyframe(y, animation);

									sliceKeyframe->fieldOfView.contains = true;
									sliceKeyframe->fieldOfView.floatValue = previousKeyframe->fieldOfView.floatValue;
								}*/
							}
						}
					}
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		ParseFbxCameraRecursive(pAnimLayer, pNode->GetChild(lModelCount), joints, animations);
    }
}

void CObjToAn8Dlg::ContainsFbxBlendShapeRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool& containsBlendShape)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	if (pNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

		FbxNode* parentNode = pNode->GetParent();
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh->GetNode())
			{
				FbxNode* pMeshNode = pMesh->GetNode();
				CString meshName = pMeshNode->GetName();;

				if (meshName.Find(":") != -1)
				{
					meshName = meshName.Mid(meshName.Find(":") + 1);
				}

				int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);

				if (lBlendShapeDeformerCount > 0)
				{
					containsBlendShape = true;
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		ContainsFbxBlendShapeRecursive(pAnimLayer, pNode->GetChild(lModelCount), containsBlendShape);
    }
}

void CObjToAn8Dlg::ParseFbxBlendShapeRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CJoint*>& joints, CAnimation* animation, bool noGroups, int& verticesOffset)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	if (pNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

		FbxNode* parentNode = pNode->GetParent();
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh->GetNode())
			{
				FbxNode* pMeshNode = pMesh->GetNode();
				CString meshName = pMeshNode->GetName();;

				if (meshName.Find(":") != -1)
				{
					meshName = meshName.Mid(meshName.Find(":") + 1);
				}

				CGroup* group = NULL;
				if (!noGroups)
				{
					for (int x = 0; x < groups.size(); x++)
					{
						if (groups[x]->name == meshName)
						{
							group = groups[x];
							break;
						}
					}
				}
				
				if (group == NULL)
				{
					if (groups.size() > 0)
					{
						group = groups[0];
					}

					for (int x = 0; x < groups.size(); x++)
					{
						if (groups[x]->polygons.size() > 0)
						{
							group = groups[x];
							break;
						}
					}
				}

				int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);

				if (lBlendShapeDeformerCount > 0)
				{
					int numberShapeControlPoints = 0;
					for(int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex)
					{
						FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

						int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
						for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
						{
							FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);

							if(lChannel)
							{
								FbxShape* lShape = lShape = lChannel->GetTargetShape(0);

								if (lShape)
								{
									// Get the percentage of influence of the shape.
									FbxAnimCurve* lAnimCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);

									int numberKeyframes = 0;
									for (int x = 0; x < lAnimCurve->KeyGetCount(); x++)
									{
										int currentKeyFrame = lAnimCurve->KeyGet(x).GetTime().GetFrameCount();
										if ((currentKeyFrame + 1) > numberKeyframes)
											numberKeyframes = currentKeyFrame + 1;
									}

									// add initial in order
									for (int x = 0; x < numberKeyframes; x++)
									{
										GetAddKeyframe(x, animation);
									}

									if (lAnimCurve)
									{
										for (int lCount = 0; lCount < numberKeyframes; lCount++)
										{
											FbxTime fbxTime;
											fbxTime.SetFrame(lCount);

											double lWeight = lAnimCurve->Evaluate(fbxTime) * 0.01;

											if (lWeight > 0)
											{
												numberShapeControlPoints = lShape->GetControlPointsCount();
												FbxVector4* shapeControlPoints = lShape->GetControlPoints();
												CKeyframe* keyframe = GetAddKeyframe(lCount, animation);

												for (int x = 0; x < numberShapeControlPoints; x++)
												{
													if ((verticesOffset + x) >= (keyframe->animationParts.size()))
													{
														CAnimationPart* animationPart = new CAnimationPart();
														CJoint* joint = joints[verticesOffset + x];
														animationPart->name = joint->name;
														animationPart->translation = float3(0, 0, 0);
														keyframe->animationParts.push_back(animationPart);

														animationPart->translation.x = ((shapeControlPoints[x][0] - joints[verticesOffset + x]->positionAbsolute.x) * lWeight);
														animationPart->translation.y = ((shapeControlPoints[x][1] - joints[verticesOffset + x]->positionAbsolute.y) * lWeight);
														animationPart->translation.z = ((shapeControlPoints[x][2] - joints[verticesOffset + x]->positionAbsolute.z) * lWeight);
													}
													else
													{
														keyframe->animationParts[(verticesOffset + x)]->translation.x += ((shapeControlPoints[x][0] - joints[verticesOffset + x]->positionAbsolute.x) * lWeight);
														keyframe->animationParts[(verticesOffset + x)]->translation.y += ((shapeControlPoints[x][1] - joints[verticesOffset + x]->positionAbsolute.y) * lWeight);
														keyframe->animationParts[(verticesOffset + x)]->translation.z += ((shapeControlPoints[x][2] - joints[verticesOffset + x]->positionAbsolute.z) * lWeight);
													}
												}
											}
										}
									}
								}
							}
						}
					}

					verticesOffset += numberShapeControlPoints;
				}
			}
		}
	}

	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
		ParseFbxBlendShapeRecursive(pAnimLayer, pNode->GetChild(lModelCount), vertices, groups, joints, animation, noGroups, verticesOffset);
    }
}

void CObjToAn8Dlg::ParseFbxBlendShapePartRecursive(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CJoint*>& joints, CAnimation* animation, bool noGroups, int& verticesOffset)
{
	if (pNode == NULL)
		return;

	CString name = pNode->GetName();

	//TODO make flag for special
	if (name.Find(":") != -1)
	{
		name = name.Mid(name.Find(":") + 1);
	}

	if (name.Find("_") == 0)
		name = name.Mid(1);

	if (pNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

		FbxNode* parentNode = pNode->GetParent();
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh->GetNode())
			{
				FbxNode* pMeshNode = pMesh->GetNode();
				CString meshName = pMeshNode->GetName();;

				if (meshName.Find(":") != -1)
				{
					meshName = meshName.Mid(meshName.Find(":") + 1);
				}

				CGroup* group = NULL;
				if (!noGroups)
				{
					for (int x = 0; x < groups.size(); x++)
					{
						if (groups[x]->name == meshName)
						{
							group = groups[x];
							break;
						}
					}
				}

				if (group == NULL)
				{
					if (groups.size() > 0)
					{
						group = groups[0];
					}

					for (int x = 0; x < groups.size(); x++)
					{
						if (groups[x]->polygons.size() > 0)
						{
							group = groups[x];
							break;
						}
					}
				}

				int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);

				if (lBlendShapeDeformerCount > 0)
				{
					if (meshName.Find("_JointAssociation") != -1)
					{
						CString jointAssociationName = meshName.Mid(meshName.Find("_JointAssociation") + 17);
						group->name = group->name.Mid(0, group->name.Find("_JointAssociation"));

						int numberShapeControlPoints = 0;
						for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex)
						{
							FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

							int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
							for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex)
							{
								FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);

								if (lChannel)
								{
									FbxShape* lShape = lShape = lChannel->GetTargetShape(0);

									if (lShape)
									{
										// Get the percentage of influence of the shape.
										FbxAnimCurve* lAnimCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);

										int numberKeyframes = 0;
										for (int x = 0; x < lAnimCurve->KeyGetCount(); x++)
										{
											int currentKeyFrame = lAnimCurve->KeyGet(x).GetTime().GetFrameCount();
											if ((currentKeyFrame + 1) > numberKeyframes)
												numberKeyframes = currentKeyFrame + 1;
										}

										// add initial in order
										for (int x = 0; x < numberKeyframes; x++)
										{
											GetAddKeyframe(x, animation);
										}

										if (lAnimCurve)
										{
											for (int lCount = 0; lCount < numberKeyframes; lCount++)
											{
												FbxTime fbxTime;
												fbxTime.SetFrame(lCount);

												double lWeight = lAnimCurve->Evaluate(fbxTime) * 0.01;

												if (lWeight > 0)
												{
													numberShapeControlPoints = lShape->GetControlPointsCount();
													FbxVector4* shapeControlPoints = lShape->GetControlPoints();
													FbxVector4* lMeshControlPoints = pMesh->GetControlPoints();
													CKeyframe* keyframe = GetAddKeyframe(lCount, animation);

													for (int z = 0; z < joints.size(); z++)
													{
														if (joints[z]->name == jointAssociationName)
														{
															joints[z]->isShapeAnimationJoint = true;

															CAnimationPart* animationPart = new CAnimationPart();
															animationPart->name = joints[z]->name;

															for (int x = 0; x < numberShapeControlPoints; x++)
															{
																CAnimationShapeVertex animationShapeVertex;

																animationShapeVertex.vertexIndex = joints[z]->controlPoints[x];
																animationShapeVertex.translation.x = ((shapeControlPoints[x][0] - vertices[animationShapeVertex.vertexIndex]->vertex.x));
																animationShapeVertex.translation.y = ((shapeControlPoints[x][1] - vertices[animationShapeVertex.vertexIndex]->vertex.y));
																animationShapeVertex.translation.z = ((shapeControlPoints[x][2] - vertices[animationShapeVertex.vertexIndex]->vertex.z));
																animationPart->shapeVertices.push_back(animationShapeVertex);
															}
															keyframe->animationParts.push_back(animationPart);

															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						verticesOffset += numberShapeControlPoints;
					}
				}
			}
		}
	}

	for (int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		ParseFbxBlendShapePartRecursive(pAnimLayer, pNode->GetChild(lModelCount), vertices, groups, joints, animation, noGroups, verticesOffset);
	}
}

void CObjToAn8Dlg::ParseFbxSkeletonRecursive(FbxNode* pNode, std::vector<CJoint*>& joints, float3 position, CJoint* parent, map<CString, float3> skeletalOverrides, map<CString, float3> skeletalOverridesRelativeScale, map<CString, float3> skeletalOverridesRelativeRotation, map<CString, float3> skeletalOverridesRelativePosition, JointType jointType)
{
	if (pNode == NULL)
		return;

	FbxVector4 lclScaling = pNode->LclScaling.Get();
	FbxVector4 lclRotation = pNode->LclRotation.Get();
	FbxVector4 lclTranslation = pNode->LclTranslation.Get();

	CString nodeName = pNode->GetName();

	if (nodeName.Find(":") != -1)
	{
		nodeName = nodeName.Mid(nodeName.Find(":") + 1);
	}

	if (nodeName.Find("_") == 0)
		nodeName = nodeName.Mid(1);

	FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eNull;
	if (pNode->GetNodeAttribute() != NULL)
		attributeType = pNode->GetNodeAttribute()->GetAttributeType();

	if ((attributeType == FbxNodeAttribute::eSkeleton) || (attributeType == FbxNodeAttribute::eNull)) // eNull is from XSI for some reason
	{
		//FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

		//int skeletonType = lSkeleton->GetSkeletonType();
		//if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
		{
			CString skeletonName;
			if (pNode->GetNodeAttribute() != NULL)
				skeletonName = pNode->GetNodeAttribute()->GetName();

			FbxNode* parentNode = pNode->GetParent();
			if ((nodeName == "TopJoint") || (parentNode != NULL))
			{
				if (nodeName.Find("TopJoint") == -1)
				{
					CJoint* joint = new CJoint();
					joint->name = nodeName;

					if (jointType == Absolute)
					{
						joint->jointType = Absolute;
						if (skeletalOverrides.find(nodeName) == skeletalOverrides.end())
						{
							position.x += lclTranslation[0];
							position.y += lclTranslation[1];
							position.z += lclTranslation[2];
							
							joint->positionAbsolute.x = position.x;
							joint->positionAbsolute.y = position.y;
							joint->positionAbsolute.z = position.z;
						}
						else
						{
							position.x = skeletalOverrides[nodeName].x;
							position.y = skeletalOverrides[nodeName].y;
							position.z = skeletalOverrides[nodeName].z;
							
							joint->positionAbsolute.x = position.x;
							joint->positionAbsolute.y = position.y;
							joint->positionAbsolute.z = position.z;
						}
					}
					else if (jointType == Relative)
					{
						joint->jointType = Relative;
						if (skeletalOverridesRelativeScale.find(nodeName) == skeletalOverridesRelativeScale.end())
						{
							joint->scaleRelative.x = lclScaling[0];
							joint->scaleRelative.y = lclScaling[1];
							joint->scaleRelative.z = lclScaling[2];

							joint->rotationRelative.x = lclRotation[0];
							joint->rotationRelative.y = lclRotation[1];
							joint->rotationRelative.z = lclRotation[2];

							joint->positionRelative.x = lclTranslation[0];
							joint->positionRelative.y = lclTranslation[1];
							joint->positionRelative.z = lclTranslation[2];
						}
						else
						{
							joint->scaleRelative.x = skeletalOverridesRelativeScale[nodeName].x;
							joint->scaleRelative.y = skeletalOverridesRelativeScale[nodeName].y;
							joint->scaleRelative.z = skeletalOverridesRelativeScale[nodeName].z;

							joint->rotationRelative.x = skeletalOverridesRelativeRotation[nodeName].x;
							joint->rotationRelative.y = skeletalOverridesRelativeRotation[nodeName].y;
							joint->rotationRelative.z = skeletalOverridesRelativeRotation[nodeName].z;

							joint->positionRelative.x = skeletalOverridesRelativePosition[nodeName].x;
							joint->positionRelative.y = skeletalOverridesRelativePosition[nodeName].y;
							joint->positionRelative.z = skeletalOverridesRelativePosition[nodeName].z;
						}
					}

					joint->parent = parent;

					if (parent != NULL)
					{
						parent->children.push_back(joint);
					}

					parent = joint;

					joints.push_back(joint);
				}
				/*else
				{
					if (skeletalOverrides.find(nodeName) == skeletalOverrides.end())
					{
						position.x += lclTranslation[0];
						position.y += lclTranslation[1];
						position.z += lclTranslation[2];
					}
					else
					{
						position.x = skeletalOverrides[nodeName].x;
						position.y = skeletalOverrides[nodeName].y;
						position.z = skeletalOverrides[nodeName].z;
					}
				}*/
			}
		}
	}

	
	for(int i = 0; i < pNode->GetChildCount(); i++)
    {
        // recursively call this
		ParseFbxSkeletonRecursive(pNode->GetChild(i), joints, position, parent, skeletalOverrides, skeletalOverridesRelativeScale, skeletalOverridesRelativeRotation, skeletalOverridesRelativePosition, jointType);
    }
}

void CObjToAn8Dlg::ParseFbxNodeRecursive(FbxNode* pNode, CGroup* currentGroup, CString inputFile, std::vector<CVerticeColor*>& verticeColors, std::vector<CNormal*>& normals, std::vector<CUVCoordinate*>& uvCoordinates, std::vector<CVertice*>& vertices, std::vector<CGroup*>& groups, std::vector<CMaterialFile*>& materialFiles, std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
										 bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool noGroups, CString& errorString, JointType jointType)
{
	if (pNode == NULL)
		return;

	if (pNode->GetNodeAttribute() != NULL)
	{
		CString nodeName = pNode->GetName();

		FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			// scaling -> rotation -> translation
			
			FbxAMatrix matrix = pNode->EvaluateGlobalTransform();

			// Apply geometric transformation (from 3ds max)
			FbxAMatrix matrixGeo;
			const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matrixGeo.SetT(lT);
			matrixGeo.SetR(lR);
			matrixGeo.SetS(lS);

			matrix  = matrix  * matrixGeo;

			FbxDouble3 scaling = matrix.GetS();
			FbxDouble3 rotation = matrix.GetR();
			FbxDouble3 translation = matrix.GetT();


			if (pMesh->GetNode())
			{
				// Same as pNode (is pMesh->GetNode())
				FbxNode* pMeshNode = pMesh->GetNode();
				CString meshName = pMeshNode->GetName();;

				if (!noGroups)
				{
					CGroup* newGroup = new CGroup();

					if (meshName.Find(":") != -1)
					{
						meshName = meshName.Mid(meshName.Find(":") + 1);
					}

					newGroup->name = meshName;

					groups.push_back(newGroup);

					currentGroup = newGroup;
				}


				CMaterialFile* currentMaterialFile;
				if (materialFiles.size() == 0)
				{
					currentMaterialFile = new CMaterialFile();
					currentMaterialFile->name = "DefaultMaterialFile";
					materialFiles.push_back(currentMaterialFile);
				}
				else
				{
					currentMaterialFile = materialFiles[0];
				}

				int meshMaterialOffset = currentMaterialFile->materials.size();

				int lMaterialCount = pNode->GetMaterialCount();

				if (lMaterialCount > 0)
				{
					FbxPropertyT<FbxDouble3> lKFbxDouble3;
					FbxPropertyT<FbxDouble> lKFbxDouble1;
					FbxColor theColor;

					for (int lCount = 0; lCount < lMaterialCount; lCount ++)
					{
						FbxSurfaceMaterial *lMaterial = pNode->GetMaterial(lCount);

						CMaterial* newMaterial = new CMaterial();
						newMaterial->name.Format("%s", lMaterial->GetName());

						currentMaterialFile->materials.push_back(newMaterial);

						const char* textureName = GetMaterialTextureName(lMaterial, FbxSurfaceMaterial::sDiffuse);

						if (textureName != NULL)
						{
							newMaterial->diffuseTexture.stringValue.Format("%s", textureName);
							newMaterial->diffuseTexture.contains = true;
						}

						//FbxDouble3 diffuseColorTriangle = GetMaterialColor(lMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
						//newMaterial->diffuseColor.r = diffuseColorTriangle[0];
						//newMaterial->diffuseColor.g = diffuseColorTriangle[1];
						//newMaterial->diffuseColor.b = diffuseColorTriangle[2];
						

						if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
						{
							// Display the Ambient Color
							lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Ambient;
							theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							newMaterial->ambientColor.r = theColor.mRed;
							newMaterial->ambientColor.g = theColor.mGreen;
							newMaterial->ambientColor.b = theColor.mBlue;
							newMaterial->ambientColor.contains = true;

							// Display the Diffuse Color
							lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Diffuse;
							theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							newMaterial->diffuseColor.r = theColor.mRed;
							newMaterial->diffuseColor.g = theColor.mGreen;
							newMaterial->diffuseColor.b = theColor.mBlue;
							newMaterial->diffuseColor.contains = true;

							// Display the Specular Color (unique to Phong materials)
							lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Specular;
							theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							newMaterial->spectralColor.r = theColor.mRed;
							newMaterial->spectralColor.g = theColor.mGreen;
							newMaterial->spectralColor.b = theColor.mBlue;
							newMaterial->spectralColor.contains = true;

							// Display the Emissive Color
							//lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Emissive;
							//theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							//DisplayColor("            Emissive: ", theColor);

							//Opacity is Transparency factor now, originally was 1- in code
							lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->TransparencyFactor;
							newMaterial->alpha.floatValue = lKFbxDouble1.Get();
							newMaterial->alpha.contains = true;

							// Display the Shininess
							lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->Shininess;
							newMaterial->phongSpectral.contains = true;
							newMaterial->phongSpectral.floatValue = (float)lKFbxDouble1.Get();

							// Display the Reflectivity
							lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->ReflectionFactor;
							newMaterial->illum.contains = true;
							newMaterial->illum.floatValue = (float)lKFbxDouble1.Get();
						}
						else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
						{
							// Display the Ambient Color
							lKFbxDouble3=((FbxSurfaceLambert *)lMaterial)->Ambient;
							theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							newMaterial->ambientColor.r = theColor.mRed;
							newMaterial->ambientColor.g = theColor.mGreen;
							newMaterial->ambientColor.b = theColor.mBlue;
							newMaterial->ambientColor.contains = true;

							// Display the Diffuse Color
							lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Diffuse;
							theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							newMaterial->diffuseColor.r = theColor.mRed;
							newMaterial->diffuseColor.g = theColor.mGreen;
							newMaterial->diffuseColor.b = theColor.mBlue;
							newMaterial->diffuseColor.contains = true;

							// Display the Emissive
							//lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Emissive;
							//theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
							//DisplayColor("            Emissive: ", theColor);

							// Display the Opacity
							lKFbxDouble1 =((FbxSurfaceLambert *)lMaterial)->TransparencyFactor;
							newMaterial->alpha.floatValue = lKFbxDouble1.Get();
							newMaterial->alpha.contains = true;
						}
						else
						{
							// unknown type
						}
					}
				}

				int lControlPointsCount = pMesh->GetControlPointsCount();
				FbxVector4* lControlPoints = pMesh->GetControlPoints();

				int meshVerticeOffset = vertices.size();
				int meshUvOffset = uvCoordinates.size();
				int meshNormalsOffset = normals.size();

				unsigned int numOfDeformers = pMesh->GetDeformerCount();

				for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; deformerIndex++)
				{
					FbxDeformer* deformer = pMesh->GetDeformer(deformerIndex);
					FbxSkin* currSkin = NULL;
					if (deformer->Is<FbxSkin>()) 
						currSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
					if (currSkin == NULL)
					{
						FbxBlendShape* lBlendShape = NULL;
						if (deformer->Is<FbxBlendShape>())
							lBlendShape = reinterpret_cast<FbxBlendShape*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eBlendShape));

						if (lBlendShape != NULL)
						{
							if (meshName.Find("_JointAssociation") != -1)
							{
								CString jointAssociationName = meshName.Mid(meshName.Find("_JointAssociation") + 17);

								CJoint* matchedJoint = NULL;
								for (int r = 0; r < joints.size(); r++)
								{
									if (joints[r]->name == jointAssociationName)
									{
										matchedJoint = joints[r];
										break;
									}
								}

								if (matchedJoint != NULL)
								{
									int numberMeshControlPoints = pMesh->GetControlPointsCount();

									for (int lIndex = 0; lIndex < numberMeshControlPoints; lIndex++)
									{
										matchedJoint->controlPoints.push_back(meshVerticeOffset + lIndex);
									}
								}
							}
						}
					}
					else
					{
						int numOfClusters = currSkin->GetClusterCount();
						for (int clusterIndex = 0; clusterIndex < numOfClusters; clusterIndex++)
						{
							FbxCluster* lCluster = currSkin->GetCluster(clusterIndex);

							FbxAMatrix transformMatrix;
							lCluster->GetTransformMatrix(transformMatrix);

							FbxAMatrix transformLinkMatrix;
							lCluster->GetTransformLinkMatrix(transformLinkMatrix);

							FbxAMatrix finalTransformMatrix = transformLinkMatrix.Inverse() * transformMatrix;

							if (!lCluster->GetLink())
								continue;

							int linkMode = lCluster->GetLinkMode();

							FbxNode* link = lCluster->GetLink();
							CString currJointName = link->GetName();

							if (currJointName.Find(":") != -1)
							{
								currJointName = currJointName.Mid(currJointName.Find(":") + 1);
							}

							if (currJointName.Find("_") == 0)
								currJointName = currJointName.Mid(1);

							int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

							if (lVertexIndexCount > 0)
							{
								CJoint* matchedJoint = NULL;
								for (int r = 0; r < joints.size(); r++)
								{
									if (joints[r]->name == currJointName)
									{
										matchedJoint = joints[r];
										break;
									}
								}

								if (matchedJoint != NULL)
								{
									for (int k = 0; k < lVertexIndexCount; ++k)
									{
										int lIndex = lCluster->GetControlPointIndices()[k];

										FbxCluster::ELinkMode clusterMode = lCluster->GetLinkMode();

										// Sometimes, the mesh can have less points than at the time of the skinning
										// because a smooth operator was active when skinning but has been deactivated during export.
										if (lIndex >= lControlPointsCount)
											continue;

										double lWeight = lCluster->GetControlPointWeights()[k];

										if (fabs(lWeight - 1.0) > EPSILONVALUES)
											//if (lWeight != 1.0)
										{
											if (fabs(lWeight) > EPSILONVALUES)
												//if (lWeight != 0.0)
											{
												CString tempStr;
												tempStr.Format("Node %s Joint %s has weight that is %f, not 1.0 Vertice %d\n", nodeName, matchedJoint->name, lWeight, lIndex);
												errorString += tempStr;
											}
										}

										matchedJoint->controlPoints.push_back(meshVerticeOffset + lIndex);
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < lControlPointsCount; i++)
				{
					CVertice* newVertice = new CVertice();
				
					FbxVector4 tempVector = lControlPoints[i];
					tempVector = matrix.MultT(tempVector);

					newVertice->vertex.x = tempVector[0];
					newVertice->vertex.y = tempVector[1];
					newVertice->vertex.z = tempVector[2];

					bool foundControlPoint = false;

					for (int r = 0; r < joints.size(); r++)
					{
						if ((std::find(joints[r]->controlPoints.begin(), joints[r]->controlPoints.end(), (meshVerticeOffset+i)) != joints[r]->controlPoints.end()))
						{
							FbxAMatrix finalTransformMatrix;
							finalTransformMatrix.SetIdentity();

							for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; deformerIndex++)
							{
								FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
								if (currSkin == NULL)
								{
									continue;
								}

								int numOfClusters = currSkin->GetClusterCount();
								for (int clusterIndex = 0; clusterIndex < numOfClusters; clusterIndex++)
								{
									FbxCluster* lCluster = currSkin->GetCluster(clusterIndex);

									FbxNode* link = lCluster->GetLink();
									CString currJointName = link->GetName();

									if (currJointName.Find(":") != -1)
									{
										currJointName = currJointName.Mid(currJointName.Find(":") + 1);
									}

									if (currJointName.Find("_") == 0)
										currJointName = currJointName.Mid(1);

									if (currJointName == joints[r]->name)
									{
										FbxCluster* lCluster = currSkin->GetCluster(clusterIndex);

										FbxAMatrix transformLinkMatrix;
										lCluster->GetTransformLinkMatrix(transformLinkMatrix);

										finalTransformMatrix = transformLinkMatrix.Inverse();
										break;
									}
								}
							}

							// Compensate for if transform link in FBX
							FbxVector4 tempVertex(newVertice->vertex.x, newVertice->vertex.y, newVertice->vertex.z);
							tempVertex = finalTransformMatrix.MultT(tempVertex);
							newVertice->vertex = float3(tempVertex.mData[0], tempVertex.mData[1], tempVertex.mData[2]);

							newVertice->vertex.x += joints[r]->positionAbsolute.x;
							newVertice->vertex.y += joints[r]->positionAbsolute.y;
							newVertice->vertex.z += joints[r]->positionAbsolute.z;
							foundControlPoint = true;
							break;
						}
					}

					vertices.push_back(newVertice);
				}

				int lPolygonCount = pMesh->GetPolygonCount();
				
				int vertexId = 0;
				for (int i = 0; i < lPolygonCount; i++)
				{
					int lPolygonSize = pMesh->GetPolygonSize(i);

					CPolygon* newPolygon = new CPolygon();
					currentGroup->polygons.push_back(newPolygon);


					for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
					{

						FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
						if (lMaterialElement != NULL)
						{
							FbxSurfaceMaterial* lMaterial = NULL;
							int lMatId = -1;
							lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
							lMatId = lMaterialElement->GetIndexArray().GetAt(i);
							if (lMatId >= 0)
							{
								newPolygon->materialName = lMaterial->GetName();
							}
						}
					}

					for (int j = 0; j < lPolygonSize; j++)
					{
						CPolygonPoint* polygonPoint = new CPolygonPoint();
						newPolygon->polygonPoints.push_back(polygonPoint);

						// indice
						int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
						polygonPoint->verticeIndex = meshVerticeOffset + lControlPointIndex;

						if (!foundVerticeColors)
						{
							foundVerticeColors = (pMesh->GetElementVertexColorCount() > 0);
						}

						FbxColor color;

						for (int l = 0; l < pMesh->GetElementVertexColorCount(); l++)
						{
							FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor( l);

							switch (leVtxc->GetMappingMode())
							{
							case FbxGeometryElement::eByControlPoint:
								switch (leVtxc->GetReferenceMode())
								{
								case FbxGeometryElement::eDirect:
									color = leVtxc->GetDirectArray().GetAt(lControlPointIndex);
									polygonPoint->verticeColorIndex = GetAddVerticeColorIndex(verticeColors, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
									break;
								case FbxGeometryElement::eIndexToDirect:
									{
										int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
										color =  leVtxc->GetDirectArray().GetAt(id);
										polygonPoint->verticeColorIndex = GetAddVerticeColorIndex(verticeColors, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
									}
									break;
								default:
									break; // other reference modes not shown here!
								}
								break;

							case FbxGeometryElement::eByPolygonVertex:
								{
									switch (leVtxc->GetReferenceMode())
									{
									case FbxGeometryElement::eDirect:
										color = leVtxc->GetDirectArray().GetAt(vertexId);
										polygonPoint->verticeColorIndex = GetAddVerticeColorIndex(verticeColors, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
										break;
									case FbxGeometryElement::eIndexToDirect:
										{
											int id = leVtxc->GetIndexArray().GetAt(vertexId);
											color = leVtxc->GetDirectArray().GetAt(id);
											polygonPoint->verticeColorIndex = GetAddVerticeColorIndex(verticeColors, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
										}
										break;
									default:
										break; // other reference modes not shown here!
									}
								}
								break;

							case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
							case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
							case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
								break;
							}
						}

						if (!foundTextureUV)
						{
							foundTextureUV = (pMesh->GetElementUVCount() > 0);
						}

						for (int l = 0; l < pMesh->GetElementUVCount(); ++l)
						{
							FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);

							float uvalue;
							float vvalue;

							switch (leUV->GetMappingMode())
							{
							case FbxGeometryElement::eByControlPoint:
								switch (leUV->GetReferenceMode())
								{
								case FbxGeometryElement::eDirect:
									uvalue = leUV->GetDirectArray().GetAt(lControlPointIndex)[0];
									vvalue = leUV->GetDirectArray().GetAt(lControlPointIndex)[0];
									polygonPoint->uvIndex = GetAddUVIndex(uvCoordinates, uvalue, vvalue);
									break;
								case FbxGeometryElement::eIndexToDirect:
									{
										int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
										uvalue = leUV->GetDirectArray().GetAt(id)[0];
										vvalue = leUV->GetDirectArray().GetAt(id)[1];
										polygonPoint->uvIndex = GetAddUVIndex(uvCoordinates, uvalue, vvalue);
									}
									break;
								default:
									break; // other reference modes not shown here!
								}
								break;

							case FbxGeometryElement::eByPolygonVertex:
								{
									int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
									switch (leUV->GetReferenceMode())
									{
									case FbxGeometryElement::eDirect:
									case FbxGeometryElement::eIndexToDirect:
										{
											uvalue = leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
											vvalue = leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];
											polygonPoint->uvIndex = GetAddUVIndex(uvCoordinates, uvalue, vvalue);
										}
										break;
									default:
										break; // other reference modes not shown here!
									}
								}
								break;

							case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
							case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
							case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
								break;
							}
						}

						if (!foundNormals)
						{
							foundNormals = (pMesh->GetElementNormalCount() > 0);
						}

						for(int l = 0; l < pMesh->GetElementNormalCount(); ++l)
						{
							FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
							float nx;
							float ny;
							float nz;
							FbxVector4 tempVector;
							FbxVector4 zeroVector;
							FbxVector4 deltaNormal;

							if(leNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
							{
								switch (leNormal->GetReferenceMode())
								{
								case FbxGeometryElement::eDirect:
									nx = leNormal->GetDirectArray().GetAt(lControlPointIndex)[0];
									ny = leNormal->GetDirectArray().GetAt(lControlPointIndex)[1];
									nz = leNormal->GetDirectArray().GetAt(lControlPointIndex)[2];

									tempVector = leNormal->GetDirectArray().GetAt(lControlPointIndex);
									tempVector = matrix.MultT(tempVector);

									zeroVector = FbxVector4(0, 0, 0);
									zeroVector = matrix.MultT(zeroVector);

									deltaNormal = tempVector - zeroVector;
									deltaNormal.Normalize();

									polygonPoint->normalIndex = GetAddNormalIndex(normals, deltaNormal[0], deltaNormal[1], deltaNormal[2]);
									break;
								case FbxGeometryElement::eIndexToDirect:
									{
										int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
										nx = leNormal->GetDirectArray().GetAt(id)[0];
										ny = leNormal->GetDirectArray().GetAt(id)[1];
										nz = leNormal->GetDirectArray().GetAt(id)[2];

										tempVector = leNormal->GetDirectArray().GetAt(id);
										tempVector = matrix.MultT(tempVector);

										zeroVector = FbxVector4(0, 0, 0);
										zeroVector = matrix.MultT(zeroVector);

										deltaNormal = tempVector - zeroVector;
										deltaNormal.Normalize();

										polygonPoint->normalIndex = GetAddNormalIndex(normals, deltaNormal[0], deltaNormal[1], deltaNormal[2]);
									}
									break;
								default:
									break; // other reference modes not shown here!
								}
							}
							else if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
							{
								switch (leNormal->GetReferenceMode())
								{
								case FbxGeometryElement::eDirect:
									nx = leNormal->GetDirectArray().GetAt(vertexId)[0];
									ny = leNormal->GetDirectArray().GetAt(vertexId)[1];
									nz = leNormal->GetDirectArray().GetAt(vertexId)[2];

									tempVector = leNormal->GetDirectArray().GetAt(vertexId);
									tempVector = matrix.MultT(tempVector);

									zeroVector = FbxVector4(0, 0, 0);
									zeroVector = matrix.MultT(zeroVector);

									deltaNormal = tempVector - zeroVector;
									deltaNormal.Normalize();

									polygonPoint->normalIndex = GetAddNormalIndex(normals, deltaNormal[0], deltaNormal[1], deltaNormal[2]);
									break;
								case FbxGeometryElement::eIndexToDirect:
									{
										int id = leNormal->GetIndexArray().GetAt(vertexId);
										nx = leNormal->GetDirectArray().GetAt(id)[0];
										ny = leNormal->GetDirectArray().GetAt(id)[1];
										nz = leNormal->GetDirectArray().GetAt(id)[2];

										tempVector = leNormal->GetDirectArray().GetAt(id);
										tempVector = matrix.MultT(tempVector);

										zeroVector = FbxVector4(0, 0, 0);
										zeroVector = matrix.MultT(zeroVector);

										deltaNormal = tempVector - zeroVector;
										deltaNormal.Normalize();

										polygonPoint->normalIndex = GetAddNormalIndex(normals, deltaNormal[0], deltaNormal[1], deltaNormal[2]);
									}
									break;
								default:
									break; // other reference modes not shown here!
								}
							}

						}
						vertexId++;
					}
				}
			}
		}
	}

	for(int i = 0; i < pNode->GetChildCount(); i++)
    {
        // recursively call this
		ParseFbxNodeRecursive(pNode->GetChild(i), currentGroup, inputFile, verticeColors, normals, uvCoordinates, vertices, groups, materialFiles, joints, animations,
			specialKeywordMode, mergeLikeMaterials, renameMaterials, foundTextureUV, foundNormals, foundVerticeColors, noGroups, errorString, jointType);
    }
}
#endif

void CObjToAn8Dlg::WriteOwnDaeSkeleton(CString& libraryVisualScenes, CJoint* joint, int indent)
{
	for (int x = 0; x < indent; x++)
		libraryVisualScenes.AppendFormat("  ");
	libraryVisualScenes.AppendFormat("<node id=\"%s\" name=\"%s\" sid=\"%s\" type=\"JOINT\">\n", joint->name, joint->name, joint->name);

	float3 positionRelative;
	if (joint->parent != NULL)
	{
		positionRelative = float3((joint->positionAbsolute.x - joint->parent->positionAbsolute.x), (joint->positionAbsolute.y - joint->parent->positionAbsolute.y), (joint->positionAbsolute.z - joint->parent->positionAbsolute.z));
	}
	else
	{
		positionRelative = float3(joint->positionAbsolute.x, joint->positionAbsolute.y, joint->positionAbsolute.z);
	}

	for (int x = 0; x < (indent + 1); x++)
		libraryVisualScenes.AppendFormat("  ");
	libraryVisualScenes.AppendFormat("<matrix sid=\"transform\">1 0 0 %f 0 1 0 %f 0 0 1 %f 0 0 0 1</matrix>\n", positionRelative.x, positionRelative.y, positionRelative.z);

	for (int x = 0; x < joint->children.size(); x++)
	{
		WriteOwnDaeSkeleton(libraryVisualScenes, joint->children[x], indent+2);
	}

	for (int x = 0; x < indent; x++)
		libraryVisualScenes.AppendFormat("  ");
	libraryVisualScenes.AppendFormat("</node>\n");
}

void CObjToAn8Dlg::WriteOwnDaeFile(CString outputFile, std::vector<CVerticeColor*> verticeColors, std::vector<CNormal*> normals, std::vector<CUVCoordinate*> uvCoordinates, std::vector<CVertice*> vertices, std::vector<CGroup*> groups, std::vector<CMaterialFile*> materialFiles,  std::vector<CJoint*>& joints, std::vector<CAnimation*>& animations,
	bool specialKeywordMode, bool mergeLikeMaterials, bool renameMaterials, bool& foundTextureUV, bool& foundNormals, bool& foundVerticeColors, bool ignoreShading, bool ignoreShadingPoint7, bool noGroups, bool primarySecondaryGroups, bool mergeHierarchicalGroups, bool regexFilterCheck, CString regexFilter, bool sortByTextureWithinGroup)
{
	// Remove .s
	std::vector<CMaterialFile*>::iterator	itermaterialFiles;
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			material->name.Replace(".", "_");
			material->name.Replace(" ", "_");
		}
	}

	std::vector<CGroup*>::iterator	itergroups;
	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));
		group->name.Replace(".", "_");
		group->name.Replace(" ", "_");

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			polygon->materialName.Replace(".", "_");
			polygon->materialName.Replace(" ", "_");
		}
	}

	CString lastGroup = "NOT A REAL GROUP";
	FILE* outFile = fopen(outputFile, "w");
	if (outFile != NULL)
	{

	}
	else
	{
		if (doMessageBoxes)
			MessageBox("Can't open output file " + outputFile);
		return;
	}

	fprintf(outFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(outFile, "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n");
	fprintf(outFile, "	<asset>\n");
	fprintf(outFile, "		<contributor>\n");
	fprintf(outFile, "		  <author>Obj2an8</author>\n");
	fprintf(outFile, "		  <authoring_tool>Obj2an8</authoring_tool>\n");
	fprintf(outFile, "		</contributor>\n");
	fprintf(outFile, "		<created>2017-08-20T09:25:21</created>\n");
	fprintf(outFile, "		<modified>2017-08-20T09:25:21</modified>\n");
	fprintf(outFile, "		<unit name=\"centimeter\" meter=\"0.01\"/>\n");
	fprintf(outFile, "		<up_axis>Y_UP</up_axis>\n");
	fprintf(outFile, "	</asset>\n");

	CString materialXmlStr;
	CString libraryEffectsXmlStr;
	CString libraryImagesXmlStr;

	materialXmlStr.AppendFormat("  <library_materials>\n");
	libraryEffectsXmlStr.AppendFormat("	<library_effects>\n");
	libraryImagesXmlStr.AppendFormat("  <library_images>\n");
	
	for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
	{
		CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

		std::vector<CMaterial*>::iterator	itermaterial;
		for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
		{
			CMaterial* material = (CMaterial*)*itermaterial;

			materialXmlStr.AppendFormat("    <material id=\"%s\" name=\"%s\">\n", material->name, material->name);
			materialXmlStr.AppendFormat("          <instance_effect url=\"#%s-fx\"/>\n", material->name);
			materialXmlStr.AppendFormat("        </material>\n");

			libraryEffectsXmlStr.AppendFormat("    <effect id=\"%s-fx\">\n", material->name);
			libraryEffectsXmlStr.AppendFormat("      <profile_COMMON>\n");
			if (material->diffuseTexture.contains)
			{
				libraryEffectsXmlStr.AppendFormat("        <newparam sid=\"%s-Img-surface\">\n", material->name);
				libraryEffectsXmlStr.AppendFormat("          <surface type=\"2D\">\n");
				libraryEffectsXmlStr.AppendFormat("            <init_from>%s-Img</init_from>\n", material->name);
				libraryEffectsXmlStr.AppendFormat("          </surface>\n");
				libraryEffectsXmlStr.AppendFormat("        </newparam>\n");
				libraryEffectsXmlStr.AppendFormat("        <newparam sid=\"%s-Img-sampler\">\n", material->name);
				libraryEffectsXmlStr.AppendFormat("          <sampler2D>\n");
				libraryEffectsXmlStr.AppendFormat("            <source>%s-Img-surface</source>\n", material->name);
				libraryEffectsXmlStr.AppendFormat("          </sampler2D>\n");
				libraryEffectsXmlStr.AppendFormat("        </newparam>\n");
			}
			libraryEffectsXmlStr.AppendFormat("        <technique sid=\"common\">\n");
			libraryEffectsXmlStr.AppendFormat("          <phong>\n");
			libraryEffectsXmlStr.AppendFormat("            <emission>\n");
			libraryEffectsXmlStr.AppendFormat("              <color>0 0 0 1</color>\n");
			libraryEffectsXmlStr.AppendFormat("            </emission>\n");
			if (material->ambientColor.contains)
			{
				libraryEffectsXmlStr.AppendFormat("            <ambient>\n");
				libraryEffectsXmlStr.AppendFormat("              <color>%f %f %f 1</color>\n", material->ambientColor.r, material->ambientColor.g, material->ambientColor.b);
				libraryEffectsXmlStr.AppendFormat("            </ambient>\n");
			}

			if ((material->diffuseColor.contains) || (material->diffuseTexture.contains))
			{
				libraryEffectsXmlStr.AppendFormat("            <diffuse>\n");
				if (material->diffuseTexture.contains)
				{
					libraryEffectsXmlStr.AppendFormat("              <texture texture=\"%s-Img-sampler\" texcoord=\"TEX0\">\n", material->name);
					libraryEffectsXmlStr.AppendFormat("              </texture>\n");
				}
				if (material->diffuseColor.contains)
				{
					libraryEffectsXmlStr.AppendFormat("              <color>%f %f %f 1</color>\n", material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b);
				}
				libraryEffectsXmlStr.AppendFormat("            </diffuse>\n");
			}

			if (material->spectralColor.contains)
			{
				libraryEffectsXmlStr.AppendFormat("            <specular>\n");
				libraryEffectsXmlStr.AppendFormat("              <color>%f %f %f 1</color>\n", material->spectralColor.r, material->spectralColor.g, material->spectralColor.b);
				libraryEffectsXmlStr.AppendFormat("            </specular>\n");
			}
			libraryEffectsXmlStr.AppendFormat("            <shininess>\n");
			libraryEffectsXmlStr.AppendFormat("              <float>20</float>\n");
			libraryEffectsXmlStr.AppendFormat("            </shininess>\n");
			libraryEffectsXmlStr.AppendFormat("            <reflective>\n");
			libraryEffectsXmlStr.AppendFormat("              <color>0 0 0 1</color>\n");
			libraryEffectsXmlStr.AppendFormat("            </reflective>\n");
			libraryEffectsXmlStr.AppendFormat("            <reflectivity>\n");
			libraryEffectsXmlStr.AppendFormat("              <float>1</float>\n");
			libraryEffectsXmlStr.AppendFormat("            </reflectivity>\n");
			libraryEffectsXmlStr.AppendFormat("            <transparent opaque=\"RGB_ZERO\">\n");
			libraryEffectsXmlStr.AppendFormat("              <color>0 0 0 1</color>\n");
			libraryEffectsXmlStr.AppendFormat("            </transparent>\n");
			if (material->alpha.contains)
			{
				libraryEffectsXmlStr.AppendFormat("            <transparency>\n");
				libraryEffectsXmlStr.AppendFormat("              <float>%f</float>\n",  material->alpha.floatValue);
				libraryEffectsXmlStr.AppendFormat("            </transparency>\n");
			}
			libraryEffectsXmlStr.AppendFormat("          </phong>\n");
			libraryEffectsXmlStr.AppendFormat("        </technique>\n");
			libraryEffectsXmlStr.AppendFormat("      </profile_COMMON>\n");
			libraryEffectsXmlStr.AppendFormat("    </effect>\n");

			if (material->diffuseTexture.contains)
			{
				libraryImagesXmlStr.AppendFormat("    <image id=\"%s-Img\" name=\"%s-Img\">\n", material->name, material->name);
				libraryImagesXmlStr.AppendFormat("      <init_from>%s</init_from>\n", material->diffuseTexture.stringValue);
				libraryImagesXmlStr.AppendFormat("    </image>\n");
			}
		}
	}

	materialXmlStr.AppendFormat("  </library_materials>\n");
	libraryEffectsXmlStr.AppendFormat("  </library_effects>\n");
	libraryImagesXmlStr.AppendFormat("  </library_images>\n");

	fprintf(outFile, materialXmlStr);
	fprintf(outFile, libraryEffectsXmlStr);
	fprintf(outFile, libraryImagesXmlStr);

	CString libraryControllersXmlStr;
	CString libraryGeometriesXmlStr;

	libraryControllersXmlStr.AppendFormat("  <library_controllers>\n");
	libraryGeometriesXmlStr.AppendFormat("  <library_geometries>\n");

	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;

		if (regexFilterCheck)
		{
			std::tr1::regex rx(regexFilter.GetBuffer());
			std::string stdString = std::string(group->name.GetBuffer());
			regexFilter.ReleaseBuffer();

			bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
			group->name.ReleaseBuffer();

			if (!isMatch)
			{
				continue;
			}
		}

		libraryGeometriesXmlStr.AppendFormat("    <geometry id=\"%sShape\" name=\"%sShape\">\n", group->name, group->name);
		libraryGeometriesXmlStr.AppendFormat("      <mesh>\n");

		CString defaultMaterial = "DefaultMaterial";

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			defaultMaterial = polygon->materialName;
			break;
		}

		/*FbxMesh* lMesh = FbxMesh::Create(pSdkManager, group->name);

		FbxSkin* lSkin = FbxSkin::Create(pScene, "");

		for (int x = 0; x < joints.size(); x++)
		{
			FbxCluster* cluster = FbxCluster::Create(pScene, "");
			cluster->SetLink(jointSkeleton[joints[x]->name]);
			cluster->SetLinkMode(FbxCluster::eNormalize);

			lSkin->AddCluster(cluster);

			jointCluster[joints[x]->name] = cluster;
		}

		lMesh->AddDeformer(lSkin);

		// create a FbxNode
		FbxNode* lNode = FbxNode::Create(pSdkManager, group->name);

		// set the node attribute
		lNode->SetNodeAttribute(lMesh);

		// set the shading mode to view texture
		lNode->SetShadingMode(FbxNode::eTextureShading);*/

		std::vector<CVertice*> subVerticeList;
		std::vector<CUVCoordinate*> subUVList;
		std::vector<CVerticeColor*> subVertexColorList;
		std::vector<CNormal*> subNormalList;
		std::vector<CString> subMaterialList;

		//lNode->AddMaterial(defaultMaterialDefinition);

		subMaterialList.push_back("DefaultMaterial");

		if (sortByTextureWithinGroup)
		{
			SortPolygonGroupByTexture(group);
		}

		int totalCountPolygonIndexes = 0;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			totalCountPolygonIndexes += polygon->polygonPoints.size();
		}

		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));


			if (!(std::find(subMaterialList.begin(), subMaterialList.end(), polygon->materialName)!=subMaterialList.end()))
			{
				CMaterial* materialFound = NULL;

				for (itermaterialFiles = materialFiles.begin(); itermaterialFiles != materialFiles.end(); itermaterialFiles++)
				{
					CMaterialFile* materialFile = ((CMaterialFile*)(*itermaterialFiles));

					std::vector<CMaterial*>::iterator	itermaterial;
					for (itermaterial = materialFile->materials.begin(); itermaterial != materialFile->materials.end(); itermaterial++)
					{
						CMaterial* material = (CMaterial*)*itermaterial;

						if (material->name == polygon->materialName)
						{
							materialFound = material;
						}
					}
				}
			}

			int totalUVs = uvCoordinates.size();

			std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;
			for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
			{
				CPolygonPoint* polygonPoint = (CPolygonPoint*)*iterpolygonpoint;

				CVertice* vertice = NULL;
				if (polygonPoint->verticeIndex != -1)
					vertice = vertices[polygonPoint->verticeIndex];
				CUVCoordinate* uv = NULL;
				if (polygonPoint->uvIndex != -1)
					uv = uvCoordinates[polygonPoint->uvIndex];
				CNormal* normal = NULL;
				if (polygonPoint->normalIndex != -1)
					normal = normals[polygonPoint->normalIndex];
				CVerticeColor* verticeColor = NULL;
				if (polygonPoint->verticeColorIndex != -1)
				{
					if (verticeColors[polygonPoint->verticeColorIndex]->contains)
						verticeColor = verticeColors[polygonPoint->verticeColorIndex];
				}

				if (vertice != NULL)
				{
					if (!(std::find(subVerticeList.begin(), subVerticeList.end(), vertice)!=subVerticeList.end()))
					{
						subVerticeList.push_back(vertice);
					}
				}

				if (uv != NULL)
				{
					if (!(std::find(subUVList.begin(), subUVList.end(), uv)!=subUVList.end()))
					{
						subUVList.push_back(uv);
					}
				}

				if (normal != NULL)
				{
					if (!(std::find(subNormalList.begin(), subNormalList.end(), normal)!=subNormalList.end()))
					{
						subNormalList.push_back(normal);
					}
				}

				if (verticeColor != NULL)
				{
					if (!(std::find(subVertexColorList.begin(), subVertexColorList.end(), verticeColor)!=subVertexColorList.end()))
					{
						subVertexColorList.push_back(verticeColor);
					}
				}
			}
		}

		//lMesh->InitControlPoints(subVerticeList.size());
		//FbxVector4* lControlPoints = lMesh->GetControlPoints();

		libraryGeometriesXmlStr.AppendFormat("        <source id=\"%sShape-positions\" name=\"%sShape-positions\">\n", group->name, group->name);
		libraryGeometriesXmlStr.AppendFormat("          <float_array id=\"%sShape-positions-array\" count=\"%d\">", group->name, subVerticeList.size() * 3);
		int subVerticeCounter = 0;
		std::vector<CVertice*>::iterator	iterPointList;
		for (iterPointList = subVerticeList.begin(); iterPointList != subVerticeList.end(); iterPointList++)
		{
			CVertice* vertice = (CVertice*)*iterPointList;

			int found = 0;

			for (int x = 0; x < joints.size(); x++)
			{
				//FbxCluster* cluster = jointCluster[joints[x]->name];

				for (int y = 0; y < joints[x]->controlPoints.size(); y++)
				{
					CVertice* controlVertice = vertices[joints[x]->controlPoints[y]];
					if (controlVertice == vertice)
					{
						libraryGeometriesXmlStr.AppendFormat("%f %f %f ", vertice->vertex.x - joints[x]->positionAbsolute.x, vertice->vertex.y - joints[x]->positionAbsolute.y, vertice->vertex.z - joints[x]->positionAbsolute.z);

						found++;
						break;
					}
				}

				if (found > 0)
					break;
			}

			if (found == 0)
				libraryGeometriesXmlStr.AppendFormat("%f %f %f ", vertice->vertex.x, vertice->vertex.y, vertice->vertex.z);

			subVerticeCounter++;
		}

		libraryGeometriesXmlStr.AppendFormat("		</float_array>\n");

		libraryGeometriesXmlStr.AppendFormat("          <technique_common>\n");
		libraryGeometriesXmlStr.AppendFormat("            <accessor source=\"#%sShape-positions-array\" count=\"%d\" stride=\"3\">\n", group->name, subVerticeList.size());
		libraryGeometriesXmlStr.AppendFormat("              <param name=\"X\" type=\"float\"/>\n");
		libraryGeometriesXmlStr.AppendFormat("              <param name=\"Y\" type=\"float\"/>\n");
		libraryGeometriesXmlStr.AppendFormat("              <param name=\"Z\" type=\"float\"/>\n");
		libraryGeometriesXmlStr.AppendFormat("            </accessor>\n");
		libraryGeometriesXmlStr.AppendFormat("          </technique_common>\n");
		libraryGeometriesXmlStr.AppendFormat("        </source>\n");





		if (joints.size() > 0)
		{
			int totalJoints = joints.size();
			bool foundTopJoint = false;
			for (int joint = 0; joint < joints.size(); joint++)
			{
				if (joints[joint]->name == "TopJoint")
				{
					foundTopJoint = true;
					break;
				}
			}
			if (!foundTopJoint)
				totalJoints++;
			libraryControllersXmlStr.AppendFormat("    <controller id=\"%sShape-skin\" name=\"%sShape-skin\">\n", group->name, group->name);
			libraryControllersXmlStr.AppendFormat("          <skin source=\"#%sShape\">\n", group->name);
			libraryControllersXmlStr.AppendFormat("            <bind_shape_matrix>1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1</bind_shape_matrix>\n");
			libraryControllersXmlStr.AppendFormat("            <source id=\"%sShape-skin-joints\" name=\"%sShape-skin-joints\">\n", group->name, group->name);
			libraryControllersXmlStr.AppendFormat("              <Name_array id=\"%sShape-skin-joints-array\" count=\"%d\">", group->name, totalJoints);
			for (int joint = 0; joint < joints.size(); joint++)
			{
				libraryControllersXmlStr.AppendFormat(joints[joint]->name + " ");
			}
			if (!foundTopJoint)
				libraryControllersXmlStr.AppendFormat("TopJoint");
			libraryControllersXmlStr.AppendFormat("</Name_array>\n");
			libraryControllersXmlStr.AppendFormat("              <technique_common>\n");
			libraryControllersXmlStr.AppendFormat("                <accessor source=\"#%sShape-skin-joints-array\" count=\"%d\" stride=\"1\">\n", group->name, totalJoints);
			libraryControllersXmlStr.AppendFormat("                  <param name=\"JOINT\" type=\"name\"/>\n");
			libraryControllersXmlStr.AppendFormat("                </accessor>\n");
			libraryControllersXmlStr.AppendFormat("              </technique_common>\n");
			libraryControllersXmlStr.AppendFormat("            </source>\n");
			libraryControllersXmlStr.AppendFormat("            <source id=\"%sShape-skin-bind_poses\" name=\"%sShape-skin-bind_poses\">\n", group->name, group->name);
			libraryControllersXmlStr.AppendFormat("              <float_array id=\"%sShape-skin-bind_poses-array\" count=\"%d\">", group->name, totalJoints * 16);
			for (int joint = 0; joint < totalJoints; joint++)
			{
				// May need fix later
				libraryControllersXmlStr.AppendFormat("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ");
			}
			libraryControllersXmlStr.AppendFormat("</float_array>\n");
			libraryControllersXmlStr.AppendFormat("              <technique_common>\n");
			libraryControllersXmlStr.AppendFormat("                <accessor source=\"#%sShape-skin-bind_poses-array\" count=\"%d\" stride=\"16\">\n", group->name, totalJoints);
			libraryControllersXmlStr.AppendFormat("                  <param name=\"TRANSFORM\" type=\"float4x4\"/>\n");
			libraryControllersXmlStr.AppendFormat("                </accessor>\n");
			libraryControllersXmlStr.AppendFormat("              </technique_common>\n");
			libraryControllersXmlStr.AppendFormat("            </source>\n");
			libraryControllersXmlStr.AppendFormat("            <source id=\"%sShape-skin-weights\" name=\"%sShape-skin-weights\">\n", group->name, group->name);
			libraryControllersXmlStr.AppendFormat("              <float_array id=\"%sShape-skin-weights-array\" count=\"%d\">1", group->name, (subVerticeList.size() * joints.size()) + 1);

			for (int verticeIndex = 0; verticeIndex < subVerticeList.size(); verticeIndex++)
			{
				int countMatchJoint = 0;
				for (int joint = 0; joint < joints.size(); joint++)
				{
					for (int y = 0; y < joints[joint]->controlPoints.size(); y++)
					{
						CVertice* controlVertice = vertices[joints[joint]->controlPoints[y]];
						if (controlVertice == subVerticeList[verticeIndex])
						{
							countMatchJoint++;
							break;
						}
					}
				}

				for (int joint = 0; joint < joints.size(); joint++)
				{
					bool matchedJoint = false;
					for (int y = 0; y < joints[joint]->controlPoints.size(); y++)
					{
						CVertice* controlVertice = vertices[joints[joint]->controlPoints[y]];
						if (controlVertice == subVerticeList[verticeIndex])
						{
							matchedJoint = true;
							break;
						}
					}

					if (matchedJoint)
					{
						if (countMatchJoint == 1)
							libraryControllersXmlStr.AppendFormat(" 1");
						else
							libraryControllersXmlStr.AppendFormat(" %f", 1.0 / (float)countMatchJoint);
					}
					else
					{
						libraryControllersXmlStr.AppendFormat(" 0");
					}
				}
			}
			libraryControllersXmlStr.AppendFormat("</float_array>\n");
			libraryControllersXmlStr.AppendFormat("              <technique_common>\n");
			libraryControllersXmlStr.AppendFormat("                <accessor source=\"#%sShape-skin-weights-array\" count=\"%d\" stride=\"1\">\n", group->name, ((subVerticeList.size() * joints.size()) + 1));
			libraryControllersXmlStr.AppendFormat("                  <param name=\"WEIGHT\" type=\"float\"/>\n");
			libraryControllersXmlStr.AppendFormat("                </accessor>\n");
			libraryControllersXmlStr.AppendFormat("              </technique_common>\n");
			libraryControllersXmlStr.AppendFormat("            </source>\n");
			libraryControllersXmlStr.AppendFormat("            <joints>\n");
			libraryControllersXmlStr.AppendFormat("              <input semantic=\"JOINT\" source=\"#%sShape-skin-joints\"/>\n", group->name);
			libraryControllersXmlStr.AppendFormat("              <input semantic=\"INV_BIND_MATRIX\" source=\"#%sShape-skin-bind_poses\"/>\n", group->name);
			libraryControllersXmlStr.AppendFormat("            </joints>\n");
			libraryControllersXmlStr.AppendFormat("            <vertex_weights count=\"%d\">\n", subVerticeList.size());
			libraryControllersXmlStr.AppendFormat("              <input semantic=\"JOINT\" source=\"#%sShape-skin-joints\" offset=\"0\"/>\n", group->name);
			libraryControllersXmlStr.AppendFormat("              <input semantic=\"WEIGHT\" source=\"#%sShape-skin-weights\" offset=\"1\"/>\n", group->name);
			libraryControllersXmlStr.AppendFormat("              <vcount>");
			for (int x = 0; x < subVerticeList.size(); x++)
			{
				libraryControllersXmlStr.AppendFormat("%d ", joints.size());
			}
			libraryControllersXmlStr.AppendFormat("</vcount>\n");
			libraryControllersXmlStr.AppendFormat("              <v>");

			int countJointsWritten = 1;
			for (int x = 0; x < subVerticeList.size(); x++)
			{
				for (int joint = 0; joint < joints.size(); joint++)
				{
					libraryControllersXmlStr.AppendFormat("%d %d ", joint, countJointsWritten++);
				}
			}
			libraryControllersXmlStr.AppendFormat("</v>\n");
			libraryControllersXmlStr.AppendFormat("            </vertex_weights>\n");
			libraryControllersXmlStr.AppendFormat("          </skin>\n");
			libraryControllersXmlStr.AppendFormat("        </controller>\n");
		}

		if (foundNormals && (subNormalList.size() > 0))
		{
			libraryGeometriesXmlStr.AppendFormat("        <source id=\"%sShape-normals\" name=\"%sShape-normals\">\n", group->name, group->name);
			libraryGeometriesXmlStr.AppendFormat("          <float_array id=\"%sShape-normals-array\" count=\"%d\">", group->name, subNormalList.size() * 3);
			std::vector<CNormal*>::iterator	iterNormal;
			for (iterNormal = subNormalList.begin(); iterNormal != subNormalList.end(); iterNormal++)
			{
				CNormal* normal = (CNormal*)*iterNormal;

				libraryGeometriesXmlStr.AppendFormat("%f %f %f ", normal->nx, normal->ny, normal->nz);
			}

			libraryGeometriesXmlStr.AppendFormat("</float_array>\n");

			libraryGeometriesXmlStr.AppendFormat("          <technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("            <accessor source=\"#%sShape-normals-array\" count=\"%d\" stride=\"3\">\n", group->name, subNormalList.size());
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"X\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"Y\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"Z\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("            </accessor>\n");
			libraryGeometriesXmlStr.AppendFormat("          </technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("        </source>\n");
		}

		if (foundTextureUV && (subUVList.size() > 0))
		{
			libraryGeometriesXmlStr.AppendFormat("        <source id=\"%sShape-default\" name=\"%sShape-default\">\n", group->name, group->name);
			libraryGeometriesXmlStr.AppendFormat("          <float_array id=\"%sShape-default-array\" count=\"%d\">", group->name, subUVList.size() * 2);

			std::vector<CUVCoordinate*>::iterator	iterUVList;
			for (iterUVList = subUVList.begin(); iterUVList != subUVList.end(); iterUVList++)
			{
				CUVCoordinate* uv = (CUVCoordinate*)*iterUVList;

				libraryGeometriesXmlStr.AppendFormat("%f %f ", uv->u, uv->v);
			}

			libraryGeometriesXmlStr.AppendFormat("</float_array>\n");

			libraryGeometriesXmlStr.AppendFormat("          <technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("            <accessor source=\"#%sShape-default-array\" count=\"%d\" stride=\"2\">\n", group->name, subUVList.size());
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"S\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"T\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("            </accessor>\n");
			libraryGeometriesXmlStr.AppendFormat("          </technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("        </source>\n");
		}

		if (subVertexColorList.size() > 0)
		{
			libraryGeometriesXmlStr.AppendFormat("        <source id=\"%sShape-colorSet0\" name=\"%sShape-colorSet0\">\n", group->name, group->name);
			libraryGeometriesXmlStr.AppendFormat("          <float_array id=\"%sShape-colorSet0-array\" count=\"%d\">", group->name, subNormalList.size() * 4);

			std::vector<CVerticeColor*>::iterator	iterVertexColorList;
			for (iterVertexColorList = subVertexColorList.begin(); iterVertexColorList != subVertexColorList.end(); iterVertexColorList++)
			{
				CVerticeColor* verticeColor = (CVerticeColor*)*iterVertexColorList;

				libraryGeometriesXmlStr.AppendFormat("%f %f %f %f ", verticeColor->color.r / 255.0f, verticeColor->color.g / 255.0f, verticeColor->color.b / 255.0f, verticeColor->color.a / 255.0f);
			}

			libraryGeometriesXmlStr.AppendFormat("</float_array>\n");

			libraryGeometriesXmlStr.AppendFormat("          <technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("            <accessor source=\"#%sShape-colorSet0-array\" count=\"%d\" stride=\"4\">\n", group->name, subVertexColorList.size());
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"R\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"G\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"B\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("              <param name=\"A\" type=\"float\"/>\n");
			libraryGeometriesXmlStr.AppendFormat("            </accessor>\n");
			libraryGeometriesXmlStr.AppendFormat("          </technique_common>\n");
			libraryGeometriesXmlStr.AppendFormat("        </source>\n");
		}

		libraryGeometriesXmlStr.AppendFormat("        <vertices id=\"%sShape-vertices\" name=\"%sShape-vertices\">\n", group->name, group->name);
		libraryGeometriesXmlStr.AppendFormat("           <input semantic=\"POSITION\" source=\"#%sShape-positions\"/>\n", group->name);
		libraryGeometriesXmlStr.AppendFormat("        </vertices>\n");

		// Group by texture now, then write out by that
		std::vector<PolygonTextureGroup> polygonTextureGroups;

		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));
			
			if (
				(polygonTextureGroups.size() > 0)
				&&
				(polygonTextureGroups[polygonTextureGroups.size() - 1].materialName == polygon->materialName)
				)
			{
				polygonTextureGroups[polygonTextureGroups.size() - 1].polygons.push_back(polygon);
			}
			else
			{
				PolygonTextureGroup polygonTextureGroup;
				polygonTextureGroup.materialName = polygon->materialName;
				polygonTextureGroup.polygons.push_back(polygon);

				polygonTextureGroups.push_back(polygonTextureGroup);
			}
		}

		bool isTriangulated = true;

		for (int polygonTextureGroupIndex = 0; polygonTextureGroupIndex < polygonTextureGroups.size(); polygonTextureGroupIndex++)
		{
			for (int subPolyCount = 0; subPolyCount < polygonTextureGroups[polygonTextureGroupIndex].polygons.size(); subPolyCount++)
			{
				CPolygon* polygon = polygonTextureGroups[polygonTextureGroupIndex].polygons[subPolyCount];

				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;

				int numPolyPointsExpected = polygon->polygonPoints.size();

				int resultCounter = 0;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					resultCounter++;
				}

				if (resultCounter != 3)
				{
					isTriangulated = false;
					break;
				}
			}
		}

		for (int polygonTextureGroupIndex = 0; polygonTextureGroupIndex < polygonTextureGroups.size(); polygonTextureGroupIndex++)
		{
			if (isTriangulated)
			{
				libraryGeometriesXmlStr.AppendFormat("        <triangles material=\"%s\" count=\"%d\">\n", polygonTextureGroups[polygonTextureGroupIndex].materialName + "-Tex", polygonTextureGroups[polygonTextureGroupIndex].polygons.size());
			}
			else
			{
				libraryGeometriesXmlStr.AppendFormat("        <polylist material=\"%s\" count=\"%d\">\n", polygonTextureGroups[polygonTextureGroupIndex].materialName + "-Tex", polygonTextureGroups[polygonTextureGroupIndex].polygons.size());
			}
			int offsetCounter = 0;
			libraryGeometriesXmlStr.AppendFormat("          <input semantic=\"VERTEX\" source=\"#%sShape-vertices\" offset=\"%d\"/>\n", group->name, offsetCounter++);
			if (foundNormals && (subNormalList.size() > 0))
			{
				libraryGeometriesXmlStr.AppendFormat("          <input semantic=\"NORMAL\" source=\"#%sShape-normals\" offset=\"%d\"/>\n", group->name, offsetCounter++);
			}
			if (foundTextureUV && (subUVList.size() > 0))
			{
				libraryGeometriesXmlStr.AppendFormat("          <input semantic=\"TEXCOORD\" source=\"#%sShape-default\" offset=\"%d\" set=\"0\"/>\n", group->name, offsetCounter++);
			}
			if (subVertexColorList.size() > 0)
			{
				libraryGeometriesXmlStr.AppendFormat("          <input semantic=\"COLOR\" source=\"#%sShape-colorSet0\" offset=\"%d\" set=\"0\"/>\n", group->name, offsetCounter++);
			}

			if (!isTriangulated)
			{
				libraryGeometriesXmlStr.AppendFormat("          <vcount>");

				for (int subPolyCount = 0; subPolyCount < polygonTextureGroups[polygonTextureGroupIndex].polygons.size(); subPolyCount++)
				{
					CPolygon* polygon = polygonTextureGroups[polygonTextureGroupIndex].polygons[subPolyCount];

					std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;

					int numPolyPointsExpected = polygon->polygonPoints.size();

					int resultCounter = 0;
					for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
					{
						resultCounter++;
					}

					libraryGeometriesXmlStr.AppendFormat("%d ", resultCounter);
				}

				libraryGeometriesXmlStr.AppendFormat("</vcount>\n");
			}
			
			libraryGeometriesXmlStr.AppendFormat("          <p>");
			
			int pointNum = 0;

			for (int subPolyCount = 0; subPolyCount < polygonTextureGroups[polygonTextureGroupIndex].polygons.size(); subPolyCount++)
			{
				CPolygon* polygon = polygonTextureGroups[polygonTextureGroupIndex].polygons[subPolyCount];

				std::vector<CPolygonPoint*>::iterator	iterpolygonpoint;

				int numPolyPointsExpected = polygon->polygonPoints.size();

				CPolygonPoint** resultingPoints = new CPolygonPoint*[polygon->polygonPoints.size()];

				int resultCounter = 0;
				for (iterpolygonpoint = polygon->polygonPoints.begin(); iterpolygonpoint != polygon->polygonPoints.end(); iterpolygonpoint++)
				{
					resultingPoints[resultCounter] = (CPolygonPoint*)*iterpolygonpoint;
					resultCounter++;
				}

				for (int x = 0; x < resultCounter; x++)
				{
					CPolygonPoint* polygonPoint = resultingPoints[x];
					CVertice* vertice = ((CVertice*)vertices[polygonPoint->verticeIndex]);

					int foundPoint = 0;
					std::vector<CVertice*>::iterator	iterVertice;
					for (iterVertice = subVerticeList.begin(); iterVertice != subVerticeList.end(); iterVertice++)
					{
						CVertice* verticeTest = (CVertice*)(*iterVertice);
						/*if (
							(vertice->vertex.x == verticeTest->vertex.x)
							&&
							(vertice->vertex.y == verticeTest->vertex.y)
							&& 
							(vertice->vertex.z == verticeTest->vertex.z)
							)*/
						if (verticeTest == vertice)
						{
							break;
						}
						foundPoint++;
					}

					if (foundPoint >= subVerticeList.size())
						foundPoint = 0; // error

					libraryGeometriesXmlStr.AppendFormat("%d ", foundPoint);

					if (foundNormals && (subNormalList.size() > 0))
					{
						if (polygonPoint->normalIndex != -1)
						{
							CNormal* normal = ((CNormal*)normals[polygonPoint->normalIndex]);

							int foundNormal = 0;
							std::vector<CNormal*>::iterator	iterNormal;
							for (iterNormal = subNormalList.begin(); iterNormal != subNormalList.end(); iterNormal++)
							{
								CNormal* normalTest = (CNormal*)(*iterNormal);
								/*if (
									(normal->nx == normalTest->nx)
									&&
									(normal->ny == normalTest->ny)
									&&
									(normal->nz == normalTest->nz)
									)*/
								if (normal == normalTest)
								{
									break;
								}
								foundNormal++;
							}

							if (foundNormal >= subNormalList.size())
								foundNormal = 0; // error

							libraryGeometriesXmlStr.AppendFormat("%d ", foundNormal);
						}
						else
						{
							libraryGeometriesXmlStr.AppendFormat("0 ");
						}
					}

					if (foundTextureUV && (subUVList.size() > 0))
					{
						CString formatIndexStr;

						if (polygonPoint->uvIndex != -1)
						{
							CUVCoordinate* uv = ((CUVCoordinate*)uvCoordinates[polygonPoint->uvIndex]);

							int foundUV = 0;
							std::vector<CUVCoordinate*>::iterator	iterUV;
							for (iterUV = subUVList.begin(); iterUV != subUVList.end(); iterUV++)
							{
								CUVCoordinate* uvTest = (CUVCoordinate*)(*iterUV);
								/*if (
									(uv->u == uvTest->u)
									&&
									(uv->v == uvTest->v))*/
								if (uv == uvTest)
								{
									break;
								}
								foundUV++;
							}

							if (foundUV >= subUVList.size())
								foundUV = 0; // error

							libraryGeometriesXmlStr.AppendFormat("%d ", foundUV);
						}
						else
						{
							libraryGeometriesXmlStr.AppendFormat("0 ");
						}
					}

					if (subVertexColorList.size() > 0)
					{
						if (polygonPoint->verticeColorIndex != -1)
						{
							CVerticeColor* verticeColor = ((CVerticeColor*)verticeColors[polygonPoint->verticeColorIndex]);
							if (verticeColor->contains)
							{
								int foundVertColor = 0;
								std::vector<CVerticeColor*>::iterator	iterVertColor;
								for (iterVertColor = subVertexColorList.begin(); iterVertColor != subVertexColorList.end(); iterVertColor++)
								{
									CVerticeColor* vertColorTest = (CVerticeColor*)(*iterVertColor);
									/*if (
										(verticeColor->color.r == vertColorTest->color.r)
										&&
										(verticeColor->color.g == vertColorTest->color.g)
										&&
										(verticeColor->color.b == vertColorTest->color.b)
										)*/
									if (verticeColor == vertColorTest)
									{
										break;
									}
									foundVertColor++;
								}

								if (foundVertColor < subVertexColorList.size())
									libraryGeometriesXmlStr.AppendFormat("%d ", foundVertColor);
								else
									libraryGeometriesXmlStr.AppendFormat("0 ");
							}
							else
							{
								// Not sure about this
								libraryGeometriesXmlStr.AppendFormat("0 ");
							}
						}
						else
						{
							// Not sure about this
							libraryGeometriesXmlStr.AppendFormat("0 ");
						}
					}

				}

				delete [] resultingPoints;
			}
			libraryGeometriesXmlStr.AppendFormat("</p>\n");
			if (isTriangulated)
			{
				libraryGeometriesXmlStr.AppendFormat("        </triangles>\n");
			}
			else
			{
				libraryGeometriesXmlStr.AppendFormat("        </polylist>\n");
			}
		}

		libraryGeometriesXmlStr.AppendFormat("       </mesh>\n");
		libraryGeometriesXmlStr.AppendFormat("    </geometry>\n");
	}

	libraryControllersXmlStr.AppendFormat("  </library_controllers>\n");
	libraryGeometriesXmlStr.AppendFormat("  </library_geometries>\n");

	fprintf(outFile, libraryControllersXmlStr);
	fprintf(outFile, libraryGeometriesXmlStr);

	CString libraryVisualScenes;
	libraryVisualScenes.AppendFormat("  <library_visual_scenes>\n");
	libraryVisualScenes.AppendFormat("	<visual_scene id=\"VisualSceneNode\" name=\"untitled\">\n");

	if (joints.size() > 0)
	{	
		int totalJoints = joints.size();
		bool foundTopJoint = false;
		for (int joint = 0; joint < joints.size(); joint++)
		{
			if (joints[joint]->name == "TopJoint")
			{
				foundTopJoint = true;
				break;
			}
		}
		if (!foundTopJoint)
			totalJoints++;

		// TODO may need adjust top joint
		libraryVisualScenes.AppendFormat("   <node id=\"TopJoint\" name=\"TopJoint\" sid=\"TopJoint\" type=\"JOINT\">\n");
        libraryVisualScenes.AppendFormat( "    <matrix sid=\"transform\">1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1</matrix>\n");

		// Recursive write out top joint then joints, relative to parent
		//1 0 0 X
		//0 1 0 Y
		//0 0 1 Z
		//0 0 0 1

		std::vector<CJoint*> rootJoints = FindRootJoints(joints);

		if (rootJoints.size() == 0)
		{
			// Will cause corruption, need figure out
		}
		else
		{
			for (int x = 0; x < rootJoints.size(); x++)
			{
				WriteOwnDaeSkeleton(libraryVisualScenes, rootJoints[x], 2);
			}
		}

		libraryVisualScenes.AppendFormat("  </node>\n");
	}

	for (itergroups = groups.begin(); itergroups != groups.end(); itergroups++)
	{
		CGroup* group = ((CGroup*)(*itergroups));

		if (group->polygons.size() == 0)
			continue;

		if (regexFilterCheck)
		{
			std::tr1::regex rx(regexFilter.GetBuffer());
			std::string stdString = std::string(group->name.GetBuffer());
			regexFilter.ReleaseBuffer();

			bool isMatch = regex_search(stdString.begin(), stdString.end(), rx);
			group->name.ReleaseBuffer();

			if (!isMatch)
			{
				continue;
			}
		}

		libraryVisualScenes.AppendFormat("      <node id=\"%s\" name=\"%s\" type=\"NODE\">\n", group->name, group->name);
		libraryVisualScenes.AppendFormat("        <matrix sid=\"transform\">1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1</matrix>\n");
		if (joints.size() > 0)
		{
			libraryVisualScenes.AppendFormat("        <instance_controller url=\"#%sShape-skin\">\n", group->name);
			libraryVisualScenes.AppendFormat("          <skeleton>#TopJoint</skeleton>\n");
		}
		else
		{
			libraryVisualScenes.AppendFormat("        <instance_geometry url=\"#%sShape\">\n", group->name);
		}

		libraryVisualScenes.AppendFormat("          <bind_material>\n");
		libraryVisualScenes.AppendFormat("            <technique_common>\n");

		std::vector<CString> materialsUsed;

		std::vector<CPolygon*>::iterator	iterpolygon;
		for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
		{
			CPolygon* polygon = ((CPolygon*)(*iterpolygon));

			if (std::find(materialsUsed.begin(), materialsUsed.end(), polygon->materialName) == materialsUsed.end())
				materialsUsed.push_back(polygon->materialName);
		}

		for (int t = 0; t < materialsUsed.size(); t++)
		{
			libraryVisualScenes.AppendFormat("              <instance_material symbol=\"%s-Tex\" target=\"#%s\">\n", materialsUsed[t], materialsUsed[t]);
			libraryVisualScenes.AppendFormat("                <bind_vertex_input semantic=\"TEX0\" input_semantic=\"TEXCOORD\" input_set=\"0\"/>\n");
			libraryVisualScenes.AppendFormat("				      </instance_material>\n");
		}

		libraryVisualScenes.AppendFormat("            </technique_common>\n");
		libraryVisualScenes.AppendFormat("          </bind_material>\n");
		if (joints.size() > 0)
		{
			libraryVisualScenes.AppendFormat("        </instance_controller>\n");
		}
		else
		{
			libraryVisualScenes.AppendFormat("        </instance_geometry>\n");
		}
		libraryVisualScenes.AppendFormat("      </node>\n");
	}

	libraryVisualScenes.AppendFormat("  </visual_scene>\n");
	libraryVisualScenes.AppendFormat("  </library_visual_scenes>\n");
	fprintf(outFile, libraryVisualScenes);

	fprintf(outFile, "    <scene>\n");
	fprintf(outFile, "      <instance_visual_scene url=\"#VisualSceneNode\"/>\n");
	fprintf(outFile, "    </scene>\n");

	if (animations.size() > 0)
	{
		fprintf(outFile, "  <library_animations>\n");
		
		for (int x = 0; x < animations.size(); x++)
		{
			if (!animations[x]->isCamera)
			{
				// Top Joint First

				float fps = 30.0;

		fprintf(outFile, "    <animation id=\"TopJoint\">\n");
		fprintf(outFile, "      <source id=\"TopJoint_transform-input\" name=\"TopJoint_transform-input\">\n");
		fprintf(outFile, "        <float_array id=\"TopJoint_transform-input-array\" count=\"%d\">", animations[x]->keyframes.size());
		for (int z = 0; z < animations[x]->keyframes.size(); z++)
			fprintf(outFile, "%f ", (float)z / (float)fps);
		fprintf(outFile, "</float_array>\n");
		fprintf(outFile, "        <technique_common>\n");
		fprintf(outFile, "          <accessor source=\"#TopJoint_transform-input-array\" count=\"%d\" stride=\"1\">\n", animations[x]->keyframes.size());
		fprintf(outFile, "            <param name=\"TIME\" type=\"float\"/>\n");
		fprintf(outFile, "          </accessor>\n");
		fprintf(outFile, "        </technique_common>\n");
		fprintf(outFile, "      </source>\n");
		fprintf(outFile, "      <source id=\"TopJoint_transform-output\" name=\"TopJoint_transform-output\">\n");
		fprintf(outFile, "        <float_array id=\"TopJoint_transform-output-array\" count=\"%d\">", animations[x]->keyframes.size() * 16);
		
		for (int z = 0; z < animations[x]->keyframes.size(); z++)
		{
			float4x4 matrixKeyframe = float4x4::identity;
			CString rotationOrder = "XYZ";
			for (int joint = 0; joint < joints.size(); joint++)
			{
				if (joints[joint]->name == "TopJoint")
				{
					rotationOrder = joints[joint]->rotationOrder;
					break;
				}
			}

			matrixKeyframe = matrixKeyframe * matrixKeyframe.Translate(float3(animations[x]->keyframes[z]->translation.x, animations[x]->keyframes[z]->translation.y, animations[x]->keyframes[z]->translation.z));
			if (rotationOrder == "XZY")
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
			}
			else if (rotationOrder == "YZX")
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
			}
			else if (rotationOrder == "YXZ")
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
			}
			else if (rotationOrder == "ZXY")
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
			}
			else if (rotationOrder == "ZYX")
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
			}
			else // XYZ
			{
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->rotation.z));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->rotation.y));
				matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->rotation.x));
			}
			matrixKeyframe = matrixKeyframe * (float4x4)matrixKeyframe.Scale(animations[x]->keyframes[z]->scale.x, animations[x]->keyframes[z]->scale.y, animations[x]->keyframes[z]->scale.z);

			fprintf(outFile, "%f %f %f %f  %f %f %f %f  %f %f %f %f  %f %f %f %f ", 
							matrixKeyframe.v[0][0], matrixKeyframe.v[0][1], matrixKeyframe.v[0][2], matrixKeyframe.v[0][3],
							matrixKeyframe.v[1][0], matrixKeyframe.v[1][1], matrixKeyframe.v[1][2], matrixKeyframe.v[1][3],
							matrixKeyframe.v[2][0], matrixKeyframe.v[2][1], matrixKeyframe.v[2][2], matrixKeyframe.v[2][3],
							matrixKeyframe.v[3][0], matrixKeyframe.v[3][1], matrixKeyframe.v[3][2], matrixKeyframe.v[3][3]
							);
		}
		fprintf(outFile, "</float_array>\n");
		fprintf(outFile, "        <technique_common>\n");
		fprintf(outFile, "          <accessor source=\"#TopJoint_transform-output-array\" count=\"%d\" stride=\"16\">\n", animations[x]->keyframes.size());
		fprintf(outFile, "            <param name=\"TRANSFORM\" type=\"float4x4\"/>\n");
		fprintf(outFile, "          </accessor>\n");
		fprintf(outFile, "        </technique_common>\n");
		fprintf(outFile, "      </source>\n");
		fprintf(outFile, "      <source id=\"TopJoint_transform-interpolation\" name=\"TopJoint_transform-interpolation\">\n");
		fprintf(outFile, "        <Name_array id=\"TopJoint_transform-interpolation-array\" count=\"%d\">", animations[x]->keyframes.size());
		for (int z = 0; z < animations[x]->keyframes.size(); z++)
		{
			fprintf(outFile, "LINEAR ");
		}
		fprintf(outFile, "</Name_array>\n");
		fprintf(outFile, "        <technique_common>\n");
		fprintf(outFile, "          <accessor source=\"#TopJoint_transform-interpolation-array\" count=\"%d\" stride=\"1\">\n", animations[x]->keyframes.size());
		fprintf(outFile, "            <param name=\"INTERPOLATION\" type=\"name\"/>\n");
		fprintf(outFile, "          </accessor>\n");
		fprintf(outFile, "        </technique_common>\n");
		fprintf(outFile, "      </source>\n");
		fprintf(outFile, "      <sampler id=\"TopJoint_transform-sampler\">\n");
		fprintf(outFile, "        <input semantic=\"INPUT\" source=\"#TopJoint_transform-input\"/>\n");
		fprintf(outFile, "        <input semantic=\"OUTPUT\" source=\"#TopJoint_transform-output\"/>\n");
		fprintf(outFile, "        <input semantic=\"INTERPOLATION\" source=\"#TopJoint_transform-interpolation\"/>\n");
		fprintf(outFile, "      </sampler>\n");
		fprintf(outFile, "      <channel source=\"#TopJoint_transform-sampler\" target=\"TopJoint/transform\"/>\n");
		fprintf(outFile, "    </animation>\n");

				for (int joint = 0; joint < joints.size(); joint++)
				{
					fprintf(outFile, "    <animation id=\"%s\">\n", joints[joint]->name);
					fprintf(outFile, "      <source id=\"%s_transform-input\" name=\"%s_transform-input\">\n", joints[joint]->name, joints[joint]->name);
					fprintf(outFile, "        <float_array id=\"%s_transform-input-array\" count=\"%d\">", joints[joint]->name, animations[x]->keyframes.size());
					for (int z = 0; z < animations[x]->keyframes.size(); z++)
						fprintf(outFile, "%f ", (float)z / (float)fps);
					fprintf(outFile, "</float_array>\n");
					fprintf(outFile, "        <technique_common>\n");
					fprintf(outFile, "          <accessor source=\"#%s_transform-input-array\" count=\"%d\" stride=\"1\">\n", joints[joint]->name, animations[x]->keyframes.size());
					fprintf(outFile, "            <param name=\"TIME\" type=\"float\"/>\n");
					fprintf(outFile, "          </accessor>\n");
					fprintf(outFile, "        </technique_common>\n");
					fprintf(outFile, "      </source>\n");
					fprintf(outFile, "      <source id=\"%s_transform-output\" name=\"%s_transform-output\">\n", joints[joint]->name, joints[joint]->name);
					fprintf(outFile, "        <float_array id=\"%s_transform-output-array\" count=\"%d\">", joints[joint]->name, animations[x]->keyframes.size() * 16);
					// TODO CALCULATE ANIMATION
					for (int z = 0; z < animations[x]->keyframes.size(); z++)
					{
						float4x4 matrixKeyframe = float4x4::identity;

						for (int w = 0; w < animations[x]->keyframes[z]->animationParts.size(); w++)
						{
							if (joints[joint]->name == animations[x]->keyframes[z]->animationParts[w]->name)
							{
								if (joints[joint]->parent == NULL)
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.Translate(float3(animations[x]->keyframes[z]->animationParts[w]->translation.x + joints[joint]->positionAbsolute.x, animations[x]->keyframes[z]->animationParts[w]->translation.y + joints[joint]->positionAbsolute.y, animations[x]->keyframes[z]->animationParts[w]->translation.z + joints[joint]->positionAbsolute.z));
								}
								else
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.Translate(float3(animations[x]->keyframes[z]->animationParts[w]->translation.x + (joints[joint]->positionAbsolute.x - joints[joint]->parent->positionAbsolute.x), animations[x]->keyframes[z]->animationParts[w]->translation.y + (joints[joint]->positionAbsolute.y - joints[joint]->parent->positionAbsolute.y), animations[x]->keyframes[z]->animationParts[w]->translation.z + (joints[joint]->positionAbsolute.z - joints[joint]->parent->positionAbsolute.z)));
								}

								if (joints[joint]->rotationOrder == "XZY")
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
								}
								else if (joints[joint]->rotationOrder == "YZX")
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
								}
								else if (joints[joint]->rotationOrder == "YXZ")
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
								}
								else if (joints[joint]->rotationOrder == "ZXY")
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
								}
								else if (joints[joint]->rotationOrder == "ZYX")
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
								}
								else // XYZ
								{
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateZ(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.z));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateY(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.y));
									matrixKeyframe = matrixKeyframe * matrixKeyframe.RotateX(AN8XToRadian(animations[x]->keyframes[z]->animationParts[w]->rotation.x));
								}
								matrixKeyframe = matrixKeyframe * (float4x4)matrixKeyframe.Scale(animations[x]->keyframes[z]->animationParts[w]->scale.x, animations[x]->keyframes[z]->animationParts[w]->scale.y, animations[x]->keyframes[z]->animationParts[w]->scale.z);

								break;
							}
						}

						fprintf(outFile, "%f %f %f %f  %f %f %f %f  %f %f %f %f  %f %f %f %f ", 
							matrixKeyframe.v[0][0], matrixKeyframe.v[0][1], matrixKeyframe.v[0][2], matrixKeyframe.v[0][3],
							matrixKeyframe.v[1][0], matrixKeyframe.v[1][1], matrixKeyframe.v[1][2], matrixKeyframe.v[1][3],
							matrixKeyframe.v[2][0], matrixKeyframe.v[2][1], matrixKeyframe.v[2][2], matrixKeyframe.v[2][3],
							matrixKeyframe.v[3][0], matrixKeyframe.v[3][1], matrixKeyframe.v[3][2], matrixKeyframe.v[3][3]
							);
					}
					fprintf(outFile, "</float_array>\n");
					fprintf(outFile, "        <technique_common>\n");
					fprintf(outFile, "          <accessor source=\"#%s_transform-output-array\" count=\"%d\" stride=\"16\">\n", joints[joint]->name, animations[x]->keyframes.size());
					fprintf(outFile, "            <param name=\"TRANSFORM\" type=\"float4x4\"/>\n");
					fprintf(outFile, "          </accessor>\n");
					fprintf(outFile, "        </technique_common>\n");
					fprintf(outFile, "      </source>\n");
					fprintf(outFile, "      <source id=\"%s_transform-interpolation\" name=\"%s_transform-interpolation\">\n", joints[joint]->name, joints[joint]->name);
					fprintf(outFile, "        <Name_array id=\"%s_transform-interpolation-array\" count=\"%d\">", joints[joint]->name, animations[x]->keyframes.size());
					for (int z = 0; z < animations[x]->keyframes.size(); z++)
					{
						fprintf(outFile, "LINEAR ");
					}
					fprintf(outFile, "</Name_array>\n");
					fprintf(outFile, "        <technique_common>\n");
					fprintf(outFile, "          <accessor source=\"#%s_transform-interpolation-array\" count=\"%d\" stride=\"1\">\n", joints[joint]->name, animations[x]->keyframes.size());
					fprintf(outFile, "            <param name=\"INTERPOLATION\" type=\"name\"/>\n");
					fprintf(outFile, "          </accessor>\n");
					fprintf(outFile, "        </technique_common>\n");
					fprintf(outFile, "      </source>\n");
					fprintf(outFile, "      <sampler id=\"%s_transform-sampler\">\n", joints[joint]->name);
					fprintf(outFile, "        <input semantic=\"INPUT\" source=\"#%s_transform-input\"/>\n", joints[joint]->name);
					fprintf(outFile, "        <input semantic=\"OUTPUT\" source=\"#%s_transform-output\"/>\n", joints[joint]->name);
					fprintf(outFile, "        <input semantic=\"INTERPOLATION\" source=\"#%s_transform-interpolation\"/>\n", joints[joint]->name);
					fprintf(outFile, "      </sampler>\n");
					fprintf(outFile, "      <channel source=\"#%s_transform-sampler\" target=\"%s/transform\"/>\n", joints[joint]->name, joints[joint]->name);
					fprintf(outFile, "    </animation>\n");
				}
			}

			// Just do one for now
			break;
		}

		fprintf(outFile, "  </library_animations>\n");
	}

	fprintf(outFile, "</COLLADA>\n");
	fflush(outFile);
	fclose(outFile);
}

CAnimationPart* CObjToAn8Dlg::GetAddPart(CString partName, CKeyframe* keyframe)
{
	for (int x = 0; x < keyframe->animationParts.size(); x++)
	{
		if (keyframe->animationParts[x]->name == partName)
		{
			return keyframe->animationParts[x];
		}
	}

	CAnimationPart* animationPart = new CAnimationPart();
	animationPart->name = partName;
	keyframe->animationParts.push_back(animationPart);
	return animationPart;
}

CKeyframe* CObjToAn8Dlg::GetAddKeyframe(int keyframeNumber, CAnimation* animation)
{
	for (int x = 0; x < animation->keyframes.size(); x++)
	{
		if (animation->keyframes[x]->number == keyframeNumber)
		{
			return animation->keyframes[x];
		}
	}

	CKeyframe* keyframe = new CKeyframe();
	keyframe->number = keyframeNumber;
	animation->keyframes.push_back(keyframe);
	return keyframe;
}

void CObjToAn8Dlg::OnDropFiles(HDROP hDropInfo)
{
	char tmpFileName[256]; 

	DragQueryFile(hDropInfo, 0, tmpFileName, 256); 

	CFileFind FileFind; 
	if (FileFind.FindFile(tmpFileName) == FALSE) return; 

	CString newDraggedFileName;
	newDraggedFileName.Format("%s", tmpFileName);

	mInputFilename.SetWindowText(newDraggedFileName);

	if (newDraggedFileName.GetLength() > 4)
	{
		int dotSpot = newDraggedFileName.ReverseFind('.');
		CString newOutName = newDraggedFileName.Mid(0, dotSpot) + ".an8";
		mOutputFilename.SetWindowText(newOutName);
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CObjToAn8Dlg::OnBnClickedButtonchoosereplaces()
{
	CFileDialog m_ldFile(TRUE, NULL, "test", OFN_HIDEREADONLY, "Replace Material File (*.rep)|*.rep||", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	if (statusFileOpen == IDOK)
	{
		mReplacesFilename.SetWindowText(m_ldFile.GetPathName());
	}
}

void CObjToAn8Dlg::OnBnClickedCheckdisableautoshading()
{
	if (mDisableAutoshading.GetCheck())
	{
		mDefaultShading.EnableWindow(false);
		mCheckScaleAmbientFactor.EnableWindow(false);
		mScaleAmbientFactor.EnableWindow(false);
		mCheckScaleDiffuseFactor.EnableWindow(false);
		mScaleDiffuseFactor.EnableWindow(false);
	}
	else
	{
		mDefaultShading.EnableWindow(true);
		mCheckScaleAmbientFactor.EnableWindow(true);
		mScaleAmbientFactor.EnableWindow(true);
		mCheckScaleDiffuseFactor.EnableWindow(true);
		mScaleDiffuseFactor.EnableWindow(true);
	}
}

void CObjToAn8Dlg::OnBnClickedCheckdefaultshading()
{
	if (mDefaultShading.GetCheck())
	{
		mDisableAutoshading.EnableWindow(false);
		mCheckScaleAmbientFactor.EnableWindow(false);
		mScaleAmbientFactor.EnableWindow(false);
		mCheckScaleDiffuseFactor.EnableWindow(false);
		mScaleDiffuseFactor.EnableWindow(false);
	}
	else
	{
		mDisableAutoshading.EnableWindow(true);
		mCheckScaleAmbientFactor.EnableWindow(true);
		mScaleAmbientFactor.EnableWindow(true);
		mCheckScaleDiffuseFactor.EnableWindow(true);
		mScaleDiffuseFactor.EnableWindow(true);
	}
}

void CObjToAn8Dlg::OnBnClickedCheckignoreshading()
{
	if (mIgnoreShading.GetCheck())
	{
		mIgnorePoint7Shades.EnableWindow(false);
	}
	else
	{
		mIgnorePoint7Shades.EnableWindow(true);
	}
}

void CObjToAn8Dlg::OnBnClickedCheckignoreshading2()
{
	if (mIgnorePoint7Shades.GetCheck())
	{
		mIgnoreShading.EnableWindow(false);
	}
	else
	{
		mIgnoreShading.EnableWindow(true);
	}
}

bool CObjToAn8Dlg::StringHasNumber(CString string)
{
	if (
		(string.Find('0') != -1) ||
		(string.Find('1') != -1) ||
		(string.Find('2') != -1) ||
		(string.Find('3') != -1) ||
		(string.Find('4') != -1) ||
		(string.Find('5') != -1) ||
		(string.Find('6') != -1) ||
		(string.Find('7') != -1) ||
		(string.Find('8') != -1) ||
		(string.Find('9') != -1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CObjToAn8Dlg::OnBnClickedButtonchooseoverrideskeleton()
{
	CFileDialog m_ldFile(TRUE, NULL, "skeleton.obj", OFN_HIDEREADONLY, "Skeleton File (*.obj)|*.obj||", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	if (statusFileOpen == IDOK)
	{
		mOverrideSkeletonFilename.SetWindowText(m_ldFile.GetPathName());
	}
}

bool CObjToAn8Dlg::IsHexDigit(char inChar)
{
	if (inChar == '0')
		return true;
	else if (inChar == '1')
		return true;
	else if (inChar == '2')
		return true;
	else if (inChar == '3')
		return true;
	else if (inChar == '4')
		return true;
	else if (inChar == '5')
		return true;
	else if (inChar == '6')
		return true;
	else if (inChar == '7')
		return true;
	else if (inChar == '8')
		return true;
	else if (inChar == '9')
		return true;
	else if (inChar == 'A')
		return true;
	else if (inChar == 'a')
		return true;
	else if (inChar == 'B')
		return true;
	else if (inChar == 'b')
		return true;
	else if (inChar == 'C')
		return true;
	else if (inChar == 'c')
		return true;
	else if (inChar == 'D')
		return true;
	else if (inChar == 'd')
		return true;
	else if (inChar == 'E')
		return true;
	else if (inChar == 'e')
		return true;
	else if (inChar == 'F')
		return true;
	else if (inChar == 'f')
		return true;
	else
		return false;
}

int CObjToAn8Dlg::HexToInt(CString inChar)
{
	if (inChar == "0")
		return 0;
	else if (inChar == "1")
		return 1;
	else if (inChar == "2")
		return 2;
	else if (inChar == "3")
		return 3;
	else if (inChar == "4")
		return 4;
	else if (inChar == "5")
		return 5;
	else if (inChar == "6")
		return 6;
	else if (inChar == "7")
		return 7;
	else if (inChar == "8")
		return 8;
	else if (inChar == "9")
		return 9;
	else if (inChar == "A")
		return 10;
	else if (inChar == "a")
		return 10;
	else if (inChar == "B")
		return 11;
	else if (inChar == "b")
		return 11;
	else if (inChar == "C")
		return 12;
	else if (inChar == "c")
		return 12;
	else if (inChar == "D")
		return 13;
	else if (inChar == "d")
		return 13;
	else if (inChar == "E")
		return 14;
	else if (inChar == "e")
		return 14;
	else if (inChar == "F")
		return 15;
	else if (inChar == "f")
		return 15;
	else
		return 0;
}

int CObjToAn8Dlg::ReturnThreeDigitsFromCharArray(char* newChar)
{
	CString tempStr;
	for (int x = 0; x < 3; x++)
	{
		if (IsHexDigit(newChar[x]))
		{
			CString tempCharStr;
			tempCharStr.Format("%s", newChar);
			tempStr += tempCharStr.Mid(x, 1);
		}
		else
			break;
	}

	int length = tempStr.GetLength();
	if (tempStr.GetLength() == 1)
		tempStr = "000";
	else if (tempStr.GetLength() == 1)
		tempStr = "00" + tempStr;
	else if (tempStr.GetLength() == 2)
		tempStr = "0" + tempStr;
	int tempShort = ((HexToInt(tempStr.Mid(0, 1)) << 0x8) | (HexToInt(tempStr.Mid(1, 1)) << 0x4) | HexToInt(tempStr.Mid(2, 1)));
	return tempShort;
}


int CObjToAn8Dlg::CharacterCount(CString csString_i, char sChar_i)
{
	if (csString_i.IsEmpty())
	{
		return 0;
	}
	int nFind = -1;
	int nCount = 0;
	while (-1 != (nFind = csString_i.Find(sChar_i, nFind + 1)))
	{
		nCount++;
	}
	return nCount;
}

bool CObjToAn8Dlg::SortPolygonGroupByTexture(CGroup* group)
{
	if (FindCaseInsensitive(group->name, "secondary") != -1)
		return true;

	int typePosition = FindCaseInsensitive(group->name, "Type", 0);
	if (typePosition != -1)
	{
		char newChar[1000];
		strcpy(newChar, group->name.Mid((typePosition + 4)));
		for (int rrr = 0; rrr < (group->name.Mid((typePosition + 4)).GetLength()); rrr++)
		{
			if ((newChar[rrr] == 0x20) || (newChar[rrr] == 0xA))
				newChar[rrr] = 0x0;
		}
		int ssbRoomType = ReturnThreeDigitsFromCharArray(newChar);
		if (ssbRoomType != 1)
			return true;
	}

	std::vector<CPolygon*> polygonsPrimary;
	std::vector<CPolygon*> polygonsSecondary;

	std::vector<CPolygon*>::iterator	iterpolygon;
	for (iterpolygon = group->polygons.begin(); iterpolygon != group->polygons.end(); iterpolygon++)
	{
		CPolygon* polygon = ((CPolygon*)(*iterpolygon));

		if ((polygon->materialName.Find("Transparent") != -1) || (polygon->materialName.Find("TopFlag") != -1) || (polygon->materialName.Find("Decal") != -1))
		{
			polygonsSecondary.push_back(polygon);
		} 
		else
		{
			polygonsPrimary.push_back(polygon);
		}
	}

	std::sort(polygonsPrimary.begin(), polygonsPrimary.end(), less_than_PolygonMaterial());

	group->polygons.clear();
	for (iterpolygon = polygonsPrimary.begin(); iterpolygon != polygonsPrimary.end(); iterpolygon++)
	{
		CPolygon* polygon = ((CPolygon*)(*iterpolygon));
		group->polygons.push_back(polygon);
	}

	for (iterpolygon = polygonsSecondary.begin(); iterpolygon != polygonsSecondary.end(); iterpolygon++)
	{
		CPolygon* polygon = ((CPolygon*)(*iterpolygon));
		group->polygons.push_back(polygon);
	}

	return true;
}