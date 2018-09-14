#pragma once
//#include "IInstructionSet.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace MIPSDisassembler
{

public ref class DisassemblerSharedFunctions
{
public:
	DisassemblerSharedFunctions(void);
	static IInstructionSet^ ActivateInstructionSet(String^ DLL, String^ _class);
};

}