#include "StdAfx.h"
#include "DisassemblerSharedFunctions.h"
// TODO REMOVE THIS AND PUT BACK IN DLL
//#include "MIPS4300.h"

namespace MIPSDisassembler
{

DisassemblerSharedFunctions::DisassemblerSharedFunctions(void)
{
}

IInstructionSet^ DisassemblerSharedFunctions::ActivateInstructionSet(String^ DLL, String^ _class)
{
	try
	{
		//Assembly::Load(DLL);
		System::Runtime::Remoting::ObjectHandle^ handler = System::Activator::CreateInstance(DLL, _class);
		
		IInstructionSet^ instructionSet = (IInstructionSet^)handler->Unwrap();
		if (instructionSet == nullptr)
		{
			MessageBox::Show("Error processing " + DLL + "," + _class, "Error loading");
			return nullptr;
		}
		else
		{
			return instructionSet;
		}
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->ToString(), "Error processing " + DLL + "," + _class);
		return nullptr;
	}

}

}