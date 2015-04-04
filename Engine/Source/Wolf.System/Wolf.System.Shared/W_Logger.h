﻿/*
	Project			 : Wolf Engine (http://WolfStudio.co). Copyright(c) Pooya Eimandar (http://PooyaEimandar.com) . All rights reserved.
	Source			 : https://github.com/PooyaEimandar/WolfEngine - Please direct any bug to hello@WolfStudio.co or tweet @PooyaEimandar on twitter
	Name			 : W_Logger.h
	Description		 : Log services which handle shared log over dlls
	Comment          :
*/

#pragma once

#include "W_Object.h"
#include <typeinfo>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

#if defined(__WIN32) 
#include "W_IO.h"
#endif

#if defined(__MAYA)
#include <maya/MGlobal.h>
#endif

inline std::string GetTime()
{
	char szBuffer[80] = "DD-MM-YY HH:MM:SS";
	struct tm _time;
	time_t rawtime;
	time(&rawtime);

	errno_t err = localtime_s(&_time, &rawtime);
	if (err)
	{
		std::string Msg = "Could not get the time from localtime_s"; 
#if defined(MAYA)
		MGlobal::displayError(Msg.c_str());
#else
		Msg += "\r\n";
		OutputDebugStringA(Msg.c_str());
#endif
	}
	else
	{
		strftime(szBuffer, sizeof(szBuffer), "%d-%b-%Y %X%p", &_time);
	}

	return std::string(szBuffer);
}

namespace Wolf
{
	namespace System
	{
		class W_Logger
		{
		public:
			//Initialize the logger and create a log file inside a Log folder into output directory
			API bool Initialize(std::string	pAppName);
			//Write and output message
			API void Write(std::wstring pMsg, std::string pState = "Info");
			//Write and output message
			API void Write(std::string pMsg, std::string pState = "Info");
			//Write an output warning message 
			API void Warning(std::string pMsg);
			//Write an output error message 
			API void Error(std::string pMsg);
			//Release all resources and close the log file
			API ULONG Release();

		private:
			bool isReleased;
			std::wofstream logFile;
		};
	}
}

extern API Wolf::System::W_Logger Logger;

inline void OnFailed(HRESULT pHR, std::string pMSG = "Undefined Error",
	std::string pTrace = "Undefined Trace", bool pExitNow = false, bool pCheckForLastDirectXError = false)
{
	using namespace std;

	if (pHR == S_OK) return;

	Logger.Write("+++++++  ERROR  +++++++");

	string errorMsg = "";
	if (pCheckForLastDirectXError)
	{
		auto err = GetLastError();
		string  errorMsg = "Error on " + pMSG + " with the following error info : ";
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
#if defined(__MAYA) || defined(__WIN32)
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
#endif
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			auto lpMsgStr = (LPCSTR)lpMsgBuf;
			string result(lpMsgStr, lpMsgStr + bufLen);
#if defined(__MAYA) || defined(__WIN32)
			LocalFree(lpMsgBuf);
#endif
			errorMsg += result;
		}

		errorMsg += "Trace info " + pTrace + ".";
	}
	else
	{
		errorMsg = "Error on " + pMSG + " with the following error info : " + "Trace info " + pTrace + ".";
	}

	Logger.Write(errorMsg);
	if (pExitNow)
	{
		abort();
	}
}

inline void DebugTrace(_In_z_ _Printf_format_string_ const char* format, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, format);

	char buff[1024] = { 0 };
	vsprintf_s(buff, format, args);
	OutputDebugStringA(buff);
#else
	UNREFERENCED_PARAMETER(format);
#endif
}