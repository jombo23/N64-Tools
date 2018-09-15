// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8File::An8File(void)
{
	p_parser = NULL;

	//IsEmpty    = true;
    //HasNormal  = false;

	NbrScene   = 0;
	NbrTexture = 0;
	NbrObject  = 0;
	NbrFigure  = 0;
	NbrSequence = 0;
	_IsExternalParser = false;
	HasDescription = false;

}

An8File::~An8File(void)
{
	if(p_parser!=NULL && _IsExternalParser == false )
	{
		delete p_parser;
		p_parser = NULL;
	}
	vTextures.clear();
	vObjects.clear();
	vScenes.clear();
}

bool An8File::LoadFile(string file_name)
{
	FileName=file_name;

	if(p_parser == NULL)
	{
		// Allows developpers to use an another AN8XPARSER
		// Using SetParser()
		p_parser = new AN8XPARSER;
	}

	if( p_parser->IsOpen == false )
	{
		// If a developper as already open the source
		// with its own AN8XPARSER
		if( p_parser->Open(file_name) == false )
		{
			delete p_parser;
			p_parser = NULL;
			return false;
		}
	}
/*
	// Header
	if( Header.Load(p_parser) == false)
	{
		delete p_parser;
		p_parser = NULL;
		return false;
	}

	// Environement
	if( Environment.Load(p_parser) == false)
	{
		delete p_parser;
		p_parser = NULL;
		return false;
	}
*/
	p_parser->ReadLine();

	do
	{
		if(p_parser->FindWord("header {"))
		{
			Header.Load(p_parser);
		} // End of if header 
		if(p_parser->FindWord("environment {"))
		{
			Environment.Load(p_parser);
		} // End of if Environment 
		if(p_parser->FindWord("description {"))
		{
			HasDescription = true;
			Description.Load(p_parser);
		} // End of if description
		if(p_parser->FindWord("texture {"))
		{
			NbrTexture+=1;
			An8Texture newTexture;
			vTextures.push_back(newTexture);
			if( vTextures[vTextures.size()-1].Load(p_parser) == false )
			{
				delete p_parser;
				p_parser = NULL;
				return false;
			}
		} // End of if texture
		else if(p_parser->FindWord("object {"))
		{
			NbrObject+=1;
			An8Object newObject;
			vObjects.push_back(newObject);
			if( vObjects[vObjects.size()-1].Load(p_parser ) == false )
			{
				delete p_parser;
				p_parser = NULL;
				return false;
			}	
		}
		else if(p_parser->FindWord("figure {") )
		{
			NbrFigure+=1;
			An8Figure newFigure;
			vFigures.push_back(newFigure);
			if( vFigures[vFigures.size()-1].Load(p_parser ) == false )
			{
				delete p_parser;
				p_parser = NULL;
				return false;
			}
		}
		else if(p_parser->FindWord("sequence {") )
		{
			NbrSequence+=1;
			An8Sequence newSequence;
			vSequences.push_back(newSequence);
			if( vSequences[vSequences.size()-1].Load(p_parser ) == false )
			{
				delete p_parser;
				p_parser = NULL;
				return false;
			}	
		}
		else if(p_parser->FindWord("scene {") )
		{
			NbrScene+=1;
			An8Scene newScene;
			vScenes.push_back(newScene);
			if( vScenes[vScenes.size()-1].Load(p_parser ) == false )
			{
				delete p_parser;
				p_parser = NULL;
				return false;
			}
			vScenes[vScenes.size()-1].FindObjectID(this);
		}
	}while( p_parser->ReadLine()==1);

	p_parser->Close();

	SetElementsPointer();
	//ComputeLooKAt();

	delete p_parser;
	p_parser = NULL;


	return true;

}

void An8File::SetParser( AN8XPARSER* i_parser )
{
	p_parser = i_parser;
	_IsExternalParser = true;
}

bool An8File::Load( void )
{
	if(p_parser == NULL)
	{
		return false;
	}

	// Header
	Header.Load(p_parser);

	// Environement
	Environment.Load(p_parser);

	p_parser->ReadLine();

	while(!p_parser->FindWord("object {"))
	{
		if(p_parser->FindWord("texture {"))
		{
			NbrTexture+=1;
			An8Texture newTexture;
			newTexture.Load(p_parser);
			 vTextures.push_back(newTexture);
		} // End of if texture
		p_parser->ReadLine();
	} // End of while no object

	do
	{
		if(p_parser->FindWord("object {"))
		{
			NbrObject+=1;
			An8Object newObject;
			newObject.Load(p_parser );
			vObjects.push_back(newObject);
		}
		else if(p_parser->FindWord("figure {") )
		{
			NbrFigure+=1;
			An8Figure newFigure;
			newFigure.Load(p_parser );
			vFigures.push_back(newFigure);
		}
		else if(p_parser->FindWord("sequence {") )
		{
			NbrSequence+=1;
			An8Sequence newSequence;
			newSequence.Load(p_parser );
			vSequences.push_back(newSequence);
		}
		else if(p_parser->FindWord("scene {") )
		{
			NbrScene+=1;
			An8Scene newScene;
			newScene.Load(p_parser );
			newScene.FindObjectID(this);
			vScenes.push_back(newScene);
		}
	}while(p_parser->ReadLine()!=0);

	SetElementsPointer();

	return true;
}

AN8XPARSER* An8File::GetParser(void)
{
	return p_parser;
}



void An8File::Ungroup( void )
{
	for( int i = 0 ; i < NbrObject ; ++i )
	{
		vObjects[i].Ungroup();
	}
}

void An8File::Triangulize( void )
{
	for( int i = 0 ; i < NbrObject ; ++i )
	{
		vObjects[i].Triangulize();
	}
}

void An8File::ComputeFaceNormal(void)
{
	for( int i = 0 ; i < NbrObject ; ++i )
	{
		vObjects[i].ComputeFaceNormal();
	}
}

//////////////////////////////////////////////////////////
//	Compute camera target								//
//////////////////////////////////////////////////////////
/*
void An8File::ComputeLooKAt(void)
{
	for( int i = 0 ; i < NbrScene ; ++i )
	{
		vScenes[i].ComputeLooKAt();
	}
}
*/


void An8File::ComputeAllPositions(void)
{
	for(int i = 0 ; i < NbrObject ; ++i)
	{
        vObjects[i].ComputeAllPositions();
	}
}

void An8File::SetElementsPointer( void )
{
	for(int i = 0 ; i < NbrScene ; ++i)
	{
		vScenes[i].SetElementsPointer();
	}
}

void An8File::ComputeAllMatrices( void )
{
	for(int i = 0 ; i < NbrObject ; ++i)
	{
		vObjects[i].ComputeAllMatrices(true);
	}
	for(int i = 0 ; i < NbrScene ; ++i)
	{
		vScenes[i].ComputeAllMatrices();
	}
}

int An8File::GetNbrMesh(void)
{
	int nbrMesh = 0;

	for(int i = 0 ; i < NbrObject ; ++i)
	{
		nbrMesh += vObjects[i].GetNbrMesh();
	}
	return nbrMesh;
}



void An8File::ComputeTextureIDForMaterial(void)
{
	for( int i = 0 ; i < NbrObject ; ++i )
	{
		for( int m = 0; m < vObjects[i].NbrMaterial ; ++m )
		{
			for( int t = 0 ; t < NbrTexture ; ++t )
			{
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].DiffuseTexName)== 0 )
					vObjects[i].vMaterials[m].DiffuseTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].AmbiantTexName)== 0 )
					vObjects[i].vMaterials[m].AmbiantTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BumpMapTexName)== 0 )
					vObjects[i].vMaterials[m].BumpMapTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].EmissiveTexName)== 0 )
					vObjects[i].vMaterials[m].EmissiveTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].EnvironmentTexName)== 0 )
					vObjects[i].vMaterials[m].EnvironmentTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].SpecularTexName)== 0 )
					vObjects[i].vMaterials[m].SpecularTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].TransparencyTexName)== 0 )
					vObjects[i].vMaterials[m].TransparencyTextureID = t;

				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackDiffuseTexName)!= -1 )
					vObjects[i].vMaterials[m].BackDiffuseTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackAmbiantTexName)!= -1 )
					vObjects[i].vMaterials[m].BackAmbiantTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackBumpMapTexName)!= -1 )
					vObjects[i].vMaterials[m].BackBumpMapTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackEmissiveTexName)!= -1 )
					vObjects[i].vMaterials[m].BackEmissiveTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackEnvironmentTexName)!= -1 )
					vObjects[i].vMaterials[m].BackEnvironmentTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackSpecularTexName)!= -1 )
					vObjects[i].vMaterials[m].BackSpecularTextureID = t;
				if( this->vTextures[t].Name.compare(vObjects[i].vMaterials[m].BackTransparencyTexName)== 0 )
					vObjects[i].vMaterials[m].BackTransparencyTextureID = t;
			}
		}
	}
}

bool An8File::HasNormal(void)
{

	bool result = false;

	for( int i = 0 ; i < NbrObject ; ++i)
	{
		if( vObjects[i].HasNormal() == true )
		{
			result = true;
			break;
		}
	}
	return result;
}

void An8File::ConvertAllToMesh( void )
{
	for( int i = 0 ; i < NbrObject ; ++i)
	{
		vObjects[i].ConvertAllToMesh();
	}
}

int An8File::GetFigureID(std::string name)
{
	for(int fig = 0; fig < (int)this->vFigures.size();++fig )
	{
		if( name.compare( this->vFigures[fig].Name) ==0)
		{
			return fig;
		}
	}
	return -1;
}

int An8File::GetObjectID(std::string name)
{
	for(int i = 0; i < (int)this->vObjects.size();++i )
	{
		if( name.compare( this->vObjects[i].Name) ==0)
		{
			return i;
		}
	}
	return -1;
}

int An8File::GetSequenceID(std::string name)
{
	for(int i = 0; i < (int)this->vSequences.size();++i )
	{
		if( name.compare( this->vSequences[i].Name) ==0)
		{
			return i;
		}
	}
	return -1;
}



