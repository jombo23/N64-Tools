#include "StdAfx.h"
#include "MIPS4300.h"


using System::Collections::Generic::List;

namespace MIPSDisassembler
{


MIPS4300::MIPS4300(void)
{
	InitializeInstructionDefinition();
	loadToFirstRegInstructions = gcnew List<String^>();
	loadToFirstRegInstructions->Add("CFC1");
	loadToFirstRegInstructions->Add("MFC1");
	loadToFirstRegInstructions->Add("DMFC1");
	loadToFirstRegInstructions->Add("MFC0");
	loadToFirstRegInstructions->Add("DSRA32");
	loadToFirstRegInstructions->Add("DSRL32");
	loadToFirstRegInstructions->Add("DSLL32");
	loadToFirstRegInstructions->Add("DSRA");
	loadToFirstRegInstructions->Add("DSRL");
	loadToFirstRegInstructions->Add("DSLL");
	loadToFirstRegInstructions->Add("SRA");
	loadToFirstRegInstructions->Add("SRL");
	loadToFirstRegInstructions->Add("SLL");
	loadToFirstRegInstructions->Add("LUI");
	loadToFirstRegInstructions->Add("DSUBU");
	loadToFirstRegInstructions->Add("DSUB");
	loadToFirstRegInstructions->Add("DADDU");
	loadToFirstRegInstructions->Add("DADD");
	loadToFirstRegInstructions->Add("AND");
	loadToFirstRegInstructions->Add("ADDU");
	loadToFirstRegInstructions->Add("ADD");
	loadToFirstRegInstructions->Add("XOR");
	loadToFirstRegInstructions->Add("OR");
	loadToFirstRegInstructions->Add("NOR");
	loadToFirstRegInstructions->Add("DSRAV");
	loadToFirstRegInstructions->Add("DSRLV");
	loadToFirstRegInstructions->Add("DSRLV");
	loadToFirstRegInstructions->Add("SRAV");
	loadToFirstRegInstructions->Add("SRAV");
	loadToFirstRegInstructions->Add("SRLV");
	loadToFirstRegInstructions->Add("SLLV");
	loadToFirstRegInstructions->Add("SLT");
	loadToFirstRegInstructions->Add("SLTU");
	loadToFirstRegInstructions->Add("SUB");
	loadToFirstRegInstructions->Add("SUBU");
	loadToFirstRegInstructions->Add("MFHI");
	loadToFirstRegInstructions->Add("MFLO");
	loadToFirstRegInstructions->Add("LD");
	loadToFirstRegInstructions->Add("LWU");
	loadToFirstRegInstructions->Add("LWR");
	loadToFirstRegInstructions->Add("LHU");
	loadToFirstRegInstructions->Add("LBU");
	loadToFirstRegInstructions->Add("LW");
	loadToFirstRegInstructions->Add("LWL");
	loadToFirstRegInstructions->Add("LH");
	loadToFirstRegInstructions->Add("LB");
	loadToFirstRegInstructions->Add("LL");
	loadToFirstRegInstructions->Add("LLD");
	loadToFirstRegInstructions->Add("LDL");
	loadToFirstRegInstructions->Add("LDR");
	loadToFirstRegInstructions->Add("DADDIU");
	loadToFirstRegInstructions->Add("DADDI");
	loadToFirstRegInstructions->Add("ANDI");
	loadToFirstRegInstructions->Add("ORI");
	loadToFirstRegInstructions->Add("XORI");
	loadToFirstRegInstructions->Add("SLTIU");
	loadToFirstRegInstructions->Add("SLTI");
	loadToFirstRegInstructions->Add("ADDIU");
	loadToFirstRegInstructions->Add("ADDI");

}

IInstruction^ MIPS4300::GetInstruction(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location)
{
	for (int x = 0; x < binaryData->Count; x++)
	{
		if ((location >= binaryData[x]->startAddress) && (location < (binaryData[x]->startAddress + binaryData[x]->binarySize)))
		{
			int instrOffset = binaryData[x]->offsetInInstructions + ((location - binaryData[x]->startAddress) / 4);
			return instructions[instrOffset];
		}
	}
}

bool MIPS4300::IsValidLocation(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location)
{
	for (int x = 0; x < binaryData->Count; x++)
	{
		if ((location >= binaryData[x]->startAddress) && (location < (binaryData[x]->startAddress + binaryData[x]->binarySize)))
		{
			return true;
		}
	}
	return false;
}

int MIPS4300::GetOffset(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, unsigned long location)
{
	for (int x = 0; x < binaryData->Count; x++)
	{
		if ((location >= binaryData[x]->startAddress) && (location < (binaryData[x]->startAddress + binaryData[x]->binarySize)))
		{
			return binaryData[x]->offsetInInstructions + ((location - binaryData[x]->startAddress) / 4);
		}
	}
}

void MIPS4300::AddExits(List<DisassembledFileDetails^>^ binaryData, array<IInstruction^>^ instructions, IInstruction^ instructionStart, unsigned long address, List<IInstruction^>^ previousChecks)
{
	int offset = GetOffset(binaryData, instructions, address);
	// find exits
	for (int y = offset; y < instructions->Length; y++)
	{
		if ((y != 0) && (instructions[y]->Address != (instructions[y-1]->Address + 4)))
			break;

		if ((instructions[y]->Target != 0xFFFFFFFF) && 
			(
			(instructions[y]->InstrType == InstructionType::Branch)
			||
			(instructions[y]->InstrType == InstructionType::BranchLikely)
			||
			(instructions[y]->InstrType == InstructionType::Jump)
			)
			)
		{
			if (IsValidLocation(binaryData, instructions, instructions[y]->Target))
			{
				IInstruction^ branchedInstruction = GetInstruction(binaryData, instructions, instructions[y]->Target);
				if (!previousChecks->Contains(branchedInstruction))
				{
					previousChecks->Add(branchedInstruction);
					AddExits(binaryData, instructions, instructionStart, branchedInstruction->Address, previousChecks);
					if ((instructions[y]->InstrType != InstructionType::BranchLikely))
						y++; // skip delay slot
				}
				else
					break;
			}
			else
			{
				break;
			}
		}
		else if (instructions[y]->InstrType == InstructionType::JumpRegister)
		{
			if (instructionStart->Exits == nullptr)
				instructionStart->Exits = gcnew List<IInstruction^>();
			if (!instructionStart->Exits->Contains(instructions[y+1]))
				instructionStart->Exits->Add(instructions[y+1]);
			instructions[y+1]->IsEnd = instructionStart;
			return;
		}
		else if (instructions[y]->InstructionName == nullptr)
		{
			break;
		}
	}
}

void MIPS4300::PostProcess(List<DisassembledFileDetails^>^ disassembledFileDetails, array<IInstruction^>^ instructions, float% progress, bool isNew)
{
	for (int x = 0; x < instructions->Length; x++)
	{
		if (instructions[x]->IsFunction)
		{
			List<String^>^ usedReg = gcnew List<String^>();
			bool branched = false;

			List<IInstruction^>^ previousChecks = gcnew List<IInstruction^>();
			try
			{
				// find exits
				for (int y = (x); y < instructions->Length; y++)
				{
					if ((y != 0) && (instructions[y]->Address != (instructions[y-1]->Address + 4)))
						break;

					if ((instructions[y]->Target != 0xFFFFFFFF) && 
						(
						(instructions[y]->InstrType == InstructionType::Branch)
						||
						(instructions[y]->InstrType == InstructionType::BranchLikely)
						||
						(instructions[y]->InstrType == InstructionType::Jump)
						)
						)
					{
						branched = true;
						if (IsValidLocation(disassembledFileDetails, instructions, instructions[y]->Target))
						{
							IInstruction^ branchedInstruction = GetInstruction(disassembledFileDetails, instructions, instructions[y]->Target);
 							if (!previousChecks->Contains(branchedInstruction))
							{
								previousChecks->Add(branchedInstruction);
								AddExits(disassembledFileDetails, instructions, instructions[x], branchedInstruction->Address, previousChecks);
								if ((instructions[y]->InstrType != InstructionType::BranchLikely))
									y++; // skip delay slot
							}
							else
								break;
						}
						else
						{
							break;
						}
					}
					else if (instructions[y]->InstrType == InstructionType::JumpRegister)
					{
						if (instructions[x]->Exits == nullptr)
							instructions[x]->Exits = gcnew List<IInstruction^>();
						if (!instructions[x]->Exits->Contains(instructions[y+1]))
							instructions[x]->Exits->Add(instructions[y+1]);
						instructions[y+1]->IsEnd = instructions[x];
						break;
					}
					else if (instructions[y]->InstructionName == nullptr)
					{
						break;
					}
					else if (loadToFirstRegInstructions->Contains(instructions[y]->InstructionName))
					{
						if (!branched)
						{
							List<String^>^ registers = instructions[y]->Registers;
							for (int r = 1; r < registers->Count; r++)
							{
								if (!usedReg->Contains(registers[r]))
								{
									if (registers[r] != "$zero")
									{
										if (instructions[x]->InputRegisters == nullptr)
											instructions[x]->InputRegisters = gcnew List<String^>();
										if (!instructions[x]->InputRegisters->Contains(registers[r]))
											instructions[x]->InputRegisters->Add(registers[r]);
									}
								}
							}
							usedReg->Add(instructions[y]->Registers[0]);
						}
					}
				}
			}
			catch (Exception^ e)
			{
				throw e;
			}
		}
	}

	for (int x = 0; x < instructions->Length; x++)
	{
		if (instructions[x]->Target == instructions[x]->Address)
		{
			if (isNew)
			{
				if (instructions[x]->Comment == nullptr)
					instructions[x]->Comment = gcnew List<String^>();
				instructions[x]->Comment->Add("(*INFINITE LOOP)");
			}
		}

		try
		{
			if ((instructions[x]->InstructionName != nullptr) && (instructions[x]->InstructionName != nullptr))
			{
				if (
					(instructions[x]->InstructionName->Equals("LUI")) ||
					((instructions[x]->InstructionName->Equals("ADDI")) && (instructions[x]->Registers[1]->Equals(System::Enum::GetName(RegisterNames::typeid, (0))))) ||
					((instructions[x]->InstructionName->Equals("ADDIU")) && (instructions[x]->Registers[1]->Equals(System::Enum::GetName(RegisterNames::typeid, (0))))) ||
					((instructions[x]->InstructionName->Equals("ORI")) && (instructions[x]->Registers[1]->Equals(System::Enum::GetName(RegisterNames::typeid, (0)))))
					)
				{
					// TODO if used in other instrs, iterate through, 
					// check for hardcoded address
 					String^ registerLoaded = instructions[x]->Registers[0];
					unsigned long currentValue;
					if (instructions[x]->InstructionName->Equals("LUI"))
						currentValue = ((unsigned long)instructions[x]->InstrDefResults[1]) << 16;
					else if (instructions[x]->InstructionName->Equals("ADDI"))
						currentValue = ((unsigned long)instructions[x]->InstrDefResults[2]);
					else if (instructions[x]->InstructionName->Equals("ADDIU"))
						currentValue = ((unsigned long)instructions[x]->InstrDefResults[2]);
					else if (instructions[x]->InstructionName->Equals("ORI"))
						currentValue = ((unsigned long)instructions[x]->InstrDefResults[2]);
					
					for (int y = (x + 1); y < instructions->Length; y++)
					{
						if (instructions[y]->Address == 0x7F011A4C)
						{
							IInstruction^ inst = instructions[y];
						}
						if (instructions[y]->IsFunction)
							break;
						else if (instructions[y]->InstructionName != nullptr)
						{
							if (
								(
								(instructions[y]->InstructionName->StartsWith("LW")) ||
								(instructions[y]->InstructionName->StartsWith("LH")) ||
								(instructions[y]->InstructionName->StartsWith("LB"))
								)
								&& 
								((registerLoaded == instructions[y]->Registers[0]) && (registerLoaded == instructions[y]->Registers[1]))
								)
							{

								unsigned long offsetNumber = currentValue;
								offsetNumber |= (unsigned long)(instructions[y]->InstrDefResults[1]);
								if (!((instructions[y]->InstructionName->Length > 2) && ((instructions[y]->InstructionName->Substring(2, 1) == "U"))))
								{
									if (((unsigned long)(instructions[y]->InstrDefResults[1])) > 0x7FFF)
									{
										offsetNumber -= 0x10000;
									}
								}
								if (isNew)
								{
									if (instructions[y]->Comment == nullptr)
										instructions[y]->Comment = gcnew List<String^>();
									if (instructions[y]->InstructionName->StartsWith("L"))
										instructions[y]->Comment->Add("(*LOAD) " + offsetNumber.ToString("X8"));
									else
										instructions[y]->Comment->Add("(*STORE) " + offsetNumber.ToString("X8"));
								}

								if (IsValidLocation(disassembledFileDetails, instructions, offsetNumber))
								{
									IInstruction^ instructionStore = GetInstruction(disassembledFileDetails, instructions, offsetNumber);
									
									if (instructionStore->Parents == nullptr)
										instructionStore->Parents = gcnew List<IInstruction^>();
									if (!instructionStore->Parents->Contains(instructions[y]))
										instructionStore->Parents->Add(instructions[y]);
								}

								break;
							}
							else if ((loadToFirstRegInstructions->Contains(instructions[y]->InstructionName)) && (registerLoaded == instructions[y]->Registers[0]))
							{
								if ((instructions[y]->InstructionName->StartsWith("ORI")
								|| instructions[y]->InstructionName->StartsWith("ADDI")
								|| instructions[y]->InstructionName->StartsWith("ADDIU"))
								&& (instructions[y]->Registers[0] == registerLoaded) && (instructions[y]->Registers[1] == registerLoaded))
								{
									if (!(instructions[y]->InstructionName->StartsWith("ORI")))
									{
										currentValue += (unsigned long)(instructions[y]->InstrDefResults[2]);
										if (((unsigned long)(instructions[y]->InstrDefResults[2])) > 0x7FFF)
										{
											currentValue -= 0x10000;
										}
									}
									else
										currentValue |= (unsigned long)(instructions[y]->InstrDefResults[2]);

									if (isNew)
									{
										if (instructions[y]->Comment == nullptr)
											instructions[y]->Comment = gcnew List<String^>();
										instructions[y]->Comment->Add("(*CONSTANT) " + currentValue.ToString("X8"));
									}
								}
								else
								{
									break;
								}
							}
							else if (instructions[y]->InstructionName->StartsWith("ORI")
								|| instructions[y]->InstructionName->StartsWith("ADDI")
								|| instructions[y]->InstructionName->StartsWith("ADDIU"))
							{
								if (instructions[y]->Registers[1] == registerLoaded)
								{
									if (!(instructions[y]->InstructionName->StartsWith("ORI")))
									{
										currentValue += (unsigned long)(instructions[y]->InstrDefResults[2]);
										if (((unsigned long)(instructions[y]->InstrDefResults[2])) > 0x7FFF)
										{
											currentValue -= 0x10000;
										}
									}
									else
										currentValue |= (unsigned long)(instructions[y]->InstrDefResults[2]);

									if (isNew)
									{
										if (instructions[y]->Comment == nullptr)
											instructions[y]->Comment = gcnew List<String^>();
										instructions[y]->Comment->Add("(*CONSTANT) " + currentValue.ToString("X8"));
									}
								}
							}
							else if (
								(instructions[y]->InstructionName->StartsWith("LW")) ||
								(instructions[y]->InstructionName->StartsWith("LH")) ||
								(instructions[y]->InstructionName->StartsWith("LB")) ||
								(instructions[y]->InstructionName->StartsWith("SW")) ||
								(instructions[y]->InstructionName->StartsWith("SH")) ||
								(instructions[y]->InstructionName->StartsWith("SB"))
								)
							{
								if (instructions[y]->Registers[1] == registerLoaded)
								{
									unsigned long offsetNumber = currentValue;
									offsetNumber += (unsigned long)(instructions[y]->InstrDefResults[1]);
									if (!((instructions[y]->InstructionName->Length > 2) && ((instructions[y]->InstructionName->Substring(2, 1) == "U"))))
									{
										if (((unsigned long)(instructions[y]->InstrDefResults[1])) > 0x7FFF)
										{
											offsetNumber -= 0x10000;
										}
									}

									if (isNew)
									{
										if (instructions[y]->Comment == nullptr)
											instructions[y]->Comment = gcnew List<String^>();
										if (instructions[y]->InstructionName->StartsWith("L"))
											instructions[y]->Comment->Add("(*LOAD) " + offsetNumber.ToString("X8"));
										else
											instructions[y]->Comment->Add("(*STORE) " + offsetNumber.ToString("X8"));
									}

									if (IsValidLocation(disassembledFileDetails, instructions, offsetNumber))
									{
										IInstruction^ instructionStore = GetInstruction(disassembledFileDetails, instructions, offsetNumber);
										
										if (instructionStore->Parents == nullptr)
											instructionStore->Parents = gcnew List<IInstruction^>();
										if (!instructionStore->Parents->Contains(instructions[y]))
											instructionStore->Parents->Add(instructions[y]);
									}
								}
							}
							else if (
								/*(instructions[y]->Target != 0xFFFFFFFF) || */
								((instructions[y+1]->InstructionName != nullptr) && (instructions[y+1]->InstructionName->Equals("J") || (instructions[y+1]->InstructionName->Equals("JR") || instructions[y+1]->InstructionName->Equals("JALR")))))
							{
								if (instructions[y+1]->InstructionName != nullptr)
								{
									if (
									(instructions[y]->InstructionName->StartsWith("BEQL")) ||
									(instructions[y]->InstructionName->StartsWith("BNEL")) || 
									(instructions[y]->InstructionName->StartsWith("BLEZL")) ||
									(instructions[y]->InstructionName->StartsWith("BGTZL")) ||
									(instructions[y]->InstructionName->StartsWith("BGEZALL")) ||
									(instructions[y]->InstructionName->StartsWith("BLTZALL"))
									)
									{
										// likelies
									}
									else
									{
										if (instructions[y+1]->InstructionName->StartsWith("ORI")
										|| instructions[y+1]->InstructionName->StartsWith("ADDI")
										|| instructions[y+1]->InstructionName->StartsWith("ADDIU"))
										{
											if (instructions[y+1]->Registers[1] == registerLoaded)
											{
												if (!(instructions[y+1]->InstructionName->StartsWith("ORI")))
												{
													currentValue += (unsigned long)(instructions[y+1]->InstrDefResults[2]);
													if (((unsigned long)(instructions[y+1]->InstrDefResults[2])) > 0x7FFF)
													{
														currentValue -= 0x10000;
													}
												}
												else
													currentValue |= (unsigned long)(instructions[y+1]->InstrDefResults[2]);

												if (isNew)
												{		
													if (instructions[y+1]->Comment == nullptr)
														instructions[y+1]->Comment = gcnew List<String^>();
													instructions[y+1]->Comment->Add("(*CONSTANT) " + currentValue.ToString("X8"));
												}
											}
										}
										else if (
											(instructions[y+1]->InstructionName->StartsWith("LW")) ||
											(instructions[y+1]->InstructionName->StartsWith("LH")) ||
											(instructions[y+1]->InstructionName->StartsWith("LB")) ||
											(instructions[y+1]->InstructionName->StartsWith("SW")) ||
											(instructions[y+1]->InstructionName->StartsWith("SH")) ||
											(instructions[y+1]->InstructionName->StartsWith("SB"))
											)
										{
											if (instructions[y+1]->Registers[1] == registerLoaded)
											{
												unsigned long offsetNumber = currentValue;
												offsetNumber += (unsigned long)(instructions[y+1]->InstrDefResults[1]);
												if (!((instructions[y+1]->InstructionName->Length > 2) && ((instructions[y+1]->InstructionName->Substring(2, 1) == "U"))))
												{
													if (((unsigned long)(instructions[y+1]->InstrDefResults[1])) > 0x7FFF)
													{
														offsetNumber -= 0x10000;
													}
												}
												if (isNew)
												{
													if (instructions[y+1]->Comment == nullptr)
														instructions[y+1]->Comment = gcnew List<String^>();
													if (instructions[y+1]->InstructionName->StartsWith("L"))
														instructions[y+1]->Comment->Add("(*LOAD) " + offsetNumber.ToString("X8"));
													else
														instructions[y+1]->Comment->Add("(*STORE) " + offsetNumber.ToString("X8"));
												}

												if (IsValidLocation(disassembledFileDetails, instructions, offsetNumber))
												{
													IInstruction^ instructionStore = GetInstruction(disassembledFileDetails, instructions, offsetNumber);

													if (instructionStore->Parents == nullptr)
														instructionStore->Parents = gcnew List<IInstruction^>();
													if (!instructionStore->Parents->Contains(instructions[y+1]))
														instructionStore->Parents->Add(instructions[y+1]);
												}
											}
										}
									}
								}
								break;
								// exits
							}
						}
						else
							break;
					}
				}
			}
			progress = 66.6 + ((((float)((float)x / (float)instructions->Length)) * 100.0) * .334);
		}
		catch (Exception^ e)
		{
			throw;			
		}
	}

	
	for (int x = 0; x < instructions->Length; x++)
	{
		//if (instructions[x]->InstructionDefinition == nullptr)
		{
			if (instructions[x]->InstructionRaw != 0x00000000)
			{
				bool prevOrNextSame = false;
				if (x != 0)
				{
					if (((instructions[x-1]->Address == instructions[x]->Address - 4)) && (IsASCII(instructions[x-1]->InstructionRaw)))
					{
						prevOrNextSame = true;
					}
				}
				if (x != (instructions->Length - 1))
				{
					if (((instructions[x+1]->Address == instructions[x]->Address + 4)) && (IsASCII(instructions[x+1]->InstructionRaw)))
					{
						prevOrNextSame = true;
					}
				}

				if (IsASCII(instructions[x]->InstructionRaw))
				{

					if (prevOrNextSame)
					{
						unsigned char byte1 = ((instructions[x]->InstructionRaw >> 24) & 0xFF);
						unsigned char byte2 = ((instructions[x]->InstructionRaw >> 16) & 0xFF);
						unsigned char byte3 = ((instructions[x]->InstructionRaw >> 8) & 0xFF);
						unsigned char byte4 = ((instructions[x]->InstructionRaw >> 0) & 0xFF);
						if (isNew)
						{
							if (instructions[x]->Comment == nullptr)
								instructions[x]->Comment = gcnew List<String^>();
							instructions[x]->Comment->Add("(*ASCII) ");
							if ((byte1 != 0x00) && (byte1 != 0x20) && (byte1 != 0xA))
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += System::Convert::ToChar(byte1);
							else
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += ".";
							if ((byte2 != 0x00) && (byte2 != 0x20) && (byte2 != 0xA))
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += System::Convert::ToChar(byte2);
							else
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += ".";
							if ((byte3 != 0x00) && (byte3 != 0x20) && (byte3 != 0xA))
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += System::Convert::ToChar(byte3);
							else
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += ".";
							if ((byte4 != 0x00) && (byte4 != 0x20) && (byte4 != 0xA))
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += System::Convert::ToChar(byte4);
							else
								instructions[x]->Comment[instructions[x]->Comment->Count-1] += ".";
						}
					}
				}
			}
		}
	}
	progress = 100.0f;
}

bool MIPS4300::IsASCII(unsigned long hex)
{
	unsigned char byte1 = ((hex >> 24) & 0xFF);
	unsigned char byte2 = ((hex >> 16) & 0xFF);
	unsigned char byte3 = ((hex >> 8) & 0xFF);
	unsigned char byte4 = ((hex >> 0) & 0xFF);

	if (
		(((byte1 >= 0x20) && (byte1 < 0x7A)) || (byte1 == 0x00)  || (byte1 == 0x0A))
		&& (((byte2 >= 0x20) && (byte2 < 0x7A)) || (byte2 == 0x00)  || (byte2 == 0x0A))
		&& (((byte3 >= 0x20) && (byte3 < 0x7A)) || (byte3 == 0x00)  || (byte3 == 0x0A))
		&& (((byte4 >= 0x20) && (byte4 < 0x7A)) || (byte4 == 0x00)  || (byte4 == 0x0A))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

List<IColorIndexes^>^ MIPS4300::Colorize(String^ currentSelection)
{
	List<IColorIndexes^>^ colorIndexes = gcnew List<IColorIndexes^>();

	array<String^>^ lines = currentSelection->Split('\n');

	int startData = 19;
	int currentOffset = 0;
	for (int x = 0; x < lines->Length; x++)
	{
		String^ line = lines[x];
		int lineLength = line->Length;

		if (lineLength > 0)
		{
			if (!line->StartsWith("."))
			{
				//colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset, 8), System::Drawing::Color::Maroon));					
				int poundOffset = line->IndexOf("#");
			
				if (poundOffset != -1)
				{
					if (
						(line->Length > startData) &&
					((line->Substring(startData)->StartsWith("DADDIU"))
					|| (line->Substring(startData)->StartsWith("DADDI"))
					|| (line->Substring(startData)->StartsWith("ANDI"))
					|| (line->Substring(startData)->StartsWith("ORI"))
					|| (line->Substring(startData)->StartsWith("XORI"))
					|| (line->Substring(startData)->StartsWith("SLTIU"))
					|| (line->Substring(startData)->StartsWith("SLTI"))
					|| (line->Substring(startData)->StartsWith("ADDIU"))
					|| (line->Substring(startData)->StartsWith("ADDI"))
					|| (line->Substring(startData)->StartsWith("LUI")))
					)
					{
						int lastCommaSpot = line->LastIndexOf("0x", poundOffset);
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + lastCommaSpot, 6, System::Drawing::Color::Green));					
					}
					else if (
						(line->Length > startData) &&
					((line->Substring(startData)->StartsWith("BC1TL"))
					|| (line->Substring(startData)->StartsWith("BC1FL"))
					|| (line->Substring(startData)->StartsWith("BC1T"))
					|| (line->Substring(startData)->StartsWith("BC1F"))
					|| (line->Substring(startData)->StartsWith("BGTZL"))
					|| (line->Substring(startData)->StartsWith("BLEZL"))
					|| (line->Substring(startData)->StartsWith("BGTZ"))
					|| (line->Substring(startData)->StartsWith("BLEZ"))
					|| (line->Substring(startData)->StartsWith("BNEZ"))
					|| (line->Substring(startData)->StartsWith("BEQZ"))
					|| (line->Substring(startData)->StartsWith("BGEZALL"))
					|| (line->Substring(startData)->StartsWith("BLTZALL"))
					|| (line->Substring(startData)->StartsWith("BGEZAL"))
					|| (line->Substring(startData)->StartsWith("BLTZAL"))
					|| (line->Substring(startData)->StartsWith("BGEZL"))
					|| (line->Substring(startData)->StartsWith("BLTZL"))
					|| (line->Substring(startData)->StartsWith("BGEZ"))
					|| (line->Substring(startData)->StartsWith("BLTZ"))
					|| (line->Substring(startData)->StartsWith("BNEL"))
					|| (line->Substring(startData)->StartsWith("BEQL"))
					|| (line->Substring(startData)->StartsWith("BNE"))
					|| (line->Substring(startData)->StartsWith("BEQ"))
					|| (line->Substring(startData)->StartsWith("JAL"))
					|| (line->Substring(startData)->StartsWith("J"))
					)
					&& (!line->Substring(startData)->StartsWith("JR"))
					)
					{
						int lastCommaSpot = line->LastIndexOf("0x", poundOffset);
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + lastCommaSpot, 10, System::Drawing::Color::Blue));					
					}
				}
				else
				{
					int indexLabel = line->IndexOf(".LABEL");
					if (indexLabel != -1)
					{
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + indexLabel, (line->Length - 10), System::Drawing::Color::DarkOrchid));					
					}
					if (line->Contains(".XREF"))
					{
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + 10, (lineLength - 10), System::Drawing::Color::Blue));					
					}
					else if (line->Contains(".LREF"))
					{
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + 10, (lineLength - 10), System::Drawing::Color::Blue));					
					}
					else if (line->Contains(".SREF"))
					{
						colorIndexes->Add((IColorIndexes^)gcnew ColorIndex(currentOffset + 10, (lineLength - 10), System::Drawing::Color::Blue));					
					}
				}
			}
		}
		currentOffset += lineLength + 1; // \n too
	}

	return colorIndexes;
}

// IInstructionSet
void MIPS4300::ParseInstruction(unsigned long startAddress, unsigned long binaryLength, unsigned long address, MIPSDisassembler::IInstruction^ instructionRef)
{
	bool found = false;
	for (int x = 0; x < instructionDefinition->Count; x++)
	{
		if ((instructionRef->InstructionRaw & (instructionDefinition[x]->Mask)) == instructionDefinition[x]->Opcode)
		{
			unsigned long target = 0xFFFFFFFF;
			InstructionType instructionType;
			instructionRef->InstructionDefinition = instructionDefinition[x];
			instructionDefinition[x]->Parse(address, instructionRef->InstructionRaw, instructionRef->Registers, target, instructionRef->InstrDefResults);
			if (target != 0xFFFFFFFF)
				instructionRef->Target = target;
			found = true;
			break;
		}
	}

	if (!found)
	{
		
	}

	
}

unsigned long MIPS4300::InstructionSize::get()
{
	return 4;
}

String^ MIPS4300::InstructionSetName::get()
	{
		return "MIPS4300";
	}

void MIPS4300::InitializeInstructionDefinition()
{
	instructionDefinition = gcnew System::Collections::Generic::List<InstructionDefinition^>();
	List<InstructionContent^>^ instContent;

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("NOP",  0xFFFFFFFF, 0x00000000, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("ERET", 0xFFFFFFFF, 0x42000018, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("TLBP", 0xFFFFFFFF, 0x42000008, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("TLBR", 0xFFFFFFFF, 0x42000001, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("TLWI", 0xFFFFFFFF, 0x42000002, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instructionDefinition->Add(gcnew InstructionDefinition("TLWR", 0xFFFFFFFF, 0x42000006, instContent, "{0}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.D.S", 0xFFFF003F, 0x46000021, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.D.W", 0xFFFF003F, 0x46800021, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.D.L", 0xFFFF003F, 0x46A00021, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.L.S", 0xFFFF003F, 0x46000025, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.L.D", 0xFFFF003F, 0x46200025, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.S.W", 0xFFFF003F, 0x46800020, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.S.D", 0xFFFF003F, 0x46200020, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.S.L", 0xFFFF003F, 0x46A00020, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.W.S", 0xFFFF003F, 0x46000024, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CVT.W.D", 0xFFFF003F, 0x46200024, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ABS.S", 0xFFFF003F, 0x46000005, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ABS.D", 0xFFFF003F, 0x46200005, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MOV.S", 0xFFFF003F, 0x46000006, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MOV.D", 0xFFFF003F, 0x46200006, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("NEG.S", 0xFFFF003F, 0x46000007, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("NEG.D", 0xFFFF003F, 0x46200007, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CEIL.L.S", 0xFFFF003F, 0x4600000A, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CEIL.L.D", 0xFFFF003F, 0x4620000A, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CEIL.W.S", 0xFFFF003F, 0x4600000E, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CEIL.W.D", 0xFFFF003F, 0x4620000E, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("FLOOR.L.S", 0xFFFF003F, 0x4600000B, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("FLOOR.L.D", 0xFFFF003F, 0x4620000B, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("FLOOR.W.S", 0xFFFF003F, 0x4600000F, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("FLOOR.W.D", 0xFFFF003F, 0x4620000F, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ROUND.L.S", 0xFFFF003F, 0x46000008, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ROUND.L.D", 0xFFFF003F, 0x46200008, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ROUND.W.S", 0xFFFF003F, 0x4600000C, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ROUND.W.D", 0xFFFF003F, 0x4620000C, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SQRT.S", 0xFFFF003F, 0x46000004, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SQRT.D", 0xFFFF003F, 0x46200004, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("TRUNC.L.S", 0xFFFF003F, 0x46000009, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("TRUNC.L.D", 0xFFFF003F, 0x46200009, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("TRUNC.W.S", 0xFFFF003F, 0x4600000D, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("TRUNC.W.D", 0xFFFF003F, 0x4620000D, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BC1TL", 0xFFFF0000, 0x45030000, instContent, "{0,-8}0x{1:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BC1FL", 0xFFFF0000, 0x45020000, instContent, "{0,-8}0x{1:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BC1T", 0xFFFF0000, 0x45010000, instContent, "{0,-8}0x{1:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BC1F", 0xFFFF0000, 0x45000000, instContent, "{0,-8}0x{1:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGT.S", 0xFFE007FF, 0x4600003F, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.LE.S", 0xFFE007FF, 0x4600003E, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGE.S", 0xFFE007FF, 0x4600003D, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.LT.S", 0xFFE007FF, 0x4600003C, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGL.S", 0xFFE007FF, 0x4600003B, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.SEQ.S", 0xFFE007FF, 0x4600003A, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGLE.S", 0xFFE007FF, 0x46000039, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.SF.S", 0xFFE007FF, 0x46000038, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.ULE.S", 0xFFE007FF, 0x46000037, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.OLE.S", 0xFFE007FF, 0x46000036, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.ULT.S", 0xFFE007FF, 0x46000035, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.OLT.S", 0xFFE007FF, 0x46000034, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.UEQ.S", 0xFFE007FF, 0x46000033, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.EQ.S", 0xFFE007FF, 0x46000032, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.UN.S", 0xFFE007FF, 0x46000031, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.F.S", 0xFFE007FF, 0x46000030, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGT.D", 0xFFE007FF, 0x4620003F, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.LE.D", 0xFFE007FF, 0x4620003E, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGE.D", 0xFFE007FF, 0x4620003D, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.LT.D", 0xFFE007FF, 0x4620003C, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGL.D", 0xFFE007FF, 0x4620003B, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.SEQ.D", 0xFFE007FF, 0x4620003A, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.NGLE.D", 0xFFE007FF, 0x46200039, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.SF.D", 0xFFE007FF, 0x46200038, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.ULE.D", 0xFFE007FF, 0x46200037, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.OLE.D", 0xFFE007FF, 0x46200036, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.ULT.D", 0xFFE007FF, 0x46200035, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.OLT.D", 0xFFE007FF, 0x46200034, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.UEQ.D", 0xFFE007FF, 0x46200033, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.EQ.D", 0xFFE007FF, 0x46200032, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.UN.D", 0xFFE007FF, 0x46200031, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("C.F.D", 0xFFE007FF, 0x46200030, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CTC1", 0xFFE007FF, 0x44C00000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CFC1", 0xFFE007FF, 0x44400000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MFC1", 0xFFE007FF, 0x44000000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MTC1", 0xFFE007FF, 0x44800000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DMFC1", 0xFFE007FF, 0x44200000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DMTC1", 0xFFE007FF, 0x44A00000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP0Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MFC0", 0xFFE007FF, 0x40000000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP0Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MTC0", 0xFFE007FF, 0x40800000, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DIV.S", 0xFFE0003F, 0x46000003, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DIV.D", 0xFFE0003F, 0x46200003, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ADD.S", 0xFFE0003F, 0x46000000, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ADD.D", 0xFFE0003F, 0x46200000, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SUB.S", 0xFFE0003F, 0x46000001, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SUB.D", 0xFFE0003F, 0x46200001, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MUL.S", 0xFFE0003F, 0x46000002, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 6, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MUL.D", 0xFFE0003F, 0x46200002, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRA32", 0xFFE0003F, 0x0000003F, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRL32", 0xFFE0003F, 0x0000003E, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSLL32", 0xFFE0003F, 0x0000003C, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRA", 0xFFE0003F, 0x0000003B, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRL", 0xFFE0003F, 0x0000003A, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSLL", 0xFFE0003F, 0x00000038, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SRA", 0xFFE0003F, 0x00000003, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SRL", 0xFFE0003F, 0x00000002, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SLL", 0xFFE0003F, 0x00000000, instContent, "{0,-8}{1}, {2}, 0x{3:X2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("LUI", 0xFFE00000, 0x3C000000, instContent, "{0,-8}{1}, 0x{2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("JR", 0xFC1FFFFF, 0x00000008, instContent, "{0,-8}{1}", InstructionType::JumpRegister));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MTHI", 0xFC1FFFFF, 0x00000011, instContent, "{0,-8}{1}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MTLO", 0xFC1FFFFF, 0x00000013, instContent, "{0,-8}{1}", InstructionType::DoubleArithmetic));
	
	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("JALR", 0xFC1F07FF, 0x00000009, instContent, "{0,-8}{1}, {2}", InstructionType::JumpAndLinkRegister));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGTZL", 0xFC1F0000, 0x5C000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLEZL", 0xFC1F0000, 0x58000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGTZ", 0xFC1F0000, 0x1C000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLEZ", 0xFC1F0000, 0x18000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BNEZ", 0xFC1F0000, 0x14000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BEQZ", 0xFC1F0000, 0x10000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGEZALL", 0xFC1F0000, 0x04130000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLTZALL", 0xFC1F0000, 0x04120000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGEZAL", 0xFC1F0000, 0x04110000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLTZAL", 0xFC1F0000, 0x04100000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGEZL", 0xFC1F0000, 0x04030000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLTZL", 0xFC1F0000, 0x04020000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BGEZ", 0xFC1F0000, 0x04010000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BLTZ", 0xFC1F0000, 0x04000000, instContent, "{0,-8}{1}, 0x{2:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TGEI", 0xFC1F0000, 0x04080000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TGEIU", 0xFC1F0000, 0x04090000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TLTI", 0xFC1F0000, 0x040A0000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TLTIU", 0xFC1F0000, 0x040B0000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TEQI", 0xFC1F0000, 0x040C0000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("TNEI", 0xFC1F0000, 0x040E0000, instContent, "{0,-8}{1}, {2:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MULT", 0xFC00FFFF, 0x00000018, instContent, "{0,-8}{1}, {2}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MULTU", 0xFC00FFFF, 0x00000019, instContent, "{0,-8}{1}, {2}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DMULT", 0xFC00FFFF, 0x0000001C, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DMULTU", 0xFC00FFFF, 0x0000001D, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DIVU", 0xFC00FFFF, 0x0000001B, instContent, "{0,-8}{1}, {2}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DIV", 0xFC00FFFF, 0x0000001A, instContent, "{0,-8}{1}, {2}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DDIVU", 0xFC00FFFF, 0x0000001F, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DDIV", 0xFC00FFFF, 0x0000001E, instContent, "{0,-8}{1}, {2}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSUBU", 0xFC0007FF, 0x0000002F, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSUB", 0xFC0007FF, 0x0000002E, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DADDU", 0xFC0007FF, 0x0000002D, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DADD", 0xFC0007FF, 0x0000002C, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("AND", 0xFC0007FF, 0x00000024, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ADDU", 0xFC0007FF, 0x00000021, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("ADD", 0xFC0007FF, 0x00000020, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("XOR", 0xFC0007FF, 0x00000026, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("OR", 0xFC0007FF, 0x00000025, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("NOR", 0xFC0007FF, 0x00000027, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRAV", 0xFC0007FF, 0x00000017, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSRLV", 0xFC0007FF, 0x00000016, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("DSLLV", 0xFC0007FF, 0x00000014, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SRAV", 0xFC0007FF, 0x00000007, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SRLV", 0xFC0007FF, 0x00000006, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SLLV", 0xFC0007FF, 0x00000004, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SLT", 0xFC0007FF, 0x0000002A, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SLTU", 0xFC0007FF, 0x0000002B, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SUB", 0xFC0007FF, 0x00000022, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SUBU", 0xFC0007FF, 0x00000023, instContent, "{0,-8}{1}, {2}, {3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 20));
	instructionDefinition->Add(gcnew InstructionDefinition("SYSCALL", 0xFC00003F, 0x0000000C, instContent, "{0,-7}{1}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 20));
	instructionDefinition->Add(gcnew InstructionDefinition("BREAK", 0xFC00003F, 0x0000000D, instContent, "{0,-7}{1}", InstructionType::Assign));
	
	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TEQ", 0xFC00003F, 0x00000034, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TGEU", 0xFC00003F, 0x00000031, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TGE", 0xFC00003F, 0x00000030, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TLT", 0xFC00003F, 0x00000032, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TLTU", 0xFC00003F, 0x00000033, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 10));
	instructionDefinition->Add(gcnew InstructionDefinition("TNE", 0xFC00003F, 0x00000036, instContent, "{0,-8}{1}, {2}, {3:X3}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 6, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SYNC", 0xFFFFF83F, 0x0000000F, instContent, "{0,-8}{1:X2}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MFHI", 0xFFFF07FF, 0x00000010, instContent, "{0,-8}{1}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 11, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("MFLO", 0xFFFF07FF, 0x00000012, instContent, "{0,-8}{1}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LD", 0xFC000000, 0xDC000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SD", 0xFC000000, 0xFC000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LDC1", 0xFC000000, 0xD4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SDC1", 0xFC000000, 0xF4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SC", 0xFC000000, 0xE0000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SCD", 0xFC000000, 0xF0000000, instContent, "{0,-5}{1}, 0x{2:X4}({3})", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LWC1", 0xFC000000, 0xC4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::COP1Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SWC1", 0xFC000000, 0xE4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("CACHE", 0xFC000000, 0xBC000000, instContent, "{0,-8}{1:X2}, 0x{2:X4}({3})", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LWU", 0xFC000000, 0x9C000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LWR", 0xFC000000, 0x98000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SWR", 0xFC000000, 0xB8000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LHU", 0xFC000000, 0x94000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LBU", 0xFC000000, 0x90000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LW", 0xFC000000, 0x8C000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SW", 0xFC000000, 0xAC000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LWL", 0xFC000000, 0x88000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SWL", 0xFC000000, 0xA8000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LH", 0xFC000000, 0x84000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SH", 0xFC000000, 0xA4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LB", 0xFC000000, 0x80000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SB", 0xFC000000, 0xA0000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LL", 0xFC000000, 0xC0000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LLD", 0xFC000000, 0xD0000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LDL", 0xFC000000, 0x68000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SDL", 0xFC000000, 0xB0000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("LDR", 0xFC000000, 0x6C000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Load));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instructionDefinition->Add(gcnew InstructionDefinition("SDR", 0xFC000000, 0xB4000000, instContent, "{0,-8}{1}, 0x{2:X4}({3})", InstructionType::Store));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("DADDIU", 0xFC000000, 0x64000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("DADDI", 0xFC000000, 0x60000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::DoubleArithmetic));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("ANDI", 0xFC000000, 0x30000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("ORI", 0xFC000000, 0x34000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("XORI", 0xFC000000, 0x38000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("SLTIU", 0xFC000000, 0x2C000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("SLTI", 0xFC000000, 0x28000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("ADDIU", 0xFC000000, 0x24000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Immediate, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("ADDI", 0xFC000000, 0x20000000, instContent, "{0,-8}{1}, {2}, 0x{3:X4}", InstructionType::Assign));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BNEL", 0xFC000000, 0x54000000, instContent, "{0,-8}{1}, {2}, 0x{3:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BEQL", 0xFC000000, 0x50000000, instContent, "{0,-8}{1}, {2}, 0x{3:X8}", InstructionType::BranchLikely));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BNE", 0xFC000000, 0x14000000, instContent, "{0,-8}{1}, {2}, 0x{3:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 21, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Register, 16, 5));
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Branch, 0, 16));
	instructionDefinition->Add(gcnew InstructionDefinition("BEQ", 0xFC000000, 0x10000000, instContent, "{0,-8}{1}, {2}, 0x{3:X8}", InstructionType::Branch));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Jump, 0, 26));
	instructionDefinition->Add(gcnew InstructionDefinition("JAL", 0xFC000000, 0x0C000000, instContent, "{0,-8}0x{1:X8}", InstructionType::JumpAndLink));

	instContent = gcnew List<InstructionContent^>();
	instContent->Add(gcnew InstructionContent(InstructionEntryType::Jump, 0, 26));
	instructionDefinition->Add(gcnew InstructionDefinition("J", 0xFC000000, 0x08000000, instContent, "{0,-8}0x{1:X8}", InstructionType::Jump));
}

}