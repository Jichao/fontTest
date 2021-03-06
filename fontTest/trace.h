#pragma once

#ifndef _CRT_WIDE
#define __CRT_WIDE(_String) L ## _String
#define _CRT_WIDE(_String) __CRT_WIDE(_String)
#endif

#ifndef _CRT_APPEND
#define __CRT_APPEND(_Value1, _Value2) _Value1 ## _Value2
#define _CRT_APPEND(_Value1, _Value2) __CRT_APPEND(_Value1, _Value2)
#endif

#define __SHORT_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __WSHORT_FILE__ (wcsrchr(_CRT_WIDE(__FILE__), '\\') ? wcsrchr(_CRT_WIDE(__FILE__), '\\') + 1 : _CRT_WIDE(__FILE__))

void Trace(const char* fileName, const int line, const char* funName, const wchar_t* format, ...);
void Trace(const char* fileName, const int line, const char* funName, const char* format, ...);
void STrace(const wchar_t* format, ...);
void STrace(const char* format, ...);

#define FTRACE(format, ...) \
	do { \
	Trace(__SHORT_FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
				} while(0);

#define FTRACE_IF(cond, format, ...) \
{ \
	do { \
	if (cond) {\
	Trace(__SHORT_FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
				} \
				} while(0);}

#ifdef _DEBUG

#define TRACE(format, ...) \
	do { \
	Trace(__SHORT_FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
		} while(0);

#define TRACE_IF(cond, format, ...) \
{ \
	do { \
	if (cond) {\
	Trace(__SHORT_FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
		} \
		} while(0);}

#define STRACE(format, ...) \
	do { \
		STrace(format, __VA_ARGS__); \
			} while(0);

#define STRACE_IF(cond, format, ...) \
	do { \
		if (cond)  \
			STrace(format, __VA_ARGS__); \
			} while(0);


#define JJBREAK_IF(cond) \
{ \
	do { \
	if (cond) \
	__debugbreak(); \
	} while(0);}
#else

#define TRACE(format, ...) \
	do { \
	} while(0);

#define TRACE_IF(cond, format, ...) \
{ \
	do { \
	} while(0);}

#endif

