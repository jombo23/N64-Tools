/*
* Copyright (c) 2002-2010, Grégory Smialek
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice, 
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation 
*     and/or other materials provided with the distribution.
*   * Neither the name of the Grégory Smialek nor the names of its contributors
*     may be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* web site: http://texel3d.free.fr/projets/liban8/index.html
*/

//////HEADER/////////////An8Loader.h///////////////////////
///     .an8 file loading module (2002-2010)			///
///     Version 10.03.17       By Grégory Smialek       ///
///////////texel3d.free.fr//////texel_3d@yahoo.com/////////

#ifndef _Anim8or_H
#define _Anim8or_H



#ifndef AN8_WIN32
#define AN8_WIN32
#endif


#define LINERETURN 1

#ifdef AN8_WIN32
	#define _CRT_SECURE_NO_DEPRECATE
#endif


#include<string>
#include <vector>

#include"AN8X.h"


#define AN8_INFINITELIGHT 1
#define AN8_LOCALLIGHT 2
#define AN8_SPOTLIGHT 3

#define AN8_UNDEFINED  0
#define AN8_FLOAT      1
#define AN8_INTEGER    2
#define AN8_POINT3     3
#define AN8_QUATERNION 4
#define AN8_STRING     5

#define AN8_COLOR      0
#define AN8_IMAGE      1
#define AN8_CUBEMAP    2
#define AN8_PANORAMA   3

#define AN8_PARENT      0
#define AN8_PATH        1
#define AN8_OTHER       2
#define AN8_FACINGOTHER 3

#define AN8_NORMAL 1

// Blending Mode
#define AN8_DECAL   0
#define AN8_DARKEN  1
#define AN8_LIGHTEN 2

// Alpha Mode
#define AN8_NONE  0
#define AN8_LAYER 1
#define AN8_FINAL 2

// CubeMap Index
#define AN8_RIGHT	0
#define AN8_LEFT	1
#define AN8_BOTTOM	2
#define AN8_TOP		3
#define AN8_FRONT	4
#define AN8_BACK	5


class An8vpi2D{
public:
	int P1;
	int P2;
	An8vpi2D();
	An8vpi2D(int pt1,int pt2);
	~An8vpi2D();
};

class An8Scene;
class An8Object;
class An8File;
class An8Controller;
class An8Material;



class An8Attribute
{
public:
	std::string Name; // Name
	int Type;         // AN8_UNDEFINED,AN8_FLOAT,AN8_INTEGER,AN8_POINT3,AN8_QUATERNION,AN8_STRING

	float          FloatValue;
	int            IntegerValue;
	AN8XVECTOR3    Point3Value;
	AN8XQUATERNION QuaternionValue;
	std::string    StringValue;

	void Load(AN8XPARSER*p_parser);

	An8Attribute();
	~An8Attribute();
};

class An8Light;
class An8Camera;
class An8ObjectElement;
class An8FigureElement;
class An8Null;

class An8Element
{
public:
	std::string Name;      // Name

	AN8XVECTOR3    Loc;         // Position (from .an8. Can by relative to something)
	AN8XQUATERNION Orientation; // quaternion rotation (from .an8. Can by relative to something)

	// Absolute poistion/orientation computed by ComputeAllMatrices()
	AN8XMATRIX  Matrix;      // Absolute Matrix (rotation, translation,scale)
	AN8XVECTOR3 LookAtLoc;   // Location
	AN8XVECTOR3 LookAtUp;    // Up vector
	AN8XVECTOR3 LookAtEye;   // Eye (in front of loc)

	float Scale;

	bool EnableRoll;
	bool Locked;


	int NbrAttribute;
	std::vector<An8Attribute> vAttributes;

	int NbrController;
	std::vector<An8Controller> vControllers;

	int RelativeTo;
    std::string TargetName;

	// For RelativeTo: Parent ( it's like groups)
	int NbrCamera;
	std::vector<An8Camera> vCameras;
	int NbrObjectElement;
	std::vector<An8ObjectElement> vObjectElements;
	int NbrFigureElement;
	std::vector<An8FigureElement> vFigureElements;
	int NbrNull;
	std::vector<An8Null> vNulls;
	int NbrLight;
	std::vector<An8Light> vLights;

	An8Element* ptrParent; // Pointer to the parent (if relative to parent)
	An8Element* ptrTarget; // Pointer to the target (facing other...)

	// Compute all absolute matrices (element and children)
	void ComputeAllMatrices( void );

	// Get Pointer of a target (facing other...)
	An8Element* GetTargetPointer(An8Element* o_ptr, std::string i_TargetName);

	// Fill ptrParent and ptrTarget (to use if you add/remove elements)
	void SetElementsPointer(An8Scene* i_Scene,An8Element* i_ptrParent);

	// You should not use this function
	void ComputeMatrices( void );

	std::vector<An8ObjectElement*> GetObjectElements(void);
	std::vector<An8FigureElement*> GetFigureElements(void);
	std::vector<An8Camera*> GetCameras(void);
	std::vector<An8Light*>  GetLights(void);
	std::vector<An8Null*>   GetNulls(void);

private:

	AN8XMATRIX OrientationMatrix;   // Alpha
	AN8XMATRIX TranslationMatrix;  // Alpha
	AN8XMATRIX ScaleMatrix;   // Alpha

	void ComputeLookAt(AN8XVECTOR4* i_targetPosition); // Alpha
	void ComputeSimpleMatrix( void ); // Alpha

public:
	An8Element(void);
public:
	~An8Element(void);

};

class An8Light : public An8Element
{
public:

	int Color[3]; // RGB Color

	// AN8_INFINITELIGHT,AN8_LOCALLIGHT,AN8_SPOTLIGHT,AN8_UNDEFINED
	int Kind;

	float InRadius;  // Local,spotlight
	float OutRadius; // Local,spotlight

	float InAngle; //spotlight
    float OutAngle; //spotlight

    bool CastShadow; // yes/no
	int  PercentShadow;

    bool  RaytraceShadow;
    bool  Soft;
    float SoftSize;
    int   MinSamples;
    int   MaxSamples;
    bool  MonteCarlo;

	bool Load(AN8XPARSER*p_parser);

public:
	An8Light(void);
public:
	~An8Light(void);
};


class An8Camera : public An8Element
{
public:

	float Fov;          ///< Fov
	int Number;

	bool Load(AN8XPARSER*p_parser);

public:
	An8Camera(void);
public:
	~An8Camera(void);
};


class An8Null : public An8Element
{
public:

	bool Load(AN8XPARSER*p_parser);

public:
	An8Null(void);
public:
	~An8Null(void);
};


class An8FigureElement : public An8Element
{
public:
	std::string SourceName;

	bool HasSequenceName;
	std::string SequenceName;

    bool CastShadow;
    bool ReceiveShadow;

	bool Load(AN8XPARSER*p_parser);

public:
	An8FigureElement(void);
	~An8FigureElement(void);
};

class An8ObjectElement : public An8Element
{
public:

	std::string SourceName;

	int ObjectID; ///< Id number of the object in vObjects

    bool CastShadow;
    bool ReceiveShadow;

	bool Load(AN8XPARSER*p_parser);

public:
	An8ObjectElement(void);
	~An8ObjectElement(void);
};



class An8FaceIndice{
public:
	int Point;    // Vertex
	int Normal;   // Normal
	int TexCoord; // Texture Coordinate

	An8FaceIndice();
	An8FaceIndice(int point,int normal,int texCoord);
	~An8FaceIndice();
};



class An8Face{
public:
	int NbrVertex;  // Number of vertices in the face

	int Mask;      // Mask. Normal if 6, 2 or 7. TexCoord if >= 4
	/*
	Binary number converted to decimal. Computed like this: It has 3 digits:
	_ 1st bit (left) is set to 1 if backfaces must be displayed or 0 if not (default value).
	_ 2nd bit: set to 1 if each vertices have there normal or 0 if not (default value).
	_ 3rd bit: set to 1 if uv coordinate are used or 0 if not (default value).
	Then, this binary number is converted to decimal.
	Some examples:
	   0: No uv coords
	   1: Show backfaces.
	   2: Vertex normal are exported (with "Debug->Output normal").
	   4: UV coords are used by the face.
	   ex: 6 : Normal + UV
*/
	int Material;    // Material ID in materiallist of An8Mesh
	int NormalOption; // If > -1 : Index of the normal of the face in vNormals of An8Mesh

	std::vector<An8FaceIndice>	vIndices;  // Vector of indices

	AN8XVECTOR3 Normal;  // Face normal from ComputeFaceNormals();
	//AN8XVECTOR3 vertexNormal[3];  Deprecated
	AN8XVECTOR3 vertexTangents[3];
	AN8XVECTOR3 vertexBinormals[3];

	bool Load(AN8XPARSER*p_parser);

	// utility functions
	bool HasVertexNormal(void);
	bool HasTexCoord(void);

	An8Face();
	~An8Face();

};

class An8Header
{
public:
	std::string Build;     // Build data version number
	std::string Version;
public:
	An8Header();
	~An8Header();
	bool Load(AN8XPARSER*p_parser);
};

struct An8Size
{
	int Width;
	int Height;
	An8Size(){Width=400;Height=300;};
	~An8Size(){};
};

struct An8Ratio
{
	float Width;
	float Height;
	An8Ratio(){Width=4;Height=3;};
	~An8Ratio(){};
};

struct An8Image
{
	An8Size Size;

public:
	An8Image();
	~An8Image();
	bool Load(AN8XPARSER*p_parser);
};

class An8Film
{
public:
	An8Size Size;
	An8Ratio Ratio;
public:
	An8Film();
	~An8Film();
	bool Load(AN8XPARSER*p_parser);
};

class An8Lighting
{
public:
	float Intensity;
	float AmbientIntensity;
public:
	An8Lighting();
	~An8Lighting();
	bool Load(AN8XPARSER*p_parser);
};


class An8Description
{
public:
	std::string String;
public:
	An8Description();
	~An8Description();
	bool Load(AN8XPARSER*p_parser);
};


class An8Environment
{
public:
	int IsAutoGrid;
	float ModelingGridSpacing;
	float SceneEditorGridSpacing;
	float GroundFloorGridSize;
	int FrameRate;
	An8Film Film;
	An8Image Image;
	An8Lighting	Lighting;
public:
	An8Environment();
	~An8Environment();
	bool Load(AN8XPARSER*p_parser);
};

class An8Texture
{
public:
	std::string Name;
	std::string FileName;

	bool IsCubeMap;  // Is this texture a CubeMap ? ( false = no)
	std::string CubeMap[6];
	int Invert;

//	int CubeMapTexID[6];

public:
	An8Texture();
	~An8Texture();
	bool Load(AN8XPARSER*p_parser);
	void ClearTexturePath(void);
};

class An8Cube{
public:
	std::string Name;
	AN8XVECTOR3 BaseOrigin;
	AN8XQUATERNION BaseOrientation;
	AN8XMATRIX Matrix;

	std::string  Material;
	int MaterialID;

	AN8XVECTOR3 Scale;
	int DivX,DivY,DivZ;

	AN8XBOUNDINGBOX BoundingBox;
	AN8XVECTOR3 BoundingSphereCenter;
	float BoundingSphereRadius;
	void ComputeBoundingElements(void);

	void Load(AN8XPARSER*p_parser);
	void ComputeMaterialID(std::vector<An8Material>& i_vMaterials);
	void ComputeMatrix(bool includeTranslation);
	void ComputePosition(AN8XQUATERNION *i_pBaseOrientation = NULL,AN8XVECTOR3* i_pBaseOrigin = NULL);
	An8Cube();
	~An8Cube();
};

class An8Sphere{
public:
	std::string    Name;

	AN8XVECTOR3    BaseOrigin;
	AN8XQUATERNION BaseOrientation;
	AN8XMATRIX     Matrix;

	int Kind;  // 0=normal, 1=geodesic

	std::string Material;
	int MaterialID;

	float Diameter;
	int   DivLong;
	int   DivLat;
	int   Geodesic;

	AN8XBOUNDINGBOX BoundingBox;
	void ComputeBoundingElements(void);

	void Load(AN8XPARSER*p_parser);
	void ComputeMaterialID(std::vector<An8Material>& i_vMaterials);
	void ComputeMatrix(bool includeTranslation);
	void ComputePosition(AN8XQUATERNION *i_pBaseOrientation = NULL,AN8XVECTOR3* i_pBaseOrigin = NULL);
	An8Sphere();
	~An8Sphere();
};

class An8Cylinder{
public:
	std::string Name;
	AN8XVECTOR3 BaseOrigin;
	AN8XQUATERNION BaseOrientation;
	AN8XMATRIX Matrix;

	std::string Material;
	int MaterialID;

	float Length;
	float StartDiameter;
	float EndDiameter;
	int DivLong;
	int DivLat;
	bool CapStart;
	bool CapEnd;

	AN8XBOUNDINGBOX BoundingBox;
	AN8XVECTOR3     BoundingSphereCenter;
	float           BoundingSphereRadius;
	void ComputeBoundingElements(void);

	void Load(AN8XPARSER*p_parser);
	void ComputeMaterialID(std::vector<An8Material>& i_vMaterials);
	void ComputeMatrix(bool includeTranslation);
	void ComputePosition(AN8XQUATERNION *i_pBaseOrientation = NULL,AN8XVECTOR3* i_pBaseOrigin = NULL);
	An8Cylinder();
	~An8Cylinder();
};

class An8MorphOffset
{
public:
	int Index;
	AN8XVECTOR3 Position;

	void Load(AN8XPARSER*p_parser);

	An8MorphOffset();
	~An8MorphOffset();
};

class An8MorphOffsets
{
public:
	std::string Name;

	int NbrMorphOffset;
	std::vector<An8MorphOffset>	 vMorphOffsets;
	void Load(AN8XPARSER*p_parser);

	An8MorphOffsets();
	~An8MorphOffsets();
};



class An8MorphTarget
{
public:
	std::string Name;
	void Load(AN8XPARSER*p_parser);
	An8MorphTarget();
	~An8MorphTarget();
};

class An8TexGen
{
public:
	std::string Mode;

    AN8XVECTOR2 Scale;
    AN8XVECTOR3 Center;
    AN8XVECTOR3 Up;
    AN8XVECTOR3 Forward;

	An8TexGen();
	~An8TexGen();

	void Load(AN8XPARSER*p_parser);
};


////An8Mesh////////////
class An8Mesh{
public:
    int IsSubdivision;  // = 0 if no, = 1 if yes
    int Working;
    int Divisions;

	std::string Name;

	std::string Material;
	AN8XVECTOR3 BaseOrigin;
	AN8XQUATERNION BaseOrientation;
	AN8XMATRIX Matrix;
	int SmoothAngle;
	int HasMatrial; // = 0 if no, = 1 if yes

	bool HasTexGen;
	An8TexGen TexGen;


	int NbrMateriallist;
	std::vector<int> vMatNumber;	// order of the material in the materiallist

	int NbrMorphOffsets;
	std::vector<An8MorphOffsets> vMorphOffsets;

	int NbrFace;
	std::vector<An8Face>	 vFaces;		 // Faces from .an8 file

	int NbrPoint;
	std::vector<AN8XVECTOR3>	 vPoints;    // Vertices from .an8 file

	int NbrEdge;
    std::vector<An8vpi2D> vEdges;		 // Edges from .an8 file

	int NbrNormal; 
	std::vector<AN8XVECTOR3>	 vNormals;   // Normals from .an8 file
	// With vNormals, 1 point can have more than 1 normal (1 per face)

	int NbrBinormal;
	std::vector<AN8XVECTOR3>	 vBinormals;   // Binormals from .an8 file

	int NbrUVCoord;
	std::vector<AN8XVECTOR2>	 vTexCoords; // used if NbrUVCoords>0


	std::vector<AN8XVECTOR3>	vSmoothNormals;	// Normals computed by ComputeSmoothNormals()
	// 1 normal per vertex: vSmoothNormals.size() = vPoints.size() 
	void ComputeSmoothNormals(void); // Compute vSmoothNormals
	// Note: This version of ComputeSmoothNormals do not create new point. Normal could be too "wrong"
	//	In futur version new points (and face index recomputation) could be created using smoothangle


	///Extra features computed by ComputeBoundingElements()
	//float BoundingBox[6];
	AN8XBOUNDINGBOX BoundingBox;
	AN8XVECTOR3 BoundingSphereCenter;
	float BoundingSphereRadius;

	An8Mesh();
	~An8Mesh();
	void Load(AN8XPARSER*p_parser,An8Object* object);
	void Triangulize(void);


	void ComputeBoundingElements(void);
	void ComputeMatrix(bool includeTranslation);
	void ComputePosition(AN8XQUATERNION *i_pBaseOrientation = NULL,AN8XVECTOR3* i_pBaseOrigin = NULL);

	void ComputeFaceNormal(void);

	// Compute and store Tangents/Binormals in faces
	// You should better use binormals from the .an8 file (if present)
	void ComputeTangentsBinormals(void);

private:
	void ReadMateriallist(AN8XPARSER*p_parser,An8Object* object);
	void ReadPoints(AN8XPARSER*p_parser);
	void ReadAn8Edges(AN8XPARSER*p_parser);
	void ReadAn8Normals(AN8XPARSER*p_parser);
	void ReadAn8Binormals(AN8XPARSER*p_parser);
	void ReadTexCoords(AN8XPARSER*p_parser);


};


class An8Group{

public:
	std::string Name;
	AN8XVECTOR3 BaseOrigin;
	AN8XQUATERNION BaseOrientation;
	AN8XMATRIX Matrix; // Orientation (Matrix)

	int NbrMesh;
	int NbrSubdivision;
	int NbrCube;
	int NbrSphere;
	int NbrCylinder;

	std::vector<An8Cube>	 vCubes;
	std::vector<An8Sphere>   vSpheres;
	std::vector<An8Cylinder> vCylinders;
	std::vector<An8Mesh>	 vMeshes;

	int NbrGroup;
	std::vector<An8Group> vGroups;

	///Extra features///
	AN8XBOUNDINGBOX BoundingBox;
	AN8XVECTOR3 BoundingSphereCenter;
	float BoundingSphereRadius;

	An8Group();
	~An8Group();
	void Load(AN8XPARSER*p_parser,An8Object* object);
	std::vector<An8Mesh> GetMeshes(void);
	std::vector<An8Cube> GetCubes(void);
	std::vector<An8Sphere> GetSpheres(void);
	std::vector<An8Cylinder> GetCylinders(void);

	void ComputeMatrix(bool includeTranslation);
	void ComputeAllMatrices(bool includeTranslation);

	void Ungroup(void);
	void Triangulize(int numMesh = -1);
	void ComputeFaceNormal(void);
	void ComputeAllPositions(AN8XQUATERNION *i_pBaseOrientation = NULL,AN8XVECTOR3* i_pBaseOrigin = NULL);

	void ConvertAllToMesh( void );

	void ComputeBoundingElements(void);

	int GetNbrMesh(void);
	int GetMeshID(std::string name);
	bool HasNormal(void);

};



////An8Materials////////////
class An8Material{

public:

	std::string Name;

	int NbrAttribute;
	std::vector<An8Attribute> vAttributes;

	bool HasBumpMap;
	bool UseAlpha;
	bool HasTransparency;
	bool HasEnvironment;

	bool   LockAmbiantDiffuse;

	int   AmbiantRGB[3];
	float AmbiantFactor;
	bool   AmbiantIsTexture;
	std::string  AmbiantTexName;
	int	  AmbiantBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   AmbiantAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   AmbiantTextureID;

	int   DiffuseRGB[3];
	float DiffuseFactor;
	bool   DiffuseIsTexture;
	std::string  DiffuseTexName;
	int	  DiffuseBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   DiffuseAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   DiffuseTextureID;

	int   SpecularRGB[3];
	float SpecularFactor;
	bool   SpecularIsTexture;
	std::string  SpecularTexName;
	int	  SpecularBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   SpecularAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   SpecularTextureID;

	int   EmissiveRGB[3];
	float EmissiveFactor;
	bool   EmissiveIsTexture;
	std::string  EmissiveTexName;
	int	  EmissiveBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   EmissiveAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   EmissiveTextureID;

	int   Alpha; // Default: 255
	float Brilliance; // Default: 1.0f
	float PhongSize; // Default: 32.0f

	std::string  TransparencyTexName;
	int   TransparencyPercent;
	int	  TransparencyBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   TransparencyAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   TransparencyTextureID;

	std::string  EnvironmentTexName;
	int   EnvironmentPercent;
	int	  EnvironmentBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   EnvironmentAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   EnvironmentTextureID;

	std::string  BumpMapTexName;
	int   BumpMapPercent;
	int	  BumpMapBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BumpMapAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BumpMapTextureID;

/// Back ////////

	bool HasBackSurface;

	bool BackHasBumpMap;
	bool BackUseAlpha;
	bool BackHasTransparency;
	bool BackHasEnvironment;

	bool BackLockAmbiantDiffuse;

	int   BackAmbiantRGB[3];
	float BackAmbiantFactor;
	bool   BackAmbiantIsTexture;
	std::string  BackAmbiantTexName;
	int	  BackAmbiantBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackAmbiantAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackAmbiantTextureID;

	int   BackDiffuseRGB[3];
	float BackDiffuseFactor;
	bool   BackDiffuseIsTexture;
	std::string  BackDiffuseTexName;
	int	  BackDiffuseBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackDiffuseAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackDiffuseTextureID;

	int   BackSpecularRGB[3];
	float BackSpecularFactor;
	bool   BackSpecularIsTexture;
	std::string  BackSpecularTexName;
	int	  BackSpecularBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackSpecularAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackSpecularTextureID;

	int   BackEmissiveRGB[3];
	float BackEmissiveFactor;
	bool   BackEmissiveIsTexture;
	std::string  BackEmissiveTexName;
	int	  BackEmissiveBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackEmissiveAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackEmissiveTextureID;

	int   BackAlpha; // Default: 255
	float BackBrilliance; // Default: 1.0f
	float BackPhongSize; // Default: 32.0f

	std::string  BackTransparencyTexName;
	int   BackTransparencyPercent;
	int	  BackTransparencyBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackTransparencyAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackTransparencyTextureID;

	std::string  BackEnvironmentTexName;
	int   BackEnvironmentPercent;
	int	  BackEnvironmentBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackEnvironmentAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackEnvironmentTextureID;

	std::string  BackBumpMapTexName;
	int   BackBumpMapPercent;
	int	  BackBumpMapBlendMode; // N8_DECAL,AN8_DARKEN,AN8_LIGHTEN
	int   BackBumpMapAlphaMode; // AN8_NONE,AN8_LAYER,AN8_FINAL
	int   BackBumpMapTextureID;

	void Load(AN8XPARSER*p_parser);
	void SetDefault(void);


	An8Material();
	~An8Material();

	private:
		void LoadSurface(AN8XPARSER*p_parser);
		void LoadBackSurface(AN8XPARSER*p_parser);
};


////An8Objects////////////
class An8Object{

public:
	std::string Name;
	int NbrMesh;
	int NbrCube;
	int NbrSphere;
	int NbrCylinder;
	int NbrMaterial;
	int  NbrSubdivision;  // Number of subdivisions in the object

	std::vector<An8Cube>	 vCubes;
	std::vector<An8Sphere>   vSpheres;
	std::vector<An8Cylinder> vCylinders;

	std::vector<An8Mesh>	 vMeshes;
	std::vector<An8Material> vMaterials;

	int NbrAttribute;
	std::vector<An8Attribute> vAttributes;

	int NbrMorphTarget;
	std::vector<An8MorphTarget> vMorphTargets;	// order of the material in the materiallist

	int NbrGroup;
	std::vector<An8Group> vGroups;

	///Extra features///
	AN8XBOUNDINGBOX BoundingBox;
	AN8XVECTOR3 BoundingSphereCenter;
	float BoundingSphereRadius;

	void Ungroup(int numGroup = -1); //
	void Triangulize(int numMesh = -1);
	void ComputeFaceNormal(void);
	void ComputeAllPositions(void);
	void ComputeAllMatrices(bool includeTranslation);

	void ConvertAllToMesh( void );

	void ComputeBoundingElements(void); // ToDo

	int GetNbrMesh(void);
	int GetMeshID(std::string name);

	bool HasNormal(void);

public:
	An8Object();
	~An8Object();

	bool Load(AN8XPARSER*p_parser);

private:
	bool LoadMaterials(AN8XPARSER*p_parser);
	bool LoadMeshes(AN8XPARSER*p_parser);
};

class An8FloatKey
{
public:

	int FrameNumber;

	int NbrFloat;
	std::vector<float> vFloats;

	std::string KnotBehavior;

	An8FloatKey();
	~An8FloatKey();
	bool Load(AN8XPARSER*p_parser);
};



class An8QKey
{
public:

	int FrameNumber;
	AN8XQUATERNION Quaternion;
	std::string KnotBehavior;

	An8QKey();
	~An8QKey();
	bool Load(AN8XPARSER*p_parser);
};

class An8BooleanKey
{
public:

	int FrameNumber;
	bool Value;
	std::string KnotBehavior;

	An8BooleanKey();
	~An8BooleanKey();
	bool Load(AN8XPARSER*p_parser);
};

class An8PointKey
{
public:

	int FrameNumber;

	AN8XVECTOR3 Value;
	AN8XVECTOR3 Forward;
	AN8XVECTOR3 Reverse;

	std::string KnotBehavior;

	An8PointKey();
	~An8PointKey();
	bool Load(AN8XPARSER*p_parser);
};

class An8Track
{
public:

	int NbrFloatKey;
	std::vector<An8FloatKey> vFloatKeys;

	int NbrQKey;
	std::vector<An8QKey> vQKeys;

	int NbrPointKey;
	std::vector<An8PointKey> vPointKeys;

	int NbrBooleanKey;
	std::vector<An8BooleanKey> vBooleanKeys;

	An8Track();
	~An8Track();
	bool Load(AN8XPARSER*p_parser);
};

class An8JointAngle
{
public:
	std::string BoneName;
	int Axe; // 1 = X, 2 = Y, 3 = Z

	bool HasTrack;
	An8Track Track;

	An8JointAngle();
	~An8JointAngle();

	bool Load(AN8XPARSER*p_parser);
};

class An8Sequence
{
public:
	std::string Name;
	std::string FigureName;
	int NbrFrame;

	int NbrJointAngle;
	std::vector<An8JointAngle> vJointAngles;

	An8Sequence();
	~An8Sequence();

	bool Load(AN8XPARSER*p_parser);
};

class An8Controller
{
public:
	std::string Name;

	bool IsUser;
	std::string Type;

	bool HasTrack;
	An8Track Track;

	An8Controller();
	~An8Controller();

	bool Load(AN8XPARSER*p_parser);
};

class An8Dof
{
public:
	std::string Axe;
	float Min;
	float Default;
	float Max;
	bool Locked;
	bool Unlimited;

	void Load(AN8XPARSER*p_parser);

	An8Dof();
	~An8Dof();
};

class An8WeightValue
{
public:
	int BoneIndex;
	float Weight;

	An8WeightValue();
	~An8WeightValue();
};

class An8VertexWeight
{
public:

	int NbrOfWeightValue;
	std::vector<An8WeightValue>	vWeightValues;

	void Load(std::string strWeight);

	An8VertexWeight();
	~An8VertexWeight();
};

class An8Weights
{
public:
	std::string MeshName;

	int NbrOfVertexWeight;
	std::vector<An8VertexWeight>	vVertexWeights;

	void Load(AN8XPARSER*p_parser);

	An8Weights();
	~An8Weights();
};

class An8NamedObject
{
public:
	std::string ObjectName;
	std::string Name;
	AN8XVECTOR3    Origin;
	AN8XQUATERNION Orientation;
	std::string Material;

	int NbrWeightedBy;
	std::vector<std::string>	vWeightedBy;

	int NbrWeights;
	std::vector<An8Weights>	vWeights;

	void Load(AN8XPARSER*p_parser);

	An8NamedObject();
	~An8NamedObject();
};

class An8Influence
{
public:
	float CenterStart;
	float InRadiusStart;
	float OutRadiusStart;
	float CenterEnd;
	float InRadiusEnd;
	float OutRadiusEnd;

	void Load(AN8XPARSER*p_parser);
	An8Influence();
	~An8Influence();
};

class An8Bone
{
public:
	std::string Name;
	std::string ParentName;

	float Length;
	AN8XQUATERNION Orientation;

	float Diameter;
	bool Locked;

	int NbrBone;
	std::vector<An8Bone>	vBones;

	int NbrSphere;
	std::vector<An8Sphere> vSpheres;

	int NbrCylinder;
	std::vector<An8Cylinder> vCylinders;

	int NbrNamedObject;
	std::vector<An8NamedObject> vNamedObjects;

	int NbrDof;
	std::vector<An8Dof> vDofs;

	bool HasInfluence;
	An8Influence Influence;


	void Load(AN8XPARSER*p_parser,std::vector<An8Material>& i_vMaterials);

	An8Bone();
	~An8Bone();
};

class An8Figure
{
public:

	std::string Name;

	int NbrMaterial;
	std::vector<An8Material> vMaterials;

	An8Bone Root;

	bool Load(AN8XPARSER*p_parser);
	An8Figure();
	~An8Figure();
};

class An8Panorama
{
public:
	float Left;
	float Right;
	float Hight;
	float Low;

	An8Panorama(){Left=-90.0f;Right=90.0f;Hight=-30.0f;Low=30.0f;};
	~An8Panorama(){};
};

////An8Scene////////////
class An8Scene{
public:
	std::string Name;     // Name of the scene

	int  BackGround[3]; // Background color
	int	 BackGroundType; // AN8_COLOR, AN8_IMAGE or AN8_CUBEMAP, AN8_PANORAMA
	std::string BackGroundImage;
	An8Panorama BackGroundPanorama;

	bool   GroundGrid;   // = true if the groundgrid is activated

	bool   HasFog;        // = 0 if no, = 1 if yes
	int   FogColor[3];    // RGB
	float FogStart;
	float FogEnd;
	int   FogPercent;

	float Znear;  // Z-Near clip plane
	float Zfar;   // Z-Far clip plane

	bool HasLighting; // By default Lighting is not present in the file
	An8Lighting	Lighting; // Lighting in the scene

	int NbrCamera;
	std::vector<An8Camera> vCameras;

	int NbrObjectElement;
	std::vector<An8ObjectElement> vObjectElements;

	int NbrFigureElement;
	std::vector<An8FigureElement> vFigureElements;

	int NbrNull;
	std::vector<An8Null> vNulls;

	int NbrLight;
	std::vector<An8Light> vLights;

	bool Load(AN8XPARSER*p_parser);
	void FindObjectID(An8File *aniFile);

	std::vector<An8ObjectElement*> GetObjectElements(void);
	std::vector<An8FigureElement*> GetFigureElements(void);
	std::vector<An8Light*> GetLights(void);
	std::vector<An8Camera*> GetCameras(void);
	std::vector<An8Null*> GetNulls(void);

	void SetElementsPointer( void );
	An8Element* GetTargetPointer(An8Element* o_ptr, std::string i_TargetName);

	void ComputeAllMatrices( void );

	An8Scene();
	~An8Scene();

};


class An8File
{
private:
	AN8XPARSER* p_parser;
	bool _IsExternalParser;

public:



	std::string FileName;

	An8Header Header;

	bool HasDescription;
	An8Description Description;

	An8Environment Environment;

   // bool IsEmpty;        // = true if the An8Scene class is empty
	bool HasNormal(void);

	// Textures
	int NbrTexture;                // Number of texture in the file
	std::vector<An8Texture>	vTextures; // List of textures

	// Objects
	int NbrObject;                // Number of objects in the file
	std::vector<An8Object>	vObjects; // List of objects in the file

	// Figures
	int NbrFigure;                // Number of Figures in the file
	std::vector<An8Figure>	vFigures; // List of Figures in the file

	// Figures
	int NbrSequence;                // Number of Figures in the file
	std::vector<An8Sequence> vSequences; // List of Figures in the file

	// Scenes
	int NbrScene;                 // Number of scenes in the file
	std::vector<An8Scene>	vScenes;  // List of scenes in the file


public:

	void ConvertAllToMesh( void );

	void Ungroup( void ); //
	void Triangulize( void );
	void ComputeFaceNormal(void);

	void ComputeAllPositions(void);
	void ComputeAllMatrices( void );
	//void ComputeTangentsBinormals(void);
	void ComputeTextureIDForMaterial(void);

	void SetElementsPointer( void );

	AN8XPARSER* GetParser();
	void SetParser(AN8XPARSER* i_pParser);

	// utility function
	int GetNbrMesh(void);
	int GetFigureID(std::string name);
	int GetObjectID(std::string name);
	int GetSequenceID(std::string name);


	An8File(void);
	~An8File(void);

	// Load a .an8 file
	bool LoadFile(std::string file_name);

	// Load a .an8 file using an external parser
	bool Load( void );
};







#endif	// _Anim8or_H
