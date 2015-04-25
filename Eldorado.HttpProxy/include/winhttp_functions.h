#pragma once
#include <Windows.h>

// WINHTTP_QUERY_RAW_HEADERS
#define WINHTTP_QUERY_CONTENT_LENGTH               5
#define WINHTTP_QUERY_URI                          13
#define WINHTTP_QUERY_STATUS_CODE                  19  
#define WINHTTP_QUERY_STATUS_TEXT                  20 
#define WINHTTP_QUERY_RAW_HEADERS_CRLF             22 

typedef LPVOID HINTERNET;

typedef WORD INTERNET_PORT;

typedef
VOID
(CALLBACK * WINHTTP_STATUS_CALLBACK)(
IN HINTERNET hInternet,
IN DWORD_PTR dwContext,
IN DWORD dwInternetStatus,
IN LPVOID lpvStatusInformation OPTIONAL,
IN DWORD dwStatusInformationLength
);

typedef WINHTTP_STATUS_CALLBACK * LPWINHTTP_STATUS_CALLBACK;

// winhttp function types
typedef BOOL(WINAPI* WinHttpCloseHandleFunction)(
	IN HINTERNET hInternet
	);

typedef HINTERNET(WINAPI* WinHttpConnectFunction)(
	IN HINTERNET hSession,
	IN LPCWSTR pswzServerName,
	IN INTERNET_PORT nServerPort,
	IN DWORD dwReserved
	);

typedef HINTERNET(WINAPI* WinHttpOpenFunction)(
	_In_opt_z_ LPCWSTR pszAgentW,
	_In_ DWORD dwAccessType,
	_In_opt_z_ LPCWSTR pszProxyW,
	_In_opt_z_ LPCWSTR pszProxyBypassW,
	_In_ DWORD dwFlags
	);

typedef HINTERNET(WINAPI* WinHttpOpenRequestFunction)(
	IN HINTERNET hConnect,
	IN LPCWSTR pwszVerb,
	IN LPCWSTR pwszObjectName,
	IN LPCWSTR pwszVersion,
	IN LPCWSTR pwszReferrer OPTIONAL,
	IN LPCWSTR FAR * ppwszAcceptTypes OPTIONAL,
	IN DWORD dwFlags
	);

typedef BOOL(WINAPI* WinHttpQueryHeadersFunction)(
	IN     HINTERNET hRequest,
	IN     DWORD     dwInfoLevel,
	IN     LPCWSTR   pwszName OPTIONAL,
	_Out_writes_bytes_to_opt_(*lpdwBufferLength, *lpdwBufferLength) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN OUT LPDWORD   lpdwBufferLength,
	IN OUT LPDWORD   lpdwIndex OPTIONAL
	);

typedef BOOL(WINAPI* WinHttpQueryOptionFunction)(
	IN HINTERNET hInternet,
	IN DWORD dwOption,
	_Out_writes_bytes_to_opt_(*lpdwBufferLength, *lpdwBufferLength) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN OUT LPDWORD lpdwBufferLength
	);

typedef BOOL(WINAPI* WinHttpReadDataFunction)(
	IN HINTERNET hRequest,
	_Out_writes_bytes_to_(dwNumberOfBytesToRead, *lpdwNumberOfBytesRead) __out_data_source(NETWORK) LPVOID lpBuffer,
	IN DWORD dwNumberOfBytesToRead,
	OUT LPDWORD lpdwNumberOfBytesRead
	);

typedef BOOL(WINAPI* WinHttpReceiveResponseFunction)(
	IN HINTERNET hRequest,
	IN LPVOID lpReserved
	);
	
typedef BOOL(WINAPI* WinHttpSendRequestFunction)(
	IN HINTERNET hRequest,
	_In_reads_opt_(dwHeadersLength) LPCWSTR lpszHeaders,
	IN DWORD dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID lpOptional,
	IN DWORD dwOptionalLength,
	IN DWORD dwTotalLength,
	IN DWORD_PTR dwContext
	);

typedef WINHTTP_STATUS_CALLBACK(WINAPI* WinHttpSetStatusCallbackFunction)(
	IN HINTERNET hInternet,
	IN WINHTTP_STATUS_CALLBACK lpfnInternetCallback,
	IN DWORD dwNotificationFlags,
	IN DWORD_PTR dwReserved
	);

typedef BOOL(WINAPI* WinHttpSetTimeoutsFunction)(
	IN HINTERNET    hInternet,
	IN int          nResolveTimeout,
	IN int          nConnectTimeout,
	IN int          nSendTimeout,
	IN int          nReceiveTimeout
	);