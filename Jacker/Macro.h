#pragma once

#if __ORBIS__
#ifndef EXPORT
#define EXPORT extern "C" __declspec (dllexport)
#endif
#else
#ifndef EXPORT
#define EXPORT extern "C"
#endif
#endif

#ifndef INTEL_SENTAX_ASM
#define INTEL_SENTAX_ASM(a) __asm(".intel_syntax noprefix\n" a);
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
