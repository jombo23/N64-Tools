#include "StdAfx.h"

#include "Instruction.h"

namespace MIPSDisassembler
{




Instruction::Instruction(void)
{
	address = 0xFFFFFFFF;
	name = nullptr;
	comment = nullptr;
	instructionRaw = 0x0;
	parents = nullptr;
	target = 0xFFFFFFFF;
	isFunction = false;
	exits = nullptr;
	isEnd = nullptr;
	instructionDefinition = nullptr;
	inputRegisters = nullptr;
}

Instruction::Instruction(unsigned long address, unsigned long instructionRaw, List<IInstruction^>^ parents)
{
	this->address = address;
	name = nullptr;
	comment = nullptr;
	this->instructionRaw = instructionRaw;
	this->parents = parents;
	target = 0xFFFFFFFF;
	isFunction = false;
	exits = nullptr;
	isEnd = nullptr;
	instructionDefinition = nullptr;
	inputRegisters = nullptr;
}


String^ Instruction::Name::get()
{
	return name;
}
void Instruction::Name::set(String^ value)
{
	name = value;
}

List<String^>^ Instruction::Comment::get()
{
	return comment;
}
void Instruction::Comment::set(List<String^>^ value)
{
	comment = value;
}

unsigned long Instruction::InstructionRaw::get()
{
	return instructionRaw;
}
void Instruction::InstructionRaw::set(unsigned long value)
{
	instructionRaw = value;
}

String^ Instruction::InstructionName::get()
{
	if (instructionDefinition != nullptr)
		return instructionDefinition->GetName();
	else
		return nullptr;
}

String^ Instruction::InstructionText::get()
{
	if (instructionDefinition != nullptr)
	{
		array<Object^>^ instrDefResults = gcnew array<Object^>(instructionDefinition->GetCountInstrResults());
		return instructionDefinition->Parse(address, instructionRaw, nullptr, target, instrDefResults);
	}
	else
		return "";
}

List<IInstruction^>^ Instruction::Parents::get()
{
	return parents;
}
void Instruction::Parents::set(List<IInstruction^>^ value)
{
	parents = value;
}

List<String^>^ Instruction::Registers::get()
{
	if (instructionDefinition != nullptr)
	{
		List<String^>^ registers = gcnew List<String^>();
		array<Object^>^ instrDefResults = gcnew array<Object^>(instructionDefinition->GetCountInstrResults());
		instructionDefinition->Parse(address, instructionRaw, registers, target, instrDefResults);
		return registers;
	}
	else
	{
		return nullptr;
	}
}

unsigned long Instruction::Target::get()
{
	return target;
}
void Instruction::Target::set(unsigned long value)
{
	target = value;
}

unsigned long Instruction::Address::get()
{
	return address;
}

void Instruction::Address::set(unsigned long value)
{
	address = value;
}


array<Object^>^ Instruction::InstrDefResults::get()
{
	if (instructionDefinition != nullptr)
	{
		array<Object^>^ instrDefResults = gcnew array<Object^>(instructionDefinition->GetCountInstrResults());
		instructionDefinition->Parse(address, instructionRaw, nullptr, target, instrDefResults);
		return instrDefResults;
	}
	else
		return nullptr;
}


InstructionType Instruction::InstrType::get()
{
	if (instructionDefinition != nullptr)
		return instructionDefinition->GetInstructionType();
	else
		return InstructionType::None;
}


bool Instruction::IsFunction::get()
{
	return isFunction;
}

void Instruction::IsFunction::set(bool value)
{
	isFunction = value;
}


List<IInstruction^>^ Instruction::Exits::get()
{
	return exits;
}

void Instruction::Exits::set(List<IInstruction^>^ value)
{
	exits = value;
}

IInstruction^ Instruction::IsEnd::get()
{
	return isEnd;
}

void Instruction::IsEnd::set(IInstruction^ value)
{
	isEnd = value;
}

IInstructionDefinition^ Instruction::InstructionDefinition::get()
{
	return instructionDefinition;
}

void Instruction::InstructionDefinition::set(IInstructionDefinition^ value)
{
	instructionDefinition = value;
}

List<String^>^ Instruction::InputRegisters::get()
{
	return inputRegisters;
}

void Instruction::InputRegisters::set( List<String^>^ value)
{
	inputRegisters = value;
}

}
