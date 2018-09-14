#pragma once
//#include "IInstructionSet.h"

using namespace System;
using namespace System::IO;
using System::Collections::Generic::List;

namespace MIPSDisassembler 
{

[Serializable]
public ref class InstructionSerialized
{
public:
	String^ address;
	String^ name;
	List<String^>^ comments;
};

[Serializable]
public ref class DisassembleSerialized
{
public:
	DisassembleSerialized(void);
	List<DisassembledFileDetails^>^ binaryData;
	String^ disassemblyType;
	List<InstructionSerialized^>^ instructions;
	bool labelsOnly;
};

}