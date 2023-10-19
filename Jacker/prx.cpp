#include <cstdio>
#include <stdio.h>

#include "System.h"
#include "Macro.h"
#include "MessageHandler.h"
#include "Patches.h"

#define PATCH_XOR 0
#define PATCH_DB 1

#define PATCH_MODE PATCH_XOR

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