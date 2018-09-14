#pragma once
#include "DisassemblerConfiguration.h"

#using <System.Xml.dll>
#using <System.dll>

using namespace System;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::IO;

namespace MIPSDisassembler
{

public ref class ReadXMLConfiguration
{
public:
	static disassembler^ ReadDisassembler(String^ filename);
};

};