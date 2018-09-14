#pragma once
#include "Instruction.h"
//#include "IInstructionSet.h"

using System::Collections::Generic::List;

namespace MIPSDisassembler
{

ref class DisassemblyContents
{
	private:
	float progress;
public:
	IInstructionSet^ instructionSet;
	DisassemblyContents(void);
	void Disassemble();
	void SetDisassemblyVariables(IInstructionSet^ instructionSet, List<DisassembledFileDetails^>^ binaryData, bool isNew);
public:
	array<Instruction^>^ instructions;
	List<DisassembledFileDetails^>^ binaryData;
	List<String^>^ setOfLines;
	IInstruction^ GetInstruction(unsigned long location);
	bool IsValidLocation(unsigned long location);
	int GetInstrOffset(unsigned long location);
	float GetProgress();
	bool isNew;
};

}