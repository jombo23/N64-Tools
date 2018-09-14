#pragma once

using namespace System;

namespace MIPSDisassembler
{

static public enum class RegisterNames { $zero, $at, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1,
	$t2, $t3, $t4, $t5, $t6, $t7, $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $t8, $t9, $k0, $k1,
	$gp, $sp, $s8, $ra};

static public enum class COP0Names { Index, Random, EntryLo0, EntryLo1, Context, Pagemask, Wired, Reserved7,
		BadVAddr, Count, EntryHi, Compare, Status, Cause, EPC, PRevID, Config, LLAddr, WatchLo,
		WatchHi, XContext, Reserved22, Reserved23, Reserved24, Reserved25, Reserved26, PErr, CacheErr,
		TagLo, TagHi, ErrorEPC };

static public enum class COP1Names { F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13,
	F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, F26, F27, F28,
	F29, F30, F31 };

static public enum class InstructionEntryType { Register, COP0Register, COP1Register, Immediate, Jump, Branch};

public ref class InstructionContent
{
public:
	InstructionEntryType EntryType;
	int Offset;
	int Size;

	InstructionContent(InstructionEntryType entryType, int offset, int size)
	{
		EntryType = entryType;
		Offset = offset;
		Size = size;
	}
};

static unsigned long CalculateBranch(int address, int branchValue) 
{
    return ((address + 4) + ((short int)(branchValue & 0xFFFF) * 4));
}

static String^ RegisterName(int regNum)
{
	return System::Enum::GetName(RegisterNames::typeid, (regNum & 0x1F));
}

static String^ COP0RegisterName(int regNum)
{
	return System::Enum::GetName(COP0Names::typeid, (regNum & 0x1F));
}

static String^ COP1RegisterName(int regNum)
{
	return System::Enum::GetName(COP1Names::typeid, (regNum & 0x1F));
}

}