// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Camera::An8Camera(void)
{
	Fov = 60.0f;   // Fov
	Locked = false;
	Number = 0;
}

An8Camera::~An8Camera(void)
{
}


bool An8Camera::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"camera");
	if(elementStart<0) return false;

	// Camera
	p_parser->GetName(&Name);
	p_parser->ReadLine();

	int elementEnd = p_parser->IndexOf(1,"}");
	while(p_parser->Length()!=1+LINERETURN && elementStart != elementEnd)
	{
		if(p_parser->FindWord("controller {"))
		{
			++NbrController;
			An8Controller newController;
			newController.Load(p_parser);
			vControllers.push_back(newController);
		}

		//camera loc
		if(p_parser->FindWord("loc {"))
		{
			std::string strLine = p_parser->GetLine();
			p_parser->CutStr(&strLine,"(",1);
			string element = p_parser->SubStr(&strLine,""," ");
			Loc.x = (float) atof( element.c_str());

			p_parser->CutStr(&strLine," ",1);
			element = p_parser->SubStr(&strLine,""," ");
			Loc.y = (float) atof( element.c_str());

			p_parser->CutStr(&strLine," ",1);
			element = p_parser->SubStr(&strLine,"",")");
			Loc.z = (float) atof( element.c_str());
		}

		//camera orientation
		if(p_parser->FindWord("orientation {"))
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
	
		// Camera Roll
		if(p_parser->FindWord("roll { 1 }"))
		{
			EnableRoll=true;
		}

		//camera fov
		if(p_parser->FindWord("fov {"))
		{
			p_parser->CutLine("{",1);
			string element = p_parser->SubStr(" "," ");
			Fov = (float) atof( element.c_str());
		}

		// locked?
		if(p_parser->FindWord("locked { }"))
		{
			Locked=true;
		}

		// facestarget { }
		if(p_parser->FindWord("facestarget { }"))
		{
			//FacesTarget   = true;
			RelativeTo = AN8_FACINGOTHER;
		}

		// orienttarget { }
		if(p_parser->FindWord("orienttarget { }"))
		{
			//OrientTarget   = true;
			RelativeTo = AN8_OTHER;
		}

		// facespath { }
		if(p_parser->FindWord("facespath { }"))
		{
			//FacesPath   = true;
			RelativeTo = AN8_PATH;
		}
			
		if(p_parser->FindWord("target {") && !p_parser->FindWord("facestarget { }"))
		{
			p_parser->GetName(&TargetName);
		}

		// Number
		if(p_parser->FindWord("number {"))
		{
			p_parser->CutLine("{",1);
			string element = p_parser->SubStr(" "," ");
			Number = atoi( element.c_str());
		}

		if(p_parser->FindWord("attribute {"))
		{
			++NbrAttribute;
			An8Attribute newAttribute;
			newAttribute.Load(p_parser);
			vAttributes.push_back(newAttribute);
		} // End of if attribute

		// Camera
		if(p_parser->FindWord("camera {"))
    	{
			NbrCamera += 1;
			An8Camera newCamera;
			newCamera.Load(p_parser);
			vCameras.push_back(newCamera);

		}

		// ObjectElement
		if(p_parser->FindWord("objectelement {"))
		{
			NbrObjectElement +=1;
			An8ObjectElement newObjectElement;
			newObjectElement.Load(p_parser);
			vObjectElements.push_back(newObjectElement);
		}

		// FigureElement
		if(p_parser->FindWord("figureelement {"))
		{
			NbrFigureElement +=1;
			An8FigureElement newFigureElement;
			newFigureElement.Load(p_parser);
			vFigureElements.push_back(newFigureElement);
		}

		// null
		if(p_parser->FindWord("null {"))
		{
			NbrNull +=1;
			An8Null newNull;
			newNull.Load(p_parser);
			vNulls.push_back(newNull);
		}
		
		if( (p_parser->FindWord("light {")) && (!p_parser->FindWord("spotlight {")) )
		{
			NbrLight +=1;
			An8Light newLight;
			newLight.Load(p_parser);
			vLights.push_back(newLight);

		}
		

		p_parser->ReadLine();
		elementEnd =p_parser->IndexOf(1,"}");

	} // While not the end

	return true;
 
}

