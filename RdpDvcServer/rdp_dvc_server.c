
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/wnd.h>
#include <winpr/wlog.h>
#include <winpr/tchar.h>
#include <winpr/thread.h>
#include <winpr/wtsapi.h>
#include <winpr/stream.h>
#include <winpr/library.h>
#include <winpr/collections.h>

#include "rdp_dvc_server.h"

static wLog* g_Log = NULL;

const char* WM_WTS_STRINGS[] =
{
	"",
	"WTS_CONSOLE_CONNECT",
	"WTS_CONSOLE_DISCONNECT",
	"WTS_REMOTE_CONNECT",
	"WTS_REMOTE_DISCONNECT",
	"WTS_SESSION_LOGON",
	"WTS_SESSION_LOGOFF",
	"WTS_SESSION_LOCK",
	"WTS_SESSION_UNLOCK",
	"WTS_SESSION_REMOTE_CONTROL",
	"WTS_SESSION_CREATE",
	"WTS_SESSION_TERMINATE",
	""
};

LRESULT CALLBACK RdpDvcWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//printf("RdpDvcWindowProc: uMsg: 0x%04X\n", uMsg);

	switch (uMsg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_WTSSESSION_CHANGE:
			if (wParam && (wParam < 13))
			{
				printf("WM_WTSSESSION_CHANGE: %s\n", WM_WTS_STRINGS[wParam]);
			}
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

int rdp_dvc_server_register_session_notification()
{
	MSG msg;
	HWND hWnd;
	HMODULE hModule;
	HINSTANCE hInstance;
	WNDCLASSEX wndClassEx;

	hModule = GetModuleHandle(NULL);

	ZeroMemory(&wndClassEx, sizeof(WNDCLASSEX));
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = 0;
	wndClassEx.lpfnWndProc = RdpDvcWindowProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hModule;
	wndClassEx.hIcon = NULL;
	wndClassEx.hCursor = NULL;
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = _T("SessionNotificationMenu");
	wndClassEx.lpszClassName = _T("SessionNotificationClass");
	wndClassEx.hIconSm = NULL;

	if (!RegisterClassEx(&wndClassEx))
	{
		printf("RegisterClassEx failure\n");
		return -1;
	}

	hInstance = wndClassEx.hInstance;

	hWnd = CreateWindowEx(0, wndClassEx.lpszClassName,
		0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance, NULL);

	if (!hWnd)
	{
		printf("CreateWindowEx failure\n");
		return -1;
	}

	if (!WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_ALL_SESSIONS))
	{
		printf("WTSRegisterSessionNotification failure\n");
		return -1;
	}

	ZeroMemory(&msg, sizeof(msg));

#ifdef _WIN32
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif

	return 0;
}

int main(int argc, char** argv)
{
	DWORD error;
	BOOL bSuccess;
	BOOL blocking;
	BOOL overlapped;
	HANDLE hFile;
	HANDLE hEvent;
	HANDLE hServer;
	HANDLE hChannel;
	DWORD waitStatus;
	BYTE fillValue = 0;
	BYTE* pBuffer = NULL;
	DWORD bytesReturned = 0;

	hEvent = NULL;
	blocking = FALSE;
	overlapped = FALSE;

#ifdef _WIN32
	blocking = TRUE;
	overlapped = TRUE;
#endif

	hServer = WTS_CURRENT_SERVER_HANDLE;

	g_Log = WLog_Get("rdp.svc.server");

#ifdef _WIN32
	WLog_SetLogAppenderType(g_Log, WLOG_APPENDER_FILE);
	WLog_OpenAppender(g_Log);
#endif

	WLog_SetLogLevel(g_Log, WLOG_DEBUG);

	WLog_Print(g_Log, WLOG_DEBUG, "RdpDvc Channel Server Open");

	//rdp_dvc_server_register_session_notification();

	hChannel = WTSVirtualChannelOpenEx(WTS_CURRENT_SESSION, "RdpDvc", WTS_CHANNEL_OPTION_DYNAMIC);

	if (!hChannel)
	{
		fprintf(stderr, "WTSVirtualChannelOpenEx failed (GetLastError() = %d)\n", GetLastError());
		return 0;
	}

	if (overlapped)
	{
		if (!WTSVirtualChannelQuery(hChannel, WTSVirtualFileHandle, (PVOID*) &pBuffer, &bytesReturned) ||
				(bytesReturned != sizeof(HANDLE)))
		{
			fprintf(stderr, "WTSVirtualChannelQuery failed (GetLastError() = %d)\n", GetLastError());
			return 0;
		}

		bSuccess = DuplicateHandle(_GetCurrentProcess(), *((HANDLE*) pBuffer),
			_GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS);

		if (!bSuccess)
		{
			fprintf(stderr, "DuplicateHandle failed (GetLastError() = %d)\n", GetLastError());
			return 0;
		}

		WTSFreeMemory(pBuffer);
	}

	if (!blocking)
	{
		if (!WTSVirtualChannelQuery(hChannel, WTSVirtualEventHandle, (PVOID*) &pBuffer, &bytesReturned) ||
				(bytesReturned != sizeof(HANDLE)))
		{
			fprintf(stderr, "WTSVirtualChannelQuery failed (GetLastError() = %d)\n", GetLastError());
			return 0;
		}

		hEvent = *((HANDLE*) pBuffer);
		WTSFreeMemory(pBuffer);
	}

	while (1)
	{
		BOOL bSuccess;
		ULONG ulBytesWritten;
		BYTE writeBuffer[1024];
		ULONG ulBytesRead;
		BYTE readBuffer[8192];
		CHANNEL_PDU_HEADER channelPduHeader;

		FillMemory(writeBuffer, sizeof(writeBuffer), fillValue);
		fillValue = (fillValue + 1) % 0xFF;

		bSuccess = WTSVirtualChannelWrite(hChannel, (PCHAR) writeBuffer, sizeof(writeBuffer), &ulBytesWritten);

		if (!bSuccess)
		{
			fprintf(stderr, "WTSVirtualChannelWrite failed (GetLastError() = %d)\n", GetLastError());
			break;
		}

		fprintf(stderr, "WTSVirtualChannelWrite - %u bytes written\n", ulBytesWritten);

		if (!blocking)
		{
			while (1)
			{
				waitStatus = WaitForSingleObject(hEvent, 250);

				if (waitStatus != WAIT_TIMEOUT)
					break;
			}
		}

		ulBytesRead = 0;

		while (!ulBytesRead)
		{
			bSuccess = WTSVirtualChannelRead(hChannel, 100,
				(PCHAR) &channelPduHeader, sizeof(CHANNEL_PDU_HEADER), &ulBytesRead);

			error = GetLastError();

			if (error == ERROR_IO_INCOMPLETE)
				continue;

			if (!bSuccess && (error != ERROR_MORE_DATA))
			{
				fprintf(stderr, "WTSVirtualChannelRead failed (GetLastError() = %d)\n", error);
				break;
			}

			if (ulBytesRead && (ulBytesRead != sizeof(CHANNEL_PDU_HEADER)))
			{
				fprintf(stderr, "WTSVirtualChannelRead failed to read channel pdu header\n");
				break;
			}
		}

		bSuccess = WTSVirtualChannelRead(hChannel, 0, (PCHAR) readBuffer, channelPduHeader.length, &ulBytesRead);

		if (!bSuccess)
		{
			fprintf(stderr, "WTSVirtualChannelRead failed (GetLastError() = %d)\n", GetLastError());
			break;
		}

		if (ulBytesRead != channelPduHeader.length)
		{
			fprintf(stderr, "WTSVirtualChannelRead failed to read channel pdu payload\n");
			break;
		}

		fprintf(stderr, "WTSVirtualChannelRead - %u bytes read\n", ulBytesRead);

		Sleep(1000);
	}

	WTSVirtualChannelClose(hChannel);

	WLog_Print(g_Log, WLOG_DEBUG, "RdpDvc Channel Server Close");

	return 0;
}

