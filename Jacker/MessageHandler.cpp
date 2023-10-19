#include "MessageHandler.h"
#include "System.h"

#include <string.h>
#include <stdio.h>

/* this function is disabled because for whatever reason this will crash in goldhen. */
void Notify(const char* a_fmt, ...)
{
#if false
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
#endif
}