// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Scene::An8Scene(void)
{
	GroundGrid    = false;   // = true if the groundgrid is activated
	HasFog        = false;        // = 0 if no, = 1 if yes
	Znear         = 50.0f;  // Z-Near clip plane
	Zfar          = 10000.0f;   // Z-Far clip plane
	NbrObjectElement = 0; // Number of objects in the scene
	NbrLight      = 0;            // Number of lights in the scene
    NbrCamera     = 0;
	NbrFigureElement = 0;
	NbrNull = 0;
	BackGroundType = AN8_COLOR;
	BackGroundImage = "";
	HasLighting = false;
}

An8Scene::~An8Scene(void)
{

}


bool An8Scene::Load(AN8XPARSER*p_parser)
{

	p_parser->GetName(&Name);

	// GroundGrid///////////////////
	while(!p_parser->FindWord("groundgrid {"))
	{
		p_parser->ReadLine();
	}

	if(p_parser->FindWord("groundgrid { 0 }"))
      GroundGrid=false;
    else
      GroundGrid=true;

	// Background///////////////////
	while(!p_parser->FindWord("background {"))
	{
		p_parser->ReadLine();
	}
	sscanf(p_parser->GetLine(),"  background { %d %d %d\n",&BackGround[0],&BackGround[1],&BackGround[2]);



	while(!p_parser->FindWord("scene {") && p_parser->IndexOf(1, "}")!= 0 ) //(p_parser->Length()!=(1+LINERETURN)) )
	{
		//int test = p_parser->GetLine()find("}",0);
		int test = p_parser->IndexOf(1, "}");


		// BkImage
		if(p_parser->FindWord("image {"))
		{
			if( BackGroundType != AN8_PANORAMA )
				BackGroundType = AN8_IMAGE;

			if(!p_parser->FindWord("\""))
				p_parser->ReadLine();

			p_parser->GetName(&BackGroundImage);
		}

		// Bk Panorama
		if(p_parser->FindWord("panorama {"))
		{
			BackGroundType = AN8_PANORAMA;

			p_parser->CutLine("(",1);
			string element = p_parser->SubStr(""," ");
			BackGroundPanorama.Left = (float) atof( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr(""," ");
			BackGroundPanorama.Right = (float) atof( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr(""," ");
			BackGroundPanorama.Hight = (float) atof( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr("",")");
			BackGroundPanorama.Low = (float) atof( element.c_str());

		}

		// Bk Cubemap
		if(p_parser->FindWord("cubemap {"))
		{
			BackGroundType = AN8_CUBEMAP;

			if(!p_parser->FindWord("\""))
				p_parser->ReadLine();

			p_parser->GetName(&BackGroundImage);
		}

		// Fog
		if(p_parser->FindWord("fog {"))
		{
			HasFog = true;

			// color//
			p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"  color { %d %d %d\n",&FogColor[0],&FogColor[1],&FogColor[2]);

			// start-end//
			p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"  fogstart { %f\n",&FogStart);

			p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"  fogend { %f\n",&FogEnd);

			// percent//
			p_parser->ReadLine();
			if(p_parser->FindWord("percent {"))
			{
				sscanf(p_parser->GetLine(),"  percent { %d\n",&FogPercent);

			}

		}

		// Znear
		if(p_parser->FindWord("znear {"))
		{
			sscanf(p_parser->GetLine(),"  znear { %f\n",&Znear);

			p_parser->ReadLine();
			if(p_parser->FindWord("zfar {")){
				sscanf(p_parser->GetLine(),"  zfar { %f\n",&Zfar);
			}

		}

		if(p_parser->FindWord("lighting {"))
		{
			HasLighting = true;
			Lighting.Load(p_parser);
		}

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

	}

	return true;
}

void An8Scene::FindObjectID(An8File *aniFile)
{
	std::vector<An8ObjectElement*> objects = GetObjectElements();

	int i,i2;		  // Loop variable

	for( i = 0 ; i < (int)objects.size() ; ++i )
	{
		objects[i]->ObjectID = -1;
		for( i2 = 0 ; i2 < aniFile->NbrObject ; ++i2 )
		{
			if( objects[i]->SourceName.compare(aniFile->vObjects[i2].Name) == 0 )
				objects[i]->ObjectID = i2;
		}
	}
}

void An8Scene::SetElementsPointer( void )
{
	for( int i = 0 ; i < NbrObjectElement ; ++i )
		vObjectElements[i].SetElementsPointer(this,NULL);
	for( int i = 0 ; i < NbrFigureElement ; ++i )
		vFigureElements[i].SetElementsPointer(this,NULL);
	for( int i = 0 ; i < NbrCamera ; ++i )
		vCameras[i].SetElementsPointer(this,NULL);
	for( int i = 0 ; i < NbrLight ; ++i )
		vLights[i].SetElementsPointer(this,NULL);
	for( int i = 0 ; i < NbrNull ; ++i )
		vNulls[i].SetElementsPointer(this,NULL);
}


An8Element* An8Scene::GetTargetPointer(An8Element* o_ptr, std::string i_TargetName)
{
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		o_ptr = vObjectElements[i].GetTargetPointer(o_ptr,i_TargetName);
		if(o_ptr != NULL )
		{
			return o_ptr;
		}
	}
	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		o_ptr = vFigureElements[i].GetTargetPointer(o_ptr,i_TargetName);
		if(o_ptr != NULL )
		{
			return o_ptr;
		}
	}
	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		o_ptr = vCameras[i].GetTargetPointer(o_ptr,i_TargetName);
		if(o_ptr != NULL )
		{
			return o_ptr;
		}
	}
	for( int i = 0 ; i < NbrLight ; ++i )
	{
		o_ptr = vLights[i].GetTargetPointer(o_ptr,i_TargetName);
		if(o_ptr != NULL )
		{
			return o_ptr;
		}
	}
	for( int i = 0 ; i < NbrNull ; ++i )
	{
		o_ptr = vNulls[i].GetTargetPointer(o_ptr,i_TargetName);
		if(o_ptr != NULL )
		{
			return o_ptr;
		}
	}

	return NULL;
}





void An8Scene::ComputeAllMatrices( void )
{
	AN8XMATRIX identityMatrix;
	for( int i = 0 ; i < NbrLight ; ++i )
	{
		vLights[i].ComputeAllMatrices();
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		vCameras[i].ComputeAllMatrices();
	}

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		vObjectElements[i].ComputeAllMatrices();
	}  

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		vFigureElements[i].ComputeAllMatrices();
	} 

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		vNulls[i].ComputeAllMatrices();
	} 
}

std::vector<An8ObjectElement*> An8Scene::GetObjectElements(void)
{
	std::vector<An8ObjectElement*> objectsToReturn;

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		objectsToReturn.push_back(&vObjectElements[i]);
		std::vector<An8ObjectElement*> objects = vObjectElements[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vFigureElements[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vCameras[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vLights[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vNulls[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	return objectsToReturn;
}

std::vector<An8FigureElement*> An8Scene::GetFigureElements(void)
{
	std::vector<An8FigureElement*> figuresToReturn;

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8FigureElement*> figures = vObjectElements[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		figuresToReturn.push_back(&vFigureElements[i]);
		std::vector<An8FigureElement*> figures = vFigureElements[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8FigureElement*> figures = vCameras[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8FigureElement*> figures = vLights[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8FigureElement*> figures = vNulls[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	return figuresToReturn;
}

std::vector<An8Light*> An8Scene::GetLights(void)
{
	std::vector<An8Light*> lightsToReturn;

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Light*> lights = vObjectElements[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Light*> lights = vFigureElements[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8Light*> lights = vCameras[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		lightsToReturn.push_back(&vLights[i]);
		std::vector<An8Light*> lights = vLights[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8Light*> lights = vNulls[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	return lightsToReturn;
}

std::vector<An8Camera*> An8Scene::GetCameras(void)
{
	std::vector<An8Camera*> camerasToReturn;

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Camera*> cameras = vObjectElements[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Camera*> cameras = vFigureElements[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		camerasToReturn.push_back(&vCameras[i]);
		std::vector<An8Camera*> cameras = vCameras[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8Camera*> cameras = vLights[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8Camera*> cameras = vNulls[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	return camerasToReturn;
}



std::vector<An8Null*> An8Scene::GetNulls(void)
{
	std::vector<An8Null*> nullsToReturn;

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Null*> nulls = vObjectElements[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Null*> nulls = vFigureElements[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8Null*> nulls = vCameras[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8Null*> nulls = vLights[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		nullsToReturn.push_back(&vNulls[i]);
		std::vector<An8Null*> nulls = vNulls[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	return nullsToReturn;
}

