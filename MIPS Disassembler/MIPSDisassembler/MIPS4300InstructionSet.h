#pragma once
#include "MIPS4300Shared.h"
//#include "IInstructionSet.h"

using System::String;
using System::Enum;
using System::Collections::Generic::List;

namespace MIPSDisassembler
{



public ref class InstructionDefinition : public IInstructionDefinition
{
public:
	String^ Name;
	unsigned long Mask;
	unsigned long Opcode;
	List<InstructionContent^>^ InstructionDefinitions;
	String^ FormatString;
	InstructionType InstrType;

	InstructionDefinition(String^ name, unsigned long mask, unsigned long opcode, List<InstructionContent^>^ instructionDefinitions, String^ formatString, InstructionType instrType)
	{
		Name = name;
		Mask =  mask;
		Opcode = opcode;
		InstructionDefinitions = instructionDefinitions;
		FormatString = formatString;
		InstrType = instrType;
	}
	
	virtual String^ InstructionDefinition::GetName()
	{
		return Name;
	}

	virtual int InstructionDefinition::GetCountInstrResults()
	{
		if (InstructionDefinitions != nullptr)
			return InstructionDefinitions->Count;
		else
			return 0;
	}

	virtual InstructionType InstructionDefinition::GetInstructionType()
	{
		if (InstructionDefinitions != nullptr)
			return InstrType;
		else
			return InstructionType::None;
	}


	virtual String^ InstructionDefinition::Parse(unsigned long address, unsigned long instruction, List<String^>^ Registers, unsigned long% target, array<Object^>^% instrDefs)
	{
		System::Text::StringBuilder strBld = Name;
		for (int x = 0; x < InstructionDefinitions->Count; x++)
		{
			unsigned long data = MIPSDisassembler::SharedFunctions::PullFromInstruction(instruction, InstructionDefinitions[x]->Offset, InstructionDefinitions[x]->Size);
			if (InstructionDefinitions[x]->EntryType == InstructionEntryType::Register)
			{
				instrDefs[x] = RegisterName(data);
				if (Registers != nullptr)
					Registers->Add(RegisterName(data));
			}
			else if (InstructionDefinitions[x]->EntryType == InstructionEntryType::COP0Register)
			{
				instrDefs[x] = COP0RegisterName(data);
				if (Registers != nullptr)
					Registers->Add(COP0RegisterName(data));
			}
			else if (InstructionDefinitions[x]->EntryType == InstructionEntryType::COP1Register)
			{
				instrDefs[x] = COP1RegisterName(data);
				if (Registers != nullptr)
					Registers->Add(COP1RegisterName(data));
			}
			else if (InstructionDefinitions[x]->EntryType == InstructionEntryType::Immediate)
			{
				instrDefs[x] = data;
			}
			else if (InstructionDefinitions[x]->EntryType == InstructionEntryType::Jump)
			{
				target = (data * 4);
				target = (address & 0xF0000000) | target;
				instrDefs[x] = target;
			}
			else if (InstructionDefinitions[x]->EntryType == InstructionEntryType::Branch)
			{
				target = CalculateBranch(address, data);
				instrDefs[x] = target;
			}
		}

		if (instrDefs->GetLength(0) == 0)
			return String::Format(FormatString, Name);
		else if (instrDefs->GetLength(0) == 1)
			return String::Format(FormatString, Name, instrDefs[0]);
		else if (instrDefs->GetLength(0) == 2)
			return String::Format(FormatString, Name, instrDefs[0], instrDefs[1]);
		else if (instrDefs->GetLength(0) == 3)
			return String::Format(FormatString, Name, instrDefs[0], instrDefs[1], instrDefs[2]);
		else if (instrDefs->GetLength(0) == 4)
			return String::Format(FormatString, Name, instrDefs[0], instrDefs[1], instrDefs[2], instrDefs[3]);
	}
};

}