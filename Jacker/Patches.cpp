#include "Patches.h"
#include "MessageHandler.h"
#include "Memory.h"
#include "Macro.h"

#include <stdint.h>

#if __ORBIS__
#include <video_out.h>
#else
#include <orbis/VideoOut.h>
#endif

NAKED int func(int)
{
	INTEL_SENTAX_ASM("mov esi, 0\ncall rax\nret");
}

void patch_xor()
{
	//
	uint8_t searchASM[]
	{
		0x44, 0x89, 0xF6,	// mov esi, r14d
	};

	//
	uint8_t patchASM[]
	{
		0x31, 0xF6, 0x90 // xor esi, esi + nop
	};

	//
	Notify("Scanning libsceVideoOut.sprx");

	//
	auto addr = byteScan
	(
		(void*)sceVideoOutSetFlipRate,
		searchASM,
		sizeof(searchASM)
	);

	if (addr)
	{
		//
		Notify("Found ASM!.. attempting to patch.");

		//
		safe_write((uintptr_t)addr, patchASM, sizeof(patchASM));
	}
}

void patch_db()
{
	//
	Notify("Scanning libsceVideoOut.sprx");

	//
	uint8_t searchASM[]
	{
		0x44, 0x89, 0xF6,	// mov     esi, r14d
		0xFF, 0xD0,			// call    rax
	};

	uint8_t searchASM2[]
	{
		0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90,
		0x90, 0x90
	};

	auto ptr = byteScan
	(
		(void*)sceVideoOutSetFlipRate,
		searchASM,
		sizeof(searchASM)
	);

	auto freeScan = byteScan
	(
		(void*)sceVideoOutSetFlipRate,
		searchASM2,
		sizeof(searchASM2)
	);

	if (ptr && freeScan)
	{
		Notify("Found ASM!.. attempting to patch.");
		try_patch((uintptr_t)ptr, (Assembly14*)freeScan, (uintptr_t)func);
	}
	else
	{
		Notify("Failed to find ASM/free space! aborting.");
	}
}