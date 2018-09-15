// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr

#include "An8Loader.h"
using namespace std;

An8Bone::An8Bone(void)
{
	Length = 0;
	Diameter = 0;
	Locked = false;

	NbrBone = 0 ;
	NbrSphere = 0 ;
	NbrCylinder = 0 ;
	NbrNamedObject = 0 ;
	NbrDof = 0 ;
	HasInfluence = false;
}

An8Bone::~An8Bone(void)
{

}

void An8Bone::Load(AN8XPARSER*p_parser,std::vector<An8Material>& i_vMaterials)
{
	int boneStart = p_parser->IndexOf(1,"b");
	if( boneStart < 0) return;

	An8Bone newBone;
	An8NamedObject newNamedObject;
	An8Sphere newSphere;
	An8Cylinder newCylinder;
	An8Dof newDof;

	// Name
	p_parser->GetName(&Name);

	// Length
	p_parser->ReadLine();
	if(p_parser->FindWord("length {"))
	{
		string element = p_parser->SubStr("{ "," }");
		Length = (float)atof( element.c_str());

	}
	else{throw "length { not found in bone chunk.";}

	int currChunkEnd = p_parser->IndexOf(1,"}");

	// While we do not find the end of the figure
	// And while we are not at the end of the bone
	while( p_parser->IndexOf(1, "}")!= 0 && boneStart != currChunkEnd)
	{
		if(p_parser->FindWord("bone {"))
		{
			++NbrBone;
			vBones.push_back(newBone);
			vBones[vBones.size()-1].ParentName = this->Name;
			vBones[vBones.size()-1].Load(p_parser,i_vMaterials);
			
		}
		else if(p_parser->FindWord("locked {"))
		{
			Locked=true;
		}
		else if(p_parser->FindWord("orientation {"))
		{
			std::string strLine = p_parser->GetLine();
			p_parser->CutStr(&strLine,"(",1);
			string element = p_parser->SubStr(&strLine,""," ");
			Orientation.x = (float) atof( element.c_str());

			p_parser->CutStr(&strLine," ",1);
			element = p_parser->SubStr(&strLine,""," ");
			Orientation.y = (float) atof( element.c_str());

			p_parser->CutStr(&strLine," ",1);
			element = p_parser->SubStr(&strLine,""," ");
			Orientation.z = (float) atof( element.c_str());

			p_parser->CutStr(&strLine," ",1);
			element = p_parser->SubStr(&strLine,"",")");
			Orientation.w = (float) atof( element.c_str());
		}
		else if(p_parser->FindWord("dof {"))
		{
			++NbrDof;
			vDofs.push_back(newDof);
			vDofs[vDofs.size()-1].Load(p_parser);
		}
		else if(p_parser->FindWord("namedobject {"))
		{
			++NbrNamedObject;
			vNamedObjects.push_back(newNamedObject);
			vNamedObjects[vNamedObjects.size()-1].Load(p_parser);
			
		}
		else if(p_parser->FindWord("sphere {"))
		{
			++NbrSphere;
			vSpheres.push_back(newSphere);
			vSpheres[vSpheres.size()-1].Load(p_parser);
			vSpheres[vSpheres.size()-1].ComputeMaterialID(i_vMaterials);
			
		}
		else if(p_parser->FindWord("cylinder {"))
		{
			++NbrCylinder;
			vCylinders.push_back(newCylinder);
			vCylinders[vCylinders.size()-1].Load(p_parser);
			vCylinders[vCylinders.size()-1].ComputeMaterialID(i_vMaterials);
			
		}
		else if(p_parser->FindWord("influence {"))
		{
			Influence.Load(p_parser);
			this->HasInfluence = true;
		}
		p_parser->ReadLine();
	    currChunkEnd =p_parser->IndexOf(1,"}");
	}

}
