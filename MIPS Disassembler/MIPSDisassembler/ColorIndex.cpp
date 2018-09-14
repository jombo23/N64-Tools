#include "StdAfx.h"
#include "ColorIndex.h"

namespace MIPSDisassembler
{

ColorIndex::ColorIndex(void)
{
}

ColorIndex::ColorIndex(int offset, int length, System::Drawing::Color colorValue)
{
	this->offset = offset;
	this->colorValue = colorValue;
	this->length = length;
}

int ColorIndex::Offset::get()
{
	return offset;
}

void ColorIndex::Offset::set(int value)
{
	offset = value;
}

int ColorIndex::Length::get()
{
	return length;
}

void ColorIndex::Length::set(int value)
{
	length = value;
}

System::Drawing::Color ColorIndex::ColorValue::get()
{
	return colorValue;
}

void ColorIndex::ColorValue::set(System::Drawing::Color value)
{
	colorValue = value;
}



}