#include "StdAfx.h"
//#include "IInstructionSet.h"
#include "ReadXMLConfiguration.h"
#include "DisassemblerConfiguration.h"
#include "DisassemblerSharedFunctions.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace MIPSDisassembler
{

disassembler^ ReadXMLConfiguration::ReadDisassembler(String^ filename)
{
	disassembler^ configFile;
	XmlSerializer^ serializer = gcnew XmlSerializer( disassembler::typeid );
	FileStream^ fs = gcnew FileStream( filename,FileMode::Open );

	configFile = dynamic_cast<disassembler^>(serializer->Deserialize( fs ));

	// test they work, remove those that don't
	if ((configFile->tab != nullptr) && (configFile->processor != nullptr))
	{
		List<MIPSDisassembler::disassembler::processorRow^> rowsToRemove = gcnew List<MIPSDisassembler::disassembler::processorRow^>();

		for (int x = 0; x < configFile->tab->Count; x++)
		{
			for each (MIPSDisassembler::disassembler::processorRow^  procRow in configFile->tab[x]->GetprocessorRows())
			{
				if ((DisassemblerSharedFunctions::ActivateInstructionSet(procRow->DLL, procRow->_class)) == nullptr)
					rowsToRemove.Add(procRow);
			}
		}

		for each (MIPSDisassembler::disassembler::processorRow^  procRow in rowsToRemove)
		{
			configFile->processor->Rows->Remove(procRow);
		}
	}
	else
	{
		MessageBox::Show("Error no disassembler processors present", "Error");
	}


	return configFile;

}

}