#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

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

void safe_write(uintptr_t dst, const void* src, size_t len);
void* byteScan(void* addr, uint8_t* pattern, size_t searchLen);
void try_patch(uintptr_t a_src, Assembly14* a_mem, uintptr_t func);