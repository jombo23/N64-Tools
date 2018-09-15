// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;


An8vpi2D::An8vpi2D(void)
{
	P1 = 0;
	P2 = 0;
}
An8vpi2D::An8vpi2D(int pt1,int pt2)
{
	P1 = pt1;
	P2 = pt2;
}
An8vpi2D::~An8vpi2D(void){}
