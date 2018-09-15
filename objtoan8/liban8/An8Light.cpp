// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Light::An8Light(void)
{
	Color[0] = 255;
	Color[1] = 255;
	Color[2] = 255;

	Kind = 0;  // 1=infinite,2=local,3=spot, 0=UFO :)

	InRadius = 0.0f; // Local,spotlight
	OutRadius = 0.0f; // Local,spotlight

	InAngle = 0.0f; //spotlight
    OutAngle = 0.0f; //spotlight

    CastShadow =  false; // yes/no
	PercentShadow = 0;

    RaytraceShadow = false;
    Soft = false;
    SoftSize = 0.0f;
    MinSamples = 0;
    MaxSamples = 0;
    MonteCarlo = false;

	EnableRoll = true;

}

An8Light::~An8Light(void)
{
}

bool An8Light::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"light");
	if(elementStart<0) return false;

	// Name
	p_parser->GetName(&Name);
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

            // Color
            if(p_parser->FindWord("color {"))
			{
              sscanf(p_parser->GetLine(),"  color { (%d %d %d\n",&Color[0],
												   &Color[1],
												   &Color[2]);
			}
			// Kind
            if(p_parser->FindWord("infinite { }"))
              Kind=1;	//infinite
			if(p_parser->FindWord("local { }"))
              Kind=2;	// local
			if(p_parser->FindWord("spotlight { }"))
              Kind=3;	//spotlight

   	         //inradius
			 if(p_parser->FindWord("inradius {"))
			 {
				sscanf(p_parser->GetLine(),"  inradius { %f\n",&InRadius);
             }

			  //outradius
			  if(p_parser->FindWord("outradius {"))
		      {
				 sscanf(p_parser->GetLine(),"  outradius { %f\n",&OutRadius);
	          }

              //InAngle
			  if(p_parser->FindWord("inangle {"))
			  {
				sscanf(p_parser->GetLine(),"  inangle { %f\n",&InAngle);
              }

              //outangle
              if(p_parser->FindWord("outangle {"))
              {
                 sscanf(p_parser->GetLine(),"  outangle { %f\n",&OutAngle);
		      }

			  //  Roll
			if(p_parser->FindWord("roll { 0 }"))
			{
			EnableRoll=false;
			}

              // castshadow
              if( (p_parser->FindWord("castshadow { }")) )
              {
			     CastShadow=true;
	          }

              //  percentshadow {
              if( (p_parser->FindWord("percentshadow {")) )
              {
                 sscanf(p_parser->GetLine(),"  percentshadow { %d\n",&PercentShadow);
              }


		// raytraceshadow
		if( (p_parser->FindWord("raytraceshadow { }")) )
		{
			RaytraceShadow=true;
		}

		// soft
		if( (p_parser->FindWord("soft { }")) )
		{
			Soft=true;
		}

		// softsize {
		if( (p_parser->FindWord("softsize {")) )
		{
			sscanf(p_parser->GetLine(),"  softsize { %f\n",&SoftSize);
		}

		// minsamples
		if( (p_parser->FindWord("minsamples {")) )
		{
			sscanf(p_parser->GetLine(),"  minsamples { %d\n",&MinSamples);
		}

		// maxsamples
		if( (p_parser->FindWord("maxsamples {")) )
		{
			sscanf(p_parser->GetLine(),"  maxsamples { %d\n",&MaxSamples);
		}

		// montecarlo
		if( (p_parser->FindWord("montecarlo { }")) )
		{
			MonteCarlo=true;
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

	}  // End of While no end of light
	return true;
}


