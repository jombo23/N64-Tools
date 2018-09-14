#include "StdAfx.h"
#include "SharedFunctions.h"


using namespace System;

namespace MIPSDisassembler 
{

SharedFunctions::SharedFunctions(void)
{
	
}


unsigned long SharedFunctions::ByteArrayToChar(array<unsigned char>^ binary, unsigned long offset)
{
	unsigned long returnValue = ((((((binary[offset] << 8) | binary[offset+1]) << 8) | binary[offset+2]) << 8) | binary[offset+3]);
	return returnValue;
}

unsigned long SharedFunctions::PullFromInstruction(unsigned long instruction, int offset, int bitCount)
{
	return ((instruction >> offset) & LongBitMasks[bitCount]);
}

int SharedFunctions::MeasureDisplayStringWidth(System::Drawing::Graphics^ graphics, String^ text, System::Drawing::Font font)
		{
			/*System::Drawing::StringFormat^ format  = gcnew System::Drawing::StringFormat ();
			System::Drawing::RectangleF^   rect    = gcnew System::Drawing::RectangleF(0, 0,
																		  1000, 1000);
			array<System::Drawing::CharacterRange^>^ ranges  = gcnew array<System::Drawing::CharacterRange^>(1);
			ranges[0] = gcnew System::Drawing::CharacterRange(0, text->Length);

			array<System::Drawing::Region^>^         regions = gcnew array<System::Drawing::Region^>(1);

			format->SetMeasurableCharacterRanges (ranges);

			regions = graphics->MeasureCharacterRanges (text, font, rect, format);
			rect    = regions[0]->GetBounds (graphics);

			return (int)(rect->Right + 1.0f);*/
			return 0;
		}


}