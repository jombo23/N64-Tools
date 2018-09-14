#pragma once

using System::Drawing::Color;

namespace MIPSDisassembler
{

public ref class ColorIndex : public IColorIndexes
{
public:
	ColorIndex(void);
	ColorIndex(int offset, int length, System::Drawing::Color colorValue);

	virtual property int Offset
	{ 
		int get(); 
		void set(int value);
	}
	virtual property int Length
	{ 
		int get(); 
		void set(int value);
	}
	virtual property System::Drawing::Color ColorValue
	{ 
		void set(System::Drawing::Color value);
		System::Drawing::Color get(); 
	}
	int offset;
	int length;
	System::Drawing::Color colorValue;
};

}