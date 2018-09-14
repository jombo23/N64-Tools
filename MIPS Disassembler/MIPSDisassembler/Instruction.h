#pragma once
//#include "IInstructionSet.h"

using System::String;
using System::Collections::Generic::List;


namespace MIPSDisassembler
{



public ref class Instruction : public IInstruction
{
public:
	Instruction(void);
	Instruction(unsigned long address, unsigned long instructionRaw, List<IInstruction^>^ parents);

private:
	String^ name;
	List<String^>^ comment;
	unsigned long instructionRaw;
	List<IInstruction^>^ parents;
	unsigned long target;
	unsigned long address;
	bool isFunction;
	List<IInstruction^>^ exits;
	IInstruction^ isEnd;
	IInstructionDefinition^ instructionDefinition;
	List<String^>^ inputRegisters;

public:
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
	virtual property List<String^>^ InputRegisters 
	{ 
		List<String^>^ get(); 
		void set(List<String^>^ value);
	}
	virtual property unsigned long Target 
	{ 
		unsigned long get(); 
		void set(unsigned long value);
	}
	virtual property unsigned long Address 
	{ 
		unsigned long get(); 
		void set(unsigned long value);
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
	virtual property String^ InstructionName 
	{ 
		String^ get(); 
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

}