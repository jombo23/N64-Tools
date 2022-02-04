	#pragma once

#include "StdAfx.h"
#include "N64AIFCAudio.h"
#include <math.h>
#include "..\N64SoundLibrary\yaz0.h"
#include "..\N64SoundLibrary\MORTDecoder.h"
#include "..\N64SoundLibrary\Kobe2ADPCMDecoder.h"
#include "..\N64SoundLibrary\QuakeDecoder.h"
#include "..\N64SoundLibrary\SnowboardKidsDecoder.h"
#include "..\N64SoundLibrary\ViewpointDecoder.h"
#include "..\N64SoundLibrary\EBBigDecompression.h"
#include "..\N64SoundLibrary\ExciteBikeSAMAudioDecompression.h"
#include "..\N64SoundLibrary\FightingForceAudioDecompression.h"
#include "..\N64SoundLibrary\MaddenAudioDecompression.h"
#include "..\N64SoundLibrary\SPRallyAudioDecompression.h"
#include "..\N64SoundLibrary\TwistedSnowboardingAudioDecompression.h"
#include "..\N64SoundLibrary\NamcoAudioDecompression.h"
#include "..\N64SoundLibrary\WDCAudioDecompression.h"
#include "..\N64SoundLibrary\AcclaimDEANAudioDecompression.h"
#include "..\N64SoundLibrary\DecompressClayfighter.h"

float CN64AIFCAudio::keyTable[0x100];

CN64AIFCAudio::CN64AIFCAudio(void)
{
	InitializeKeyTable();
}

CN64AIFCAudio::~CN64AIFCAudio(void)
{
}

CString CN64AIFCAudio::CompareALEnv(ALEnv* alEnv1, ALEnv* alEnv2)
{
	CString errors = "";
	if (alEnv1->attackTime != alEnv2->attackTime)
		errors += "Differing sound attackTime counts\r\n";
	if (alEnv1->decayTime != alEnv2->decayTime)
		errors += "Differing sound decayTime counts\r\n";
	if (alEnv1->releaseTime != alEnv2->releaseTime)
		errors += "Differing sound releaseTime counts\r\n";
	if (alEnv1->attackVolume != alEnv2->attackVolume)
		errors += "Differing sound attackVolume counts\r\n";
	if (alEnv1->decayVolume != alEnv2->decayVolume)
		errors += "Differing sound decayVolume counts\r\n";
	return errors;
}

CString CN64AIFCAudio::CompareALKey(ALKey* alKey1, ALKey* alKey2)
{
	CString errors = "";
	if (alKey1->velocitymin != alKey2->velocitymin)
		errors += "Differing sound velocitymin counts\r\n";
	if (alKey1->velocitymax != alKey2->velocitymax)
		errors += "Differing sound velocitymax counts\r\n";
	if (alKey1->keymin != alKey2->keymin)
		errors += "Differing sound keymin counts\r\n";
	if (alKey1->keymax != alKey2->keymax)
		errors += "Differing sound keymax counts\r\n";
	if (alKey1->keybase != alKey2->keybase)
		errors += "Differing sound keybase counts\r\n";
	if (alKey1->detune != alKey2->detune)
		errors += "Differing sound detune counts\r\n";
	return errors;
}

CString CN64AIFCAudio::CompareALTbl(ALWave* alWave1, ALWave* alWave2, bool checkBase)
{
	CString errors = "";
	if (alWave1->len == alWave2->len)
	{
		for (int z = 0; z < alWave1->len; z++)
		{
			if (alWave1->wavData[z] != alWave2->wavData[z])
			{
				errors += "tbl mismatch\r\n";
				return errors;
			}
		}
	}
	else
	{
		errors += "tbl mismatch size\r\n";
	}

	if (checkBase)
	{
		if (alWave1->base != alWave2->base)
		{
			errors += "tbl base mismatch\r\n";
		}
	}

	return errors;
}

CString CN64AIFCAudio::CompareALBook(ALWave* alWave1, ALWave* alWave2)
{
	CString errors = "";
	if ((alWave1->type == 0) && (alWave2->type == 0))
	{
		if (alWave1->adpcmWave->book->order != alWave2->adpcmWave->book->order)
		{
			errors += "Differing book->order\r\n";
			return errors;
		}
		if (alWave1->adpcmWave->book->npredictors != alWave2->adpcmWave->book->npredictors)
		{
			errors += "Differing book->npredictors\r\n";
			return errors;
		}

		for (int z = 0; z < (alWave1->adpcmWave->book->order * alWave1->adpcmWave->book->npredictors * 8); z++)
		{
			if (alWave1->adpcmWave->book->predictors[z] != alWave2->adpcmWave->book->predictors[z])
				errors += "Differing predictors \r\n";
		}
	}
	else if (alWave1->type != alWave2->type)
	{
		errors += "mismatch on book types";
	}
	return errors;
}

CString CN64AIFCAudio::CompareALLoop(ALWave* alWave1, ALWave* alWave2)
{
	CString errors = "";
	if ((alWave1->type == 0) && (alWave2->type == 0))
	{
		if ((alWave1->adpcmWave->loop != NULL) && (alWave2->adpcmWave->loop != NULL))
		{
			if (alWave1->adpcmWave->loop->start != alWave2->adpcmWave->loop->start)
				errors += "Differing loop start flags\r\n";
			if (alWave1->adpcmWave->loop->end != alWave2->adpcmWave->loop->end)
				errors += "Differing loop end flags\r\n";
			if (alWave1->adpcmWave->loop->count != alWave2->adpcmWave->loop->count)
				errors += "Differing loop count flags\r\n";

			for (int z = 0; z < 0x10; z++)
			{
				if (alWave1->adpcmWave->loop->state[z] != alWave2->adpcmWave->loop->state[z])
					errors += "Differing loop state flags\r\n";
			}
		}
		else if (((alWave1->adpcmWave->loop != NULL) && (alWave2->adpcmWave->loop == NULL)) || 
			((alWave1->adpcmWave->loop == NULL) && (alWave2->adpcmWave->loop != NULL)))
		{
			errors += "Differing loop null\r\n";
		}
	}
	else if ((alWave1->type == 1) && (alWave2->type == 1))
	{
		if ((alWave1->rawWave->loop != NULL) && (alWave2->rawWave->loop != NULL))
		{
			if (alWave1->rawWave->loop->start != alWave2->rawWave->loop->start)
				errors += "Differing loop start flags\r\n";
			if (alWave1->rawWave->loop->end != alWave2->rawWave->loop->end)
				errors += "Differing loop end flags\r\n";
			if (alWave1->rawWave->loop->count != alWave2->rawWave->loop->count)
				errors += "Differing loop count flags\r\n";
		}
		else if (((alWave1->rawWave->loop != NULL) && (alWave2->rawWave->loop == NULL)) || 
			((alWave1->rawWave->loop == NULL) && (alWave2->rawWave->loop != NULL)))
		{
			errors += "Differing loop null flags\r\n";
		}
	}
	else
	{
		errors += "Mismatch on loop types";
	}
	return errors;
}

CString CN64AIFCAudio::CompareALWav(ALWave* alWave1, ALWave* alWave2)
{
	CString errors = "";
	if (alWave1->len != alWave2->len)
		errors += "Differing sound len\r\n";
	if (alWave1->type != alWave2->type)
		errors += "Differing sound type\r\n";
	if (alWave1->flags != alWave2->flags)
		errors += "Differing sound flags\r\n";

	if ((alWave1->type == 0) && (alWave2->type == 0))
	{
		errors += CompareALLoop(alWave1, alWave2);
		errors += CompareALBook(alWave1, alWave2);
	}
	else if ((alWave1->type == 1) && (alWave2->type == 1))
	{
		errors += CompareALLoop(alWave1, alWave2);
	}

	errors += CompareALTbl(alWave1, alWave2);

	return errors;
}

CString CN64AIFCAudio::CompareALSound(ALSound* alSound1, ALSound* alSound2)
{
	CString errors = "";

	if (alSound1->samplePan != alSound2->samplePan)
		errors += "Differing instr samplePan counts\r\n";
	if (alSound1->sampleVolume != alSound2->sampleVolume)
		errors += "Differing instr sampleVolume counts\r\n";
	if (alSound1->flags != alSound2->flags)
		errors += "Differing instr flags counts\r\n";

	errors += CompareALEnv(&alSound1->env, &alSound2->env);
	errors += CompareALKey(&alSound1->key, &alSound2->key);
	errors += CompareALWav(&alSound1->wav, &alSound2->wav);	
	
	return errors;
}

CString CN64AIFCAudio::CompareALInstrument(ALInst* alInst1, ALInst* alInst2)
{
	CString errors = "";

	if (alInst1->volume != alInst2->volume)
		errors += "Differing instr volume counts\r\n";
	if (alInst1->pan != alInst2->pan)
		errors += "Differing instr pan counts\r\n";
	if (alInst1->priority != alInst2->priority)
		errors += "Differing instr priority counts\r\n";
	if (alInst1->flags != alInst2->flags)
		errors += "Differing instr flags counts\r\n";
	if (alInst1->tremType != alInst2->tremType)
		errors += "Differing instr tremType counts\r\n";
	if (alInst1->tremRate != alInst2->tremRate)
		errors += "Differing instr tremRate counts\r\n";
	if (alInst1->tremDepth != alInst2->tremDepth)
		errors += "Differing instr tremDepth counts\r\n";
	if (alInst1->tremDelay != alInst2->tremDelay)
		errors += "Differing instr tremDelay counts\r\n";
	if (alInst1->vibType != alInst2->vibType)
		errors += "Differing instr vibType counts\r\n";
	if (alInst1->vibRate != alInst2->vibRate)
		errors += "Differing instr vibRate counts\r\n";
	if (alInst1->vibDelay != alInst2->vibDelay)
		errors += "Differing instr vibDelay counts\r\n";
	if (alInst1->bendRange != alInst2->bendRange)
		errors += "Differing instr bendRange counts\r\n";
	if (alInst1->soundCount != alInst2->soundCount)
	{
		errors += "Differing instr soundCount counts\r\n";
		return errors;
	}
	else
	{
		for (int y = 0; y < alInst1->soundCount; y++)
		{
			errors += CompareALSound(alInst1->sounds[y], alInst2->sounds[y]);
		}
	}
	
	return errors;
}

CString CN64AIFCAudio::CompareALBanks(ALBank* alBank1, ALBank* alBank2)
{
	CString errors = "";
	if (alBank1->count != alBank2->count)
		errors += "Differing instr counts\r\n";
	if (alBank1->flags != alBank2->flags)
		errors += "Differing bank flags counts\r\n";
	if (alBank1->samplerate != alBank2->samplerate)
		errors += "Differing bank samplerate counts\r\n";

	if ((alBank1->percussion == NULL) && (alBank2->percussion != NULL))
		errors += "Differing percussion +1 -2";
	else if ((alBank1->percussion != NULL) && (alBank2->percussion == NULL))
		errors += "Differing percussion -1 +2";
	else if ((alBank1->percussion != NULL) && (alBank2->percussion != NULL))
		errors += CompareALInstrument(alBank1->percussion, alBank2->percussion);

	for (int x = 0; x < alBank1->count; x++)
	{
		if (x < alBank2->count)
		{
			errors += CompareALInstrument(alBank1->inst[x], alBank2->inst[x]);
		}
	}

	return errors;
}

bool CN64AIFCAudio::ExtractEADPercussionRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile)
{
	if (alBank->countEADPercussion > 0)
	{
		if (alBank->eadPercussion[sound].wav.type == AL_RAW16_WAVE)
		{
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			fwrite(&alBank->eadPercussion[sound].wav.wavData[1], 1, (alBank->eadPercussion[sound].wav.len-2), outFileTempRaw);
			fclose(outFileTempRaw);

			return true;
		}
		else
		{
			// hack for mario kart
			if ((alBank->eadPercussion[sound].wav.adpcmWave == NULL) || (alBank->eadPercussion[sound].wav.adpcmWave->book == NULL))
				return false;

			signed short* outRawData = new signed short[alBank->eadPercussion[sound].wav.len * 4];
			int nSamples = decode(alBank->eadPercussion[sound].wav.wavData, outRawData, alBank->eadPercussion[sound].wav.len, alBank->eadPercussion[sound].wav.adpcmWave->book, ((alBank->eadPercussion[sound].wav.wavFlags & 0x30) == 0x30));
			
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			for (int x = 0; x < nSamples; x++)
			{
				fwrite(&outRawData[x], 1,2, outFileTempRaw);
			}

			fclose(outFileTempRaw);

			delete [] outRawData;
			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractSfxRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile)
{
	if (alBank->countSfx > 0)
	{
		if (alBank->alSfx[sound]->type == AL_RAW16_WAVE)
		{
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			fwrite(&alBank->alSfx[sound]->wavData[1], 1, (alBank->alSfx[sound]->len-2), outFileTempRaw);
			fclose(outFileTempRaw);

			return true;
		}
		else
		{
			// hack for mario kart
			if ((alBank->alSfx[sound]->adpcmWave == NULL) || (alBank->alSfx[sound]->adpcmWave->book == NULL))
				return false;

			signed short* outRawData = new signed short[alBank->alSfx[sound]->len * 4];
			int nSamples = decode(alBank->alSfx[sound]->wavData, outRawData, alBank->alSfx[sound]->len, alBank->alSfx[sound]->adpcmWave->book, ((alBank->alSfx[sound]->wavFlags & 0x30) == 0x30));
			
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			for (int x = 0; x < nSamples; x++)
			{
				fwrite(&outRawData[x], 1,2, outFileTempRaw);
			}

			fclose(outFileTempRaw);

			delete [] outRawData;
			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractEADPercussion(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	if (alBank->countEADPercussion > 0)
	{
		float samplingRateFloat = (float)samplingRate;

		/*if (!ignoreKeyBase)
		{
			if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
			{
				samplingRateFloat = samplingRateFloat / (*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3 == 0) ? 1 : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3);
			}
		}*/

		if (halfSamplingRate)
		{
			samplingRateFloat = samplingRateFloat / 2;
		}


		if (alBank->eadPercussion[sound].wav.type == AL_RAW16_WAVE)
		{
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			unsigned char* wavHeader = new unsigned char[0x28];


			wavHeader[0x0] = 0x52;
			wavHeader[0x1] = 0x49;
			wavHeader[0x2] = 0x46;
			wavHeader[0x3] = 0x46;
			unsigned long chunkSize = 0x2C + (alBank->eadPercussion[sound].wav.len-2) + 0x2C - 0x8;
			if (
				(alBank->eadPercussion[sound].wav.rawWave->loop != NULL)
				&&
				(alBank->eadPercussion[sound].wav.rawWave->loop->count > 0)
				)
			{
				chunkSize += 0x18;
			}
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
			wavHeader[0x8] = 0x57;
			wavHeader[0x9] = 0x41;
			wavHeader[0xA] = 0x56;
			wavHeader[0xB] = 0x45;
			wavHeader[0xC] = 0x66;
			wavHeader[0xD] = 0x6D;
			wavHeader[0xE] = 0x74;
			wavHeader[0xF] = 0x20;
			wavHeader[0x10] = 0x10;
			wavHeader[0x11] = 0x00;
			wavHeader[0x12] = 0x00;
			wavHeader[0x13] = 0x00;
			wavHeader[0x14] = 0x01;
			wavHeader[0x15] = 0x00;
			wavHeader[0x16] = 0x01;
			wavHeader[0x17] = 0x00;
			wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
			wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
			wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
			wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
			wavHeader[0x20] = 0x02;
			wavHeader[0x21] = 0x00;
			wavHeader[0x22] = 0x10;
			wavHeader[0x23] = 0x00;
			wavHeader[0x24] = 0x64;
			wavHeader[0x25] = 0x61;
			wavHeader[0x26] = 0x74;
			wavHeader[0x27] = 0x61;

			fwrite(wavHeader, 1, 0x28, outFileTempRaw );

			delete [] wavHeader;

			unsigned long length = (alBank->eadPercussion[sound].wav.len-2);
			fwrite(&length, 1, 4, outFileTempRaw);

			fwrite(&alBank->eadPercussion[sound].wav.wavData[1], 1, (alBank->eadPercussion[sound].wav.len-2), outFileTempRaw);

			
			if (
				(alBank->eadPercussion[sound].wav.rawWave->loop != NULL)
				&&
				(alBank->eadPercussion[sound].wav.rawWave->loop->count > 0)
				)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((alBank->eadPercussion[sound].wav.rawWave->loop->start >> 0) & 0xFF);
				wavHeader[0x35] = ((alBank->eadPercussion[sound].wav.rawWave->loop->start >> 8) & 0xFF);
				wavHeader[0x36] = ((alBank->eadPercussion[sound].wav.rawWave->loop->start >> 16) & 0xFF);
				wavHeader[0x37] = ((alBank->eadPercussion[sound].wav.rawWave->loop->start >> 24) & 0xFF);
				wavHeader[0x38] = (((alBank->eadPercussion[sound].wav.rawWave->loop->end) >> 0) & 0xFF);
				wavHeader[0x39] = (((alBank->eadPercussion[sound].wav.rawWave->loop->end) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alBank->eadPercussion[sound].wav.rawWave->loop->end) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alBank->eadPercussion[sound].wav.rawWave->loop->end) >> 24) & 0xFF);

				if (alBank->eadPercussion[sound].wav.rawWave->loop->count == 0xFFFFFFFF)
				{
					wavHeader[0x40] = 0x00;
					wavHeader[0x41] = 0x00;
					wavHeader[0x42] = 0x00;
					wavHeader[0x43] = 0x00;
				}
				else
				{
					wavHeader[0x40] = (((alBank->eadPercussion[sound].wav.rawWave->loop->count) >> 0) & 0xFF);
					wavHeader[0x41] = (((alBank->eadPercussion[sound].wav.rawWave->loop->count) >> 8) & 0xFF);
					wavHeader[0x42] = (((alBank->eadPercussion[sound].wav.rawWave->loop->count) >> 16) & 0xFF);
					wavHeader[0x43] = (((alBank->eadPercussion[sound].wav.rawWave->loop->count) >> 24) & 0xFF);
				}

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x24;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}


			fclose(outFileTempRaw);

			return true;
		}
		else
		{
			// hack for mario kart
			if ((alBank->eadPercussion[sound].wav.adpcmWave == NULL) || (alBank->eadPercussion[sound].wav.adpcmWave->book == NULL))
				return false;

			signed short* outRawData = new signed short[alBank->eadPercussion[sound].wav.len * 4];
			int nSamples = decode(alBank->eadPercussion[sound].wav.wavData, outRawData, alBank->eadPercussion[sound].wav.len, alBank->eadPercussion[sound].wav.adpcmWave->book, ((alBank->eadPercussion[sound].wav.wavFlags & 0x30) == 0x30));
			
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			unsigned char* wavHeader = new unsigned char[0x28];


			wavHeader[0x0] = 0x52;
			wavHeader[0x1] = 0x49;
			wavHeader[0x2] = 0x46;
			wavHeader[0x3] = 0x46;
			unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
			if (
				(alBank->eadPercussion[sound].wav.adpcmWave->loop != NULL)
				&&
				(alBank->eadPercussion[sound].wav.adpcmWave->loop->count > 0)
				)
			{
				chunkSize += 0x18;
			}
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
			wavHeader[0x8] = 0x57;
			wavHeader[0x9] = 0x41;
			wavHeader[0xA] = 0x56;
			wavHeader[0xB] = 0x45;
			wavHeader[0xC] = 0x66;
			wavHeader[0xD] = 0x6D;
			wavHeader[0xE] = 0x74;
			wavHeader[0xF] = 0x20;
			wavHeader[0x10] = 0x10;
			wavHeader[0x11] = 0x00;
			wavHeader[0x12] = 0x00;
			wavHeader[0x13] = 0x00;
			wavHeader[0x14] = 0x01;
			wavHeader[0x15] = 0x00;
			wavHeader[0x16] = 0x01;
			wavHeader[0x17] = 0x00;
			wavHeader[0x18] = ((samplingRate >> 0) & 0xFF);
			wavHeader[0x19] = ((samplingRate >> 8) & 0xFF);
			wavHeader[0x1A] = ((samplingRate >> 16) & 0xFF);
			wavHeader[0x1B] = ((samplingRate >> 24) & 0xFF);
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
			wavHeader[0x20] = 0x02;
			wavHeader[0x21] = 0x00;
			wavHeader[0x22] = 0x10;
			wavHeader[0x23] = 0x00;
			wavHeader[0x24] = 0x64;
			wavHeader[0x25] = 0x61;
			wavHeader[0x26] = 0x74;
			wavHeader[0x27] = 0x61;

			fwrite(wavHeader, 1, 0x28, outFileTempRaw );

			delete [] wavHeader;

			unsigned long length = (nSamples * 2);
			fwrite(&length, 1, 4, outFileTempRaw);

			for (int x = 0; x < nSamples; x++)
			{
				fwrite(&outRawData[x], 1,2, outFileTempRaw);
			}

			if (
				(alBank->eadPercussion[sound].wav.adpcmWave->loop != NULL)
				&&
				(alBank->eadPercussion[sound].wav.adpcmWave->loop->count > 0)
				)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((alBank->eadPercussion[sound].wav.adpcmWave->loop->start >> 0) & 0xFF);
				wavHeader[0x35] = ((alBank->eadPercussion[sound].wav.adpcmWave->loop->start >> 8) & 0xFF);
				wavHeader[0x36] = ((alBank->eadPercussion[sound].wav.adpcmWave->loop->start >> 16) & 0xFF);
				wavHeader[0x37] = ((alBank->eadPercussion[sound].wav.adpcmWave->loop->start >> 24) & 0xFF);
				wavHeader[0x38] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->end) >> 0) & 0xFF);
				wavHeader[0x39] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->end) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->end) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->end) >> 24) & 0xFF);

				if (alBank->eadPercussion[sound].wav.adpcmWave->loop->count == 0xFFFFFFFF)
				{
					wavHeader[0x40] = 0x00;
					wavHeader[0x41] = 0x00;
					wavHeader[0x42] = 0x00;
					wavHeader[0x43] = 0x00;
				}
				else
				{
					wavHeader[0x40] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->count) >> 0) & 0xFF);
					wavHeader[0x41] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->count) >> 8) & 0xFF);
					wavHeader[0x42] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->count) >> 16) & 0xFF);
					wavHeader[0x43] = (((alBank->eadPercussion[sound].wav.adpcmWave->loop->count) >> 24) & 0xFF);
				}

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x24;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}

			fclose(outFileTempRaw);

			delete [] outRawData;
			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractLoopEADPercussion(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	float samplingRateFloat = (float)samplingRate;

	if (alBank->countEADPercussion > 0)
	{
		ALWave* alWave = &alBank->eadPercussion[sound].wav;

		if (halfSamplingRate)
		{
			samplingRateFloat = samplingRateFloat / 2;
		}

		if (alWave->type == AL_RAW16_WAVE)
		{
			if (alWave->rawWave->loop == NULL)
			{
				MessageBox(NULL, "No loop", "Error", NULL);
				return false;
			}
			int numberSamples = alWave->len/2;

			if (
				(alWave->rawWave->loop->start > numberSamples)
				|| (alWave->rawWave->loop->end > numberSamples))
			{
				MessageBox(NULL, "Loop past sample count", "Error", NULL);
				return false;
			}

			int newNumberSamples = (alWave->rawWave->loop->end - alWave->rawWave->loop->start);

			if (newNumberSamples == 0)
			{
				MessageBox(NULL, "No loop", "Error", NULL);
				return false;
			}
			
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			unsigned char* wavHeader = new unsigned char[0x28];


			wavHeader[0x0] = 0x52;
			wavHeader[0x1] = 0x49;
			wavHeader[0x2] = 0x46;
			wavHeader[0x3] = 0x46;
			unsigned long chunkSize = 0x2C + (newNumberSamples*2) + 0x2C - 0x8;
			if (alWave->rawWave->loop != NULL)
			{
				chunkSize += 0x18;
			}
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
			wavHeader[0x8] = 0x57;
			wavHeader[0x9] = 0x41;
			wavHeader[0xA] = 0x56;
			wavHeader[0xB] = 0x45;
			wavHeader[0xC] = 0x66;
			wavHeader[0xD] = 0x6D;
			wavHeader[0xE] = 0x74;
			wavHeader[0xF] = 0x20;
			wavHeader[0x10] = 0x10;
			wavHeader[0x11] = 0x00;
			wavHeader[0x12] = 0x00;
			wavHeader[0x13] = 0x00;
			wavHeader[0x14] = 0x01;
			wavHeader[0x15] = 0x00;
			wavHeader[0x16] = 0x01;
			wavHeader[0x17] = 0x00;
			wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
			wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
			wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
			wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
			wavHeader[0x20] = 0x02;
			wavHeader[0x21] = 0x00;
			wavHeader[0x22] = 0x10;
			wavHeader[0x23] = 0x00;
			wavHeader[0x24] = 0x64;
			wavHeader[0x25] = 0x61;
			wavHeader[0x26] = 0x74;
			wavHeader[0x27] = 0x61;

			fwrite(wavHeader, 1, 0x28, outFileTempRaw );

			delete [] wavHeader;

			unsigned long length = newNumberSamples*2;
			fwrite(&length, 1, 4, outFileTempRaw);

			fwrite(&alWave->wavData[1+(alWave->rawWave->loop->start*2)], 1, (newNumberSamples*2), outFileTempRaw);

			
			if (alWave->rawWave->loop != NULL)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
					(alBank->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBank->soundBankFormat == ZELDAFORMAT)
					|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;

				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((0 >> 0) & 0xFF);
				wavHeader[0x35] = ((0 >> 8) & 0xFF);
				wavHeader[0x36] = ((0 >> 16) & 0xFF);
				wavHeader[0x37] = ((0 >> 24) & 0xFF);
				wavHeader[0x38] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 0) & 0xFF);
				wavHeader[0x39] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 24) & 0xFF);

				if (alWave->rawWave->loop->count == 0xFFFFFFFF)
				{
					wavHeader[0x40] = 0x00;
					wavHeader[0x41] = 0x00;
					wavHeader[0x42] = 0x00;
					wavHeader[0x43] = 0x00;
				}
				else
				{
					wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
					wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
					wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
					wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
				}

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x24;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
					(alBank->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBank->soundBankFormat == ZELDAFORMAT)
					|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}


			fclose(outFileTempRaw);
		}
		else
		{
			// hack for mario kart
			if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
				return false;

			if (alWave->adpcmWave->loop == NULL)
			{
				MessageBox(NULL, "No loop", "Error", NULL);
				return false;
			}

			signed short* outRawData = new signed short[alWave->len * 4];
			int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));

			if (
				(alWave->adpcmWave->loop->start > nSamples)
				|| (alWave->adpcmWave->loop->end > nSamples))
			{
				delete [] outRawData;
				MessageBox(NULL, "Loop past sample count", "Error", NULL);
				return false;
			}

			int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

			if (newNumberSamples == 0)
			{
				delete [] outRawData;
				MessageBox(NULL, "No loop", "Error", NULL);
				return false;
			}
			
			FILE* outFileTempRaw = fopen(outputFile, "wb");
			if (outFileTempRaw == NULL)
			{
				MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
				return false;
			}

			unsigned char* wavHeader = new unsigned char[0x28];


			wavHeader[0x0] = 0x52;
			wavHeader[0x1] = 0x49;
			wavHeader[0x2] = 0x46;
			wavHeader[0x3] = 0x46;
			unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
			if (alWave->adpcmWave->loop != NULL)
			{
				chunkSize += 0x18;
			}
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
			wavHeader[0x8] = 0x57;
			wavHeader[0x9] = 0x41;
			wavHeader[0xA] = 0x56;
			wavHeader[0xB] = 0x45;
			wavHeader[0xC] = 0x66;
			wavHeader[0xD] = 0x6D;
			wavHeader[0xE] = 0x74;
			wavHeader[0xF] = 0x20;
			wavHeader[0x10] = 0x10;
			wavHeader[0x11] = 0x00;
			wavHeader[0x12] = 0x00;
			wavHeader[0x13] = 0x00;
			wavHeader[0x14] = 0x01;
			wavHeader[0x15] = 0x00;
			wavHeader[0x16] = 0x01;
			wavHeader[0x17] = 0x00;
			wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
			wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
			wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
			wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
			wavHeader[0x20] = 0x02;
			wavHeader[0x21] = 0x00;
			wavHeader[0x22] = 0x10;
			wavHeader[0x23] = 0x00;
			wavHeader[0x24] = 0x64;
			wavHeader[0x25] = 0x61;
			wavHeader[0x26] = 0x74;
			wavHeader[0x27] = 0x61;

			fwrite(wavHeader, 1, 0x28, outFileTempRaw );

			delete [] wavHeader;

			unsigned long length = (newNumberSamples * 2);
			fwrite(&length, 1, 4, outFileTempRaw);

			for (int x = 0; x < newNumberSamples; x++)
			{
				fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
			}

			if (alWave->adpcmWave->loop != NULL)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
					(alBank->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBank->soundBankFormat == ZELDAFORMAT)
					|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((0 >> 0) & 0xFF);
				wavHeader[0x35] = ((0 >> 8) & 0xFF);
				wavHeader[0x36] = ((0 >> 16) & 0xFF);
				wavHeader[0x37] = ((0 >> 24) & 0xFF);
				wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
				wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

				if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
				{
					wavHeader[0x40] = 0x00;
					wavHeader[0x41] = 0x00;
					wavHeader[0x42] = 0x00;
					wavHeader[0x43] = 0x00;
				}
				else
				{
					wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
					wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
					wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
					wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
				}


				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x24;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (
					(alBank->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBank->soundBankFormat == ZELDAFORMAT)
					|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
				{
					float keybaseFloat = *reinterpret_cast<float*> (&alBank->eadPercussion[sound].keyBase);
					wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
				}
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

				delete [] wavHeader;
			}

			fclose(outFileTempRaw);

			delete [] outRawData;
		}

		return true;

	}
	return false;
}

bool CN64AIFCAudio::ExtractSfx(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	if (alBank->countSfx > 0)
	{
		float samplingRateFloat = (float)samplingRate;

		/*if (!ignoreKeyBase)
		{
			if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
			{
				samplingRateFloat = samplingRateFloat / (*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3 == 0) ? 1 : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3);
			}
		}*/

		if (halfSamplingRate)
		{
			samplingRateFloat = samplingRateFloat / 2;
		}


		if (alBank->alSfx[sound] != NULL)
		{
			if (alBank->alSfx[sound]->type == AL_RAW16_WAVE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (alBank->alSfx[sound]->len-2) + 0x2C - 0x8;
				if (
					(alBank->alSfx[sound]->rawWave->loop != NULL)
					&&
					(alBank->alSfx[sound]->rawWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (alBank->alSfx[sound]->len-2);
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alBank->alSfx[sound]->wavData[1], 1, (alBank->alSfx[sound]->len-2), outFileTempRaw);

				
				if (
					(alBank->alSfx[sound]->rawWave->loop != NULL)
					&&
					(alBank->alSfx[sound]->rawWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alBank->alSfx[sound]->rawWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alBank->alSfx[sound]->rawWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alBank->alSfx[sound]->rawWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alBank->alSfx[sound]->rawWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alBank->alSfx[sound]->rawWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alBank->alSfx[sound]->rawWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alBank->alSfx[sound]->rawWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alBank->alSfx[sound]->rawWave->loop->end) >> 24) & 0xFF);

					if (alBank->alSfx[sound]->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alBank->alSfx[sound]->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alBank->alSfx[sound]->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alBank->alSfx[sound]->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alBank->alSfx[sound]->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}


				fclose(outFileTempRaw);

				return true;
			}
			else
			{
				// hack for mario kart
				if ((alBank->alSfx[sound]->adpcmWave == NULL) || (alBank->alSfx[sound]->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alBank->alSfx[sound]->len * 4];
				int nSamples = decode(alBank->alSfx[sound]->wavData, outRawData, alBank->alSfx[sound]->len, alBank->alSfx[sound]->adpcmWave->book, ((alBank->alSfx[sound]->wavFlags & 0x30) == 0x30));
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
				if (
					(alBank->alSfx[sound]->adpcmWave->loop != NULL)
					&&
					(alBank->alSfx[sound]->adpcmWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = ((samplingRate >> 0) & 0xFF);
				wavHeader[0x19] = ((samplingRate >> 8) & 0xFF);
				wavHeader[0x1A] = ((samplingRate >> 16) & 0xFF);
				wavHeader[0x1B] = ((samplingRate >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (nSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				if (
					(alBank->alSfx[sound]->adpcmWave->loop != NULL)
					&&
					(alBank->alSfx[sound]->adpcmWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alBank->alSfx[sound]->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alBank->alSfx[sound]->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alBank->alSfx[sound]->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alBank->alSfx[sound]->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alBank->alSfx[sound]->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alBank->alSfx[sound]->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alBank->alSfx[sound]->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alBank->alSfx[sound]->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alBank->alSfx[sound]->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alBank->alSfx[sound]->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alBank->alSfx[sound]->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alBank->alSfx[sound]->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alBank->alSfx[sound]->adpcmWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
				return true;
			}
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractLoopSfx(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	if (alBank->countSfx > 0)
	{
		float samplingRateFloat = (float)samplingRate;

		/*if (!ignoreKeyBase)
		{
			if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
			{
				samplingRateFloat = samplingRateFloat / (*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3 == 0) ? 1 : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3);
			}
		}*/

		if (halfSamplingRate)
		{
			samplingRateFloat = samplingRateFloat / 2;
		}


		if (alBank->alSfx[sound] != NULL)
		{
			ALWave* alWave = alBank->alSfx[sound];

			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				if (alWave->rawWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				int numberSamples = alWave->len / 2;

				if (
					(alWave->rawWave->loop->start > numberSamples)
					|| (alWave->rawWave->loop->end > numberSamples))
				{
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->rawWave->loop->end - alWave->rawWave->loop->start);

				if (newNumberSamples == 0)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples*2) + 0x2C - 0x8;
				if (alWave->rawWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = newNumberSamples*2;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alWave->wavData[1+(alWave->rawWave->loop->start*2)], 1, (newNumberSamples*2), outFileTempRaw);

				
				if (alWave->rawWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}


				fclose(outFileTempRaw);
			}
			else
			{
				// hack for mario kart
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				if (alWave->adpcmWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));

				if (
					(alWave->adpcmWave->loop->start > nSamples)
					|| (alWave->adpcmWave->loop->end > nSamples))
				{
					delete [] outRawData;
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

				if (newNumberSamples == 0)
				{
					delete [] outRawData;
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
				if (alWave->adpcmWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (newNumberSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < newNumberSamples; x++)
				{
					fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
				}

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}

			return true;

		}
	}
	return false;
}

unsigned char CN64AIFCAudio::ConvertEADGameValueToKeyBase(float eadKeyvalue)
{
	float keybaseReal = (((eadKeyvalue - 0.0) < 0.00001) ? 1.0f : eadKeyvalue);

	float smallestDistance = 9999999999999.0f;
	unsigned char realKey = 0;

	for (int x = 0; x < 0x100; x++)
	{
		float distance = (fabs(keybaseReal - CN64AIFCAudio::keyTable[x]));

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			realKey = x;
		}
	}

	if (realKey > 0x7F)
		realKey = 0x7F;

	return realKey;
}

bool CN64AIFCAudio::ExtractRawPCMData(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, byte primSel)
{
	if (alBank->inst[instrument]->soundCount > 0)
	{
		if (alBank->inst[instrument]->sounds[sound] != NULL)
		{
			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (alWave->len);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int s = 0; s < alWave->len; s+=2)
				{
					fwrite(&alWave->wavData[s+1], 1, 1, outFileTempRaw);
					fwrite(&alWave->wavData[s], 1, 1, outFileTempRaw);
				}
				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (nSamples * 2);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				// TODO better
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->inst[instrument]->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				// TODO better
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			else if (alWave->type == AL_MORT_WAVE)
			{
				CMORTDecoder mortDecoder;
				std::vector<unsigned short> pcmSamples;
				// at 0 engine thought no data
				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);
				if (mortDecoder.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples))
				{
					FILE* outFileTempRaw = fopen(outputFile, "wb");
					if (outFileTempRaw == NULL)
					{
						delete [] tempWav;

						MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
						return false;
					}

					unsigned char* wavHeader = new unsigned char[0x28];

					wavHeader[0x0] = 0x52;
					wavHeader[0x1] = 0x49;
					wavHeader[0x2] = 0x46;
					wavHeader[0x3] = 0x46;
					unsigned long chunkSize = 0x2C + (pcmSamples.size() * 2) + 0x2C - 0x8;
					wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
					wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
					wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
					wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
					wavHeader[0x8] = 0x57;
					wavHeader[0x9] = 0x41;
					wavHeader[0xA] = 0x56;
					wavHeader[0xB] = 0x45;
					wavHeader[0xC] = 0x66;
					wavHeader[0xD] = 0x6D;
					wavHeader[0xE] = 0x74;
					wavHeader[0xF] = 0x20;
					wavHeader[0x10] = 0x10;
					wavHeader[0x11] = 0x00;
					wavHeader[0x12] = 0x00;
					wavHeader[0x13] = 0x00;
					wavHeader[0x14] = 0x01; //WAVE_FORMAT_PCM
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x02; //2 channels
					wavHeader[0x17] = 0x00;
					wavHeader[0x18] = (((unsigned long)alBank->inst[instrument]->samplerate >> 0) & 0xFF);
					wavHeader[0x19] = (((unsigned long)alBank->inst[instrument]->samplerate >> 8) & 0xFF);
					wavHeader[0x1A] = (((unsigned long)alBank->inst[instrument]->samplerate >> 16) & 0xFF);
					wavHeader[0x1B] = (((unsigned long)alBank->inst[instrument]->samplerate >> 24) & 0xFF);
					wavHeader[0x1C] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 0) & 0xFF);
					wavHeader[0x1D] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 8) & 0xFF);
					wavHeader[0x1E] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 16) & 0xFF);
					wavHeader[0x1F] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 24) & 0xFF);
					wavHeader[0x20] = 0x04;
					wavHeader[0x21] = 0x00;
					wavHeader[0x22] = 0x10;
					wavHeader[0x23] = 0x00;
					wavHeader[0x24] = 0x64;
					wavHeader[0x25] = 0x61;
					wavHeader[0x26] = 0x74;
					wavHeader[0x27] = 0x61;

					fwrite(wavHeader, 1, 0x28, outFileTempRaw );

					delete [] wavHeader;

					unsigned long length = (pcmSamples.size() * 2);
					fwrite(&length, 1, 4, outFileTempRaw);

					for (size_t x = 0; x < pcmSamples.size(); x++)
					{
						fwrite(&(pcmSamples[x]), 1, 2, outFileTempRaw);
					}


					// SMPL
					wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;

					fclose(outFileTempRaw);
				}
				delete [] tempWav;
			}
			else if (alWave->type == AL_KOBE2_WAVE)
			{
				CKobe2ADPCMDecoder kobe2Decode;

				int soundType = alBank->inst[instrument]->flags;
				int numberChunks = alBank->inst[instrument]->sounds[sound]->wav.len / 0xC0;

				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);

				std::vector<unsigned short> pcmSamples;
				kobe2Decode.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples, numberChunks, soundType);

				if (soundType == 2)
				{
					WriteWav(outputFile, alBank->inst[instrument]->samplerate / 2, pcmSamples);
				}
				else if (soundType == 3)
				{
					WriteWavStereo(outputFile, alBank->inst[instrument]->samplerate * 2, pcmSamples);
				}

				delete [] tempWav;
			}
			// Full credit to Musyx goes to Bobby Smiles (from Mupen emulator)
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);
				
				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (nSamples * 2);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				
				//unsigned long length = alWave->len;
				//fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(&alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if ((alWave->type == AL_EXCITEBIKE_SAM) || (alWave->type == AL_EXCITEBIKE_SFX) || (alWave->type == AL_EXCITEBIKE_SNG))
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_WDC)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_SOUTHPARKRALLY)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_TWISTEDEDGESNOWBOARDING)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_TWISTEDEDGEMUSIC)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_NAMCOMUSEUM)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_TOPGEARHYPERBIKE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ACCLAIM_DEAN)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_MP3)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_MADDENBNKB)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_MADDEN64)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_FIGHTINGFORCE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}

			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractPercussionRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile)
{
	if (alBank->percussion->soundCount > 0)
	{
		if (alBank->percussion->sounds[sound] != NULL)
		{
			ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

			if (alWave->type == AL_RAW16_WAVE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (alWave->len);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int s = 0; s < alWave->len; s+=2)
				{
					fwrite(&alWave->wavData[s+1], 1, 1, outFileTempRaw);
					fwrite(&alWave->wavData[s], 1, 1, outFileTempRaw);
				}
				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (nSamples * 2);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				// TODO better
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->percussion->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				// TODO better
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			// Full credit to Musyx goes to Bobby Smiles (from Mupen emulator)
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);
				
				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				//unsigned long length = (nSamples * 2);
				//fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				
				//unsigned long length = alWave->len;
				//fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(&alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}

			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractRawSound(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool halfSamplingRate)
{
	float samplingRateFloat = (float)samplingRate;

	if (alBank->inst[instrument]->soundCount > 0)
	{
		if (alBank->inst[instrument]->sounds[sound] != NULL)
		{
			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
			}

			/*if (!ignoreKeyBase)
			{
				if (
					(alBank->soundBankFormat == STANDARDFORMAT)
					|| (alBank->soundBankFormat == STANDARDRNCCOMPRESSED)
					|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
					|| (alBank->soundBankFormat == STANDARDRNXCOMPRESSED)
					|| (alBank->soundBankFormat == BLASTCORPSZLBSTANDARD)
					|| (alBank->soundBankFormat == NINDEC)
					)
				{
					samplingRateFloat = samplingRateFloat / CN64AIFCAudio::keyTable[alBank->inst[instrument]->sounds[sound]->key.keybase];
				}
				else if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					samplingRateFloat = samplingRateFloat / (((*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3) - 0.0) < 0.00001) ? 1.0f : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3));
				}
			}*/

			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (alWave->len) + 0x2C - 0x8;
				if (
					(alWave->rawWave->loop != NULL)
					&&
					(alWave->rawWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int s = 0; s < alWave->len; s+=2)
				{
					fwrite(&alWave->wavData[s+1], 1, 1, outFileTempRaw);
					fwrite(&alWave->wavData[s], 1, 1, outFileTempRaw);
				}

				
				if (
					(alWave->rawWave->loop != NULL)
					&&
					(alWave->rawWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->rawWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->rawWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->rawWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->rawWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (nSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->inst[instrument]->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			else if (alWave->type == AL_MORT_WAVE)
			{
				CMORTDecoder mortDecoder;
				std::vector<unsigned short> pcmSamples;
				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);
				if (mortDecoder.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples))
				{
					FILE* outFileTempRaw = fopen(outputFile, "wb");
					if (outFileTempRaw == NULL)
					{
						delete [] tempWav;
						MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
						return false;
					}

					unsigned char* wavHeader = new unsigned char[0x28];

					wavHeader[0x0] = 0x52;
					wavHeader[0x1] = 0x49;
					wavHeader[0x2] = 0x46;
					wavHeader[0x3] = 0x46;
					unsigned long chunkSize = 0x2C + (pcmSamples.size() * 2) + 0x2C - 0x8;
					wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
					wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
					wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
					wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
					wavHeader[0x8] = 0x57;
					wavHeader[0x9] = 0x41;
					wavHeader[0xA] = 0x56;
					wavHeader[0xB] = 0x45;
					wavHeader[0xC] = 0x66;
					wavHeader[0xD] = 0x6D;
					wavHeader[0xE] = 0x74;
					wavHeader[0xF] = 0x20;
					wavHeader[0x10] = 0x10;
					wavHeader[0x11] = 0x00;
					wavHeader[0x12] = 0x00;
					wavHeader[0x13] = 0x00;
					wavHeader[0x14] = 0x01; //WAVE_FORMAT_PCM
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x02; //2 channels
					wavHeader[0x17] = 0x00;
					wavHeader[0x18] = (((unsigned long)alBank->inst[instrument]->samplerate >> 0) & 0xFF);
					wavHeader[0x19] = (((unsigned long)alBank->inst[instrument]->samplerate >> 8) & 0xFF);
					wavHeader[0x1A] = (((unsigned long)alBank->inst[instrument]->samplerate >> 16) & 0xFF);
					wavHeader[0x1B] = (((unsigned long)alBank->inst[instrument]->samplerate >> 24) & 0xFF);
					wavHeader[0x1C] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 0) & 0xFF);
					wavHeader[0x1D] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 8) & 0xFF);
					wavHeader[0x1E] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 16) & 0xFF);
					wavHeader[0x1F] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 24) & 0xFF);
					wavHeader[0x20] = 0x04;
					wavHeader[0x21] = 0x00;
					wavHeader[0x22] = 0x10;
					wavHeader[0x23] = 0x00;
					wavHeader[0x24] = 0x64;
					wavHeader[0x25] = 0x61;
					wavHeader[0x26] = 0x74;
					wavHeader[0x27] = 0x61;

					fwrite(wavHeader, 1, 0x28, outFileTempRaw );

					delete [] wavHeader;

					unsigned long length = (pcmSamples.size() * 2);
					fwrite(&length, 1, 4, outFileTempRaw);

					for (size_t x = 0; x < pcmSamples.size(); x++)
					{
						fwrite(&(pcmSamples[x]), 1, 2, outFileTempRaw);
					}


					// SMPL
					wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;

					fclose(outFileTempRaw);
				}

				delete [] tempWav;
			}
			else if (alWave->type == AL_KOBE2_WAVE)
			{
				CKobe2ADPCMDecoder kobe2Decode;

				int soundType = alBank->inst[instrument]->flags;
				int numberChunks = alBank->inst[instrument]->sounds[sound]->wav.len / 0xC0;

				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);

				std::vector<unsigned short> pcmSamples;
				kobe2Decode.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples, numberChunks, soundType);

				if (soundType == 2)
				{
					WriteWav(outputFile, samplingRateFloat / 2, pcmSamples);
				}
				else if (soundType == 3)
				{
					WriteWavStereo(outputFile, samplingRateFloat * 2, pcmSamples);
				}

				delete [] tempWav;
			}
			else if (alWave->type == AL_EXCITEBIKE_SAM)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CExciteBikeSAMAudioDecompression samDecompression;
				samDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_WDC)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				unsigned char predictorData[0x80];
				for (int x = 0; x < 0x40; x++)
				{
					CSharedFunctions::WriteShortToBuffer(predictorData, x*2, alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors[x]);
				}

				CWDCAudioDecompression wdcAudioDecompression;
				wdcAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, predictorData, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_EXCITEBIKE_SFX)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CExciteBikeSAMAudioDecompression samDecompression;
				unsigned char* data = NULL;
				int dataLength = 0;
				samDecompression.DecodeBIGSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, data, dataLength, samplingRateFloat);
				delete [] data;
			}
			else if (alWave->type == AL_NAMCOMUSEUM)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CNamcoAudioDecompression namcoAudioDecompression;
				namcoAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_TOPGEARHYPERBIKE)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CTopGearHyperbikeAudioDecompression topGearHyperbikeAudioDecompression;
				topGearHyperbikeAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_ACCLAIM_DEAN)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CAcclaimDEANAudioDecompression acclaimDEANAudioDecompression;
				acclaimDEANAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_MP3)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_EXCITEBIKE_SNG)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CEBBigDecompression bigDecompression;
				CExciteBikeSAMAudioDecompression samDecompression;

				unsigned long sizeROM = CharArrayToLong(alBank->inst[instrument]->sounds[sound]->wav.wavData);
				unsigned long offsetSfx = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[4]);
				unsigned long offsetIdx = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[8]);
				unsigned long offsetBig = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[12]);

				unsigned char* outputSfx;
				int sfxSize = 0;

				bigDecompression.DecodeFile(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], offsetSfx, outputSfx, sfxSize);

				std::vector<EBSoundIdx> ebSoundIndexes = samDecompression.GetSoundIndexes(outputSfx, offsetIdx, offsetBig);

				for (int index = 0; index < ebSoundIndexes.size(); index++)
				{
					samDecompression.DecodeBIGSound(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], ebSoundIndexes[index].offset, ebSoundIndexes[index].size, "", ebSoundIndexes[index].data, ebSoundIndexes[index].dataLength, samplingRateFloat);
				}

				if (!samDecompression.DecodeSNG(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], sizeROM, outputFile, ebSoundIndexes, samplingRateFloat))
				{
					MessageBox(NULL, "Bad sng " + outputFile, "Error", NULL);
				}

				for (int x = 0; x < ebSoundIndexes.size(); x++)
				{
					delete [] ebSoundIndexes[x].data;
					ebSoundIndexes[x].data = NULL;
				}

				delete [] outputSfx;
			}
			else if (alWave->type == AL_SOUTHPARKRALLY)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CSPRallyAudioDecompression spRallyAudioDecompression;

				unsigned char* outputBuffer = new unsigned char[alBank->inst[instrument]->sounds[sound]->wav.decompressedLength];
				spRallyAudioDecompression.FUN_00003424(alBank->inst[instrument]->sounds[sound]->wav.wavData, outputBuffer, 0, 0, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength / 0x10, 0);
				bool result = spRallyAudioDecompression.SaveProcessedWav(outputBuffer, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, 0, samplingRateFloat, outputFile);

				delete [] outputBuffer;
			}
			else if (alWave->type == AL_TWISTEDEDGESNOWBOARDING)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CTwistedSnowboardingAudioDecompression twistedSnowboardingAudioDecompression;

				signed short previousValue1 = 0;
				signed short previousValue2 = 0;

				int inputSpot = 0;

				unsigned char* output = new unsigned char[0x100000];
				int outputLength = 0;
				twistedSnowboardingAudioDecompression.FUN_00001220(&alBank->inst[instrument]->sounds[sound]->wav.wavData[0x10], inputSpot, output, outputLength, &alBank->inst[instrument]->sounds[sound]->wav.wavData[0], (alBank->inst[instrument]->sounds[sound]->wav.len / 9), previousValue1, previousValue2);
				
				unsigned char* processedWav = new unsigned char[outputLength];
				for (int x = 0; x < outputLength; x += 2)
				{
					processedWav[x] = output[x + 1];
					processedWav[x + 1] = output[x];
				}

				twistedSnowboardingAudioDecompression.SaveProcessedWav(processedWav, outputLength, 1, samplingRateFloat, outputFile);

				delete [] output;
				delete [] processedWav;
			}
			else if (alWave->type == AL_TWISTEDEDGEMUSIC)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CTwistedSnowboardingAudioDecompression twistedSnowboardingAudioDecompression;

				signed short previousValue1 = 0;
				signed short previousValue2 = 0;

				int inputSpot = 0;

				unsigned char* output = new unsigned char[0x2000000];
				int outputLength = 0;
				
				while (inputSpot < alBank->inst[instrument]->sounds[sound]->wav.len)
				{
					unsigned long flag = CSharedFunctions::CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[inputSpot + 0]);
					if (flag == 0xFFFFFFFF)
						break;
					int countEntries = CSharedFunctions::CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[inputSpot + 4]);
					countEntries = countEntries >> 5 << 5;
					int byteChange = (countEntries >> 5) + ((countEntries >> 5) << 3);

					int inputSpotPrev = inputSpot;

					if (flag)
						inputSpot += 0x424;
					else
						inputSpot += 0x24;


					int subSpot = 0;
					twistedSnowboardingAudioDecompression.FUN_00001220(&alBank->inst[instrument]->sounds[sound]->wav.wavData[inputSpot], subSpot, output, outputLength, &alBank->inst[instrument]->sounds[sound]->wav.wavData[inputSpotPrev + 0x14], byteChange / 9, previousValue1, previousValue2);
					
					inputSpot += subSpot;
				}
				
				unsigned char* processedWav = new unsigned char[outputLength];
				for (int x = 0; x < outputLength; x += 2)
				{
					processedWav[x] = output[x + 1];
					processedWav[x + 1] = output[x];
				}

				twistedSnowboardingAudioDecompression.SaveProcessedWav(processedWav, outputLength, 1, samplingRateFloat, outputFile);

				delete [] output;
				delete [] processedWav;
			}
			else if (alWave->type == AL_MADDENBNKB)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CMaddenAudioDecompression maddenAudioDecompression;
				maddenAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavFlags, alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_MADDEN64)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CMaddenAudioDecompression maddenAudioDecompression;
				maddenAudioDecompression.DecompressSoundMadden64(alBank->inst[instrument]->sounds[sound]->wav.wavFlags, alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_FIGHTINGFORCE)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CFightingForceAudioDecompression fightingForceAudioDecompression;

				fightingForceAudioDecompression.DecodeWav(alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, samplingRateFloat);
			}
			// Full credit to Musyx goes to Bobby Smiles (from Mupen emulator)
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);
				
				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (nSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);
				
				fclose(outFileTempRaw);
			}

			return true;

		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractLoopSound(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool halfSamplingRate)
{
	float samplingRateFloat = (float)samplingRate;
	//samplingRateFloat = samplingRateFloat / CN64AIFCAudio::keyTable[alBank->inst[instrument]->sounds[sound]->key.keybase];

	if (alBank->inst[instrument]->soundCount > 0)
	{
		if (alBank->inst[instrument]->sounds[sound] != NULL)
		{
			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
			}


			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				if (alWave->rawWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				int numberSamples = alWave->len/2;

				if (
					(alWave->rawWave->loop->start > numberSamples)
					|| (alWave->rawWave->loop->end > numberSamples))
				{
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->rawWave->loop->end - alWave->rawWave->loop->start);

				if (newNumberSamples == 0)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples*2) + 0x2C - 0x8;
				if (alWave->rawWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = newNumberSamples*2;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alWave->wavData[1+(alWave->rawWave->loop->start*2)], 1, (newNumberSamples*2), outFileTempRaw);

				
				if (alWave->rawWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}


				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));

				if (
					(alWave->adpcmWave->loop->start > nSamples)
					|| (alWave->adpcmWave->loop->end > nSamples))
				{
					delete [] outRawData;
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

				if (newNumberSamples == 0)
				{
					delete [] outRawData;
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
				if (alWave->adpcmWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (newNumberSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < newNumberSamples; x++)
				{
					fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
				}

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->inst[instrument]->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			else if (alWave->type == AL_MORT_WAVE)
			{
				CMORTDecoder mortDecoder;
				std::vector<unsigned short> pcmSamples;
				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);
				if (mortDecoder.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples))
				{
					FILE* outFileTempRaw = fopen(outputFile, "wb");
					if (outFileTempRaw == NULL)
					{
						delete [] tempWav;
						MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
						return false;
					}

					unsigned char* wavHeader = new unsigned char[0x28];

					wavHeader[0x0] = 0x52;
					wavHeader[0x1] = 0x49;
					wavHeader[0x2] = 0x46;
					wavHeader[0x3] = 0x46;
					unsigned long chunkSize = 0x2C + (pcmSamples.size() * 2) + 0x2C - 0x8;
					wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
					wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
					wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
					wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
					wavHeader[0x8] = 0x57;
					wavHeader[0x9] = 0x41;
					wavHeader[0xA] = 0x56;
					wavHeader[0xB] = 0x45;
					wavHeader[0xC] = 0x66;
					wavHeader[0xD] = 0x6D;
					wavHeader[0xE] = 0x74;
					wavHeader[0xF] = 0x20;
					wavHeader[0x10] = 0x10;
					wavHeader[0x11] = 0x00;
					wavHeader[0x12] = 0x00;
					wavHeader[0x13] = 0x00;
					wavHeader[0x14] = 0x01; //WAVE_FORMAT_PCM
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x02; //2 channels
					wavHeader[0x17] = 0x00;
					wavHeader[0x18] = (((unsigned long)alBank->inst[instrument]->samplerate >> 0) & 0xFF);
					wavHeader[0x19] = (((unsigned long)alBank->inst[instrument]->samplerate >> 8) & 0xFF);
					wavHeader[0x1A] = (((unsigned long)alBank->inst[instrument]->samplerate >> 16) & 0xFF);
					wavHeader[0x1B] = (((unsigned long)alBank->inst[instrument]->samplerate >> 24) & 0xFF);
					wavHeader[0x1C] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 0) & 0xFF);
					wavHeader[0x1D] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 8) & 0xFF);
					wavHeader[0x1E] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 16) & 0xFF);
					wavHeader[0x1F] = ((((unsigned long)alBank->inst[instrument]->samplerate * 2 * 2) >> 24) & 0xFF);
					wavHeader[0x20] = 0x04;
					wavHeader[0x21] = 0x00;
					wavHeader[0x22] = 0x10;
					wavHeader[0x23] = 0x00;
					wavHeader[0x24] = 0x64;
					wavHeader[0x25] = 0x61;
					wavHeader[0x26] = 0x74;
					wavHeader[0x27] = 0x61;

					fwrite(wavHeader, 1, 0x28, outFileTempRaw );

					delete [] wavHeader;

					unsigned long length = (pcmSamples.size() * 2);
					fwrite(&length, 1, 4, outFileTempRaw);

					for (size_t x = 0; x < pcmSamples.size(); x++)
					{
						fwrite(&(pcmSamples[x]), 1, 2, outFileTempRaw);
					}


					// SMPL
					wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;

					fclose(outFileTempRaw);
				}

				delete [] tempWav;
			}
			else if (alWave->type == AL_KOBE2_WAVE)
			{
				CKobe2ADPCMDecoder kobe2Decode;

				int soundType = alBank->inst[instrument]->flags;
				int numberChunks = alBank->inst[instrument]->sounds[sound]->wav.len / 0xC0;

				unsigned char* tempWav = new unsigned char[0x1000 + alBank->inst[instrument]->sounds[sound]->wav.len];
				memcpy(&tempWav[0x1000], alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len);

				std::vector<unsigned short> pcmSamples;
				kobe2Decode.Decode(tempWav, alBank->inst[instrument]->sounds[sound]->wav.len, 0x1000, alBank->inst[instrument]->sounds[sound]->wav.len, pcmSamples, numberChunks, soundType);

				if (soundType == 2)
				{
					WriteWav(outputFile, samplingRateFloat / 2, pcmSamples);
				}
				else if (soundType == 3)
				{
					WriteWavStereo(outputFile, samplingRateFloat * 2, pcmSamples);
				}

				delete [] tempWav;
			}
			else if (alWave->type == AL_EXCITEBIKE_SAM)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CExciteBikeSAMAudioDecompression samDecompression;
				samDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_WDC)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				unsigned char predictorData[0x80];
				for (int x = 0; x < 0x40; x++)
				{
					CSharedFunctions::WriteShortToBuffer(predictorData, x*2, alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors[x]);
				}

				CWDCAudioDecompression wdcAudioDecompression;
				wdcAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, predictorData, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_SOUTHPARKRALLY)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CSPRallyAudioDecompression spRallyAudioDecompression;

				unsigned char* outputBuffer = new unsigned char[alBank->inst[instrument]->sounds[sound]->wav.decompressedLength];
				spRallyAudioDecompression.FUN_00003424(alBank->inst[instrument]->sounds[sound]->wav.wavData, outputBuffer, 0, 0, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength / 0x10, 0);
				bool result = spRallyAudioDecompression.SaveProcessedWav(outputBuffer, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, 0, samplingRateFloat, outputFile);

				delete [] outputBuffer;
			}
			else if (alWave->type == AL_EXCITEBIKE_SFX)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CExciteBikeSAMAudioDecompression samDecompression;
				unsigned char* data = NULL;
				int dataLength = 0;
				samDecompression.DecodeBIGSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, data, dataLength, samplingRateFloat);
				delete [] data;
			}
			else if (alWave->type == AL_EXCITEBIKE_SNG)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CEBBigDecompression bigDecompression;
				CExciteBikeSAMAudioDecompression samDecompression;

				unsigned long sizeROM = CharArrayToLong(alBank->inst[instrument]->sounds[sound]->wav.wavData);
				unsigned long offsetSfx = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[4]);
				unsigned long offsetIdx = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[8]);
				unsigned long offsetBig = CharArrayToLong(&alBank->inst[instrument]->sounds[sound]->wav.wavData[12]);

				unsigned char* outputSfx;
				int sfxSize = 0;

				bigDecompression.DecodeFile(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], offsetSfx, outputSfx, sfxSize);

				std::vector<EBSoundIdx> ebSoundIndexes = samDecompression.GetSoundIndexes(outputSfx, offsetIdx, offsetBig);

				for (int index = 0; index < ebSoundIndexes.size(); index++)
				{
					samDecompression.DecodeBIGSound(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], ebSoundIndexes[index].offset, ebSoundIndexes[index].size, "", ebSoundIndexes[index].data, ebSoundIndexes[index].dataLength, samplingRateFloat);
				}

				if (!samDecompression.DecodeSNG(&alBank->inst[instrument]->sounds[sound]->wav.wavData[16], sizeROM, outputFile, ebSoundIndexes, samplingRateFloat))
				{
					MessageBox(NULL, "Bad sng " + outputFile, "Error", NULL);
				}

				for (int x = 0; x < ebSoundIndexes.size(); x++)
				{
					delete [] ebSoundIndexes[x].data;
					ebSoundIndexes[x].data = NULL;
				}

				delete [] outputSfx;
			}
			else if (alWave->type == AL_NAMCOMUSEUM)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CNamcoAudioDecompression namcoAudioDecompression;
				namcoAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_TOPGEARHYPERBIKE)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CTopGearHyperbikeAudioDecompression topGearHyperbikeAudioDecompression;
				topGearHyperbikeAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_ACCLAIM_DEAN)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CAcclaimDEANAudioDecompression acclaimDEANAudioDecompression;
				acclaimDEANAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_MP3)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_MADDENBNKB)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CMaddenAudioDecompression maddenAudioDecompression;
				maddenAudioDecompression.DecompressSound(alBank->inst[instrument]->sounds[sound]->wav.wavFlags, alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_MADDEN64)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CMaddenAudioDecompression maddenAudioDecompression;
				maddenAudioDecompression.DecompressSoundMadden64(alBank->inst[instrument]->sounds[sound]->wav.wavFlags, alBank->inst[instrument]->sounds[sound]->wav.wavData, 0, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.decompressedLength, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_FIGHTINGFORCE)
			{
				if (halfSamplingRate)
				{
					samplingRateFloat = samplingRateFloat / 2;
				}

				CFightingForceAudioDecompression fightingForceAudioDecompression;

				fightingForceAudioDecompression.DecodeWav(alBank->inst[instrument]->sounds[sound]->wav.wavData, alBank->inst[instrument]->sounds[sound]->wav.len, outputFile, samplingRateFloat);
			}
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if (alWave->adpcmWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);

				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				if (
					(alWave->adpcmWave->loop->start > nSamples)
					|| (alWave->adpcmWave->loop->end > nSamples))
				{
					delete [] outRawData;
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

				if (newNumberSamples == 0)
				{
					delete [] outRawData;
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
				if (alWave->adpcmWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (newNumberSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < newNumberSamples; x++)
				{
					fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
				}

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
					{
						float keybaseFloat;

						if (primSel == PRIMARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBase);
						}
						else if (primSel == PREVIOUS)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev);
						}
						else if (primSel == SECONDARY)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec);
						}
						
						wavHeader[0x14] = ConvertEADGameValueToKeyBase(keybaseFloat);
					}
					else if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}

			return true;

		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractPercussion(CString mainFolder, ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	float samplingRateFloat = (float)samplingRate;

	if (alBank->percussion->soundCount > 0)
	{
		if (alBank->percussion->sounds[sound] != NULL)
		{
			ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

			/*if (!ignoreKeyBase)
			{
				if (
					(alBank->soundBankFormat == STANDARDFORMAT)
					|| (alBank->soundBankFormat == STANDARDRNCCOMPRESSED)
					|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
					|| (alBank->soundBankFormat == STANDARDRNXCOMPRESSED)
					|| (alBank->soundBankFormat == BLASTCORPSZLBSTANDARD)
					|| (alBank->soundBankFormat == NINDEC)
					)
				{
					samplingRateFloat = samplingRateFloat / CN64AIFCAudio::keyTable[alBank->percussion->sounds[sound]->key.keybase];
				}
				else if (
						(alBank->soundBankFormat == SUPERMARIO64FORMAT)
						|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
						|| (alBank->soundBankFormat == ZELDAFORMAT)
						|| (alBank->soundBankFormat == STARFOX64FORMAT)
					)
				{
					samplingRateFloat = samplingRateFloat / (((*reinterpret_cast<float*> (&alBank->percussion->sounds[sound]->unknown3) - 0.0) < 0.00001) ? 1.0f : *reinterpret_cast<float*> (&alBank->percussion->sounds[sound]->unknown3));
				}
			}*/

			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (alWave->len) + 0x2C - 0x8;
				if (
					(alWave->rawWave->loop != NULL)
					&&
					(alWave->rawWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (alWave->len);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int s = 0; s < alWave->len; s+=2)
				{
					fwrite(&alWave->wavData[s+1], 1, 1, outFileTempRaw);
					fwrite(&alWave->wavData[s], 1, 1, outFileTempRaw);
				}

				
				if (
					(alWave->rawWave->loop != NULL)
					&&
					(alWave->rawWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->rawWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->rawWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->rawWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->rawWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (nSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->percussion->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			// Full credit to Musyx goes to Bobby Smiles (from Mupen emulator)
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);
				
				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (nSamples * 2) + 0x2C - 0x8;
				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)alBank->percussion->samplerate >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)alBank->percussion->samplerate >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)alBank->percussion->samplerate >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)alBank->percussion->samplerate >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)alBank->percussion->samplerate * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)alBank->percussion->samplerate * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)alBank->percussion->samplerate * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)alBank->percussion->samplerate * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (nSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < nSamples; x++)
				{
					fwrite(&outRawData[x], 1,2, outFileTempRaw);
				}

				if (
					(alWave->adpcmWave->loop != NULL)
					&&
					(alWave->adpcmWave->loop->count > 0)
					)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(&alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alWave->wavData, 1, alWave->len, outFileTempRaw);

				fclose(outFileTempRaw);
			}

			return true;

		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractLoopPercussion(CString mainFolder, ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate)
{
	float samplingRateFloat = (float)samplingRate;

	if (alBank->percussion->soundCount > 0)
	{
		if (alBank->percussion->sounds[sound] != NULL)
		{
			ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
			{
				if (alWave->rawWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				int numberSamples = alWave->len/2;

				if (
					(alWave->rawWave->loop->start > numberSamples)
					|| (alWave->rawWave->loop->end > numberSamples))
				{
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->rawWave->loop->end - alWave->rawWave->loop->start);

				if (newNumberSamples == 0)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples*2) + 0x2C - 0x8;
				if (alWave->rawWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = newNumberSamples*2;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alWave->wavData[1+(alWave->rawWave->loop->start*2)], 1, (newNumberSamples*2), outFileTempRaw);

				
				if (alWave->rawWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;

					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end - alWave->rawWave->loop->start) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}


				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book, ((alWave->wavFlags & 0x30) == 0x30));

				if (
					(alWave->adpcmWave->loop->start > nSamples)
					|| (alWave->adpcmWave->loop->end > nSamples))
				{
					delete [] outRawData;
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

				if (newNumberSamples == 0)
				{
					delete [] outRawData;
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
				if (alWave->adpcmWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (newNumberSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < newNumberSamples; x++)
				{
					fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
				}

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if (alWave->type == AL_VOX_WAVE)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(directory + "tempASA3d.vox", "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);

				::SetCurrentDirectory(mainFolder);

				CString tempStr;
				tempStr.Format("sox -r %d -c 1 \"%s\" -e signed -t wavpcm \"%s\"", alBank->percussion->samplerate, directory + "tempASA3d.vox", outputFile);
				hiddenExec(_T(tempStr.GetBuffer()), mainFolder);
			}
			else if (alWave->type == AL_WAV)
			{
				CString directory = outputFile.Mid(0, (outputFile.ReverseFind('\\')+1));

				FILE* outFileTemp = fopen(outputFile, "wb");
				if (outFileTemp == NULL)
				{
					return false;
				}
				fwrite(alBank->percussion->sounds[sound]->wav.wavData, 1, alBank->percussion->sounds[sound]->wav.len, outFileTemp);
				fflush(outFileTemp);
				fclose(outFileTemp);
			}
			else if (alWave->type == AL_MUSYX_WAVE)
			{
				if (alWave->adpcmWave->loop == NULL)
				{
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decodemusyxadpcm(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);

				nSamples = alWave->sampleRateNotInDefaultNintendoSpec;

				if (
					(alWave->adpcmWave->loop->start > nSamples)
					|| (alWave->adpcmWave->loop->end > nSamples))
				{
					delete [] outRawData;
					MessageBox(NULL, "Loop past sample count", "Error", NULL);
					return false;
				}

				int newNumberSamples = (alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start);

				if (newNumberSamples == 0)
				{
					delete [] outRawData;
					MessageBox(NULL, "No loop", "Error", NULL);
					return false;
				}
				
				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char* wavHeader = new unsigned char[0x28];


				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + (newNumberSamples * 2) + 0x2C - 0x8;
				if (alWave->adpcmWave->loop != NULL)
				{
					chunkSize += 0x18;
				}
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
				wavHeader[0x20] = 0x02;
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = 0x10;
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				delete [] wavHeader;

				unsigned long length = (newNumberSamples * 2);
				fwrite(&length, 1, 4, outFileTempRaw);

				for (int x = 0; x < newNumberSamples; x++)
				{
					fwrite(&outRawData[alWave->adpcmWave->loop->start+x], 1,2, outFileTempRaw);
				}

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((0 >> 0) & 0xFF);
					wavHeader[0x35] = ((0 >> 8) & 0xFF);
					wavHeader[0x36] = ((0 >> 16) & 0xFF);
					wavHeader[0x37] = ((0 >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end - alWave->adpcmWave->loop->start) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x24;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->percussion->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->percussion->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, (wavHeader[4] + 8), outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}
			else if ((alWave->type == AL_SIGNED_RAW8) || (alWave->type == AL_SIGNED_RAW16))
			{
				int flags = (alWave->type == AL_SIGNED_RAW16);

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				if ((flags & 1) == 1)
				{
					fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);
				}
				else
				{
					for (int r = 0; r < alWave->len; r++)
					{
						int value = (signed char)alWave->wavData[r];
						value += 128;
						fwrite(&value, 1, 1, outFileTempRaw);

					}
				}

				fclose(outFileTempRaw);
			}
			else if (alWave->type == AL_UNSIGNED_RAW8)
			{
				int flags = 0;

				FILE* outFileTempRaw = fopen(outputFile, "wb");
				if (outFileTempRaw == NULL)
				{
					MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
					return false;
				}

				unsigned char wavHeader[0x28];

				wavHeader[0x0] = 0x52;
				wavHeader[0x1] = 0x49;
				wavHeader[0x2] = 0x46;
				wavHeader[0x3] = 0x46;
				unsigned long chunkSize = 0x2C + alWave->len;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
				wavHeader[0x8] = 0x57;
				wavHeader[0x9] = 0x41;
				wavHeader[0xA] = 0x56;
				wavHeader[0xB] = 0x45;
				wavHeader[0xC] = 0x66;
				wavHeader[0xD] = 0x6D;
				wavHeader[0xE] = 0x74;
				wavHeader[0xF] = 0x20;
				wavHeader[0x10] = 0x10;
				wavHeader[0x11] = 0x00;
				wavHeader[0x12] = 0x00;
				wavHeader[0x13] = 0x00;
				wavHeader[0x14] = 0x01;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x01;
				wavHeader[0x17] = 0x00;
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
				wavHeader[0x20] = (1 + (flags & 1));
				wavHeader[0x21] = 0x00;
				wavHeader[0x22] = (((flags & 1) + 1) * 8);
				wavHeader[0x23] = 0x00;
				wavHeader[0x24] = 0x64;
				wavHeader[0x25] = 0x61;
				wavHeader[0x26] = 0x74;
				wavHeader[0x27] = 0x61;

				fwrite(wavHeader, 1, 0x28, outFileTempRaw );

				unsigned long length = alWave->len;
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(alWave->wavData, 1, alWave->len, outFileTempRaw);
				
				fclose(outFileTempRaw);
			}

			return true;

		}
	}
	return false;
}

BOOL CN64AIFCAudio::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
	::SetCurrentDirectory(currentDirectory);
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));


   if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength()-1))
   {
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength()-1));
   }

   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox(NULL, "For some reason Process Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

bool CN64AIFCAudio::AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddSound(alBank, instrument);

	bool replace = ReplaceSoundWithWavData(alBank, instrument, (alBank->inst[instrument]->soundCount-1), rawWavFileName, samplingRate, type, PRIMARY, false, false);
	if (replace)
	{
		UpdateAudioOffsets(alBank);
	}
	return replace;
}
bool CN64AIFCAudio::AddSound(ALBank*& alBank, int instrument)
{
	ALSound** newSounds = new ALSound*[alBank->inst[instrument]->soundCount + 1];
	for (int x = 0; x < alBank->inst[instrument]->soundCount; x++)
	{
		newSounds[x] = alBank->inst[instrument]->sounds[x];
	}

	newSounds[alBank->inst[instrument]->soundCount] = new ALSound();
	newSounds[alBank->inst[instrument]->soundCount]->env.attackTime = 0;
	newSounds[alBank->inst[instrument]->soundCount]->env.attackVolume = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->env.decayTime = 0xFFFFFFFF;
	newSounds[alBank->inst[instrument]->soundCount]->env.decayVolume = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->env.releaseTime = 0;
	newSounds[alBank->inst[instrument]->soundCount]->flags = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.detune = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.keybase = 0x3C;
	newSounds[alBank->inst[instrument]->soundCount]->key.keymax = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->key.keymin = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.velocitymax = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->key.velocitymin = 0;
	newSounds[alBank->inst[instrument]->soundCount]->samplePan = 0x40;
	newSounds[alBank->inst[instrument]->soundCount]->sampleVolume = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->wav.base = 0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.len = 4;
	newSounds[alBank->inst[instrument]->soundCount]->wav.type = AL_RAW16_WAVE;
	newSounds[alBank->inst[instrument]->soundCount]->wav.flags = 0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData = new unsigned char[4];
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[0] = 0x3;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[1] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[2] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[3] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.rawWave = new ALRAWWaveInfo();
	newSounds[alBank->inst[instrument]->soundCount]->wav.rawWave->loop = NULL;

	delete [] alBank->inst[instrument]->sounds;
	alBank->inst[instrument]->sounds = newSounds;

	alBank->inst[instrument]->soundCount++;

	return true;
}

bool CN64AIFCAudio::AddSfx(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddSfx(alBank);

	bool replace = ReplaceSfxWithWavData(alBank, (alBank->countSfx-1), rawWavFileName, samplingRate, type, false, false);
	if (replace)
	{
		UpdateAudioOffsets(alBank);
	}
	return replace;
}

bool CN64AIFCAudio::AddSfx(ALBank*& alBank)
{
	ALWave** newSounds = new ALWave*[alBank->countSfx + 1];
	for (int x = 0; x < alBank->countSfx; x++)
	{
		newSounds[x] = alBank->alSfx[x];
	}

	newSounds[alBank->countSfx] = new ALWave();
	newSounds[alBank->countSfx]->base = 0;
	newSounds[alBank->countSfx]->len = 9;
	newSounds[alBank->countSfx]->flags = 0;
	newSounds[alBank->countSfx]->wavData = new unsigned char[9];
	newSounds[alBank->countSfx]->wavData[0] = 0x0;
	newSounds[alBank->countSfx]->wavData[1] = 0x0;
	newSounds[alBank->countSfx]->wavData[2] = 0x0;
	newSounds[alBank->countSfx]->wavData[3] = 0x0;
	newSounds[alBank->countSfx]->wavData[4] = 0x0;
	newSounds[alBank->countSfx]->wavData[5] = 0x0;
	newSounds[alBank->countSfx]->wavData[6] = 0x0;
	newSounds[alBank->countSfx]->wavData[7] = 0x0;
	newSounds[alBank->countSfx]->wavData[8] = 0x0;
	newSounds[alBank->countSfx]->type = AL_ADPCM_WAVE;
	newSounds[alBank->countSfx]->adpcmWave = new ALADPCMWaveInfo();
	newSounds[alBank->countSfx]->adpcmWave->loop = NULL;

	newSounds[alBank->countSfx]->adpcmWave->loop = new ALADPCMloop();
	newSounds[alBank->countSfx]->adpcmWave->loop->start = 0;
	newSounds[alBank->countSfx]->adpcmWave->loop->end = 1;
	newSounds[alBank->countSfx]->adpcmWave->loop->count = 0;

	newSounds[alBank->countSfx]->adpcmWave->book = new ALADPCMBook();
	newSounds[alBank->countSfx]->adpcmWave->book->npredictors = 1;
	newSounds[alBank->countSfx]->adpcmWave->book->order = 2;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors = new signed short[newSounds[alBank->countSfx]->adpcmWave->book->npredictors * newSounds[alBank->countSfx]->adpcmWave->book->order * 8];
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[0] = 0xFAE2;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[1] = 0xFAD0;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[2] = 0xFE04;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[3] = 0x014F;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[4] = 0x0298;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[5] = 0x01CB;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[6] = 0x0029;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[7] = 0xFF03;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[8] = 0x081C;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[9] = 0x031A;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[10] = 0xFDF5;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[11] = 0xFBF2;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[12] = 0xFD32;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[13] = 0xFFC1;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[14] = 0x018B;
	newSounds[alBank->countSfx]->adpcmWave->book->predictors[15] = 0x01B9;

	if (alBank->alSfx != NULL)
		delete [] alBank->alSfx;

	alBank->alSfx = newSounds;

	alBank->countSfx++;

	return true;
}


bool CN64AIFCAudio::AddPercussion(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddPercussion(alBank);

	bool replace = ReplacePercussionWithWavData(alBank, (alBank->percussion->soundCount-1), rawWavFileName, samplingRate, type, false);
	if (replace)
	{
		UpdateAudioOffsets(alBank);
	}
	return replace;
}

bool CN64AIFCAudio::AddEADPercussion(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddEADPercussion(alBank);

	bool replace = ReplaceEADPercussionWithWavData(alBank, (alBank->countEADPercussion-1), rawWavFileName, samplingRate, type, false, false);
	if (replace)
	{
		UpdateAudioOffsets(alBank);
	}
	return replace;
}

bool CN64AIFCAudio::AddEADPercussion(ALBank*& alBank)
{
	EADPercussion* newSounds = new EADPercussion[alBank->countEADPercussion + 1];

	for (int x = 0; x < alBank->countEADPercussion; x++)
	{
		newSounds[x] = alBank->eadPercussion[x];
	}

	newSounds[alBank->countEADPercussion].adsrEAD[0x0] = 0x0002;
	newSounds[alBank->countEADPercussion].adsrEAD[0x1] = 0x7FBC;
	newSounds[alBank->countEADPercussion].adsrEAD[0x2] = 0x000A;
	newSounds[alBank->countEADPercussion].adsrEAD[0x3] = 0x7148;
	newSounds[alBank->countEADPercussion].adsrEAD[0x4] = 0x7FBC;
	newSounds[alBank->countEADPercussion].adsrEAD[0x5] = 0x673E;
	newSounds[alBank->countEADPercussion].adsrEAD[0x6] = 0xFFFF;
	newSounds[alBank->countEADPercussion].adsrEAD[0x7] = 0x0000;
	newSounds[alBank->countEADPercussion].unknown1A = 0x00;
	newSounds[alBank->countEADPercussion].pan = 0x40;
	newSounds[alBank->countEADPercussion].unknown1C = 0x00;
	newSounds[alBank->countEADPercussion].keyBase = 0x3C;
	newSounds[alBank->countEADPercussion].wav.base = 0;
	newSounds[alBank->countEADPercussion].wav.len = 9;
	
	newSounds[alBank->countEADPercussion].wav.flags = 0;
	newSounds[alBank->countEADPercussion].wav.wavData = new unsigned char[9];
	newSounds[alBank->countEADPercussion].wav.wavData[0] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[1] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[2] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[3] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[4] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[5] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[6] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[7] = 0x0;
	newSounds[alBank->countEADPercussion].wav.wavData[8] = 0x0;

	newSounds[alBank->countEADPercussion].wav.type = AL_ADPCM_WAVE;
	newSounds[alBank->countEADPercussion].wav.adpcmWave = new ALADPCMWaveInfo();
	
	newSounds[alBank->countEADPercussion].wav.adpcmWave->loop = new ALADPCMloop();
	newSounds[alBank->countEADPercussion].wav.adpcmWave->loop->start = 0;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->loop->end = 1;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->loop->count = 0;

	newSounds[alBank->countEADPercussion].wav.adpcmWave->book = new ALADPCMBook();
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->npredictors = 1;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->order = 2;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors = new signed short[newSounds[alBank->countEADPercussion].wav.adpcmWave->book->npredictors * newSounds[alBank->countEADPercussion].wav.adpcmWave->book->order * 8];
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[0] = 0xFAE2;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[1] = 0xFAD0;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[2] = 0xFE04;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[3] = 0x014F;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[4] = 0x0298;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[5] = 0x01CB;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[6] = 0x0029;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[7] = 0xFF03;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[8] = 0x081C;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[9] = 0x031A;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[10] = 0xFDF5;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[11] = 0xFBF2;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[12] = 0xFD32;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[13] = 0xFFC1;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[14] = 0x018B;
	newSounds[alBank->countEADPercussion].wav.adpcmWave->book->predictors[15] = 0x01B9;

	if (alBank->eadPercussion != NULL)
		delete [] alBank->eadPercussion;
	alBank->eadPercussion = newSounds;

	alBank->countEADPercussion++;

	return true;
}

bool CN64AIFCAudio::DeleteInstrument(ALBank*& alBank, int instrSel)
{
	if (instrSel >= alBank->count)
		return false;

	DisposeALInst(alBank->inst[instrSel]);

	ALInst** newInstruments = new ALInst*[alBank->count - 1];
	for (int x = 0; x < instrSel; x++)
	{
		newInstruments[x] = alBank->inst[x];
	}

	for (int x = (instrSel + 1); x < alBank->count; x++)
	{
		newInstruments[x-1] = alBank->inst[x];
	}

	delete [] alBank->inst;
	alBank->inst = newInstruments;

	alBank->count--;
	return true;
}

bool CN64AIFCAudio::AddInstrument(ALBank*& alBank)
{
	ALInst** newInstruments = new ALInst*[alBank->count + 1];
	for (int x = 0; x < alBank->count; x++)
	{
		newInstruments[x] = alBank->inst[x];
	}

	newInstruments[alBank->count] = new ALInst();
	newInstruments[alBank->count]->bendRange = 0x0000;
	newInstruments[alBank->count]->flags = 0x00;
	newInstruments[alBank->count]->pan = 0x40;
	newInstruments[alBank->count]->priority = 0x05;
	newInstruments[alBank->count]->samplerate = 0;

	newInstruments[alBank->count]->soundCount = 0;
	newInstruments[alBank->count]->sounds = NULL;

	newInstruments[alBank->count]->tremDelay = 0x00;
	newInstruments[alBank->count]->tremDepth = 0x00;
	newInstruments[alBank->count]->tremRate = 0x00;
	newInstruments[alBank->count]->tremType = 0x00;
	newInstruments[alBank->count]->vibDelay = 0x00;
	newInstruments[alBank->count]->vibDepth = 0x00;
	newInstruments[alBank->count]->vibRate = 0x00;
	newInstruments[alBank->count]->vibType = 0x00;
	newInstruments[alBank->count]->volume = 0x7F;

	delete [] alBank->inst;
	alBank->inst = newInstruments;

	alBank->count++;
	return true;
}

bool CN64AIFCAudio::AddPercussion(ALBank*& alBank)
{
	ALSound** newSounds = new ALSound*[alBank->percussion->soundCount + 1];
	for (int x = 0; x < alBank->percussion->soundCount; x++)
	{
		newSounds[x] = alBank->percussion->sounds[x];
	}

	newSounds[alBank->percussion->soundCount] = new ALSound();
	newSounds[alBank->percussion->soundCount]->env.attackTime = 0;
	newSounds[alBank->percussion->soundCount]->env.attackVolume = 0x7F;
	newSounds[alBank->percussion->soundCount]->env.decayTime = 0xFFFFFFFF;
	newSounds[alBank->percussion->soundCount]->env.decayVolume = 0x7F;
	newSounds[alBank->percussion->soundCount]->env.releaseTime = 0;
	newSounds[alBank->percussion->soundCount]->flags = 0;
	newSounds[alBank->percussion->soundCount]->key.detune = 0;
	newSounds[alBank->percussion->soundCount]->key.keybase = 0x3C;
	newSounds[alBank->percussion->soundCount]->key.keymax = 0x7F;
	newSounds[alBank->percussion->soundCount]->key.keymin = 0;
	newSounds[alBank->percussion->soundCount]->key.velocitymax = 0x7F;
	newSounds[alBank->percussion->soundCount]->key.velocitymin = 0;
	newSounds[alBank->percussion->soundCount]->samplePan = 0x40;
	newSounds[alBank->percussion->soundCount]->sampleVolume = 0x7F;
	newSounds[alBank->percussion->soundCount]->wav.base = 0;
	newSounds[alBank->percussion->soundCount]->wav.len = 9;
	
	newSounds[alBank->percussion->soundCount]->wav.type = AL_ADPCM_WAVE;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave = new ALADPCMWaveInfo();
	
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->loop = NULL;

	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book = new ALADPCMBook();
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->npredictors = 1;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->order = 2;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors = new signed short[newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->npredictors * newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->order * 8];
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[0] = 0xFAE2;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[1] = 0xFAD0;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[2] = 0xFE04;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[3] = 0x014F;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[4] = 0x0298;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[5] = 0x01CB;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[6] = 0x0029;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[7] = 0xFF03;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[8] = 0x081C;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[9] = 0x031A;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[10] = 0xFDF5;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[11] = 0xFBF2;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[12] = 0xFD32;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[13] = 0xFFC1;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[14] = 0x018B;
	newSounds[alBank->percussion->soundCount]->wav.adpcmWave->book->predictors[15] = 0x01B9;

	if (alBank->percussion->sounds != NULL)
		delete [] alBank->percussion->sounds;

	alBank->percussion->sounds = newSounds;

	alBank->percussion->soundCount++;

	return true;
}

void CN64AIFCAudio::DeleteSound(ALBank*& alBank, int instrument, int sound)
{
	if ((alBank->soundBankFormat != STANDARDFORMAT) && (alBank->soundBankFormat != STANDARDFORMATRAWALLOWED) && (alBank->soundBankFormat != SUPERMARIO64FORMAT)  && (alBank->soundBankFormat != N64PTRWAVETABLETABLEV2))
	{
		::MessageBox(NULL, "Cannot delete sound in this format", "Cannot delete", NULL);
		return;
	}
	ALSound** newSounds = new ALSound*[alBank->inst[instrument]->soundCount - 1];
	for (int x = 0; x < sound; x++)
	{
		newSounds[x] = alBank->inst[instrument]->sounds[x];
	}

	for (int x = (sound+1); x < alBank->inst[instrument]->soundCount; x++)
	{
		newSounds[x-1] = alBank->inst[instrument]->sounds[x];
	}

	if (alBank->inst[instrument]->sounds[sound]->wav.type == AL_ADPCM_WAVE)
	{
		if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave != NULL)
		{
			if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop != NULL)
			{
				delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop;
			}
			if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book != NULL)
			{
				delete [] alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors;
				delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book;
			}
			delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave;
		}
	}
	else if (alBank->inst[instrument]->sounds[sound]->wav.type == AL_RAW16_WAVE)
	{
		if (alBank->inst[instrument]->sounds[sound]->wav.rawWave != NULL)
		{
			if (alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop != NULL)
			{
				delete alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop;
			}
			delete alBank->inst[instrument]->sounds[sound]->wav.rawWave;
		}
	}
	else if (alBank->inst[instrument]->sounds[sound]->wav.type == AL_MUSYX_WAVE)
	{
		if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave != NULL)
		{
			if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop != NULL)
			{
				delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop;
			}
			if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book != NULL)
			{
				delete [] alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors;
				delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book;
			}
			delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave;
		}
	}

	if (alBank->inst[instrument]->sounds[sound]->wav.wavData != NULL)
		delete [] alBank->inst[instrument]->sounds[sound]->wav.wavData;

	delete alBank->inst[instrument]->sounds[sound];

	delete [] alBank->inst[instrument]->sounds;
	alBank->inst[instrument]->sounds = newSounds;

	alBank->inst[instrument]->soundCount--;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::DeleteSfx(ALBank*& alBank, int sound)
{
	if (alBank->soundBankFormat != ZELDAFORMAT)
	{
		::MessageBox(NULL, "Cannot delete sound in this format", "Cannot delete", NULL);
		return;
	}

	if (alBank->alSfx[sound]->type == AL_ADPCM_WAVE)
	{
		if (alBank->alSfx[sound]->adpcmWave != NULL)
		{
			if (alBank->alSfx[sound]->adpcmWave->loop != NULL)
			{
				delete alBank->alSfx[sound]->adpcmWave->loop;
			}
			if (alBank->alSfx[sound]->adpcmWave->book != NULL)
			{
				delete [] alBank->alSfx[sound]->adpcmWave->book->predictors;
				delete alBank->alSfx[sound]->adpcmWave->book;
			}
			delete alBank->alSfx[sound]->adpcmWave;
		}
	}
	else if (alBank->alSfx[sound]->type == AL_RAW16_WAVE)
	{
		if (alBank->alSfx[sound]->rawWave != NULL)
		{
			if (alBank->alSfx[sound]->rawWave->loop != NULL)
			{
				delete alBank->alSfx[sound]->rawWave->loop;
			}
			delete alBank->alSfx[sound]->rawWave;
		}
	}
	else if (alBank->alSfx[sound]->type == AL_MUSYX_WAVE)
	{
		if (alBank->alSfx[sound]->adpcmWave != NULL)
		{
			if (alBank->alSfx[sound]->adpcmWave->loop != NULL)
			{
				delete alBank->alSfx[sound]->adpcmWave->loop;
			}
			if (alBank->alSfx[sound]->adpcmWave->book != NULL)
			{
				delete [] alBank->alSfx[sound]->adpcmWave->book->predictors;
				delete alBank->alSfx[sound]->adpcmWave->book;
			}
			delete alBank->alSfx[sound]->adpcmWave;
		}
	}

	if (alBank->alSfx[sound]->wavData != NULL)
		delete [] alBank->alSfx[sound]->wavData;

	delete alBank->alSfx[sound];

	if (alBank->countSfx == 1)
	{
		delete [] alBank->alSfx;
		alBank->alSfx = NULL;
	}
	else
	{
		ALWave** newSounds = new ALWave*[alBank->countSfx - 1];
		for (int x = 0; x < sound; x++)
		{
			newSounds[x] = alBank->alSfx[x];
		}

		for (int x = (sound+1); x < alBank->countSfx; x++)
		{
			newSounds[x-1] = alBank->alSfx[x];
		}

		if (alBank->alSfx != NULL)
			delete [] alBank->alSfx;

		alBank->alSfx = newSounds;
	}

	alBank->countSfx--;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::DeletePercussion(ALBank*& alBank, int sound)
{
	if ((alBank->soundBankFormat != STANDARDFORMAT) && (alBank->soundBankFormat != STANDARDFORMATRAWALLOWED))
	{
		::MessageBox(NULL, "Cannot delete sound in this format", "Cannot delete", NULL);
		return;
	}

	if (alBank->percussion->sounds[sound]->wav.type == AL_ADPCM_WAVE)
	{
		if (alBank->percussion->sounds[sound]->wav.adpcmWave != NULL)
		{
			if (alBank->percussion->sounds[sound]->wav.adpcmWave->loop != NULL)
			{
				delete alBank->percussion->sounds[sound]->wav.adpcmWave->loop;
			}
			if (alBank->percussion->sounds[sound]->wav.adpcmWave->book != NULL)
			{
				delete [] alBank->percussion->sounds[sound]->wav.adpcmWave->book->predictors;
				delete alBank->percussion->sounds[sound]->wav.adpcmWave->book;
			}
			delete alBank->percussion->sounds[sound]->wav.adpcmWave;
		}
	}
	else if (alBank->percussion->sounds[sound]->wav.type == AL_RAW16_WAVE)
	{
		if (alBank->percussion->sounds[sound]->wav.rawWave != NULL)
		{
			if (alBank->percussion->sounds[sound]->wav.rawWave->loop != NULL)
			{
				delete alBank->percussion->sounds[sound]->wav.rawWave->loop;
			}
			delete alBank->percussion->sounds[sound]->wav.rawWave;
		}
	}
	else if (alBank->percussion->sounds[sound]->wav.type == AL_MUSYX_WAVE)
	{
		if (alBank->percussion->sounds[sound]->wav.adpcmWave != NULL)
		{
			if (alBank->percussion->sounds[sound]->wav.adpcmWave->loop != NULL)
			{
				delete alBank->percussion->sounds[sound]->wav.adpcmWave->loop;
			}
			if (alBank->percussion->sounds[sound]->wav.adpcmWave->book != NULL)
			{
				delete [] alBank->percussion->sounds[sound]->wav.adpcmWave->book->predictors;
				delete alBank->percussion->sounds[sound]->wav.adpcmWave->book;
			}
			delete alBank->percussion->sounds[sound]->wav.adpcmWave;
		}
	}

	if (alBank->percussion->sounds[sound]->wav.wavData != NULL)
		delete [] alBank->percussion->sounds[sound]->wav.wavData;

	delete alBank->percussion->sounds[sound];

	if (alBank->percussion->soundCount == 1)
	{
		delete [] alBank->percussion->sounds;
		alBank->percussion->sounds = NULL;
	}
	else
	{
		ALSound** newSounds = new ALSound*[alBank->percussion->soundCount - 1];
		for (int x = 0; x < sound; x++)
		{
			newSounds[x] = alBank->percussion->sounds[x];
		}

		for (int x = (sound+1); x < alBank->percussion->soundCount; x++)
		{
			newSounds[x-1] = alBank->percussion->sounds[x];
		}

		if (alBank->percussion->sounds != NULL)
			delete [] alBank->percussion->sounds;

		alBank->percussion->sounds = newSounds;
	}

	alBank->percussion->soundCount--;

	UpdateAudioOffsets(alBank);
}
void CN64AIFCAudio::DeleteEADPercussion(ALBank*& alBank, int sound)
{
	if ((alBank->soundBankFormat != ZELDAFORMAT) && (alBank->soundBankFormat != SUPERMARIO64FORMAT)  && (alBank->soundBankFormat != STARFOX64FORMAT))
	{
		::MessageBox(NULL, "Cannot delete sound in this format", "Cannot delete", NULL);
		return;
	}

	if (alBank->eadPercussion[sound].wav.type == AL_ADPCM_WAVE)
	{
		if (alBank->eadPercussion[sound].wav.adpcmWave != NULL)
		{
			if (alBank->eadPercussion[sound].wav.adpcmWave->loop != NULL)
			{
				delete alBank->eadPercussion[sound].wav.adpcmWave->loop;
			}
			if (alBank->eadPercussion[sound].wav.adpcmWave->book != NULL)
			{
				delete [] alBank->eadPercussion[sound].wav.adpcmWave->book->predictors;
				delete alBank->eadPercussion[sound].wav.adpcmWave->book;
			}
			delete alBank->eadPercussion[sound].wav.adpcmWave;
		}
	}
	else if (alBank->eadPercussion[sound].wav.type == AL_RAW16_WAVE)
	{
		if (alBank->eadPercussion[sound].wav.rawWave != NULL)
		{
			if (alBank->eadPercussion[sound].wav.rawWave->loop != NULL)
			{
				delete alBank->eadPercussion[sound].wav.rawWave->loop;
			}
			delete alBank->eadPercussion[sound].wav.rawWave;
		}
	}
	else if (alBank->eadPercussion[sound].wav.type == AL_MUSYX_WAVE)
	{
		if (alBank->eadPercussion[sound].wav.adpcmWave != NULL)
		{
			if (alBank->eadPercussion[sound].wav.adpcmWave->loop != NULL)
			{
				delete alBank->eadPercussion[sound].wav.adpcmWave->loop;
			}
			if (alBank->eadPercussion[sound].wav.adpcmWave->book != NULL)
			{
				delete [] alBank->eadPercussion[sound].wav.adpcmWave->book->predictors;
				delete alBank->eadPercussion[sound].wav.adpcmWave->book;
			}
			delete alBank->eadPercussion[sound].wav.adpcmWave;
		}
	}

	if (alBank->eadPercussion[sound].wav.wavData != NULL)
		delete [] alBank->eadPercussion[sound].wav.wavData;

	if (alBank->countEADPercussion == 1)
	{
		delete [] alBank->eadPercussion;
		alBank->eadPercussion = NULL;
	}
	else
	{
		EADPercussion* newSounds = new EADPercussion[alBank->countEADPercussion - 1];
		for (int x = 0; x < sound; x++)
		{
			newSounds[x] = alBank->eadPercussion[x];
		}

		for (int x = (sound+1); x < alBank->countEADPercussion; x++)
		{
			newSounds[x-1] = alBank->eadPercussion[x];
		}

		delete [] alBank->eadPercussion;
		alBank->eadPercussion = newSounds;
	}

	alBank->countEADPercussion--;

	UpdateAudioOffsets(alBank);
}
void CN64AIFCAudio::MoveUpSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == 0)
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound-1];
	alBank->inst[instrument]->sounds[sound-1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::MoveUpPercussion(ALBank*& alBank, int sound)
{
	if (sound == 0)
		return;

	ALSound* soundAl = alBank->percussion->sounds[sound-1];
	alBank->percussion->sounds[sound-1] = alBank->percussion->sounds[sound];
	alBank->percussion->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	unsigned long tempLong = Flip32Bit(alWave->adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alWave->adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alWave->adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ExportPercussionPredictors(ALBank*& alBank, int sound, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

	unsigned long tempLong = Flip32Bit(alWave->adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alWave->adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alWave->adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ExportEADPercussionPredictors(ALBank*& alBank, int percussion, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->eadPercussion[percussion].wav;

	unsigned long tempLong = Flip32Bit(alWave->adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alWave->adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alWave->adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ExportSfxPredictors(ALBank*& alBank, int sfx, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = alBank->alSfx[sfx];

	unsigned long tempLong = Flip32Bit(alWave->adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alWave->adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alWave->adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	if (fileSize < 0x8)
	{
		MessageBox(NULL, "Error too small file", "Error", NULL);
		fclose(inFile);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	
	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	alWave->adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alWave->adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alWave->adpcmWave->book->predictors;
	alWave->adpcmWave->book->predictors = new signed short[(alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		alWave->adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ImportPercussionPredictors(ALBank*& alBank, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	if (fileSize < 0x8)
	{
		MessageBox(NULL, "Error too small file", "Error", NULL);
		fclose(inFile);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	
	ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

	alWave->adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alWave->adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alWave->adpcmWave->book->predictors;
	alWave->adpcmWave->book->predictors = new signed short[(alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		alWave->adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ImportEADPercussionPredictors(ALBank*& alBank, int percussion, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	if (fileSize < 0x8)
	{
		MessageBox(NULL, "Error too small file", "Error", NULL);
		fclose(inFile);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	
	ALWave* alWave = &alBank->eadPercussion[percussion].wav;

	alWave->adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alWave->adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alWave->adpcmWave->book->predictors;
	alWave->adpcmWave->book->predictors = new signed short[(alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		alWave->adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ImportSfxPredictors(ALBank*& alBank, int sfx, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	if (fileSize < 0x8)
	{
		MessageBox(NULL, "Error too small file", "Error", NULL);
		fclose(inFile);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	
	ALWave* alWave = alBank->alSfx[sfx];

	alWave->adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alWave->adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alWave->adpcmWave->book->predictors;
	alWave->adpcmWave->book->predictors = new signed short[(alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		alWave->adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::UpdateAudioOffsets(ALBank* alBank)
{
	std::vector<ALBank*> alBanks;
	alBanks.push_back(alBank);
	UpdateAudioOffsets(alBanks);
}

void CN64AIFCAudio::UpdateAudioOffsets(std::vector<ALBank*> alBanks)
{
	if (alBanks.size() > 0)
	{
		ALBank* alBank = alBanks[0];

		unsigned char* ctl = NULL;
		unsigned char* tbl = NULL;
		int ctlCounter, tblCounter;
		// update base offsets
		if (alBank->soundBankFormat == STANDARDFORMAT)
		{
			// Not sure why was using this...but messes up the multi-banks here so just don't do it for now
			return;

			WriteAudio(alBanks, ctl, ctlCounter, tbl, tblCounter);
		}
		else if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
		{
			//WriteAudioSuperMario(alBank, ctl, ctlCounter, tbl, tblCounter);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2)
		{
			WriteAudioN64PtrWavetableV2(alBank, ctl, ctlCounter, tbl, tblCounter);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
		{
			WriteAudioN64PtrWavetableV2Blitz("", alBank, ctl, ctlCounter, tbl, tblCounter);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV1)
		{
			WriteAudioN64PtrWavetableV1(alBank, ctl, ctlCounter, tbl, tblCounter);
		}
		if (ctl != NULL)
			delete [] ctl;
		if (tbl != NULL)
			delete [] tbl;
	}
}

void CN64AIFCAudio::ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	fwrite(alWave->wavData, 1, alWave->len, outFile);

	fclose(outFile);
}

void CN64AIFCAudio::ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	alWave->len = fileSize;
	delete [] alWave->wavData;

	alWave->wavData = new unsigned char[fileSize];
	fread(alWave->wavData, 1, fileSize, inFile);
	fclose(inFile);

	if ((alBank->soundBankFormat == SUPERMARIO64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == STARFOX64FORMAT))
	{
		alWave->adpcmWave->loop->start = 0;
		alWave->adpcmWave->loop->end = (fileSize * 7) / 4;
		alWave->adpcmWave->loop->count = 0;
	}

	UpdateAudioOffsets(alBank);
}


void CN64AIFCAudio::ExportPercussionRawData(ALBank*& alBank, int sound, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

	fwrite(alWave->wavData, 1, alWave->len, outFile);

	fclose(outFile);
}

void CN64AIFCAudio::ImportPercussionRawData(ALBank*& alBank, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

	alWave->len = fileSize;
	delete [] alWave->wavData;

	alWave->wavData = new unsigned char[fileSize];
	fread(alWave->wavData, 1, fileSize, inFile);
	fclose(inFile);

	if ((alBank->soundBankFormat == SUPERMARIO64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == STARFOX64FORMAT))
	{
		alWave->adpcmWave->loop->start = 0;
		alWave->adpcmWave->loop->end = (fileSize * 7) / 4;
		alWave->adpcmWave->loop->count = 0;
	}

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ExportEADRawPercussionData(ALBank*& alBank, int sound, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->eadPercussion[sound].wav;

	fwrite(alWave->wavData, 1, alWave->len, outFile);

	fclose(outFile);
}

void CN64AIFCAudio::ImportEADRawPercussionData(ALBank*& alBank, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	ALWave* alWave = &alBank->eadPercussion[sound].wav;
	
	alWave->len = fileSize;
	delete [] alWave->wavData;

	alWave->wavData = new unsigned char[fileSize];
	fread(alWave->wavData, 1, fileSize, inFile);
	fclose(inFile);

	if ((alBank->soundBankFormat == SUPERMARIO64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == STARFOX64FORMAT))
	{
		alWave->adpcmWave->loop->start = 0;
		alWave->adpcmWave->loop->end = (fileSize * 7) / 4;
		alWave->adpcmWave->loop->count = 0;
	}

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ExportRawSfxData(ALBank*& alBank, int sound, CString fileName)
{
	ALWave* alWave = alBank->alSfx[sound];

	if (alWave != NULL)
	{

		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error opening file", "Error", NULL);
			return;
		}

		fwrite(alWave->wavData, 1, alWave->len, outFile);

		fclose(outFile);
	}
	else
	{
		MessageBox(NULL, "Nothing to export", "Error", NULL);
		return;
	}
}

void CN64AIFCAudio::ImportRawSfxData(ALBank*& alBank, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	ALWave* alWave = alBank->alSfx[sound];
	
	alWave->len = fileSize;
	delete [] alWave->wavData;

	alWave->wavData = new unsigned char[fileSize];
	fread(alWave->wavData, 1, fileSize, inFile);
	fclose(inFile);

	if ((alBank->soundBankFormat == SUPERMARIO64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == STARFOX64FORMAT))
	{
		alWave->adpcmWave->loop->start = 0;
		alWave->adpcmWave->loop->end = (fileSize * 7) / 4;
		alWave->adpcmWave->loop->count = 0;
	}

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::MoveDownSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == (alBank->inst[instrument]->soundCount-1))
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound+1];
	alBank->inst[instrument]->sounds[sound+1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::MoveDownPercussion(ALBank*& alBank, int sound)
{
	if (sound == (alBank->percussion->soundCount-1))
		return;

	ALSound* soundAl = alBank->percussion->sounds[sound+1];
	alBank->percussion->sounds[sound+1] = alBank->percussion->sounds[sound];
	alBank->percussion->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

bool CN64AIFCAudio::ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate, bool& hasLoopData, unsigned char& keyBase, unsigned long& loopStart, unsigned long& loopEnd, unsigned long& loopCount)
{
	FILE* inWavFile = fopen(rawWavFileName, "rb");
	if (inWavFile == NULL)
	{
		MessageBox(NULL, "Error cannot read wav file", "Error", NULL);
		return false;
	}

	fseek(inWavFile, 0, SEEK_END);
	int fileSize = ftell(inWavFile);
	rewind(inWavFile);

	unsigned char* wavData = new unsigned char[fileSize];
	fread(wavData, 1, fileSize, inWavFile);
	fclose(inWavFile);

	if (((((((wavData[0] << 8) | wavData[1]) << 8) | wavData[2]) << 8) | wavData[3]) != 0x52494646)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not RIFF wav", "Error", NULL);
		return false;
	}

	if (((((((wavData[0x8] << 8) | wavData[0x9]) << 8) | wavData[0xA]) << 8) | wavData[0xB]) != 0x57415645)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not WAVE wav", "Error", NULL);
		return false;
	}

	bool endFlag = false;

	unsigned long currentOffset = 0xC;

	unsigned short channels = 0;
	samplingRate = 0;
	unsigned short bitRate = 0;

	bool returnFlag = false;

	while (!endFlag)
	{
		if (currentOffset >= (fileSize - 8))
			break;

		unsigned long sectionType = ((((((wavData[currentOffset] << 8) | wavData[currentOffset + 1]) << 8) | wavData[currentOffset + 2]) << 8) | wavData[currentOffset + 3]);

		if (sectionType == 0x666D7420) // fmt
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			channels = ((wavData[currentOffset + 0xB] << 8) | wavData[currentOffset + 0xA]);

			if (channels != 0x0001)
			{
				MessageBox(NULL, "Warning: Only mono wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			samplingRate = ((((((wavData[currentOffset + 0xF] << 8) | wavData[currentOffset + 0xE]) << 8) | wavData[currentOffset + 0xD]) << 8) | wavData[currentOffset + 0xC]);
			
			bitRate = ((wavData[currentOffset + 0x17] << 8) | wavData[currentOffset + 0x16]);

			currentOffset += chunkSize + 8;
		}
		else if (sectionType == 0x64617461) // data
		{
			rawLength = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			if ((channels == 0) || (samplingRate == 0) || (bitRate == 0))
			{
				MessageBox(NULL, "Incorrect section type (missing fmt)", "Error unknown wav format", NULL);
				endFlag = true;
				returnFlag = false;
			}

			if (bitRate == 0x0010)
			{
				rawData = new unsigned char[rawLength];
				for (int x = 0; x < rawLength; x++)
				{
					rawData[x] = wavData[currentOffset + 0x8 + x];
				}
			
				returnFlag = true;
			}
			else
			{
				MessageBox(NULL, "Error only 16-bit PCM wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			currentOffset += rawLength + 8;
		}
		else if (sectionType == 0x736D706C) // smpl
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			unsigned long numSampleBlocks = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x24]));
			if (numSampleBlocks > 0)
			{
				hasLoopData = true;

				keyBase = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x14])) & 0xFF;
				loopStart = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x34]));
				loopEnd = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x38]));
				loopCount = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x40]));
				if (loopCount == 0)
					loopCount = 0xFFFFFFFF;
			}

			currentOffset += 8 + chunkSize;
		}
		else
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			currentOffset += 8 + chunkSize;
		}
	}

	delete [] wavData;
	return returnFlag;
}

bool CN64AIFCAudio::ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, byte primSel, bool decode8Only, bool samePred)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (alBank->inst[instrument]->sounds[sound] == NULL)
		return false;

	bool hasLoopData = false;
	unsigned char keyBase = 0x3C;
	unsigned long loopStart = 0x00000000;
	unsigned long loopEnd = 0x00000000;
	unsigned long loopCount = 0x00000000;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
	{
		return false;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	ALADPCMBook* samePredValue = NULL;

	if (alWave->type == AL_ADPCM_WAVE)
	{
		if (samePred)
		{
			if ((alWave->adpcmWave->book == NULL) || (alWave->adpcmWave->book->predictors == NULL))
			{
				delete [] wavPCMData;
				MessageBox(NULL, "Can't use same predictors since none exist", "Error", NULL);
				return false;
			}

			samePredValue = new ALADPCMBook();
			samePredValue->npredictors = alWave->adpcmWave->book->npredictors;
			samePredValue->order = alWave->adpcmWave->book->order;
			samePredValue->predictors = new short[(samePredValue->order * samePredValue->npredictors * 8)];
			for (int z = 0; z < (samePredValue->order * samePredValue->npredictors * 8); z++)
			{
				samePredValue->predictors[z] = alWave->adpcmWave->book->predictors[z];
			}
		}

		if (alWave->adpcmWave->loop != NULL)
		{
			delete alWave->adpcmWave->loop;
			alWave->adpcmWave->loop = NULL;
		}

		if (alWave->adpcmWave->book != NULL)
		{
			if (alWave->adpcmWave->book->predictors != NULL)
			{
				delete [] alWave->adpcmWave->book->predictors;
				alWave->adpcmWave->book->predictors = NULL;
			}
			delete alWave->adpcmWave->book;
			alWave->adpcmWave->book = NULL;
		}

		delete alWave->adpcmWave->book;
		delete alWave->adpcmWave;
		alWave->adpcmWave = NULL;
		alWave->rawWave = NULL;
	}
	else if (alWave->type == AL_RAW16_WAVE)
	{
		if (alWave->rawWave->loop != NULL)
		{
			delete alWave->rawWave->loop;
			alWave->rawWave->loop = NULL;
		}
		delete alWave->rawWave;
		alWave->rawWave = NULL;
		alWave->adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alWave->wavFlags = (alWave->wavFlags & (0xFF ^ 0x30));

		alWave->type = AL_RAW16_WAVE;
		alWave->rawWave = new ALRAWWaveInfo();
		alWave->rawWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength);
		alWave->rawWave->loop->count = 0;*/

		delete [] alWave->wavData;
		alWave->wavData = new unsigned char[rawLength];

		for (int x = 0; x < rawLength; x+=2)
		{
			alWave->wavData[x] = wavPCMData[x+1];
			alWave->wavData[x+1] = wavPCMData[x];
		}
		alWave->len = rawLength;

		alBank->inst[instrument]->sounds[sound]->flags = 0;
		/*alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
		alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F;
		alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0x7F;
		alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
		
		alBank->inst[instrument]->sounds[sound]->key.detune = 0;*/
		if (hasLoopData)
			alBank->inst[instrument]->sounds[sound]->key.keybase = keyBase;
		/*else
			alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
		alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
		alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
		alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
		alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;*/
		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->rawWave->loop = new ALRawLoop();
			alWave->rawWave->loop->count = loopCount;
			alWave->rawWave->loop->start = loopStart;
			alWave->rawWave->loop->end = loopEnd;
		}
	}
	else
	{
		alWave->wavFlags = (alWave->wavFlags & (0xFF ^ 0x30)) | ((int)decode8Only * 0x30);

		alWave->type = AL_ADPCM_WAVE;
		alWave->adpcmWave = new ALADPCMWaveInfo();
		alWave->adpcmWave->loop = NULL;

		alWave->adpcmWave->book = new ALADPCMBook();

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		if (!samePred)
		{
			if (decode8Only)
			{
				alWave->adpcmWave->book->predictors = new signed short[0x10];
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->book->predictors[x] = 0x00;

				alWave->adpcmWave->book->npredictors = 1;
				alWave->adpcmWave->book->order = 2;
			}
			else
			{
				alWave->adpcmWave->book->predictors = determineBestPredictors(alBank, alWave->adpcmWave->book->npredictors, alWave->adpcmWave->book->order, pcmSamples, numberSamples);
			}
		}
		else
		{
			alWave->adpcmWave->book = samePredValue;
		}

		delete [] alWave->wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		if (decode8Only)
		{
			encode_half(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}
		else
		{
			encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}

		int vadpcmOutputLengthPad = vadpcmOutputLength;
		if ((vadpcmOutputLengthPad % 2) == 1)
			vadpcmOutputLengthPad++;

		alWave->wavData = new unsigned char[vadpcmOutputLengthPad];
		for (int x = 0; x < vadpcmOutputLengthPad; x++)
			alWave->wavData[x] = 0x00;

		memcpy(alWave->wavData, vadpcmData, vadpcmOutputLength);
		alWave->len = vadpcmOutputLengthPad;

		if ((alBank->soundBankFormat == SUPERMARIO64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == STARFOX64FORMAT))
		{
			alWave->adpcmWave->loop = new ALADPCMloop();
			alWave->adpcmWave->loop->start = 0;
			alWave->adpcmWave->loop->end = ((vadpcmOutputLength * 7) / 4);
			alWave->adpcmWave->loop->count = 0;

			if (hasLoopData)
			{
				if (primSel == PRIMARY)
				{
					alBank->inst[instrument]->sounds[sound]->floatKeyBase = *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[keyBase]);
				}
				else if (primSel == PREVIOUS)
				{
					alBank->inst[instrument]->sounds[sound]->floatKeyBasePrev = *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[keyBase]);
				}
				else if (primSel == SECONDARY)
				{
					alBank->inst[instrument]->sounds[sound]->floatKeyBaseSec = *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[keyBase]);
				}
			}
		}
		else
		{
			alBank->inst[instrument]->sounds[sound]->flags = 0;
			/*alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
			alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F;
			alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
			alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0x7F;
			alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
			alBank->inst[instrument]->sounds[sound]->key.detune = 0;*/
			if (hasLoopData)
				alBank->inst[instrument]->sounds[sound]->key.keybase = keyBase;
			/*else
				alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
			alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
			alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
			alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
			alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
			alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
			alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;*/
			alWave->flags = 0;

			if (hasLoopData)
			{
				alWave->adpcmWave->loop = new ALADPCMloop();
				alWave->adpcmWave->loop->count = loopCount;
				alWave->adpcmWave->loop->start = loopStart;
				alWave->adpcmWave->loop->end = loopEnd;
				alWave->adpcmWave->loop->unknown1 = 0;
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];

			}
		}

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	UpdateAudioOffsets(alBank);

	return true;
}

bool CN64AIFCAudio::ReplacePercussionWithWavData(ALBank*& alBank, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool samePred)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (alBank->percussion == NULL)
		return false;

	bool hasLoopData = false;
	unsigned char keyBase = 0x3C;
	unsigned long loopStart = 0x00000000;
	unsigned long loopEnd = 0x00000000;
	unsigned long loopCount = 0x00000000;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
	{
		return false;
	}

	ALWave* alWave = &alBank->percussion->sounds[sound]->wav;

	ALADPCMBook* samePredValue = NULL;

	if (alWave->type == AL_ADPCM_WAVE)
	{
		if (alWave->adpcmWave->loop != NULL)
		{
			delete alWave->adpcmWave->loop;
			alWave->adpcmWave->loop = NULL;
		}

		if (samePred)
		{
			if ((alWave->adpcmWave->book == NULL) || (alWave->adpcmWave->book->predictors == NULL))
			{
				delete [] wavPCMData;
				MessageBox(NULL, "Can't use same predictors since none exist", "Error", NULL);
				return false;
			}

			samePredValue = new ALADPCMBook();
			samePredValue->npredictors = alWave->adpcmWave->book->npredictors;
			samePredValue->order = alWave->adpcmWave->book->order;
			samePredValue->predictors = new short[(samePredValue->order * samePredValue->npredictors * 8)];
			for (int z = 0; z < (samePredValue->order * samePredValue->npredictors * 8); z++)
			{
				samePredValue->predictors[z] = alWave->adpcmWave->book->predictors[z];
			}
		}

		if (alWave->adpcmWave->book != NULL)
		{
			if (alWave->adpcmWave->book->predictors != NULL)
			{
				delete [] alWave->adpcmWave->book->predictors;
				alWave->adpcmWave->book->predictors = NULL;
			}
			delete alWave->adpcmWave->book;
			alWave->adpcmWave->book = NULL;
		}

		delete alWave->adpcmWave->book;
		delete alWave->adpcmWave;
		alWave->adpcmWave = NULL;
		alWave->rawWave = NULL;
	}
	else if (alWave->type == AL_RAW16_WAVE)
	{
		if (alWave->rawWave->loop != NULL)
		{
			delete alWave->rawWave->loop;
			alWave->rawWave->loop = NULL;
		}
		delete alWave->rawWave;
		alWave->rawWave = NULL;
		alWave->adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alWave->type = AL_RAW16_WAVE;
		alWave->rawWave = new ALRAWWaveInfo();
		alWave->rawWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		delete [] alWave->wavData;
		alWave->wavData = new unsigned char[rawLength];

		for (int x = 0; x < rawLength; x+=2)
		{
			alWave->wavData[x] = wavPCMData[x+1];
			alWave->wavData[x+1] = wavPCMData[x];
		}
		alWave->len = rawLength;

		alBank->percussion->sounds[sound]->flags = 0;
		/*alBank->percussion->sounds[sound]->env.attackTime = 0;
		alBank->percussion->sounds[sound]->env.attackVolume = 0x7F;
		alBank->percussion->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->percussion->sounds[sound]->env.decayVolume = 0x7F;
		alBank->percussion->sounds[sound]->env.releaseTime = 0;
		
		alBank->percussion->sounds[sound]->key.detune = 0;*/
		if (hasLoopData)
			alBank->percussion->sounds[sound]->key.keybase = keyBase;
		/*else
			alBank->percussion->sounds[sound]->key.keybase = 0x3C;
		alBank->percussion->sounds[sound]->key.keymax = 0x7F;
		alBank->percussion->sounds[sound]->key.keymin = 0;
		alBank->percussion->sounds[sound]->key.velocitymax = 0x7F;
		alBank->percussion->sounds[sound]->key.velocitymin = 0;
		alBank->percussion->sounds[sound]->samplePan = 0x40;
		alBank->percussion->sounds[sound]->sampleVolume = 0x7F;*/
		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->rawWave->loop = new ALRawLoop();
			alWave->rawWave->loop->count = loopCount;
			alWave->rawWave->loop->start = loopStart;
			alWave->rawWave->loop->end = loopEnd;
		}
	}
	else
	{
		alWave->type = AL_ADPCM_WAVE;
		alWave->adpcmWave = new ALADPCMWaveInfo();
		alWave->adpcmWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		alWave->adpcmWave->book = new ALADPCMBook();

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		if (!samePred)
		{
			alWave->adpcmWave->book->predictors = determineBestPredictors(alBank, alWave->adpcmWave->book->npredictors, alWave->adpcmWave->book->order, pcmSamples, numberSamples);
		}
		else
		{
			alWave->adpcmWave->book = samePredValue;
		}

		delete [] alWave->wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);

		int vadpcmOutputLengthPad = vadpcmOutputLength;
		if ((vadpcmOutputLengthPad % 2) == 1)
			vadpcmOutputLengthPad++;

		alWave->wavData = new unsigned char[vadpcmOutputLengthPad];
		for (int x = 0; x < vadpcmOutputLengthPad; x++)
			alWave->wavData[x] = 0x00;

		memcpy(alWave->wavData, vadpcmData, vadpcmOutputLength);
		alWave->len = vadpcmOutputLengthPad;

		alBank->percussion->sounds[sound]->flags = 0;
		/*alBank->percussion->sounds[sound]->env.attackTime = 0;
		alBank->percussion->sounds[sound]->env.attackVolume = 0x7F;
		alBank->percussion->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->percussion->sounds[sound]->env.decayVolume = 0x7F;
		alBank->percussion->sounds[sound]->env.releaseTime = 0;
		alBank->percussion->sounds[sound]->key.detune = 0;*/
		if (hasLoopData)
			alBank->percussion->sounds[sound]->key.keybase = keyBase;
		/*else
			alBank->percussion->sounds[sound]->key.keybase = 0x3C;
		alBank->percussion->sounds[sound]->key.keymax = 0x7F;
		alBank->percussion->sounds[sound]->key.keymin = 0;
		alBank->percussion->sounds[sound]->key.velocitymax = 0x7F;
		alBank->percussion->sounds[sound]->key.velocitymin = 0;
		alBank->percussion->sounds[sound]->samplePan = 0x40;
		alBank->percussion->sounds[sound]->sampleVolume = 0x7F;*/
		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->adpcmWave->loop = new ALADPCMloop();
			alWave->adpcmWave->loop->count = loopCount;
			alWave->adpcmWave->loop->start = loopStart;
			alWave->adpcmWave->loop->end = loopEnd;
			alWave->adpcmWave->loop->unknown1 = 0;
			for (int x = 0; x < 0x10; x++)
				alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];

		}

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	UpdateAudioOffsets(alBank);

	return true;
}

bool CN64AIFCAudio::ReplaceEADPercussionWithWavData(ALBank*& alBank, int percussion, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool decode8Only, bool samePred)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (alBank->eadPercussion == NULL)
		return false;

	bool hasLoopData = false;
	unsigned char keyBase = 0x3C;
	unsigned long loopStart = 0x00000000;
	unsigned long loopEnd = 0x00000000;
	unsigned long loopCount = 0x00000000;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
	{
		return false;
	}

	ALWave* alWave = NULL;
	alWave = &alBank->eadPercussion[percussion].wav;

	ALADPCMBook* samePredValue = NULL;

	if (alWave->type == AL_ADPCM_WAVE)
	{
		if (alWave->adpcmWave->loop != NULL)
		{
			delete alWave->adpcmWave->loop;
			alWave->adpcmWave->loop = NULL;
		}

		if (samePred)
		{
			if ((alWave->adpcmWave->book == NULL) || (alWave->adpcmWave->book->predictors == NULL))
			{
				delete [] wavPCMData;
				MessageBox(NULL, "Can't use same predictors since none exist", "Error", NULL);
				return false;
			}

			samePredValue = new ALADPCMBook();
			samePredValue->npredictors = alWave->adpcmWave->book->npredictors;
			samePredValue->order = alWave->adpcmWave->book->order;
			samePredValue->predictors = new short[(samePredValue->order * samePredValue->npredictors * 8)];
			for (int z = 0; z < (samePredValue->order * samePredValue->npredictors * 8); z++)
			{
				samePredValue->predictors[z] = alWave->adpcmWave->book->predictors[z];
			}
		}

		if (alWave->adpcmWave->book != NULL)
		{
			if (alWave->adpcmWave->book->predictors != NULL)
			{
				delete [] alWave->adpcmWave->book->predictors;
				alWave->adpcmWave->book->predictors = NULL;
			}
			delete alWave->adpcmWave->book;
			alWave->adpcmWave->book = NULL;
		}

		delete alWave->adpcmWave->book;
		delete alWave->adpcmWave;
		alWave->adpcmWave = NULL;
		alWave->rawWave = NULL;
	}
	else if (alWave->type == AL_RAW16_WAVE)
	{
		if (alWave->rawWave->loop != NULL)
		{
			delete alWave->rawWave->loop;
			alWave->rawWave->loop = NULL;
		}
		delete alWave->rawWave;
		alWave->rawWave = NULL;
		alWave->adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alWave->type = AL_RAW16_WAVE;
		alWave->rawWave = new ALRAWWaveInfo();
		alWave->rawWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		delete [] alWave->wavData;
		alWave->wavData = new unsigned char[rawLength];

		for (int x = 0; x < rawLength; x+=2)
		{
			alWave->wavData[x] = wavPCMData[x+1];
			alWave->wavData[x+1] = wavPCMData[x];
		}
		alWave->len = rawLength;

		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->rawWave->loop = new ALRawLoop();
			alWave->rawWave->loop->count = loopCount;
			alWave->rawWave->loop->start = loopStart;
			alWave->rawWave->loop->end = loopEnd;
		}
	}
	else
	{
		alWave->type = AL_ADPCM_WAVE;
		alWave->adpcmWave = new ALADPCMWaveInfo();
		alWave->adpcmWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		alWave->adpcmWave->book = new ALADPCMBook();

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		if (!samePred)
		{
			if (decode8Only)
			{
				alWave->adpcmWave->book->predictors = new signed short[0x10];
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->book->predictors[x] = 0x00;

				alWave->adpcmWave->book->npredictors = 1;
				alWave->adpcmWave->book->order = 2;
			}
			else
			{
				alWave->adpcmWave->book->predictors = determineBestPredictors(alBank, alWave->adpcmWave->book->npredictors, alWave->adpcmWave->book->order, pcmSamples, numberSamples);
			}
		}
		else
		{
			alWave->adpcmWave->book = samePredValue;
		}

		delete [] alWave->wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		if (decode8Only)
		{
			encode_half(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}
		else
		{
			encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}

		int vadpcmOutputLengthPad = vadpcmOutputLength;
		if ((vadpcmOutputLengthPad % 2) == 1)
			vadpcmOutputLengthPad++;

		alWave->wavData = new unsigned char[vadpcmOutputLengthPad];
		for (int x = 0; x < vadpcmOutputLengthPad; x++)
			alWave->wavData[x] = 0x00;

		memcpy(alWave->wavData, vadpcmData, vadpcmOutputLength);
		alWave->len = vadpcmOutputLengthPad;

		if ((alBank->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBank->soundBankFormat == ZELDAFORMAT)
				|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
		{
			alWave->adpcmWave->loop = new ALADPCMloop();
			alWave->adpcmWave->loop->start = 0;
			alWave->adpcmWave->loop->end = ((vadpcmOutputLength * 7) / 4);
			alWave->adpcmWave->loop->count = 0;

			if (hasLoopData)
			{
				alBank->eadPercussion[percussion].keyBase = *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[keyBase]);
			}
		}
		else
		{
			alWave->flags = 0;

			if (hasLoopData)
			{
				alWave->adpcmWave->loop = new ALADPCMloop();
				alWave->adpcmWave->loop->count = loopCount;
				alWave->adpcmWave->loop->start = loopStart;
				alWave->adpcmWave->loop->end = loopEnd;
				alWave->adpcmWave->loop->unknown1 = 0;
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];
			}
		}

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	UpdateAudioOffsets(alBank);

	return true;
}

bool CN64AIFCAudio::ReplaceSfxWithWavData(ALBank*& alBank, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool decode8Only, bool samePred)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (alBank->alSfx == NULL)
		return false;

	bool hasLoopData = false;
	unsigned char keyBase = 0x3C;
	unsigned long loopStart = 0x00000000;
	unsigned long loopEnd = 0x00000000;
	unsigned long loopCount = 0x00000000;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
	{
		return false;
	}

	ALWave* alWave = alBank->alSfx[sound];

	ALADPCMBook* samePredValue = NULL;

	if (alWave->type == AL_ADPCM_WAVE)
	{
		if (alWave->adpcmWave->loop != NULL)
		{
			delete alWave->adpcmWave->loop;
			alWave->adpcmWave->loop = NULL;
		}

		if (samePred)
		{
			if ((alWave->adpcmWave->book == NULL) || (alWave->adpcmWave->book->predictors == NULL))
			{
				delete [] wavPCMData;
				MessageBox(NULL, "Can't use same predictors since none exist", "Error", NULL);
				return false;
			}

			samePredValue = new ALADPCMBook();
			samePredValue->npredictors = alWave->adpcmWave->book->npredictors;
			samePredValue->order = alWave->adpcmWave->book->order;
			samePredValue->predictors = new short[(samePredValue->order * samePredValue->npredictors * 8)];
			for (int z = 0; z < (samePredValue->order * samePredValue->npredictors * 8); z++)
			{
				samePredValue->predictors[z] = alWave->adpcmWave->book->predictors[z];
			}
		}

		if (alWave->adpcmWave->book != NULL)
		{
			if (alWave->adpcmWave->book->predictors != NULL)
			{
				delete [] alWave->adpcmWave->book->predictors;
				alWave->adpcmWave->book->predictors = NULL;
			}
			delete alWave->adpcmWave->book;
			alWave->adpcmWave->book = NULL;
		}

		delete alWave->adpcmWave->book;
		delete alWave->adpcmWave;
		alWave->adpcmWave = NULL;
		alWave->rawWave = NULL;
	}
	else if (alWave->type == AL_RAW16_WAVE)
	{
		if (alWave->rawWave->loop != NULL)
		{
			delete alWave->rawWave->loop;
			alWave->rawWave->loop = NULL;
		}
		delete alWave->rawWave;
		alWave->rawWave = NULL;
		alWave->adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alWave->type = AL_RAW16_WAVE;
		alWave->rawWave = new ALRAWWaveInfo();
		alWave->rawWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		delete [] alWave->wavData;
		alWave->wavData = new unsigned char[rawLength];

		for (int x = 0; x < rawLength; x+=2)
		{
			alWave->wavData[x] = wavPCMData[x+1];
			alWave->wavData[x+1] = wavPCMData[x];
		}
		alWave->len = rawLength;

		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->rawWave->loop = new ALRawLoop();
			alWave->rawWave->loop->count = loopCount;
			alWave->rawWave->loop->start = loopStart;
			alWave->rawWave->loop->end = loopEnd;
		}
	}
	else
	{
		alWave->type = AL_ADPCM_WAVE;
		alWave->adpcmWave = new ALADPCMWaveInfo();
		alWave->adpcmWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		alWave->adpcmWave->book = new ALADPCMBook();

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		if (!samePred)
		{
			if (decode8Only)
			{
				alWave->adpcmWave->book->predictors = new signed short[0x10];
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->book->predictors[x] = 0x00;

				alWave->adpcmWave->book->npredictors = 1;
				alWave->adpcmWave->book->order = 2;
			}
			else
			{
				alWave->adpcmWave->book->predictors = determineBestPredictors(alBank, alWave->adpcmWave->book->npredictors, alWave->adpcmWave->book->order, pcmSamples, numberSamples);
			}
		}
		else
		{
			alWave->adpcmWave->book = samePredValue;
		}

		delete [] alWave->wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		if (decode8Only)
		{
			encode_half(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}
		else
		{
			encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);
		}

		int vadpcmOutputLengthPad = vadpcmOutputLength;
		if ((vadpcmOutputLengthPad % 2) == 1)
			vadpcmOutputLengthPad++;

		alWave->wavData = new unsigned char[vadpcmOutputLengthPad];
		for (int x = 0; x < vadpcmOutputLengthPad; x++)
			alWave->wavData[x] = 0x00;

		memcpy(alWave->wavData, vadpcmData, vadpcmOutputLength);
		alWave->len = vadpcmOutputLengthPad;

		if ((alBank->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBank->soundBankFormat == ZELDAFORMAT)
				|| (alBank->soundBankFormat == STARFOX64FORMAT)
				)
		{
			alWave->adpcmWave->loop = new ALADPCMloop();
			alWave->adpcmWave->loop->start = 0;
			alWave->adpcmWave->loop->end = ((vadpcmOutputLength * 7) / 4);
			alWave->adpcmWave->loop->count = 0;

			if (hasLoopData)
			{
				alBank->alSfx[sound]->unknown1 = *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[keyBase]);
			}
		}
		else
		{
			alWave->flags = 0;

			if (hasLoopData)
			{
				alWave->adpcmWave->loop = new ALADPCMloop();
				alWave->adpcmWave->loop->count = loopCount;
				alWave->adpcmWave->loop->start = loopStart;
				alWave->adpcmWave->loop->end = loopEnd;
				alWave->adpcmWave->loop->unknown1 = 0;
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];
			}
		}

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	UpdateAudioOffsets(alBank);

	return true;
}

void CN64AIFCAudio::WriteAudioToFile(ALBank* alBank, CString outFileNameCtl, CString outFileNameTbl)
{
	std::vector<ALBank*> alBanks;
	alBanks.push_back(alBank);
	UpdateAudioOffsets(alBanks);
}

void CN64AIFCAudio::WriteAudioToFile(std::vector<ALBank*> alBanks, CString outFileNameCtl, CString outFileNameTbl)
{
	if (alBanks.size() > 0)
	{
		ALBank* alBank = alBanks[0];
		unsigned char* ctl;
		unsigned char *tbl;
		int ctlSize, tblSize;
		if (alBank->soundBankFormat == STANDARDFORMAT)
		{
			WriteAudio(alBanks, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2)
		{
			WriteAudioN64PtrWavetableV2(alBank, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
		{
			WriteAudioN64PtrWavetableV2Blitz("", alBank, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV1)
		{
			WriteAudioN64PtrWavetableV1(alBank, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
		{
			//WriteAudioSuperMario(alBank, ctl, ctlSize, tbl, tblSize);
		}
		else if ((alBank->soundBankFormat == STARFOX64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == TUROKFORMAT)  || (alBank->soundBankFormat == STANDARDRNCCOMPRESSED)|| (alBank->soundBankFormat == CLAYFIGHTER) || (alBank->soundBankFormat == STANDARDRNXCOMPRESSED) || (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2))
		{
			MessageBox(NULL, "Sorry, format not supported", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
		{
			MessageBox(NULL, "Sorry, no encoding yet for YAY0 format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == TAZHUFFMAN)
		{
			MessageBox(NULL, "Sorry, no encoding yet for huffman format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == ARMYMENFORMAT)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Army Men format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEQUAKE2)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Quake N64Wave format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLESNOWBOARDKIDS)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Snowboard Kids N64Wave format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLEVIEWPOINT)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Viewpoint 2064 N64Wave format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
		{
			MessageBox(NULL, "Sorry, no encoding yet for ZLib N64Wave format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == TITUS)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Titus format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == SYDNEY)
		{
			MessageBox(NULL, "Sorry, no encoding yet for Sydney format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == NINDEC)
		{
			WriteAudioNinDec(alBanks, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBank->soundBankFormat == MKMYTHOLOGIES)
		{
			MessageBox(NULL, "Sorry, no encoding yet for BOFS format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == ZLIBSN64)
		{
			MessageBox(NULL, "Sorry, no encoding yet for ZLIBSN64 format", "Error", NULL);
			return;
		}
		else if (alBank->soundBankFormat == SN64)
		{
			MessageBox(NULL, "Sorry, no encoding yet for SN64 format", "Error", NULL);
			return;
		}
		else
		{
			MessageBox(NULL, "Sorry, no encoding yet for format", "Error", NULL);
			return;
		}

		FILE* outFileCtl = fopen(outFileNameCtl, "wb");
		if (outFileCtl == NULL)
		{
			MessageBox(NULL, "Cannot open ctl file", "Error", NULL);
			return;
		}

		FILE* outFileTbl = fopen(outFileNameTbl, "wb");
		if (outFileTbl == NULL)
		{
			fclose(outFileCtl);
			MessageBox(NULL, "Cannot open tbl file", "Error", NULL);
			return;
		}

		fwrite(ctl, 1, ctlSize, outFileCtl);
		fwrite(tbl, 1, tblSize, outFileTbl);

		fclose(outFileCtl);
		fclose(outFileTbl);

		delete [] ctl;
		delete [] tbl;
	}
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV1(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x2000000];
	unsigned long outputCtlCounter = 0;

	for (int x = 0; x < 0x2000000; x++)
		temporaryCtlBuffer[x] = 0x00;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4E363420);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x50747254);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x61626C65);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x73202000);
	outputCtlCounter += 4;

	unsigned long outputTblCounter = 0;

	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x4E363420);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x57617665);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x5461626C);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x65732000);
	outputTblCounter += 4;

	

	unsigned long* instrumentSoundStartLookup = new unsigned long[alBank->count];
	unsigned long* bookOffsetsWav = new unsigned long[alBank->count];
	unsigned long* adpcmRawLoopOffsetsWav = new unsigned long[alBank->count];
	unsigned long* tblOffsets = new unsigned long[alBank->count];

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
	outputCtlCounter += 4;

	unsigned long startWriteInstrumentOffsets = outputCtlCounter;
	// placeholder
	outputCtlCounter += (alBank->count * 4);
	
	for (int x = 0; x < alBank->count; x++)
	{
		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		instrumentSoundStartLookup[x] = outputCtlCounter;

		int y = 0; // sound

		tblOffsets[x] = outputTblCounter;

		bool foundSameTbl = false;

		/*for (int r = 0; r < x; r++)
		{
			int z = 0;

			if (
				(alBank->inst[x]->sounds[y]->wav.base == alBank->inst[r]->sounds[z]->wav.base)
				&& (alBank->inst[x]->sounds[y]->wav.len == alBank->inst[r]->sounds[z]->wav.len)
				)
			{
				bool mismatchValues = false;
				for (int rr = 0; rr < alBank->inst[x]->sounds[y]->wav.len; rr++)
				{
					if (alBank->inst[x]->sounds[y]->wav.wavData[rr] != alBank->inst[r]->sounds[z]->wav.wavData[rr])
					{
						mismatchValues = true;
						break;
					}
				}
				if (!mismatchValues)
				{
					foundSameTbl = true;
					tblOffsets[x] = tblOffsets[r];
					break;
				}
				else
				{
					continue;
				}
			}
		}*/

		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x]);
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter + 4, alBank->inst[x]->sounds[y]->wav.len);

		if (!foundSameTbl)
		{
			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
			{
				temporaryTblBuffer[outputTblCounter + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
			}
			outputTblCounter += alBank->inst[x]->sounds[y]->wav.len;

			if ((outputTblCounter % 0x10) != 0)
			{
				int pad = 0x10 - (outputTblCounter % 0x10);
				for (int z = 0; z < pad; z++)
				{
					temporaryTblBuffer[outputTblCounter++] = 0;
				}
			}
		}

		int sameLoopBank = -1;
		int sameBank = -1;

		/*if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			int w = 0;
			for (int r = 0; r < x; r++)
			{
			
				if ((alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop != NULL))
				{
					if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->start) 
						&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->end)
						&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->count))
					{
						bool goOn = false;
						for (int ww = 0; ww < 0x10; ww++)
						{
							if (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->state[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[ww])
							{
								goOn = true;
								break;
							}
						}

						if (goOn)
							continue;

						adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[r];
						sameLoopBank = r;
						break;
					}
				}
			}
		}
		 
		

		for (int r = 0; r < x; r++)
		{
			int w = 0;

			if (alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL)
			{
				if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->order) 
					&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->npredictors))
				{
					bool goOn = false;
					for (int ww = 0; ww < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); ww++)
					{
						if (alBank->inst[r]->sounds[w]->wav.adpcmWave->book->predictors[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[ww])
						{
							goOn = true;
							break;
						}
					}

					if (goOn)
						continue;

					sameBank = r;
					break;
				}
			}
		}*/

		
		unsigned long bookWriteSpot = outputCtlCounter + 0x10;
		unsigned long loopWriteSpot = outputCtlCounter + 0xC;

		outputCtlCounter = outputCtlCounter + 0x18;

		if (sameBank == -1)
		{
			bookOffsetsWav[x] = outputCtlCounter;
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, outputCtlCounter);

			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors);
			outputCtlCounter += 4;

			for (int z = 0; z < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
				temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
			}

			
			// game does this not sure why
			/*int pad = 8;
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}*/
		}
		else
		{
			bookOffsetsWav[x] = bookOffsetsWav[sameBank];
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[sameBank]);
		}

		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			if (sameLoopBank == -1)
			{
				adpcmRawLoopOffsetsWav[x] = outputCtlCounter;
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count);
				outputCtlCounter += 4;
				for (int z = 0; z < 0x10; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
					temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
				}
			}
			else
			{
				adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[sameLoopBank];
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[sameLoopBank]);
			}
		}
		else
		{
			adpcmRawLoopOffsetsWav[x] = 0x00000000;
			// null loop
			WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x2C, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		WriteLongToBuffer(temporaryCtlBuffer, startWriteInstrumentOffsets, instrumentSoundStartLookup[x]);
		startWriteInstrumentOffsets += 4;
	}

	delete [] instrumentSoundStartLookup;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] tblOffsets;

	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
	
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV2(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x2000000];
	unsigned long outputCtlCounter = 0;

	for (int x = 0; x < 0x2000000; x++)
		temporaryCtlBuffer[x] = 0x00;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4E363420);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x50747254);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x61626C65);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x73563200);
	outputCtlCounter += 4;

	// don't name bank
	outputCtlCounter += 0x10;
	// placeholders for offsets
	outputCtlCounter += 0x10;

	unsigned long outputTblCounter = 0;

	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x4E363420);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x57617665);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x5461626C);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x65732000);
	outputTblCounter += 4;

	

	unsigned long* instrumentSoundStartLookup = new unsigned long[alBank->count];
	unsigned long* bookOffsetsWav = new unsigned long[alBank->count];
	unsigned long* adpcmRawLoopOffsetsWav = new unsigned long[alBank->count];
	unsigned long* tblOffsets = new unsigned long[alBank->count];

	WriteLongToBuffer(temporaryCtlBuffer, 0x20, alBank->count);
	for (int x = 0; x < alBank->count; x++)
	{
		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		instrumentSoundStartLookup[x] = outputCtlCounter;

		int y = 0; // sound

		tblOffsets[x] = outputTblCounter;

		bool foundSameTbl = false;

		for (int r = 0; r < x; r++)
		{
			int z = 0;

			if (
				(alBank->inst[x]->sounds[y]->wav.base == alBank->inst[r]->sounds[z]->wav.base)
				&& (alBank->inst[x]->sounds[y]->wav.len == alBank->inst[r]->sounds[z]->wav.len)
				)
			{
				bool mismatchValues = false;
				for (int rr = 0; rr < alBank->inst[x]->sounds[y]->wav.len; rr++)
				{
					if (alBank->inst[x]->sounds[y]->wav.wavData[rr] != alBank->inst[r]->sounds[z]->wav.wavData[rr])
					{
						mismatchValues = true;
						break;
					}
				}
				if (!mismatchValues)
				{
					foundSameTbl = true;
					tblOffsets[x] = tblOffsets[r];
					break;
				}
				else
				{
					continue;
				}
			}
		}

		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x]);
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter + 4, alBank->inst[x]->sounds[y]->wav.len);

		if (!foundSameTbl)
		{
			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
			{
				temporaryTblBuffer[outputTblCounter + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
			}
			outputTblCounter += alBank->inst[x]->sounds[y]->wav.len;

			if ((outputTblCounter % 0x10) != 0)
			{
				int pad = 0x10 - (outputTblCounter % 0x10);
				for (int z = 0; z < pad; z++)
				{
					temporaryTblBuffer[outputTblCounter++] = 0;
				}
			}
		}

		int sameLoopBank = -1;


		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			int w = 0;
			for (int r = 0; r < x; r++)
			{
			
				if ((alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop != NULL))
				{
					if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->start) 
						&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->end)
						&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->count))
					{
						bool goOn = false;
						for (int ww = 0; ww < 0x10; ww++)
						{
							if (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->state[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[ww])
							{
								goOn = true;
								break;
							}
						}

						if (goOn)
							continue;

						adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[r];
						sameLoopBank = r;
						break;
					}
				}
			}
		}
		 
		int sameBank = -1;

		for (int r = 0; r < x; r++)
		{
			int w = 0;

			if (alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL)
			{
				if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->order) 
					&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->npredictors))
				{
					bool goOn = false;
					for (int ww = 0; ww < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); ww++)
					{
						if (alBank->inst[r]->sounds[w]->wav.adpcmWave->book->predictors[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[ww])
						{
							goOn = true;
							break;
						}
					}

					if (goOn)
						continue;

					sameBank = r;
					break;
				}
			}
		}

		
		unsigned long bookWriteSpot = outputCtlCounter + 0x10;
		unsigned long loopWriteSpot = outputCtlCounter + 0xC;

		outputCtlCounter = outputCtlCounter + 0x18;

		if (sameBank == -1)
		{
			bookOffsetsWav[x] = outputCtlCounter;
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, outputCtlCounter);

			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors);
			outputCtlCounter += 4;

			for (int z = 0; z < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
				temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
			}

			
			// game does this not sure why
			/*int pad = 8;
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}*/
		}
		else
		{
			bookOffsetsWav[x] = bookOffsetsWav[sameBank];
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[sameBank]);
		}

		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			if (sameLoopBank == -1)
			{
				adpcmRawLoopOffsetsWav[x] = outputCtlCounter;
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count);
				outputCtlCounter += 4;
				for (int z = 0; z < 0x10; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
					temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
				}
			}
			else
			{
				adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[sameLoopBank];
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[sameLoopBank]);
			}
		}
		else
		{
			adpcmRawLoopOffsetsWav[x] = 0x00000000;
			// null loop
			WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x2C, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, instrumentSoundStartLookup[x]);
		outputCtlCounter += 4;
	}
	WriteLongToBuffer(temporaryCtlBuffer, 0x24, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		// Coarse Tune
		temporaryCtlBuffer[outputCtlCounter] = alBank->inst[x]->volume;
		outputCtlCounter++;
	}

	if ((outputCtlCounter % 4) != 0)
	{
		int pad = 4 - (outputCtlCounter % 4);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x28, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		// Fine tune
		temporaryCtlBuffer[outputCtlCounter] = alBank->inst[x]->pan;
		outputCtlCounter++;
	}

	if ((outputCtlCounter % 4) != 0)
	{
		int pad = 4 - (outputCtlCounter % 4);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	delete [] instrumentSoundStartLookup;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] tblOffsets;

	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV2Yay0(CString mainFolder, ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* ctlTemp = NULL;
	int ctlSizeTemp = 0;

	WriteAudioN64PtrWavetableV2(alBank, ctlTemp, ctlSizeTemp, tbl, tblSize);

	unsigned char* outputBuffer = new unsigned char[0x800000];
	CNintendoEncoder ninEnc;
	int compSize = ninEnc.encode(ctlTemp, ctlSizeTemp, outputBuffer, "Yay0");

	delete [] ctlTemp;

	if (compSize > 0)
	{
		ctl = new unsigned char[compSize];
		memcpy(ctl, outputBuffer, compSize);
		ctlSize = compSize;

		delete [] outputBuffer;
	}
	else
	{
		delete [] outputBuffer;
		MessageBox(NULL, "Error writing output temp file to disk", "Error", NULL);
	}
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV2Blitz(CString mainFolder, ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* ctlTemp = NULL;
	int ctlSizeTemp = 0;

	WriteAudioN64PtrWavetableV2(alBank, ctlTemp, ctlSizeTemp, tbl, tblSize);

	::SetCurrentDirectory(mainFolder);

	FILE* outFileInput = fopen(mainFolder + "tempASA3da.bin", "wb");
	if (outFileInput != NULL)
	{
		fwrite(ctlTemp, 1, ctlSizeTemp, outFileInput);
		fclose(outFileInput);

		CString tempStr;
		tempStr.Format("precomp1x.exe \"%s\" \"%s\"", mainFolder + "tempASA3da.bin", mainFolder + "tempASA3daout.bin");
		if (hiddenExec(_T(tempStr.GetBuffer()), mainFolder))
		{
			ctlSize = GetSizeFile(mainFolder + "tempASA3daout.bin");
			if (ctlSize > 0)
			{
				FILE* outFileOutput = fopen(mainFolder + "tempASA3daout.bin", "rb");
				if (outFileOutput != NULL)
				{
					ctl = new unsigned char[ctlSize];
					fread(ctl, 1, ctlSize, outFileOutput);
					fclose(outFileOutput);
				}
				else
				{
					MessageBox(NULL, "Error writing output temp file to disk", "Error", NULL);
				}
			}
			else
			{
				MessageBox(NULL, "Error empty output temp file", "Error", NULL);
			}
			::DeleteFile(mainFolder + "tempASA3da.bin");
			::DeleteFile(mainFolder + "tempASA3daout.bin");
		}
	}
	else
	{
		MessageBox(NULL, "Error writing input temp file to disk", "Error", NULL);
	}

	delete [] ctlTemp;
}

bool CN64AIFCAudio::CompareLoops(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (wavOut->adpcmWave->loop == NULL)
		return false;

	if ((wavIn->adpcmWave->loop->start == wavOut->adpcmWave->loop->start) 
		&& (wavIn->adpcmWave->loop->end == wavOut->adpcmWave->loop->end)
		&& (wavIn->adpcmWave->loop->count == wavOut->adpcmWave->loop->count))
	{
		if (wavIn->adpcmWave->loop->start != 0)
		{
			for (int ww = 0; ww < 0x10; ww++)
			{
				if (wavIn->adpcmWave->loop->state[ww] != wavOut->adpcmWave->loop->state[ww])
				{
					return -1;
				}
			}
		}

		return true;
	}
	return false;
}

bool CN64AIFCAudio::CompareBooks(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (wavOut->adpcmWave->book == NULL)
		return false;

	if ((wavIn->adpcmWave->book->order == wavOut->adpcmWave->book->order) 
		&& (wavIn->adpcmWave->book->npredictors == wavOut->adpcmWave->book->npredictors))
	{
		for (int ww = 0; ww < (wavIn->adpcmWave->book->order * wavIn->adpcmWave->book->npredictors * 8); ww++)
		{
			if (wavIn->adpcmWave->book->predictors[ww] != wavOut->adpcmWave->book->predictors[ww])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool CN64AIFCAudio::CompareWavs(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (
		(wavIn->base == wavOut->base)
		&& (wavIn->len == wavOut->len)
		&& (wavIn->unknown1 == wavOut->unknown1)
		&& (wavIn->unknown2 == wavOut->unknown2)
		&& (wavIn->unknown3 == wavOut->unknown3)
		&& (wavIn->unknown4 == wavOut->unknown4)
		)
	{
		if (((wavIn->adpcmWave->loop == NULL) && (wavOut->adpcmWave->loop != NULL))
			|| ((wavIn->adpcmWave->loop != NULL) && (wavOut->adpcmWave->loop == NULL)))
		{
			return false;
		}

		if ((wavIn->adpcmWave->loop == NULL) && (wavOut->adpcmWave->loop == NULL))
		{
			
		}
		else
		{
			if (
				(wavIn->adpcmWave->loop->start == wavOut->adpcmWave->loop->start)
				&& (wavIn->adpcmWave->loop->end == wavOut->adpcmWave->loop->end)
				&& (wavIn->adpcmWave->loop->count == wavOut->adpcmWave->loop->count))
			{
				if (wavIn->adpcmWave->loop->start != 0)
				{
					for (int z = 0; z < 0x10; z++)
					{
						if (wavIn->adpcmWave->loop->state[z] != wavOut->adpcmWave->loop->state[z])
						{
							return false;
						}
					}
				}
			}
			else
			{
				return false;
			}
		}

		if ((wavIn->adpcmWave->book->npredictors == wavOut->adpcmWave->book->npredictors)
			&& (wavIn->adpcmWave->book->order == wavOut->adpcmWave->book->order))
		{
			for (int z = 0; z < (wavIn->adpcmWave->book->npredictors * wavIn->adpcmWave->book->order * 8); z++)
			{
				if (wavIn->adpcmWave->book->predictors[z] != wavOut->adpcmWave->book->predictors[z])
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool CN64AIFCAudio::CompareTbls(ALWave* wavIn, ALWave* wavOut)
{
	if (
		(wavIn->len == wavOut->len)
		)
	{
		for (int rr = 0; rr < wavIn->len; rr++)
		{
			if (wavIn->wavData[rr] != wavOut->wavData[rr])
			{
				return false;
			}
		}
		
		return true;
	}
	return false;
}

void CN64AIFCAudio::CheckAddWriteZeldaWaveData(ALWave* waveData, int instrumentIndex, std::vector<ALADPCMBook*>& predictorData, std::vector<unsigned long>& offsetPredictorData, std::vector<ALADPCMloop*>& loopData, std::vector<unsigned long>& offsetLoopData, std::vector<int>& waveDataIndex, std::vector<int>& predictorIndex, std::vector<int>& loopIndex, unsigned char* temporaryCtlBuffer, unsigned char* temporaryTblBuffer, unsigned long& outputTblCounter, std::vector<unsigned char>& waveFlag, std::vector<unsigned long>& waveDataLength, std::vector<unsigned long>& offsetWaveData, int& dataOffset, std::vector<unsigned long>& offsetSoundData, std::vector<int>& soundDataIndex, std::vector<int>& soundDataOverallWaveIndex, std::vector<int>& soundDataOverallPredictorIndex, std::vector<int>& soundDataOverallLoopIndex, std::vector<unsigned char>& soundDataOverallWaveFlag, int tblIndex)
{
	int x = instrumentIndex;

	soundDataIndex[x] = -1;
	waveDataIndex[x] = -1;
	predictorIndex[x] = -1;
	loopIndex[x] = -1;

	// Check if wave exists in entire table, and write offset
	for (int y = 0; y < offsetWaveData.size(); y++)
	{
		if (
			(CompareBytes(waveData->wavData, waveData->len, &temporaryTblBuffer[offsetWaveData[y]], waveDataLength[y]))
			)
		{
			waveDataIndex[x] = y;
			break;
		}
	}

	// Check if predictor exists, if not, write it
	ALADPCMBook* book = waveData->adpcmWave->book;
	for (int y = 0; y < predictorData.size(); y++)
	{

		if (
			(predictorData[y]->npredictors == book->npredictors)
			&& (predictorData[y]->order == book->order)
			)
		{
			bool matchPred = true;
			for (int z = 0; z < (predictorData[y]->npredictors * predictorData[y]->order * 8); z++)
			{
				if (predictorData[y]->predictors[z] != book->predictors[z])
				{
					matchPred = false;
				}
			}

			if (matchPred)
			{
				predictorIndex[x] = y;
				break;
			}
		}
	}

	// Check if loop exists, if not, write it
	ALADPCMloop* loop = waveData->adpcmWave->loop;
	for (int y = 0; y < loopData.size(); y++)
	{
		if (
			(loop != NULL)
			&& (loopData[y] != NULL)
			)
		{
			if (
				(loop->start == loopData[y]->start)
				&& (loop->end == loopData[y]->end)
				&& (loop->count == loopData[y]->count)
				)
			{
				if (loop->start == 0)
				{
					loopIndex[x] = y;
					break;
				}
				else
				{
					bool matchLoop = true;
					for (int z = 0; z < 0x10; z++)
					{
						if (loop->state[z] != loopData[y]->state[z])
						{
							matchLoop = false;
							break;
						}
					}

					if (matchLoop)
					{
						loopIndex[x] = y;
						break;
					}
				}
			}
		}
	}

	if ((waveDataIndex[x] != -1) && (predictorIndex[x] != -1) && (loopIndex[x] != -1))
	{
		// Don't need to write an entry, if exists
		for (int z = 0; z < soundDataOverallWaveIndex.size(); z++)
		{
			if (
				(soundDataOverallWaveIndex[z] == waveDataIndex[x])
				&& (soundDataOverallPredictorIndex[z] == predictorIndex[x])
				&& (soundDataOverallLoopIndex[z] == loopIndex[x])
				&& (soundDataOverallWaveFlag[z] == waveData->wavFlags)
				)
			{
				soundDataIndex[x] = z;
				break;
			}
		}
	}

	/*if (soundDataIndex[x] == -1)
	{
		// For some reason the game has all loops are unique except if full match
		loopIndex[x] = -1;
	}*/

	if (waveDataIndex[x] == -1)
	{
		waveFlag.push_back(waveData->wavFlags);
		waveDataLength.push_back(waveData->len);
		offsetWaveData.push_back(outputTblCounter);

		memcpy(&temporaryTblBuffer[outputTblCounter], waveData->wavData, waveData->len);
		outputTblCounter += waveData->len;

		if ((outputTblCounter % 0x10) != 0)
		{
			int remain = 0x10 - (outputTblCounter % 0x10);
			while (remain > 0)
			{
				temporaryTblBuffer[outputTblCounter] = 0x00;
				outputTblCounter++;
				remain--;
			}
		}

		waveDataIndex[x] = (waveFlag.size()-1);
	}

	bool addSoundIndex = false;

	if (soundDataIndex[x] == -1)
	{
		soundDataIndex[x] = offsetSoundData.size();
		offsetSoundData.push_back(dataOffset);
		addSoundIndex = true;

		unsigned long waveLength = waveDataLength[waveDataIndex[x]];
		WriteLongToBuffer(temporaryCtlBuffer, dataOffset, waveLength | (waveData->wavFlags << 24));

		dataOffset += 4;

		unsigned long waveAddress = offsetWaveData[waveDataIndex[x]];
		WriteLongToBuffer(temporaryCtlBuffer, dataOffset, waveAddress);
		dataOffset += 4;

		int dataSubOffset = dataOffset + 8;

		if (predictorIndex[x] != -1)
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 4, offsetPredictorData[predictorIndex[x]]);
		}
		else
		{
			if (book != NULL)
			{
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 4, dataSubOffset);

				dataSubOffset += 8 + (book->order * book->npredictors * 0x10);

				if ((dataSubOffset % 0x10) != 0x0)
				{
					int remain = 0x10 - (dataSubOffset % 0x10);
					dataSubOffset += remain;
				}
			}
			else
			{
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 4, 0x00000000);
			}
		}

		if (loop == NULL)
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
		}
		else if (loopIndex[x] != -1)
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetLoopData[loopIndex[x]]);
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, dataSubOffset);
		}

		dataOffset += 8;
	}


	if (predictorIndex[x] == -1)
	{
		if (book != NULL)
		{
			offsetPredictorData.push_back(dataOffset);
			predictorIndex[x] = predictorData.size();
			predictorData.push_back(book);

			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, book->order);
			dataOffset += 4;
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, book->npredictors);
			dataOffset += 4;
			for (int z = 0; z < (book->order * book->npredictors * 8); z++)
			{
				WriteShortToBuffer(temporaryCtlBuffer, dataOffset, book->predictors[z]);
				dataOffset += 2;
			}

			if ((dataOffset % 0x10) != 0)
			{
				int remain = 0x10 - (dataOffset % 0x10);
				while (remain > 0)
				{
					temporaryCtlBuffer[dataOffset] = 0x00;
					dataOffset++;
					remain--;
				}
			}
		}
	}


	if (loopIndex[x] == -1)
	{
		offsetLoopData.push_back(dataOffset);
		loopIndex[x] = loopData.size();
		loopData.push_back(loop);

		if (loop == NULL)
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
			dataOffset += 4;
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, (waveData->len * 7) / 4);
			dataOffset += 4;
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
			dataOffset += 4;
			// Pad
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
			dataOffset += 4;
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, loop->start);
			dataOffset += 4;
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, loop->end);
			dataOffset += 4;
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, loop->count);
			dataOffset += 4;
			// Pad
			WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
			dataOffset += 4;

			if (loop->start != 0x00000000)
			{
				for (int z = 0; z < 0x10; z++)
				{
					WriteShortToBuffer(temporaryCtlBuffer, dataOffset, loop->state[z]);
					dataOffset += 2;
				}
			}
		}
	}

	if (addSoundIndex)
	{
		soundDataOverallWaveIndex.push_back(waveDataIndex[x]);
		soundDataOverallPredictorIndex.push_back(predictorIndex[x]);
		soundDataOverallLoopIndex.push_back(loopIndex[x]);
		soundDataOverallWaveFlag.push_back(waveData->wavFlags);
	}
}

void CN64AIFCAudio::CheckAddWriteZeldaADSRData(unsigned short* adsrEAD, int instrumentIndex, std::vector<unsigned long>& offsetADSR, std::vector<unsigned short*>& adsrData, std::vector<int>& adsrIndex, unsigned char* temporaryCtlBuffer, int& dataOffset)
{
	int x = instrumentIndex;

	adsrIndex[x] = -1;

	for (int y = 0; y < adsrData.size(); y++)
	{
		bool matchADSR = true;
		for (int z = 0; z < 8; z++)
		{
			if (adsrData[y][z] != adsrEAD[z])
			{
				matchADSR = false;
				break;
			}
		}

		if (matchADSR)
		{
			adsrIndex[x] = y;
			break;
		}
	}

	if (adsrIndex[x] == -1)
	{
		adsrIndex[x] = adsrData.size();
		adsrData.push_back((unsigned short*)adsrEAD);
		offsetADSR.push_back(dataOffset);

		for (int z = 0; z < 8; z++)
		{
			WriteShortToBuffer(temporaryCtlBuffer, dataOffset, adsrEAD[z]);
			dataOffset += 2;
		}
	}
}

void CN64AIFCAudio::WriteAudioStarFox(unsigned char* ROM, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd)
{
	if (offsetZeldaCtlTable == 0)
	{
		ctl = NULL;
		tbl = NULL;
		MessageBox(NULL, "Cannot import for this game", "Error", NULL);
		return;
	}

	unsigned char* ctlTblTableData = ROM;
	int ctlTblTableDataSize = romSize;

	if (isCompressedZeldaCtlTblTables)
	{
		YAZ0 yaz;
		//CNintendoEncoder ninEnc;
		ctlTblTableData = new unsigned char[0x2000000];
		int fileSizeCompressed =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
		//int decodedSize = ninEnc.decode(&rom[compressedZeldaCtlTblTableStart], fileSizeCompressed, ctlTblTableData);
		ctlTblTableDataSize = yaz.yaz0_decode(&ROM[compressedZeldaCtlTblTableStart], ctlTblTableData, fileSizeCompressed);
		if (ctlTblTableDataSize == 0)
		{
			delete [] ctlTblTableData;
			return;
		}
	}

	ctl = new unsigned char[0x2000000];
	tbl = new unsigned char[0x2000000];
	for (int x = 0; x < 0x2000000; x++)
	{
		ctl[x] = 0;
		tbl[x] = 0;
	}
	int numberCtls = CharArrayToShort(&ctlTblTableData[offsetZeldaCtlTable]);
	int numberTbls = CharArrayToShort(&ctlTblTableData[offsetZeldaTblTable]);

	int passedInResults = results.size();
	if (numberCtls != passedInResults)
	{
		ctl = NULL;
		tbl = NULL;
		ctlSize = 0;
		tblSize = 0;
		MessageBox(NULL, "Count of Ctls doesn't match ROM, error", "Error", NULL);
		return;
	}

	
	std::vector<unsigned char>* waveFlagArray = new std::vector<unsigned char>[numberTbls];
	std::vector<unsigned long>* waveDataLengthArray = new std::vector<unsigned long>[numberTbls];
	std::vector<unsigned long>* offsetWaveDataArray = new std::vector<unsigned long>[numberTbls];

	unsigned char** temporaryTblBuffer = new unsigned char*[numberTbls];
	unsigned long* outputTblCounter = new unsigned long[numberTbls];
	for (int y = 0; y < numberTbls; y++)
	{
		outputTblCounter[y] = 0;

		temporaryTblBuffer[y] = new unsigned char[0x2000000];
		for (int x = 0; x < 0x2000000; x++)
		{
			temporaryTblBuffer[y][x] = 0;
		}
	}

	ctlSize = 0;

	for (int soundBankNumber = 0; soundBankNumber < results.size(); soundBankNumber++)
	{
		if (CharArrayToLong(&ROM[(offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 4)]) == 0x00000000)
		{
			// Skip 0 Sized originally
			continue;
		}

		int tblIndex1 = ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * soundBankNumber) + 0xA];

		// Weird Zelda hack of table of 1 uses 0, and empty size
		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1) + 4]) == 0x0)
			tblIndex1--;

		int tblIndex2 = ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * soundBankNumber) + 0xB];

		// Weird Zelda hack of table of 1 uses 0, and empty size
		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2) + 4]) == 0x0)
			tblIndex2--;

		if (tblIndex2 == 0xFF)
			tblIndex2 = tblIndex1;

		unsigned char* temporaryCtlBuffer = new unsigned char[0x100000];
		for (int x = 0; x < 0x100000; x++)
		{
			temporaryCtlBuffer[x] = 0;
		}

		ALBank* alBank = results[soundBankNumber].bank;

		int instrumentCount = alBank->count;

		int dataOffset = 0x4 + instrumentCount * 0x4;

		if ((dataOffset % 0x10) != 0)
		{
			int remain = 0x10 - (dataOffset % 0x10);
			while (remain > 0)
			{
				temporaryCtlBuffer[dataOffset] = 0x00;
				dataOffset++;
				remain--;
			}
		}

		std::vector<ALADPCMBook*> predictorData;
		std::vector<unsigned long> offsetPredictorData;
		std::vector<ALADPCMloop*> loopData;
		std::vector<unsigned long> offsetLoopData;

		std::vector<unsigned long> offsetSoundData;
		std::vector<int> soundDataOverallWaveIndex;
		std::vector<int> soundDataOverallPredictorIndex;
		std::vector<int> soundDataOverallLoopIndex;
		std::vector<unsigned char> soundDataOverallWaveFlag;

		std::vector<int> soundDataIndex;
		std::vector<int> waveDataIndex;
		std::vector<int> predictorIndex;
		std::vector<int> loopIndex;

		soundDataIndex.resize(instrumentCount * 3 + alBank->countEADPercussion);
		waveDataIndex.resize(instrumentCount * 3 + alBank->countEADPercussion);
		predictorIndex.resize(instrumentCount * 3 + alBank->countEADPercussion);
		loopIndex.resize(instrumentCount * 3 + alBank->countEADPercussion);

		int totalInstrumentCount = 0;

		// Prewrite the data
		for (int x = 0; x < instrumentCount; x++)
		{
			if (alBank->inst[x] != NULL)
			{
				if (alBank->inst[x]->sounds == NULL)
					continue;

				int wavTblIndex;
				if (alBank->inst[x]->sounds[0]->hasWavePrevious)
				{
					ALWave* waveDataPrev = &alBank->inst[x]->sounds[0]->wavPrevious;
					wavTblIndex = ((waveDataPrev->wavFlags & 0xC) >> 2);
					if (wavTblIndex == 0)
						CheckAddWriteZeldaWaveData(waveDataPrev, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
					else if (wavTblIndex == 1)
						CheckAddWriteZeldaWaveData(waveDataPrev, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				}
				else
				{
					waveDataIndex[totalInstrumentCount] = -1;
					predictorIndex[totalInstrumentCount] = -1;
					loopIndex[totalInstrumentCount] = -1;
				}
				totalInstrumentCount++;

				ALWave* waveData = &alBank->inst[x]->sounds[0]->wav;
				wavTblIndex = ((waveData->wavFlags & 0xC) >> 2);
				if (wavTblIndex == 0)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				else if (wavTblIndex == 1)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				totalInstrumentCount++;

				if (alBank->inst[x]->sounds[0]->hasWaveSecondary)
				{
					ALWave* waveDataSec = &alBank->inst[x]->sounds[0]->wavSecondary;
					wavTblIndex = ((waveDataSec->wavFlags & 0xC) >> 2);
					if (wavTblIndex == 0)
						CheckAddWriteZeldaWaveData(waveDataSec, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
					else if (wavTblIndex == 1)
						CheckAddWriteZeldaWaveData(waveDataSec, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				}
				else
				{
					waveDataIndex[totalInstrumentCount] = -1;
					predictorIndex[totalInstrumentCount] = -1;
					loopIndex[totalInstrumentCount] = -1;
				}
				totalInstrumentCount++;
			}
		}



		for (int x = 0; x < alBank->countEADPercussion; x++)
		{
			if (alBank->eadPercussion[x].keyBase != 0x0)
			{
				if (alBank->eadPercussion[x].wav.adpcmWave != NULL)
				{
					ALWave* waveData = &alBank->eadPercussion[x].wav;
					int wavTblIndex = ((waveData->wavFlags & 0xC) >> 2);
					if (wavTblIndex == 0)
						CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
					else if (wavTblIndex == 1)
						CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				}
			}
			totalInstrumentCount++;
		}

		std::vector<unsigned long> offsetADSR;
		std::vector<unsigned short*> adsrData;

		std::vector<int> adsrIndex;
		adsrIndex.resize(instrumentCount + alBank->countEADPercussion);

		for (int x = 0; x < instrumentCount; x++)
		{
			if (alBank->inst[x] != NULL)
			{
				if (alBank->inst[x]->sounds != NULL)
				{
					if ((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL))
					{
						CheckAddWriteZeldaADSRData(alBank->inst[x]->sounds[0]->adsrEAD, x, offsetADSR, adsrData, adsrIndex, temporaryCtlBuffer, dataOffset);
					}
				}
			}
		}


		for (int x = 0; x < alBank->countEADPercussion; x++)
		{
			if (alBank->eadPercussion[x].wav.adpcmWave != NULL)
			{
				CheckAddWriteZeldaADSRData(alBank->eadPercussion[x].adsrEAD, x + instrumentCount, offsetADSR, adsrData, adsrIndex, temporaryCtlBuffer, dataOffset);
			}
		}

		totalInstrumentCount = 0;

		for (int x = 0; x < instrumentCount; x++)
		{
			if ((alBank->inst[x] != NULL) && (alBank->inst[x]->sounds != NULL))
			{
				// Write instrument offset
				WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (x * 4), dataOffset);

				WriteLongToBuffer(temporaryCtlBuffer, dataOffset, alBank->inst[x]->sounds[0]->unknown1);
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 4, offsetADSR[adsrIndex[x]]);

				if (alBank->inst[x]->sounds[0]->hasWavePrevious)
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x8, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0xC, alBank->inst[x]->sounds[0]->floatKeyBasePrev);
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x8, 0x00000000);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0xC, 0x00000000);
				}
				totalInstrumentCount++;

				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x10, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x14, alBank->inst[x]->sounds[0]->floatKeyBase);
				totalInstrumentCount++;

				if (alBank->inst[x]->sounds[0]->hasWaveSecondary)
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x18, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x1C, alBank->inst[x]->sounds[0]->floatKeyBaseSec);
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x18, 0x00000000);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x1C, 0x00000000);
				}
				totalInstrumentCount++;

				dataOffset += 0x20;
			}
			else
			{
				WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (x * 4), 0x00000000);
			}
		}

		if (alBank->countEADPercussion > 0)
		{
			std::vector<unsigned long> offsetEADPercussion;
			for (int x = 0; x < alBank->countEADPercussion; x++)
			{
				if (alBank->eadPercussion[x].wav.adpcmWave != NULL)
				{
					offsetEADPercussion.push_back(dataOffset);

					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].unknown1A;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].pan;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].unknown1C;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = 0x00;
					dataOffset++;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, alBank->eadPercussion[x].keyBase);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetADSR[adsrIndex[x + instrumentCount]]);
					dataOffset += 4;
				}
				else
				{
					offsetEADPercussion.push_back(0x00000000);
				}
				totalInstrumentCount++;
			}

			WriteLongToBuffer(temporaryCtlBuffer, 0, dataOffset);

			for (int x = 0; x < alBank->countEADPercussion; x++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetEADPercussion[x]);
				dataOffset += 4;
			}

			if ((dataOffset % 0x10) != 0)
			{
				int remain = 0x10 - (dataOffset % 0x10);
				while (remain > 0)
				{
					temporaryCtlBuffer[dataOffset] = 0x00;
					dataOffset++;
					remain--;
				}
			}
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, 0, 0x00000000);
		}

		if ((dataOffset % 0x10) != 0x0)
		{
			int remain = 0x10 - (dataOffset % 0x10);
			while (remain > 0)
			{
				temporaryCtlBuffer[dataOffset] = 0x00;
				dataOffset++;
				remain--;
			}
		}

		WriteLongToBuffer(ctlTblTableData, offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10), ctlSize);
		WriteLongToBuffer(ctlTblTableData, offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 4, dataOffset);
		ctlTblTableData[offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 0xC] = instrumentCount;
		ctlTblTableData[offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 0xD] = alBank->countEADPercussion;

		memcpy(&ctl[ctlSize], temporaryCtlBuffer, dataOffset);
		ctlSize += dataOffset;

		/*CString tempDebugStr;
		tempDebugStr.Format("C:\\temp\\zelda%04X.ctl", soundBankNumber);
		FILE* outTestCtl = fopen(tempDebugStr, "wb");
		fwrite(temporaryCtlBuffer, 1, dataOffset, outTestCtl);
		fclose(outTestCtl);*/



		delete [] temporaryCtlBuffer;
	}

	tblSize = 0;
	for (int y = 0; y < numberTbls; y++)
	{
		if (outputTblCounter[y] == 0x0)
		{
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10), 0x00000000);
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10) + 4, 0x00000000);
		}
		else
		{
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10), tblSize);
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10) + 4, outputTblCounter[y]);

			memcpy(&tbl[tblSize], temporaryTblBuffer[y], outputTblCounter[y]);

			tblSize += outputTblCounter[y];
		}
	}

	/*FILE* outTestCtl = fopen("C:\\temp\\zelda.ctl", "wb");
	fwrite(ctl, 1, ctlSize, outTestCtl);
	fclose(outTestCtl);

	FILE* outTestTbl = fopen("C:\\temp\\zelda.tbl", "wb");
	fwrite(tbl, 1, tblSize, outTestTbl);
	fclose(outTestTbl);*/

	for (int y = 0; y < numberTbls; y++)
	{
		delete [] temporaryTblBuffer[y];
	}
	delete [] temporaryTblBuffer;

	delete [] outputTblCounter;

	if (isCompressedZeldaCtlTblTables)
	{
		CNintendoEncoder ninEnc;
		unsigned char* output = new unsigned char[0x800000];
		int compressedNewSize = 0x800000;
		compressedNewSize = ninEnc.encode(ctlTblTableData, ctlTblTableDataSize, output, "Yaz0");
		YAZ0 yaz;
		//yaz.yaz0_encode(ctlTblTableData, ctlTblTableDataSize, output, &compressedNewSize);
		delete [] ctlTblTableData;

		if (compressedNewSize == 0)
		{
			delete [] output;
			delete [] ctl;
			delete [] tbl;
			ctl = NULL;
			tbl = NULL;
			return;
		}

		int fileSizeCompressedMax =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
		if (compressedNewSize > fileSizeCompressedMax)
		{
			MessageBox(NULL, "Could not compress the tables compressed file", "Error", NULL);
			delete [] output;
			delete [] ctl;
			delete [] tbl;
			ctl = NULL;
			tbl = NULL;
			return;
		}
		
		memcpy(&ROM[compressedZeldaCtlTblTableStart], output, compressedNewSize);
		for (int x = (compressedZeldaCtlTblTableStart + compressedNewSize); x < compressedZeldaCtlTblTableEnd; x++)
		{
			ROM[x] = 0x00;
		}

		delete [] output;
	}
}

void CN64AIFCAudio::WriteAudioZelda(unsigned char* ROM, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd)
{
	if (offsetZeldaCtlTable == 0)
	{
		ctl = NULL;
		tbl = NULL;
		MessageBox(NULL, "Cannot import for this game", "Error", NULL);
		return;
	}

	unsigned char* ctlTblTableData = ROM;
	int ctlTblTableDataSize = romSize;

	if (isCompressedZeldaCtlTblTables)
	{
		YAZ0 yaz;
		//CNintendoEncoder ninEnc;
		ctlTblTableData = new unsigned char[0x800000];
		int fileSizeCompressed =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
		//int decodedSize = ninEnc.decode(&rom[compressedZeldaCtlTblTableStart], fileSizeCompressed, ctlTblTableData);
		ctlTblTableDataSize = yaz.yaz0_decode(&ROM[compressedZeldaCtlTblTableStart], ctlTblTableData, fileSizeCompressed);
		if (ctlTblTableDataSize == 0)
		{
			delete [] ctlTblTableData;
			return;
		}
	}

	ctl = new unsigned char[0x2000000];
	tbl = new unsigned char[0x2000000];
	for (int x = 0; x < 0x2000000; x++)
	{
		ctl[x] = 0;
		tbl[x] = 0;
	}
	int numberCtls = CharArrayToShort(&ctlTblTableData[offsetZeldaCtlTable]);
	int numberTbls = CharArrayToShort(&ctlTblTableData[offsetZeldaTblTable]);

	int passedInResults = results.size();
	if (numberCtls != passedInResults)
	{
		ctl = NULL;
		tbl = NULL;
		ctlSize = 0;
		tblSize = 0;
		MessageBox(NULL, "Count of Ctls doesn't match ROM, error", "Error", NULL);
		return;
	}

	
	std::vector<unsigned char>* waveFlagArray = new std::vector<unsigned char>[numberTbls];
	std::vector<unsigned long>* waveDataLengthArray = new std::vector<unsigned long>[numberTbls];
	std::vector<unsigned long>* offsetWaveDataArray = new std::vector<unsigned long>[numberTbls];

	unsigned char** temporaryTblBuffer = new unsigned char*[numberTbls];
	unsigned long* outputTblCounter = new unsigned long[numberTbls];
	for (int y = 0; y < numberTbls; y++)
	{
		outputTblCounter[y] = 0;

		temporaryTblBuffer[y] = new unsigned char[0x2000000];
		for (int x = 0; x < 0x2000000; x++)
		{
			temporaryTblBuffer[y][x] = 0;
		}
	}

	ctlSize = 0;

	for (int soundBankNumber = 0; soundBankNumber < results.size(); soundBankNumber++)
	{
		if (CharArrayToLong(&ROM[(offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 4)]) == 0x00000000)
		{
			// Skip 0 Sized originally
			continue;
		}

		int tblIndex1 = ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * soundBankNumber) + 0xA];

		// Weird Zelda hack of table of 1 uses 0, and empty size
		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1) + 4]) == 0x0)
			tblIndex1--;

		int tblIndex2 = ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * soundBankNumber) + 0xB];

		// Weird Zelda hack of table of 1 uses 0, and empty size
		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2) + 4]) == 0x0)
			tblIndex2--;

		if (tblIndex2 == 0xFF)
			tblIndex2 = tblIndex1;

		unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
		for (int x = 0; x < 0x2000000; x++)
		{
			temporaryCtlBuffer[x] = 0;
		}

		ALBank* alBank = results[soundBankNumber].bank;

		int instrumentCount = alBank->count;

		int dataOffset = 0x4 + 0x4 + instrumentCount * 0x4;

		if ((dataOffset % 0x10) != 0)
		{
			int remain = 0x10 - (dataOffset % 0x10);
			while (remain > 0)
			{
				temporaryCtlBuffer[dataOffset] = 0x00;
				dataOffset++;
				remain--;
			}
		}

		std::vector<ALADPCMBook*> predictorData;
		std::vector<unsigned long> offsetPredictorData;
		std::vector<ALADPCMloop*> loopData;
		std::vector<unsigned long> offsetLoopData;

		std::vector<unsigned long> offsetSoundData;
		std::vector<int> soundDataOverallWaveIndex;
		std::vector<int> soundDataOverallPredictorIndex;
		std::vector<int> soundDataOverallLoopIndex;
		std::vector<unsigned char> soundDataOverallWaveFlag;

		std::vector<int> soundDataIndex;
		std::vector<int> waveDataIndex;
		std::vector<int> predictorIndex;
		std::vector<int> loopIndex;

		soundDataIndex.resize(instrumentCount * 3 + alBank->countEADPercussion + alBank->countSfx);
		waveDataIndex.resize(instrumentCount * 3 + alBank->countEADPercussion + alBank->countSfx);
		predictorIndex.resize(instrumentCount * 3 + alBank->countEADPercussion + alBank->countSfx);
		loopIndex.resize(instrumentCount * 3 + alBank->countEADPercussion + alBank->countSfx);

		int totalInstrumentCount = 0;

		// Prewrite the data
		for (int x = 0; x < instrumentCount; x++)
		{
			if (alBank->inst[x] != NULL)
			{
				if (alBank->inst[x]->sounds == NULL)
					continue;

				if (alBank->inst[x]->sounds[0] == NULL)
					continue;

				int wavTblIndex;
				if (alBank->inst[x]->sounds[0]->hasWavePrevious)
				{
					ALWave* waveDataPrev = &alBank->inst[x]->sounds[0]->wavPrevious;
					wavTblIndex = ((waveDataPrev->wavFlags & 0xC) >> 2);
					if (wavTblIndex == 0)
						CheckAddWriteZeldaWaveData(waveDataPrev, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
					else if (wavTblIndex == 1)
						CheckAddWriteZeldaWaveData(waveDataPrev, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				}
				else
				{
					waveDataIndex[totalInstrumentCount] = -1;
					predictorIndex[totalInstrumentCount] = -1;
					loopIndex[totalInstrumentCount] = -1;
				}
				totalInstrumentCount++;

				ALWave* waveData = &alBank->inst[x]->sounds[0]->wav;
				wavTblIndex = ((waveData->wavFlags & 0xC) >> 2);
				if (wavTblIndex == 0)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				else if (wavTblIndex == 1)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				totalInstrumentCount++;

				if (alBank->inst[x]->sounds[0]->hasWaveSecondary)
				{
					ALWave* waveDataSec = &alBank->inst[x]->sounds[0]->wavSecondary;
					wavTblIndex = ((waveDataSec->wavFlags & 0xC) >> 2);
					if (wavTblIndex == 0)
						CheckAddWriteZeldaWaveData(waveDataSec, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
					else if (wavTblIndex == 1)
						CheckAddWriteZeldaWaveData(waveDataSec, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				}
				else
				{
					waveDataIndex[totalInstrumentCount] = -1;
					predictorIndex[totalInstrumentCount] = -1;
					loopIndex[totalInstrumentCount] = -1;
				}
				totalInstrumentCount++;
			}
		}

		for (int x = 0; x < alBank->countEADPercussion; x++)
		{
			if (alBank->eadPercussion[x].keyBase != 0x0)
			{
				ALWave* waveData = &alBank->eadPercussion[x].wav;
				int wavTblIndex = ((waveData->wavFlags & 0xC) >> 2);
				if (wavTblIndex == 0)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				else if (wavTblIndex == 1)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
			}
			totalInstrumentCount++;
		}

		for (int x = 0; x < alBank->countSfx; x++)
		{
			ALWave* waveData = alBank->alSfx[x];
			if (waveData != NULL)
			{
				int wavTblIndex = ((waveData->wavFlags & 0xC) >> 2);
				if (wavTblIndex == 0)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex1], outputTblCounter[tblIndex1], waveFlagArray[tblIndex1], waveDataLengthArray[tblIndex1], offsetWaveDataArray[tblIndex1], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
				else if (wavTblIndex == 1)
					CheckAddWriteZeldaWaveData(waveData, totalInstrumentCount, predictorData, offsetPredictorData, loopData, offsetLoopData, waveDataIndex, predictorIndex, loopIndex, temporaryCtlBuffer, temporaryTblBuffer[tblIndex2], outputTblCounter[tblIndex2], waveFlagArray[tblIndex2], waveDataLengthArray[tblIndex2], offsetWaveDataArray[tblIndex2], dataOffset, offsetSoundData, soundDataIndex, soundDataOverallWaveIndex, soundDataOverallPredictorIndex, soundDataOverallLoopIndex, soundDataOverallWaveFlag, wavTblIndex);
			}
			totalInstrumentCount++;
		}


		std::vector<unsigned long> offsetADSR;
		std::vector<unsigned short*> adsrData;

		std::vector<int> adsrIndex;
		adsrIndex.resize(instrumentCount + alBank->countEADPercussion);

		for (int x = 0; x < instrumentCount; x++)
		{
			if (alBank->inst[x] != NULL)
			{
				if (alBank->inst[x]->sounds != NULL)
				{
					if ((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL))
					{
						CheckAddWriteZeldaADSRData(alBank->inst[x]->sounds[0]->adsrEAD, x, offsetADSR, adsrData, adsrIndex, temporaryCtlBuffer, dataOffset);
					}
				}
			}
		}

		for (int x = 0; x < alBank->countEADPercussion; x++)
		{
			if (alBank->eadPercussion[x].keyBase != 0x0)
			{
				CheckAddWriteZeldaADSRData(alBank->eadPercussion[x].adsrEAD, x + instrumentCount, offsetADSR, adsrData, adsrIndex, temporaryCtlBuffer, dataOffset);
			}
		}

		totalInstrumentCount = 0;

		for (int x = 0; x < instrumentCount; x++)
		{
			if ((alBank->inst[x] != NULL) && (alBank->inst[x]->sounds != NULL))
			{
				// Write instrument offset
				WriteLongToBuffer(temporaryCtlBuffer, 0x8 + (x * 4), dataOffset);

				if ((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL))
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, alBank->inst[x]->sounds[0]->unknown1);
				else
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x0);

				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 4, offsetADSR[adsrIndex[x]]);

				if (((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL)) &&  (alBank->inst[x]->sounds[0]->hasWavePrevious))
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x8, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0xC, alBank->inst[x]->sounds[0]->floatKeyBasePrev);
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x8, 0x00000000);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0xC, 0x00000000);
				}
				totalInstrumentCount++;

				WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x10, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
				if ((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL))
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x14, alBank->inst[x]->sounds[0]->floatKeyBase);
				else
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x14, 0x00000000);
				totalInstrumentCount++;

				if (((alBank->inst[x]->soundCount > 0) && (alBank->inst[x]->sounds[0] != NULL)) &&  (alBank->inst[x]->sounds[0]->hasWaveSecondary))
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x18, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x1C, alBank->inst[x]->sounds[0]->floatKeyBaseSec);
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x18, 0x00000000);
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset + 0x1C, 0x00000000);
				}
				totalInstrumentCount++;

				dataOffset += 0x20;
			}
			else
			{
				WriteLongToBuffer(temporaryCtlBuffer, 0x8 + (x * 4), 0x00000000);
			}
		}

		if (alBank->countEADPercussion > 0)
		{
			std::vector<unsigned long> offsetEADPercussion;
			for (int x = 0; x < alBank->countEADPercussion; x++)
			{
				if (alBank->eadPercussion[x].keyBase != 0x0)
				{
					offsetEADPercussion.push_back(dataOffset);

					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].unknown1A;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].pan;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = alBank->eadPercussion[x].unknown1C;
					dataOffset++;
					temporaryCtlBuffer[dataOffset] = 0x00;
					dataOffset++;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, alBank->eadPercussion[x].keyBase);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetADSR[adsrIndex[instrumentCount + x]]);
					dataOffset += 4;
				}
				else
				{
					offsetEADPercussion.push_back(0x00000000);
				}
				totalInstrumentCount++;
			}

			WriteLongToBuffer(temporaryCtlBuffer, 0, dataOffset);

			for (int x = 0; x < alBank->countEADPercussion; x++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetEADPercussion[x]);
				dataOffset += 4;
			}

			if ((dataOffset % 0x10) != 0)
			{
				int remain = 0x10 - (dataOffset % 0x10);
				while (remain > 0)
				{
					temporaryCtlBuffer[dataOffset] = 0x00;
					dataOffset++;
					remain--;
				}
			}
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, 0, 0x00000000);
		}

		if (alBank->countSfx > 0)
		{
			WriteLongToBuffer(temporaryCtlBuffer, 4, dataOffset);

			for (int x = 0; x < alBank->countSfx; x++)
			{
				if (alBank->alSfx[x] != NULL)
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, offsetSoundData[soundDataIndex[totalInstrumentCount]]);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, alBank->alSfx[x]->unknown1);
					dataOffset += 4;
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
					dataOffset += 4;

					WriteLongToBuffer(temporaryCtlBuffer, dataOffset, 0x00000000);
					dataOffset += 4;
				}

				totalInstrumentCount++;
			}
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, 4, 0x00000000);
		}

		if ((dataOffset % 0x10) != 0x0)
		{
			int remain = 0x10 - (dataOffset % 0x10);
			while (remain > 0)
			{
				temporaryCtlBuffer[dataOffset] = 0x00;
				dataOffset++;
				remain--;
			}
		}

		WriteLongToBuffer(ctlTblTableData, offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10), ctlSize);
		WriteLongToBuffer(ctlTblTableData, offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 4, dataOffset);
		ctlTblTableData[offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 0xC] = instrumentCount;
		ctlTblTableData[offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 0xD] = alBank->countEADPercussion;
		ctlTblTableData[offsetZeldaCtlTable + 0x10 + (soundBankNumber * 0x10) + 0xF] = alBank->countSfx;

		memcpy(&ctl[ctlSize], temporaryCtlBuffer, dataOffset);
		ctlSize += dataOffset;

		/*CString tempDebugStr;
		tempDebugStr.Format("C:\\temp\\zelda%04X.ctl", soundBankNumber);
		FILE* outTestCtl = fopen(tempDebugStr, "wb");
		fwrite(temporaryCtlBuffer, 1, dataOffset, outTestCtl);
		fclose(outTestCtl);*/



		delete [] temporaryCtlBuffer;
	}

	tblSize = 0;
	for (int y = 0; y < numberTbls; y++)
	{
		if (outputTblCounter[y] == 0x0)
		{
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10), 0x00000000);
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10) + 4, 0x00000000);
		}
		else
		{
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10), tblSize);
			WriteLongToBuffer(ctlTblTableData, offsetZeldaTblTable + 0x10 + (y * 0x10) + 4, outputTblCounter[y]);

			memcpy(&tbl[tblSize], temporaryTblBuffer[y], outputTblCounter[y]);

			tblSize += outputTblCounter[y];
		}
	}

	/*FILE* outTestCtl = fopen("C:\\temp\\zelda.ctl", "wb");
	fwrite(ctl, 1, ctlSize, outTestCtl);
	fclose(outTestCtl);

	FILE* outTestTbl = fopen("C:\\temp\\zelda.tbl", "wb");
	fwrite(tbl, 1, tblSize, outTestTbl);
	fclose(outTestTbl);*/

	for (int y = 0; y < numberTbls; y++)
	{
		delete [] temporaryTblBuffer[y];
	}
	delete [] temporaryTblBuffer;

	delete [] outputTblCounter;

	if (isCompressedZeldaCtlTblTables)
	{
		CNintendoEncoder ninEnc;
		unsigned char* output = new unsigned char[0x800000];
		int compressedNewSize = 0x800000;
		compressedNewSize = ninEnc.encode(ctlTblTableData, ctlTblTableDataSize, output, "Yaz0");
		YAZ0 yaz;
		//yaz.yaz0_encode(ctlTblTableData, ctlTblTableDataSize, output, &compressedNewSize);
		delete [] ctlTblTableData;

		if (compressedNewSize == 0)
		{
			delete [] output;
			delete [] ctl;
			delete [] tbl;
			ctl = NULL;
			tbl = NULL;
			return;
		}

		int fileSizeCompressedMax =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
		if (compressedNewSize > fileSizeCompressedMax)
		{
			MessageBox(NULL, "Could not compress the tables compressed file", "Error", NULL);
			delete [] output;
			delete [] ctl;
			delete [] tbl;
			ctl = NULL;
			tbl = NULL;
			return;
		}
		
		memcpy(&ROM[compressedZeldaCtlTblTableStart], output, compressedNewSize);
		for (int x = (compressedZeldaCtlTblTableStart + compressedNewSize); x < compressedZeldaCtlTblTableEnd; x++)
		{
			ROM[x] = 0x00;
		}

		delete [] output;
	}
}

bool CN64AIFCAudio::CompareBytes(unsigned char* in, int inLength, unsigned char* out, int outLength)
{
	if (inLength != outLength)
		return false;

	for (int x = 0; x < inLength; x++)
	{
		if (in[x] != out[x])
			return false;
	}

	return true;
}

void CN64AIFCAudio::WriteAudioSuperMario(std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x2000000];
	for (int x = 0; x < 0x2000000; x++)
	{
		temporaryCtlBuffer[x] = 0;
		temporaryTblBuffer[x] = 0;
	}
	unsigned long outputCtlCounter = 0;
	
	//bank #
	int bankCount = 1;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x0001);
	outputCtlCounter += 2;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, results.size());
	outputCtlCounter += 2;

	for (int x = 0; x < results.size(); x++)
	{
		// offset to bank later in file
		outputCtlCounter += 8;
	}

	if ((outputCtlCounter % 0x10) != 0)
		outputCtlCounter += ((0x10 - (outputCtlCounter % 0x10)));

	unsigned long tblSizes = 0;
	unsigned char* tblData = new unsigned char[0x2000000];

	for (int y = 0; y < 0x2000000; y++)
		tblData[y] = 0;

	std::vector<unsigned long> overallTblOffsets;
	std::vector<ALWave*> overallTblDataWaves;

	for (int soundBankNumber = 0; soundBankNumber < results.size(); soundBankNumber++)
	{
		ALBank* alBank = results[soundBankNumber].bank;

		unsigned long instrumentALLookup = 0;
		unsigned long* instrumentSoundStartLookup = new unsigned long[alBank->count+alBank->countEADPercussion];
		unsigned long* offsetsEnv = new unsigned long[alBank->count+alBank->countEADPercussion];
		unsigned long* offsetsWav = new unsigned long[alBank->count+alBank->countEADPercussion];
		unsigned long* bookOffsetsWav = new unsigned long[alBank->count+alBank->countEADPercussion];
		unsigned long* adpcmRawLoopOffsetsWav = new unsigned long[alBank->count+alBank->countEADPercussion];

		unsigned long* tblOffsets = new unsigned long[alBank->count+alBank->countEADPercussion];

		unsigned long* offsetsWavSecondary = new unsigned long[alBank->count];
		unsigned long* bookOffsetsWavSecondary = new unsigned long[alBank->count];
		unsigned long* adpcmRawLoopOffsetsWavSecondary = new unsigned long[alBank->count];

		unsigned long* tblOffsetsSecondary = new unsigned long[alBank->count];

		unsigned long* offsetsWavPrevious = new unsigned long[alBank->count];
		unsigned long* bookOffsetsWavPrevious = new unsigned long[alBank->count];
		unsigned long* adpcmRawLoopOffsetsWavPrevious = new unsigned long[alBank->count];

		unsigned long* tblOffsetsPrevious = new unsigned long[alBank->count];

		instrumentALLookup = outputCtlCounter;

		// ctl bank offset
		WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (0x8 * soundBankNumber), instrumentALLookup);

		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->countEADPercussion);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->unknown2);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->unknown3);
		outputCtlCounter += 4;

		// Write percussion bank offsets here after
		outputCtlCounter += 4;

		unsigned long soundOffsetSpot = outputCtlCounter;

		outputCtlCounter += (alBank->count * 4);

		// is this 8 or 0x10?
		if ((outputCtlCounter % 0x10) != 0)
		{
			int pad = 0x10 - (outputCtlCounter % 0x10);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		unsigned long soundDataOffset = instrumentALLookup + 0x10;

		for (int y = 0; y < alBank->count; y++)
		{
			if ((alBank->inst[y] != NULL) && (alBank->inst[y]->sounds != NULL) && (alBank->inst[y]->soundCount > 0))
			{
				offsetsWavPrevious[y] = (outputCtlCounter - soundDataOffset);

				if (alBank->inst[y]->sounds[0]->hasWavePrevious)
				{
					int entireMatch = -1;
					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (!(alBank->inst[w]->sounds[0]->hasWavePrevious))
									continue;

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavPrevious))
								{
									entireMatch = w;
									offsetsWavPrevious[y] = offsetsWavPrevious[entireMatch];
									bookOffsetsWavPrevious[y] = bookOffsetsWavPrevious[entireMatch];
									tblOffsetsPrevious[y] = tblOffsetsPrevious[entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}


					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wav))
								{
									entireMatch = w;
									offsetsWavPrevious[y] = offsetsWav[entireMatch];
									bookOffsetsWavPrevious[y] = bookOffsetsWav[entireMatch];
									tblOffsetsPrevious[y] = tblOffsets[entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (!(alBank->inst[w]->sounds[0]->hasWaveSecondary))
									continue;

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavSecondary))
								{
									entireMatch = w;
									offsetsWavPrevious[y] = offsetsWavSecondary[entireMatch];
									bookOffsetsWavPrevious[y] = bookOffsetsWavSecondary[entireMatch];
									tblOffsetsPrevious[y] = tblOffsetsSecondary[entireMatch];
									break;										
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						int sameBook = -1;

						unsigned long loopOutputPosition = outputCtlCounter + 0x20;
						if (alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book != NULL)
						{						
							bookOffsetsWavPrevious[y] = loopOutputPosition;

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWavePrevious)
										{
											if (CompareBooks(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavPrevious))
											{
												sameBook = w;
												bookOffsetsWavPrevious[y] = bookOffsetsWavPrevious[w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (CompareBooks(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wav))
										{
											sameBook = w;
											bookOffsetsWavPrevious[y] = bookOffsetsWav[w];
											break;
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
										{
											if (CompareBooks(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavSecondary))
											{
												sameBook = w;
												bookOffsetsWavPrevious[y] = bookOffsetsWavSecondary[w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[y]->sounds[0]->wav.adpcmWave->book->npredictors * alBank->inst[y]->sounds[0]->wav.adpcmWave->book->order) * 0x10);
								if ((loopOutputPosition % 0x10) != 0)
									loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
							}

						}
						else
						{
							// Used?
							MessageBox(NULL, "Error book", "Error", NULL);
						}

						int sameLoop = -1;

						if (alBank->inst[y]->sounds[0]->wav.adpcmWave->loop != NULL)
						{
							
							adpcmRawLoopOffsetsWavPrevious[y] = loopOutputPosition;

							if (alBank->inst[y]->sounds[0]->wav.adpcmWave->loop != NULL)
							{
								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (alBank->inst[w]->sounds[0]->hasWavePrevious)
											{
												if (CompareLoops(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavPrevious))
												{
													adpcmRawLoopOffsetsWavPrevious[y] = adpcmRawLoopOffsetsWavPrevious[w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (CompareLoops(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wav))
											{
												adpcmRawLoopOffsetsWavPrevious[y] = adpcmRawLoopOffsetsWav[w];
												sameLoop = w;
												break;
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
											{
												if (CompareLoops(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[w]->sounds[0]->wavSecondary))
												{
													adpcmRawLoopOffsetsWavPrevious[y] = adpcmRawLoopOffsetsWavSecondary[w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}
							}
							
						}
						else
						{
							// Used?
							MessageBox(NULL, "Error loop", "Error", NULL);
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.unknown1);
						outputCtlCounter += 4;

						tblOffsetsPrevious[y] = tblSizes;

						bool foundSameTbl = false;

						if (!foundSameTbl)
						{
							for (int z = 0; z < overallTblOffsets.size(); z++)
							{
								if (CompareTbls(&alBank->inst[y]->sounds[0]->wavPrevious, overallTblDataWaves[z]))
								{
									foundSameTbl = true;
									tblOffsetsPrevious[y] = overallTblOffsets[z];
									break;
								}
							}
						}

						if (!foundSameTbl)
						{
							int loopEnd = y;
							
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (alBank->inst[z]->sounds[0]->hasWavePrevious)
									{
										if (CompareTbls(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[z]->sounds[0]->wavPrevious))
										{
											foundSameTbl = true;
											tblOffsetsPrevious[y] = tblOffsetsPrevious[z];
											break;
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							int loopEnd = y;	
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (CompareTbls(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[z]->sounds[0]->wav))
									{
										foundSameTbl = true;
										tblOffsetsPrevious[y] = tblOffsets[z];
										break;
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							int loopEnd = y;
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (alBank->inst[z]->sounds[0]->hasWaveSecondary)
									{
										if (CompareTbls(&alBank->inst[y]->sounds[0]->wavPrevious, &alBank->inst[z]->sounds[0]->wavSecondary))
										{
											foundSameTbl = true;
											tblOffsetsPrevious[y] = tblOffsetsSecondary[z];
											break;
										}
									}
								}
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsetsPrevious[y]);
						outputCtlCounter += 4;


						if (!foundSameTbl)
						{
							overallTblOffsets.push_back(tblOffsetsPrevious[y]);
							overallTblDataWaves.push_back(&alBank->inst[y]->sounds[0]->wavPrevious);

							unsigned long curEntrySize = alBank->inst[y]->sounds[0]->wavPrevious.len;
							if ((alBank->inst[y]->sounds[0]->wavPrevious.len % 0x10) != 0)
								curEntrySize += (0x10 - (alBank->inst[y]->sounds[0]->wavPrevious.len % 0x10));

							for (int z = 0; z < alBank->inst[y]->sounds[0]->wavPrevious.len; z++)
							{
								tblData[tblSizes + z] = alBank->inst[y]->sounds[0]->wavPrevious.wavData[z];
							}

							tblSizes += curEntrySize;
							
							if ((tblSizes % 0x10) != 0)
							{
								tblSizes += (0x10 - (tblSizes % 0x10));
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWavPrevious[y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWavPrevious[y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.len);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.unknown2);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.unknown3);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.unknown4);
						outputCtlCounter += 4;

						if (sameBook == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->order);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->npredictors);
							outputCtlCounter += 4;


							for (int z = 0; z < (alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->order * alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->npredictors * 8); z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->predictors[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->book->predictors[z]) & 0xFF);
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}

						if (sameLoop == -1)
						{

							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->count);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->unknown1);
							outputCtlCounter += 4;

							if (alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->state[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavPrevious.adpcmWave->loop->state[z]) & 0xFF);
								}
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}
					}
				}





				offsetsWav[y] = (outputCtlCounter - soundDataOffset);

				int entireMatch = -1;
				if (entireMatch == -1)
				{
					for (int w = 0; w < (y + 1); w++)
					{
						if (entireMatch != -1)
							break;

						if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
						{
							if (!(alBank->inst[w]->sounds[0]->hasWavePrevious))
								continue;

							if (CompareWavs(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavPrevious))
							{
								entireMatch = w;
								offsetsWav[y] = offsetsWavPrevious[entireMatch];
								bookOffsetsWav[y] = bookOffsetsWavPrevious[entireMatch];
								tblOffsets[y] = tblOffsetsPrevious[entireMatch];
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}


				if (entireMatch == -1)
				{
					for (int w = 0; w < y; w++)
					{
						if (entireMatch != -1)
							break;

						if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
						{
							if (CompareWavs(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wav))
							{
								entireMatch = w;
								offsetsWav[y] = offsetsWav[entireMatch];
								bookOffsetsWav[y] = bookOffsetsWav[entireMatch];
								tblOffsets[y] = tblOffsets[entireMatch];
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}

				if (entireMatch == -1)
				{
					for (int w = 0; w < y; w++)
					{
						if (entireMatch != -1)
							break;

						if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
						{
							if (!(alBank->inst[w]->sounds[0]->hasWaveSecondary))
								continue;

							if (CompareWavs(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavSecondary))
							{
								entireMatch = w;
								offsetsWav[y] = offsetsWavSecondary[entireMatch];
								bookOffsetsWav[y] = bookOffsetsWavSecondary[entireMatch];
								tblOffsets[y] = tblOffsetsSecondary[entireMatch];
								break;
							}
						}
					}
				}

				if (entireMatch == -1)
				{
					int sameBook = -1;

					unsigned long loopOutputPosition = outputCtlCounter + 0x20;
					if (alBank->inst[y]->sounds[0]->wav.adpcmWave->book != NULL)
					{						
						bookOffsetsWav[y] = loopOutputPosition;

						if (sameBook == -1)
						{
							for (int w = 0; w < (y + 1); w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (alBank->inst[w]->sounds[0]->hasWavePrevious)
									{
										if (CompareBooks(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavPrevious))
										{
											sameBook = w;
											bookOffsetsWav[y] = bookOffsetsWavPrevious[w];
											break;
										}
									}
								}
							}
						}

						if (sameBook == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (CompareBooks(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wav))
									{
										sameBook = w;
										bookOffsetsWav[y] = bookOffsetsWav[w];
										break;
									}
								}
							}
						}

						if (sameBook == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
									{
										if (CompareBooks(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavSecondary))
										{
											sameBook = w;
											bookOffsetsWav[y] = bookOffsetsWavSecondary[w];
											break;
										}
									}
								}
							}
						}

						if (sameBook == -1)
						{
							loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[y]->sounds[0]->wav.adpcmWave->book->npredictors * alBank->inst[y]->sounds[0]->wav.adpcmWave->book->order) * 0x10);
							if ((loopOutputPosition % 0x10) != 0)
								loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
						}

					}
					else
					{
						// Used?
						MessageBox(NULL, "Error book", "Error", NULL);
					}

					int sameLoop = -1;

					if (alBank->inst[y]->sounds[0]->wav.adpcmWave->loop != NULL)
					{
						
						adpcmRawLoopOffsetsWav[y] = loopOutputPosition;

						if (alBank->inst[y]->sounds[0]->wav.adpcmWave->loop != NULL)
						{
							if (sameLoop == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWavePrevious)
										{
											if (CompareLoops(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavPrevious))
											{
												adpcmRawLoopOffsetsWav[y] = adpcmRawLoopOffsetsWavPrevious[w];
												sameLoop = w;
												break;
											}
										}
									}
								}
							}

							if (sameLoop == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (CompareLoops(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wav))
										{
											adpcmRawLoopOffsetsWav[y] = adpcmRawLoopOffsetsWav[w];
											sameLoop = w;
											break;
										}
									}
								}
							}

							if (sameLoop == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
										{
											if (CompareLoops(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[w]->sounds[0]->wavSecondary))
											{
												adpcmRawLoopOffsetsWav[y] = adpcmRawLoopOffsetsWavSecondary[w];
												sameLoop = w;
												break;
											}
										}
									}
								}
							}
						}
						
					}
					else
					{
						// Used?
						MessageBox(NULL, "Error loop", "Error", NULL);
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.unknown1);
					outputCtlCounter += 4;

					tblOffsets[y] = tblSizes;

					bool foundSameTbl = false;

					if (!foundSameTbl)
					{
						for (int z = 0; z < overallTblOffsets.size(); z++)
						{
							if (CompareTbls(&alBank->inst[y]->sounds[0]->wav, overallTblDataWaves[z]))
							{
								foundSameTbl = true;
								tblOffsets[y] = overallTblOffsets[z];
								break;
							}
						}
					}

					if (!foundSameTbl)
					{
						int loopEnd = y;
						for (int z = 0; z < loopEnd; z++)
						{
							if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
							{
								if (alBank->inst[z]->sounds[0]->hasWavePrevious)
								{
									if (CompareTbls(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[z]->sounds[0]->wavPrevious))
									{
										foundSameTbl = true;
										tblOffsets[y] = tblOffsetsPrevious[z];
										break;
									}
								}
							}
						}
					}

					if (!foundSameTbl)
					{
						int loopEnd = y;
						for (int z = 0; z < loopEnd; z++)
						{
							if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
							{
								if (CompareTbls(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[z]->sounds[0]->wav))
								{
									foundSameTbl = true;
									tblOffsets[y] = tblOffsets[z];
									break;
								}
							}
						}
					}

					if (!foundSameTbl)
					{
						int loopEnd = y;
						for (int z = 0; z < loopEnd; z++)
						{
							if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
							{
								if (alBank->inst[z]->sounds[0]->hasWaveSecondary)
								{
									if (CompareTbls(&alBank->inst[y]->sounds[0]->wav, &alBank->inst[z]->sounds[0]->wavSecondary))
									{
										foundSameTbl = true;
										tblOffsets[y] = tblOffsetsSecondary[z];
										break;
									}
								}
							}
						}
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[y]);
					outputCtlCounter += 4;


					if (!foundSameTbl)
					{
						overallTblOffsets.push_back(tblOffsets[y]);
						overallTblDataWaves.push_back(&alBank->inst[y]->sounds[0]->wav);

						unsigned long curEntrySize = alBank->inst[y]->sounds[0]->wav.len;
						if ((alBank->inst[y]->sounds[0]->wav.len % 0x10) != 0)
							curEntrySize += (0x10 - (alBank->inst[y]->sounds[0]->wav.len % 0x10));

						for (int z = 0; z < alBank->inst[y]->sounds[0]->wav.len; z++)
						{
							tblData[tblSizes + z] = alBank->inst[y]->sounds[0]->wav.wavData[z];
						}

						tblSizes += curEntrySize;
						
						if ((tblSizes % 0x10) != 0)
						{
							tblSizes += (0x10 - (tblSizes % 0x10));
						}
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWav[y] - soundDataOffset));
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWav[y] - soundDataOffset));
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.len);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.unknown2);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.unknown3);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.unknown4);
					outputCtlCounter += 4;

					if (sameBook == -1)
					{
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->book->order);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->book->npredictors);
						outputCtlCounter += 4;
						
						for (int z = 0; z < (alBank->inst[y]->sounds[0]->wav.adpcmWave->book->order * alBank->inst[y]->sounds[0]->wav.adpcmWave->book->npredictors * 8); z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wav.adpcmWave->book->predictors[z]) & 0xFF);
						}

						if ((outputCtlCounter % 0x10) != 0)
							outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
					}

					if (sameLoop == -1)
					{
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->start);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->end);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->count);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->unknown1);
						outputCtlCounter += 4;

						if (alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->start != 0)
						{
							for (int z = 0; z < 0x10; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wav.adpcmWave->loop->state[z]) & 0xFF);
							}
						}

						if ((outputCtlCounter % 0x10) != 0)
							outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
					}
				}

				offsetsWavSecondary[y] = (outputCtlCounter - soundDataOffset);

				if (alBank->inst[y]->sounds[0]->hasWaveSecondary)
				{


					int entireMatch = -1;
					if (entireMatch == -1)
					{
						for (int w = 0; w < (y + 1); w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (!(alBank->inst[w]->sounds[0]->hasWavePrevious))
									continue;

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavPrevious))
								{
									entireMatch = w;
									offsetsWavSecondary[y] = offsetsWavPrevious[entireMatch];
									bookOffsetsWavSecondary[y] = bookOffsetsWavPrevious[entireMatch];
									tblOffsetsSecondary[y] = tblOffsetsPrevious[entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}


					if (entireMatch == -1)
					{
						for (int w = 0; w < (y + 1); w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wav))
								{
									entireMatch = w;
									offsetsWavSecondary[y] = offsetsWav[entireMatch];
									bookOffsetsWavSecondary[y] = bookOffsetsWav[entireMatch];
									tblOffsetsSecondary[y] = tblOffsets[entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (!(alBank->inst[w]->sounds[0]->hasWaveSecondary))
									continue;

								if (CompareWavs(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavSecondary))
								{
									entireMatch = w;
									offsetsWavSecondary[y] = offsetsWavSecondary[entireMatch];
									bookOffsetsWavSecondary[y] = bookOffsetsWavSecondary[entireMatch];
									tblOffsetsSecondary[y] = tblOffsetsSecondary[entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						int sameBook = -1;

						unsigned long loopOutputPosition = outputCtlCounter + 0x20;
						if (alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book != NULL)
						{						
							bookOffsetsWavSecondary[y] = loopOutputPosition;

							if (sameBook == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWavePrevious)
										{
											if (CompareBooks(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavPrevious))
											{
												sameBook = w;
												bookOffsetsWavSecondary[y] = bookOffsetsWavPrevious[w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (CompareBooks(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wav))
										{
											sameBook = w;
											bookOffsetsWavSecondary[y] = bookOffsetsWav[w];
											break;
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
									{
										if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
										{
											if (CompareBooks(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavSecondary))
											{
												sameBook = w;
												bookOffsetsWavSecondary[y] = bookOffsetsWavSecondary[w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[y]->sounds[0]->wav.adpcmWave->book->npredictors * alBank->inst[y]->sounds[0]->wav.adpcmWave->book->order) * 0x10);
								if ((loopOutputPosition % 0x10) != 0)
									loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
							}

						}
						else
						{
							// Used?
							MessageBox(NULL, "Error book", "Error", NULL);
						}

						int sameLoop = -1;

						if (alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop != NULL)
						{
							
							adpcmRawLoopOffsetsWavSecondary[y] = loopOutputPosition;

							if (alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop != NULL)
							{
								if (sameLoop == -1)
								{
									for (int w = 0; w < (y + 1); w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (alBank->inst[w]->sounds[0]->hasWavePrevious)
											{
												if (CompareLoops(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavPrevious))
												{
													adpcmRawLoopOffsetsWavSecondary[y] = adpcmRawLoopOffsetsWavPrevious[w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < (y + 1); w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (CompareLoops(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wav))
											{
												adpcmRawLoopOffsetsWavSecondary[y] = adpcmRawLoopOffsetsWav[w];
												sameLoop = w;
												break;
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
										{
											if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
											{
												if (CompareLoops(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[w]->sounds[0]->wavSecondary))
												{
													adpcmRawLoopOffsetsWavSecondary[y] = adpcmRawLoopOffsetsWavSecondary[w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}
							}
						}
						else
						{
							// Used?
							MessageBox(NULL, "Error loop", "Error", NULL);
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.unknown1);
						outputCtlCounter += 4;

						tblOffsetsSecondary[y] = tblSizes;

						bool foundSameTbl = false;

						if (!foundSameTbl)
						{
							for (int z = 0; z < overallTblOffsets.size(); z++)
							{
								if (CompareTbls(&alBank->inst[y]->sounds[0]->wavSecondary, overallTblDataWaves[z]))
								{
									foundSameTbl = true;
									tblOffsetsSecondary[y] = overallTblOffsets[z];
									break;
								}
							}
						}
						
						if (!foundSameTbl)
						{
							int loopEnd = (y + 1);
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (alBank->inst[z]->sounds[0]->hasWavePrevious)
									{
										if (CompareTbls(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[z]->sounds[0]->wavPrevious))
										{
											foundSameTbl = true;
											tblOffsetsSecondary[y] = tblOffsetsPrevious[z];
											break;
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							int loopEnd = (y + 1);
							
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (CompareTbls(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[z]->sounds[0]->wav))
									{
										foundSameTbl = true;
										tblOffsetsSecondary[y] = tblOffsets[z];
										break;
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							int loopEnd = y;
							for (int z = 0; z < loopEnd; z++)
							{
								if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
								{
									if (alBank->inst[z]->sounds[0]->hasWaveSecondary)
									{
										if (CompareTbls(&alBank->inst[y]->sounds[0]->wavSecondary, &alBank->inst[z]->sounds[0]->wavSecondary))
										{
											foundSameTbl = true;
											tblOffsetsSecondary[y] = tblOffsetsSecondary[z];
											break;
										}
									}
								}
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsetsSecondary[y]);
						outputCtlCounter += 4;


						if (!foundSameTbl)
						{
							overallTblOffsets.push_back(tblOffsetsSecondary[y]);
							overallTblDataWaves.push_back(&alBank->inst[y]->sounds[0]->wavSecondary);

							unsigned long curEntrySize = alBank->inst[y]->sounds[0]->wavSecondary.len;
							if ((alBank->inst[y]->sounds[0]->wavSecondary.len % 0x10) != 0)
								curEntrySize += (0x10 - (alBank->inst[y]->sounds[0]->wavSecondary.len % 0x10));

							for (int z = 0; z < alBank->inst[y]->sounds[0]->wavSecondary.len; z++)
							{
								tblData[tblSizes + z] = alBank->inst[y]->sounds[0]->wavSecondary.wavData[z];
							}

							tblSizes += curEntrySize;
							
							if ((tblSizes % 0x10) != 0)
							{
								tblSizes += (0x10 - (tblSizes % 0x10));
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWavSecondary[y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWavSecondary[y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.len);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.unknown2);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.unknown3);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.unknown4);
						outputCtlCounter += 4;

						
						if (sameBook == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->order);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->npredictors);
							outputCtlCounter += 4;

							for (int z = 0; z < (alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->order * alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->npredictors * 8); z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->predictors[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->book->predictors[z]) & 0xFF);
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}

						if (sameLoop == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->count);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->unknown1);
							outputCtlCounter += 4;

							if (alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->state[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[y]->sounds[0]->wavSecondary.adpcmWave->loop->state[z]) & 0xFF);
								}
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}
					}
				}
			}
		}

		int startPercussionInListing = alBank->count;

		for (int y = 0; y < alBank->countEADPercussion; y++)
		{
			if(alBank->eadPercussion[y].wav.adpcmWave == NULL)
			{
				offsetsWav[startPercussionInListing+y] = 0;
				continue;
			}


			offsetsWav[startPercussionInListing+y] = (outputCtlCounter - soundDataOffset);

			int entireMatch = -1;
			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->count; w++)
				{
					if (entireMatch != -1)
						break;

					if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
					{
						
						if (!alBank->inst[w]->sounds[0]->hasWavePrevious)
							continue;

						if (CompareWavs(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavPrevious))
						{
							entireMatch = w;
							offsetsWav[startPercussionInListing+y] = offsetsWavPrevious[entireMatch];
							bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWavPrevious[entireMatch];
							tblOffsets[startPercussionInListing+y] = tblOffsetsPrevious[entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->count; w++)
				{
					if (entireMatch != -1)
						break;

					if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
					{
						if (CompareWavs(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wav))
						{
							entireMatch = w;
							offsetsWav[startPercussionInListing+y] = offsetsWav[entireMatch];
							bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWav[entireMatch];
							tblOffsets[startPercussionInListing+y] = tblOffsets[entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->count; w++)
				{
					if (entireMatch != -1)
						break;

					if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
					{
						if (!alBank->inst[w]->sounds[0]->hasWaveSecondary)
							continue;

						if (CompareWavs(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavSecondary))
						{
							entireMatch = w;
							offsetsWav[startPercussionInListing+y] = offsetsWavSecondary[entireMatch];
							bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWavSecondary[entireMatch];
							tblOffsets[startPercussionInListing+y] = tblOffsetsSecondary[entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{				
				for (int w = 0; w < y; w++)
				{
					if (entireMatch != -1)
						break;


					if (CompareWavs(&alBank->eadPercussion[y].wav, &alBank->eadPercussion[w].wav))
					{
						entireMatch = w;
						offsetsWav[startPercussionInListing+y] = offsetsWav[startPercussionInListing+w];
						bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWav[startPercussionInListing+w];
						tblOffsets[startPercussionInListing+y] = tblOffsets[startPercussionInListing+w];
						break;
					}
				}
			}



			if (entireMatch == -1)
			{
				int sameBook = -1;

				unsigned long loopOutputPosition = outputCtlCounter + 0x20;
				if (alBank->eadPercussion[y].wav.adpcmWave->book != NULL)
				{						
					bookOffsetsWav[startPercussionInListing+y] = loopOutputPosition;

					/*if (sameBook == -1)
					{
						for (int w = 0; w < alBank->count; w++)
						{
							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (alBank->inst[w]->sounds[0]->hasWavePrevious)
								{
									if (CompareBooks(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavPrevious))
									{
										sameBook = w;
										bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWavPrevious[x][w];
										break;
									}
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < alBank->count; w++)
						{
							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (CompareBooks(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wav))
								{
									sameBook = w;
									bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWav[x][w];
									break;
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < alBank->count; w++)
						{
							if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
							{
								if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
								{
									if (CompareBooks(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavSecondary))
									{
										sameBook = w;
										bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWavSecondary[x][w];
										break;
									}
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (CompareBooks(&alBank->eadPercussion[y].wav, &alBank->inst[x]->eadPercussion[w].wav))
							{
								sameBook = w;
								bookOffsetsWav[startPercussionInListing+y] = bookOffsetsWav[x][startPercussionInListing+w];
								break;
							}
						}
					}*/

					if (sameBook == -1)
					{
						loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->eadPercussion[y].wav.adpcmWave->book->npredictors * alBank->eadPercussion[y].wav.adpcmWave->book->order) * 0x10);
						if ((loopOutputPosition % 0x10) != 0)
							loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
					}

				}
				else
				{
					// Used?
					MessageBox(NULL, "Error book", "Error", NULL);
				}

				int sameLoop = -1;

				if (alBank->eadPercussion[y].wav.adpcmWave->loop != NULL)
				{
					
					adpcmRawLoopOffsetsWav[startPercussionInListing+y] = loopOutputPosition;

					if (alBank->eadPercussion[y].wav.adpcmWave->loop != NULL)
					{
						/*if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->count; w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (alBank->inst[w]->sounds[0]->hasWavePrevious)
									{
										if (CompareLoops(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavPrevious))
										{
											adpcmRawLoopOffsetsWav[startPercussionInListing+y] = adpcmRawLoopOffsetsWavPrevious[x][w];
											sameLoop = w;
											break;
										}
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->count; w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (CompareLoops(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wav))
									{
										adpcmRawLoopOffsetsWav[startPercussionInListing+y] = adpcmRawLoopOffsetsWav[x][w];
										sameLoop = w;
										break;
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->count; w++)
							{
								if ((alBank->inst[w]->soundCount > 0) && (alBank->inst[w]->sounds[0] != NULL))
								{
									if (alBank->inst[w]->sounds[0]->hasWaveSecondary)
									{
										if (CompareLoops(&alBank->eadPercussion[y].wav, &alBank->inst[w]->sounds[0]->wavSecondary))
										{
											adpcmRawLoopOffsetsWav[startPercussionInListing+y] = adpcmRawLoopOffsetsWavSecondary[x][w];
											sameLoop = w;
											break;
										}
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if (CompareLoops(&alBank->eadPercussion[y].wav, &alBank->inst[x]->eadPercussion[w].wav))
								{
									sameBook = w;
									adpcmRawLoopOffsetsWav[startPercussionInListing+y] = adpcmRawLoopOffsetsWav[x][startPercussionInListing+w];
								}
							}
						}*/
					}
					
				}
				else
				{
					// Used?
					MessageBox(NULL, "Error loop", "Error", NULL);
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.unknown1);
				outputCtlCounter += 4;

				tblOffsets[startPercussionInListing+y] = tblSizes;

				bool foundSameTbl = false;

				if (!foundSameTbl)
				{
					for (int z = 0; z < overallTblOffsets.size(); z++)
					{
						if (CompareTbls(&alBank->eadPercussion[y].wav, overallTblDataWaves[z]))
						{
							foundSameTbl = true;
							tblOffsets[startPercussionInListing+y] = overallTblOffsets[z];
							break;
						}
					}
				}

				if (!foundSameTbl)
				{	
					int loopEnd = alBank->count;
					for (int z = 0; z < loopEnd; z++)
					{
						if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
						{
							if (alBank->inst[z]->sounds[0]->hasWavePrevious)
							{
								if (CompareTbls(&alBank->eadPercussion[y].wav, &alBank->inst[z]->sounds[0]->wavPrevious))
								{
									foundSameTbl = true;
									tblOffsets[startPercussionInListing+y] = tblOffsetsPrevious[z];
									break;
								}
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					int loopEnd = alBank->count;
					for (int z = 0; z < loopEnd; z++)
					{
						if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
						{
							if (CompareTbls(&alBank->eadPercussion[y].wav, &alBank->inst[z]->sounds[0]->wav))
							{
								foundSameTbl = true;
								tblOffsets[startPercussionInListing+y] = tblOffsets[z];
								break;
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					int loopEnd = alBank->count;
					for (int z = 0; z < loopEnd; z++)
					{
						if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
						{
							if (alBank->inst[z]->sounds[0]->hasWaveSecondary)
							{
								if (CompareTbls(&alBank->eadPercussion[y].wav, &alBank->inst[z]->sounds[0]->wavSecondary))
								{
									foundSameTbl = true;
									tblOffsets[startPercussionInListing+y] = tblOffsetsSecondary[z];
									break;
								}
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					int startPercussionInListingNew = alBank->count;

					int loopEnd = y;
					for (int z = 0; z < loopEnd; z++)
					{
						if (CompareTbls(&alBank->eadPercussion[y].wav, &alBank->eadPercussion[z].wav))
						{
							foundSameTbl = true;
							tblOffsets[startPercussionInListing+y] = tblOffsets[startPercussionInListingNew+z];
							break;
						}
					}
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[startPercussionInListing+y]);
				outputCtlCounter += 4;


				if (!foundSameTbl)
				{
					overallTblOffsets.push_back(tblOffsets[startPercussionInListing+y]);
					overallTblDataWaves.push_back(&alBank->eadPercussion[y].wav);

					unsigned long curEntrySize = alBank->eadPercussion[y].wav.len;
					if ((alBank->eadPercussion[y].wav.len % 0x10) != 0)
						curEntrySize += (0x10 - (alBank->eadPercussion[y].wav.len % 0x10));

					for (int z = 0; z < alBank->eadPercussion[y].wav.len; z++)
					{
						tblData[tblSizes + z] = alBank->eadPercussion[y].wav.wavData[z];
					}

					tblSizes += curEntrySize;
					
					if ((tblSizes % 0x10) != 0)
					{
						tblSizes += (0x10 - (tblSizes % 0x10));
					}
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWav[startPercussionInListing+y] - soundDataOffset));
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWav[startPercussionInListing+y] - soundDataOffset));
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.len);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.unknown2);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.unknown3);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.unknown4);
				outputCtlCounter += 4;

				if (sameBook == -1)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->book->order);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->book->npredictors);
					outputCtlCounter += 4;

					for (int z = 0; z < (alBank->eadPercussion[y].wav.adpcmWave->book->order * alBank->eadPercussion[y].wav.adpcmWave->book->npredictors * 8); z++)
					{
						temporaryCtlBuffer[outputCtlCounter++] = ((alBank->eadPercussion[y].wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
						temporaryCtlBuffer[outputCtlCounter++] = ((alBank->eadPercussion[y].wav.adpcmWave->book->predictors[z]) & 0xFF);
					}

					if ((outputCtlCounter % 0x10) != 0)
						outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
				}

				if (sameLoop == -1)
				{

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->loop->start);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->loop->end);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->loop->count);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].wav.adpcmWave->loop->unknown1);
					outputCtlCounter += 4;

					if (alBank->eadPercussion[y].wav.adpcmWave->loop->start != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->eadPercussion[y].wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->eadPercussion[y].wav.adpcmWave->loop->state[z]) & 0xFF);
						}
					}

					if ((outputCtlCounter % 0x10) != 0)
						outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
				}
			}
		}

		for (int y = 0; y < alBank->count; y++)
		{
			offsetsEnv[y] = (outputCtlCounter - soundDataOffset);

			if ((alBank->inst[y]->soundCount > 0) && (alBank->inst[y]->sounds[0] != NULL))
			{

				bool foundPast = false;

				for (int z = 0; z < y; z++)
				{
					if (
						((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
						&&
						((alBank->inst[y]->soundCount > 0) && (alBank->inst[y]->sounds[0] != NULL))
						)
					{
						if (
							(alBank->inst[y]->sounds[0]->adsrEAD[0] == alBank->inst[z]->sounds[0]->adsrEAD[0])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[1] == alBank->inst[z]->sounds[0]->adsrEAD[1])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[2] == alBank->inst[z]->sounds[0]->adsrEAD[2])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[3] == alBank->inst[z]->sounds[0]->adsrEAD[3])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[4] == alBank->inst[z]->sounds[0]->adsrEAD[4])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[5] == alBank->inst[z]->sounds[0]->adsrEAD[5])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[6] == alBank->inst[z]->sounds[0]->adsrEAD[6])
							&& (alBank->inst[y]->sounds[0]->adsrEAD[7] == alBank->inst[z]->sounds[0]->adsrEAD[7])
							)
						{
							foundPast = true;
							offsetsEnv[y] = offsetsEnv[z];
							break;
						}
					}
				}

				if (foundPast)
					continue;

				for (int z = 0; z < 8; z++)
				{
					WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->adsrEAD[z]);
					outputCtlCounter += 2;
				}
			}
		}

		for (int y = 0; y < alBank->countEADPercussion; y++)
		{
			if(alBank->eadPercussion[y].wav.adpcmWave == NULL)
			{
				offsetsEnv[startPercussionInListing+y] = 0;
				continue;
			}


			offsetsEnv[startPercussionInListing+y] = (outputCtlCounter - soundDataOffset);

			bool foundPast = false;

			for (int z = 0; z < alBank->count; z++)
			{
				if ((alBank->inst[z]->soundCount > 0) && (alBank->inst[z]->sounds[0] != NULL))
				{
					if (
						(alBank->eadPercussion[y].adsrEAD[0] == alBank->inst[z]->sounds[0]->adsrEAD[0])
						&& (alBank->eadPercussion[y].adsrEAD[1] == alBank->inst[z]->sounds[0]->adsrEAD[1])
						&& (alBank->eadPercussion[y].adsrEAD[2] == alBank->inst[z]->sounds[0]->adsrEAD[2])
						&& (alBank->eadPercussion[y].adsrEAD[3] == alBank->inst[z]->sounds[0]->adsrEAD[3])
						&& (alBank->eadPercussion[y].adsrEAD[4] == alBank->inst[z]->sounds[0]->adsrEAD[4])
						&& (alBank->eadPercussion[y].adsrEAD[5] == alBank->inst[z]->sounds[0]->adsrEAD[5])
						&& (alBank->eadPercussion[y].adsrEAD[6] == alBank->inst[z]->sounds[0]->adsrEAD[6])
						&& (alBank->eadPercussion[y].adsrEAD[7] == alBank->inst[z]->sounds[0]->adsrEAD[7])
						)
					{
						foundPast = true;
						offsetsEnv[startPercussionInListing+y] = offsetsEnv[z];
						break;
					}
				}
			}

			if (foundPast)
				continue;

			for (int z = 0; z < y; z++)
			{
				if (
					(alBank->eadPercussion[y].adsrEAD[0] == alBank->eadPercussion[z].adsrEAD[0])
					&& (alBank->eadPercussion[y].adsrEAD[1] == alBank->eadPercussion[z].adsrEAD[1])
					&& (alBank->eadPercussion[y].adsrEAD[2] == alBank->eadPercussion[z].adsrEAD[2])
					&& (alBank->eadPercussion[y].adsrEAD[3] == alBank->eadPercussion[z].adsrEAD[3])
					&& (alBank->eadPercussion[y].adsrEAD[4] == alBank->eadPercussion[z].adsrEAD[4])
					&& (alBank->eadPercussion[y].adsrEAD[5] == alBank->eadPercussion[z].adsrEAD[5])
					&& (alBank->eadPercussion[y].adsrEAD[6] == alBank->eadPercussion[z].adsrEAD[6])
					&& (alBank->eadPercussion[y].adsrEAD[7] == alBank->eadPercussion[z].adsrEAD[7])
					)
				{
					foundPast = true;
					offsetsEnv[startPercussionInListing+y] = offsetsEnv[startPercussionInListing+z];
					break;
				}
			}

			if (foundPast)
				continue;

			for (int z = 0; z < 8; z++)
			{
				WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].adsrEAD[z]);
				outputCtlCounter += 2;
			}
		}

		for (int y = 0; y < alBank->count; y++)
		{
			if ((alBank->inst[y]->soundCount > 0) && (alBank->inst[y]->sounds[0] != NULL))
			{
				WriteLongToBuffer(temporaryCtlBuffer, soundOffsetSpot + (y * 0x4), (outputCtlCounter - soundDataOffset));

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->unknown1);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[y]);
				outputCtlCounter += 4;

				if (alBank->inst[y]->sounds[0]->hasWavePrevious)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWavPrevious[y]);
					outputCtlCounter += 4;
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
					outputCtlCounter += 4;
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->floatKeyBasePrev);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[y]);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->floatKeyBase);
				outputCtlCounter += 4;

				if (alBank->inst[y]->sounds[0]->hasWaveSecondary)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWavSecondary[y]);
					outputCtlCounter += 4;
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
					outputCtlCounter += 4;
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->sounds[0]->floatKeyBaseSec);
				outputCtlCounter += 4;
			}
			else
			{
				WriteLongToBuffer(temporaryCtlBuffer, soundOffsetSpot + (y * 0x4), 0x00000000);
			}
		}

		if ((outputCtlCounter % 0x10) != 0)
			outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));

		

		if (alBank->countEADPercussion == 0)
		{
			WriteLongToBuffer(temporaryCtlBuffer, soundDataOffset, 0x00000000);
		}
		else
		{
			unsigned long startPercussionDataOffsets = (outputCtlCounter  - soundDataOffset);
			for (int y = 0; y < alBank->countEADPercussion; y++)
			{
				if(alBank->eadPercussion[y].wav.adpcmWave == NULL)
					continue;


				temporaryCtlBuffer[outputCtlCounter] = alBank->eadPercussion[y].unknown1A;
				outputCtlCounter++;

				temporaryCtlBuffer[outputCtlCounter] = alBank->eadPercussion[y].pan;
				outputCtlCounter++;

				WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].unknown1C);
				outputCtlCounter += 2;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[alBank->count + y]);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->eadPercussion[y].keyBase);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[alBank->count + y]);
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 0x10) != 0)
				outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));

			WriteLongToBuffer(temporaryCtlBuffer, soundDataOffset, (outputCtlCounter  - soundDataOffset));

			for (int y = 0; y < alBank->countEADPercussion; y++)
			{
				if(alBank->eadPercussion[y].wav.adpcmWave == NULL)
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
				else
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, startPercussionDataOffsets + (y * 0x10));
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 0x10) != 0)
				outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));
		}

		// ctl bank size
		WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (0x8 * soundBankNumber) + 0x4, (outputCtlCounter - instrumentALLookup));

		delete [] instrumentSoundStartLookup;
		delete [] offsetsEnv;
		delete [] offsetsWav;
		delete [] bookOffsetsWav;
		delete [] adpcmRawLoopOffsetsWav;
		delete [] tblOffsets;

		delete [] offsetsWavSecondary;
		delete [] bookOffsetsWavSecondary;
		delete [] adpcmRawLoopOffsetsWavSecondary;
		delete [] tblOffsetsSecondary;

		delete [] offsetsWavPrevious;
		delete [] bookOffsetsWavPrevious;
		delete [] adpcmRawLoopOffsetsWavPrevious;
		delete [] tblOffsetsPrevious;
	}

	unsigned long outputTblCounter = 0;

	WriteShortToBuffer(temporaryTblBuffer, outputTblCounter, 0x0002);
	outputTblCounter += 2;

	WriteShortToBuffer(temporaryTblBuffer, outputTblCounter, results.size());
	outputTblCounter += 2;

	for (int x = 0; x < results.size(); x++)
	{
		// offset to bank later in file
		outputTblCounter += 8;
	}

	if ((outputTblCounter % 0x10) != 0)
		outputTblCounter += ((0x10 - (outputTblCounter % 0x10)));

	//outputTblCounter = (4 + (soundBankNumber * 8));

	unsigned long endTblHeader = outputTblCounter;

	memcpy(&temporaryTblBuffer[outputTblCounter], tblData, tblSizes);

	outputTblCounter += tblSizes;

	if ((outputTblCounter % 0x10) != 0)
		outputTblCounter += ((0x10 - (outputTblCounter % 0x10)));

	for (int x = 0; x < results.size(); x++)
	{
		WriteLongToBuffer(temporaryTblBuffer, 0x4 + (x * 0x8), endTblHeader);
		WriteLongToBuffer(temporaryTblBuffer, 0x4 + (x * 0x8) + 0x4, tblSizes);
	}


	delete[] tblData;

	
	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
}

void CN64AIFCAudio::WriteKonamiADSR(unsigned char* ROM, ALBank*& alBank, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset)
{
	if (endInstrumentOffset != 0xFFFFFFFF)
	{
		for (int x = 0; x < alBank->konamiADSR.size(); x++)
		{
			KonamiADSR adsr = alBank->konamiADSR[x];
			ROM[instrumentOffset + (x * 7)] = adsr.coarseTune;
			ROM[instrumentOffset + (x * 7) + 1] = adsr.fineTune;
			ROM[instrumentOffset + (x * 7) + 2] = adsr.attackTime;
			ROM[instrumentOffset + (x * 7) + 3] = adsr.decayTime;
			ROM[instrumentOffset + (x * 7) + 4] = adsr.releaseTime;
			ROM[instrumentOffset + (x * 7) + 5] = adsr.unknownOne;
			ROM[instrumentOffset + (x * 7) + 6] = adsr.unknownTwo;
		}

		for (int x = 0; x < alBank->konamiDrumsADSR.size(); x++)
		{
			KonamiADSRDrum adsr = alBank->konamiDrumsADSR[x];
			ROM[startDrumOffset + (x * 8)] = adsr.instrumentLookup;
			ROM[startDrumOffset + (x * 8) + 1] = adsr.coarseTune;
			ROM[startDrumOffset + (x * 8) + 2] = adsr.fineTune;
			ROM[startDrumOffset + (x * 8) + 3] = adsr.attackTime;
			ROM[startDrumOffset + (x * 8) + 4] = adsr.decayTime;
			ROM[startDrumOffset + (x * 8) + 5] = adsr.releaseTime;
			ROM[startDrumOffset + (x * 8) + 6] = adsr.unknownOne;
			ROM[startDrumOffset + (x * 8) + 7] = adsr.unknownTwo;
		}
	}
}

void CN64AIFCAudio::WriteKonami8ADSR(unsigned char* ROM, ALBank*& alBank, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset)
{
	if (endInstrumentOffset != 0xFFFFFFFF)
	{
		for (int x = 0; x < alBank->konamiADSR.size(); x++)
		{
			KonamiADSR adsr = alBank->konamiADSR[x];
			ROM[instrumentOffset + (x * 8)] = adsr.coarseTune;
			ROM[instrumentOffset + (x * 8) + 1] = adsr.fineTune;
			ROM[instrumentOffset + (x * 8) + 2] = adsr.attackTime;
			ROM[instrumentOffset + (x * 8) + 3] = adsr.decayTime;
			ROM[instrumentOffset + (x * 8) + 4] = adsr.releaseTime;
			ROM[instrumentOffset + (x * 8) + 5] = adsr.unknownOne;
			ROM[instrumentOffset + (x * 8) + 6] = adsr.unknownTwo;
		}

		for (int x = 0; x < alBank->konamiDrumsADSR.size(); x++)
		{
			KonamiADSRDrum adsr = alBank->konamiDrumsADSR[x];
			ROM[startDrumOffset + (x * 0xA)] = adsr.coarseTune;
			ROM[startDrumOffset + (x * 0xA) + 1] = adsr.fineTune;
			ROM[startDrumOffset + (x * 0xA) + 2] = adsr.attackTime;
			ROM[startDrumOffset + (x * 0xA) + 3] = adsr.decayTime;
			ROM[startDrumOffset + (x * 0xA) + 4] = adsr.releaseTime;
			ROM[startDrumOffset + (x * 0xA) + 5] = adsr.unknownOne;
			ROM[startDrumOffset + (x * 0xA) + 6] = adsr.unknownTwo;
			ROM[startDrumOffset + (x * 0xA) + 9] = adsr.instrumentLookup;
		}
	}
}


void CN64AIFCAudio::WriteAudio(std::vector<ALBank*> alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x2000000];
	for (int x = 0; x < 0x2000000; x++)
	{
		temporaryCtlBuffer[x] = 0;
		temporaryTblBuffer[x] = 0;
	}

	unsigned long outputCtlCounter = 0;
	
	// magic
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4231);
	outputCtlCounter += 2;

	// bank #
	int bankCount = alBank.size();
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, bankCount);
	outputCtlCounter += 2;

	for (int x = 0; x < bankCount; x++)
	{
		// offset to bank later in file
		outputCtlCounter += 4;
	}

	unsigned long outputTblCounter = 0;

	unsigned long* percussionALLookup = new unsigned long[bankCount];
	unsigned long** percussioninstrumentSoundStartLookup = new unsigned long*[bankCount];
	unsigned long** percussionoffsetsEnv = new unsigned long*[bankCount];
	unsigned long** percussionoffsetsKey = new unsigned long*[bankCount];
	unsigned long** percussionoffsetsWav = new unsigned long*[bankCount];
	unsigned long** percussionbookOffsetsWav = new unsigned long*[bankCount];
	unsigned long** percussionadpcmRawLoopOffsetsWav = new unsigned long*[bankCount];
	unsigned long** percussionrawLoopOffsetsWav = new unsigned long*[bankCount];
	unsigned long** percussiontblOffsets = new unsigned long*[bankCount];

	unsigned long** instrumentALLookup = new unsigned long*[bankCount];
	unsigned long*** instrumentSoundStartLookup = new unsigned long**[bankCount];
	unsigned long*** offsetsEnv = new unsigned long**[bankCount];
	unsigned long*** offsetsKey = new unsigned long**[bankCount];
	unsigned long*** offsetsWav = new unsigned long**[bankCount];
	unsigned long*** bookOffsetsWav = new unsigned long**[bankCount];
	unsigned long*** adpcmRawLoopOffsetsWav = new unsigned long**[bankCount];
	unsigned long*** rawLoopOffsetsWav = new unsigned long**[bankCount];
	unsigned long*** tblOffsets = new unsigned long**[bankCount];

	for (int subBank = 0; subBank < bankCount; subBank++)
	{
		ALBank* alBankSub = alBank[subBank];

		percussionALLookup[subBank] = 0;	
		percussioninstrumentSoundStartLookup[subBank] = NULL;
		percussionoffsetsEnv[subBank] = NULL;
		percussionoffsetsKey[subBank] = NULL;
		percussionoffsetsWav[subBank] = NULL;
		percussionbookOffsetsWav[subBank] = NULL;
		percussionadpcmRawLoopOffsetsWav[subBank] = NULL;
		percussionrawLoopOffsetsWav[subBank] = NULL;
		percussiontblOffsets[subBank] = NULL;

		if (alBankSub->percussion != NULL)
		{
			percussioninstrumentSoundStartLookup[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionoffsetsEnv[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionoffsetsKey[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionoffsetsWav[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionbookOffsetsWav[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionadpcmRawLoopOffsetsWav[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussionrawLoopOffsetsWav[subBank] = new unsigned long[alBankSub->percussion->soundCount];
			percussiontblOffsets[subBank] = new unsigned long[alBankSub->percussion->soundCount];
		}


		instrumentALLookup[subBank] = new unsigned long[alBankSub->count];
		instrumentSoundStartLookup[subBank] = new unsigned long*[alBankSub->count];
		offsetsEnv[subBank] = new unsigned long*[alBankSub->count];
		offsetsKey[subBank] = new unsigned long*[alBankSub->count];
		offsetsWav[subBank] = new unsigned long*[alBankSub->count];
		bookOffsetsWav[subBank] = new unsigned long*[alBankSub->count];
		adpcmRawLoopOffsetsWav[subBank] = new unsigned long*[alBankSub->count];
		rawLoopOffsetsWav[subBank] = new unsigned long*[alBankSub->count];
		tblOffsets[subBank] = new unsigned long*[alBankSub->count];

		for (int x = 0; x < alBankSub->count; x++)
		{
			instrumentSoundStartLookup[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			offsetsEnv[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			offsetsKey[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			offsetsWav[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			bookOffsetsWav[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			adpcmRawLoopOffsetsWav[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			rawLoopOffsetsWav[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
			tblOffsets[subBank][x] = new unsigned long[alBankSub->inst[x]->soundCount];
		}

		if (alBankSub->percussion != NULL)
		{
			unsigned long envStart = outputCtlCounter;

			for (int y = 0; y < alBankSub->percussion->soundCount; y++)
			{
				bool foundPast = false;

				int loopEnd = y;
				
				for (int z = 0; z < loopEnd; z++)
				{
					if (CompareALEnv(&alBankSub->percussion->sounds[y]->env, &alBankSub->percussion->sounds[z]->env) == "")
					{
						foundPast = true;
						percussionoffsetsEnv[subBank][y] = percussionoffsetsEnv[subBank][z];
						break;
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					if (alBank[s]->percussion != NULL)
					{
						for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
						{
							if (CompareALEnv(&alBankSub->percussion->sounds[y]->env, &alBank[s]->percussion->sounds[z]->env) == "")
							{
								foundPast = true;
								percussionoffsetsEnv[subBank][y] = percussionoffsetsEnv[s][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					for (int i = 0; i < alBank[s]->count; i++)
					{
						for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
						{
							if (CompareALEnv(&alBankSub->percussion->sounds[y]->env, &alBank[s]->inst[i]->sounds[z]->env) == "")
							{
								foundPast = true;
								percussionoffsetsEnv[subBank][y] = offsetsEnv[s][i][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				percussionoffsetsEnv[subBank][y] = outputCtlCounter;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->env.attackTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->env.decayTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->env.releaseTime);
				outputCtlCounter += 4;
				temporaryCtlBuffer[outputCtlCounter] = alBankSub->percussion->sounds[y]->env.attackVolume;
				outputCtlCounter += 1;
				temporaryCtlBuffer[outputCtlCounter] = alBankSub->percussion->sounds[y]->env.decayVolume;
				outputCtlCounter += 1;
				WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->env.zeroPad);
				outputCtlCounter += 2;
			}
		}

		for (int x = 0; x < alBankSub->count; x++)
		{
			unsigned long envStart = outputCtlCounter;
			for (int y = 0; y < alBankSub->inst[x]->soundCount; y++)
			{
				bool foundPast = false;

				if (alBankSub->percussion != NULL)
				{
					for (int z = 0; z < alBankSub->percussion->soundCount; z++)
					{
						if (CompareALEnv(&alBankSub->inst[x]->sounds[y]->env, &alBankSub->percussion->sounds[z]->env) == "")
						{
							foundPast = true;
							offsetsEnv[subBank][x][y] = percussionoffsetsEnv[subBank][z];
							break;
						}
					}

					if (foundPast)
						continue;
				}

				for (int r = 0; r < (x + 1); r++)
				{
					int loopEnd = y;
					if (r != x)
					{
						loopEnd = alBankSub->inst[r]->soundCount;
					}
					for (int z = 0; z < loopEnd; z++)
					{
						if (CompareALEnv(&alBankSub->inst[x]->sounds[y]->env, &alBankSub->inst[r]->sounds[z]->env) == "")
						{
							foundPast = true;
							offsetsEnv[subBank][x][y] = offsetsEnv[subBank][r][z];
							break;
						}
					}
				}

				if (foundPast)
					continue;


				for (int s = 0; s < (subBank - 1); s++)
				{
					if (alBank[s]->percussion != NULL)
					{
						for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
						{
							if (CompareALEnv(&alBankSub->inst[x]->sounds[y]->env, &alBank[s]->percussion->sounds[z]->env) == "")
							{
								foundPast = true;
								offsetsEnv[subBank][x][y] = percussionoffsetsEnv[s][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					for (int i = 0; i < alBank[s]->count; i++)
					{
						for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
						{
							if (CompareALEnv(&alBankSub->inst[x]->sounds[y]->env, &alBank[s]->inst[i]->sounds[z]->env) == "")
							{
								foundPast = true;
								offsetsEnv[subBank][x][y] = offsetsEnv[s][i][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				offsetsEnv[subBank][x][y] = outputCtlCounter;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->env.attackTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->env.decayTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->env.releaseTime);
				outputCtlCounter += 4;
				temporaryCtlBuffer[outputCtlCounter] = alBankSub->inst[x]->sounds[y]->env.attackVolume;
				outputCtlCounter += 1;
				temporaryCtlBuffer[outputCtlCounter] = alBankSub->inst[x]->sounds[y]->env.decayVolume;
				outputCtlCounter += 1;
				WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->env.zeroPad);
				outputCtlCounter += 2;
			}
		}

		if (alBankSub->percussion != NULL)
		{
			if ((outputCtlCounter % 0x10) != 0)
			{
				outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
			}

			unsigned long keyStart = outputCtlCounter;
			for (int y = 0; y < alBankSub->percussion->soundCount; y++)
			{
				bool foundPast = false;

				int loopEnd = y;
				
				for (int z = 0; z < loopEnd; z++)
				{
					if (CompareALKey(&alBankSub->percussion->sounds[y]->key, &alBankSub->percussion->sounds[z]->key) == "")
					{
						foundPast = true;
						percussionoffsetsKey[subBank][y] = percussionoffsetsKey[subBank][z];
						break;
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					if (alBank[s]->percussion != NULL)
					{
						for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
						{
							if (CompareALKey(&alBankSub->percussion->sounds[y]->key, &alBank[s]->percussion->sounds[z]->key) == "")
							{
								foundPast = true;
								percussionoffsetsKey[subBank][y] = percussionoffsetsKey[s][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					for (int i = 0; i < alBank[s]->count; i++)
					{
						for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
						{
							if (CompareALKey(&alBankSub->percussion->sounds[y]->key, &alBank[s]->inst[i]->sounds[z]->key) == "")
							{
								foundPast = true;
								percussionoffsetsKey[subBank][y] = offsetsKey[s][i][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;


				percussionoffsetsKey[subBank][y] = outputCtlCounter;

				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.velocitymin;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.velocitymax;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.keymin;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.keymax;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.keybase;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->key.detune;

				if ((outputCtlCounter % 8) != 0)
				{
					int pad = 8 - (outputCtlCounter % 8);
					for (int z = 0; z < pad; z++)
					{
						temporaryCtlBuffer[outputCtlCounter++] = 0;
					}
				}
			}


			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}

		for (int x = 0; x < alBankSub->count; x++)
		{
			if ((outputCtlCounter % 0x10) != 0)
			{
				outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
			}

			unsigned long keyStart = outputCtlCounter;
			for (int y = 0; y < alBankSub->inst[x]->soundCount; y++)
			{
				bool foundPast = false;

				if (alBankSub->percussion != NULL)
				{
					for (int z = 0; z < alBankSub->percussion->soundCount; z++)
					{
						if (CompareALKey(&alBankSub->inst[x]->sounds[y]->key, &alBankSub->percussion->sounds[z]->key) == "")
						{
							foundPast = true;
							offsetsKey[subBank][x][y] = percussionoffsetsKey[subBank][z];
							break;
						}
					}

					if (foundPast)
						continue;
				}

				for (int r = 0; r < (x + 1); r++)
				{
					int loopEnd = y;
					if (r != x)
					{
						loopEnd = alBankSub->inst[r]->soundCount;
					}
					for (int z = 0; z < loopEnd; z++)
					{
						if (CompareALKey(&alBankSub->inst[x]->sounds[y]->key, &alBankSub->inst[r]->sounds[z]->key) == "")
						{
							foundPast = true;
							offsetsKey[subBank][x][y] = offsetsKey[subBank][r][z];
							break;
						}
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					if (alBank[s]->percussion != NULL)
					{
						for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
						{
							if (CompareALKey(&alBankSub->inst[x]->sounds[y]->key, &alBank[s]->percussion->sounds[z]->key) == "")
							{
								foundPast = true;
								offsetsKey[subBank][x][y] = percussionoffsetsKey[s][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;

				for (int s = 0; s < (subBank - 1); s++)
				{
					for (int i = 0; i < alBank[s]->count; i++)
					{
						for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
						{
							if (CompareALKey(&alBankSub->inst[x]->sounds[y]->key, &alBank[s]->inst[i]->sounds[z]->key) == "")
							{
								foundPast = true;
								offsetsKey[subBank][x][y] = offsetsKey[s][i][z];
								break;
							}
						}
					}
				}

				if (foundPast)
					continue;


				offsetsKey[subBank][x][y] = outputCtlCounter;

				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.velocitymin;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.velocitymax;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.keymin;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.keymax;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.keybase;
				temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->key.detune;

				if ((outputCtlCounter % 8) != 0)
				{
					int pad = 8 - (outputCtlCounter % 8);
					for (int z = 0; z < pad; z++)
					{
						temporaryCtlBuffer[outputCtlCounter++] = 0;
					}
				}
			}


			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}

		if (alBankSub->percussion != NULL)
		{
			unsigned long wavStart = outputCtlCounter;

			for (int y = 0; y < alBankSub->percussion->soundCount; y++)
			{
				percussioninstrumentSoundStartLookup[subBank][y] = outputCtlCounter;

				percussionoffsetsWav[subBank][y] = outputCtlCounter + 0x10;

				bool entireSoundMatch = false;

				int entireMatch = -1;
				
				int loopEnd = y;
				
				for (int w = 0; w < loopEnd; w++)
				{
					if (CompareALWav(&alBankSub->percussion->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
					{
						entireMatch = w;
						percussionoffsetsWav[subBank][y] = percussionoffsetsWav[subBank][entireMatch];
						percussionbookOffsetsWav[subBank][y] = percussionbookOffsetsWav[subBank][entireMatch];
						percussiontblOffsets[subBank][y] = percussiontblOffsets[subBank][entireMatch];

						if (CompareALSound(alBankSub->percussion->sounds[y], alBankSub->percussion->sounds[entireMatch]) == "")
						{
							percussioninstrumentSoundStartLookup[subBank][y] = percussioninstrumentSoundStartLookup[subBank][entireMatch];
							entireSoundMatch = true;
							break;
						}
						else
						{
							continue;
						}
					}
				}

				if (!entireSoundMatch)
				{
					for (int s = 0; s < (subBank - 1); s++)
					{
						if (alBank[s]->percussion != NULL)
						{
							for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
							{
								if (CompareALWav(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
								{
									entireMatch = z;
									percussionoffsetsWav[subBank][y] = percussionoffsetsWav[s][z];
									percussionbookOffsetsWav[subBank][y] = percussionbookOffsetsWav[s][z];
									percussiontblOffsets[subBank][y] = percussiontblOffsets[s][z];

									if (CompareALSound(alBankSub->percussion->sounds[y], alBank[s]->percussion->sounds[z]) == "")
									{
										percussioninstrumentSoundStartLookup[subBank][y] = percussioninstrumentSoundStartLookup[s][y];
										entireSoundMatch = true;
										break;
									}
									else
									{
										continue;
									}
								}
							}
						}
					}
				}

				if (!entireSoundMatch)
				{
					for (int s = 0; s < (subBank - 1); s++)
					{
						for (int i = 0; i < alBank[s]->count; i++)
						{
							for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
							{
								if (CompareALWav(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
								{
									entireMatch = z;
									percussionoffsetsWav[subBank][y] = offsetsWav[s][i][entireMatch];
									percussionbookOffsetsWav[subBank][y] = bookOffsetsWav[s][i][entireMatch];
									percussiontblOffsets[subBank][y] = tblOffsets[s][i][entireMatch];
									
									if (CompareALSound(alBankSub->percussion->sounds[y], alBank[s]->percussion->sounds[z]) == "")
									{
										percussioninstrumentSoundStartLookup[subBank][y] = instrumentSoundStartLookup[s][i][z];
										entireSoundMatch = true;
										break;
									}
									else
									{
										continue;
									}
								}
							}
						}
					}
				}
				

				if (!entireSoundMatch)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussionoffsetsEnv[subBank][y]);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussionoffsetsKey[subBank][y]);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussionoffsetsWav[subBank][y]);
					outputCtlCounter += 4;

					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->samplePan;
					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->sampleVolume;
					WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
					outputCtlCounter += 2;
				}

				if (entireMatch == -1)
				{
				
					percussiontblOffsets[subBank][y] = outputTblCounter;

					int foundSameTbl = false;

					int loopEnd = y;
						
					for (int z = 0; z < loopEnd; z++)
					{
						if (CompareALTbl(&alBankSub->percussion->sounds[y]->wav, &alBankSub->percussion->sounds[z]->wav, true) == "")
						{
							foundSameTbl = true;
							percussiontblOffsets[subBank][y] = percussiontblOffsets[subBank][z];
							break;
						}
						else
						{
							continue;
						}
					}

					for (int s = 0; s < (subBank - 1); s++)
					{
						if (alBank[s]->percussion != NULL)
						{
							for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
							{
								if (CompareALTbl(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
								{
									foundSameTbl = true;
									percussiontblOffsets[subBank][y] = percussiontblOffsets[s][z];
									break;
								}
							}
						}
					}

					for (int s = 0; s < (subBank - 1); s++)
					{
						for (int i = 0; i < alBank[s]->count; i++)
						{
							for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
							{
								if (CompareALTbl(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
								{
									foundSameTbl = true;
									percussiontblOffsets[subBank][y] = tblOffsets[s][i][z];
									break;
								}
							}
						}
					}

					// update base
					//alBankSub->percussion->sounds[y]->wav.base = percussiontblOffsets[subBank][x][y];

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussiontblOffsets[subBank][y]);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.len);
					outputCtlCounter += 4;

					if (!foundSameTbl)
					{
						for (int z = 0; z < alBankSub->percussion->sounds[y]->wav.len; z++)
						{
							temporaryTblBuffer[outputTblCounter + z] = alBankSub->percussion->sounds[y]->wav.wavData[z];
						}
						outputTblCounter += alBankSub->percussion->sounds[y]->wav.len;

						if ((outputTblCounter % 8) != 0)
						{
							int pad = 8 - (outputTblCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryTblBuffer[outputTblCounter++] = 0;
							}
						}
					}

					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->sounds[y]->wav.type;
					temporaryCtlBuffer[outputCtlCounter++] = 0; // offset type
					temporaryCtlBuffer[outputCtlCounter++] = 0; // pad
					temporaryCtlBuffer[outputCtlCounter++] = 0; // pad

					if (alBankSub->percussion->sounds[y]->wav.type == AL_ADPCM_WAVE)
					{
						int sameLoop = -1;

						if (alBankSub->percussion->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							if (sameLoop != -1)
								break;

							int loopEnd = y;
							
							for (int w = 0; w < loopEnd; w++)
							{
								if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
								{
									percussionadpcmRawLoopOffsetsWav[subBank][y] = percussionadpcmRawLoopOffsetsWav[subBank][w];
									sameLoop = w;
									break;
								}
							}
						}

						for (int s = 0; s < (subBank - 1); s++)
						{
							if (alBank[s]->percussion != NULL)
							{
								for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
								{
									if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
									{
										percussionadpcmRawLoopOffsetsWav[subBank][y] = percussionadpcmRawLoopOffsetsWav[s][z];
										sameLoop = z;
										break;
									}
								}
							}
						}

						for (int s = 0; s < (subBank - 1); s++)
						{
							for (int i = 0; i < alBank[s]->count; i++)
							{
								for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
								{
									if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
									{
										percussionadpcmRawLoopOffsetsWav[subBank][y] = adpcmRawLoopOffsetsWav[s][i][z];
										sameLoop = z;
									}
								}
							}
						}

						unsigned long loopWriteSpot = outputCtlCounter;
						unsigned long bookWriteSpot = outputCtlCounter + 4;
						 
						int same = -1;
						
						if (same != -1)
							break;

						int loopEnd = y;
						
						for (int w = 0; w < loopEnd; w++)
						{
							if (alBankSub->percussion->sounds[w]->wav.adpcmWave != NULL)
							{
								if (CompareALBook(&alBankSub->percussion->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
								{
									percussionbookOffsetsWav[subBank][y] = percussionbookOffsetsWav[subBank][w];
									same = w;
									break;
								}
							}
						}

						for (int s = 0; s < (subBank - 1); s++)
						{
							if (alBank[s]->percussion != NULL)
							{
								for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
								{
									if (CompareALBook(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
									{
										percussionbookOffsetsWav[subBank][y] = percussionbookOffsetsWav[s][z];
										same = z;
										break;
									}
								}
							}
						}

						for (int s = 0; s < (subBank - 1); s++)
						{
							for (int i = 0; i < alBank[s]->count; i++)
							{
								for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
								{
									if (CompareALBook(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
									{
										percussionbookOffsetsWav[subBank][y] = bookOffsetsWav[s][i][z];
										same = z;
										break;
									}
								}
							}
						}

						// write later loop/predictor offsets
						outputCtlCounter += 8;


						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}

						if (alBankSub->percussion->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							if (sameLoop == -1)
							{
								percussionadpcmRawLoopOffsetsWav[subBank][y] = outputCtlCounter;
								WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.adpcmWave->loop->start);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.adpcmWave->loop->end);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.adpcmWave->loop->count);
								outputCtlCounter += 4;
								for (int z = 0; z < 0x10; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->percussion->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->percussion->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, percussionadpcmRawLoopOffsetsWav[subBank][y]);
							}
						}
						else
						{
							// null loop
							WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
						}

						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}

						if (same == -1)
						{
							percussionbookOffsetsWav[subBank][y] = outputCtlCounter;
							WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, outputCtlCounter);

							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.adpcmWave->book->order);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.adpcmWave->book->npredictors);
							outputCtlCounter += 4;

							for (int z = 0; z < (alBankSub->percussion->sounds[y]->wav.adpcmWave->book->order * alBankSub->percussion->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->percussion->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->percussion->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
							}

							
							// game does this not sure why
							/*int pad = 8;
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}*/
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, percussionbookOffsetsWav[subBank][y]);
						}
					}
					else if (alBankSub->percussion->sounds[y]->wav.type == AL_RAW16_WAVE)
					{

						if (alBankSub->percussion->sounds[y]->wav.rawWave->loop != NULL)
						{
							int same = -1;

							int loopEnd = y;
							
							for (int w = 0; w < loopEnd; w++)
							{
								if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
								{
									percussionrawLoopOffsetsWav[subBank][y] = percussionrawLoopOffsetsWav[subBank][w];
									same = w;
									break;
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								if (alBank[s]->percussion != NULL)
								{
									for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
									{
										if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
										{
											percussionrawLoopOffsetsWav[subBank][y] = percussionrawLoopOffsetsWav[s][z];
											same = z;
											break;
										}
									}
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								for (int i = 0; i < alBank[s]->count; i++)
								{
									for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
									{
										if (CompareALLoop(&alBankSub->percussion->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
										{
											percussionrawLoopOffsetsWav[subBank][y] = rawLoopOffsetsWav[s][i][z];
											same = z;
											break;
										}
									}
								}
							}

							if (same == -1)
							{
								unsigned long loopWriteSpot = outputCtlCounter;

								outputCtlCounter += 4;

								if ((outputCtlCounter % 8) != 0)
								{
									int pad = 8 - (outputCtlCounter % 8);
									for (int z = 0; z < pad; z++)
									{
										temporaryCtlBuffer[outputCtlCounter++] = 0;
									}
								}

								percussionrawLoopOffsetsWav[subBank][y] = (outputCtlCounter);
								WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.rawWave->loop->start);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.rawWave->loop->end);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->sounds[y]->wav.rawWave->loop->count);
								outputCtlCounter += 4;
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussionrawLoopOffsetsWav[subBank][y]);
								outputCtlCounter += 4;
							}
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
							outputCtlCounter += 4;
						}

						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}
					}
				}
			}

			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}







		for (int x = 0; x < alBankSub->count; x++)
		{
			if (alBankSub->percussion != NULL)
			{
				CString percussionIdentical = CompareALInstrument(alBankSub->inst[x], alBankSub->percussion);
				if (percussionIdentical == "")
				{
					continue;
				}
			}

			int instrumentMatch = -1;
			for (int xx = 0; xx < x; xx++)
			{
				CString instrumentIdentical = CompareALInstrument(alBankSub->inst[x], alBankSub->inst[xx]);
				if (instrumentIdentical == "")
				{
					instrumentMatch = xx;
					break;
				}
			}

			if (instrumentMatch != -1)
			{
				for (int y = 0; y < alBankSub->inst[x]->soundCount; y++)
				{
					instrumentSoundStartLookup[subBank][x][y] = instrumentSoundStartLookup[subBank][instrumentMatch][y];
					offsetsWav[subBank][x][y] = offsetsWav[subBank][instrumentMatch][y];
					offsetsEnv[subBank][x][y] = offsetsEnv[subBank][instrumentMatch][y];
					offsetsKey[subBank][x][y] = offsetsKey[subBank][instrumentMatch][y];
					bookOffsetsWav[subBank][x][y] = bookOffsetsWav[subBank][instrumentMatch][y];
					tblOffsets[subBank][x][y] = tblOffsets[subBank][instrumentMatch][y];

					if (alBankSub->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
					{
						adpcmRawLoopOffsetsWav[subBank][x][y] = adpcmRawLoopOffsetsWav[subBank][instrumentMatch][y];
					}
					else if (alBankSub->inst[x]->sounds[y]->wav.type == AL_RAW16_WAVE)
					{
						rawLoopOffsetsWav[subBank][x][y] = rawLoopOffsetsWav[subBank][instrumentMatch][y];
					}
				}
				continue;
			}

			unsigned long wavStart = outputCtlCounter;

			for (int y = 0; y < alBankSub->inst[x]->soundCount; y++)
			{
				instrumentSoundStartLookup[subBank][x][y] = outputCtlCounter;

				offsetsWav[subBank][x][y] = outputCtlCounter + 0x10;

				bool entireSoundMatch = false;

				int entireMatchBank = -1;
				int entireMatch = -1;
				for (int r = 0; r < (x + 1); r++)
				{
					int loopEnd = y;
					if (r != x)
					{
						loopEnd = alBankSub->inst[r]->soundCount;
					}

					for (int w = 0; w < loopEnd; w++)
					{
						if (CompareALWav(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->inst[r]->sounds[w]->wav) == "")
						{

							entireMatchBank = r;
							entireMatch = w;
							offsetsWav[subBank][x][y] = offsetsWav[subBank][entireMatchBank][entireMatch];
							bookOffsetsWav[subBank][x][y] = bookOffsetsWav[subBank][entireMatchBank][entireMatch];
							tblOffsets[subBank][x][y] = tblOffsets[subBank][entireMatchBank][entireMatch];

							if (CompareALSound(alBankSub->inst[x]->sounds[y], alBankSub->inst[entireMatchBank]->sounds[entireMatch]) == "")
							{
								instrumentSoundStartLookup[subBank][x][y] = instrumentSoundStartLookup[subBank][entireMatchBank][entireMatch];
								entireSoundMatch = true;
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}

				if (!entireSoundMatch)
				{
					for (int s = 0; s < (subBank - 1); s++)
					{
						if (alBank[s]->percussion != NULL)
						{
							for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
							{
								if (CompareALWav(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
								{
									entireMatchBank = s;
									entireMatch = z;
									offsetsWav[subBank][x][y] = percussionoffsetsWav[s][z];
									bookOffsetsWav[subBank][x][y] = percussionbookOffsetsWav[s][z];
									tblOffsets[subBank][x][y] = percussiontblOffsets[s][z];

									if (CompareALSound(alBankSub->inst[x]->sounds[y], alBank[s]->percussion->sounds[z]) == "")
									{
										instrumentSoundStartLookup[subBank][x][y] = percussioninstrumentSoundStartLookup[s][z];
										entireSoundMatch = true;
										break;
									}
									else
									{
										continue;
									}
								}
							}
						}
					}
				}
				
				if (!entireSoundMatch)
				{
					for (int s = 0; s < (subBank - 1); s++)
					{
						for (int i = 0; i < alBank[s]->count; i++)
						{
							for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
							{
								if (CompareALWav(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
								{
									entireMatchBank = i;
									entireMatch = z;
									offsetsWav[subBank][x][y] = offsetsWav[s][i][z];
									bookOffsetsWav[subBank][x][y] = bookOffsetsWav[s][i][z];
									tblOffsets[subBank][x][y] = tblOffsets[s][i][z];

									if (CompareALSound(alBankSub->inst[x]->sounds[y], alBank[s]->inst[i]->sounds[z]) == "")
									{
										instrumentSoundStartLookup[subBank][x][y] = instrumentSoundStartLookup[s][i][z];
										entireSoundMatch = true;
										break;
									}
									else
									{
										continue;
									}
								}
							}
						}
					}
				}


				if (!entireSoundMatch)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[subBank][x][y]);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsKey[subBank][x][y]);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[subBank][x][y]);
					outputCtlCounter += 4;

					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->samplePan;
					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->sampleVolume;
					WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
					outputCtlCounter += 2;
				}

				if (entireMatch == -1)
				{
				
					tblOffsets[subBank][x][y] = outputTblCounter;

					int foundSameTbl = false;

					for (int r = 0; r < (x + 1); r++)
					{
						int loopEnd = y;
						if (r != x)
						{
							loopEnd = alBankSub->inst[r]->soundCount;
						}
						for (int z = 0; z < loopEnd; z++)
						{
							if (CompareALTbl(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->inst[r]->sounds[z]->wav, true) == "")
							{
								foundSameTbl = true;
								tblOffsets[subBank][x][y] = tblOffsets[subBank][r][z];
								break;
							}
						}
					}

					for (int s = 0; s < (subBank - 1); s++)
					{
						if (alBank[s]->percussion != NULL)
						{
							for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
							{
								if (CompareALTbl(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
								{
									foundSameTbl = true;
									tblOffsets[subBank][x][y] = percussiontblOffsets[s][z];
									break;
								}
							}
						}
					}

					for (int s = 0; s < (subBank - 1); s++)
					{
						for (int i = 0; i < alBank[s]->count; i++)
						{
							for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
							{
								if (CompareALTbl(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
								{
									foundSameTbl = true;
									tblOffsets[subBank][x][y] = tblOffsets[s][i][z];
									break;
								}
							}
						}
					}

					// update base
					//alBankSub->inst[x]->sounds[y]->wav.base = tblOffsets[subBank][x][y];

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[subBank][x][y]);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.len);
					outputCtlCounter += 4;

					if (!foundSameTbl)
					{
						for (int z = 0; z < alBankSub->inst[x]->sounds[y]->wav.len; z++)
						{
							temporaryTblBuffer[outputTblCounter + z] = alBankSub->inst[x]->sounds[y]->wav.wavData[z];
						}
						outputTblCounter += alBankSub->inst[x]->sounds[y]->wav.len;

						if ((outputTblCounter % 8) != 0)
						{
							int pad = 8 - (outputTblCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryTblBuffer[outputTblCounter++] = 0;
							}
						}
					}

					temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[x]->sounds[y]->wav.type;
					temporaryCtlBuffer[outputCtlCounter++] = 0; // offset type
					temporaryCtlBuffer[outputCtlCounter++] = 0; // pad
					temporaryCtlBuffer[outputCtlCounter++] = 0; // pad

					if (alBankSub->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
					{
						int sameLoop = -1;
						int sameLoopBank = -1;


						if (alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (sameLoop != -1)
									break;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBankSub->inst[r]->soundCount;
								}
								for (int w = 0; w < loopEnd; w++)
								{
									if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->inst[r]->sounds[w]->wav) == "")
									{
										adpcmRawLoopOffsetsWav[subBank][x][y] = adpcmRawLoopOffsetsWav[subBank][r][w];
										sameLoopBank = r;
										sameLoop = w;
										break;
									}
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								if (alBank[s]->percussion != NULL)
								{
									for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
									{
										if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
										{
											adpcmRawLoopOffsetsWav[subBank][x][y] = percussionadpcmRawLoopOffsetsWav[s][z];
											sameLoopBank = s;
											sameLoop = z;
											break;
										}
									}
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								for (int i = 0; i < alBank[s]->count; i++)
								{
									for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
									{
										if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
										{
											adpcmRawLoopOffsetsWav[subBank][x][y] = adpcmRawLoopOffsetsWav[s][i][z];
											sameLoopBank = i;
											sameLoop = z;
											break;
										}
									}
								}
							}
						}

						unsigned long loopWriteSpot = outputCtlCounter;
						unsigned long bookWriteSpot = outputCtlCounter + 4;
						 
						int same = -1;
						int sameBank = -1;

						for (int r = 0; r < (x + 1); r++)
						{
							if (same != -1)
								break;

							int loopEnd = y;
							if (r != x)
							{
								loopEnd = alBankSub->inst[r]->soundCount;
							}
							for (int w = 0; w < loopEnd; w++)
							{
								if (alBankSub->inst[r]->sounds[w]->wav.adpcmWave != NULL)
								{
									if (CompareALBook(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->inst[r]->sounds[w]->wav) == "")
									{
										sameBank = r;
										same = w;
										bookOffsetsWav[subBank][x][y] = bookOffsetsWav[subBank][r][w];
										break;
									}
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								if (alBank[s]->percussion != NULL)
								{
									for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
									{
										if (CompareALBook(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
										{
											sameBank = s;
											same = z;
											bookOffsetsWav[subBank][x][y] = percussionbookOffsetsWav[s][z];
											break;
										}
									}
								}
							}

							for (int s = 0; s < (subBank - 1); s++)
							{
								for (int i = 0; i < alBank[s]->count; i++)
								{
									for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
									{
										if (CompareALBook(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
										{
											sameBank = i;
											same = z;
											bookOffsetsWav[subBank][x][y] = bookOffsetsWav[s][i][z];
											break;
										}
									}
								}
							}
						}

						// write later loop/predictor offsets
						outputCtlCounter += 8;


						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}

						if (alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							if (sameLoop == -1)
							{
								bool foundSameInPercussion = false;
								if (alBankSub->percussion != NULL)
								{
									for (int w = 0; w < alBankSub->percussion->soundCount; w++)
									{
										if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
										{
											adpcmRawLoopOffsetsWav[subBank][x][y] = percussionadpcmRawLoopOffsetsWav[subBank][w];

											foundSameInPercussion = true;
											break;
										}
									}
								}
								
								if (!foundSameInPercussion)
								{
									adpcmRawLoopOffsetsWav[subBank][x][y] = outputCtlCounter;
									WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

									WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
									outputCtlCounter += 4;
									WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop->end);
									outputCtlCounter += 4;
									WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop->count);
									outputCtlCounter += 4;
									for (int z = 0; z < 0x10; z++)
									{
										temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
										temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
									}
								}
								else
								{
									WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[subBank][x][y]);
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[subBank][x][y]);
							}
						}
						else
						{
							// null loop
							WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
						}

						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}

						if (same == -1)
						{
							bool foundSameInPercussion = false;

							if (alBankSub->percussion != NULL)
							{
								for (int w = 0; w < alBankSub->percussion->soundCount; w++)
								{
									if (alBankSub->percussion->sounds[w]->wav.adpcmWave != NULL)
									{
										if (CompareALBook(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->percussion->sounds[w]->wav) == "")
										{
											bookOffsetsWav[subBank][x][y] = percussionbookOffsetsWav[subBank][w];
											foundSameInPercussion = true;
											break;
										}
									}
								}
							}

							if (!foundSameInPercussion)
							{
								bookOffsetsWav[subBank][x][y] = outputCtlCounter;
								WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, outputCtlCounter);

								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->order);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors);
								outputCtlCounter += 4;

								for (int z = 0; z < (alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBankSub->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[subBank][x][y]);
							}

							
							// game does this not sure why
							/*int pad = 8;
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}*/
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[subBank][x][y]);
						}
					}
					else if (alBankSub->inst[x]->sounds[y]->wav.type == AL_RAW16_WAVE)
					{

						if (alBankSub->inst[x]->sounds[y]->wav.rawWave->loop != NULL)
						{
							int sameBank = -1;
							int same = -1;

							for (int r = 0; r < (x + 1); r++)
							{
								if (same != -1)
									break;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBankSub->inst[r]->soundCount;
								}
								for (int w = 0; w < loopEnd; w++)
								{
									if ((alBankSub->inst[r]->sounds[w]->wav.rawWave != NULL) && (alBankSub->inst[r]->sounds[w]->wav.rawWave->loop != NULL))
									{
										if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBankSub->inst[r]->sounds[w]->wav) == "")
										{
											sameBank = r;
											rawLoopOffsetsWav[subBank][x][y] = rawLoopOffsetsWav[subBank][r][w];
											same = w;
											break;
										}
									}
								}

								for (int s = 0; s < (subBank - 1); s++)
								{
									if (alBank[s]->percussion != NULL)
									{
										for (int z = 0; z < alBank[s]->percussion->soundCount; z++)
										{
											if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->percussion->sounds[z]->wav) == "")
											{
												sameBank = s;
												rawLoopOffsetsWav[subBank][x][y] = percussionrawLoopOffsetsWav[s][z];
												same = z;
												break;
											}
										}
									}
								}

								for (int s = 0; s < (subBank - 1); s++)
								{
									for (int i = 0; i < alBank[s]->count; i++)
									{
										for (int z = 0; z < alBank[s]->inst[i]->soundCount; z++)
										{
											if (CompareALLoop(&alBankSub->inst[x]->sounds[y]->wav, &alBank[s]->inst[i]->sounds[z]->wav) == "")
											{
												sameBank = i;
												rawLoopOffsetsWav[subBank][x][y] = rawLoopOffsetsWav[s][i][z];
												same = z;
												break;
											}
										}
									}
								}
							}

							if (same == -1)
							{
								unsigned long loopWriteSpot = outputCtlCounter;

								outputCtlCounter += 4;

								if ((outputCtlCounter % 8) != 0)
								{
									int pad = 8 - (outputCtlCounter % 8);
									for (int z = 0; z < pad; z++)
									{
										temporaryCtlBuffer[outputCtlCounter++] = 0;
									}
								}

								rawLoopOffsetsWav[subBank][x][y] = (outputCtlCounter);
								WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.rawWave->loop->start);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.rawWave->loop->end);
								outputCtlCounter += 4;
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[x]->sounds[y]->wav.rawWave->loop->count);
								outputCtlCounter += 4;
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, rawLoopOffsetsWav[subBank][x][y]);
								outputCtlCounter += 4;
							}
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
							outputCtlCounter += 4;
						}

						if ((outputCtlCounter % 8) != 0)
						{
							int pad = 8 - (outputCtlCounter % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = 0;
							}
						}
					}
				}
			}

			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}

		unsigned long startALSound = outputCtlCounter;

		if (alBankSub->percussion != NULL)
		{
			percussionALLookup[subBank] = outputCtlCounter;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->volume;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->pan;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->priority;
			temporaryCtlBuffer[outputCtlCounter++] = 0; //offset type
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->tremType;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->tremRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->tremDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->tremDelay;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->vibType;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->vibRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->vibDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->percussion->vibDelay;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->bendRange);
			outputCtlCounter += 2;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->percussion->soundCount);
			outputCtlCounter += 2;
			
			for (int z = 0; z < alBankSub->percussion->soundCount; z++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (percussioninstrumentSoundStartLookup[subBank][z]));
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}

		for (int y = 0; y < alBankSub->count; y++)
		{
			int instrumentMatch = -1;
			for (int xx = 0; xx < y; xx++)
			{
				CString instrumentIdentical = CompareALInstrument(alBankSub->inst[y], alBankSub->inst[xx]);
				if (instrumentIdentical == "")
				{
					instrumentMatch = xx;
					break;
				}
			}

			if (instrumentMatch != -1)
			{
				instrumentALLookup[subBank][y] = instrumentALLookup[subBank][instrumentMatch];
				continue;
			}
			else
			{
				if (alBankSub->percussion != NULL)
				{
					CString percussionIdentical = CompareALInstrument(alBankSub->inst[y], alBankSub->percussion);
					if (percussionIdentical == "")
					{
						instrumentALLookup[subBank][y] = percussionALLookup[subBank];
						continue;
					}
				}
			}

			instrumentALLookup[subBank][y] = outputCtlCounter;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->volume;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->pan;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->priority;
			temporaryCtlBuffer[outputCtlCounter++] = 0; //offset type
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->tremType;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->tremRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->tremDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->tremDelay;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->vibType;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->vibRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->vibDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBankSub->inst[y]->vibDelay;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[y]->bendRange);
			outputCtlCounter += 2;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->inst[y]->soundCount);
			outputCtlCounter += 2;
			
			for (int z = 0; z < alBankSub->inst[y]->soundCount; z++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (instrumentSoundStartLookup[subBank][y][z]));
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}

		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		unsigned long startBank = outputCtlCounter;
		WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (subBank * 4), startBank);

		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->count);
		outputCtlCounter += 2;
		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0); // offset type
		outputCtlCounter += 2;
		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBankSub->pad);
		outputCtlCounter += 2;
		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, (alBankSub->samplerate));
		outputCtlCounter += 2;

		if (alBankSub->percussion != NULL)
		{
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, percussionALLookup[subBank]);
			outputCtlCounter += 4;
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
			outputCtlCounter += 4;
		}

		for (int y = 0; y < alBankSub->count; y++)
		{
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, instrumentALLookup[subBank][y]);
			outputCtlCounter += 4;
		}
		outputCtlCounter += 4;

		if ((outputCtlCounter % 0x10) != 0)
		{
			int pad = 0x10 - (outputCtlCounter % 0x10);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}
	}
	
	for (int subBank = 0; subBank < bankCount; subBank++)
	{
		ALBank* alBankSub = alBank[subBank];

		if (alBankSub->percussion != NULL)
		{
			delete [] percussioninstrumentSoundStartLookup[subBank];
			delete [] percussionoffsetsEnv[subBank];
			delete [] percussionoffsetsKey[subBank];
			delete [] percussionoffsetsWav[subBank];
			delete [] percussionbookOffsetsWav[subBank];
			delete [] percussionadpcmRawLoopOffsetsWav[subBank];
			delete [] percussionrawLoopOffsetsWav[subBank];
			delete [] percussiontblOffsets[subBank];
		}

		for (int x = 0; x < alBankSub->count; x++)
		{
			delete [] instrumentSoundStartLookup[subBank][x];
			delete [] offsetsEnv[subBank][x];
			delete [] offsetsKey[subBank][x];
			delete [] offsetsWav[subBank][x];
			delete [] bookOffsetsWav[subBank][x];
			delete [] adpcmRawLoopOffsetsWav[subBank][x];
			delete [] rawLoopOffsetsWav[subBank][x];
			delete [] tblOffsets[subBank][x];
		}

		delete [] instrumentALLookup[subBank];
		delete [] instrumentSoundStartLookup[subBank];
		delete [] offsetsEnv[subBank];
		delete [] offsetsKey[subBank];
		delete [] offsetsWav[subBank];
		delete [] bookOffsetsWav[subBank];
		delete [] adpcmRawLoopOffsetsWav[subBank];
		delete [] rawLoopOffsetsWav[subBank];
		delete [] tblOffsets[subBank];
	}

	delete [] percussioninstrumentSoundStartLookup;
	delete [] percussionoffsetsEnv;
	delete [] percussionoffsetsKey;
	delete [] percussionoffsetsWav;
	delete [] percussionbookOffsetsWav;
	delete [] percussionadpcmRawLoopOffsetsWav;
	delete [] percussionrawLoopOffsetsWav;
	delete [] percussiontblOffsets;

	delete [] instrumentALLookup;
	delete [] instrumentSoundStartLookup;
	delete [] offsetsEnv;
	delete [] offsetsKey;
	delete [] offsetsWav;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] rawLoopOffsetsWav;
	delete [] tblOffsets;
	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
}

void CN64AIFCAudio::WriteAudioNinDec(std::vector<ALBank*> alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* ctlTemp = NULL;
	int ctlSizeTemp = 0;

	WriteAudio(alBank, ctlTemp, ctlSizeTemp, tbl, tblSize);

	unsigned char* outputBuffer = new unsigned char[0x800000];
	CNinDec ninEnc;
	int compSize = ninEnc.encode(ctlTemp, ctlSizeTemp, outputBuffer);

	delete [] ctlTemp;

	if (compSize > 0)
	{
		ctl = new unsigned char[compSize];
		memcpy(ctl, outputBuffer, compSize);
		ctlSize = compSize;

		delete [] outputBuffer;
	}
	else
	{
		delete [] outputBuffer;
		MessageBox(NULL, "Error writing output temp file to disk", "Error", NULL);
	}
}

void CN64AIFCAudio::WriteAudioConker(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, int ctlOffsetPartTwo, unsigned char*& ctl2, int& ctlSize2)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x2000000];
	unsigned char* temporaryCtlBuffer2 = new unsigned char[0x2000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x4000000];
	for (int x = 0; x < 0x2000000; x++)
	{
		temporaryCtlBuffer[x] = 0;
		temporaryCtlBuffer2[x] = 0;
	}

	for (int x = 0; x < 0x4000000; x++)
	{
		temporaryTblBuffer[x] = 0;
	}
	unsigned long outputCtlCounter = 0;
	unsigned long outputCtlCounter2 = 0;
	
	// magic
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4231);
	outputCtlCounter += 2;

	// bank #
	int bankCount = 1;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, bankCount);
	outputCtlCounter += 2;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000008);
	outputCtlCounter += 4;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
	outputCtlCounter += 2;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
	outputCtlCounter += 2;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
	outputCtlCounter += 2;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->samplerate);
	outputCtlCounter += 2;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
	outputCtlCounter += 4;

	unsigned long writeSoundOffset = outputCtlCounter;
	outputCtlCounter += (alBank->count * 4);


	unsigned long outputTblCounter = 0;

	std::vector<unsigned long> waveDataLength;
	std::vector<unsigned long> offsetWaveData;

	for (int instrumentIndex = 0; instrumentIndex < alBank->count; instrumentIndex++)
	{
		if (instrumentIndex == 0)
		{
			WriteLongToBuffer(temporaryCtlBuffer, writeSoundOffset, outputCtlCounter);

			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->volume;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->pan;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->priority;
			temporaryCtlBuffer[outputCtlCounter++] = 0; //offset type
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->tremType;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->tremRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->tremDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->tremDelay;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->vibType;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->vibRate;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->vibDepth;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[instrumentIndex]->vibDelay;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[instrumentIndex]->bendRange);
			outputCtlCounter += 2;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[instrumentIndex]->soundCount);
			outputCtlCounter += 2;	
		}
		else
		{
			WriteLongToBuffer(temporaryCtlBuffer, writeSoundOffset, ((((outputCtlCounter2 >> 3) << 8)) | alBank->inst[instrumentIndex]->sounds[0]->unknown1));

			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->volume;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->pan;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->priority;
			temporaryCtlBuffer2[outputCtlCounter2++] = 0; //offset type
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->tremType;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->tremRate;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->tremDepth;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->tremDelay;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->vibType;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->vibRate;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->vibDepth;
			temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->vibDelay;
			WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->bendRange);
			outputCtlCounter2 += 2;
			WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->soundCount);
			outputCtlCounter2 += 2;	
		}

		writeSoundOffset += 4;
		
		

		if (instrumentIndex == 0)
		{
			for (int y = 0; y < alBank->inst[instrumentIndex]->soundCount; y++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (((outputCtlCounter2 >> 3) << 8) | alBank->inst[instrumentIndex]->sounds[y]->unknown1));
				outputCtlCounter += 4;

				unsigned long startOffset = outputCtlCounter2;

				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x00000010);
				outputCtlCounter2 += 4;

				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x00000020);
				outputCtlCounter2 += 4;

				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x00000028);
				outputCtlCounter2 += 4;

				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->samplePan;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->sampleVolume;
				WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0);
				outputCtlCounter2 += 2;

				// Env
				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.attackTime);
				outputCtlCounter2 += 4;
				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.decayTime);
				outputCtlCounter2 += 4;
				WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.releaseTime);
				outputCtlCounter2 += 4;
				temporaryCtlBuffer2[outputCtlCounter2] = alBank->inst[instrumentIndex]->sounds[y]->env.attackVolume;
				outputCtlCounter2 += 1;
				temporaryCtlBuffer2[outputCtlCounter2] = alBank->inst[instrumentIndex]->sounds[y]->env.decayVolume;
				outputCtlCounter2 += 1;
				WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.zeroPad);
				outputCtlCounter2 += 2;


				// Key
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.velocitymin;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.velocitymax;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keymin;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keymax;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keybase;
				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.detune;

				WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x0000);
				outputCtlCounter2 += 2;

				int waveDataIndex = -1;
				for (int z = 0; z < offsetWaveData.size(); z++)
				{
					if (
						(CompareBytes(alBank->inst[instrumentIndex]->sounds[y]->wav.wavData, alBank->inst[instrumentIndex]->sounds[y]->wav.len, &temporaryTblBuffer[offsetWaveData[z]], waveDataLength[z]))
						)
					{
						waveDataIndex = z;
						break;
					}
				}

				if (waveDataIndex == -1)
				{
					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, outputTblCounter);
					outputCtlCounter2 += 4;
					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
					outputCtlCounter2 += 4;

					waveDataLength.push_back(alBank->inst[instrumentIndex]->sounds[y]->wav.len);
					offsetWaveData.push_back(outputTblCounter);

					memcpy(&temporaryTblBuffer[outputTblCounter], alBank->inst[instrumentIndex]->sounds[y]->wav.wavData, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
					outputTblCounter += alBank->inst[instrumentIndex]->sounds[y]->wav.len;

					if ((outputTblCounter % 8) != 0)
					{
						int remain = 8 - (outputTblCounter % 8);
						while (remain > 0)
						{
							temporaryTblBuffer[outputTblCounter] = 0x00;
							outputTblCounter++;
							remain--;
						}
					}
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, offsetWaveData[waveDataIndex]);
					outputCtlCounter2 += 4;
					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
					outputCtlCounter2 += 4;
				}

				temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->wav.type;
				temporaryCtlBuffer2[outputCtlCounter2++] = 0; // offset type
				temporaryCtlBuffer2[outputCtlCounter2++] = 0; // pad
				temporaryCtlBuffer2[outputCtlCounter2++] = 0; // pad

				
				unsigned long loopWriteSpot = outputCtlCounter2;
				unsigned long bookWriteSpot = outputCtlCounter2 + 4;
				unsigned long extraWriteSpot = outputCtlCounter2 + 8;

				outputCtlCounter2 += 0xC;
				if ((outputCtlCounter2 % 8) != 0)
				{
					int pad = 8 - (outputCtlCounter2 % 8);
					for (int z = 0; z < pad; z++)
					{
						temporaryCtlBuffer2[outputCtlCounter2++] = 0;
					}
				}


				if (alBank->inst[instrumentIndex]->sounds[y]->wav.type == AL_ADPCM_WAVE)
				{
					WriteLongToBuffer(temporaryCtlBuffer2, bookWriteSpot, outputCtlCounter2 - startOffset);

					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->order);
					outputCtlCounter2 += 4;
					WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->npredictors);
					outputCtlCounter2 += 4;

					for (int z = 0; z < (alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
					{
						temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
						temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
					}

					if (((outputCtlCounter2 - startOffset) % 0x10) != 0)
					{
						int pad = 0x10 - ((outputCtlCounter2 - startOffset) % 0x10);
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer2[outputCtlCounter2++] = 0;
						}
					}

					if (alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop != NULL)
					{
						WriteLongToBuffer(temporaryCtlBuffer2, loopWriteSpot, outputCtlCounter2 - startOffset);

						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->start);
						outputCtlCounter2 += 4;
						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->end);
						outputCtlCounter2 += 4;
						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->count);
						outputCtlCounter2 += 4;
						for (int z = 0; z < 0x10; z++)
						{
							temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
							temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
						}
					}
					else
					{
						// null loop
						WriteLongToBuffer(temporaryCtlBuffer2, loopWriteSpot, 0x0);
					}

					if (((outputCtlCounter2 - startOffset) % 0x10) != 0)
					{
						int pad = 0x10 - ((outputCtlCounter2 - startOffset) % 0x10);
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer2[outputCtlCounter2++] = 0;
						}
					}

					if (alBank->inst[instrumentIndex]->sounds[y]->conkerShorts.size() > 0) 
					{
						WriteLongToBuffer(temporaryCtlBuffer2, extraWriteSpot, outputCtlCounter2 - startOffset);

						for (int z = 0; z < alBank->inst[instrumentIndex]->sounds[y]->conkerShorts.size(); z++)
						{
							WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->conkerShorts[z]);
							outputCtlCounter2 += 2;
						}

						WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0xFFFF);
						outputCtlCounter2 += 2;

						if ((outputCtlCounter2 % 4) != 0)
						{
							WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x0000);
							outputCtlCounter2 += 2;
						}
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer2, extraWriteSpot, 0x00000000);
					}
				}
				else
				{
					MessageBox(NULL, "Invalid wav type", "Error", NULL);
					return;
				}

				if ((outputCtlCounter2 % 0x8) != 0)
				{
					int pad = 0x8 - (outputCtlCounter2 % 0x8);
					for (int z = 0; z < pad; z++)
					{
						temporaryCtlBuffer2[outputCtlCounter2++] = 0;
					}
				}
			}
		}
		else
		{
			std::vector<unsigned long> offsetSoundData;
			std::vector<unsigned long> offsetWaves;
			std::vector<int> soundDataOverallEnvIndex;
			std::vector<int> soundDataOverallKeyIndex;
			std::vector<int> soundDataOverallWaveIndex;
			std::vector<int> soundDataOverallPredictorIndex;
			std::vector<int> soundDataOverallLoopIndex;

			std::vector<ALEnv*> envs;
			std::vector<unsigned long> offsetEnvs;
			std::vector<ALKey*> keys;
			std::vector<unsigned long> offsetKeys;

			std::vector<ALADPCMBook*> predictorData;
			std::vector<unsigned long> offsetPredictorData;
			std::vector<ALADPCMloop*> loopData;
			std::vector<unsigned long> offsetLoopData;

			unsigned long startOffset = outputCtlCounter2 - 0x10;
			outputCtlCounter2 += (alBank->inst[instrumentIndex]->soundCount * 4);

			for (int y = 0; y < alBank->inst[instrumentIndex]->soundCount; y++)
			{
				int envIndex = -1;
				for (int z = 0; z < envs.size(); z++)
				{
					if (
						(alBank->inst[instrumentIndex]->sounds[y]->env.attackTime == envs[z]->attackTime)
						&& (alBank->inst[instrumentIndex]->sounds[y]->env.decayTime == envs[z]->decayTime)
						&& (alBank->inst[instrumentIndex]->sounds[y]->env.releaseTime == envs[z]->releaseTime)
						&& (alBank->inst[instrumentIndex]->sounds[y]->env.attackVolume == envs[z]->attackVolume)
						&& (alBank->inst[instrumentIndex]->sounds[y]->env.decayVolume == envs[z]->decayVolume)
						)
					{
						envIndex = z;
						break;
					}
				}

				int keyIndex = -1;
				for (int z = 0; z < keys.size(); z++)
				{
					if (
							(alBank->inst[instrumentIndex]->sounds[y]->key.velocitymin == keys[z]->velocitymin)
							&& (alBank->inst[instrumentIndex]->sounds[y]->key.velocitymax == keys[z]->velocitymax)
							&& (alBank->inst[instrumentIndex]->sounds[y]->key.keymin == keys[z]->keymin)
							&& (alBank->inst[instrumentIndex]->sounds[y]->key.keymax == keys[z]->keymax)
							&& (alBank->inst[instrumentIndex]->sounds[y]->key.keybase == keys[z]->keybase)
							&& (alBank->inst[instrumentIndex]->sounds[y]->key.detune == keys[z]->detune)
							)
					{
						keyIndex = z;
						break;
					}
				}
			
				int waveDataIndex = -1;
				for (int z = 0; z < offsetWaveData.size(); z++)
				{
					if (
						(CompareBytes(alBank->inst[instrumentIndex]->sounds[y]->wav.wavData, alBank->inst[instrumentIndex]->sounds[y]->wav.len, &temporaryTblBuffer[offsetWaveData[z]], waveDataLength[z]))
						)
					{
						waveDataIndex = z;
						break;
					}
				}

				// Check if predictor exists, if not, write it
				int predictorIndex = -1;
				ALADPCMBook* book = alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->book;
				for (int z = 0; z < predictorData.size(); z++)
				{

					if (
						(predictorData[z]->npredictors == book->npredictors)
						&& (predictorData[z]->order == book->order)
						)
					{
						bool matchPred = true;
						for (int p = 0; p < (predictorData[z]->npredictors * predictorData[z]->order * 8); p++)
						{
							if (predictorData[z]->predictors[p] != book->predictors[p])
							{
								matchPred = false;
							}
						}

						if (matchPred)
						{
							predictorIndex = z;
							break;
						}
					}
				}

				// Check if loop exists, if not, write it
				int loopIndex = -1;
				ALADPCMloop* loop = alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop;
				for (int z = 0; z < loopData.size(); z++)
				{
					if (
						(loop != NULL)
						&& (loopData[z] != NULL)
						)
					{
						if (
							(loop->start == loopData[z]->start)
							&& (loop->end == loopData[z]->end)
							&& (loop->count == loopData[z]->count)
							)
						{
							if (loop->start == 0)
							{
								loopIndex = z;
								break;
							}
							else
							{
								bool matchLoop = true;
								for (int l = 0; l < 0x10; l++)
								{
									if (loop->state[l] != loopData[z]->state[l])
									{
										matchLoop = false;
										break;
									}
								}

								if (matchLoop)
								{
									loopIndex = z;
									break;
								}
							}
						}
					}
				}

				int soundIndex = -1;
				for (int z = 0; z < offsetSoundData.size(); z++)
				{
					if ((soundDataOverallEnvIndex[z] == envIndex) && (soundDataOverallKeyIndex[z] == keyIndex) && (soundDataOverallWaveIndex[z] == waveDataIndex)
						&& (soundDataOverallLoopIndex[z] == loopIndex) && (soundDataOverallPredictorIndex[z] == predictorIndex))
					{
						soundIndex = z;
						break;
					}
				}

				// For some reason closer matches game, it doesn't optimize at all
				//envIndex = -1;
				//keyIndex = -1;
				//soundIndex = -1;
				//loopIndex = -1;
				//predictorIndex = -1;
				
				if (soundIndex == -1)
				{
					WriteLongToBuffer(temporaryCtlBuffer2, startOffset + 0x10 + (y * 4), outputCtlCounter2 - startOffset);

					unsigned long writeOffsetsSpot = outputCtlCounter2;

					outputCtlCounter2 += 0xC;

					temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->samplePan;
					temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->sampleVolume;
					WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0);
					outputCtlCounter2 += 2;

					if (envIndex == -1)
					{
						envs.push_back(&alBank->inst[instrumentIndex]->sounds[y]->env);
						offsetEnvs.push_back(outputCtlCounter2);
						WriteLongToBuffer(temporaryCtlBuffer2, writeOffsetsSpot, outputCtlCounter2 - startOffset);

						// Env
						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.attackTime);
						outputCtlCounter2 += 4;
						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.decayTime);
						outputCtlCounter2 += 4;
						WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.releaseTime);
						outputCtlCounter2 += 4;
						temporaryCtlBuffer2[outputCtlCounter2] = alBank->inst[instrumentIndex]->sounds[y]->env.attackVolume;
						outputCtlCounter2 += 1;
						temporaryCtlBuffer2[outputCtlCounter2] = alBank->inst[instrumentIndex]->sounds[y]->env.decayVolume;
						outputCtlCounter2 += 1;
						WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->env.zeroPad);
						outputCtlCounter2 += 2;

						envIndex = envs.size() - 1;
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer2, writeOffsetsSpot, offsetEnvs[envIndex] - startOffset);
					}

					if (keyIndex == -1)
					{
						keys.push_back(&alBank->inst[instrumentIndex]->sounds[y]->key);
						offsetKeys.push_back(outputCtlCounter2);
						WriteLongToBuffer(temporaryCtlBuffer2, writeOffsetsSpot + 4, outputCtlCounter2 - startOffset);

						// Key
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.velocitymin;
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.velocitymax;
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keymin;
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keymax;
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.keybase;
						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->key.detune;

						WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x0000);
						outputCtlCounter2 += 2;

						keyIndex = keys.size() - 1;
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer2, writeOffsetsSpot + 4, offsetKeys[keyIndex] - startOffset);
					}

					int fullMatchWaveData = -1;
					for (int z = 0; z < soundDataOverallWaveIndex.size(); z++)
					{
						if (
							(soundDataOverallWaveIndex[z] == waveDataIndex)
							&& (soundDataOverallPredictorIndex[z] == predictorIndex)
							&& (soundDataOverallLoopIndex[z] == loopIndex)
							)
						{
							fullMatchWaveData = z;
							break;
						}
					}

					unsigned long offsetLocationWaveData = outputCtlCounter2;
					if (fullMatchWaveData == -1)
					{
						WriteLongToBuffer(temporaryCtlBuffer2, writeOffsetsSpot + 8, outputCtlCounter2 - startOffset);

						if (waveDataIndex == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, outputTblCounter);
							outputCtlCounter2 += 4;
							WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
							outputCtlCounter2 += 4;

							waveDataLength.push_back(alBank->inst[instrumentIndex]->sounds[y]->wav.len);
							offsetWaveData.push_back(outputTblCounter);

							memcpy(&temporaryTblBuffer[outputTblCounter], alBank->inst[instrumentIndex]->sounds[y]->wav.wavData, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
							outputTblCounter += alBank->inst[instrumentIndex]->sounds[y]->wav.len;

							if ((outputTblCounter % 8) != 0)
							{
								int remain = 8 - (outputTblCounter % 8);
								while (remain > 0)
								{
									temporaryTblBuffer[outputTblCounter] = 0x00;
									outputTblCounter++;
									remain--;
								}
							}
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, offsetWaveData[waveDataIndex] - startOffset);
							outputCtlCounter2 += 4;
							WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.len);
							outputCtlCounter2 += 4;
						}

						temporaryCtlBuffer2[outputCtlCounter2++] = alBank->inst[instrumentIndex]->sounds[y]->wav.type;
						temporaryCtlBuffer2[outputCtlCounter2++] = 0; // offset type
						temporaryCtlBuffer2[outputCtlCounter2++] = 0; // pad
						temporaryCtlBuffer2[outputCtlCounter2++] = 0; // pad

						unsigned long loopWriteSpot = outputCtlCounter2;
						unsigned long bookWriteSpot = outputCtlCounter2 + 4;
						unsigned long extraWriteSpot = outputCtlCounter2 + 8;

						outputCtlCounter2 += 0xC;

						if (((outputCtlCounter2 - startOffset) % 8) != 0)
						{
							int pad = 8 - ((outputCtlCounter2 - startOffset) % 8);
							for (int z = 0; z < pad; z++)
							{
								temporaryCtlBuffer2[outputCtlCounter2++] = 0;
							}
						}

						if (alBank->inst[instrumentIndex]->sounds[y]->wav.type == AL_ADPCM_WAVE)
						{
							if (predictorIndex == -1)
							{
								if (book != NULL)
								{
									WriteLongToBuffer(temporaryCtlBuffer2, bookWriteSpot, outputCtlCounter2 - startOffset);

									offsetPredictorData.push_back(outputCtlCounter2);
									predictorIndex = predictorData.size();
									predictorData.push_back(book);

									WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, book->order);
									outputCtlCounter2 += 4;
									WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, book->npredictors);
									outputCtlCounter2 += 4;
									for (int z = 0; z < (book->order * book->npredictors * 8); z++)
									{
										WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, book->predictors[z]);
										outputCtlCounter2 += 2;
									}

									if (((outputCtlCounter2 - startOffset) % 0x10) != 0)
									{
										int pad = 0x10 - ((outputCtlCounter2 - startOffset) % 0x10);
										for (int z = 0; z < pad; z++)
										{
											temporaryCtlBuffer2[outputCtlCounter2++] = 0;
										}
									}
								}
								else
								{
									WriteLongToBuffer(temporaryCtlBuffer2, bookWriteSpot, 0x00000000);
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer2, bookWriteSpot, offsetPredictorData[predictorIndex] - startOffset);
							}

							if (((outputCtlCounter2 - startOffset) % 8) != 0)
							{
								int pad = 8 - ((outputCtlCounter2 - startOffset) % 8);
								for (int z = 0; z < pad; z++)
								{
									temporaryCtlBuffer2[outputCtlCounter2++] = 0;
								}
							}

							if (loopIndex == -1)
							{
								if (alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop != NULL)
								{
									offsetLoopData.push_back(outputCtlCounter2);
									loopIndex = loopData.size();
									loopData.push_back(loop);

									WriteLongToBuffer(temporaryCtlBuffer2, loopWriteSpot, outputCtlCounter2 - startOffset);

									WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->start);
									outputCtlCounter2 += 4;
									WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->end);
									outputCtlCounter2 += 4;
									WriteLongToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->count);
									outputCtlCounter2 += 4;
									for (int z = 0; z < 0x10; z++)
									{
										temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
										temporaryCtlBuffer2[outputCtlCounter2++] = ((alBank->inst[instrumentIndex]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
									}
								}
								else
								{
									WriteLongToBuffer(temporaryCtlBuffer2, loopWriteSpot, 0x00000000);
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer2, loopWriteSpot, offsetLoopData[loopIndex] - startOffset);
							}

							if (alBank->inst[instrumentIndex]->sounds[y]->conkerShorts.size() > 0) 
							{
								WriteLongToBuffer(temporaryCtlBuffer2, extraWriteSpot, outputCtlCounter2 - startOffset);

								for (int z = 0; z < alBank->inst[instrumentIndex]->sounds[y]->conkerShorts.size(); z++)
								{
									WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, alBank->inst[instrumentIndex]->sounds[y]->conkerShorts[z]);
									outputCtlCounter2 += 2;
								}

								WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0xFFFF);
								outputCtlCounter2 += 2;

								if ((outputCtlCounter2 % 4) != 0)
								{
									WriteShortToBuffer(temporaryCtlBuffer2, outputCtlCounter2, 0x0000);
									outputCtlCounter2 += 2;
								}
							}
							else
							{
								WriteLongToBuffer(temporaryCtlBuffer2, extraWriteSpot, 0x00000000);
							}
						}
						else
						{
							MessageBox(NULL, "Invalid wav type", "Error", NULL);
							return;
						}
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer, writeOffsetsSpot + 8, offsetWaves[fullMatchWaveData] - startOffset);

						offsetLocationWaveData = offsetWaves[fullMatchWaveData];
					}

					offsetSoundData.push_back(writeOffsetsSpot);
					offsetWaves.push_back(offsetLocationWaveData);
					soundDataOverallEnvIndex.push_back(envIndex);
					soundDataOverallKeyIndex.push_back(keyIndex);
					soundDataOverallWaveIndex.push_back(waveDataIndex);
					soundDataOverallLoopIndex.push_back(loopIndex);
					soundDataOverallPredictorIndex.push_back(predictorIndex);
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer2, startOffset + (y * 4), offsetSoundData[soundIndex] - startOffset);
				}
			}

			int pad = 0x8 - (outputCtlCounter2 % 0x8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer2[outputCtlCounter2++] = 0;
			}
		}
	}

	if ((outputCtlCounter % 0x8) != 0)
	{
		int pad = 8 - (outputCtlCounter % 8);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	ctl = temporaryCtlBuffer;
	ctl2 = temporaryCtlBuffer2;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
	ctlSize2 = outputCtlCounter2;
}

bool CN64AIFCAudio::InjectCtlTblIntoROMArrayInPlace(unsigned char* ROM, unsigned char* ctl, int ctlSize, unsigned char* tbl, int tblSize, unsigned long ctlOffset, unsigned long tblOffset, unsigned long maxCtl, unsigned long maxTbl, unsigned char* ctl2, int ctlSize2, int ctlOffset2, int maxCtl2)
{
	if (ctlSize > maxCtl)
	{
		CString ctlError;
		ctlError.Format("Error ctl too big, %04X is too big for %04X", ctlSize, maxCtl);
		MessageBox(NULL, ctlError, "Error", NULL);
		return false;
	}
	if (tblSize > maxTbl)
	{
		CString tblError;
		tblError.Format("Error tbl too big, %04X is too big for %04X", tblSize, maxTbl);

		MessageBox(NULL, tblError, "Error", NULL);
		return false;
	}
	if (ctl2 != NULL)
	{
		if (ctlSize2 > maxCtl2)
		{
			CString ctlError2;
			ctlError2.Format("Error ctl 2 too big, %04X is too big for %04X", ctlSize2, maxCtl2);

			MessageBox(NULL, ctlError2, "Error", NULL);
			return false;
		}
	}
	for (int x = ctlOffset; x < (ctlOffset + ctlSize); x++)
	{
		ROM[x] = ctl[x - ctlOffset];
	}
	for (int x = tblOffset; x < (tblOffset + tblSize); x++)
	{
		ROM[x] = tbl[x - tblOffset];
	}
	if (ctl2 != NULL)
	{
		for (int x = ctlOffset2; x < (ctlOffset2 + ctlSize2); x++)
		{
			ROM[x] = ctl2[x - ctlOffset2];
		}
	}
	return true;
}

void CN64AIFCAudio::DisposeALInst(ALInst*& alInst)
{
	if (alInst->sounds != NULL)
	{
		for (int y = 0; y < alInst->soundCount; y++)
		{
			if (alInst->sounds[y] != NULL)
			{
				if (alInst->sounds[y]->wav.type == AL_ADPCM_WAVE)
				{
					if (alInst->sounds[y]->wav.adpcmWave != NULL)
					{
						if (alInst->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							delete alInst->sounds[y]->wav.adpcmWave->loop;
						}
						if (alInst->sounds[y]->wav.adpcmWave->book != NULL)
						{
							delete [] alInst->sounds[y]->wav.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wav.adpcmWave->book;
						}
						delete alInst->sounds[y]->wav.adpcmWave;
					}
				}
				else if (alInst->sounds[y]->wav.type == AL_RAW16_WAVE)
				{
					if (alInst->sounds[y]->wav.rawWave != NULL)
					{
						if (alInst->sounds[y]->wav.rawWave->loop != NULL)
						{
							delete alInst->sounds[y]->wav.rawWave->loop;
						}
						delete alInst->sounds[y]->wav.rawWave;
					}
				}
				else if (alInst->sounds[y]->wav.type == AL_MUSYX_WAVE)
				{
					if (alInst->sounds[y]->wav.adpcmWave != NULL)
					{
						if (alInst->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							delete alInst->sounds[y]->wav.adpcmWave->loop;
						}
						if (alInst->sounds[y]->wav.adpcmWave->book != NULL)
						{
							delete [] alInst->sounds[y]->wav.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wav.adpcmWave->book;
						}
						delete alInst->sounds[y]->wav.adpcmWave;
					}
				}

				if (alInst->sounds[y]->wav.wavData != NULL)
					delete [] alInst->sounds[y]->wav.wavData;




				if (alInst->sounds[y]->hasWavePrevious)
				{
					if (alInst->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE)
					{
						if (alInst->sounds[y]->wavPrevious.adpcmWave != NULL)
						{
							if (alInst->sounds[y]->wavPrevious.adpcmWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavPrevious.adpcmWave->loop;
							}
							delete [] alInst->sounds[y]->wavPrevious.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wavPrevious.adpcmWave->book;
							delete alInst->sounds[y]->wavPrevious.adpcmWave;
						}
					}
					else if (alInst->sounds[y]->wavPrevious.type== AL_RAW16_WAVE)
					{
						if (alInst->sounds[y]->wavPrevious.rawWave != NULL)
						{
							if (alInst->sounds[y]->wavPrevious.rawWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavPrevious.rawWave->loop;
							}
							delete alInst->sounds[y]->wavPrevious.rawWave;
						}
					}
					else if (alInst->sounds[y]->wavPrevious.type == AL_MUSYX_WAVE)
					{
						if (alInst->sounds[y]->wavPrevious.adpcmWave != NULL)
						{
							if (alInst->sounds[y]->wavPrevious.adpcmWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavPrevious.adpcmWave->loop;
							}
							delete [] alInst->sounds[y]->wavPrevious.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wavPrevious.adpcmWave->book;
							delete alInst->sounds[y]->wavPrevious.adpcmWave;
						}
					}

					if (alInst->sounds[y]->wavPrevious.wavData != NULL)
						delete [] alInst->sounds[y]->wavPrevious.wavData;
				}

				if (alInst->sounds[y]->hasWaveSecondary)
				{
					if (alInst->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE)
					{
						if (alInst->sounds[y]->wavSecondary.adpcmWave != NULL)
						{
							if (alInst->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavSecondary.adpcmWave->loop;
							}
							delete [] alInst->sounds[y]->wavSecondary.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wavSecondary.adpcmWave->book;
							delete alInst->sounds[y]->wavSecondary.adpcmWave;
						}
					}
					else if (alInst->sounds[y]->wavSecondary.type == AL_RAW16_WAVE)
					{
						if (alInst->sounds[y]->wavSecondary.rawWave != NULL)
						{
							if (alInst->sounds[y]->wavSecondary.rawWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavSecondary.rawWave->loop;
							}
							delete alInst->sounds[y]->wavSecondary.rawWave;
						}
					}
					else if (alInst->sounds[y]->wavSecondary.type == AL_MUSYX_WAVE)
					{
						if (alInst->sounds[y]->wavSecondary.adpcmWave != NULL)
						{
							if (alInst->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
							{
								delete alInst->sounds[y]->wavSecondary.adpcmWave->loop;
							}
							delete [] alInst->sounds[y]->wavSecondary.adpcmWave->book->predictors;
							delete alInst->sounds[y]->wavSecondary.adpcmWave->book;
							delete alInst->sounds[y]->wavSecondary.adpcmWave;
						}
					}

					if (alInst->sounds[y]->wavSecondary.wavData != NULL)
						delete [] alInst->sounds[y]->wavSecondary.wavData;
				}


				delete alInst->sounds[y];
			}
		}

		delete alInst->sounds;
	}

	delete alInst;
}

void CN64AIFCAudio::DisposeT1Bank(T1Bank*& t1Bank)
{
	if (t1Bank != NULL)
	{
		delete t1Bank;
		t1Bank = NULL;
	}
}

void CN64AIFCAudio::DisposeALBank(ALBank*& alBank)
{
	if (alBank != NULL)
	{
		alBank->konamiADSR.clear();
		alBank->konamiDrumsADSR.clear();

		if (alBank->eadPercussion != NULL)
		{
			for (int y = 0; y < alBank->countEADPercussion; y++)
			{
				if (alBank->eadPercussion[y].wav.type == AL_ADPCM_WAVE)
				{
					if (alBank->eadPercussion[y].wav.adpcmWave != NULL)
					{
						if (alBank->eadPercussion[y].wav.adpcmWave->loop != NULL)
						{
							delete alBank->eadPercussion[y].wav.adpcmWave->loop;
						}
						if (alBank->eadPercussion[y].wav.adpcmWave->book != NULL)
						{
							delete [] alBank->eadPercussion[y].wav.adpcmWave->book->predictors;
							delete alBank->eadPercussion[y].wav.adpcmWave->book;
						}
						delete alBank->eadPercussion[y].wav.adpcmWave;
					}
				}
				else if (alBank->eadPercussion[y].wav.type == AL_RAW16_WAVE)
				{
					if (alBank->eadPercussion[y].wav.rawWave != NULL)
					{
						if (alBank->eadPercussion[y].wav.rawWave->loop != NULL)
						{
							delete alBank->eadPercussion[y].wav.rawWave->loop;
						}
						delete alBank->eadPercussion[y].wav.rawWave;
					}
				}
				else if (alBank->eadPercussion[y].wav.type == AL_MUSYX_WAVE)
				{
					if (alBank->eadPercussion[y].wav.adpcmWave != NULL)
					{
						if (alBank->eadPercussion[y].wav.adpcmWave->loop != NULL)
						{
							delete alBank->eadPercussion[y].wav.adpcmWave->loop;
						}
						delete alBank->eadPercussion[y].wav.adpcmWave;
					}
				}

				if (alBank->eadPercussion[y].wav.wavData != NULL)
					delete [] alBank->eadPercussion[y].wav.wavData;
			}

			delete [] alBank->eadPercussion;
		}

		if (alBank->alSfx != NULL)
		{
			for (int x = 0; x < alBank->countSfx; x++)
			{
				if (alBank->alSfx[x] != NULL)
				{
					if (alBank->alSfx[x]->type == AL_ADPCM_WAVE)
					{
						if (alBank->alSfx[x]->adpcmWave != NULL)
						{
							if (alBank->alSfx[x]->adpcmWave->loop != NULL)
							{
								delete alBank->alSfx[x]->adpcmWave->loop;
							}
							if (alBank->alSfx[x]->adpcmWave->book != NULL)
							{
								delete [] alBank->alSfx[x]->adpcmWave->book->predictors;
								delete alBank->alSfx[x]->adpcmWave->book;
							}
							delete alBank->alSfx[x]->adpcmWave;
						}
					}
					else if (alBank->alSfx[x]->type == AL_RAW16_WAVE)
					{
						if (alBank->alSfx[x]->rawWave != NULL)
						{
							if (alBank->alSfx[x]->rawWave->loop != NULL)
							{
								delete alBank->alSfx[x]->rawWave->loop;
							}
							delete alBank->alSfx[x]->rawWave;
						}
					}
					else if (alBank->alSfx[x]->type == AL_MUSYX_WAVE)
					{
						if (alBank->alSfx[x]->adpcmWave != NULL)
						{
							if (alBank->alSfx[x]->adpcmWave->loop != NULL)
							{
								delete alBank->alSfx[x]->adpcmWave->loop;
							}
							if (alBank->alSfx[x]->adpcmWave->book != NULL)
							{
								delete [] alBank->alSfx[x]->adpcmWave->book->predictors;
								delete alBank->alSfx[x]->adpcmWave->book;
							}
							delete alBank->alSfx[x]->adpcmWave;
						}
					}

					if (alBank->alSfx[x]->wavData != NULL)
						delete [] alBank->alSfx[x]->wavData;

					delete alBank->alSfx[x];
				}
			}

			delete [] alBank->alSfx;
		}

		if (alBank->percussion != NULL)
		{
			if (alBank->percussion->sounds != NULL)
			{
				for (int y = 0; y < alBank->percussion->soundCount; y++)
				{
					if (alBank->percussion->sounds[y] != NULL)
					{
						if (alBank->percussion->sounds[y]->wav.type == AL_ADPCM_WAVE)
						{
							if (alBank->percussion->sounds[y]->wav.adpcmWave != NULL)
							{
								if (alBank->percussion->sounds[y]->wav.adpcmWave->loop != NULL)
								{
									delete alBank->percussion->sounds[y]->wav.adpcmWave->loop;
								}
								if (alBank->percussion->sounds[y]->wav.adpcmWave->book != NULL)
								{
									delete [] alBank->percussion->sounds[y]->wav.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wav.adpcmWave->book;
								}
								delete alBank->percussion->sounds[y]->wav.adpcmWave;
							}
						}
						else if (alBank->percussion->sounds[y]->wav.type == AL_RAW16_WAVE)
						{
							if (alBank->percussion->sounds[y]->wav.rawWave != NULL)
							{
								if (alBank->percussion->sounds[y]->wav.rawWave->loop != NULL)
								{
									delete alBank->percussion->sounds[y]->wav.rawWave->loop;
								}
								delete alBank->percussion->sounds[y]->wav.rawWave;
							}
						}
						else if (alBank->percussion->sounds[y]->wav.type == AL_MUSYX_WAVE)
						{
							if (alBank->percussion->sounds[y]->wav.adpcmWave != NULL)
							{
								if (alBank->percussion->sounds[y]->wav.adpcmWave->loop != NULL)
								{
									delete alBank->percussion->sounds[y]->wav.adpcmWave->loop;
								}
								if (alBank->percussion->sounds[y]->wav.adpcmWave->book != NULL)
								{
									delete [] alBank->percussion->sounds[y]->wav.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wav.adpcmWave->book;
								}
								delete alBank->percussion->sounds[y]->wav.adpcmWave;
							}
						}

						if (alBank->percussion->sounds[y]->wav.wavData != NULL)
							delete [] alBank->percussion->sounds[y]->wav.wavData;




						if (alBank->percussion->sounds[y]->hasWavePrevious)
						{
							if (alBank->percussion->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavPrevious.adpcmWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavPrevious.adpcmWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave->loop;
									}
									delete [] alBank->percussion->sounds[y]->wavPrevious.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave->book;
									delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave;
								}
							}
							else if (alBank->percussion->sounds[y]->wavPrevious.type== AL_RAW16_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavPrevious.rawWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavPrevious.rawWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavPrevious.rawWave->loop;
									}
									delete alBank->percussion->sounds[y]->wavPrevious.rawWave;
								}
							}
							else if (alBank->percussion->sounds[y]->wavPrevious.type == AL_MUSYX_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavPrevious.adpcmWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavPrevious.adpcmWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave->loop;
									}
									delete [] alBank->percussion->sounds[y]->wavPrevious.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave->book;
									delete alBank->percussion->sounds[y]->wavPrevious.adpcmWave;
								}
							}

							if (alBank->percussion->sounds[y]->wavPrevious.wavData != NULL)
								delete [] alBank->percussion->sounds[y]->wavPrevious.wavData;
						}

						if (alBank->percussion->sounds[y]->hasWaveSecondary)
						{
							if (alBank->percussion->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavSecondary.adpcmWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave->loop;
									}
									delete [] alBank->percussion->sounds[y]->wavSecondary.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave->book;
									delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave;
								}
							}
							else if (alBank->percussion->sounds[y]->wavSecondary.type == AL_RAW16_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavSecondary.rawWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavSecondary.rawWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavSecondary.rawWave->loop;
									}
									delete alBank->percussion->sounds[y]->wavSecondary.rawWave;
								}
							}
							else if (alBank->percussion->sounds[y]->wavSecondary.type == AL_MUSYX_WAVE)
							{
								if (alBank->percussion->sounds[y]->wavSecondary.adpcmWave != NULL)
								{
									if (alBank->percussion->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
									{
										delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave->loop;
									}
									delete [] alBank->percussion->sounds[y]->wavSecondary.adpcmWave->book->predictors;
									delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave->book;
									delete alBank->percussion->sounds[y]->wavSecondary.adpcmWave;
								}
							}

							if (alBank->percussion->sounds[y]->wavSecondary.wavData != NULL)
								delete [] alBank->percussion->sounds[y]->wavSecondary.wavData;
						}


						delete alBank->percussion->sounds[y];
					}
				}

				delete alBank->percussion->sounds;
			}

			delete alBank->percussion;
		}

		if (alBank->inst != NULL)
		{
			for (int x = 0; x < alBank->count; x++)
			{
				DisposeALInst(alBank->inst[x]);
			}

			delete alBank->inst;
		}

		delete alBank;
	}
}


ALBank* CN64AIFCAudio::ReadAudioTurok(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankTrueOffset, unsigned long mask, unsigned char* ROM, int bankNumber)
{
	ALBank* alBank = ReadAudio(ROM, &ROM[0], ctlSize, ctlOffset, tbl, bankTrueOffset, mask, bankNumber);
	if (alBank != NULL)
	{
		alBank->soundBankFormat = TUROKFORMAT;
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV1(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + 0x14;

	// header size 0x14
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV1;
	alBank->count = (unsigned short)CharArrayToLong(&ctl[ctlOffset+0x10]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);
			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetInstrument + 0xC]);

							if (loopOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									loopOffset = loopOffset & 0xFFFFFF;
								else
									loopOffset = ctlOffset + loopOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
							}

						
							if (predictorOffset != 0x00000000)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									predictorOffset = predictorOffset & 0xFFFFFF;
								else
									predictorOffset = ctlOffset + predictorOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
								}
							}
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
							return NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioExciteBikeSAM(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = EXCITEBIKESAM;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			CEBBigDecompression bigDecompression;
			unsigned char* outputSfx;
			int sfxSize = 0;

			bigDecompression.DecodeFile(ctl, ctlOffset, outputSfx, sfxSize);

			int sizeSAM = Flip32Bit(CharArrayToLong(&outputSfx[numberInstruments + 4])) + 8;
			alBank->inst[x]->samplerate = CharArrayToLong(&outputSfx[numberInstruments + 0x18]);
			alBank->inst[x]->sounds[y]->wav.len = sizeSAM;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &outputSfx[numberInstruments], alBank->inst[x]->sounds[y]->wav.len);
			delete [] outputSfx;
			
			alBank->inst[x]->sounds[y]->wav.type = AL_EXCITEBIKE_SAM;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioKobeSAM(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = EXCITEBIKESAM;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = tblOffset;

			CEBBigDecompression bigDecompression;
			unsigned char* outputSfx;
			int sfxSize = 0;

			int outputSfxOffset = ctlOffset;
			bigDecompression.DecodeFile(ctl, outputSfxOffset, outputSfx, sfxSize);

			int sizeSAM = Flip32Bit(CharArrayToLong(&outputSfx[tblOffset + 4])) + 8;
			alBank->inst[x]->samplerate = CharArrayToLong(&outputSfx[tblOffset + 0x18]);
			alBank->inst[x]->sounds[y]->wav.len = sizeSAM;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &outputSfx[tblOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_EXCITEBIKE_SAM;
			delete [] outputSfx;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioWDCSFX(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments)
{
	std::vector<int> sfxOffset;
	for (int tableSfxOffset = ctlOffset; tableSfxOffset < tblOffset; tableSfxOffset += 4)
	{
		int data = CSharedFunctions::CharArrayToLong(ctl, tableSfxOffset);
		if (std::find(sfxOffset.begin(), sfxOffset.end(), data) == sfxOffset.end())
			sfxOffset.push_back(data);
	}
	std::sort(sfxOffset.begin(), sfxOffset.end());

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = WDCSFX;
	alBank->count = sfxOffset.size() - 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 44100;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = sfxOffset[x];

			CWDCAudioDecompression wdcDecompression;
			
			int ROMPREDICTOROFFSET = sfxOffset[x] + 4;
			int ROMPREDICTORLENGTH = 0x80;
			int ROMSAMPLEDATAOFFSET = ROMPREDICTOROFFSET + 0x80;

			unsigned long readValue = CSharedFunctions::CharArrayToLong(ctl, sfxOffset[x]);
			int lowestBits = ((readValue) & 0x1F);
			int inputSize = ((readValue + 0x1F) >> 5) * 2 * 9;

			int outputSize = inputSize * 9;

			alBank->inst[x]->sounds[y]->wav.len = inputSize;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = outputSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ROMSAMPLEDATAOFFSET], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 4;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[0x40];
			for (int p = 0; p < 0x40; p++)
			{
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[p] = CSharedFunctions::CharArrayToShort(ctl, ROMPREDICTOROFFSET + p * 2);
			}

			alBank->inst[x]->sounds[y]->wav.type = AL_WDC;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioWDCInstruments(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments, unsigned long mask)
{
	std::vector<int> instrumentOffset;
	instrumentOffset.push_back(numberInstruments); // shared samples
	for (int tableSfxOffset = ctlOffset; tableSfxOffset < tblOffset; tableSfxOffset += 4)
	{
		int data = CSharedFunctions::CharArrayToLong(ctl, tableSfxOffset);
		if (std::find(instrumentOffset.begin(), instrumentOffset.end(), data) == instrumentOffset.end())
			instrumentOffset.push_back(data);
	}
	std::sort(instrumentOffset.begin(), instrumentOffset.end());

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = WDCINSTRUMENTS;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 44100;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	{
		std::vector<int> currentInstrumentOffsetStarts;
		std::vector<int> predictorOffsets;
		std::vector<int> sampleOffsets;

		// Precheck sound count
		int currentInstrumentOffset = instrumentOffset[mask];
		while (CSharedFunctions::CharArrayToLong(&ctl[currentInstrumentOffset]) != 0xFFFFFFFF)
		{
			int instrumentStart = currentInstrumentOffset;
			unsigned long flags = CSharedFunctions::CharArrayToLong(&ctl[currentInstrumentOffset]);
			
			//FILE* outFileDMEM = fopen("C:\\temp\\dmem.bin", "wb");

			unsigned long readValue = CSharedFunctions::CharArrayToLong(ctl, currentInstrumentOffset + 4);
			int lowestBits = ((readValue) & 0x1F);
			int inputSize = ((readValue + 0x1F) >> 5) * 9;
			int outputSize = inputSize * 9; // * 4 just to let it play better with silence

			currentInstrumentOffset += 0x14;
			int ROMPREDICTOROFFSET = currentInstrumentOffset;
			int ROMPREDICTORLENGTH = 0x80;
			currentInstrumentOffset += ROMPREDICTORLENGTH;

			if (flags & 1)
				currentInstrumentOffset += 0x400;

			if (flags & 0x80)
			{
				if (currentInstrumentOffset & 1)
					currentInstrumentOffset++;
				continue;
			}

			int ROMSAMPLEDATAOFFSET = currentInstrumentOffset;
			int ROMSAMPLEDATALENGTH = inputSize / 9 * 9;

			predictorOffsets.push_back(ROMPREDICTOROFFSET);
			sampleOffsets.push_back(ROMSAMPLEDATAOFFSET);
			currentInstrumentOffsetStarts.push_back(instrumentStart);

			currentInstrumentOffset += inputSize;

			if (currentInstrumentOffset & 1)
				currentInstrumentOffset++;
		}

		alBank->count = currentInstrumentOffsetStarts.size();
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;

			alBank->inst[x]->soundCount = 1;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			int y = 0;
			{
				alBank->inst[x]->sounds[y] = new ALSound();

				alBank->inst[x]->sounds[y]->hasWavePrevious = false;
				alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
				alBank->inst[x]->sounds[y]->flags = 0;

				alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
				alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
				alBank->inst[x]->sounds[y]->wav.base = currentInstrumentOffsetStarts[x];

				CWDCAudioDecompression wdcDecompression;
				
				int ROMPREDICTOROFFSET = predictorOffsets[x];
				int ROMPREDICTORLENGTH = 0x80;
				int ROMSAMPLEDATAOFFSET = sampleOffsets[x];

				unsigned long readValue = CSharedFunctions::CharArrayToLong(ctl, currentInstrumentOffsetStarts[x] + 4);
				int lowestBits = ((readValue) & 0x1F);
				int inputSize = ((readValue + 0x1F) >> 5) * 9;

				int outputSize = inputSize * 9;

				alBank->inst[x]->sounds[y]->wav.len = inputSize;
				alBank->inst[x]->sounds[y]->wav.decompressedLength = outputSize;
				alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
				memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ROMSAMPLEDATAOFFSET], alBank->inst[x]->sounds[y]->wav.len);
				
				alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 4;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[0x40];
				for (int p = 0; p < 0x40; p++)
				{
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[p] = CSharedFunctions::CharArrayToShort(ctl, ROMPREDICTOROFFSET + p * 2);
				}

				alBank->inst[x]->sounds[y]->wav.type = AL_WDC;
			}
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioExciteBikeSFX(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{
	CEBBigDecompression bigDecompression;
	unsigned char* outputSfx;
	int sfxSize = 0;

	bigDecompression.DecodeFile(ctl, ctlOffset, outputSfx, sfxSize);

	CExciteBikeSAMAudioDecompression samDecompression;
	std::vector<EBSoundIdx> ebSoundIndexes = samDecompression.GetSoundIndexes(outputSfx, numberInstruments, mask);


	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = EXCITEBIKESFX;
	alBank->count = ebSoundIndexes.size();
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		if (ebSoundIndexes[x].flags == 1)
			alBank->inst[x]->samplerate = 11025;
		else
			alBank->inst[x]->samplerate = 22050;
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = ebSoundIndexes[x].size;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ebSoundIndexes[x].offset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_EXCITEBIKE_SFX;
		}
	}
	delete [] outputSfx;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioSouthParkRally(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments)
{
	CSPRallyAudioDecompression spRallyAudioDecompression;

	int startSoundTable = ctlOffset;
	int endSoundTable = tblOffset;
	std::vector<SPRallySoundTableEntry> soundTableEntries = spRallyAudioDecompression.GetSoundTableEntries(ctl, startSoundTable, endSoundTable);

	unsigned long fileTableHashMultiplier = 3;
	unsigned long fileTableStart = numberInstruments;
	std::vector<SPRallyFileTableEntry> fileTable = spRallyAudioDecompression.GetFileTableEntries(ctl, fileTableStart, fileTableHashMultiplier);

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SOUTHPARKRALLY;
	alBank->count = soundTableEntries.size();
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		CString nameROMStr;
		SPRallyFileTableEntry currentEntry = spRallyAudioDecompression.GetSoundFile(ctl, soundTableEntries[x], fileTable, fileTableHashMultiplier, nameROMStr);

		nameROMStr.Replace("sound/", "");
		alBank->inst[x]->name = nameROMStr;
		unsigned long dataPointer = fileTableStart + CSharedFunctions::CharArrayToLong(&ctl[currentEntry.offset]);
		unsigned long sizeData = CSharedFunctions::CharArrayToLong(&ctl[currentEntry.offset + 4]);

		unsigned long numberSamples = (((unsigned __int64)sizeData * (unsigned __int64)0x38E38E39) >> 32) & 0xFFFFFFFF;
		numberSamples = numberSamples >> 1;

		unsigned long currentFileOffset = dataPointer;
		int compressedSize = (currentEntry.offset - currentFileOffset);

		alBank->inst[x]->samplerate = soundTableEntries[x].samplingRate;
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = compressedSize;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = numberSamples * 0x10;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[currentFileOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_SOUTHPARKRALLY;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMP3(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MP3;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 44100;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = tblOffset - ctlOffset;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ctlOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_MP3;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioTwistedEdgeSnowboarding(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = TWISTEDEDGESNOWBOARDING;
	alBank->count = ((tblOffset - ctlOffset) / 8);
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		int tableSpot = ctlOffset + (x * 8);
		int soundROMOffsetSpot = CSharedFunctions::CharArrayToLong(&ctl[tableSpot]);
		int soundDataSize = CSharedFunctions::CharArrayToLong(&ctl[tableSpot + 4]) - CSharedFunctions::CharArrayToLong(&ctl[tableSpot]);

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = soundROMOffsetSpot;

			alBank->inst[x]->sounds[y]->wav.len = soundDataSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[soundROMOffsetSpot], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_TWISTEDEDGESNOWBOARDING;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioTwistedEdgeMusic(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = TWISTEDEDGEMUSIC;
	alBank->count = ((tblOffset - ctlOffset) / 4);
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		int soundROMOffsetSpot = CSharedFunctions::CharArrayToLong(&ctl[ctlOffset + (x * 4)]);
		int soundDataSize = CSharedFunctions::CharArrayToLong(&ctl[tblOffset + (x * 4)]) - CSharedFunctions::CharArrayToLong(&ctl[ctlOffset + (x * 4)]);

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = soundROMOffsetSpot;

			alBank->inst[x]->sounds[y]->wav.len = soundDataSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[soundROMOffsetSpot], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_TWISTEDEDGEMUSIC;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioIndividualSouthParkRally(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	CSPRallyAudioDecompression spRallyAudioDecompression;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SOUTHPARKRALLY;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 8000;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		unsigned long dataPointer = ctlOffset;
		unsigned long sizeData = CSharedFunctions::CharArrayToLong(&ctl[tblOffset + 4]);

		unsigned long numberSamples = (((unsigned __int64)sizeData * (unsigned __int64)0x38E38E39) >> 32) & 0xFFFFFFFF;
		numberSamples = numberSamples >> 1;

		unsigned long currentFileOffset = dataPointer;
		int compressedSize = (tblOffset - currentFileOffset);

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = compressedSize;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = numberSamples * 0x10;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[currentFileOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_SOUTHPARKRALLY;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioExciteBikeSNG(unsigned char* ctl, int romSize, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments, unsigned long mask)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = EXCITEBIKESNG;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			CEBBigDecompression bigDecompression;
			unsigned char* outputSfx;
			int sfxSize = 0;

			bigDecompression.DecodeFile(ctl, ctlOffset, outputSfx, sfxSize);

			int sizeSNG = Flip32Bit(CharArrayToLong(&outputSfx[numberInstruments + 4])) + 8;

			alBank->inst[x]->sounds[y]->wav.len = sizeSNG;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[16 + romSize + alBank->inst[x]->sounds[y]->wav.len];
			WriteLongToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, 0, romSize);
			WriteLongToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, 4, ctlOffset);
			WriteLongToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, 8, tblOffset);
			WriteLongToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, 12, mask);
			memcpy(&alBank->inst[x]->sounds[y]->wav.wavData[16], ctl, romSize);
			memcpy(&alBank->inst[x]->sounds[y]->wav.wavData[16 + romSize], &outputSfx[numberInstruments], alBank->inst[x]->sounds[y]->wav.len);
			delete [] outputSfx;
			
			alBank->inst[x]->sounds[y]->wav.type = AL_EXCITEBIKE_SNG;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioFightingForce(unsigned char* ctl, int romSize, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	CEBBigDecompression bigDecompression;
	unsigned char* outputSfx;
	int sfxSize = 0;

	bigDecompression.DecodeFile(ctl, ctlOffset, outputSfx, sfxSize);

	CFightingForceAudioDecompression fightingForceAudioDecompression;
	unsigned long wavTableStart = ctlOffset;
	std::vector<WavEntry> wavEntries;

	unsigned long wavTableSpot = wavTableStart;
	while (CSharedFunctions::CharArrayToLong(&ctl[wavTableSpot]) != 0x00000000)
	{
		CString tempStr;
		while (CSharedFunctions::CharArrayToLong(&ctl[wavTableSpot]) != 0x2E776176)
		{
			tempStr += (char)ctl[wavTableSpot];
			wavTableSpot++;
		}
		tempStr += ".wav";
		wavTableSpot += 4;
		unsigned long offset = CSharedFunctions::Flip32Bit(CSharedFunctions::CharArrayToLong(&ctl[wavTableSpot]));
		wavTableSpot += 4;
		unsigned long length = CSharedFunctions::Flip32Bit(CSharedFunctions::CharArrayToLong(&ctl[wavTableSpot]));
		wavTableSpot += 4;

		wavEntries.push_back(WavEntry(offset, length, tempStr));
	}


	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = FIGHTINGFORCE;
	alBank->count = wavEntries.size();
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->name = wavEntries[x].name;
		alBank->inst[x]->samplerate = 11025;
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = wavEntries[x].offset;

			alBank->inst[x]->sounds[y]->wav.len = wavEntries[x].size;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &tbl[wavEntries[x].offset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_FIGHTINGFORCE;
		}
	}
	delete [] outputSfx;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioH20Raw816(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = H20RAW816;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			CH20Decoder h20dec;
			unsigned char* tempBuffer = new unsigned char[0x100000];
			alBank->inst[x]->sounds[y]->wav.len = h20dec.decPolaris(&ctl[alBank->inst[x]->sounds[y]->wav.base], ctlSize, tempBuffer);

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, tempBuffer, alBank->inst[x]->sounds[y]->wav.len);
			delete [] tempBuffer;
			
			if (numberInstruments == 0)
				alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW8;
			else
				alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW16;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioTetrisphereRaw816(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = TETRISPHERERAW816;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			CTetrisphereDecoder tetDec;
			unsigned char* tempBuffer = new unsigned char[0x100000];
			alBank->inst[x]->sounds[y]->wav.len = tetDec.sphereDecompress(&ctl[alBank->inst[x]->sounds[y]->wav.base], tempBuffer);

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, tempBuffer, alBank->inst[x]->sounds[y]->wav.len);
			delete [] tempBuffer;
			
			if (numberInstruments == 0)
				alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW8;
			else
				alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW16;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudio16BitPCM(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = RAW16BITPCM;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = tblOffset - ctlOffset;

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ctlOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			// Byteflip
			for (int r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r += 2)
			{
				CSharedFunctions::WriteShortToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, r, CSharedFunctions::Flip16Bit(CSharedFunctions::CharArrayToShort(alBank->inst[x]->sounds[y]->wav.wavData, r)));
			}
			
			alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW16;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudio8BitPCM(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset, int numberInstruments)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = RAW8BITPCM;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;

			alBank->inst[x]->sounds[y]->wav.len = tblOffset - ctlOffset;

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[ctlOffset], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_UNSIGNED_RAW8;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRNCN64PtrOffset(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long offset)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	RncDecoder rnc;
	int returnedSize = rnc.unpackM1(&ctl[ctlOffset], outputDecompressed, 0x0, fileSizeCompressed);
	unsigned long ctlSizeUncompressed = fileSizeCompressed;
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[offset], ctlSizeUncompressed, 0, tbl);
	alBank->soundBankFormat = RNCCOMPRESSEDN64PTR;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioAVL_0Ptr(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = ctlSize;
	CMidwayDecoder midwayDec;
	int returnedSize = midwayDec.dec(&ctl[ctlOffset], fileSizeCompressed, outputDecompressed, "LZSS");
	unsigned long ctlSizeUncompressed = fileSizeCompressed;
	ALBank* alBank = ReadAudioN64PtrWavetableV2(outputDecompressed, ctlSizeUncompressed, 0, tbl);
	alBank->soundBankFormat = AVL_0PTR;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioSno(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long expectedSize)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	CSnoDecoder snoDecoder;
	snoDecoder.dec(&ctl[ctlOffset], ctlSize, expectedSize, outputDecompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], expectedSize, 0, tbl);
	alBank->soundBankFormat = SNOW;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRNCN64Ptr(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	return ReadAudioRNCN64PtrOffset(ctl, ctlSize, ctlOffset, tbl, 0);
}

ALBank* CN64AIFCAudio::ReadAudioClayfighter(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
		
	unsigned long header = CSharedFunctions::CharArrayToLong(ctl, ctlOffset);
	int lengthInput = CSharedFunctions::CharArrayToLong(ctl, ctlOffset + 4);
	int lengthOutput = CSharedFunctions::CharArrayToLong(ctl, ctlOffset + 8);

	int decompressedSize = CDecompressClayfighter::Decompress(outputDecompressed, &ctl[ctlOffset + 0xC]);

	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[numberInstruments], decompressedSize, 0, tbl, 0, 0, 0);
	alBank->soundBankFormat = CLAYFIGHTER;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRNC(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	RncDecoder rnc;
	int decompressedSize = rnc.unpackM1(&ctl[ctlOffset], outputDecompressed, 0x0, fileSizeCompressed);

	/*CString tempStr;
	tempStr.Format("C:\\temp\\nbajam99usrnc%08X.bin", ctlOffset);
	FILE* a = fopen(tempStr, "wb");
	fwrite(outputDecompressed, 1, decompressedSize, a);
	fflush(a);
	fclose(a);*/

	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], decompressedSize, 0, tbl, 0, 0, bankNumber);
	alBank->soundBankFormat = STANDARDRNCCOMPRESSED;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRNCDean(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = RNCDEAN;
	alBank->count = ((tblOffset - ctlOffset) / 0xC);
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		int y = 0;
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			
			unsigned long offset = ctlOffset + CSharedFunctions::CharArrayToLong(ctl, (ctlOffset + (x * 0xC)));
			unsigned long decompressedSize = CSharedFunctions::CharArrayToLong(ctl, (ctlOffset + (x * 0xC)) + 4);
			unsigned long crc = CSharedFunctions::CharArrayToLong(ctl, (ctlOffset + (x * 0xC)) + 8);
			alBank->inst[x]->sounds[y]->wav.base = offset;
			
			unsigned char* outputDecompressed = new unsigned char[0x2000000];

			if (CSharedFunctions::CharArrayToLong(ctl, offset) == 0x52415721) // RAW!
			{
				int rawLength = CSharedFunctions::CharArrayToLong(ctl, offset + 4);
				memcpy(outputDecompressed, &ctl[offset+8], rawLength);
			}
			else
			{
				int fileSizeCompressed = -1;
				RncDecoder rnc;
				int decodedSize = rnc.unpackM1(&ctl[offset], outputDecompressed, 0x0, fileSizeCompressed);
			}

			int sampleRate = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x0]);
			alBank->inst[x]->samplerate = sampleRate;

			int unknown = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x4]);
			int audioDataSize = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x8]);

			alBank->inst[x]->sounds[y]->wav.len = audioDataSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &outputDecompressed[0xC], alBank->inst[x]->sounds[y]->wav.len);

			alBank->inst[x]->sounds[y]->wav.type = AL_ACCLAIM_DEAN;

			delete [] outputDecompressed;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioDeanTable(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = DEANTABLE;
	alBank->count = CSharedFunctions::CharArrayToLong(ROM, ctlOffset);
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		int y = 0;
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			
			unsigned long offset = ctlOffset + CSharedFunctions::CharArrayToLong(ctl, (ctlOffset + ((x + 1) * 0x4)));
			alBank->inst[x]->sounds[y]->wav.base = offset;
			
			unsigned char* outputDecompressed = &ROM[offset];

			int sampleRate = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x0]);
			alBank->inst[x]->samplerate = sampleRate;

			int unknown = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x4]);
			int audioDataSize = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x8]);

			alBank->inst[x]->sounds[y]->wav.len = audioDataSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &outputDecompressed[0xC], alBank->inst[x]->sounds[y]->wav.len);

			alBank->inst[x]->sounds[y]->wav.type = AL_ACCLAIM_DEAN;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioDeanIndividual(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = DEANINDIVIDUAL;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11025;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		int y = 0;
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			
			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;
			
			unsigned char* outputDecompressed = &ROM[ctlOffset];

			int sampleRate = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x0]);
			alBank->inst[x]->samplerate = sampleRate;

			int unknown = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x4]);
			int audioDataSize = CSharedFunctions::CharArrayToLong(&outputDecompressed[0x8]);

			alBank->inst[x]->sounds[y]->wav.len = audioDataSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &outputDecompressed[0xC], alBank->inst[x]->sounds[y]->wav.len);

			alBank->inst[x]->sounds[y]->wav.type = AL_ACCLAIM_DEAN;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRNX(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber)
{
	unsigned char* outputDecompressedA = new unsigned char[0x2000000];
	unsigned char* outputDecompressedB = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	RncDecoder rnc;
	int size1 = rnc.unpackM1(&ctl[ctlOffset], outputDecompressedA, 0x0, fileSizeCompressed);

	int nextCtlSpot = ctlOffset + fileSizeCompressed;
	nextCtlSpot += (8 - (fileSizeCompressed % 8));

	nextCtlSpot += 0x10;

	int size2 = rnc.unpackM1(&ctl[nextCtlSpot], outputDecompressedB, 0x0, fileSizeCompressed);

	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	for (int x = 0; x < size1; x++)
	{
		outputDecompressed[x] = outputDecompressedA[x];
	}

	for (int x = 0; x < size2; x++)
	{
		outputDecompressed[x+size1] = outputDecompressedB[x];
	}

	delete [] outputDecompressedA;
	delete [] outputDecompressedB;

	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], fileSizeCompressed, 0, tbl, 0, 0, bankNumber);
	alBank->soundBankFormat = STANDARDRNXCOMPRESSED;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64SN64(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x2C]);

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SN64;
	alBank->count = numberInstruments;

	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x0000;

	unsigned long numberElementsSectionOne = 0;
	for (int x = 0; x < numberInstruments; x++)
	{
		unsigned long numberCheck = (CharArrayToLong(&ctl[ctlOffset + x*4]) & mask);
		if (numberCheck > numberElementsSectionOne)
			numberElementsSectionOne = numberCheck;
	}
	numberElementsSectionOne++;

	unsigned long dataOffset = ctlOffset + (numberInstruments * 0x4);
	if ((dataOffset % 0x8) != 0)
		dataOffset += 4;

	unsigned long numberElementsSectionTwo = 0;
	for (int x = 0; x < numberElementsSectionOne; x++)
	{
		unsigned long numberCheck = (CharArrayToShort(&ctl[dataOffset + x*0x14 + 0xA]) & mask);
		if (numberCheck > numberElementsSectionTwo)
			numberElementsSectionTwo = numberCheck;
	}
	numberElementsSectionTwo++;

	dataOffset = dataOffset + (numberElementsSectionOne * 0x14);
	if ((dataOffset % 0x8) != 0)
		dataOffset += 4;

	unsigned long numberLoops = 0x00000000;
	bool foundOneLoop = false;
	for (int x = 0; x < numberElementsSectionTwo; x++)
	{
		unsigned long numberCheck = (CharArrayToLong(&ctl[dataOffset + (x * 0x18) + 0x10]));
		if (numberCheck == 0xFFFFFFFF)
			continue;
		else
			foundOneLoop = true;

		if (numberCheck > numberLoops)
		{
			numberLoops = numberCheck;
		}
	}
	if (foundOneLoop)
		numberLoops++;
	
	alBank->inst = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long instrumentNumber = (CharArrayToLong(&ctl[ctlOffset + x*4]) & mask);

			alBank->inst[x]->soundCount = 1;
			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			unsigned long dataOffset = ctlOffset + (numberInstruments * 0x4);
			if ((dataOffset % 0x8) != 0)
				dataOffset += 4;

			alBank->inst[x]->sounds[0]->floatKeyBase = ctl[dataOffset + 4];
			alBank->inst[x]->pan = ctl[dataOffset + 1];
			alBank->inst[x]->volume = ctl[dataOffset + 2];

			// Maybe ?? Vol Pan Keybase, 0x14 per
			unsigned long dataNumber = (CharArrayToShort(&ctl[dataOffset + (instrumentNumber * 0x14)+ 0xA]) & mask);

			dataOffset = dataOffset + (numberElementsSectionOne * 0x14);
			if ((dataOffset % 0x8) != 0)
				dataOffset += 4;
			unsigned long rawDataOffset = dataOffset + (dataNumber * 0x18);

			unsigned long loopDataOffset;
			unsigned long predictorDataOffset;

			loopDataOffset = dataOffset + (numberElementsSectionTwo * 0x18) + 0x8;
			predictorDataOffset = dataOffset + (numberElementsSectionTwo * 0x18) + 0x8 + (numberLoops * 0x30);


			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y]->flags = 0x0000;

				if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
				{
					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[rawDataOffset + 0x0]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[rawDataOffset + 0x4]);
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					

					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						
						unsigned long predictorOffset = predictorDataOffset + 0x108 * dataNumber;
						unsigned long loopOffset = CharArrayToLong(&ctl[rawDataOffset + 0x10]);

						if (loopOffset != 0xFFFFFFFF)
						{
							loopOffset = loopDataOffset + loopOffset;
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

					
						if (predictorOffset != 0x00000000)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
						else
						{
							MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
						return NULL;
					}
				}
				else
				{
					DisposeALBank(alBank);
					MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
					return NULL;
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64SN64Zlib(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(MORTALKOMBAT);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

	ALBank* alBank = NULL;
	if (outputDecompressed != NULL)
	{
		unsigned long fileSizeUncompressedLong = fileSizeUncompressed;
		alBank = ReadAudioN64SN64(&outputDecompressed[0], fileSizeUncompressedLong, 0, tbl, numberInstruments, mask);
		alBank->soundBankFormat = ZLIBSN64;
		delete [] outputDecompressed;
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableBlitz(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	CBlitzDecoder blitzdec;
	blitzdec.dec(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2BLITZ;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioNinDec(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeCompressed)
{
	CNinDec ninDec;
	
	int expectedSize = ((((((ctl[ctlOffset+3] << 8) | ctl[ctlOffset+2]) << 8) | ctl[ctlOffset+1]) << 8) | ctl[ctlOffset+0]);

	unsigned char* outputDecompressed = new unsigned char[expectedSize*2]; // can go longer
	
	ninDec.dec(&ctl[ctlOffset], fileSizeCompressed, outputDecompressed);
	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], ctlSize, 0, tbl, 0, 0, 0);
	alBank->soundBankFormat = NINDEC;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioSydney(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeCompressed)
{
	SydneyDecoder decode;
	
	int expectedSize = CharArrayToLong(&ctl[ctlOffset+4]);

	unsigned char* outputDecompressed = new unsigned char[expectedSize];
	
	decode.dec(&ctl[ctlOffset+0x10], fileSizeCompressed, expectedSize, outputDecompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = SYDNEY;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64Titus(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	int fileSizeCompressed = -1;
	SupermanDecoder decode;
	int expectedSize = decode.header(&ctl[ctlOffset], fileSizeCompressed);
	unsigned char* outputDecompressed = new unsigned char[expectedSize];
	
	decode.dec(&ctl[ctlOffset+0x11], fileSizeCompressed, expectedSize, outputDecompressed);
	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], ctlSize, 0, tbl, 0x0, 0x0, 0);
	alBank->soundBankFormat = TITUS;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableQuake2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	CQuakeDecoder quakeDecoder;
	quakeDecoder.aridecode(&ctl[ctlOffset], 0x2000000, outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLEQUAKE2;
	delete [] outputDecompressed;
	return alBank;
}
ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableSnowboardKids(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	CSnowboardKidsDecoder snowboardKidsDecoder;
	snowboardKidsDecoder.decode(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV1(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLESNOWBOARDKIDS;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2Viewpoints(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeDecompressed)
{
	unsigned char* outputDecompressed = new unsigned char[fileSizeDecompressed];
	int fileSizeCompressed = -1;
	CViewpointDecoder viewpointDecoder;
	viewpointDecoder.dec(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed, fileSizeDecompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLEVIEWPOINT;
	delete [] outputDecompressed;
	return alBank;
}


ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2YAY0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	YAY0 yay0;
	yay0.decodeAll(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2YAY0;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMaddenBnkB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	// Madden 99 U
	//00590A74 First sounds
	//00680AF8 Second sounds
	//8039CD70 Madden It's in the game sound
	//8032C020 raw data
	//80338E38 output buffer

	unsigned short numberBands = 1;
	int startOffset = 0x14;
	unsigned long bankOffset = ctlOffset + startOffset;

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MADDENBNKB;
	alBank->count = (unsigned short)CharArrayToShort(&ctl[ctlOffset+0x6]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = bankOffset + (4 * x) + CharArrayToLong(&ctl[bankOffset + x*4]);

			if ((offsetInstrument == 0x0000) || (CharArrayToLong(&ctl[offsetInstrument]) != 0x50540200)) // PATb
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			alBank->inst[x]->soundCount = 1;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y]->hasWavePrevious = false;
				alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
				alBank->inst[x]->sounds[y]->flags = 0;

				alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
				alBank->inst[x]->sounds[y]->wav.rawWave = NULL;

				alBank->inst[x]->sounds[y]->wav.type = AL_MADDENBNKB;
				alBank->inst[x]->sounds[y]->wav.flags = 0x0000;

				alBank->inst[x]->sounds[y]->wav.base = 0;
				alBank->inst[x]->sounds[y]->wav.wavFlags = MADDENSTANDARDAUDIO;

				for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
				{
					if (CharArrayToShort(&ctl[z]) == 0x8402)
					{
						// sampling rate
						alBank->inst[x]->samplerate = CharArrayToShort(&ctl[z+2]);
					}
					else if (CharArrayToShort(&ctl[z]) == 0x8301)
					{
						if (ctl[z + 2] == 0x7)
							alBank->inst[x]->sounds[y]->wav.wavFlags = MADDENSTANDARDAUDIO;
						else if (ctl[z + 2] == 0x9)
							alBank->inst[x]->sounds[y]->wav.wavFlags = MADDENFLOATAUDIO;
						else
							throw;
					}
					else if (CharArrayToShort(&ctl[z]) == 0x8804)
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[z + 2]);
						if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0))
							break;
					}
					else if (CharArrayToShort(&ctl[z]) == 0x8C02)
					{
						alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToShort(&ctl[z+2]);
						if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0))
							break;
					}
					else if (CharArrayToShort(&ctl[z]) == 0x8502)
					{
						// # decoded samples
						alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToShort(&ctl[z+2]);
						if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0))
							break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x8503) || (CharArrayToShort(&ctl[z]) == 0x8C03))
					{
						// TODO check if maybe should be double and if is output length not input length
						alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToLong(&ctl[z+1]) & 0xFFFFFF;
						if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0))
							break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
					{
						break;
					}
				}

				if ((alBank->inst[x]->sounds[y]->wav.decompressedLength == 0) || (alBank->inst[x]->sounds[y]->wav.base == 0))
				{
					alBank->inst[x]->volume = 0;
					alBank->inst[x]->pan = 0;
					alBank->inst[x]->priority = 0;
					alBank->inst[x]->flags = 0;
					alBank->inst[x]->tremType = 0;
					alBank->inst[x]->tremRate = 0;
					alBank->inst[x]->tremDepth = 0;
					alBank->inst[x]->tremDelay = 0;
					alBank->inst[x]->vibType = 0;
					alBank->inst[x]->vibRate = 0;
					alBank->inst[x]->vibDepth = 0;
					alBank->inst[x]->vibDelay = 0;
					alBank->inst[x]->bendRange = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}

				unsigned long startSoundTblData = CharArrayToLong(&ctl[ctlOffset + 0x8]);
				unsigned long lengthTblData = CharArrayToLong(&ctl[ctlOffset + 0xC]);
				alBank->inst[x]->sounds[y]->wav.len = lengthTblData - (alBank->inst[x]->sounds[y]->wav.base - startSoundTblData);

				alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

				for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
				{
					alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMadden64(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, int tblOffset)
{
	unsigned char* tbl = &ctl[tblOffset];

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MADDEN64;
	alBank->count = ((tblOffset - ctlOffset) / 0xC);
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		int tableSpot = ctlOffset + (x * 0xC);
		unsigned long compressedDataOffset = ctlOffset + CSharedFunctions::CharArrayToLong(ctl, tableSpot);
		unsigned long decompressedSize = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 4);
		unsigned char compressionFlag = ctl[tableSpot + 8];
		unsigned long compressedDataSize = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 8) & 0xFFFFFF;

		if (compressionFlag != 0)
			throw;

		alBank->inst[x]->soundCount = 1;
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();

			alBank->inst[x]->sounds[y]->hasWavePrevious = false;
			alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
			alBank->inst[x]->sounds[y]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
			alBank->inst[x]->sounds[y]->wav.base = compressedDataOffset;

			int soundCompressedSize = CSharedFunctions::Flip16Bit(CSharedFunctions::CharArrayToShort(&ctl[compressedDataOffset + 2]));
			alBank->inst[x]->sounds[y]->wav.decompressedLength = (soundCompressedSize / 9) * 0x10;

			unsigned short unknownType = CSharedFunctions::Flip16Bit(CSharedFunctions::CharArrayToShort(&ctl[compressedDataOffset]));
			alBank->inst[x]->sounds[y]->wav.len = soundCompressedSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];
			memcpy(alBank->inst[x]->sounds[y]->wav.wavData, &ctl[compressedDataOffset + 4], alBank->inst[x]->sounds[y]->wav.len);
			
			alBank->inst[x]->sounds[y]->wav.type = AL_MADDEN64;
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioSeparatedBnkB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	int startOffset = 8;
	unsigned long bankOffset = ctlOffset + startOffset;

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SEPARATEDBNKB;
	alBank->count = 0x80;
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = bankOffset + CharArrayToLong(&ctl[bankOffset + x*4]);

			if ((offsetInstrument == 0x0000) || (CharArrayToLong(&ctl[offsetInstrument]) != 0x50415462)) // PATb
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			alBank->inst[x]->pan = ctl[offsetInstrument + 0xA];
			alBank->inst[x]->volume = ctl[offsetInstrument + 0xB];
			offsetInstrument = offsetInstrument + CharArrayToLong(&ctl[offsetInstrument + 0xC]) + 0x34;

			alBank->inst[x]->soundCount = 1;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				if (CharArrayToLong(&ctl[offsetInstrument]) == 0x544D6E62)
				{
					alBank->inst[x]->samplerate = CharArrayToShort(&ctl[offsetInstrument + 8]);
					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0xC]);

					unsigned long predictorOffset = offsetInstrument + 0x70;

					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
					// MUST PAD to 4s

					if (predictorOffset != 0x00000000)
					{
						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
						
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
					}
					else
					{
						alBank->inst[x]->sounds[y]->wav.type = AL_RAW16_WAVE;
						alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
					
						for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
						{
							if (CharArrayToShort(&ctl[z]) == 0x8C02)
							{
								alBank->inst[x]->sounds[y]->wav.len = CharArrayToShort(&ctl[z+2]);
								break;
							}
							else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
							{
								break;
							}
						}
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
					}
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioBnkB(unsigned char* ctl, int romSize, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	int startOffset;
	if (ctl[ctlOffset+4] == 0x4)
		startOffset = 0x14;
	else if (ctl[ctlOffset+4] == 0x2)
		startOffset = 0xC;
	else
		startOffset = 0x14;
	unsigned long bankOffset = ctlOffset + startOffset;

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = BNKB;
	alBank->count = (unsigned short)CharArrayToShort(&ctl[ctlOffset+0x6]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 16000;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	unsigned long tblOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+8]);

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			offsetInstrument = bankOffset + x*4 + offsetInstrument;

			alBank->inst[x]->soundCount = 1;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y]->wav.base = 0;
				unsigned long predictorOffset = 0;
				for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
				{
					if (CharArrayToShort(&ctl[z]) == 0x8804)
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[z + 2]);
						break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
					{
						break;
					}
				}
				for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
				{
					if ((CharArrayToShort(&ctl[z]) == 0x8F88) && (ctl[z+2] == 0x00))
					{
						predictorOffset = z + 2;
						break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
					{
						break;
					}
				}
				alBank->inst[x]->sounds[y]->wav.len = 0x80000;

				alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
				alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
				// MUST PAD to 4s

				if (predictorOffset != 0x00000000)
				{
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

					alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}

					for (int z = predictorOffset + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order + alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors) * 0x10); z < (predictorOffset + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order + alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors) * 0x10) + 0x7C); z++)
					{
						if (CharArrayToShort(&ctl[z]) == 0x8C02)
						{
							alBank->inst[x]->sounds[y]->wav.len = CharArrayToShort(&ctl[z+2]);
							break;
						}
						else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
						{
							break;
						}
					}
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
				}
				else
				{
					alBank->inst[x]->sounds[y]->wav.type = AL_SIGNED_RAW16;

					for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
					{
						if ((CharArrayToShort(&ctl[z]) == 0x8402) || (CharArrayToShort(&ctl[z]) == 0x8C42))
						{
							// sampling rate
							alBank->inst[x]->samplerate = CharArrayToShort(&ctl[z+2]);
						}
						else if (CharArrayToShort(&ctl[z]) == 0x8301)
						{
							alBank->inst[x]->sounds[y]->wav.type = AL_MADDENBNKB;
							if (ctl[z + 2] == 0x7)
								alBank->inst[x]->sounds[y]->wav.wavFlags = MADDENSTANDARDAUDIO;
							else if (ctl[z + 2] == 0x9)
								alBank->inst[x]->sounds[y]->wav.wavFlags = MADDENFLOATAUDIO;
							else
								throw;

							if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0) && (alBank->inst[x]->sounds[y]->wav.type == AL_MADDENBNKB))
								break;
						}
						else if (CharArrayToShort(&ctl[z]) == 0x8804)
						{
							alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[z + 2]);
							if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0) && (alBank->inst[x]->sounds[y]->wav.type == AL_MADDENBNKB))
								break;
						}
						else if (CharArrayToShort(&ctl[z]) == 0x8C02)
						{
							alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToShort(&ctl[z+2]);
							if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0) && (alBank->inst[x]->sounds[y]->wav.type == AL_MADDENBNKB))
								break;
						}
						else if (CharArrayToShort(&ctl[z]) == 0x8502)
						{
							// # decoded samples
							alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToShort(&ctl[z+2]);
							if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0) && (alBank->inst[x]->sounds[y]->wav.type == AL_MADDENBNKB))
								break;
						}
						else if ((CharArrayToShort(&ctl[z]) == 0x8503) || (CharArrayToShort(&ctl[z]) == 0x8C03))
						{
							// TODO check if maybe should be double and if is output length not input length
							alBank->inst[x]->sounds[y]->wav.decompressedLength = CharArrayToLong(&ctl[z+1]) & 0xFFFFFF;
							if ((alBank->inst[x]->sounds[y]->wav.decompressedLength > 0) && (alBank->inst[x]->sounds[y]->wav.base > 0) && (alBank->inst[x]->sounds[y]->wav.type == AL_MADDENBNKB))
								break;
						}
						if (CharArrayToShort(&ctl[z]) == 0x8C02)
						{
							alBank->inst[x]->sounds[y]->wav.len = CharArrayToShort(&ctl[z+2]);
							break;
						}
						else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
						{
							break;
						}
					}

					if ((tblOffset + alBank->inst[x]->sounds[y]->wav.base + alBank->inst[x]->sounds[y]->wav.len) > romSize)
						alBank->inst[x]->sounds[y]->wav.len = romSize - (tblOffset + alBank->inst[x]->sounds[y]->wav.base);

					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}

					if (alBank->inst[x]->sounds[y]->wav.type == AL_SIGNED_RAW16)
					{
						alBank->inst[x]->sounds[y]->wav.len = alBank->inst[x]->sounds[y]->wav.decompressedLength;
						// Byteflip
						for (int r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r += 2)
						{
							CSharedFunctions::WriteShortToBuffer(alBank->inst[x]->sounds[y]->wav.wavData, r, CSharedFunctions::Flip16Bit(CSharedFunctions::CharArrayToShort(alBank->inst[x]->sounds[y]->wav.wavData, r)));
						}
					}
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64MKMythologies(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long uncompressedSize)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	CMKMythologiesDecode compression;
	compression.dec(&ctl[ctlOffset], fileSizeCompressed, uncompressedSize, outputDecompressed);
	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], ctlSize, 0, tbl, 0x0, 0x0, 0);
	alBank->soundBankFormat = MKMYTHOLOGIES;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioArmyMenCompressed(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, bool littleEndianCompressedHeader, bool sarge2style)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = -1;
	n643docompression compression;
	compression.dec(&ctl[ctlOffset], compression.uncompressedSize(&ctl[ctlOffset], littleEndianCompressedHeader), outputDecompressed, fileSizeCompressed, littleEndianCompressedHeader, sarge2style);

	//FILE* outA = fopen("C:\\temp\\army.ctl", "wb");
	//fwrite(&outputDecompressed[0], 1, ctlSize, outA);
	//fclose(outA);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = ARMYMENFORMAT;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMegamanN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	if (CharArrayToLong(&ctl[ctlOffset]) == 0x4E363420)
	{
		return ReadAudioN64PtrWavetableV2(ctl, ctlSize, ctlOffset, tbl);
	}
	else
	{
		unsigned char* outputDecompressed = new unsigned char[0x2000000];
		int fileSizeCompressed = -1;
		MidwayLZ compression;
		ctlSize = compression.decLZSS0B(&ctl[ctlOffset], fileSizeCompressed, outputDecompressed);

		ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
		alBank->soundBankFormat = MEGAMAN64PTRV2;
		delete [] outputDecompressed;
		return alBank;
	}
}

ALBank* CN64AIFCAudio::ReadAudioTazN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	CString inTempFileName;
	inTempFileName.Format("tempASAS%08X.mus", ctlOffset);

	CString outTempFileName;
	outTempFileName.Format("tempASAS%08X.musb", ctlOffset);

	::DeleteFile(inTempFileName);
	::DeleteFile(outTempFileName);

	FILE* outTempIn = fopen(inTempFileName, "wb");
	if (outTempIn == NULL)
	{
		return NULL;
	}
	unsigned long expectedSize = CharArrayToLong(&ctl[ctlOffset+4]) - 0x14;
	fwrite(&expectedSize, 1, 4, outTempIn);
	fwrite(&ctl[ctlOffset+0x14], 1, ctlSize-0x14, outTempIn);
	fflush(outTempIn);
	fclose(outTempIn);

	flzh huffman;
	huffman.infile = fopen(inTempFileName, "rb");
	if (huffman.infile == NULL)
	{
		::DeleteFile(inTempFileName);
		return NULL;
	}

	huffman.outfile = fopen(outTempFileName, "wb");
	if (huffman.outfile == NULL)
	{
		return NULL;
	}

	huffman.Decode();

	fflush(huffman.outfile);
	fclose(huffman.infile);
	fclose(huffman.outfile);

	ctlSize = GetSizeFile(outTempFileName);

	FILE* inTempIn = fopen(outTempFileName, "rb");
	if (inTempIn == NULL)
	{
		return NULL;
	}
	
	unsigned char* outputDecompressed = new unsigned char[ctlSize];
	fread(outputDecompressed, 1, ctlSize, inTempIn);

	fclose(inTempIn);

	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = TAZHUFFMAN;
	delete [] outputDecompressed;

	::DeleteFile(inTempFileName);
	::DeleteFile(outTempFileName);
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioPaperMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = PAPERMARIO;
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 32000;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	unsigned long bankSize = CharArrayToLong(&ctl[ctlOffset + 4]);
	alBank->bankName.Format("%s", &ctl[ctlOffset + 0x8]);

	alBank->count = 0;
	for (int x = 0x12; x < 0x32; x+=2)
	{
		unsigned short instrumentOffset = CharArrayToShort(&ctl[ctlOffset + x]);
		if (instrumentOffset == 0x0000)
			break;

		alBank->count++;
	}

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = ctlOffset + CharArrayToShort(&ctl[ctlOffset + 0x12 + (x * 2)]);

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}
			
			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wav.unknown1 = CharArrayToLong(&ctl[offsetInstrument + 0x1C]);
						alBank->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec = CharArrayToLong(&ctl[offsetInstrument + 0x20]);

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0x18]);
							
							unsigned long loopOffset = offsetInstrument + 0xC;

							if ((CharArrayToLong(&ctl[loopOffset]) != 0) || (CharArrayToLong(&ctl[loopOffset + 0x4]) != 0) || (CharArrayToLong(&ctl[loopOffset + 0x8]) != 0))
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
							}

						
							if (predictorOffset != 0x00000000)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									predictorOffset = predictorOffset & 0xFFFFFF;
								else
									predictorOffset = ctlOffset + predictorOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 1;
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + z * 2]);
								}
							}
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
							return NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioB0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = B0;
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->count = 1;

	unsigned long bankOffset = ctlOffset;

	int soundCount = CharArrayToLong(&ctl[bankOffset]) / 0x10;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		int x = 0;
		{
			alBank->inst[x]->soundCount = soundCount;
			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				unsigned long offsetSound = bankOffset + (0x10 * y);

				alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound+0xC];
				alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound+0xD];
				alBank->inst[x]->sounds[y]->flags = ctl[offsetSound+0xE];

				
				if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
				{
					unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound+ 0x0]);
					offsetEnv = bankOffset + offsetEnv;

					alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
					alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
					alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
					alBank->inst[x]->sounds[y]->env.attackVolume = ctl[offsetEnv + 0xC];
					alBank->inst[x]->sounds[y]->env.decayVolume = ctl[offsetEnv + 0xD];
					alBank->inst[x]->sounds[y]->env.zeroPad = CharArrayToShort(&ctl[offsetEnv + 0xE]);



					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x8]);
					offsetWaveTable = bankOffset + offsetWaveTable;

					unsigned char flag = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) >> 24; // hack for NUD-DSCJ-JPN, can't imagine impact others
					if (flag == 0x40) // Means using 64DD IPL Bank
						alBank->inst[x]->sounds[y]->wav.base = (CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF) + 0x140000;
					else
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					

					alBank->inst[x]->sounds[y]->wav.type = ctl[offsetWaveTable + 0x8];
					alBank->inst[x]->sounds[y]->wav.flags = ctl[offsetWaveTable + 0x9];
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);

						if (loopOffset != 0)
						{
							loopOffset = bankOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							predictorOffset = bankOffset + predictorOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
					}
					else if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = bankOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.rawWave->loop = new ALRawLoop();
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);	
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
						return NULL;
					}
				}
				else
				{
					DisposeALBank(alBank);
					MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
					return NULL;
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioDuckDodgers(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = DUCKDODGERS;
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	unsigned long bankSize = CharArrayToLong(&ctl[ctlOffset + 4]);
	alBank->bankName.Format("%s", &ctl[ctlOffset + 0x8]);

	alBank->count = CharArrayToLong(&ctl[ctlOffset + 0xC]);

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = ctlOffset + 0x10 + (0x18 * x);

			alBank->inst[x]->flags = 0;

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}
			
			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x10]);

						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0xC]) + 0xA0;
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0xC]) + 0x20;
							unsigned long loopOffset = 0;//CharArrayToLong(&ctl[offsetInstrument + 0xC]);

							if (loopOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									loopOffset = loopOffset & 0xFFFFFF;
								else
									loopOffset = ctlOffset + loopOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
							}

						
							if (predictorOffset != 0x00000000)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									predictorOffset = predictorOffset & 0xFFFFFF;
								else
									predictorOffset = ctlOffset + predictorOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 4;
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + z * 2]);
								}
							}
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
							return NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x2C]);

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2;
	alBank->count = (unsigned short)CharArrayToLong(&ctl[ctlOffset+0x20]);
	unsigned long endOffsets = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x24]);
	unsigned long endExtraBytes = ctlOffset + CharArrayToLong(&ctl[ctlOffset + 0x28]);
	ctlSize = CharArrayToLong(&ctl[ctlOffset + 0x28]) + (alBank->count * 4);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->bankName.Format("%s", &ctl[ctlOffset + 0x14]);
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);
			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->volume = 0;
				alBank->inst[x]->pan = 0;
				alBank->inst[x]->priority = 0;
				alBank->inst[x]->flags = 0;
				alBank->inst[x]->tremType = 0;
				alBank->inst[x]->tremRate = 0;
				alBank->inst[x]->tremDepth = 0;
				alBank->inst[x]->tremDelay = 0;
				alBank->inst[x]->vibType = 0;
				alBank->inst[x]->vibRate = 0;
				alBank->inst[x]->vibDepth = 0;
				alBank->inst[x]->vibDelay = 0;
				alBank->inst[x]->bendRange = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;

			alBank->inst[x]->flags = 0;
			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetInstrument + 0xC]);

							if (loopOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									loopOffset = loopOffset & 0xFFFFFF;
								else
									loopOffset = ctlOffset + loopOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
							}

						
							if (predictorOffset != 0x00000000)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									predictorOffset = predictorOffset & 0xFFFFFF;
								else
									predictorOffset = ctlOffset + predictorOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
								}
							}
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
							return NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}

		// Semitones
		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->volume = ctl[endOffsets + x];
		}

		// Fine tones
		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->pan = ctl[endExtraBytes + x];
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}



ALBank* CN64AIFCAudio::ReadAudioZelda(std::vector<ctlTblResult>& results, unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset, int instrumentCount, unsigned long endSpot, unsigned char* rom, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, int ctlIndex, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd, bool isZLib)
{
	unsigned char* tbl1 = &rom[tblOffset];
	unsigned char* tbl2 = &rom[tblOffset];

	// IPL
	unsigned char* tbl3 = &rom[0];
	unsigned char* tbl4 = &rom[0];

	/*static int bankNumber = 0;
	FILE* outA = fopen("C:\\temp\\keyparse.txt", "a");
	fprintf(outA, "\n\nBank:%02X\n", bankNumber++);
	fclose(outA);*/
	ALBank* alBank = new ALBank();

	unsigned short numberBands = 1;
	unsigned long bankOffset = 0;

	unsigned long offsetInstrumentSection1 = CharArrayToLong(&ctl[ctlOffset]);
	unsigned long offsetInstrumentSection2 = CharArrayToLong(&ctl[ctlOffset + 4]);
	unsigned long endOffsetInstrument1Section1;
	unsigned long endOffsetInstrument1Section2;

	int instrumentCountNormal = instrumentCount;
	int instrumentCountSection1 = 0;
	int instrumentCountSection2 = 0;

	if (offsetInstrumentSection1 != NULL)
	{
		if (offsetInstrumentSection2 != NULL)
		{
			endOffsetInstrument1Section1 = offsetInstrumentSection2;
			endOffsetInstrument1Section2 = endSpot - ctlOffset;

			instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);
			instrumentCountSection2 = ((endOffsetInstrument1Section2 - offsetInstrumentSection2) / 8);
		}
		else
		{
			endOffsetInstrument1Section1 = endSpot - ctlOffset;

			instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);
		}
		
		// Make all percussion in bank 1 percussion instrument
	}
	else if (offsetInstrumentSection2 != NULL)
	{
		endOffsetInstrument1Section2 = endSpot - ctlOffset;
		instrumentCountSection2 = ((endOffsetInstrument1Section2 - offsetInstrumentSection2) / 8);
	}

	if (startZeldaCtlData != 0)
	{
		unsigned char* ctlTblTableData = rom;

		if (isCompressedZeldaCtlTblTables)
		{
			if (isZLib)
			{
				GECompression gedecompress;
				gedecompress.SetGame(MORTALKOMBAT);
				int fileSizeCompressed =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
				gedecompress.SetCompressedBuffer(&rom[compressedZeldaCtlTblTableStart], fileSizeCompressed);
				int fileSizeUncompressed;
				ctlTblTableData = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);
				if (ctlTblTableData == NULL)
				{
					delete alBank;
					return NULL;
				}
			}
			else
			{
				YAZ0 yaz;
				//CNintendoEncoder ninEnc;
				ctlTblTableData = new unsigned char[0x800000];
				int fileSizeCompressed =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
				//int decodedSize = ninEnc.decode(&rom[compressedZeldaCtlTblTableStart], fileSizeCompressed, ctlTblTableData);
				int decodedSize = yaz.yaz0_decode(&rom[compressedZeldaCtlTblTableStart], ctlTblTableData, fileSizeCompressed);
				if (decodedSize == 0)
				{
					delete [] ctlTblTableData;
					delete alBank;
					return NULL;
				}
			}
		}

		unsigned long ctlLength = CharArrayToLong(&ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 4]);

		int backwards = 0;
		while (ctlLength == 0)
		{
			backwards -= 0x10;
			ctlLength = CharArrayToLong(&ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 4]);
		}

		instrumentCountNormal = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xC];
		instrumentCount = instrumentCountNormal;

		instrumentCountSection1 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xD];
		instrumentCountSection2 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xF];

		ctlOffset = startZeldaCtlData + CharArrayToLong(&ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex)]);

		offsetInstrumentSection1 = CharArrayToLong(&rom[ctlOffset]);
		offsetInstrumentSection2 = CharArrayToLong(&rom[ctlOffset + 4]);

		int tblIndex1 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xA];

		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1) + 4]) == 0x0)
			tblIndex1--;

		tblOffset = startZeldaTblData + CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1)]);
		tbl1 = &rom[tblOffset];

		int tblIndex2 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xB];

		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2) + 4]) == 0x0)
			tblIndex2--;

		if (tblIndex2 != 0xFF)
		{
			tblOffset = startZeldaTblData + CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2)]);
			tbl2 = &rom[tblOffset];
		}
		else
		{
			tbl2 = tbl1;
		}

		if (isCompressedZeldaCtlTblTables)
		{
			delete [] ctlTblTableData;
			ctlTblTableData = NULL;
		}
	}

	alBank->count = instrumentCount;
	alBank->soundBankFormat = ZELDAFORMAT;
	alBank->flags = 0x0000;
	alBank->pad = 0x0000;
	alBank->samplerate = 32000;
	alBank->percussion = 0x0000;

	alBank->inst = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = instrumentCountSection1;
	if (alBank->countEADPercussion > 0)
	{
		alBank->eadPercussion = new EADPercussion[alBank->countEADPercussion];
	}
	else
	{
		alBank->eadPercussion = NULL;
	}

	alBank->countSfx = instrumentCountSection2;
	if (alBank->countSfx > 0)
	{
		alBank->alSfx = new ALWave*[alBank->countSfx];
	}
	else
	{
		alBank->alSfx = NULL;
	}

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < instrumentCountNormal + instrumentCountSection1 + instrumentCountSection2; x++)
		{
			if (x < instrumentCountNormal)
			{
				unsigned long offsetInstrument = CharArrayToLong(&ctl[ctlOffset + x*4 + 0x8]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->volume = 0;
					alBank->inst[x]->pan = 0;
					alBank->inst[x]->priority = 0;
					alBank->inst[x]->flags = 0;
					alBank->inst[x]->tremType = 0;
					alBank->inst[x]->tremRate = 0;
					alBank->inst[x]->tremDepth = 0;
					alBank->inst[x]->tremDelay = 0;
					alBank->inst[x]->vibType = 0;
					alBank->inst[x]->vibRate = 0;
					alBank->inst[x]->vibDepth = 0;
					alBank->inst[x]->vibDelay = 0;
					alBank->inst[x]->bendRange = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;
				
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSoundData = offsetInstrument;

					alBank->inst[x]->sounds[y]->unknown1 = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->floatKeyBasePrev = CharArrayToLong(&ctl[offsetInstrument + 0xC]);
					alBank->inst[x]->sounds[y]->floatKeyBase = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					alBank->inst[x]->sounds[y]->floatKeyBaseSec = CharArrayToLong(&ctl[offsetInstrument + 0x1C]);
					unsigned long offsetADRSData = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
					if (offsetADRSData != 0)
					{
						offsetADRSData = ctlOffset + offsetADRSData;

						for (int z = 0; z < 8; z++)
						{
							alBank->inst[x]->sounds[y]->adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
						}
					}


					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSoundData+ 0x10]);
					offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

					//30 special somehow
					alBank->inst[x]->sounds[y]->wav.wavFlags = ctl[offsetWaveTable];
					
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						if (((alBank->inst[x]->sounds[y]->wav.wavFlags & 0xC) >> 2) == 0)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (((alBank->inst[x]->sounds[y]->wav.wavFlags & 0xC) >> 2) == 1)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (((alBank->inst[x]->sounds[y]->wav.wavFlags & 0xC) >> 2) == 2)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (((alBank->inst[x]->sounds[y]->wav.wavFlags & 0xC) >> 2) == 3)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							predictorOffset = ctlOffset + predictorOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
					}


					unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSoundData+ 0x18]);
					if (offsetWaveTableSecondary == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
						offsetWaveTableSecondary = ctlOffset + offsetWaveTableSecondary;

						alBank->inst[x]->sounds[y]->wavSecondary.base = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
						alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

						alBank->inst[x]->sounds[y]->wavSecondary.wavFlags = ctl[offsetWaveTableSecondary];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							if (((alBank->inst[x]->sounds[y]->wavSecondary.wavFlags & 0xC) >> 2) == 0)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
							else if (((alBank->inst[x]->sounds[y]->wavSecondary.wavFlags & 0xC) >> 2) == 1)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wavSecondary.base + r];	
							else if (((alBank->inst[x]->sounds[y]->wavSecondary.wavFlags & 0xC) >> 2) == 2)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
							else if (((alBank->inst[x]->sounds[y]->wavSecondary.wavFlags & 0xC) >> 2) == 3)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wavSecondary.base + r];	
						}
						
						alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
							unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

							if (loopOffsetSecondary != 0)
							{
								loopOffsetSecondary = ctlOffset + loopOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
							}

							if (predictorOffsetSecondary != 0)
							{
								predictorOffsetSecondary = ctlOffset + predictorOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
								}
							}
						}
					}

					unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSoundData+ 0x8]);
					if (offsetWaveTablePrevious == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWavePrevious = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWavePrevious = true;
						offsetWaveTablePrevious = ctlOffset + offsetWaveTablePrevious;

						alBank->inst[x]->sounds[y]->wavPrevious.base = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
						alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavPrevious.wavFlags = ctl[offsetWaveTablePrevious];

						alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							if (((alBank->inst[x]->sounds[y]->wavPrevious.wavFlags & 0xC) >> 2) == 0)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (((alBank->inst[x]->sounds[y]->wavPrevious.wavFlags & 0xC) >> 2) == 1)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (((alBank->inst[x]->sounds[y]->wavPrevious.wavFlags & 0xC) >> 2) == 2)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (((alBank->inst[x]->sounds[y]->wavPrevious.wavFlags & 0xC) >> 2) == 3)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
							unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

							if (loopOffsetPrevious != 0)
							{
								loopOffsetPrevious = ctlOffset + loopOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
							}

							if (predictorOffsetPrevious != 0)
							{
								predictorOffsetPrevious = ctlOffset + predictorOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
								}
							}
						}
					}

					/*float keyBasePrev = *reinterpret_cast<float*> (&alBank->inst[x]->sounds[y]->floatKeyBasePrev);
					float floatKeyBase = *reinterpret_cast<float*> (&alBank->inst[x]->sounds[y]->floatKeyBase);
					float floatKeyBaseSec = *reinterpret_cast<float*> (&alBank->inst[x]->sounds[y]->floatKeyBaseSec);

					unsigned char keyBasePrevKey = ConvertEADGameValueToKeyBase(keyBasePrev);
					unsigned char floatKeyBaseKey = ConvertEADGameValueToKeyBase(floatKeyBase);
					unsigned char floatKeyBaseSecKey = ConvertEADGameValueToKeyBase(floatKeyBaseSec);

					FILE* outA = fopen("C:\\temp\\keyparse.txt", "a");
					if (alBank->inst[x]->sounds[y]->hasWavePrevious && alBank->inst[x]->sounds[y]->hasWaveSecondary)
						fprintf(outA, "Inst:%02X Prev %02X Prim %02X Sec  %02X   (%08X)", x, keyBasePrevKey, floatKeyBaseKey, floatKeyBaseSecKey, alBank->inst[x]->sounds[y]->unknown1);
					else if (alBank->inst[x]->sounds[y]->hasWavePrevious)
						fprintf(outA, "Inst:%02X Prev %02X Prim %02X           (%08X)", x, keyBasePrevKey, floatKeyBaseKey, alBank->inst[x]->sounds[y]->unknown1);
					else if (alBank->inst[x]->sounds[y]->hasWaveSecondary)
						fprintf(outA, "Inst:%02X         Prim %02X Sec  %02X   (%08X)", x, floatKeyBaseKey, floatKeyBaseSecKey, alBank->inst[x]->sounds[y]->unknown1);
					else
						fprintf(outA, "Inst:%02X         Prim %02X           (%08X)", x, floatKeyBaseKey, alBank->inst[x]->sounds[y]->unknown1);

					if (offsetADRSData != 0)
					{
						fprintf(outA, " ");
						for (int zz = 0; zz < 0x10; zz++)
						{
							fprintf(outA, "%02X", ctl[offsetADRSData+zz]);
							if (((zz % 2) == 1) && (zz > 0))
								fprintf(outA, " ");
						}
					}

					fprintf(outA, "\n");
					fclose(outA);*/
				}
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1))
			{
				int percussionInNumber = x - instrumentCountNormal;

				alBank->eadPercussion[percussionInNumber].wav.adpcmWave = NULL;
				alBank->eadPercussion[percussionInNumber].keyBase = 0;
				alBank->eadPercussion[percussionInNumber].wav.type = AL_ADPCM_WAVE;

				unsigned long percussionItemOffset = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection1 + (x-instrumentCountNormal)*4]);
				if (percussionItemOffset == 0x0000)
				{
					continue;
				}

				percussionItemOffset = ctlOffset + percussionItemOffset;

				unsigned long offsetWaveTable = ctlOffset + CharArrayToLong(&ctl[percussionItemOffset+0x4]);

				alBank->eadPercussion[percussionInNumber].unknown1A = ctl[percussionItemOffset+0x0];
				alBank->eadPercussion[percussionInNumber].pan = ctl[percussionItemOffset+0x1];
				alBank->eadPercussion[percussionInNumber].unknown1C = CharArrayToShort(&ctl[percussionItemOffset+0x2]);

				alBank->eadPercussion[percussionInNumber].keyBase = CharArrayToLong(&ctl[percussionItemOffset+0x8]);

				unsigned long offsetADRSData = CharArrayToLong(&ctl[percussionItemOffset + 0xC]);
				if (offsetADRSData != 0)
				{
					offsetADRSData = ctlOffset + offsetADRSData;

					for (int z = 0; z < 8; z++)
					{
						alBank->eadPercussion[percussionInNumber].adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
					}
				}

				alBank->eadPercussion[percussionInNumber].wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
				alBank->eadPercussion[percussionInNumber].wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

				alBank->eadPercussion[percussionInNumber].wav.wavFlags = ctl[ctlOffset];

				alBank->eadPercussion[percussionInNumber].wav.wavData = new unsigned char[alBank->eadPercussion[percussionInNumber].wav.len];

				for (int  r = 0; r < alBank->eadPercussion[percussionInNumber].wav.len; r++)
				{
					if (((alBank->eadPercussion[percussionInNumber].wav.wavFlags & 0xC) >> 2) == 0)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl1[alBank->eadPercussion[percussionInNumber].wav.base + r];
					else if (((alBank->eadPercussion[percussionInNumber].wav.wavFlags & 0xC) >> 2) == 1)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl2[alBank->eadPercussion[percussionInNumber].wav.base + r];					
					else if (((alBank->eadPercussion[percussionInNumber].wav.wavFlags & 0xC) >> 2) == 2)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl3[alBank->eadPercussion[percussionInNumber].wav.base + r];
					else if (((alBank->eadPercussion[percussionInNumber].wav.wavFlags & 0xC) >> 2) == 3)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl4[alBank->eadPercussion[percussionInNumber].wav.base + r];
				}

				alBank->eadPercussion[percussionInNumber].wav.type = AL_ADPCM_WAVE;
				alBank->eadPercussion[percussionInNumber].wav.flags = 0;

				alBank->eadPercussion[percussionInNumber].wav.adpcmWave = new ALADPCMWaveInfo();
				unsigned long loopOffset;
				unsigned long predictorOffset;
				
				loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
				predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

				if (loopOffset != 0)
				{
					loopOffset = ctlOffset + loopOffset;

					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop = new ALADPCMloop();
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

					if (alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->start != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
						}
					}
				}
				else
				{
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop = NULL;
				}

				if (predictorOffset != 0)
				{
					predictorOffset = ctlOffset + predictorOffset;

					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book = new ALADPCMBook();
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->predictors = new signed short[alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order * alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order * alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}
				}
				else
				{
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book = NULL;
				}
				continue;
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1 + instrumentCountSection2))
			{
				int soundInNumber = x - instrumentCountNormal - instrumentCountSection1;

				unsigned long offsetInstrument = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection2 + (x-instrumentCountNormal-instrumentCountSection1)*8]);
				if (offsetInstrument == 0x0000)
				{
					alBank->alSfx[soundInNumber] = NULL;
					continue;
				}

				offsetInstrument = ctlOffset + offsetInstrument;

				unsigned long offsetSound = offsetInstrument;


				alBank->alSfx[soundInNumber] = new ALWave();
				// Float key
				alBank->alSfx[soundInNumber]->unknown1 = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection2 + (x-instrumentCountNormal-instrumentCountSection1)*8 + 4]);

				alBank->alSfx[soundInNumber]->wavData = NULL;

				alBank->alSfx[soundInNumber]->base = CharArrayToLong(&ctl[offsetSound + 0x4]);
				alBank->alSfx[soundInNumber]->len = CharArrayToLong(&ctl[offsetSound + 0x0]) & 0xFFFFFF;
				alBank->alSfx[soundInNumber]->wavData = new unsigned char[alBank->alSfx[soundInNumber]->len];

				alBank->alSfx[soundInNumber]->wavFlags = ctl[offsetSound];

				for (int r = 0; r < alBank->alSfx[soundInNumber]->len; r++)
				{
					if (((alBank->alSfx[soundInNumber]->wavFlags & 0xC) >> 2) == 0)
						alBank->alSfx[soundInNumber]->wavData[r] = tbl1[alBank->alSfx[soundInNumber]->base + r];
					else if (((alBank->alSfx[soundInNumber]->wavFlags & 0xC) >> 2) == 1)
						alBank->alSfx[soundInNumber]->wavData[r] = tbl2[alBank->alSfx[soundInNumber]->base + r];
					else if (((alBank->alSfx[soundInNumber]->wavFlags & 0xC) >> 2) == 2)
						alBank->alSfx[soundInNumber]->wavData[r] = tbl3[alBank->alSfx[soundInNumber]->base + r];
					else if (((alBank->alSfx[soundInNumber]->wavFlags & 0xC) >> 2) == 3)
						alBank->alSfx[soundInNumber]->wavData[r] = tbl4[alBank->alSfx[soundInNumber]->base + r];
				}
				
				alBank->alSfx[soundInNumber]->type == AL_ADPCM_WAVE;
				alBank->alSfx[soundInNumber]->flags = 0x00;

				alBank->alSfx[soundInNumber]->adpcmWave = new ALADPCMWaveInfo();

				unsigned long predictorOffset = CharArrayToLong(&ctl[offsetSound + 0xC]);
				if (predictorOffset != 0)
				{
					if (alBank->alSfx[soundInNumber]->flags == 0x1)
						predictorOffset = predictorOffset & 0xFFFFFF;
					else
						predictorOffset = ctlOffset + predictorOffset;

					alBank->alSfx[soundInNumber]->adpcmWave->loop = NULL;

					alBank->alSfx[soundInNumber]->adpcmWave->book = new ALADPCMBook();
					alBank->alSfx[soundInNumber]->adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->alSfx[soundInNumber]->adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->alSfx[soundInNumber]->adpcmWave->book->predictors = new signed short[alBank->alSfx[soundInNumber]->adpcmWave->book->order * alBank->alSfx[soundInNumber]->adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->alSfx[soundInNumber]->adpcmWave->book->order * alBank->alSfx[soundInNumber]->adpcmWave->book->npredictors * 8; z++)
					{
						alBank->alSfx[soundInNumber]->adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}
				}

				unsigned long loopOffset = CharArrayToLong(&ctl[offsetSound + 0x8]);
				if (loopOffset != 0)
				{
					if (alBank->alSfx[soundInNumber]->flags == 0x1)
						loopOffset = loopOffset & 0xFFFFFF;
					else
						loopOffset = ctlOffset + loopOffset;

					alBank->alSfx[soundInNumber]->adpcmWave->loop = new ALADPCMloop();
					alBank->alSfx[soundInNumber]->adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
					alBank->alSfx[soundInNumber]->adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
					alBank->alSfx[soundInNumber]->adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
					alBank->alSfx[soundInNumber]->adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);
					if (alBank->alSfx[soundInNumber]->adpcmWave->loop->start != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							alBank->alSfx[soundInNumber]->adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
						}
					}
				}
				else
				{
					alBank->alSfx[soundInNumber]->adpcmWave->loop = NULL;
				}
				continue;
			}
		}
	}

	return alBank;
}


ALBank* CN64AIFCAudio::ReadAudioStarFox(unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset, int instrumentCount, unsigned long endSpot, unsigned char* rom, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, int ctlIndex, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd)
{
	/*FILE* sfx = fopen("C:\\temp\\ys.txt", "a");
	fprintf(sfx, "Read Audio Ctl %08X,Tbl %08X\n", ctlOffset, tblOffset);
	fclose(sfx);*/

	unsigned char* tbl1 = &rom[tblOffset];
	unsigned char* tbl2 = &rom[tblOffset];

	// IPL Absolute Offsets Tbl 2/3
	unsigned char* tbl3 = &rom[0];
	unsigned char* tbl4 = &rom[0];

	unsigned long tblOffset2 = tblOffset;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = STARFOX64FORMAT;
	alBank->count = instrumentCount;
	alBank->flags = 0x0000;
	alBank->pad = 0x0000;
	alBank->samplerate = 8000;
	alBank->percussion = 0x0000;

	unsigned short numberBands = 1;
	unsigned long bankOffset = 0;

	int instrumentCountNormal = instrumentCount;
	int instrumentCountSection1 = 0;

	unsigned long offsetInstrumentSection1 = CharArrayToLong(&ctl[ctlOffset]);
	unsigned long endOffsetInstrument1Section1 = 0;


	if (startZeldaCtlData != 0)
	{
		unsigned char* ctlTblTableData = rom;

		if (isCompressedZeldaCtlTblTables)
		{
			YAZ0 yaz;
			//CNintendoEncoder ninEnc;
			ctlTblTableData = new unsigned char[0x800000];
			int fileSizeCompressed =  compressedZeldaCtlTblTableEnd - compressedZeldaCtlTblTableStart + 1;
			//int decodedSize = ninEnc.decode(&rom[compressedZeldaCtlTblTableStart], fileSizeCompressed, ctlTblTableData);
			int decodedSize = yaz.yaz0_decode(&rom[compressedZeldaCtlTblTableStart], ctlTblTableData, fileSizeCompressed);
			if (decodedSize == 0)
			{
				delete [] ctlTblTableData;
				delete alBank;
				return NULL;
			}
		}

		unsigned long ctlLength = CharArrayToLong(&ctlTblTableData[offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 4]);

		int backwards = 0;
		while (ctlLength == 0)
		{
			backwards -= 0x10;
			ctlLength = CharArrayToLong(&ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 4]);
		}

		instrumentCountNormal = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xC];
		instrumentCount = instrumentCountNormal;

		instrumentCountSection1 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xD];
	
		ctlOffset = startZeldaCtlData + CharArrayToLong(&ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex)]);

		offsetInstrumentSection1 = CharArrayToLong(&rom[ctlOffset]);

		int tblIndex1 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xA];

		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1) + 4]) == 0x0)
			tblIndex1--;

		tblOffset = startZeldaTblData + CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex1)]);
		tbl1 = &rom[tblOffset];

		int tblIndex2 = ctlTblTableData[backwards + offsetZeldaCtlTable + 0x10 + (0x10 * ctlIndex) + 0xB];

		while (CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2) + 4]) == 0x0)
			tblIndex2--;

		if (tblIndex2 != 0xFF)
		{
			tblOffset2 = startZeldaTblData + CharArrayToLong(&ctlTblTableData[offsetZeldaTblTable + 0x10 + (0x10 * tblIndex2)]);
			tbl2 = &rom[tblOffset2];
		}
		else
		{
			tbl2 = tbl1;
		}

		if (isCompressedZeldaCtlTblTables)
		{
			delete [] ctlTblTableData;
			ctlTblTableData = NULL;
		}

		alBank->eadPercussion = new EADPercussion[instrumentCountSection1];
	}
	else if (offsetInstrumentSection1 != NULL)
	{
		endOffsetInstrument1Section1 = endSpot - ctlOffset;
		instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);

		alBank->eadPercussion = new EADPercussion[instrumentCountSection1];
	}
	else
	{
		alBank->eadPercussion = NULL;
	}


	

	

	alBank->inst = NULL;
	alBank->countEADPercussion = instrumentCountSection1;
	

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count + alBank->countEADPercussion; x++)
		{		
			if (x < instrumentCountNormal)
			{
				unsigned long offsetInstrument = CharArrayToLong(&ctl[ctlOffset + x*4 + 0x4]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->volume = 0;
					alBank->inst[x]->pan = 0;
					alBank->inst[x]->priority = 0;
					alBank->inst[x]->flags = 0;
					alBank->inst[x]->tremType = 0;
					alBank->inst[x]->tremRate = 0;
					alBank->inst[x]->tremDepth = 0;
					alBank->inst[x]->tremDelay = 0;
					alBank->inst[x]->vibType = 0;
					alBank->inst[x]->vibRate = 0;
					alBank->inst[x]->vibDepth = 0;
					alBank->inst[x]->vibDelay = 0;
					alBank->inst[x]->bendRange = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;
				
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSoundData = offsetInstrument;

					alBank->inst[x]->sounds[y]->unknown1 = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->floatKeyBasePrev = CharArrayToLong(&ctl[offsetInstrument + 0xC]);
					alBank->inst[x]->sounds[y]->floatKeyBase = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					alBank->inst[x]->sounds[y]->floatKeyBaseSec = CharArrayToLong(&ctl[offsetInstrument + 0x1C]);
					unsigned long offsetADRSData = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
					if (offsetADRSData != 0)
					{
						offsetADRSData = ctlOffset + offsetADRSData;

						for (int z = 0; z < 8; z++)
						{
							alBank->inst[x]->sounds[y]->adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
						}
					}


					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSoundData+ 0x10]);
					offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					alBank->inst[x]->sounds[y]->wav.wavFlags = ctl[offsetWaveTable];

					/*FILE* sfx = fopen("C:\\temp\\ys.txt", "a");
					fprintf(sfx, "%02X %02X\n", x, alBank->inst[x]->sounds[y]->wav.wavFlags);
					fclose(sfx);*/

					unsigned char wavFlags = ((alBank->inst[x]->sounds[y]->wav.wavFlags & 0xC) >> 2);

					/*FILE* outFileTest = fopen("C:\\temp\\testInstr.txt", "a");
					if (wavFlags == 0)
						fprintf(outFileTest, "%04X,%04X,Prim,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset, tblOffset + alBank->inst[x]->sounds[y]->wav.base, tblOffset + alBank->inst[x]->sounds[y]->wav.base + alBank->inst[x]->sounds[y]->wav.len);
					else if (wavFlags == 1)
						fprintf(outFileTest, "%04X,%04X,Prim,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset2, tblOffset2 + alBank->inst[x]->sounds[y]->wav.base, tblOffset2 + alBank->inst[x]->sounds[y]->wav.base + alBank->inst[x]->sounds[y]->wav.len);
					else if (wavFlags == 2)
						fprintf(outFileTest, "%04X,%04X,Prim,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wav.base, 0 + alBank->inst[x]->sounds[y]->wav.base + alBank->inst[x]->sounds[y]->wav.len);
					else if (wavFlags == 3)
						fprintf(outFileTest, "%04X,%04X,Prim,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wav.base, 0 + alBank->inst[x]->sounds[y]->wav.base + alBank->inst[x]->sounds[y]->wav.len);
					fclose(outFileTest);*/

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						if (wavFlags == 0)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (wavFlags == 1)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (wavFlags == 2)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wav.base + r];
						else if (wavFlags == 3)
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							predictorOffset = ctlOffset + predictorOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
					}


					unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSoundData+ 0x18]);
					if (offsetWaveTableSecondary == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
						offsetWaveTableSecondary = ctlOffset + offsetWaveTableSecondary;

						alBank->inst[x]->sounds[y]->wavSecondary.base = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
						alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

						alBank->inst[x]->sounds[y]->wavSecondary.wavFlags = ctl[offsetWaveTableSecondary];

						unsigned char wavFlags = ((alBank->inst[x]->sounds[y]->wavSecondary.wavFlags & 0xC) >> 2);

						/*FILE* outFileTest = fopen("C:\\temp\\testInstr.txt", "a");
						if (wavFlags == 0)
							fprintf(outFileTest, "%04X,%04X,Sec,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset, tblOffset + alBank->inst[x]->sounds[y]->wavSecondary.base, tblOffset + alBank->inst[x]->sounds[y]->wavSecondary.base + alBank->inst[x]->sounds[y]->wavSecondary.len);
						else if (wavFlags == 1)
							fprintf(outFileTest, "%04X,%04X,Sec,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset2, tblOffset2 + alBank->inst[x]->sounds[y]->wavSecondary.base, tblOffset2 + alBank->inst[x]->sounds[y]->wavSecondary.base + alBank->inst[x]->sounds[y]->wavSecondary.len);
						else if (wavFlags == 2)
							fprintf(outFileTest, "%04X,%04X,Sec,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wavSecondary.base, 0 + alBank->inst[x]->sounds[y]->wavSecondary.base + alBank->inst[x]->sounds[y]->wavSecondary.len);
						else if (wavFlags == 3)
							fprintf(outFileTest, "%04X,%04X,Sec,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wavSecondary.base, 0 + alBank->inst[x]->sounds[y]->wavSecondary.base + alBank->inst[x]->sounds[y]->wavSecondary.len);
						fclose(outFileTest);*/

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							if (wavFlags == 0)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
							else if (wavFlags == 1)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wavSecondary.base + r];							
							else if (wavFlags == 2)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
							else if (wavFlags == 3)
								alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
							unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

							if (loopOffsetSecondary != 0)
							{
								loopOffsetSecondary = ctlOffset + loopOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
							}

							if (predictorOffsetSecondary != 0)
							{
								predictorOffsetSecondary = ctlOffset + predictorOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
								}
							}
						}
					}

					unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSoundData+ 0x8]);
					if (offsetWaveTablePrevious == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWavePrevious = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWavePrevious = true;
						offsetWaveTablePrevious = ctlOffset + offsetWaveTablePrevious;

						alBank->inst[x]->sounds[y]->wavPrevious.base = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
						alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

						alBank->inst[x]->sounds[y]->wavPrevious.wavFlags = ctl[offsetWaveTablePrevious];

						unsigned char wavFlags = ((alBank->inst[x]->sounds[y]->wavPrevious.wavFlags & 0xC) >> 2);

						/*FILE* outFileTest = fopen("C:\\temp\\testInstr.txt", "a");
						if (wavFlags == 0)
							fprintf(outFileTest, "%04X,%04X,Prev,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset, tblOffset + alBank->inst[x]->sounds[y]->wavPrevious.base, tblOffset + alBank->inst[x]->sounds[y]->wavPrevious.base + alBank->inst[x]->sounds[y]->wavPrevious.len);
						else if (wavFlags == 1)
							fprintf(outFileTest, "%04X,%04X,Prev,%02X,%08X,%08X,%08X\n", x, y, wavFlags, tblOffset2, tblOffset2 + alBank->inst[x]->sounds[y]->wavPrevious.base, tblOffset2 + alBank->inst[x]->sounds[y]->wavPrevious.base + alBank->inst[x]->sounds[y]->wavPrevious.len);
						else if (wavFlags == 2)
							fprintf(outFileTest, "%04X,%04X,Prev,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wavPrevious.base, 0 + alBank->inst[x]->sounds[y]->wavPrevious.base + alBank->inst[x]->sounds[y]->wavPrevious.len);
						else if (wavFlags == 3)
							fprintf(outFileTest, "%04X,%04X,Prev,%02X,%08X,%08X,%08X\n", x, y, wavFlags, 0, 0 + alBank->inst[x]->sounds[y]->wavPrevious.base, 0 + alBank->inst[x]->sounds[y]->wavPrevious.base + alBank->inst[x]->sounds[y]->wavPrevious.len);
						fclose(outFileTest);*/

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							if (wavFlags == 0)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl1[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (wavFlags == 1)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl2[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (wavFlags == 2)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl3[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
							else if (wavFlags == 3)
								alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl4[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
							unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

							if (loopOffsetPrevious != 0)
							{
								loopOffsetPrevious = ctlOffset + loopOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
							}

							if (predictorOffsetPrevious != 0)
							{
								predictorOffsetPrevious = ctlOffset + predictorOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
								}
							}
						}
					}
				}
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1))
			{
				int percussionInNumber = x - instrumentCountNormal;

				unsigned long percussionItemOffset = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection1 + (x-instrumentCountNormal)*4]);
				if (percussionItemOffset == 0x0000)
				{
					continue;
				}
				percussionItemOffset = ctlOffset + percussionItemOffset;

				unsigned long offsetWaveTable = ctlOffset + CharArrayToLong(&ctl[percussionItemOffset+0x4]);

				alBank->eadPercussion[percussionInNumber].unknown1A = ctl[percussionItemOffset+0x0];
				alBank->eadPercussion[percussionInNumber].pan = ctl[percussionItemOffset+0x1];
				alBank->eadPercussion[percussionInNumber].unknown1C = CharArrayToShort(&ctl[percussionItemOffset+0x2]);

				alBank->eadPercussion[percussionInNumber].keyBase = CharArrayToLong(&ctl[percussionItemOffset+0x8]);

				unsigned long offsetADRSData = CharArrayToLong(&ctl[percussionItemOffset + 0xC]);
				if (offsetADRSData != 0)
				{
					offsetADRSData = ctlOffset + offsetADRSData;

					for (int z = 0; z < 8; z++)
					{
						alBank->eadPercussion[percussionInNumber].adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
					}
				}

				alBank->eadPercussion[percussionInNumber].wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
				alBank->eadPercussion[percussionInNumber].wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

				alBank->eadPercussion[percussionInNumber].wav.wavData = new unsigned char[alBank->eadPercussion[percussionInNumber].wav.len];

				alBank->eadPercussion[percussionInNumber].wav.wavFlags = ctl[offsetWaveTable];

				unsigned char wavFlags = ((alBank->eadPercussion[percussionInNumber].wav.wavFlags & 0xC) >> 2);

				/*FILE* outFileTest = fopen("C:\\temp\\testInstr.txt", "a");
				if (wavFlags == 0)
					fprintf(outFileTest, "%04X,%04X,Drum,%02X,%08X,%08X,%08X\n", 0, percussionInNumber, wavFlags, tblOffset, tblOffset + alBank->eadPercussion[percussionInNumber].wav.base, tblOffset + alBank->eadPercussion[percussionInNumber].wav.base + alBank->eadPercussion[percussionInNumber].wav.len);
				else if (wavFlags == 1)
					fprintf(outFileTest, "%04X,%04X,Drum,%02X,%08X,%08X,%08X\n", 0, percussionInNumber, wavFlags, tblOffset2, tblOffset2 + alBank->eadPercussion[percussionInNumber].wav.base, tblOffset2 + alBank->eadPercussion[percussionInNumber].wav.base + alBank->eadPercussion[percussionInNumber].wav.len);
				else if (wavFlags == 2)
					fprintf(outFileTest, "%04X,%04X,Drum,%02X,%08X,%08X,%08X\n", 0, percussionInNumber, wavFlags, 0, 0 + alBank->eadPercussion[percussionInNumber].wav.base, 0 + alBank->eadPercussion[percussionInNumber].wav.base + alBank->eadPercussion[percussionInNumber].wav.len);
				else if (wavFlags == 3)
					fprintf(outFileTest, "%04X,%04X,Drum,%02X,%08X,%08X,%08X\n", 0, percussionInNumber, wavFlags, 0, 0 + alBank->eadPercussion[percussionInNumber].wav.base, 0 + alBank->eadPercussion[percussionInNumber].wav.base + alBank->eadPercussion[percussionInNumber].wav.len);
				fclose(outFileTest);*/

				for (int  r = 0; r < alBank->eadPercussion[percussionInNumber].wav.len; r++)
				{
					if (wavFlags == 0)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl1[alBank->eadPercussion[percussionInNumber].wav.base + r];
					else if (wavFlags == 1)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl2[alBank->eadPercussion[percussionInNumber].wav.base + r];
					else if (wavFlags == 2)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl3[alBank->eadPercussion[percussionInNumber].wav.base + r];
					else if (wavFlags == 3)
						alBank->eadPercussion[percussionInNumber].wav.wavData[r] = tbl4[alBank->eadPercussion[percussionInNumber].wav.base + r];
				}

				alBank->eadPercussion[percussionInNumber].wav.type = AL_ADPCM_WAVE;
				alBank->eadPercussion[percussionInNumber].wav.flags = 0;

				alBank->eadPercussion[percussionInNumber].wav.adpcmWave = new ALADPCMWaveInfo();
				unsigned long loopOffset;
				unsigned long predictorOffset;
				
				loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
				predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

				if (loopOffset != 0)
				{
					loopOffset = ctlOffset + loopOffset;

					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop = new ALADPCMloop();
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

					if (alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->start != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
						}
					}
				}
				else
				{
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->loop = NULL;
				}

				if (predictorOffset != 0)
				{
					predictorOffset = ctlOffset + predictorOffset;

					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book = new ALADPCMBook();
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->predictors = new signed short[alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order * alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->order * alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}
				}
				else
				{
					alBank->eadPercussion[percussionInNumber].wav.adpcmWave->book = NULL;
				}
				continue;
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize, int bankNumber)
{
	FILE* outDebug = NULL;
	//FILE* outDebug = fopen("C:\\temp\\mariodata.txt", "w");;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SUPERMARIO64FORMAT;

	//int bankCount = CharArrayToShort(&ctl[ctlOffset+2]);

	alBank->flags = 0x0000;
	alBank->samplerate = 22050;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		unsigned long offsetInstrument = ctlOffset + CharArrayToLong(&ctl[ctlOffset + 4 + (bankNumber * 8)]);
		unsigned long offsetTbl = CharArrayToLong(&tbl[4 + (bankNumber * 8)]);

		alBank->count = CharArrayToLong(&ctl[offsetInstrument]);
		alBank->inst = new ALInst*[alBank->count];

		alBank->countEADPercussion = CharArrayToLong(&ctl[offsetInstrument+4]);
		alBank->eadPercussion = NULL;

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		alBank->unknown2 = CharArrayToLong(&ctl[offsetInstrument+8]);
		alBank->unknown3 = CharArrayToLong(&ctl[offsetInstrument+0xC]);

		unsigned long offsetSoundData = offsetInstrument + 0x10;

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->soundCount = 0;

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int r = 0; r < alBank->inst[x]->soundCount; r++)
				{
					alBank->inst[x]->sounds[r] = new ALSound();
					alBank->inst[x]->sounds[r]->wav.wavData = NULL;
				}

				int y = 0;
				unsigned long offsetSound = CharArrayToLong(&ctl[offsetSoundData + 4 + x*0x4]);
				if (offsetSound == 0x0)
				{
					delete alBank->inst[x]->sounds[y];
					alBank->inst[x]->sounds[y] = NULL;
					alBank->inst[x]->soundCount = 0;
					continue;
				}

				offsetSound = offsetSoundData + offsetSound;

				alBank->inst[x]->sounds[y]->flags = 0x0;

				alBank->inst[x]->sounds[y]->unknown1 = CharArrayToLong(&ctl[offsetSound+ 0x0]);
				alBank->inst[x]->sounds[y]->floatKeyBasePrev = CharArrayToLong(&ctl[offsetSound+ 0xC]);
				alBank->inst[x]->sounds[y]->floatKeyBase = CharArrayToLong(&ctl[offsetSound+ 0x14]);
				alBank->inst[x]->sounds[y]->floatKeyBaseSec = CharArrayToLong(&ctl[offsetSound+ 0x1C]);
				unsigned long offsetADRSData = CharArrayToLong(&ctl[offsetSound + 0x4]);
				if (offsetADRSData != 0)
				{
					offsetADRSData = offsetInstrument + offsetADRSData + 0x10;

					for (int z = 0; z < 8; z++)
					{
						alBank->inst[x]->sounds[y]->adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
					}
				}

				unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x10]);
				offsetWaveTable = offsetSoundData + offsetWaveTable;

				alBank->inst[x]->sounds[y]->wav.unknown1 = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
				alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
				alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);
				alBank->inst[x]->sounds[y]->wav.unknown2 = CharArrayToLong(&ctl[offsetWaveTable + 0x14]);
				alBank->inst[x]->sounds[y]->wav.unknown3 = CharArrayToLong(&ctl[offsetWaveTable + 0x18]);
				alBank->inst[x]->sounds[y]->wav.unknown4 = CharArrayToLong(&ctl[offsetWaveTable + 0x1C]);

				alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

				for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
				{
					alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[offsetTbl + alBank->inst[x]->sounds[y]->wav.base + r];
				}

				if (outDebug != NULL)
				{
					fprintf(outDebug, "Instrument %02X Sound %02X Length %08X ", x, y, alBank->inst[x]->sounds[y]->wav.len);
					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						fprintf(outDebug, "%02X", alBank->inst[x]->sounds[y]->wav.wavData[r]);
					}
					fprintf(outDebug, "\n");
				}
				
				alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
				alBank->inst[x]->sounds[y]->wav.flags = 0;
				// MUST PAD to 4s

				if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
				{
					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
					unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
					unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

					if (loopOffset != 0)
					{
						loopOffset = offsetSoundData + loopOffset;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

						if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0 ||
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count != 0)
						{
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
							}
						}
					}
					else
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
					}

					if (predictorOffset != 0)
					{
						predictorOffset = offsetSoundData + predictorOffset;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
					}
				}


				unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSound+ 0x18]);
				if (offsetWaveTableSecondary == 0x00000000)
					alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
				else
				{
					alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
					offsetWaveTableSecondary = offsetSoundData + offsetWaveTableSecondary;

					alBank->inst[x]->sounds[y]->wavSecondary.unknown1 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]);
					alBank->inst[x]->sounds[y]->wavSecondary.base = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
					alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x10]);
					alBank->inst[x]->sounds[y]->wavSecondary.unknown2 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x14]);
					alBank->inst[x]->sounds[y]->wavSecondary.unknown3 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x18]);
					alBank->inst[x]->sounds[y]->wavSecondary.unknown4 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x1C]);

					alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
					{
						alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl[offsetTbl + alBank->inst[x]->sounds[y]->wavSecondary.base + r];
					}

					if (outDebug != NULL)
					{
						fprintf(outDebug, "Instrument %02X Sound Secondary %02X Length %08X ", x, y, alBank->inst[x]->sounds[y]->wavSecondary.len);
						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							fprintf(outDebug, "%02X", alBank->inst[x]->sounds[y]->wavSecondary.wavData[r]);
						}
						fprintf(outDebug, "\n");
					}
					
					alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
						unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

						if (loopOffsetSecondary != 0)
						{
							loopOffsetSecondary = offsetSoundData + loopOffsetSecondary;

							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

							if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0 ||
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count != 0)

							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
						}

						if (predictorOffsetSecondary != 0)
						{
							predictorOffsetSecondary = offsetSoundData + predictorOffsetSecondary;

							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
							}
						}
					}
				}

				unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSound+ 0x8]);
				if (offsetWaveTablePrevious == 0x00000000)
					alBank->inst[x]->sounds[y]->hasWavePrevious = false;
				else
				{
					alBank->inst[x]->sounds[y]->hasWavePrevious = true;
					offsetWaveTablePrevious = offsetSoundData + offsetWaveTablePrevious;

					alBank->inst[x]->sounds[y]->wavPrevious.unknown1 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]);
					alBank->inst[x]->sounds[y]->wavPrevious.base = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
					alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x10]);
					alBank->inst[x]->sounds[y]->wavPrevious.unknown2 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x14]);
					alBank->inst[x]->sounds[y]->wavPrevious.unknown3 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x18]);
					alBank->inst[x]->sounds[y]->wavPrevious.unknown4 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x1C]);

					alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
					{
						alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl[offsetTbl + alBank->inst[x]->sounds[y]->wavPrevious.base + r];
					}

					if (outDebug != NULL)
					{
						fprintf(outDebug, "Instrument %02X Sound Previous %02X Length %08X ", x, y, alBank->inst[x]->sounds[y]->wavPrevious.len);
						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							fprintf(outDebug, "%02X", alBank->inst[x]->sounds[y]->wavPrevious.wavData[r]);
						}
						fprintf(outDebug, "\n");
					}
					
					alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
						unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

						if (loopOffsetPrevious != 0)
						{
							loopOffsetPrevious = offsetSoundData + loopOffsetPrevious;

							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

							if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0 ||
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count != 0)

							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
						}

						if (predictorOffsetPrevious != 0)
						{
							predictorOffsetPrevious = offsetSoundData + predictorOffsetPrevious;

							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
							}
						}
					}
				}
			}
		}

		if (alBank->countEADPercussion > 0)
		{
			unsigned long percussionOffset = offsetSoundData + CharArrayToLong(&ctl[offsetInstrument+0x10]);
			alBank->eadPercussion = new EADPercussion[alBank->countEADPercussion];

			for (int y = 0; y < alBank->countEADPercussion; y++)
			{
				alBank->eadPercussion[y].wav.type = AL_ADPCM_WAVE;

				if (CharArrayToLong(&ctl[percussionOffset+(4*y)]) == 0)
				{
					alBank->countEADPercussion = y;

					alBank->eadPercussion[y].wav.adpcmWave = NULL;
					break;
				}

				unsigned long percussionItemOffset = offsetSoundData + CharArrayToLong(&ctl[percussionOffset+(4*y)]);
				
				unsigned long offsetWaveTable = offsetSoundData + CharArrayToLong(&ctl[percussionItemOffset+0x4]);

				alBank->eadPercussion[y].unknown1A = ctl[percussionItemOffset+0x0];
				alBank->eadPercussion[y].pan = ctl[percussionItemOffset+0x1];
				alBank->eadPercussion[y].unknown1C = CharArrayToShort(&ctl[percussionItemOffset+0x2]);

				alBank->eadPercussion[y].keyBase = CharArrayToLong(&ctl[percussionItemOffset+0x8]);

				unsigned long offsetADRSData = CharArrayToLong(&ctl[percussionItemOffset + 0xC]);
				if (offsetADRSData != 0)
				{
					offsetADRSData = offsetInstrument + offsetADRSData + 0x10;

					for (int z = 0; z < 8; z++)
					{
						alBank->eadPercussion[y].adsrEAD[z] = CharArrayToShort(&ctl[offsetADRSData + z*2]);
					}
				}

				alBank->eadPercussion[y].wav.unknown1 = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
				alBank->eadPercussion[y].wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
				alBank->eadPercussion[y].wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);
				alBank->eadPercussion[y].wav.unknown2 = CharArrayToLong(&ctl[offsetWaveTable + 0x14]);
				alBank->eadPercussion[y].wav.unknown3 = CharArrayToLong(&ctl[offsetWaveTable + 0x18]);
				alBank->eadPercussion[y].wav.unknown4 = CharArrayToLong(&ctl[offsetWaveTable + 0x1C]);

				alBank->eadPercussion[y].wav.wavData = new unsigned char[alBank->eadPercussion[y].wav.len];

				for (int  r = 0; r < alBank->eadPercussion[y].wav.len; r++)
				{
					alBank->eadPercussion[y].wav.wavData[r] = tbl[offsetTbl + alBank->eadPercussion[y].wav.base + r];
				}

				if (outDebug != NULL)
				{
					fprintf(outDebug, "Percussion %02X Length %08X ", y, alBank->eadPercussion[y].wav.len);
					for (int  r = 0; r < alBank->eadPercussion[y].wav.len; r++)
					{
						fprintf(outDebug, "%02X", alBank->eadPercussion[y].wav.wavData[r]);
					}
					fprintf(outDebug, "\n");
				}
				
				alBank->eadPercussion[y].wav.type = AL_ADPCM_WAVE;
				alBank->eadPercussion[y].wav.flags = 0;

				alBank->eadPercussion[y].wav.adpcmWave = new ALADPCMWaveInfo();
				unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
				unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

				if (loopOffset != 0)
				{
					loopOffset = offsetSoundData + loopOffset;

					alBank->eadPercussion[y].wav.adpcmWave->loop = new ALADPCMloop();
					alBank->eadPercussion[y].wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
					alBank->eadPercussion[y].wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
					alBank->eadPercussion[y].wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
					alBank->eadPercussion[y].wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

					if (alBank->eadPercussion[y].wav.adpcmWave->loop->start != 0 ||
						alBank->eadPercussion[y].wav.adpcmWave->loop->count != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							alBank->eadPercussion[y].wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
						}
					}
				}
				else
				{
					alBank->eadPercussion[y].wav.adpcmWave->loop = NULL;
				}

				if (predictorOffset != 0)
				{
					predictorOffset = offsetSoundData + predictorOffset;

					alBank->eadPercussion[y].wav.adpcmWave->book = new ALADPCMBook();
					alBank->eadPercussion[y].wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->eadPercussion[y].wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->eadPercussion[y].wav.adpcmWave->book->predictors = new signed short[alBank->eadPercussion[y].wav.adpcmWave->book->order * alBank->eadPercussion[y].wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->eadPercussion[y].wav.adpcmWave->book->order * alBank->eadPercussion[y].wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->eadPercussion[y].wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}
				}
				else
				{
					alBank->eadPercussion[y].wav.adpcmWave->book = NULL;
				}
			}
		}
		else
		{
			alBank->eadPercussion = NULL;
		}
	}

	if (outDebug != NULL)
	{
		fflush(outDebug);
		fclose(outDebug);
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRawAllowed(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber)
{
	ALBank* alBank = ReadAudio(ROM, ctl, ctlSize, ctlOffset, tbl, ctlFlaggedOffset, mask, bankNumber);
	alBank->soundBankFormat = STANDARDFORMATRAWALLOWED;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2ZLIB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(MORTALKOMBAT);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

	ALBank* alBank = NULL;
	if (outputDecompressed != NULL)
	{
		unsigned long fileSizeUncompressedLong = fileSizeUncompressed;
		alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], fileSizeUncompressedLong, 0, tbl);
		alBank->soundBankFormat = N64PTRWAVETABLETABLEV2ZLIB;
		delete [] outputDecompressed;
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRawTest(unsigned char* rawData, int size)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = STANDARDFORMAT;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11050;
	alBank->percussion = 0;
	
	alBank->inst = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->volume = 0;
			alBank->inst[x]->pan = 0;
			alBank->inst[x]->priority = 0;
			alBank->inst[x]->flags = 0;
			alBank->inst[x]->tremType = 0;
			alBank->inst[x]->tremRate = 0;
			alBank->inst[x]->tremDepth = 0;
			alBank->inst[x]->tremDelay = 0;
			alBank->inst[x]->vibType = 0;
			alBank->inst[x]->vibRate = 0;
			alBank->inst[x]->vibDepth = 0;
			alBank->inst[x]->vibDelay = 0;
			alBank->inst[x]->bendRange = 0;
			alBank->inst[x]->soundCount = 1;

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{
				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->samplePan = 0;
					alBank->inst[x]->sounds[y]->sampleVolume = 0;
					alBank->inst[x]->sounds[y]->flags = 0;

					
					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->env.attackTime = 0;
						alBank->inst[x]->sounds[y]->env.decayTime = 0;
						alBank->inst[x]->sounds[y]->env.releaseTime = 0;
						alBank->inst[x]->sounds[y]->env.attackVolume = 0;
						alBank->inst[x]->sounds[y]->env.decayVolume = 0;

						alBank->inst[x]->sounds[y]->key.velocitymin = 0;
						alBank->inst[x]->sounds[y]->key.velocitymax = 0;
						alBank->inst[x]->sounds[y]->key.keymin = 0;
						alBank->inst[x]->sounds[y]->key.keymax = 0;
						alBank->inst[x]->sounds[y]->key.keybase = 0;
						alBank->inst[x]->sounds[y]->key.detune = 0;

						alBank->inst[x]->sounds[y]->wav.base = 0;
						alBank->inst[x]->sounds[y]->wav.len = size;
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = rawData[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_RAW16_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0;
						// MUST PAD to 4s

					
						alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioVox(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int samplingRate)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = VOX;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	int numberBanks = 1;
	alBank->inst = new ALInst*[numberBanks];

	alBank->inst[0] = new ALInst();
	alBank->inst[0]->samplerate = 44100;
	alBank->inst[0]->sounds = NULL;

	alBank->inst[0]->soundCount = 1;

	alBank->inst[0]->sounds = new ALSound*[alBank->inst[0]->soundCount];

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y] = new ALSound();
		alBank->inst[0]->sounds[y]->wav.wavData = NULL;

		alBank->inst[0]->samplerate = samplingRate;
	}

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y]->flags = 0;

		alBank->inst[0]->sounds[y]->wav.base = ctlOffset;
		alBank->inst[0]->sounds[y]->wav.len = ctlSize;
		alBank->inst[0]->sounds[y]->wav.wavData = new unsigned char[ctlSize];

		for (int  r = 0; r < alBank->inst[0]->sounds[y]->wav.len; r++)
		{
			alBank->inst[0]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[0]->sounds[y]->wav.base + r];
		}
		

		alBank->inst[0]->sounds[y]->wav.type = AL_VOX_WAVE;
		alBank->inst[0]->sounds[y]->wav.flags = 0;
		// MUST PAD to 4s
		
		alBank->inst[0]->sounds[y]->wav.adpcmWave = NULL;
		alBank->inst[0]->sounds[y]->wav.rawWave = NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioWav(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = WAV;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	int numberBanks = 1;
	alBank->inst = new ALInst*[numberBanks];

	alBank->inst[0] = new ALInst();
	alBank->inst[0]->samplerate = 44100;
	alBank->inst[0]->sounds = NULL;

	alBank->inst[0]->soundCount = 1;

	alBank->inst[0]->sounds = new ALSound*[alBank->inst[0]->soundCount];

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y] = new ALSound();
		alBank->inst[0]->sounds[y]->wav.wavData = NULL;

		alBank->inst[0]->samplerate = Flip32Bit(CharArrayToLong(&ctl[ctlOffset + 0x18]));
	}

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y]->flags = 0;

		alBank->inst[0]->sounds[y]->wav.base = ctlOffset;
		alBank->inst[0]->sounds[y]->wav.len = ctlSize;
		alBank->inst[0]->sounds[y]->wav.wavData = new unsigned char[ctlSize];

		for (int  r = 0; r < alBank->inst[0]->sounds[y]->wav.len; r++)
		{
			alBank->inst[0]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[0]->sounds[y]->wav.base + r];
		}
		

		alBank->inst[0]->sounds[y]->wav.type = AL_WAV;
		alBank->inst[0]->sounds[y]->wav.flags = 0;
		// MUST PAD to 4s
		
		alBank->inst[0]->sounds[y]->wav.adpcmWave = NULL;
		alBank->inst[0]->sounds[y]->wav.rawWave = NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMORT(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MORT;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	int numberBanks = 1;
	alBank->inst = new ALInst*[numberBanks];

	alBank->inst[0] = new ALInst();
	alBank->inst[0]->samplerate = 44100;
	alBank->inst[0]->sounds = NULL;

	alBank->inst[0]->soundCount = 1;

	alBank->inst[0]->sounds = new ALSound*[alBank->inst[0]->soundCount];

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y] = new ALSound();
		alBank->inst[0]->sounds[y]->wav.wavData = NULL;

		alBank->inst[0]->samplerate = CharArrayToShort(&ctl[ctlOffset + 6]);
	}

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y]->flags = 0;

		alBank->inst[0]->sounds[y]->wav.base = ctlOffset;
		alBank->inst[0]->sounds[y]->wav.len = ctlSize;
		alBank->inst[0]->sounds[y]->wav.wavData = new unsigned char[ctlSize];

		for (int  r = 0; r < alBank->inst[0]->sounds[y]->wav.len; r++)
		{
			alBank->inst[0]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[0]->sounds[y]->wav.base + r];
		}
		

		alBank->inst[0]->sounds[y]->wav.type = AL_MORT_WAVE;
		alBank->inst[0]->sounds[y]->wav.flags = 0;
		// MUST PAD to 4s
		
		alBank->inst[0]->sounds[y]->wav.adpcmWave = NULL;
		alBank->inst[0]->sounds[y]->wav.rawWave = NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioNamcoMuseum(unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = NAMCOMUSEUM;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();

		alBank->inst[x]->samplerate = 11025;
		alBank->inst[x]->sounds = NULL;

		unsigned short samplingRate = CharArrayToShort(&ctl[ctlOffset + 2]);
		int decompressedSize = CharArrayToLong(&ctl[ctlOffset + 4]);

		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		alBank->inst[x]->samplerate = samplingRate;

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;

			alBank->inst[x]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.base = ctlOffset;
			alBank->inst[x]->sounds[y]->wav.len = tblOffset - ctlOffset;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = decompressedSize;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
			

			alBank->inst[x]->sounds[y]->wav.type = AL_NAMCOMUSEUM;
			alBank->inst[x]->sounds[y]->wav.flags = 0;
			// MUST PAD to 4s
			
			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioTopGearHyperBike(unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = TOPGEARHYPERBIKE;
	alBank->count = ((tblOffset - ctlOffset) / 0x18);
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();

		int tableSpot = ctlOffset + (x * 0x18);
		unsigned long compressedSpot = ctlOffset + CSharedFunctions::CharArrayToLong(ctl, tableSpot);
		unsigned long numberUncompressedSamples = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 4);
		unsigned long extraValue = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 8);
		unsigned short samplingRate = CSharedFunctions::CharArrayToShort(ctl, tableSpot + 12);
		unsigned short unknown = CSharedFunctions::CharArrayToShort(ctl, tableSpot + 14);
		unsigned short unknown2 = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 16);
		unsigned short unknown3 = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 20);

		alBank->inst[x]->samplerate = samplingRate;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		alBank->inst[x]->samplerate = samplingRate * 2;

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;

			alBank->inst[x]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.base = compressedSpot;
			alBank->inst[x]->sounds[y]->wav.len = (numberUncompressedSamples + 1) / 2;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = numberUncompressedSamples;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
			

			alBank->inst[x]->sounds[y]->wav.type = AL_TOPGEARHYPERBIKE;
			alBank->inst[x]->sounds[y]->wav.flags = 0;
			// MUST PAD to 4s
			
			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioTopGearOverdrive(unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = TOPGEAROVERDRIVE;
	alBank->count = ((tblOffset - ctlOffset) / 0x14);
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();

		int tableSpot = ctlOffset + (x * 0x14);
		unsigned long compressedSpot = ctlOffset + CSharedFunctions::CharArrayToLong(ctl, tableSpot);
		unsigned long numberUncompressedSamples = CSharedFunctions::CharArrayToShort(ctl, tableSpot + 4);
		unsigned short samplingRate = CSharedFunctions::CharArrayToShort(ctl, tableSpot + 6);
		unsigned short unknown = CSharedFunctions::CharArrayToShort(ctl, tableSpot + 8);
		unsigned short unknown2 = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 12);
		unsigned short unknown3 = CSharedFunctions::CharArrayToLong(ctl, tableSpot + 16);

		alBank->inst[x]->samplerate = samplingRate;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		alBank->inst[x]->samplerate = samplingRate * 2;

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;

			alBank->inst[x]->flags = 0;

			alBank->inst[x]->sounds[y]->wav.base = compressedSpot;
			alBank->inst[x]->sounds[y]->wav.len = (numberUncompressedSamples + 1) / 2;
			alBank->inst[x]->sounds[y]->wav.decompressedLength = numberUncompressedSamples;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
			

			alBank->inst[x]->sounds[y]->wav.type = AL_TOPGEARHYPERBIKE;
			alBank->inst[x]->sounds[y]->wav.flags = 0;
			// MUST PAD to 4s
			
			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioKobe2(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = KOBE2;
	alBank->count = CharArrayToLong(&ctl[ctlOffset + 0x10]);
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();

		alBank->inst[x]->samplerate = 11025;
		alBank->inst[x]->sounds = NULL;

		int soundType = ctl[ctlOffset + 0x14 + (0x10 * x)];
		unsigned long address = ctlOffset + CharArrayToLong(&ctl[ctlOffset + 0x14 + (0x10 * x) + 0xC]);
		int numberChunks = CharArrayToShort(&ctl[ctlOffset + 0x14 + (0x10 * x) + 4]);

		int size = (numberChunks * 0xC0);
		unsigned short samplingRate = CharArrayToShort(&ctl[ctlOffset + 0x14 + (0x10 * x) + 2]);

		if (soundType == 2)
			samplingRate /= 2;
		else if (soundType == 3)
			samplingRate *= 2;

		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		alBank->inst[x]->samplerate = samplingRate;

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;

			alBank->inst[x]->flags = soundType;

			alBank->inst[x]->sounds[y]->wav.base = address;
			alBank->inst[x]->sounds[y]->wav.len = size;
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = ctl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
			

			alBank->inst[x]->sounds[y]->wav.type = AL_KOBE2_WAVE;
			alBank->inst[x]->sounds[y]->wav.flags = 0;
			// MUST PAD to 4s
			
			alBank->inst[x]->sounds[y]->wav.adpcmWave = NULL;
			alBank->inst[x]->sounds[y]->wav.rawWave = NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudio64dd(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned long header = CharArrayToLong(&ctl[ctlOffset]);
	if (header != 0x19970123)
	{
		MessageBox(NULL, "Error", "Bad header for 64dd bank", NULL);
		return NULL;
	}

	int numberWaveforms = CharArrayToLong(&ctl[ctlOffset+4]);
	unsigned long wavePointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+8]);
	unsigned long bookPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0xC]);
	unsigned long loopPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x10]);
	unsigned long bookDataPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x14]);
	unsigned long loopDataPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x18]);
	unsigned long informationPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x1C]);

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64DD;
	alBank->count = numberWaveforms;
	alBank->flags = 0;
	alBank->percussion = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 32000;
		alBank->inst[x]->sounds = NULL;

		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;
		}

		unsigned long wavePointerSound = ctlOffset + CharArrayToLong(&ctl[wavePointer + (x * 4)]);
		unsigned long bookPointerSound = ctlOffset + CharArrayToLong(&ctl[bookPointer + (x * 4)]);
		unsigned long loopPointerSound = ctlOffset + CharArrayToLong(&ctl[loopPointer + (x * 4)]);
		unsigned long informationPointerSound = informationPointer + (x * 0x20);

		WaveInformation waveInfo;
		waveInfo.version = CharArrayToLong(&ctl[informationPointerSound]);
		waveInfo.adpcmsize = CharArrayToLong(&ctl[informationPointerSound+4]);
		waveInfo.samplesize = CharArrayToLong(&ctl[informationPointerSound+8]);
		waveInfo.loop_startsamplepoint = CharArrayToLong(&ctl[informationPointerSound+0xC]);
		waveInfo.loop_endsamplepoint = CharArrayToLong(&ctl[informationPointerSound+0x10]);
		waveInfo.loop_count = CharArrayToShort(&ctl[informationPointerSound+0x14]);
		waveInfo.book_bytes = ctl[informationPointerSound+0x16];
		waveInfo.samplingbaseMIDIkey = ctl[informationPointerSound+0x17];
		waveInfo.samplingrate = CharArrayToFloat(&ctl[informationPointerSound+0x18]);
		waveInfo.tuning_C4_32K = CharArrayToFloat(&ctl[informationPointerSound+0x1C]);
		
		alBank->inst[x]->sounds[0]->flags = 0;

		alBank->inst[x]->sounds[0]->wav.sampleRateNotInDefaultNintendoSpec = waveInfo.samplingrate;
		alBank->inst[x]->sounds[0]->key.keybase = waveInfo.samplingbaseMIDIkey;
		alBank->inst[x]->sounds[0]->key.detune = waveInfo.tuning_C4_32K;

		alBank->inst[x]->sounds[0]->wav.base = wavePointerSound;
		alBank->inst[x]->sounds[0]->wav.len = waveInfo.adpcmsize;
		alBank->inst[x]->sounds[0]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[0]->wav.len];

		for (int  r = 0; r < alBank->inst[x]->sounds[0]->wav.len; r++)
		{
			alBank->inst[x]->sounds[0]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[0]->wav.base + r];
		}
		

		alBank->inst[x]->sounds[0]->wav.type = AL_ADPCM_WAVE;
		alBank->inst[x]->sounds[0]->wav.flags = 0;
		// MUST PAD to 4s
		

		if ((alBank->inst[x]->sounds[0]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[0]->flags == 0x0) || (alBank->inst[x]->sounds[0]->flags == 0x1)))
		{
			alBank->inst[x]->sounds[0]->wav.adpcmWave = new ALADPCMWaveInfo();
			
			alBank->inst[x]->sounds[0]->wav.adpcmWave->loop = new ALADPCMloop();
			alBank->inst[x]->sounds[0]->wav.adpcmWave->loop->start = waveInfo.loop_startsamplepoint;
			alBank->inst[x]->sounds[0]->wav.adpcmWave->loop->end = waveInfo.loop_endsamplepoint;
			alBank->inst[x]->sounds[0]->wav.adpcmWave->loop->count = waveInfo.loop_count;
			for (int z = 0; z < 0x10; z++)
			{
				alBank->inst[x]->sounds[0]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopPointerSound + z * 2]);
			}

			alBank->inst[x]->sounds[0]->wav.adpcmWave->book = new ALADPCMBook();
			alBank->inst[x]->sounds[0]->wav.adpcmWave->book->order = 2;
			alBank->inst[x]->sounds[0]->wav.adpcmWave->book->npredictors = 2;
			alBank->inst[x]->sounds[0]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[0]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[0]->wav.adpcmWave->book->npredictors * 8];

			for (int z = 0; z < alBank->inst[x]->sounds[0]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[0]->wav.adpcmWave->book->npredictors * 8; z++)
			{
				alBank->inst[x]->sounds[0]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[bookPointerSound + z * 2]);
			}
		}
		else
		{
			DisposeALBank(alBank);
			MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
			return NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioConker(unsigned char* ctl, int ctlSize, int ctlOffset, int ctlOffsetPartTwo, unsigned char* tbl)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(CONKER);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* ctlDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

	if (ctlDecompressed == NULL)
	{
		return NULL;
	}

	unsigned long bankOffset = 0x0;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = CONKERFORMAT;

	alBank->count = CharArrayToShort(&ctlDecompressed[bankOffset+0x8]);
	alBank->samplerate = CharArrayToShort(&ctlDecompressed[bankOffset+0xE]);

	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		unsigned long offsetInstrument = CharArrayToLong(&ctlDecompressed[0x14 + (x * 4)]);
		
		bool isSoundType = (offsetInstrument & 0x1);

		if (!isSoundType)
		{
			alBank->inst[x]->volume = ctlDecompressed[offsetInstrument];
			alBank->inst[x]->pan = ctlDecompressed[offsetInstrument+1];
			alBank->inst[x]->priority = ctlDecompressed[offsetInstrument+2];
			alBank->inst[x]->flags = ctlDecompressed[offsetInstrument+3];
			alBank->inst[x]->tremType = ctlDecompressed[offsetInstrument+4];
			alBank->inst[x]->tremRate = ctlDecompressed[offsetInstrument+5];
			alBank->inst[x]->tremDepth = ctlDecompressed[offsetInstrument+6];
			alBank->inst[x]->tremDelay = ctlDecompressed[offsetInstrument+7];
			alBank->inst[x]->vibType = ctlDecompressed[offsetInstrument+8];
			alBank->inst[x]->vibRate = ctlDecompressed[offsetInstrument+9];
			alBank->inst[x]->vibDepth = ctlDecompressed[offsetInstrument+0xA];
			alBank->inst[x]->vibDelay = ctlDecompressed[offsetInstrument+0xB];
			alBank->inst[x]->bendRange = CharArrayToShort(&ctlDecompressed[offsetInstrument+0xC]);
			alBank->inst[x]->soundCount = CharArrayToShort(&ctlDecompressed[offsetInstrument+0xE]);
		}
		else
		{
			offsetInstrument = ctlOffsetPartTwo + ((offsetInstrument >> 8) << 3);
			alBank->inst[x]->volume = ctl[offsetInstrument];
			alBank->inst[x]->pan = ctl[offsetInstrument+1];
			alBank->inst[x]->priority = ctl[offsetInstrument+2];
			alBank->inst[x]->flags = ctl[offsetInstrument+3];
			alBank->inst[x]->tremType = ctl[offsetInstrument+4];
			alBank->inst[x]->tremRate = ctl[offsetInstrument+5];
			alBank->inst[x]->tremDepth = ctl[offsetInstrument+6];
			alBank->inst[x]->tremDelay = ctl[offsetInstrument+7];
			alBank->inst[x]->vibType = ctl[offsetInstrument+8];
			alBank->inst[x]->vibRate = ctl[offsetInstrument+9];
			alBank->inst[x]->vibDepth = ctl[offsetInstrument+0xA];
			alBank->inst[x]->vibDelay = ctl[offsetInstrument+0xB];
			alBank->inst[x]->bendRange = CharArrayToShort(&ctl[offsetInstrument+0xC]);
			alBank->inst[x]->soundCount = CharArrayToShort(&ctl[offsetInstrument+0xE]);
		}
		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;
		}

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			unsigned long offsetSound;
			unsigned long baseOffsetSound;
			if (!isSoundType)
			{
				offsetSound = ctlOffsetPartTwo + (((CharArrayToLong(&ctlDecompressed[offsetInstrument+0x10+(y*4)])) >> 8) << 3);
				baseOffsetSound = offsetSound;
				alBank->inst[x]->sounds[y]->unknown1 = ctlDecompressed[offsetInstrument+0x10+(y*4) + 3];
			}
			else
			{
				offsetSound = offsetInstrument + (CharArrayToLong(&ctl[offsetInstrument+0x10+(y*4)]));
				baseOffsetSound = offsetInstrument;
				alBank->inst[x]->sounds[y]->unknown1 = ctlDecompressed[0x14 + (x * 4) + 3];
			}

			unsigned long offsetEnv = baseOffsetSound + CharArrayToLong(&ctl[offsetSound]);
			unsigned long offsetKey = baseOffsetSound + CharArrayToLong(&ctl[offsetSound+4]);
			unsigned long offsetWav = baseOffsetSound + CharArrayToLong(&ctl[offsetSound+8]);

			alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound + 0xC];
			alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound + 0xD];
			alBank->inst[x]->sounds[y]->flags = ctl[offsetSound + 0xE];
			
			alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
			alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
			alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
			alBank->inst[x]->sounds[y]->env.attackVolume = ctl[offsetEnv + 0xC];
			alBank->inst[x]->sounds[y]->env.decayVolume = ctl[offsetEnv + 0xD];
			alBank->inst[x]->sounds[y]->env.zeroPad = CharArrayToShort(&ctl[offsetEnv + 0xE]);

			alBank->inst[x]->sounds[y]->key.velocitymin = ctl[offsetKey+0x0];
			alBank->inst[x]->sounds[y]->key.velocitymax = ctl[offsetKey+0x1];
			alBank->inst[x]->sounds[y]->key.keymin = ctl[offsetKey+0x2];
			alBank->inst[x]->sounds[y]->key.keymax = ctl[offsetKey+0x3];
			alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetKey+0x4];
			alBank->inst[x]->sounds[y]->key.detune = ctl[offsetKey+0x5];

			alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWav + 0x0]);
			alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWav + 0x4]);
			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
			
			// MUST PAD to 4s

			alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

			unsigned long loopOffset = CharArrayToLong(&ctl[offsetWav + 0xC]);

			if (loopOffset != 0)
			{
				loopOffset = baseOffsetSound + loopOffset;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
				for (int z = 0; z < 0x10; z++)
				{
					alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
				}
			}
			else
			{
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
			}

			unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWav + 0x10]);

			if (predictorOffset != 0)
			{
				predictorOffset = baseOffsetSound + predictorOffset;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);;
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

				for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
				{
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 8 + z * 2]);
				}
			}

			unsigned long extraOffset = CharArrayToLong(&ctl[offsetWav + 0x14]);
			if (extraOffset != 0)
			{
				extraOffset = baseOffsetSound + extraOffset;

				while (CharArrayToShort(&ctl[extraOffset]) != 0xFFFF)
				{
					alBank->inst[x]->sounds[y]->conkerShorts.push_back(CharArrayToShort(&ctl[extraOffset]));
					extraOffset += 2;
				}
			}
			else
			{
				alBank->inst[x]->sounds[y]->conkerShorts.clear();
			}
		}
	}

	delete [] ctlDecompressed;

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioBanjoTooie(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber)
{
	unsigned long bankOffset = ctlOffset;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = BANJOTOOIEAUDIO;
	alBank->count = CharArrayToShort(&ctl[bankOffset]);
	alBank->flags = CharArrayToShort(&ctl[bankOffset+2]);
	alBank->pad = CharArrayToShort(&ctl[bankOffset+4]);
	alBank->samplerate = CharArrayToShort(&ctl[bankOffset+6]);

	unsigned long soundDataOffset = bankOffset + 8 + (alBank->count * 0x12);
	if ((soundDataOffset % 4) != 0)
	{
		soundDataOffset = soundDataOffset + (4 - (soundDataOffset % 4));
	}

	alBank->inst = NULL;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = (bankOffset + 0x8 + x*0x12);

			alBank->inst[x]->volume = ctl[offsetInstrument];
			alBank->inst[x]->pan = ctl[offsetInstrument+1];
			alBank->inst[x]->priority = ctl[offsetInstrument+2];
			alBank->inst[x]->flags = ctl[offsetInstrument+3];
			alBank->inst[x]->tremType = ctl[offsetInstrument+4];
			alBank->inst[x]->tremRate = ctl[offsetInstrument+5];
			alBank->inst[x]->tremDepth = ctl[offsetInstrument+6];
			alBank->inst[x]->tremDelay = ctl[offsetInstrument+7];
			alBank->inst[x]->vibType = ctl[offsetInstrument+8];
			alBank->inst[x]->vibRate = ctl[offsetInstrument+9];
			alBank->inst[x]->vibDepth = ctl[offsetInstrument+0xA];
			alBank->inst[x]->vibDelay = ctl[offsetInstrument+0xB];
			alBank->inst[x]->bendRange = CharArrayToShort(&ctl[offsetInstrument+0xC]);
			alBank->inst[x]->soundCount = CharArrayToShort(&ctl[offsetInstrument+0xE]);

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSound = soundDataOffset + (CharArrayToShort(&ctl[offsetInstrument + 0x10]) * 0xD8) + (0xD8 * y);
					
					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetSound + 0x0]);
						alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetSound + 0x4]);
						alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetSound + 0x8]);
						alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetSound + 0xC]);
						alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetSound + 0xE]);

						alBank->inst[x]->sounds[y]->key.velocitymin = ctl[offsetSound+0x10];
						alBank->inst[x]->sounds[y]->key.velocitymax = ctl[offsetSound+0x11];
						alBank->inst[x]->sounds[y]->key.keymin = ctl[offsetSound+0x12];
						alBank->inst[x]->sounds[y]->key.keymax = ctl[offsetSound+0x13];
						alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetSound+0x14];
						alBank->inst[x]->sounds[y]->key.detune = ctl[offsetSound+0x15];

						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetSound + 0x18]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetSound + 0x1C]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						
						// MUST PAD to 4s

						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = offsetSound + 0x20;
						if ((CharArrayToLong(&ctl[loopOffset]) == 0x00000000) && (CharArrayToLong(&ctl[loopOffset+4]) == 0x00000000) && (CharArrayToLong(&ctl[loopOffset+8]) == 0x00000000))
						{
							loopOffset = 0;
						}

						unsigned long predictorOffset = offsetSound + 0x50;

						if (loopOffset != 0)
						{						
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 0x2;
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 0x4;
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + z * 2]);
							}
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioZLibMusyxSmall(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(ROGUESQUADRON);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);
	if (outputDecompressed != NULL)
	{
		ALBank* alBank = ReadAudioMusyxSmallerTable(&outputDecompressed[0], fileSizeUncompressed, 0, tbl, numberInstruments, lastSoundEnd);
		alBank->soundBankFormat = MUSYXSMALLZLIB;
		delete [] outputDecompressed;
		return alBank;
	}
	else
	{
		return NULL;
	}
}

ALBank* CN64AIFCAudio::ReadAudioZLibMusyx78DA(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(ROGUESQUADRON);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);
	if (outputDecompressed != NULL)
	{
		ALBank* alBank = ReadAudioMusyx(&outputDecompressed[0], fileSizeUncompressed, 0, tbl, numberInstruments, lastSoundEnd);
		alBank->soundBankFormat = MUSYXZLIB;
		delete [] outputDecompressed;
		return alBank;
	}
	else
	{
		return NULL;
	}
}

ALBank* CN64AIFCAudio::ReadAudioZLibMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd, int game)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(game);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);
	if (outputDecompressed != NULL)
	{
		ALBank* alBank = ReadAudioMusyx(&outputDecompressed[0], fileSizeUncompressed, 0, tbl, numberInstruments, lastSoundEnd);
		alBank->soundBankFormat = MUSYXZLIB;
		delete [] outputDecompressed;
		return alBank;
	}
	else
	{
		return NULL;
	}
}

ALBank* CN64AIFCAudio::ReadAudioBlastCorps(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = ctlSize;
	BlastCorpsLZBDecoder blastCorpsLzb;
	int decompressedSize = blastCorpsLzb.dec(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudio(ROM, &outputDecompressed[0], decompressedSize, 0, tbl, 0x0, 0x0, 0);
	alBank->soundBankFormat = BLASTCORPSZLBSTANDARD;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioLzMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd)
{
	unsigned char* outputDecompressed = new unsigned char[0x2000000];
	int fileSizeCompressed = ctlSize;
	MidwayLZ midwayLz;
	int decompressedSize = midwayLz.dec(&ctl[ctlOffset], fileSizeCompressed, outputDecompressed);
	ALBank* alBank = ReadAudioMusyx(&outputDecompressed[0], decompressedSize, 0, tbl, numberInstruments, lastSoundEnd);
	alBank->soundBankFormat = MUSYXLZ;
	delete [] outputDecompressed;
	return alBank;
}

// Full credit to Musyx goes to Bobby Smiles (from Mupen emulator)

ALBank* CN64AIFCAudio::ReadAudioMusyxSmallerTable(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MUSYX;
	alBank->count = numberInstruments;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 0x2B11;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		unsigned long offsetInstrument = ctlOffset + x*0x18;
	
		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;
		}

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
			alBank->inst[x]->sounds[y]->wav.type = AL_MUSYX_WAVE;

			alBank->inst[x]->samplerate = CharArrayToShort(&ctl[offsetInstrument+0xA]);
			alBank->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec = CharArrayToShort(&ctl[offsetInstrument+0xE]);

			alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetInstrument + 0x8];

			unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument+4]);

			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[offsetInstrument+0x10]);
			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start + CharArrayToLong(&ctl[offsetInstrument+0x14]);

			if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end)
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = 0xFFFFFFFF;
			else
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 8;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
			{
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&tbl[predictorOffset + z * 2]);
			}

			alBank->inst[x]->sounds[y]->wav.base = predictorOffset + (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 0x10);
			if (x == (alBank->count-1))
			{
				alBank->inst[x]->sounds[y]->wav.len = lastSoundEnd - CharArrayToLong(&ctl[offsetInstrument+0x4]);
			}
			else
			{
				alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument+0x18 + 0x4]) - CharArrayToLong(&ctl[offsetInstrument+0x4]);
			}

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MUSYX;
	alBank->count = numberInstruments;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 0x2B11;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;

	alBank->inst = new ALInst*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x] = new ALInst();
		alBank->inst[x]->samplerate = 0;
		alBank->inst[x]->sounds = NULL;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		unsigned long offsetInstrument = ctlOffset + x*0x1C;
	
		alBank->inst[x]->soundCount = 1;

		alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y] = new ALSound();
			alBank->inst[x]->sounds[y]->wav.wavData = NULL;
		}

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
			alBank->inst[x]->sounds[y]->wav.type = AL_MUSYX_WAVE;

			alBank->inst[x]->samplerate = CharArrayToShort(&ctl[offsetInstrument+0xE]);
			alBank->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec = CharArrayToLong(&ctl[offsetInstrument+0x10]) & 0xFFFFFF;

			alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetInstrument + 0xC];

			unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument+4]);

			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[offsetInstrument+0x14]);
			alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start + CharArrayToLong(&ctl[offsetInstrument+0x18]);
			if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end)
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = 0xFFFFFFFF;
			else
				alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = 0;

			alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = 2;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = 8;
			alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
			{
				alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&tbl[predictorOffset + z * 2]);
			}

			alBank->inst[x]->sounds[y]->wav.base = predictorOffset + (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 0x10);
			if (x == (alBank->count-1))
			{
				alBank->inst[x]->sounds[y]->wav.len = lastSoundEnd - CharArrayToLong(&ctl[offsetInstrument+0x4]);
			}
			else
			{
				alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument+0x1C + 0x4]) - CharArrayToLong(&ctl[offsetInstrument+0x4]);
			}

			alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

			for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
			{
				alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioKonami(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset)
{
	ALBank* alBank = ReadAudio(ROM, &ROM[0], ctlSize, ctlOffset, tbl, 0x0, 0xFFFFFFFF, 0);
	alBank->soundBankFormat = KONAMICTL;

	if (endInstrumentOffset != 0xFFFFFFFF)
	{
		alBank->konamiADSR.clear();
		for (int x = 0; x < (endInstrumentOffset - instrumentOffset) / 7; x++)
		{
			KonamiADSR adsr;
			adsr.coarseTune = ROM[instrumentOffset + (x * 7)];
			adsr.fineTune = ROM[instrumentOffset + (x * 7) + 1];
			adsr.attackTime = ROM[instrumentOffset + (x * 7) + 2];
			adsr.decayTime = ROM[instrumentOffset + (x * 7) + 3];
			adsr.releaseTime = ROM[instrumentOffset + (x * 7) + 4];
			adsr.unknownOne = ROM[instrumentOffset + (x * 7) + 5];
			adsr.unknownTwo = ROM[instrumentOffset + (x * 7) + 6];

			alBank->konamiADSR.push_back(adsr);
		}

		alBank->konamiDrumsADSR.clear();
		if (endDrumOffset != 0x00000000)
		{
			for (int x = 0; x < (endDrumOffset - startDrumOffset) / 8; x++)
			{
				KonamiADSRDrum adsr;
				adsr.instrumentLookup = ROM[startDrumOffset + (x * 8)];
				adsr.coarseTune = ROM[startDrumOffset + (x * 8) + 1];
				adsr.fineTune = ROM[startDrumOffset + (x * 8) + 2];
				adsr.attackTime = ROM[startDrumOffset + (x * 8) + 3];
				adsr.decayTime = ROM[startDrumOffset + (x * 8) + 4];
				adsr.releaseTime = ROM[startDrumOffset + (x * 8) + 5];
				adsr.unknownOne = ROM[startDrumOffset + (x * 8) + 6];
				adsr.unknownTwo = ROM[startDrumOffset + (x * 8) + 7];

				alBank->konamiDrumsADSR.push_back(adsr);
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioKonami8(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset)
{
	ALBank* alBank = ReadAudio(ROM, &ROM[0], ctlSize, ctlOffset, tbl, 0x0, 0xFFFFFFFF, 0);
	alBank->soundBankFormat = KONAMI8CTL;

	if (endInstrumentOffset != 0xFFFFFFFF)
	{
		alBank->konamiADSR.clear();
		for (int x = 0; x < (endInstrumentOffset - instrumentOffset) / 8; x++)
		{
			KonamiADSR adsr;
			adsr.coarseTune = ROM[instrumentOffset + (x * 8)];
			adsr.fineTune = ROM[instrumentOffset + (x * 8) + 1];
			adsr.attackTime = ROM[instrumentOffset + (x * 8) + 2];
			adsr.decayTime = ROM[instrumentOffset + (x * 8) + 3];
			adsr.releaseTime = ROM[instrumentOffset + (x * 8) + 4];
			adsr.unknownOne = ROM[instrumentOffset + (x * 8) + 5];
			adsr.unknownTwo = ROM[instrumentOffset + (x * 8) + 6];

			alBank->konamiADSR.push_back(adsr);
		}

		alBank->konamiDrumsADSR.clear();
		if (endDrumOffset != 0x00000000)
		{
			for (int x = 0; x < (endDrumOffset - startDrumOffset) / 0xA; x++)
			{
				KonamiADSRDrum adsr;
				adsr.coarseTune = ROM[startDrumOffset + (x * 0xA)];
				adsr.fineTune = ROM[startDrumOffset + (x * 0xA) + 1];
				adsr.attackTime = ROM[startDrumOffset + (x * 0xA) + 2];
				adsr.decayTime = ROM[startDrumOffset + (x * 0xA) + 3];
				adsr.releaseTime = ROM[startDrumOffset + (x * 0xA) + 4];
				adsr.unknownOne = ROM[startDrumOffset + (x * 0xA) + 5];
				adsr.unknownTwo = ROM[startDrumOffset + (x * 0xA) + 6];
				adsr.instrumentLookup = ROM[startDrumOffset + (x * 0xA) + 9];

				alBank->konamiDrumsADSR.push_back(adsr);
			}
		}
	}

	return alBank;
}

T1Bank* CN64AIFCAudio::ReadT1Bank(unsigned char* ROM, unsigned long t1Start, unsigned long t1End)
{
	T1Bank* t1Bank = NULL;;

	if ((t1Start != 0x00000000) && (t1End != 0x00000000))
	{
		t1Bank = new T1Bank();

		unsigned short numberT1 = CharArrayToShort(&ROM[t1Start + 2]);

		for (int x = 0; x < numberT1; x++)
		{
			T1BankEntry t1BankEntry;
			t1BankEntry.bankNumber = ROM[t1Start + 4 + (x * 8)];
			t1BankEntry.instrumentNumber = ROM[t1Start + 4 + (x * 8) + 1];
			t1BankEntry.soundNumber = ROM[t1Start + 4 + (x * 8) + 2];
			t1BankEntry.flags = ROM[t1Start + 4 + (x * 8) + 3];
			t1BankEntry.frequency = CharArrayToLong(&ROM[t1Start + 4 + (x * 8) + 4]);

			t1Bank->t1Entries.push_back(t1BankEntry);
		}
	}

	return t1Bank;
}

bool CN64AIFCAudio::WriteT1Bank(T1Bank* t1Bank, unsigned char* ROM, unsigned long t1Start, unsigned long t1End)
{
	if ((t1Start != 0x00000000) && (t1End != 0x00000000) && (t1Bank != NULL))
	{
		if ((t1Bank->t1Entries.size() * 8) > (t1End - t1Start - 4))
		{
			return false;
		}

		for (int x = 0; x < t1Bank->t1Entries.size(); x++)
		{
			ROM[t1Start + 4 + (x * 8)] = t1Bank->t1Entries[x].bankNumber;
			ROM[t1Start + 4 + (x * 8) + 1] = t1Bank->t1Entries[x].instrumentNumber;
			ROM[t1Start + 4 + (x * 8) + 2] = t1Bank->t1Entries[x].soundNumber;
			ROM[t1Start + 4 + (x * 8) + 3] = t1Bank->t1Entries[x].flags;
			WriteLongToBuffer(ROM, t1Start + 4 + (x * 8) + 4, t1Bank->t1Entries[x].frequency);
		}

		return true;
	}

	return false;
}

ALBank* CN64AIFCAudio::ReadAudio(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber)
{
	unsigned short numberBands = CharArrayToShort(&ctl[ctlOffset+0x2]);
	unsigned long bankOffset = CharArrayToLong(&ctl[ctlOffset+ 0x4 + (bankNumber * 0x4)]);
	if (((bankOffset >> 16) & 0xF0) > 0)
		bankOffset = ctlFlaggedOffset + (bankOffset - mask);
	else
		bankOffset = ctlOffset + bankOffset;

	ALBank* alBank = new ALBank();
	alBank->subBank = bankNumber;
	alBank->soundBankFormat = STANDARDFORMAT;
	alBank->count = CharArrayToShort(&ctl[bankOffset]);
	alBank->flags = CharArrayToShort(&ctl[bankOffset+2]);
	alBank->pad = CharArrayToShort(&ctl[bankOffset+4]);
	alBank->samplerate = CharArrayToShort(&ctl[bankOffset+6]);
	unsigned long offsetPercussion = CharArrayToLong(&ctl[bankOffset+8]);
	if (offsetPercussion != NULL)
	{
		alBank->percussion = new ALInst();
		alBank->percussion->samplerate = 0;
		alBank->percussion->sounds = NULL;

		if (alBank->flags == 0x0100)
			offsetPercussion = ctlFlaggedOffset + (offsetPercussion - mask);
		else
			offsetPercussion = ctlOffset + offsetPercussion;

		alBank->percussion->volume = ctl[offsetPercussion];
		alBank->percussion->pan = ctl[offsetPercussion+1];
		alBank->percussion->priority = ctl[offsetPercussion+2];
		alBank->percussion->flags = ctl[offsetPercussion+3];
		alBank->percussion->tremType = ctl[offsetPercussion+4];
		alBank->percussion->tremRate = ctl[offsetPercussion+5];
		alBank->percussion->tremDepth = ctl[offsetPercussion+6];
		alBank->percussion->tremDelay = ctl[offsetPercussion+7];
		alBank->percussion->vibType = ctl[offsetPercussion+8];
		alBank->percussion->vibRate = ctl[offsetPercussion+9];
		alBank->percussion->vibDepth = ctl[offsetPercussion+0xA];
		alBank->percussion->vibDelay = ctl[offsetPercussion+0xB];
		alBank->percussion->bendRange = CharArrayToShort(&ctl[offsetPercussion+0xC]);
		alBank->percussion->soundCount = CharArrayToShort(&ctl[offsetPercussion+0xE]);

		if ((alBank->percussion->flags == 0x0) || (alBank->percussion->flags == 0x1))
		{	
			alBank->percussion->sounds = new ALSound*[alBank->percussion->soundCount];

			for (int y = 0; y < alBank->percussion->soundCount; y++)
			{
				alBank->percussion->sounds[y] = new ALSound();
				alBank->percussion->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->percussion->soundCount; y++)
			{
				unsigned long offsetSound = CharArrayToLong(&ctl[offsetPercussion+ 0x10 + y*0x4]);
				if (alBank->percussion->flags == 0x1)
					offsetSound = ctlFlaggedOffset + (offsetSound - mask);
				else
					offsetSound = ctlOffset + offsetSound;

				alBank->percussion->sounds[y]->samplePan = ctl[offsetSound+0xC];
				alBank->percussion->sounds[y]->sampleVolume = ctl[offsetSound+0xD];
				alBank->percussion->sounds[y]->flags = ctl[offsetSound+0xE];

				
				if ((alBank->percussion->sounds[y]->flags == 0x0) || (alBank->percussion->sounds[y]->flags == 0x1))
				{
					unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound+ 0x0]);
					if (alBank->percussion->sounds[y]->flags == 0x1)
						offsetEnv = ctlFlaggedOffset + (offsetEnv - mask);
					else
						offsetEnv = ctlOffset + offsetEnv;

					unsigned long offsetKeyMap = CharArrayToLong(&ctl[offsetSound+ 0x4]);
					if (alBank->percussion->sounds[y]->flags == 0x1)
						offsetKeyMap = ctlFlaggedOffset + (offsetKeyMap - mask);
					else
						offsetKeyMap = ctlOffset + offsetKeyMap;

					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x8]);
					if (alBank->percussion->sounds[y]->flags == 0x1)
						offsetWaveTable = ctlFlaggedOffset + (offsetWaveTable - mask);
					else
						offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->percussion->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
					alBank->percussion->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
					alBank->percussion->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
					alBank->percussion->sounds[y]->env.attackVolume = ctl[offsetEnv + 0xC];
					alBank->percussion->sounds[y]->env.decayVolume = ctl[offsetEnv + 0xD];
					alBank->percussion->sounds[y]->env.zeroPad = CharArrayToShort(&ctl[offsetEnv + 0xE]);

					alBank->percussion->sounds[y]->key.velocitymin = ctl[offsetKeyMap];
					alBank->percussion->sounds[y]->key.velocitymax = ctl[offsetKeyMap+0x1];
					alBank->percussion->sounds[y]->key.keymin = ctl[offsetKeyMap+0x2];
					alBank->percussion->sounds[y]->key.keymax = ctl[offsetKeyMap+0x3];
					alBank->percussion->sounds[y]->key.keybase = ctl[offsetKeyMap+0x4];
					alBank->percussion->sounds[y]->key.detune = ctl[offsetKeyMap+0x5];

					unsigned char flag = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) >> 24; // hack for NUD-DSCJ-JPN, can't imagine impact others
					if (flag == 0x40) // Means using 64DD IPL Bank
						alBank->percussion->sounds[y]->wav.base = (CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF) + 0x140000;
					else
						alBank->percussion->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
					alBank->percussion->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->percussion->sounds[y]->wav.wavData = new unsigned char[alBank->percussion->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->percussion->sounds[y]->wav.len; r++)
					{
						if (flag == 0x40)
						{
							alBank->percussion->sounds[y]->wav.wavData[r] = ROM[alBank->percussion->sounds[y]->wav.base + r];
						}
						else
						{
							alBank->percussion->sounds[y]->wav.wavData[r] = tbl[alBank->percussion->sounds[y]->wav.base + r];
						}
					}
					

					alBank->percussion->sounds[y]->wav.type = ctl[offsetWaveTable + 0x8];
					alBank->percussion->sounds[y]->wav.flags = ctl[offsetWaveTable + 0x9];
					// MUST PAD to 4s

					if ((alBank->percussion->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->percussion->sounds[y]->flags == 0x0) || (alBank->percussion->sounds[y]->flags == 0x1)))
					{
						alBank->percussion->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);

						if (loopOffset != 0)
						{
							if (alBank->percussion->sounds[y]->flags == 0x1)
								loopOffset = ctlFlaggedOffset + (loopOffset - mask);
							else
								loopOffset = ctlOffset + loopOffset;

							alBank->percussion->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->percussion->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->percussion->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->percussion->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							for (int z = 0; z < 0x10; z++)
							{
								alBank->percussion->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
							}
						}
						else
						{
							alBank->percussion->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							if (alBank->percussion->sounds[y]->flags == 0x1)
								predictorOffset = ctlFlaggedOffset + (predictorOffset - mask);
							else
								predictorOffset = ctlOffset + predictorOffset;

							alBank->percussion->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->percussion->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->percussion->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->percussion->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->percussion->sounds[y]->wav.adpcmWave->book->order * alBank->percussion->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->percussion->sounds[y]->wav.adpcmWave->book->order * alBank->percussion->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->percussion->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
					}
					else if ((alBank->percussion->sounds[y]->flags == 0x0) || (alBank->percussion->sounds[y]->flags == 0x1))
					{
						alBank->percussion->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							if (alBank->percussion->sounds[y]->flags == 0x1)
								loopOffset = ctlFlaggedOffset + (loopOffset - mask);
							else
								loopOffset = ctlOffset + loopOffset;

							alBank->percussion->sounds[y]->wav.rawWave->loop = new ALRawLoop();
							alBank->percussion->sounds[y]->wav.rawWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->percussion->sounds[y]->wav.rawWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->percussion->sounds[y]->wav.rawWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);	
						}
						else
						{
							alBank->percussion->sounds[y]->wav.rawWave->loop = NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
						return NULL;
					}
				}
				else
				{
					DisposeALBank(alBank);
					MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
					return NULL;
				}
			}
		}
		else
		{
			DisposeALBank(alBank);
			MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
			return NULL;
		}
	}
	
	else
	{
		alBank->percussion = NULL;
	}

	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		if (alBank->count > 0)
		{
			alBank->inst = new ALInst*[alBank->count];

			for (int x = 0; x < alBank->count; x++)
			{
				alBank->inst[x] = new ALInst();
				alBank->inst[x]->samplerate = 0;
				alBank->inst[x]->sounds = NULL;
			}

			for (int x = 0; x < alBank->count; x++)
			{
				unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + 0xC + x * 4]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->volume = 0;
					alBank->inst[x]->pan = 0;
					alBank->inst[x]->priority = 0;
					alBank->inst[x]->flags = 0;
					alBank->inst[x]->tremType = 0;
					alBank->inst[x]->tremRate = 0;
					alBank->inst[x]->tremDepth = 0;
					alBank->inst[x]->tremDelay = 0;
					alBank->inst[x]->vibType = 0;
					alBank->inst[x]->vibRate = 0;
					alBank->inst[x]->vibDepth = 0;
					alBank->inst[x]->vibDelay = 0;
					alBank->inst[x]->bendRange = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}

				if (alBank->flags == 0x0100)
					offsetInstrument = ctlFlaggedOffset + (offsetInstrument - mask);
				else
					offsetInstrument = ctlOffset + offsetInstrument;

				alBank->inst[x]->volume = ctl[offsetInstrument];
				alBank->inst[x]->pan = ctl[offsetInstrument + 1];
				alBank->inst[x]->priority = ctl[offsetInstrument + 2];
				alBank->inst[x]->flags = ctl[offsetInstrument + 3];
				alBank->inst[x]->tremType = ctl[offsetInstrument + 4];
				alBank->inst[x]->tremRate = ctl[offsetInstrument + 5];
				alBank->inst[x]->tremDepth = ctl[offsetInstrument + 6];
				alBank->inst[x]->tremDelay = ctl[offsetInstrument + 7];
				alBank->inst[x]->vibType = ctl[offsetInstrument + 8];
				alBank->inst[x]->vibRate = ctl[offsetInstrument + 9];
				alBank->inst[x]->vibDepth = ctl[offsetInstrument + 0xA];
				alBank->inst[x]->vibDelay = ctl[offsetInstrument + 0xB];
				alBank->inst[x]->bendRange = CharArrayToShort(&ctl[offsetInstrument + 0xC]);
				alBank->inst[x]->soundCount = CharArrayToShort(&ctl[offsetInstrument + 0xE]);

				if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
				{
					alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

					for (int y = 0; y < alBank->inst[x]->soundCount; y++)
					{
						alBank->inst[x]->sounds[y] = new ALSound();
						alBank->inst[x]->sounds[y]->wav.wavData = NULL;
					}

					for (int y = 0; y < alBank->inst[x]->soundCount; y++)
					{
						unsigned long offsetSound = CharArrayToLong(&ctl[offsetInstrument + 0x10 + y * 0x4]);
						if (alBank->inst[x]->flags == 0x1)
							offsetSound = ctlFlaggedOffset + (offsetSound - mask);
						else
							offsetSound = ctlOffset + offsetSound;

						alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound + 0xC];
						alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound + 0xD];
						alBank->inst[x]->sounds[y]->flags = ctl[offsetSound + 0xE];


						if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
						{
							unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound + 0x0]);
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								offsetEnv = ctlFlaggedOffset + (offsetEnv - mask);
							else
								offsetEnv = ctlOffset + offsetEnv;

							unsigned long offsetKeyMap = CharArrayToLong(&ctl[offsetSound + 0x4]);
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								offsetKeyMap = ctlFlaggedOffset + (offsetKeyMap - mask);
							else
								offsetKeyMap = ctlOffset + offsetKeyMap;

							unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound + 0x8]);
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								offsetWaveTable = ctlFlaggedOffset + (offsetWaveTable - mask);
							else
								offsetWaveTable = ctlOffset + offsetWaveTable;

							alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
							alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
							alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
							alBank->inst[x]->sounds[y]->env.attackVolume = ctl[offsetEnv + 0xC];
							alBank->inst[x]->sounds[y]->env.decayVolume = ctl[offsetEnv + 0xD];
							alBank->inst[x]->sounds[y]->env.zeroPad = CharArrayToShort(&ctl[offsetEnv + 0xE]);

							alBank->inst[x]->sounds[y]->key.velocitymin = ctl[offsetKeyMap];
							alBank->inst[x]->sounds[y]->key.velocitymax = ctl[offsetKeyMap + 0x1];
							alBank->inst[x]->sounds[y]->key.keymin = ctl[offsetKeyMap + 0x2];
							alBank->inst[x]->sounds[y]->key.keymax = ctl[offsetKeyMap + 0x3];
							alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetKeyMap + 0x4];
							alBank->inst[x]->sounds[y]->key.detune = ctl[offsetKeyMap + 0x5];

							unsigned char flag = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) >> 24; // hack for NUD-DSCJ-JPN, can't imagine impact others
							if (flag == 0x40) // Means using 64DD IPL Bank
								alBank->inst[x]->sounds[y]->wav.base = (CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF) + 0x140000;
							else
								alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
							alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
							alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

							for (int r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
							{
								if (flag == 0x40)
								{
									alBank->inst[x]->sounds[y]->wav.wavData[r] = ROM[alBank->inst[x]->sounds[y]->wav.base + r];
								}
								else
								{
									alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
								}
							}


							alBank->inst[x]->sounds[y]->wav.type = ctl[offsetWaveTable + 0x8];
							alBank->inst[x]->sounds[y]->wav.flags = ctl[offsetWaveTable + 0x9];
							// MUST PAD to 4s

							if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
								unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
								unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);

								if (loopOffset != 0)
								{
									if (alBank->inst[x]->sounds[y]->flags == 0x1)
										loopOffset = ctlFlaggedOffset + (loopOffset - mask);
									else
										loopOffset = ctlOffset + loopOffset;

									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
									}
								}
								else
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
								}

								if (predictorOffset != 0)
								{
									if (alBank->inst[x]->sounds[y]->flags == 0x1)
										predictorOffset = ctlFlaggedOffset + (predictorOffset - mask);
									else
										predictorOffset = ctlOffset + predictorOffset;

									alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

									for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
									{
										alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
									}

									/*if (
										(alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order == 1)
										&&
										(alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors == 1)
										)*/
									// WWF Warzone had  DEAN with 2/4
									{
										// DEAN Acclaim
										if (
											(alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[0] == 0x4445)
											&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[1] == 0x414E)
											)
										{
											alBank->inst[x]->sounds[y]->wav.type = AL_ACCLAIM_DEAN;
										}
										else if (
											(alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[0] == 0x524B)
											&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[1] == 0x4446)
											)
										{
											MessageBox(NULL, "Figure out Acclaim RKDF!!", "Warning", NULL);
										}
									}
								}
							}
							else if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
							{
								alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
								unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

								if (loopOffset != 0)
								{
									if (alBank->inst[x]->sounds[y]->flags == 0x1)
										loopOffset = ctlFlaggedOffset + (loopOffset - mask);
									else
										loopOffset = ctlOffset + loopOffset;

									alBank->inst[x]->sounds[y]->wav.rawWave->loop = new ALRawLoop();
									alBank->inst[x]->sounds[y]->wav.rawWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
									alBank->inst[x]->sounds[y]->wav.rawWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
									alBank->inst[x]->sounds[y]->wav.rawWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								}
								else
								{
									alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
								}
							}
							else
							{
								DisposeALBank(alBank);
								MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
								return NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
							return NULL;
						}
					}
				}
				else
				{
					DisposeALBank(alBank);
					MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
					return NULL;
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMarioParty2E(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask)
{
	unsigned short numberBands = CharArrayToShort(&ctl[ctlOffset+0x2]);
	unsigned long bankOffset = ctlOffset;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = MARIOPARTY2E;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 22050;
	alBank->percussion = 0;
	alBank->eadPercussion = NULL;
	alBank->countEADPercussion = 0;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		int x = 0;
		{
			alBank->inst[x]->soundCount = CharArrayToShort(&ctl[bankOffset-2]) / 0x10;
			if (alBank->inst[x]->soundCount == 0x0000)
			{
				// Mario Party
				alBank->inst[x]->soundCount = CharArrayToLong(&ctl[bankOffset]) / 0x10;
			}
			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				unsigned long offsetSound = ctlOffset + (0x10 * y);

				alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound+0xC];
				alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound+0xD];
				alBank->inst[x]->sounds[y]->flags = ctl[offsetSound+0xE];

				
				if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
				{
					unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound+ 0x0]);
					if (alBank->inst[x]->sounds[y]->flags == 0x1)
						offsetEnv = ctlFlaggedOffset + (offsetEnv - mask);
					else
						offsetEnv = ctlOffset + offsetEnv;

					alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetSound+ 0x4]);

					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x8]);
					if (alBank->inst[x]->sounds[y]->flags == 0x1)
						offsetWaveTable = ctlFlaggedOffset + (offsetWaveTable - mask);
					else
						offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
					alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
					alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
					alBank->inst[x]->sounds[y]->env.attackVolume = ctl[offsetEnv + 0xC];
					alBank->inst[x]->sounds[y]->env.decayVolume = ctl[offsetEnv + 0xD];
					alBank->inst[x]->sounds[y]->env.zeroPad = CharArrayToShort(&ctl[offsetEnv + 0xE]);

					unsigned char flag = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) >> 24; // hack for NUD-DSCJ-JPN, can't imagine impact others
					if (flag == 0x40) // Means using 64DD IPL Bank
						alBank->inst[x]->sounds[y]->wav.base = (CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF) + 0x140000;
					else
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						if (flag == 0x40)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = ROM[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
					}
					

					alBank->inst[x]->sounds[y]->wav.type = ctl[offsetWaveTable + 0x8];
					alBank->inst[x]->sounds[y]->wav.flags = ctl[offsetWaveTable + 0x9];
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);

						if (loopOffset != 0)
						{
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								loopOffset = ctlFlaggedOffset + (loopOffset - mask);
							else
								loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								predictorOffset = ctlFlaggedOffset + (predictorOffset - mask);
							else
								predictorOffset = ctlOffset + predictorOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
					}
					else if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							if (alBank->inst[x]->sounds[y]->flags == 0x1)
								loopOffset = ctlFlaggedOffset + (loopOffset - mask);
							else
								loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.rawWave->loop = new ALRawLoop();
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.rawWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);	
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
						return NULL;
					}
				}
				else
				{
					DisposeALBank(alBank);
					MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
					return NULL;
				}
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

int CN64AIFCAudio::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}


unsigned short CN64AIFCAudio::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64AIFCAudio::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64AIFCAudio::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

float CN64AIFCAudio::CharArrayToFloat(unsigned char* currentSpot)
{
	unsigned long tempLong = (Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot)));
	return (*reinterpret_cast<float*> (&tempLong));
}

unsigned long CN64AIFCAudio::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

void CN64AIFCAudio::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CN64AIFCAudio::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

int CN64AIFCAudio::ReadCtlTblLocations(unsigned char* ROM, int romSize, std::vector<ctlTblResult>& results)
{
	int numResults = 0;
	for (int x = 0x0; x < (romSize - 4); x+=4)
	{
		unsigned long value = CharArrayToLong(&ROM[x]);

 		if (value == 0x42310001)
		{
			results.resize(numResults+1);
			// possible ctl
			unsigned long bankOffset = CharArrayToLong(&ROM[x+4]);
			if ((bankOffset >> 24) != 0x80) // not pointer
				bankOffset = (x + bankOffset);
			else // absolute pointer, mask
				bankOffset = (bankOffset & 0xFFFFFF);

			if (bankOffset > romSize)
				continue;

			unsigned short numberInstruments = CharArrayToShort(&ROM[bankOffset]);

			unsigned long endInstrumentOffsets = (bankOffset + 0xC + (4 * numberInstruments));

			if (endInstrumentOffsets > romSize)
				continue;

			unsigned long tbl = endInstrumentOffsets;
			if ((endInstrumentOffsets % 0x10) != 0)
				tbl += (0x10 - (endInstrumentOffsets % 0x10));
			else
				tbl += 0x10;

			while (CharArrayToLong(&ROM[tbl]) != 0)
			{
				tbl -= 4;
			}

			tbl += 4;

			results[numResults].ctlOffset = x;
			results[numResults].tblOffset = tbl;
			results[numResults].ctlSize = (results[numResults].tblOffset - results[numResults].ctlOffset);
			if (results[numResults].tblOffset > results[numResults].ctlOffset)
				results[numResults].tblSize = (romSize - results[numResults].tblOffset);
			else
				results[numResults].tblSize = (results[numResults].ctlOffset - results[numResults].tblOffset);

			results[numResults].bank = ReadAudio(ROM, &ROM[0], results[numResults].ctlSize, results[numResults].ctlOffset, &ROM[results[numResults].tblOffset], 0, 0xFFFFFFFF, 0);

			if (results[numResults].bank != NULL)
			{
				unsigned long tblHighestSpot = 0;


				
				for (int y = 0; y < results[numResults].bank->count; y++)
				{
					for (int z = 0; z < results[numResults].bank->inst[y]->soundCount; z++)
					{
						if ((results[numResults].bank->inst[y]->sounds[z]->wav.base + results[numResults].bank->inst[y]->sounds[z]->wav.len) > tblHighestSpot)
							tblHighestSpot = (results[numResults].bank->inst[y]->sounds[z]->wav.base + results[numResults].bank->inst[y]->sounds[z]->wav.len);
					}
				}

				if ((tblHighestSpot % 0x10) != 0)
					tblHighestSpot += (0x10 - (tblHighestSpot % 0x10));

				results[numResults].tblSize = tblHighestSpot;

				numResults++;

				if (numResults == 10)
					return numResults;
			}
		}
	}
	return numResults;
}

// ***************	//
// VADPCM Decoding	//
// By Ice Mario!	//
// ***************	//

static const short itable[16] =
{
	0,1,2,3,4,5,6,7,
	-8,-7,-6,-5,-4,-3,-2,-1,
};

static const short itable_half[4] =
{
	0,1,
	-2,-1,
};

//#define ENCODE

signed short SignExtend(unsigned b, // number of bits representing the number in x
						int x      // sign extend this b-bit number to r
)
{
	

	int m = 1 << (b - 1); // mask can be pre-computed if b is fixed

	x = x & ((1 << b) - 1);  // (Skip this if bits in x above position b are already zero.)
	return (x ^ m) - m;
}

void CN64AIFCAudio::decode_8( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8])
{
	int i;
	signed short tmp[8];
	signed long total = 0;
	signed short sample =0;
	memset(out, 0, sizeof(signed short)*8);

	signed short *pred2 = (pred1 + 8);

	//printf("pred2[] = %x\n" , pred2[0]);
	for(i=0; i<8; i++)
	{
		tmp[i] = itable[(i&1) ? (*in++ & 0xf) : ((*in >> 4) & 0xf)] << index;
		tmp[i] = SignExtend(index+4, tmp[i]);
	}

	for(i=0; i<8; i++)
	{
		total = (pred1[i] * lastsmp[6]);
		total+= (pred2[i] * lastsmp[7]);

		if (i>0)
		{
			for(int x=i-1; x>-1; x--)
			{
				total += ( tmp[((i-1)-x)] * pred2[x] );
				//printf("sample: %x - pred: %x - _smp: %x\n" , ((i-1)-x) , pred2[x] , tmp[((i-1)-x)]);
			}
		}

		//printf("pred = %x | total = %x\n" , pred2[0] , total);
		float result = ((tmp[i] << 0xb) + total) >> 0xb;
		if (result > 32767)
			sample = 32767;
		else if (result < -32768)
			sample = -32768;
		else
			sample = (signed short)result;

		out[i] = sample;
	}
	// update the last sample set for subsequent iterations
	memcpy(lastsmp, out, sizeof(signed short)*8);
}

void CN64AIFCAudio::decode_8_half( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8])
{
	int i;
	signed short tmp[8];
	signed long total = 0;
	signed short sample =0;
	memset(out, 0, sizeof(signed short)*8);

	signed short *pred2 = (pred1 + 8);

	//printf("pred2[] = %x\n" , pred2[0]);

	tmp[0] = (((((*in) & 0xC0) >> 6) & 0x3)) << (index);
	tmp[0] = SignExtend(index+2, tmp[0]);
	tmp[1] = (((((*in) & 0x30) >> 4) & 0x3)) << (index);
	tmp[1] = SignExtend(index+2, tmp[1]);
	tmp[2] = (((((*in) & 0x0C) >> 2) & 0x3)) << (index);
	tmp[2] = SignExtend(index+2, tmp[2]);
	tmp[3] = ((((*in++) & 0x03) & 0x3)) << (index);
	tmp[3] = SignExtend(index+2, tmp[3]);
	tmp[4] = (((((*in) & 0xC0) >> 6) & 0x3)) << (index);
	tmp[4] = SignExtend(index+2, tmp[4]);
	tmp[5] = (((((*in) & 0x30) >> 4) & 0x3)) << (index);
	tmp[5] = SignExtend(index+2, tmp[5]);
	tmp[6] = (((((*in) & 0x0C) >> 2) & 0x3)) << (index);
	tmp[6] = SignExtend(index+2, tmp[6]);
	tmp[7] = ((((*in++) & 0x03) & 0x3)) << (index);
	tmp[7] = SignExtend(index+2, tmp[7]);

	for(i=0; i<8; i++)
	{
		total = (pred1[i] * lastsmp[6]);
		total+= (pred2[i] * lastsmp[7]);

		if (i>0)
		{
			for(int x=i-1; x>-1; x--)
			{
				total += ( tmp[((i-1)-x)] * pred2[x] );
				//printf("sample: %x - pred: %x - _smp: %x\n" , ((i-1)-x) , pred2[x] , tmp[((i-1)-x)]);
			}
		}

		//printf("pred = %x | total = %x\n" , pred2[0] , total);
		float result = ((tmp[i] << 0xb) + total) >> 0xb;
		if (result > 32767)
			sample = 32767;
		else if (result < -32768)
			sample = -32768;
		else
			sample = (signed short)result;

		out[i] = sample;
	}
	// update the last sample set for subsequent iterations
	memcpy(lastsmp, out, sizeof(signed short)*8);
}


signed short adpcm_get_predicted_sample(unsigned char byte, unsigned char mask,
                                          unsigned lshift, unsigned rshift)
{
    signed short sample = ((signed short)byte & (signed short)mask) << lshift;
    sample >>= rshift; /* signed */
    return sample;
}

void adpcm_get_predicted_frame(signed short *dst, unsigned char *src,
                                      unsigned char *nibbles,
                                      unsigned int rshift)
{
    unsigned int i;

    *(dst++) = (src[0] << 8) | src[1];
    *(dst++) = (src[2] << 8) | src[3];

    for (i = 1; i < 16; ++i) 
	{
        unsigned char byteData = nibbles[i];

        *(dst++) = adpcm_get_predicted_sample(byteData, 0xf0,  8, rshift);
        *(dst++) = adpcm_get_predicted_sample(byteData, 0x0f, 12, rshift);
    }
}

signed short clamp_s16(int x)
{
    x = (x < (-0x7fff - 1)) ? (-0x7fff - 1): x;
    x = (x > 0x7fff) ? 0x7fff: x;

    return x;
}

int rdot(unsigned int n, const signed short *x, signed short *y)
{
    int accu = 0;

    y += n;

    while (n != 0) {
        accu += ((int)*(x++) * (int)*(--y));
        --n;
    }

    return accu;
}

void adpcm_decode_upto_8_samples(signed short *dst, signed short *src,
                                        signed short *cb_entry,
                                        signed short *last_samples,
                                        unsigned int size)
{
    signed short * book1 = cb_entry;
    signed short * book2 = cb_entry + 8;

    signed short l1 = last_samples[0];
    signed short l2 = last_samples[1];

    unsigned int i;
    int accu;

    for (i = 0; i < size; ++i) {
        accu = (int)src[i] << 11;
        accu += book1[i] * l1 + book2[i] * l2 + rdot(i, book2, src);
        dst[i] = clamp_s16(accu >> 11);
    }
}

unsigned long CN64AIFCAudio::decodemusyxadpcm( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book )
{
	signed short frame[32];

	int samples = 0;
 
    for (unsigned int i = 0; i < len; i+=0x28)
	{
		// TODO process the small missing end if figure out how
		if ((len - i) < 0x28)
		{
			return samples;	
		}

        unsigned char c2 = in[i+0x8];

		c2 = c2 % 0x80;

		//if ((c2 & 0xf0) > (book->order * book->npredictors * 8))
			//return samples;

		signed short* bookPredictors = (c2 & 0xf0) + book->predictors;
        unsigned int rshift = (c2 & 0x0f);

        adpcm_get_predicted_frame(frame, &in[i], &in[i+0x8], rshift);

        memcpy(out, frame, 2 * sizeof(frame[0]));
        adpcm_decode_upto_8_samples(out +  2, frame +  2, bookPredictors, out     , 6);
        adpcm_decode_upto_8_samples(out +  8, frame +  8, bookPredictors, out +  6, 8);
        adpcm_decode_upto_8_samples(out + 16, frame + 16, bookPredictors, out + 14, 8);
        adpcm_decode_upto_8_samples(out + 24, frame + 24, bookPredictors, out + 22, 8);

		out += 32;
		samples += 32;

		c2 = in[i+0x18];

		c2 = c2 % 0x80;

		//if ((c2 & 0xf0) > (book->order * book->npredictors * 8))
			//return samples;

        bookPredictors = (c2 & 0xf0) + book->predictors;
        rshift = (c2 & 0x0f);

        adpcm_get_predicted_frame(frame, &in[i+4], &in[i+0x18], rshift);

        memcpy(out, frame, 2 * sizeof(frame[0]));
        adpcm_decode_upto_8_samples(out +  2, frame +  2, bookPredictors, out     , 6);
        adpcm_decode_upto_8_samples(out +  8, frame +  8, bookPredictors, out +  6, 8);
        adpcm_decode_upto_8_samples(out + 16, frame + 16, bookPredictors, out + 14, 8);
        adpcm_decode_upto_8_samples(out + 24, frame + 24, bookPredictors, out + 22, 8);

		out += 32;
		samples += 32;
    }

	return samples;
}

unsigned long CN64AIFCAudio::decode( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book, bool decode8Only )
{
	signed short lastsmp[8];

	for (int x = 0; x < 8; x++)
		lastsmp[x] = 0;

	int index;
	int pred;
	unsigned char cmd;
	unsigned char *pin = in;
	signed short *pout = out;
	int j;
	unsigned char n1,n2;
	int total = 0;
	int _tmp;

	int samples = 0;

	// flip the predictors
	signed short *preds = (signed short*)malloc( 32 * book->npredictors );
	for (int p = 0; p < (8 * book->order * book->npredictors); p++)
	{
		preds[p] = book->predictors[p];
	}

	if (!decode8Only)
	{
		int _len = (len / 9) * 9;	//make sure length was actually a multiple of 9

		while(_len > 0)
		{
			index = (*in >> 4) & 0xf;
			pred = (*in & 0xf);

			// to not make zelda crash but doesn't fix it
			pred = pred % (book->npredictors);

			_len--;

			signed short * pred1 = &preds[ pred * 16] ;

			decode_8(++in, out, index, pred1, lastsmp);
			in+=4;	_len-=4;	out+=8;

			decode_8(in, out, index, pred1, lastsmp);
			in+=4;	_len-=4;	out+=8;

			samples += 16;
		}
	}
	else
	{
		int _len = (len / 5) * 5;	//make sure length was actually a multiple of 5

		while(_len > 0)
		{
			index = (*in >> 4) & 0xf;
			pred = (*in & 0xf);

			// to not make zelda crash but doesn't fix it
			pred = pred % (book->npredictors);

			_len--;

			signed short * pred1 = &preds[ pred * 16] ;

			decode_8_half(++in, out, index, pred1, lastsmp);
			in+=2;	_len-=2;	out+=8;

			decode_8_half(in, out, index, pred1, lastsmp);
			in+=2;	_len-=2;	out+=8;

			samples += 16;
		}
	}

	free(preds);

	return samples;
}

int CN64AIFCAudio::determineBestEncodeIndexAndPredictor_half(signed short* predictors, int numPredictors, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex, int& predictorIndex)
{
        predictorIndex = 0;
        float bestPredIndex = 99999999999.0f;
 
        int bestEncodeIndex = 0;
       
        for(int p = 0; p < numPredictors; p++)
        {
                signed short* tempSampleSet = new signed short[8];
                signed short* tmp = new signed short[8];
 
                int index = 0;
                bestFitIndex = 99999999999.0f;
 
                signed short* pred1 = &predictors[p*0x10 + 0];
                signed short* pred2 = &predictors[p*0x10 + 8];
 
                for (int testIndex = 0; testIndex < 16; testIndex++)
                {
                        for (int x = 0; x < 8; x++)
                        {
                                tempSampleSet[x] = lastSampleSet[x];
                        }
 
                        float tempFitIndex = 0;
                        for (int r = 0; r < 2; r++)
                        {
                                for (int i = 0; i < 8; i++)
                                {
                                        signed short sample = inPCMSamples[(r*8)+i];
 
                                        signed long total = pred1[i] * tempSampleSet[6];
                                        total += (pred2[i] * tempSampleSet[7]);
 
                                        if (i>0)
                                        {
                                                for(int x=i-1; x>-1; x--)
                                                {
                                                        total += ( tmp[((i-1)-x)] * pred2[x] );
                                                }
                                        }
 
                                        float bestFit = 9999999999;
                                        int bestMatch = 0;
 
                                        for (int x = 0; x < 4; x++)
                                        {
                                                signed short tmpValue = (itable_half[x] << testIndex);
                                                float newValue = (((tmpValue << 0xB) + total) >> 0xB);
                                                if ((fabs((float)(sample - newValue))) < bestFit)
                                                {
                                                        bestFit = (fabs((float)(sample - newValue)));
                                                        bestMatch = x;
                                                }
                                        }
 
                                        tmp[i] = (itable_half[bestMatch] << testIndex);
                                        tempFitIndex += bestFit;
                                }
 
                       
                                for (int x = 0; x < 8; x++)
                                {
                                        tempSampleSet[x] = inPCMSamples[(r*8)+x];
                                }
                        }
 
                        if (tempFitIndex < bestFitIndex)
                        {
                                bestFitIndex = tempFitIndex;
                                index = testIndex;
                        }
                }
 
                if (bestFitIndex < bestPredIndex)
                {
                        bestPredIndex = bestFitIndex;
                        predictorIndex = p;
                        bestEncodeIndex = index;
                }
               
                delete [] tmp;
                delete [] tempSampleSet;
        }
 
        return bestEncodeIndex;
}

int CN64AIFCAudio::determineBestEncodeIndexAndPredictor(signed short* predictors, int numPredictors, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex, int& predictorIndex)
{
        predictorIndex = 0;
        float bestPredIndex = 99999999999.0f;
 
        int bestEncodeIndex = 0;
       
        for(int p = 0; p < numPredictors; p++)
        {
                signed short* tempSampleSet = new signed short[8];
                signed short* tmp = new signed short[8];
 
                int index = 0;
                bestFitIndex = 99999999999.0f;
 
                signed short* pred1 = &predictors[p*0x10 + 0];
                signed short* pred2 = &predictors[p*0x10 + 8];
 
                for (int testIndex = 0; testIndex < 16; testIndex++)
                {
                        for (int x = 0; x < 8; x++)
                        {
                                tempSampleSet[x] = lastSampleSet[x];
                        }
 
                        float tempFitIndex = 0;
                        for (int r = 0; r < 2; r++)
                        {
                                for (int i = 0; i < 8; i++)
                                {
                                        signed short sample = inPCMSamples[(r*8)+i];
 
                                        signed long total = pred1[i] * tempSampleSet[6];
                                        total += (pred2[i] * tempSampleSet[7]);
 
                                        if (i>0)
                                        {
                                                for(int x=i-1; x>-1; x--)
                                                {
                                                        total += ( tmp[((i-1)-x)] * pred2[x] );
                                                }
                                        }
 
                                        float bestFit = 9999999999;
                                        int bestMatch = 0;
 
                                        for (int x = 0; x < 16; x++)
                                        {
                                                signed short tmpValue = (itable[x] << testIndex);
                                                float newValue = (((tmpValue << 0xB) + total) >> 0xB);
                                                if ((fabs((float)(sample - newValue))) < bestFit)
                                                {
                                                        bestFit = (fabs((float)(sample - newValue)));
                                                        bestMatch = x;
                                                }
                                        }
 
                                        tmp[i] = (itable[bestMatch] << testIndex);
                                        tempFitIndex += bestFit;
                                }
 
                       
                                for (int x = 0; x < 8; x++)
                                {
                                        tempSampleSet[x] = inPCMSamples[(r*8)+x];
                                }
                        }
 
                        if (tempFitIndex < bestFitIndex)
                        {
                                bestFitIndex = tempFitIndex;
                                index = testIndex;
                        }
                }
 
                if (bestFitIndex < bestPredIndex)
                {
                        bestPredIndex = bestFitIndex;
                        predictorIndex = p;
                        bestEncodeIndex = index;
                }
               
                delete [] tmp;
                delete [] tempSampleSet;
        }
 
        return bestEncodeIndex;
}

signed short* CN64AIFCAudio::determineBestPredictors(ALBank* alBank, unsigned long& npredictors, unsigned long& norder, signed short* inPCMSamples, int numberSamplesIn)
{
	int numPredTests = 0xC;
	
	signed short** predictors = new signed short*[numPredTests];
	float* entropySet = new float[numPredTests];

	for (int x = 0; x < numPredTests; x++)
	{
		predictors[x] = new signed short[0x10];
		entropySet[x] = 0;
	}

	int predNum = 0;

	// todo find some way to calculate this
	predictors[predNum][0] = 0xFAE2;
	predictors[predNum][1] = 0xFAD0;
	predictors[predNum][2] = 0xFE04;
	predictors[predNum][3] = 0x014F;
	predictors[predNum][4] = 0x0298;
	predictors[predNum][5] = 0x01CB;
	predictors[predNum][6] = 0x0029;
	predictors[predNum][7] = 0xFF03;
	predictors[predNum][8] = 0x081C;
	predictors[predNum][9] = 0x031A;
	predictors[predNum][10] = 0xFDF5;
	predictors[predNum][11] = 0xFBF2;
	predictors[predNum][12] = 0xFD32;
	predictors[predNum][13] = 0xFFC1;
	predictors[predNum][14] = 0x018B;
	predictors[predNum][15] = 0x01B9;

	unsigned long vadpcmOutputLength;
	unsigned char* vadpcmData = new unsigned char[numberSamplesIn];

	ALADPCMBook book;
	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;




	predictors[predNum][0] = 0xF8A1;
	predictors[predNum][1] = 0xF25D;
	predictors[predNum][2] = 0xED92;
	predictors[predNum][3] = 0xEA7A;
	predictors[predNum][4] = 0xE92C;
	predictors[predNum][5] = 0xE99D;
	predictors[predNum][6] = 0xEBA1;
	predictors[predNum][7] = 0xEEF3;
	predictors[predNum][8] = 0x0ECC;
	predictors[predNum][9] = 0x1400;
	predictors[predNum][10] = 0x175B;
	predictors[predNum][11] = 0x18C5;
	predictors[predNum][12] = 0x184B;
	predictors[predNum][13] = 0x161B;
	predictors[predNum][14] = 0x1280;
	predictors[predNum][15] = 0x0DDA;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0xFD78;
	predictors[predNum][1] = 0xFF80;
	predictors[predNum][2] = 0x00B4;
	predictors[predNum][3] = 0x004C;
	predictors[predNum][4] = 0xFFD6;
	predictors[predNum][5] = 0xFFE0;
	predictors[predNum][6] = 0x0007;
	predictors[predNum][7] = 0x000C;
	predictors[predNum][8] = 0x0194;
	predictors[predNum][9] = 0xFDC8;
	predictors[predNum][10] = 0xFF10;
	predictors[predNum][11] = 0x0085;
	predictors[predNum][12] = 0x0066;
	predictors[predNum][13] = 0xFFEA;
	predictors[predNum][14] = 0xFFDB;
	predictors[predNum][15] = 0x0000;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0x0210;
	predictors[predNum][1] = 0x00FD;
	predictors[predNum][2] = 0x0101;
	predictors[predNum][3] = 0x00BC;
	predictors[predNum][4] = 0x009C;
	predictors[predNum][5] = 0x007B;
	predictors[predNum][6] = 0x0063;
	predictors[predNum][7] = 0x004F;
	predictors[predNum][8] = 0x03D4;
	predictors[predNum][9] = 0x03E5;
	predictors[predNum][10] = 0x02D9;
	predictors[predNum][11] = 0x025E;
	predictors[predNum][12] = 0x01DE;
	predictors[predNum][13] = 0x0181;
	predictors[predNum][14] = 0x0133;
	predictors[predNum][15] = 0x00F6;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0x01CF;
	predictors[predNum][1] = 0x003E;
	predictors[predNum][2] = 0x0071;
	predictors[predNum][3] = 0x001D;
	predictors[predNum][4] = 0x001D;
	predictors[predNum][5] = 0x000B;
	predictors[predNum][6] = 0x0008;
	predictors[predNum][7] = 0x0003;
	predictors[predNum][8] = 0x0112;
	predictors[predNum][9] = 0x01F4;
	predictors[predNum][10] = 0x0081;
	predictors[predNum][11] = 0x0082;
	predictors[predNum][12] = 0x002F;
	predictors[predNum][13] = 0x0024;
	predictors[predNum][14] = 0x000F;
	predictors[predNum][15] = 0x000A;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFCC0;
	predictors[predNum][1] = 0xFBEC;
	predictors[predNum][2] = 0xFC33;
	predictors[predNum][3] = 0xFCE2;
	predictors[predNum][4] = 0xFDA2;
	predictors[predNum][5] = 0xFE4B;
	predictors[predNum][6] = 0xFED1;
	predictors[predNum][7] = 0xFF36;
	predictors[predNum][8] = 0x0A0B;
	predictors[predNum][9] = 0x095C;
	predictors[predNum][10] = 0x07AC;
	predictors[predNum][11] = 0x05D5;
	predictors[predNum][12] = 0x0435;
	predictors[predNum][13] = 0x02E9;
	predictors[predNum][14] = 0x01F2;
	predictors[predNum][15] = 0x0143;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFBF6;
	predictors[predNum][1] = 0xFBDF;
	predictors[predNum][2] = 0xFDD1;
	predictors[predNum][3] = 0xFFDA;
	predictors[predNum][4] = 0x00F4;
	predictors[predNum][5] = 0x010C;
	predictors[predNum][6] = 0x0097;
	predictors[predNum][7] = 0x0013;
	predictors[predNum][8] = 0x082E;
	predictors[predNum][9] = 0x0453;
	predictors[predNum][10] = 0x004B;
	predictors[predNum][11] = 0xFE1D;
	predictors[predNum][12] = 0xFDED;
	predictors[predNum][13] = 0xFED4;
	predictors[predNum][14] = 0xFFDA;
	predictors[predNum][15] = 0x0070;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFB63;
	predictors[predNum][1] = 0xF930;
	predictors[predNum][2] = 0xF89A;
	predictors[predNum][3] = 0xF901;
	predictors[predNum][4] = 0xF9F0;
	predictors[predNum][5] = 0xFB15;
	predictors[predNum][6] = 0xFC3C;
	predictors[predNum][7] = 0xFD46;
	predictors[predNum][8] = 0x0BD0;
	predictors[predNum][9] = 0x0CD4;
	predictors[predNum][10] = 0x0C21;
	predictors[predNum][11] = 0x0A84;
	predictors[predNum][12] = 0x0888;
	predictors[predNum][13] = 0x0688;
	predictors[predNum][14] = 0x04BA;
	predictors[predNum][15] = 0x0336;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xF9D0;
	predictors[predNum][1] = 0xF692;
	predictors[predNum][2] = 0xF66C;
	predictors[predNum][3] = 0xF8B4;
	predictors[predNum][4] = 0xFC4A;
	predictors[predNum][5] = 0xFFFF;
	predictors[predNum][6] = 0x02DD;
	predictors[predNum][7] = 0x045D;
	predictors[predNum][8] = 0x0C30;
	predictors[predNum][9] = 0x0C62;
	predictors[predNum][10] = 0x096F;
	predictors[predNum][11] = 0x04CB;
	predictors[predNum][12] = 0x0002;
	predictors[predNum][13] = 0xFC4D;
	predictors[predNum][14] = 0xFA5C;
	predictors[predNum][15] = 0xFA44;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0x0000;
	predictors[predNum][1] = 0x0000;
	predictors[predNum][2] = 0x0000;
	predictors[predNum][3] = 0x0000;
	predictors[predNum][4] = 0x0000;
	predictors[predNum][5] = 0x0000;
	predictors[predNum][6] = 0x0000;
	predictors[predNum][7] = 0x0000;
	predictors[predNum][8] = 0x0000;
	predictors[predNum][9] = 0x0000;
	predictors[predNum][10] = 0x0000;
	predictors[predNum][11] = 0x0000;
	predictors[predNum][12] = 0x0000;
	predictors[predNum][13] = 0x0000;
	predictors[predNum][14] = 0x0000;
	predictors[predNum][15] = 0x0000;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFA92;
	predictors[predNum][1] = 0xF99C;
	predictors[predNum][2] = 0xFC2B;
	predictors[predNum][3] = 0xFFD4;
	predictors[predNum][4] = 0x0266;
	predictors[predNum][5] = 0x02F0;
	predictors[predNum][6] = 0x01D5;
	predictors[predNum][7] = 0x0029;
	predictors[predNum][8] = 0x096A;
	predictors[predNum][9] = 0x05A5;
	predictors[predNum][10] = 0x0041;
	predictors[predNum][11] = 0xFC77;
	predictors[predNum][12] = 0xFBAB;
	predictors[predNum][13] = 0xFD4E;
	predictors[predNum][14] = 0xFFC4;
	predictors[predNum][15] = 0x018E;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFB0A;
	predictors[predNum][1] = 0xF88C;
	predictors[predNum][2] = 0xF7E3;
	predictors[predNum][3] = 0xF870;
	predictors[predNum][4] = 0xF9AD;
	predictors[predNum][5] = 0xFB31;
	predictors[predNum][6] = 0xFCB4;
	predictors[predNum][7] = 0xFE07;
	predictors[predNum][8] = 0x0C04;
	predictors[predNum][9] = 0x0D15;
	predictors[predNum][10] = 0x0C32;
	predictors[predNum][11] = 0x0A33;
	predictors[predNum][12] = 0x07C1;
	predictors[predNum][13] = 0x0551;
	predictors[predNum][14] = 0x032D;
	predictors[predNum][15] = 0x0179;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	int numFinalPred = 4;
	npredictors = numFinalPred;
	norder = 2;

	selectionSortAscending(entropySet, predictors, numPredTests);

	

	signed short* finalPred = new signed short[numFinalPred * 0x10];
	for (int x = 0; x < numFinalPred; x++)
	{
		for (int y = 0; y < 0x10; y++)
		{
			finalPred[x*0x10+y] = predictors[x][y];
		}
	}

	for (int x = 0; x < numPredTests; x++)
	{
		delete [] predictors[x];
	}

	delete [] predictors;
	delete [] entropySet;

	/*FILE* inFile2 = fopen("C:\\temp\\predictorset.bin", "rb");
	fseek(inFile2, 0, SEEK_END);
	int size = ftell(inFile2);
	fseek(inFile2, 0, SEEK_SET);
	unsigned char* buffer2 = new unsigned char[size];
	fread(buffer2, 1, (size), inFile2);
	fclose(inFile2);

	float bestEntropy = 99999999999.0f;
	unsigned long bestPredOffset = 0;

	for (unsigned long x = 0; x < size; x += 0x20)
	{
		signed short* predictors = new signed short[0x10];
		for (int r = 0; r < 0x10; r++)
		{
			predictors[r] = (signed short)((buffer2[x+r*2+0] << 8) | buffer2[x+r*2+1]);
		}

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamplesIn];

		ALADPCMBook book;
		book.npredictors = 1;
		book.order = 2;
		book.predictors = predictors;
		float entropy = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

		if (entropy < bestEntropy)
		{
			bestPredOffset = x;
			bestEntropy = entropy;
		}

		delete [] vadpcmData;
		delete [] predictors;
	}
	
	delete [] buffer2;

	signed short* finalPred = new signed short[0x10];
	for (int x = 0; x < 0x10; x++)
	{
		finalPred[x] = (signed short)((buffer2[bestPredOffset + x*2+0] << 8) | buffer2[bestPredOffset + x*2+1]);
	}*/

	return finalPred;
}

void CN64AIFCAudio::selectionSortAscending(float *array, signed short** arrayCopy, int length)
{
	int i,j,minat;
	float min;
	for(i=0;i<(length-1);i++)
	{
		minat=i;
		min=array[i];

		for(j=i+1;j<(length);j++) //select the min of the rest of array
		{
			if(min>array[j])   //ascending order for descending reverse
			{
				minat=j;  //the position of the min element 
				min=array[j];
			}
		}
		float temp=array[i] ;
		array[i]=array[minat];  //swap 
		array[minat]=temp;

		signed short tempArrayCopy[0x10];
		for (int r = 0; r < 0x10; r++)
		{
			tempArrayCopy[r] = arrayCopy[i][r];
			arrayCopy[i][r] = arrayCopy[minat][r];
			arrayCopy[minat][r] = tempArrayCopy[r];
		}
	}
}

int CN64AIFCAudio::determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex)
{
	signed short* tempSampleSet = new signed short[8];
	signed short* tmp = new signed short[8];

	int index = 0;
	bestFitIndex = 99999999999.0f;

	for (int testIndex = 0; testIndex < 16; testIndex++)
	{
		for (int x = 0; x < 8; x++)
		{
			tempSampleSet[x] = lastSampleSet[x];
		}

		float tempFitIndex = 0;
		for (int r = 0; r < 2; r++)
		{
			for (int i = 0; i < 8; i++)
			{
				signed short sample = inPCMSamples[(r*8)+i];

				signed long total = pred1[i] * tempSampleSet[6];
				total += (pred2[i] * tempSampleSet[7]);

				if (i>0)
				{
					for(int x=i-1; x>-1; x--)
					{
						total += ( tmp[((i-1)-x)] * pred2[x] );
					}
				}

				float bestFit = 9999999999;
				int bestMatch = 0;

				for (int x = 0; x < 16; x++)
				{
					signed short newValue = ((((itable[x] << testIndex) << 0xB) + total) >> 0xB);
					if ((fabs((float)(sample - newValue))) < bestFit)
					{
						bestFit = (fabs((float)(sample - newValue)));
						bestMatch = x;
					}
				}

				tmp[i] = (itable[bestMatch] << testIndex);
				tempFitIndex += bestFit;
			}

			
			for (int x = 0; x < 8; x++)
			{
				tempSampleSet[x] = inPCMSamples[(r*8)+x];
			}
		}

		if (tempFitIndex < bestFitIndex)
		{
			bestFitIndex = tempFitIndex;
			index = testIndex;
		}
	}

	delete [] tmp;
	delete [] tempSampleSet;

	return index;
}

float CN64AIFCAudio::encode_half(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book)
{
	float entropy = 0.0f;

	signed short* lastSampleSet = new signed short[8];
	for (int x = 0; x < 8; x++)
		lastSampleSet[x] = 0x0;

	signed short* tmp = new signed short[8];

	lenOut = 0;

	for (int y = 0; y < numberSamplesIn; y += 16)
	{
		float totalBestFitDelta = 0;

		signed short* pred1;
		signed short* pred2;

		int predictor = 0;
		int index = 0;

		index = determineBestEncodeIndexAndPredictor_half(book->predictors, book->npredictors, lastSampleSet, &inPCMSamples[y], totalBestFitDelta, predictor);

		pred1 = &book->predictors[predictor*0x10 + 0];
		pred2 = &book->predictors[predictor*0x10 + 8];

		outVADPCM[lenOut++] = ((index << 4) | predictor);

		for (int r = 0; r < 2; r++)
		{
			signed short resultingValue[8];
			for (int i = 0; i < 8; i++)
			{
				signed short sample = 0;
				if ((y + (r * 8) + i) < numberSamplesIn)
                {
					sample = inPCMSamples[y+(r*8)+i];
				}

				signed long total = pred1[i] * lastSampleSet[6];
				total += (pred2[i] * lastSampleSet[7]);

				if (i>0)
				{
					for(int x=i-1; x>-1; x--)
					{
						total += ( tmp[((i-1)-x)] * pred2[x] );
					}
				}

				float bestFit = 9999999999;
				int bestMatch = 0;
				

				for (int x = 0; x < 4; x++)
				{
					signed short newValue = ((((itable_half[x] << index) << 0xB) + total) >> 0xB);
					if ((fabs((float)(sample - newValue))) < bestFit)
					{
						bestFit = (fabs((float)(sample - newValue)));
						bestMatch = x;
						resultingValue[i] = newValue;
					}
				}

				tmp[i] = (itable_half[bestMatch] << index);

				if ((i % 4) == 0)
					outVADPCM[lenOut] = ((bestMatch << 6) & 0xC0);
				else if ((i % 4) == 1)
					outVADPCM[lenOut] |= ((bestMatch << 4) & 0x30);
				else if ((i % 4) == 2)
					outVADPCM[lenOut] |= ((bestMatch << 2) & 0x0C);
				else
				{
					outVADPCM[lenOut] = (outVADPCM[lenOut] | (bestMatch & 0x3));
					lenOut++;
				}

				entropy += bestFit;
			}

			for (int x = 0; x < 8; x++)
			{
				//lastSampleSet[x] = inPCMSamples[y+(r*8)+x];
				lastSampleSet[x] = resultingValue[x];
			}
		}
	}


	if ((numberSamplesIn % 16) != 0)
	{
		// just cut it off for now
	}

	delete [] lastSampleSet;
	delete [] tmp;

	return entropy;
}

float CN64AIFCAudio::encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book)
{
	float entropy = 0.0f;

	signed short* lastSampleSet = new signed short[8];
	for (int x = 0; x < 8; x++)
		lastSampleSet[x] = 0x0;

	signed short* tmp = new signed short[8];

	lenOut = 0;

	for (int y = 0; y < numberSamplesIn; y += 16)
	{
		float totalBestFitDelta = 0;

		signed short* pred1;
		signed short* pred2;

		int predictor = 0;
		int index = 0;

		index = determineBestEncodeIndexAndPredictor(book->predictors, book->npredictors, lastSampleSet, &inPCMSamples[y], totalBestFitDelta, predictor);

		pred1 = &book->predictors[predictor*0x10 + 0];
		pred2 = &book->predictors[predictor*0x10 + 8];

		//index = determineBestEncodeIndex(pred1, pred2, lastSampleSet, &inPCMSamples[y], totalBestFitDelta);

		outVADPCM[lenOut++] = ((index << 4) | predictor);

		for (int r = 0; r < 2; r++)
		{
			signed short resultingValue[8];
			for (int i = 0; i < 8; i++)
			{
				signed short sample = 0;
				if ((y + (r * 8) + i) < numberSamplesIn)
                {
					sample = inPCMSamples[y+(r*8)+i];
				}

				signed long total = pred1[i] * lastSampleSet[6];
				total += (pred2[i] * lastSampleSet[7]);

				if (i>0)
				{
					for(int x=i-1; x>-1; x--)
					{
						total += ( tmp[((i-1)-x)] * pred2[x] );
					}
				}

				float bestFit = 9999999999;
				int bestMatch = 0;
				

				for (int x = 0; x < 16; x++)
				{
					signed short newValue = ((((itable[x] << index) << 0xB) + total) >> 0xB);
					if ((fabs((float)(sample - newValue))) < bestFit)
					{
						bestFit = (fabs((float)(sample - newValue)));
						bestMatch = x;
						resultingValue[i] = newValue;
					}
				}

				tmp[i] = (itable[bestMatch] << index);

				if ((i % 2) == 0)
					outVADPCM[lenOut] = (bestMatch << 4);
				else
				{
					outVADPCM[lenOut] = (outVADPCM[lenOut] | bestMatch);
					lenOut++;
				}

				entropy += bestFit;
			}

			for (int x = 0; x < 8; x++)
			{
				//lastSampleSet[x] = inPCMSamples[y+(r*8)+x];
				lastSampleSet[x] = resultingValue[x];
			}
		}
	}


	if ((numberSamplesIn % 16) != 0)
	{
		// just cut it off for now
	}

	delete [] lastSampleSet;
	delete [] tmp;

	return entropy;
}


void CN64AIFCAudio::InitializeKeyTable()
{
	//FILE* outKeyMapping = fopen("C:\\temp\\keymapping.txt", "w");
	for (int x = 0; x < 0xFF; x++)
	{
		CN64AIFCAudio::keyTable[x] = pow(2, (float)((60.0 - (float)x) / 12.0));

		//fprintf(outKeyMapping, "%02X (%d) : %f (%08X)\n", x, x, CN64AIFCAudio::keyTable[x], *reinterpret_cast<unsigned long*> (&CN64AIFCAudio::keyTable[x]));
	}
	//fclose(outKeyMapping);
}

bool CN64AIFCAudio::WriteWav(CString wavFilename, float samplingRate, std::vector<unsigned short> pcmSamples)
{
	FILE* outFileTempRaw = fopen(wavFilename, "wb");
	if (outFileTempRaw == NULL)
	{
		MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
		return false;
	}

	unsigned char* wavHeader = new unsigned char[0x28];

	wavHeader[0x0] = 0x52;
	wavHeader[0x1] = 0x49;
	wavHeader[0x2] = 0x46;
	wavHeader[0x3] = 0x46;
	unsigned long chunkSize = 0x2C + (pcmSamples.size() * 2) + 0x2C - 0x8;
	wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
	wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
	wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
	wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
	wavHeader[0x8] = 0x57;
	wavHeader[0x9] = 0x41;
	wavHeader[0xA] = 0x56;
	wavHeader[0xB] = 0x45;
	wavHeader[0xC] = 0x66;
	wavHeader[0xD] = 0x6D;
	wavHeader[0xE] = 0x74;
	wavHeader[0xF] = 0x20;
	wavHeader[0x10] = 0x10;
	wavHeader[0x11] = 0x00;
	wavHeader[0x12] = 0x00;
	wavHeader[0x13] = 0x00;
	wavHeader[0x14] = 0x01; //WAVE_FORMAT_PCM
	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x01; //1 channel
	wavHeader[0x17] = 0x00;
	wavHeader[0x18] = (((unsigned long)samplingRate >> 0) & 0xFF);
	wavHeader[0x19] = (((unsigned long)samplingRate >> 8) & 0xFF);
	wavHeader[0x1A] = (((unsigned long)samplingRate >> 16) & 0xFF);
	wavHeader[0x1B] = (((unsigned long)samplingRate >> 24) & 0xFF);
	wavHeader[0x1C] = ((((unsigned long)samplingRate * 2) >> 0) & 0xFF);
	wavHeader[0x1D] = ((((unsigned long)samplingRate * 2) >> 8) & 0xFF);
	wavHeader[0x1E] = ((((unsigned long)samplingRate * 2) >> 16) & 0xFF);
	wavHeader[0x1F] = ((((unsigned long)samplingRate * 2) >> 24) & 0xFF);
	wavHeader[0x20] = 0x02;
	wavHeader[0x21] = 0x00;
	wavHeader[0x22] = 0x10;
	wavHeader[0x23] = 0x00;
	wavHeader[0x24] = 0x64;
	wavHeader[0x25] = 0x61;
	wavHeader[0x26] = 0x74;
	wavHeader[0x27] = 0x61;

	fwrite(wavHeader, 1, 0x28, outFileTempRaw );

	delete [] wavHeader;

	unsigned long length = (pcmSamples.size() * 2);
	fwrite(&length, 1, 4, outFileTempRaw);

	for (size_t x = 0; x < pcmSamples.size(); x++)
	{
		fwrite(&(pcmSamples[x]), 1, 2, outFileTempRaw);
	}


	// SMPL
	wavHeader = new unsigned char[0x44];
	for (int x = 0; x < 0x44; x++)
		wavHeader[x] = 0x00;

	wavHeader[0x0] = 0x73;
	wavHeader[0x1] = 0x6D;
	wavHeader[0x2] = 0x70;
	wavHeader[0x3] = 0x6C;
	wavHeader[0x4] = 0x24;
	wavHeader[0x5] = 0x00;
	wavHeader[0x6] = 0x00;
	wavHeader[0x7] = 0x00;
	
	wavHeader[0x14] = 0x3C;

	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x00;
	wavHeader[0x17] = 0x00;

	fwrite(wavHeader, 1, 0x24 + 8, outFileTempRaw );

	delete [] wavHeader;

	fclose(outFileTempRaw);
	
	return true;
}

bool CN64AIFCAudio::WriteWavStereo(CString wavFilename, float samplingRate, std::vector<unsigned short> pcmSamples)
{
	FILE* outFileTempRaw = fopen(wavFilename, "wb");
	if (outFileTempRaw == NULL)
	{
		MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
		return false;
	}

	unsigned char* wavHeader = new unsigned char[0x28];

	wavHeader[0x0] = 0x52;
	wavHeader[0x1] = 0x49;
	wavHeader[0x2] = 0x46;
	wavHeader[0x3] = 0x46;
	unsigned long chunkSize = 0x2C + (pcmSamples.size() * 2) + 0x2C - 0x8;
	wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
	wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
	wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
	wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
	wavHeader[0x8] = 0x57;
	wavHeader[0x9] = 0x41;
	wavHeader[0xA] = 0x56;
	wavHeader[0xB] = 0x45;
	wavHeader[0xC] = 0x66;
	wavHeader[0xD] = 0x6D;
	wavHeader[0xE] = 0x74;
	wavHeader[0xF] = 0x20;
	wavHeader[0x10] = 0x10;
	wavHeader[0x11] = 0x00;
	wavHeader[0x12] = 0x00;
	wavHeader[0x13] = 0x00;
	wavHeader[0x14] = 0x01; //WAVE_FORMAT_PCM
	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x02; //2 channels
	wavHeader[0x17] = 0x00;
	wavHeader[0x18] = (((unsigned long)samplingRate >> 0) & 0xFF);
	wavHeader[0x19] = (((unsigned long)samplingRate >> 8) & 0xFF);
	wavHeader[0x1A] = (((unsigned long)samplingRate >> 16) & 0xFF);
	wavHeader[0x1B] = (((unsigned long)samplingRate >> 24) & 0xFF);
	wavHeader[0x1C] = ((((unsigned long)samplingRate * 2 * 2) >> 0) & 0xFF);
	wavHeader[0x1D] = ((((unsigned long)samplingRate * 2 * 2) >> 8) & 0xFF);
	wavHeader[0x1E] = ((((unsigned long)samplingRate * 2 * 2) >> 16) & 0xFF);
	wavHeader[0x1F] = ((((unsigned long)samplingRate * 2 * 2) >> 24) & 0xFF);
	wavHeader[0x20] = 0x04;
	wavHeader[0x21] = 0x00;
	wavHeader[0x22] = 0x10;
	wavHeader[0x23] = 0x00;
	wavHeader[0x24] = 0x64;
	wavHeader[0x25] = 0x61;
	wavHeader[0x26] = 0x74;
	wavHeader[0x27] = 0x61;

	fwrite(wavHeader, 1, 0x28, outFileTempRaw );

	delete [] wavHeader;

	unsigned long length = (pcmSamples.size() * 2);
	fwrite(&length, 1, 4, outFileTempRaw);

	for (size_t x = 0; x < pcmSamples.size(); x++)
	{
		fwrite(&pcmSamples[x], 1, 2, outFileTempRaw);
	}


	// SMPL
	wavHeader = new unsigned char[0x44];
	for (int x = 0; x < 0x44; x++)
		wavHeader[x] = 0x00;

	wavHeader[0x0] = 0x73;
	wavHeader[0x1] = 0x6D;
	wavHeader[0x2] = 0x70;
	wavHeader[0x3] = 0x6C;
	wavHeader[0x4] = 0x24;
	wavHeader[0x5] = 0x00;
	wavHeader[0x6] = 0x00;
	wavHeader[0x7] = 0x00;
	
	wavHeader[0x14] = 0x3C;

	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x00;
	wavHeader[0x17] = 0x00;

	fwrite(wavHeader, 1, 0x24 + 8, outFileTempRaw );

	delete [] wavHeader;

	fclose(outFileTempRaw);
	
	return true;
}

unsigned long CN64AIFCAudio::ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned short upper = CharArrayToShort(&GEROM[upperLocation+2]);
	unsigned short lower = CharArrayToShort(&GEROM[lowerLocation+2]);
	if (lower > 0x7FFF)
	{
		return ((upper - 1) << 16) | lower;
	}
	else
	{
		return ((upper) << 16) | lower;
	}
}