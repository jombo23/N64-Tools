// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Texture::An8Texture(void)
{
	Invert=0;
	IsCubeMap=false;
}

An8Texture::~An8Texture(void)
{

}

/////////////////////////////////////////
// Clear the path of texture file
// Ex: c:\textures\test.jpg => test.jpg
/////////////////////////////////////////
void An8Texture::ClearTexturePath(void)
{

	if( FileName.find("\\",0) != std::string::npos )
	{
		int last = (int)FileName.find_last_of("\\");
		FileName = FileName.substr(last+1,FileName.length()-last-1);
	}


	for(int i=0;i<6;++i)
	{
		if( CubeMap[i].find("\\",0) != std::string::npos )
	{
		int last = (int)CubeMap[i].find_last_of("\\");
		CubeMap[i] = CubeMap[i].substr(last+1,(int)CubeMap[i].length()-last-1);
	}
	}
}

bool An8Texture::Load(AN8XPARSER*p_parser)
{
	p_parser->GetName(&Name);
	p_parser->ReadLine();

	if(p_parser->FindWord("invert {")) // Invert image(s)
	{
		Invert=1;
		p_parser->ReadLine();
	}
	if(p_parser->FindWord("cubemap {")) // CubeMap
	{
		IsCubeMap = true ;
		p_parser->ReadLine();
		for(int i=0;i<6;++i)
		{
			while(!p_parser->FindWord("file {"))
			{
				p_parser->ReadLine();
			}
            if(!p_parser->FindWord("\""))   // File Name is in next line
            {
				p_parser->ReadLine();
				if(p_parser->FindWord("\\"))
				{
					p_parser->GetName(&CubeMap[i]);
				}
				else
				{
					CubeMap[i] = "";
				}
			}
            else
            {
				if(p_parser->FindWord("\\"))
				{
					p_parser->GetName(&CubeMap[i]);
				}
				else
				{
					CubeMap[i] = "";
				}
            }
           p_parser->ReadLine();
		} // End of for(i=0;i<6;++i)
	}
	else // No CubeMap
	{
		if(!p_parser->FindWord("\""))   // File Name is in next line
		{
           p_parser->ReadLine();
           p_parser->GetName(&FileName);
		}
		else
		{
           p_parser->GetName(&FileName);
		}
	} // End of no cubemap

	return true;
}
