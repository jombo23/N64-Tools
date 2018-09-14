#pragma once

using namespace System;
using namespace System::Collections::Generic; 


namespace MIPSDisassembler
{

static public enum class InstructionType { None, Assign, Jump, JumpAndLink, Branch, BranchLikely, BranchAndLink, JumpRegister, JumpAndLinkRegister, DoubleArithmetic, Load, Store };

public interface class IInstructionDefinition
{
public:
	virtual String^ Parse(unsigned long address, unsigned long instruction, List<String^>^ Registers, unsigned long% target, array<Object^>^% instrDefs);
	virtual int GetCountInstrResults();
	virtual String^ GetName();
	virtual InstructionType GetInstructionType();
	
};

[Serializable]
public ref class DisassembledFileDetails
{
public:
	DisassembledFileDetails::DisassembledFileDetails(String^ filename, unsigned long startAddress, unsigned long binarySize, array<unsigned char>^ binary, unsigned long dataStart)
	{
		this->filename = filename;
		this->startAddress = startAddress;
		this->binarySize = binarySize;
		this->binary = binary;
		this->offsetInInstructions = 0;
		this->dataStart = dataStart;
	}
	String^ filename;
	unsigned long startAddress;
	unsigned long binarySize;
	array<unsigned char>^ binary;
	int offsetInInstructions;
	unsigned long dataStart;
};



public interface class IInstruction
{
public:
	virtual property String^ InstructionName 
	{ 
		String^ get(); 
	}
	virtual property String^ Name 
	{ 
		String^ get(); 
		void set(String^ value);
	}
	virtual property List<String^>^ Comment 
	{ 
		List<String^>^ get(); 
		void set(List<String^>^ value);
	}
	virtual property unsigned long InstructionRaw 
	{ 
		unsigned long get(); 
		void set(unsigned long value);
	}
	virtual property String^ InstructionText 
	{ 
		String^ get(); 
	}
	virtual property List<IInstruction^>^ Parents 
	{ 
		List<IInstruction^>^ get(); 
		void set(List<IInstruction^>^ value);
	}
	virtual property List<String^>^ Registers 
	{ 
		List<String^>^ get(); 
	}
	virtual property unsigned long Target 
	{ 
		unsigned long get(); 
		void set(unsigned long value);
	}
	virtual property unsigned long Address 
	{ 
		void set(unsigned long value);
		unsigned long get(); 
	}
	virtual property array<Object^>^ InstrDefResults
	{ 
		array<Object^>^ get(); 
	}
	virtual property InstructionType InstrType
	{
		InstructionType get(); 
	}
	virtual property bool IsFunction
	{
		bool get(); 
		void set(bool value);
	}
	virtual property List<IInstruction^>^ Exits
	{
		List<IInstruction^>^ get(); 
		void set(List<IInstruction^>^ value);
	}
	virtual property IInstruction^ IsEnd
	{
		IInstruction^ get(); 
		void set(IInstruction^ value);
	}
	virtual property IInstructionDefinition^ InstructionDefinition
	{
		IInstructionDefinition^ get(); 
		void set(IInstructionDefinition^ value);
	}
};

public interface class IColorIndexes
{
	public:
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
};

public interface class IInstructionSet
{
public:
	virtual void ParseInstruction(unsigned long startAddress, unsigned long binaryLength, unsigned long address, IInstruction^ instructionRef);
	virtual void PostProcess(List<DisassembledFileDetails^>^ disassembledFileDetails, array<IInstruction^>^ instructions, float% progress);
	virtual List<IColorIndexes^>^ Colorize(String^ currentSelection);
	virtual property unsigned long InstructionSize 
	{ 
		unsigned long get(); 
	}
	virtual property String^ InstructionSetName
	{
		String^ get();
	}
};

}