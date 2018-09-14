#pragma once

#include "MIPS4300Shared.h"
#include "MIPS4300InstructionSet.h"
//#include "IInstructionSet.h"

using System::Collections::Generic::List;


namespace MIPSDisassembler
{



public ref class MIPS4300 : public MIPSDisassembler::IInstructionSet
{
public:
	MIPS4300(void);

	// IInstructionSet
	virtual property unsigned long InstructionSize { unsigned long get(); }
	virtual property String^ InstructionSetName { String^ get(); }
	virtual void ParseInstruction(unsigned long startAddress, unsigned long binaryLength, unsigned long address, MIPSDisassembler::IInstruction^ instructionRef);
	virtual void PostProcess(List<DisassembledFileDetails^>^ disassembledFileDetails, array<IInstruction^>^ instructions, float% progress, bool isNew);
	virtual List<IColorIndexes^>^ Colorize(String^ currentSelection);

private:
	// Instructions
	List<InstructionDefinition^>^ instructionDefinition;
	void InitializeInstructionDefinition();
	List<String^>^ loadToFirstRegInstructions;
	void AddExits(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, IInstruction^ instructionStart, unsigned long address, List<IInstruction^>^ previousChecks);
	int GetOffset(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location);

	IInstruction^ GetInstruction(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location);
	bool IsValidLocation(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location);
	bool IsASCII(unsigned long hex);
};

}