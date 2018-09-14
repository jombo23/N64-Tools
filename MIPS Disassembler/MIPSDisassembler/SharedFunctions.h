#pragma once

using System::String;

namespace MIPSDisassembler 
{

public ref class SharedFunctions
{
public:
	SharedFunctions(void);
	static unsigned long ByteArrayToChar(array<unsigned char>^ binary, unsigned long offset);
	static array<unsigned long>^ LongBitMasks = gcnew array<unsigned long>(33) { 
		0x0,
		0x1,        0x3,        0x7,        0xF,
		0x1F,       0x3F,       0x7F,       0xFF,
		0x1FF,      0x3FF,      0x7FF,      0xFFF,
		0x1FFF,     0x3FFF,     0x7FFF,     0xFFFF,
		0x1FFFF,    0x3FFFF,    0x7FFFF,    0xFFFFF,
		0x1FFFFF,   0x3FFFFF,   0x7FFFFF,   0xFFFFFF,
		0x1FFFFFF,  0x3FFFFFF,  0x7FFFFFF,  0xFFFFFFF,
		0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
	};
	static unsigned long PullFromInstruction(unsigned long instruction, int offset, int bitCount);
	static int MeasureDisplayStringWidth(System::Drawing::Graphics^ graphics, String^ text, System::Drawing::Font font);
};

}