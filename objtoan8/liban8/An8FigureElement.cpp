// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8FigureElement::An8FigureElement(void)
{
	HasSequenceName = false;

	EnableRoll = true;

    CastShadow = false;
    ReceiveShadow = false;

}

An8FigureElement::~An8FigureElement(void)
{

}


bool An8FigureElement::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"figureelement");
	if(elementStart<0) return false;

	// Name
	p_parser->Get2Names(&Name,&SourceName);
	p_parser->ReadLine();

	int elementEnd = p_parser->IndexOf(1,"}");
	while(p_parser->Length()!=1+LINERETURN && elementStart != elementEnd)
	//while(p_parser->Length()!=(3+LINERETURN))
	{
		
		if(p_parser->FindWord("controller {"))
		{

			++NbrController;
			An8Controller newController;
			newController.Load(p_parser);
			vControllers.push_back(newController);
		}


		// Location
		if(p_parser->FindWord("loc {"))
		{
			sscanf(p_parser->GetLine(),"  loc { (%f %f %f\n",&Loc.x,
												   &Loc.y,
												   &Loc.z);

		}

		// Orientation
		if(p_parser->FindWord("orientation {"))
		{
			//LocOption=2;
			sscanf(p_parser->GetLine(),"  orientation { (%f %f %f %f\n",&Orientation.x,
												   &Orientation.y,
												   &Orientation.z,
												   &Orientation.w);

		}
		// Scale
		if(p_parser->FindWord("scale {"))
		{
              sscanf(p_parser->GetLine(),"  scale { %f\n",&Scale);
		}


		// namedsequence { }
		if(p_parser->FindWord("namedsequence {"))
		{
			p_parser->GetName(&SequenceName);
		}

		//  Roll
		if(p_parser->FindWord("roll { 0 }"))
		{
			EnableRoll = false;
		}


		// cast shadow?
		if(p_parser->FindWord("castshadow { }"))
		{
			CastShadow=true;
		}
		// receive shadow?
		if(p_parser->FindWord("receiveshadows { }"))
		{
			ReceiveShadow=true;
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

	}	// End of while inside the objectelement
	return true;
}
