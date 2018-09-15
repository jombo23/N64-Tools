// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8FaceIndice::An8FaceIndice(void)
{
	Point = 0;    // Vertex
	Normal = 0;   // Normal
	TexCoord = 0; // Texture Coordinate
}

An8FaceIndice::An8FaceIndice(int point,int normal,int texCoord)
{
	this->Point = point;
	this->Normal = normal;
	this->TexCoord = texCoord;
}

An8FaceIndice::~An8FaceIndice(void)
{

}
