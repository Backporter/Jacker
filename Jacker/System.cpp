#include "System.h"

#if __ORBIS__
int(*_sceKernelSendNotificationRequest)(int, OrbisNotificationRequest*, size_t, int);

int64_t sceKernelSendNotificationRequest(int device, OrbisNotificationRequest* req, size_t size, int blocking)
{
	if (_sceKernelSendNotificationRequest)
		return _sceKernelSendNotificationRequest(device, req, size, blocking);
}
#endif

void Init()
{
#if __ORBIS__
	auto libkernelhandle = sceKernelLoadStartModule("libkernel.sprx", 0, 0, 0, 0, 0);
	if (libkernelhandle > 0)
	{
		sceKernelDlsym(libkernelhandle, "sceKernelSendNotificationRequest", (void**)&_sceKernelSendNotificationRequest);
	}
#endif
}