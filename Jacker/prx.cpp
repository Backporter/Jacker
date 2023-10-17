#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <cstdarg>

#if __ORBIS__
#include <kernel.h>
#include <video_out.h>
#else
#include <orbis/libkernel.h>
#include <orbis/VideoOut.h>
#endif

#define PATCH_XOR 0
#define PATCH_DB 1

//
#define PATCH_MODE PATCH_XOR

#if __ORBIS__
enum OrbisNotificationRequestType
{
	NotificationRequest = 0,
	SystemNotification = 1,
	SystemNotificationWithUserId = 2,
	SystemNotificationWithDeviceId = 3,
	SystemNotificationWithDeviceIdRelatedToUser = 4,
	SystemNotificationWithText = 5,
	SystemNotificationWithTextRelatedToUser = 6,
	SystemNotificationWithErrorCode = 7,
	SystemNotificationWithAppId = 8,
	SystemNotificationWithAppName = 9,
	SystemNotificationWithAppInfo = 9,
	SystemNotificationWithAppNameRelatedToUser = 10,
	SystemNotificationWithParams = 11,
	SendSystemNotificationWithUserName = 12,
	SystemNotificationWithUserNameInfo = 13,
	SendAddressingSystemNotification = 14,
	AddressingSystemNotificationWithDeviceId = 15,
	AddressingSystemNotificationWithUserName = 16,
	AddressingSystemNotificationWithUserId = 17,
	UNK_1 = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	UNK_2 = 102,
};

struct OrbisNotificationRequest
{
	enum OrbisNotificationRequestType type;
	int reqId;
	int priority;
	int msgId;
	int targetId;
	int userId;
	int unk1;
	int unk2;
	int appId;
	int errorNum;
	int unk3;
	unsigned char useIconImageUri;
	char message[1024];
	char iconUri[1024];
	char unk[1024];
};

int(*_sceKernelSendNotificationRequest)(int, OrbisNotificationRequest*, size_t, int);

int64_t sceKernelSendNotificationRequest(int device, OrbisNotificationRequest* req, size_t size, int blocking)
{
	if (_sceKernelSendNotificationRequest)
		return _sceKernelSendNotificationRequest(device, req, size, blocking);
}

static void Init()
{
	auto libkernelhandle = sceKernelLoadStartModule("libkernel.sprx", 0, 0, 0, 0, 0);
	if (libkernelhandle > 0)
	{
		sceKernelDlsym(libkernelhandle, "sceKernelSendNotificationRequest", (void**)&_sceKernelSendNotificationRequest);
	}
}

void Notify(const char* a_fmt, ...)
{
	/*
	//
	OrbisNotificationRequest Buffer;

	va_list args;
	va_start(args, a_fmt);
	[[maybe_unused]] size_t length = vsprintf(Buffer.message, a_fmt, args);
	va_end(args);

	Buffer.type = OrbisNotificationRequestType::NotificationRequest;
	Buffer.unk3 = 0;
	Buffer.useIconImageUri = 1;
	Buffer.targetId = -1;

	strncpy(Buffer.iconUri, "https://www.akcpetinsurance.com/res/akc/images/icons/home/home_dog.png", sizeof(Buffer.iconUri));

	sceKernelSendNotificationRequest(0, &Buffer, 3120, 0);
	*/
}

#ifndef EXPORT
#define EXPORT extern "C" __declspec (dllexport)
#endif
#else
#ifndef EXPORT
#define EXPORT extern "C"
#endif

static void Init()
{
}

void Notify(const char* a_fmt, ...)
{
}
#endif

#ifndef VM_PROT_NONE
#define	VM_PROT_NONE		0x00
#endif

#ifndef VM_PROT_READ
#define VM_PROT_READ		0x01
#endif

#ifndef VM_PROT_WRITE
#define VM_PROT_WRITE		0x02
#endif

#ifndef VM_PROT_EXECUTE
#define VM_PROT_EXECUTE		0x04
#endif

#ifndef VM_PROT_DEFAULT
#define VM_PROT_DEFAULT		(VM_PROT_READ | VM_PROT_WRITE)
#endif

#ifndef VM_PROT_ALL
#define VM_PROT_ALL			(VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)
#endif

#ifndef NAKED
#define NAKED __attribute__((naked))
#endif

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


#pragma region
#pragma pack(push, 1)
// 5
struct Assembly5
{
	uint8_t opcode;
	int32_t disp;
};
static_assert(offsetof(Assembly5, opcode) == 0);
static_assert(offsetof(Assembly5, disp) == 1);
static_assert(sizeof(Assembly5) == 5);


// 14
struct Assembly14
{
	uint8_t  jmp;
	uint8_t  modrm;
	uint32_t disp;
	uint64_t addr;
};
static_assert(offsetof(Assembly14, jmp) == 0x0);
static_assert(offsetof(Assembly14, modrm) == 0x1);
static_assert(offsetof(Assembly14, disp) == 0x2);
static_assert(offsetof(Assembly14, addr) == 0x6);
static_assert(sizeof(Assembly14) == 14);
#pragma pack(pop)
#pragma endregion

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

NAKED int func(int handle)
{
	__asm
	(
		".intel_syntax noprefix\n"
		"mov esi, 2\n"
		"call rax\n"
		"ret"
	);
}

void try_patch(uintptr_t a_src, Assembly14* mem)
{
	// we'll create this then copy it into the scee prx's memory.
	Assembly5  sceVideoOutPrxCall;
	Assembly14 sceVideoOutTrampoline;

	//
	auto disp = reinterpret_cast<const uint8_t*>(mem) - reinterpret_cast<const uint8_t*>(a_src + sizeof(Assembly5));
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
		safe_write((uint64_t)mem, &sceVideoOutTrampoline, sizeof(sceVideoOutTrampoline));
	}
}

static void patch_db()
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
		try_patch((uintptr_t)ptr, (Assembly14*)freeScan);
	}
	else
	{
		Notify("Failed to find ASM/free space! aborting.");
	}
}

static void patch_xor()
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

EXPORT int module_start(size_t argc, const void* argv)
{
	//
	Init();

	//
	Notify("Hello from PRX!.");

	switch (PATCH_MODE)
	{
	case PATCH_XOR:
		patch_xor();
		break;
	case PATCH_DB:
		patch_db();
		break;
	}

	return 0;
}

EXPORT int module_stop(size_t argc, const void* argv) { return 0; }