#include <string>

#include <Windows.h>
#include <tchar.h>
#include "Logger.h"

#include "winhttp_functions.h"

// globals

HINSTANCE g_proxy_dll_hInstance;
HINSTANCE g_original_dll_hInstance;
WinHttpCloseHandleFunction oWinHttpCloseHandle;
WinHttpConnectFunction oWinHttpConnect;
WinHttpOpenFunction oWinHttpOpen;
WinHttpOpenRequestFunction oWinHttpOpenRequest;
WinHttpQueryHeadersFunction oWinHttpQueryHeaders;
WinHttpQueryOptionFunction oWinHttpQueryOption;
WinHttpReadDataFunction oWinHttpReadData;
WinHttpReceiveResponseFunction oWinHttpReceiveResponse;
WinHttpSendRequestFunction oWinHttpSendRequest;
WinHttpSetStatusCallbackFunction oWinHttpSetStatusCallback;
WinHttpSetTimeoutsFunction oWinHttpSetTimeouts;

// prototypes

Logger& theLogger();

// winhttp exports

BOOL WINAPI WinHttpCloseHandle(
	IN HINTERNET hInternet
	)
{
	theLogger().LogFormat(_T("Calling WinHttpCloseHandle(%x)"), hInternet);
	return oWinHttpCloseHandle(hInternet);
}

HINTERNET WINAPI WinHttpConnect(
	IN HINTERNET hSession,
	IN LPCWSTR pswzServerName,
	IN INTERNET_PORT nServerPort,
	IN DWORD dwReserved
	)
{
	theLogger().LogFormat(_T("Calling WinHttpConnect(%x, %s, %d)"), hSession, pswzServerName, nServerPort);
	HINTERNET result = oWinHttpConnect(hSession, pswzServerName, nServerPort, dwReserved);
	theLogger().LogFormat(_T("WinHttpConnect(%x, %s, %d) returned %x"), hSession, pswzServerName, nServerPort, result);
	return result;
}

HINTERNET WINAPI WinHttpOpen(
	_In_opt_z_ LPCWSTR pszAgentW,
	_In_ DWORD dwAccessType,
	_In_opt_z_ LPCWSTR pszProxyW,
	_In_opt_z_ LPCWSTR pszProxyBypassW,
	_In_ DWORD dwFlags
	)
{
	theLogger().LogFormat(_T("Calling WinHttpOpen(...)"));
	HINTERNET result = oWinHttpOpen(pszAgentW, dwAccessType, pszProxyW, pszProxyBypassW, dwFlags);
	theLogger().LogFormat(_T("WinHttpOpen(...) returned %x"), result);
	return result;
}

HINTERNET WINAPI WinHttpOpenRequest(
	IN HINTERNET hConnect,
	IN LPCWSTR pwszVerb,
	IN LPCWSTR pwszObjectName,
	IN LPCWSTR pwszVersion,
	IN LPCWSTR pwszReferrer OPTIONAL,
	IN LPCWSTR FAR * ppwszAcceptTypes OPTIONAL,
	IN DWORD dwFlags
	)
{
	theLogger().LogFormatW(
		L"Calling WinHttpOpenRequest(%x, %s, %s, %s, %s, %s, %d)",
		hConnect,
		pwszVerb,
		pwszObjectName,
		pwszVersion,
		pwszReferrer,
		ppwszAcceptTypes,
		dwFlags);
	HINTERNET result = oWinHttpOpenRequest(
		hConnect,
		pwszVerb,
		pwszObjectName,
		pwszVersion,
		pwszReferrer,
		ppwszAcceptTypes,
		dwFlags);
	theLogger().LogFormat(_T("WinHttpOpenRequest(%x, ...) returned %x"), hConnect, result);
	return result;
}

BOOL WINAPI WinHttpQueryHeaders(
	IN     HINTERNET hRequest,
	IN     DWORD     dwInfoLevel,
	IN     LPCWSTR   pwszName OPTIONAL,
	_Out_writes_bytes_to_opt_(*lpdwBufferLength, *lpdwBufferLength) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN OUT LPDWORD   lpdwBufferLength,
	IN OUT LPDWORD   lpdwIndex OPTIONAL
	)
{
	theLogger().LogFormatW(L"Calling WinHttpQueryHeaders(%x, %d, %s, ...)", hRequest, dwInfoLevel, pwszName);
	BOOL result = oWinHttpQueryHeaders(hRequest, dwInfoLevel, pwszName, lpBuffer, lpdwBufferLength, lpdwIndex);

	std::wstring headerName;
	std::wstring headerValue;
	switch (dwInfoLevel & 0xFFFF)
	{
	case WINHTTP_QUERY_CONTENT_LENGTH:
		headerName = L"ContentLength";
		headerValue = std::to_wstring(*static_cast<int*>(lpBuffer));
		break;
	case WINHTTP_QUERY_STATUS_CODE:
		headerName = L"StatusCode";
		headerValue = std::to_wstring(*static_cast<int*>(lpBuffer));
		break;
	case WINHTTP_QUERY_STATUS_TEXT:
		headerName = L"StatusText";
		headerValue = std::wstring(static_cast<wchar_t*>(lpBuffer), (*lpdwBufferLength) / sizeof(wchar_t));
		break;
	case WINHTTP_QUERY_RAW_HEADERS_CRLF:
		headerName = L"AllHeaders";
		headerValue = std::wstring(static_cast<wchar_t*>(lpBuffer), (*lpdwBufferLength) / sizeof(wchar_t));
		break;
	default:
		break;
	}
	theLogger().LogFormatW(
		L"WinHttpQueryHeaders(%x, %d, ... ) returned: %s: %s",
		hRequest, dwInfoLevel,
		headerName.c_str(),
		headerValue.c_str());
	return result;
}

BOOL WINAPI WinHttpQueryOption(
	IN HINTERNET hInternet,
	IN DWORD dwOption,
	_Out_writes_bytes_to_opt_(*lpdwBufferLength, *lpdwBufferLength) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN OUT LPDWORD lpdwBufferLength
	)
{
	theLogger().LogFormat(_T("Calling WinHttpQueryOption(%x, %d, ...)"), hInternet, dwOption);
	return oWinHttpQueryOption(hInternet, dwOption, lpBuffer, lpdwBufferLength);
}

BOOL WINAPI WinHttpReadData(
	IN HINTERNET hRequest,
	_Out_writes_bytes_to_(dwNumberOfBytesToRead, *lpdwNumberOfBytesRead) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN DWORD dwNumberOfBytesToRead,
	OUT LPDWORD lpdwNumberOfBytesRead
	)
{
	theLogger().LogFormat(_T("Calling WinHttpReadData(%x, ...)"), hRequest);
	BOOL result = oWinHttpReadData(hRequest, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
	std::string readData = std::string(static_cast<char*>(lpBuffer), dwNumberOfBytesToRead);
	theLogger().LogFormatA("WinHttpReadData(%x, ...) returend: %s", hRequest, readData.c_str());
	return result;
}

BOOL WINAPI WinHttpReceiveResponse(
	IN HINTERNET hRequest,
	IN LPVOID lpReserved
	)
{
	theLogger().LogFormat(_T("Calling WinHttpReceiveResponse(%x, ...)"), hRequest);
	return oWinHttpReceiveResponse(hRequest, lpReserved);
}

BOOL WINAPI WinHttpSendRequest(
	IN HINTERNET hRequest,
	_In_reads_opt_(dwHeadersLength) LPCWSTR lpszHeaders,
	IN DWORD dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID lpOptional,
	IN DWORD dwOptionalLength,
	IN DWORD dwTotalLength,
	IN DWORD_PTR dwContext
	)
{
	theLogger().LogFormat(_T("Calling WinHttpSendRequest(%x, ...)"), hRequest);

	std::wstring header = dwHeadersLength == -1 ? std::wstring(lpszHeaders) : std::wstring(lpszHeaders, dwHeadersLength);
	theLogger().LogFormatW(L"WinHttpSendRequest(%x, ...) called with header: %s", hRequest, header.c_str());

	char* pBody = static_cast<char*>(lpOptional);
	std::string body = dwOptionalLength == -1 ? std::string(pBody) : std::string(pBody, dwOptionalLength);
	theLogger().LogFormatA("WinHttpSendRequest(%x, ...) called with body: %s", hRequest, body.c_str());

	return oWinHttpSendRequest(
		hRequest,
		lpszHeaders,
		dwHeadersLength,
		lpOptional,
		dwOptionalLength,
		dwTotalLength,
		dwContext);
}

WINHTTP_STATUS_CALLBACK WINAPI WinHttpSetStatusCallback(
	IN HINTERNET hInternet,
	IN WINHTTP_STATUS_CALLBACK lpfnInternetCallback,
	IN DWORD dwNotificationFlags,
	IN DWORD_PTR dwReserved
	)
{
	theLogger().LogFormat(_T("Calling WinHttpSetStatusCallback(%x, ...)"), hInternet);
	return oWinHttpSetStatusCallback(hInternet, lpfnInternetCallback, dwNotificationFlags, dwReserved);
}

BOOL WINAPI WinHttpSetTimeouts(
	IN HINTERNET hInternet,
	IN int nResolveTimeout,
	IN int nConnectTimeout,
	IN int nSendTimeout,
	IN int nReceiveTimeout
	)
{
	theLogger().LogFormat(
		_T("Calling WinHttpSetTimeouts(%x, %d, %d, %d, %d)"),
		hInternet,
		nResolveTimeout,
		nConnectTimeout,
		nSendTimeout,
		nReceiveTimeout);
	return oWinHttpSetTimeouts(hInternet, nResolveTimeout, nConnectTimeout, nSendTimeout, nReceiveTimeout);
}

// dll functions

Logger& theLogger() {
	static Logger logger = Logger(_T("winhttp.log"));
	return logger;
}

void Exit(TCHAR* reason)
{
	theLogger().LogFormat(_T("Exit called with reason (%s)"), reason);
	ExitProcess(1);
}

void Exit()
{
	Exit(_T(""));
}

template<typename FunctionPointerType>
void SetOriginalDllFunctionAddress(const CHAR* originalFunctionName, FunctionPointerType &pFunctionPointer)
{
	FARPROC originalFunctionAddress = GetProcAddress(g_original_dll_hInstance, originalFunctionName);
	if (!originalFunctionAddress)
	{
		Exit(_T("Unable to get an original function address"));
	}
	pFunctionPointer = reinterpret_cast<FunctionPointerType>(originalFunctionAddress);
}

void InitializeOriginalDllFunctions()
{
	theLogger().LogFormat(_T("Initializing original function addresses"));
	SetOriginalDllFunctionAddress("WinHttpCloseHandle", oWinHttpCloseHandle);
	SetOriginalDllFunctionAddress("WinHttpConnect", oWinHttpConnect);
	SetOriginalDllFunctionAddress("WinHttpOpen", oWinHttpOpen);
	SetOriginalDllFunctionAddress("WinHttpOpenRequest", oWinHttpOpenRequest);
	SetOriginalDllFunctionAddress("WinHttpQueryHeaders", oWinHttpQueryHeaders);
	SetOriginalDllFunctionAddress("WinHttpQueryOption", oWinHttpQueryOption);
	SetOriginalDllFunctionAddress("WinHttpReadData", oWinHttpReadData);
	SetOriginalDllFunctionAddress("WinHttpReceiveResponse", oWinHttpReceiveResponse);
	SetOriginalDllFunctionAddress("WinHttpSendRequest", oWinHttpSendRequest);
	SetOriginalDllFunctionAddress("WinHttpSetStatusCallback", oWinHttpSetStatusCallback);
	SetOriginalDllFunctionAddress("WinHttpSetTimeouts", oWinHttpSetTimeouts);
	theLogger().LogFormat(_T("Original function addresses initialized"));
}

HMODULE LoadOriginalDll()
{
	theLogger().LogFormat(_T("Loading the original winhttp.dll"));
	TCHAR original_dll_path_buffer[MAX_PATH];
	GetSystemDirectory(original_dll_path_buffer, MAX_PATH);
	_tcscat_s(original_dll_path_buffer, MAX_PATH, _T("\\winhttp.dll"));

	HMODULE original_dll_hInstance = LoadLibrary(original_dll_path_buffer);
	if (!original_dll_hInstance)
	{
		Exit(_T("Unable to load the original winhttp.dll"));
	}

	theLogger().LogFormat(_T("Original winhttp.dll was loaded successfully"));
	return original_dll_hInstance;
}

void InitializeDll(HINSTANCE hInstance)
{
	theLogger().LogFormat(_T("("###############################################"));
	theLogger().LogFormat(_T("Initializing the winhttp proxy dll"));
	DisableThreadLibraryCalls(hInstance);
	g_proxy_dll_hInstance = hInstance;
	g_original_dll_hInstance = LoadOriginalDll();
	InitializeOriginalDllFunctions();
	theLogger().LogFormat(_T("Proxy dll initialized"));
	theLogger().LogFormat(_T("("###############################################"));
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID /*reserved*/)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		InitializeDll(hInstance);
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		return true;
	}

	return false;
}