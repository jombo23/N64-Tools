#include "StdAfx.h"
#include "DisassemblyContents.h"

using System::Windows::Forms::MessageBox;

namespace MIPSDisassembler
{

DisassemblyContents::DisassemblyContents(void)
{
	progress = 0.0;
	setOfLines = gcnew List<String^>();
}


float DisassemblyContents::GetProgress()
{
	return progress;
}

IInstruction^ DisassemblyContents::GetInstruction(unsigned long location)
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

bool DisassemblyContents::IsValidLocation(unsigned long location)
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

int DisassemblyContents::GetInstrOffset(unsigned long location)
{
	for (int x = 0; x < binaryData->Count; x++)
	{
		if ((location >= binaryData[x]->startAddress) && (location < (binaryData[x]->startAddress + binaryData[x]->binarySize)))
		{
			int instrOffset = binaryData[x]->offsetInInstructions + ((location - binaryData[x]->startAddress) / 4);
			return instrOffset;
		}
	}

	return -1;
}

void DisassemblyContents::Disassemble()
{
	progress = 0.0;
	
	int totalInstr = 0;
	for (int x = 0; x < binaryData->Count; x++)
	{
		binaryData[x]->offsetInInstructions = totalInstr;
		totalInstr += (binaryData[x]->binarySize / 4);
	}

	// First pass parse
	instructions = gcnew array<Instruction^>(totalInstr);
	int currentInstr = binaryData[0]->offsetInInstructions;
	for (int x = 0; x < binaryData->Count; x++)
	{
		currentInstr = binaryData[x]->offsetInInstructions;
		for (unsigned long address = 0; address < binaryData[x]->binarySize; address+=4)
		{
			try
			{
				instructions[currentInstr+(address/4)] = gcnew Instruction();
				instructions[currentInstr+(address/4)]->Address = (binaryData[x]->startAddress + address);
				instructions[currentInstr+(address/4)]->InstructionRaw = MIPSDisassembler::SharedFunctions::ByteArrayToChar(binaryData[x]->binary, address);


				instructionSet->ParseInstruction(binaryData[x]->startAddress, binaryData[x]->binarySize, (binaryData[x]->startAddress + address), instructions[currentInstr+(address/4)]);
				
  				progress = (((float)((float)address / (float)binaryData[x]->binarySize)) * 100.0) * .333;
			}
			catch (Exception^ e)
			{
				MessageBox::Show("Exception " + e);
				return;
			}
		}
	}

	progress = 33.3;


	// Second pass, parent based on targets
	for (int x = 0; x < totalInstr; x++)
	{
		try
		{
			unsigned long target = instructions[x]->Target;
			if (target != 0xFFFFFFFF)
			{
				// check if above start, and below end
				if (IsValidLocation(target))
				{
					IInstruction^ targetInstruction = GetInstruction(target);
					if (targetInstruction->Parents == nullptr)
					{
						targetInstruction->Parents = gcnew List<IInstruction^>();
					}

					IInstruction^ instructionInterV = (IInstruction^)instructions[x];
					targetInstruction->Parents->Add(instructionInterV);

					if ((instructionInterV->InstrType == InstructionType::JumpAndLink)
						|| (instructionInterV->InstrType == InstructionType::BranchAndLink))
					{
						targetInstruction->IsFunction = true;
					}
				}
			}
		}
		catch (Exception^ e)
		{
			MessageBox::Show("Exception " + e);
			return;
		}
		progress = 33.3 + ((((float)((float)x / (float)totalInstr)) * 100.0) * .333);
	}

	progress = 66.6;

	try
	{
		instructionSet->PostProcess(binaryData, instructions, progress, isNew);
	}
	catch(Exception^ e)
	{
		MessageBox::Show("Error post-processing " + e->ToString());
	}

	
	
	array<bool>^ asmDataTracked = gcnew array<bool>(instructions->Length);
	for (int x = 0; x < instructions->Length; x++)
		asmDataTracked[x] = false;

	for (int instrOffset = 0; instrOffset < binaryData->Count; instrOffset++)
	{
		for (unsigned long address = binaryData[instrOffset]->startAddress; address < (binaryData[instrOffset]->startAddress + binaryData[instrOffset]->binarySize); address += 4)
		{
			IInstruction^ currentInstruction = GetInstruction(address);

			if (currentInstruction->IsFunction)
			{
				if (currentInstruction->Exits != nullptr)
				{
					unsigned long start = currentInstruction->Address;
					for (int x = 0; x < currentInstruction->Exits->Count; x++)
					{
						unsigned long end = currentInstruction->Exits[x]->Address + 4;

						for (unsigned long spot = start; spot < end; spot++)
						{
							if (GetInstrOffset(spot) != -1)
							{
								asmDataTracked[GetInstrOffset(spot)] = true;
							}
						}
					}
				}
			}
		}
	}

	String^ fileName = "C:\\temp\\textfile.txt";
	//System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(fileName);
	System::IO::StreamWriter^ sw = nullptr;

	// Check if beginnings are good
	unsigned long checkAddress = 0xFFFFFFFF;
	for (int x = 0; x < instructions->Length; x++)
	{
		if (instructions[x]->Address != checkAddress)
		{
			for (int y = x; y < instructions->Length; y++)
			{
				if (asmDataTracked[y] == true)
				{
					int countGood = 0;
					for (int z = x; z < y; z++)
					{
						if (instructions[z]->InstructionDefinition != nullptr)
							countGood++;
					}

					double percentGood = (double)countGood / (double)(y - x);
					if (percentGood >= 0.95)
					{
						for (int rr = x; rr < y; rr++)
						{
							asmDataTracked[rr] = true;
						}
					}
					break;
				}
			}
		}
		checkAddress = instructions[x]->Address + 4;
	}

	// check if missed heavy asm sections, add 'em
	checkAddress = instructions[0]->Address;
	for (int x = 0; x < instructions->Length; x++)
	{		
		if (instructions[x]->Address != checkAddress)
		{
			checkAddress = instructions[x+1]->Address;
			continue;
		}

	
		int countTracked = 0;
		int countGood = 0;
		int countNop = 0;
		int endSpot = x;
		for (int y = x; ((y < (x + 500)) && (y < instructions->Length)); y++)
		{
			if (asmDataTracked[y] == true)
			{
				countTracked++;
			}
			if ((instructions[y]->InstructionRaw == 0x00000000)
				|| (instructions[y]->InstructionRaw == 0xFFFF0000)
				|| (instructions[y]->InstructionRaw == 0x0000FFFF)
				|| (instructions[y]->InstructionRaw == 0xFFFFFFFF))
			{
				countNop++;
			}

			if (instructions[y]->InstructionDefinition != nullptr)
			{
				countGood++;
			}
			endSpot = y;
		}
		if ((countNop < 120))
		{
			double percentGood = (double)countGood / (double)(endSpot - x + 1);
			if (percentGood >= 0.95)
			{
				for (int y = x; ((y < (x + 500)) && (y < instructions->Length)); y++)
				{
					asmDataTracked[y] = true;
				}
				if (sw != nullptr)
					sw->WriteLine("Added " + instructions[x]->Address.ToString("X8"));
			}
		}

		x += (500 - 1);
		if (x >= instructions->Length)
			break;

		checkAddress = (instructions[x]->Address + 4);
	}
	
	/*
	// Check segments
	for (int x = 0; x < instructions->Length; x++)
	{
		if (asmDataTracked[x] == true)
		{
			int endSpot = x;
			for (int y = (x + 1); y < instructions->Length; y++)
			{
				if (asmDataTracked[y] == false)
				{
					y--;
					endSpot = y;
					break;
				}
			}

			int countGood = 0;
			for (int y = x; y < endSpot; y++)
			{
				if (instructions[y]->InstructionDefinition != nullptr)
					countGood++;
			}
			
			double percentGood = (double)countGood / (double)(endSpot - x + 1);
			if (percentGood < 0.95)
			{
				for (int y = x; y < (endSpot + 1); y++)
				{
					asmDataTracked[y] = false;
				}
			}

			x = endSpot;
		}
	}

	// Merge close segments
	for (int x = 0; x < instructions->Length; x++)
	{
		if (asmDataTracked[x] == true)
		{
			int endSpot = x;
			
			for (int y = (x + 1); y < instructions->Length; y++)
			{
				if ((x != 0) && (instructions[x]->Address != (instructions[x-1]->Address + 4)))
				{
					x = y;
					break;
				}	
				
				if (asmDataTracked[y] == false)
				{
					bool merged = false;
					// merge segments close
					for (int z = y; ((z < instructions->Length) && (z < (y + 0x5000))); z++)
					{
						if (asmDataTracked[z] == true)
						{

							int countGood = 0;
							for (int zzzz = y; zzzz < z; zzzz++)
							{
								if (instructions[zzzz]->InstructionDefinition != nullptr)
									countGood++;
							}
							
							double percentGood = (double)countGood / (double)(z - y + 1);
							if ((instructions[z]->Address - instructions[y]->Address) < 100)
							{
								// close
							}
							else if (percentGood < 0.95)
							{
								break;
							}
							merged = true;
							for (int rr = y; rr < z; rr++)
								asmDataTracked[rr] = true;
							break;
						}
					}
					if (merged)
						continue;
					y--;
					x = y;
					break;
				}
			}
		}
	}

	// just check if afterwards and before are instructions and include last ones
	for (int x = 0; x < instructions->Length; x++)
	{
		if (asmDataTracked[x] == true)
		{
			int endSpot = x;
			for (int y = (x + 1); y < instructions->Length; y++)
			{
				if (asmDataTracked[y] == false)
				{
					y--;
					endSpot = y;
					break;
				}
			}

			// set prev ones
			for (int y = (x - 1); y >= 0; y--)
			{
				int countInRow = 0;
				int countTest = 0;
				for (int z = y; (z >= 0); z--)
				{
					if ((instructions[y]->InstructionRaw == 0x00000000)
					|| (instructions[y]->InstructionRaw == 0xFFFF0000)
					|| (instructions[y]->InstructionRaw == 0x0000FFFF)
					|| (instructions[y]->InstructionRaw == 0xFFFFFFFF))
					{
						countInRow++;
					}
					if (countTest > 10)
						break;
					countTest++;
				}

				if (countInRow > 4)
					break;

				if (asmDataTracked[y] == true)
				{
					break;
				}

				if (instructions[y]->InstructionDefinition != nullptr)
				{
					asmDataTracked[y] = true;
				}
				else
				{
					break;
				}

				if ((y == 0) || (instructions[y]->Address != (instructions[y-1]->Address + 4)))
					break;
			}

			// set next ones
			for (int y = (endSpot + 1); y < instructions->Length; y++)
			{
				int countInRow = 0;
				int countTest = 0;
				for (int z = y; z < instructions->Length; z++)
				{
					if ((instructions[y]->InstructionRaw == 0x00000000)
					|| (instructions[y]->InstructionRaw == 0xFFFF0000)
					|| (instructions[y]->InstructionRaw == 0x0000FFFF)
					|| (instructions[y]->InstructionRaw == 0xFFFFFFFF))
					{
						countInRow++;
						

						if (countInRow > 4)
							break;
					}
					countTest++;
					if (countTest > 10)
						break;
				}

				if (asmDataTracked[y] == true)
				{
					break;
				}

				if (instructions[y]->InstructionDefinition != nullptr)
				{
					asmDataTracked[y] = true;
				}
				else
				{
					break;
				}

				if ((y == (instructions->Length - 1)) || (instructions[y+1]->Address != (instructions[y]->Address + 4)))
					break;
			}

			x = endSpot + 1;
			
		}
	}

	// things that have parents should still be kept, their regions, prob interspersed

	// done segmenting
	int prevSpot = 0;
	for (int x = 0; x < instructions->Length; x++)
	{
		if (asmDataTracked[x] == true)
		{
			int endSpot = x;
			for (int y = (x + 1); y < instructions->Length; y++)
			{
				if (asmDataTracked[y] == false)
				{
					y--;
					endSpot = y;
					break;
				}
			}

			if (sw != nullptr)
				sw->WriteLine(String::Format("Segment {0:X8} to {1:X8}", instructions[x]->Address, instructions[endSpot]->Address));
			x = endSpot + 1;

			prevSpot = x;
			
		}
		else
		{
			// check up for parents?
			// check down for parents?
			instructions[x]->InstructionDefinition = nullptr;
			instructions[x]->Target = 0xFFFFFFFF;
			instructions[x]->IsFunction = false;
			instructions[x]->IsEnd = nullptr;
			if (instructions[x]->Exits != nullptr)
			{
				instructions[x]->Exits->Clear();
				instructions[x]->Exits = nullptr;
			}
		}
	}

	// prune bad parents, TODO maybe later improve
	for (int x = 0; x < instructions->Length; x++)
	{
		if (asmDataTracked[x] == false)
		{
			if (instructions[x]->InstructionDefinition == nullptr)
			{
				if (instructions[x]->Parents != nullptr)
				{
					List<IInstruction^>^ instructToRemove = gcnew List<IInstruction^>();
					for (int y = 0; y < instructions[x]->Parents->Count; y++)
					{
						if (instructions[x]->Parents[y]->InstructionDefinition == nullptr)
						{
							instructToRemove->Add(instructions[x]->Parents[y]);
						}
						else
						{
							// this is weird spot where parent is valid, but you are not
							// maybe means we're code, but not always 
						}
					}

					for each (Instruction^ instrDel in instructToRemove)
					{
						instructions[x]->Parents->Remove(instrDel);
					}

					if (instructions[x]->Parents->Count == 0)
						instructions[x]->Parents = nullptr;
				}
			}
		}
	}*/
	

	if (sw != nullptr)
		sw->Close();

	progress = 100.0;
}

void DisassemblyContents::SetDisassemblyVariables(IInstructionSet^ instructionSet, List<DisassembledFileDetails^>^ binaryData, bool isNew)
{
	this->instructionSet = instructionSet;
	this->binaryData = binaryData;
	this->isNew = isNew;
}

}