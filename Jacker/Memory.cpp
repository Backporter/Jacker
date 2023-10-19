#include "Memory.h"

//
#include "MessageHandler.h"
#include "System.h"
#include "Macro.h"

void safe_write(uintptr_t dst, const void* src, size_t len)
{
	if (dst)
	{
		int32_t prot = VM_PROT_ALL;
		int32_t ret = sceKernelQueryMemoryProtection((void*)dst, NULL, NULL, &prot);
		if (ret < 0)
		{
			return;
		}

		sceKernelMprotect((const void*)dst, len, VM_PROT_ALL);
		memcpy((void *)dst, src, len);
		sceKernelMprotect((void*)dst, len, prot);
	}
}

void* byteScan(void* addr, uint8_t* pattern, size_t searchLen)
{
	// only to preverent a deadlock, the amount of bytes that are scanned in total is 256 * 10 = (2560)
	constexpr size_t illteration_atempts = 10;

	// make sure to pass proper paramaters.
	if (!addr || !pattern)
		return nullptr;

	// sanity check
	if (searchLen > 256)
		searchLen = 256;

	//
	uint8_t* searchAddr = (uint8_t*)addr;
	uint8_t buf[256]{ 0 };

	for (int i = 0; i < illteration_atempts; i++)
	{
		//
		memcpy(buf, (const void*)searchAddr, sizeof(buf));

		//
		for (int j = 0; j < sizeof(buf); j++)
		{
			if (!memcmp(&buf[j], pattern, searchLen))
				return searchAddr + j;
		}

		//
		searchAddr += sizeof(buf);
	}

	return nullptr;
}

void try_patch(uintptr_t a_src, Assembly14* a_mem, uintptr_t func)
{
	// we'll create this then copy it into the scee prx's memory.
	Assembly5  sceVideoOutPrxCall;
	Assembly14 sceVideoOutTrampoline;

	//
	auto disp = reinterpret_cast<const uint8_t*>(a_mem) - reinterpret_cast<const uint8_t*>(a_src + sizeof(Assembly5));
	if (disp >= UINT32_MAX)
	{
		Notify("Unable to create hook due to memory order. (ASLR? / %p)", disp);
	}
	else
	{
		//
		sceVideoOutPrxCall.opcode = 0xE8;
		sceVideoOutPrxCall.disp = disp;

		//
		safe_write((uint64_t)a_src, &sceVideoOutPrxCall, sizeof(sceVideoOutPrxCall));

		//
		sceVideoOutTrampoline.jmp = 0xFF;
		sceVideoOutTrampoline.modrm = 0x25;
		sceVideoOutTrampoline.disp = 0;
		sceVideoOutTrampoline.addr = (uintptr_t)func;

		//
		safe_write((uint64_t)a_mem, &sceVideoOutTrampoline, sizeof(sceVideoOutTrampoline));
	}
}