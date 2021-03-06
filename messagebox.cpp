// A simple program whose sole job is to execute modal MessageBox()/MessageBeep() API calls and returning the result.
// Useful for notifying the user and/or asking Yes/No/FileNotFound questions.  Limited to what MessageBox() can do.
//
// (C) 2019 CubicleSoft.  All Rights Reserved.

// Implemented as a single file compilation unit.

#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS

#ifdef _MBCS
#undef _MBCS
#endif

#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <tchar.h>

typedef int (__stdcall *MessageBoxTimeoutFunc)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds);

#ifdef SUBSYSTEM_WINDOWS
// If the caller is a console application and is waiting for this application to complete, then attach to the console.
void InitVerboseMode(void)
{
	if (::AttachConsole(ATTACH_PARENT_PROCESS))
	{
		if (::GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
		{
			freopen("CONOUT$", "w", stdout);
			setvbuf(stdout, NULL, _IONBF, 0);
		}

		if (::GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
		{
			freopen("CONOUT$", "w", stderr);
			setvbuf(stderr, NULL, _IONBF, 0);
		}
	}
}
#endif

void DumpSyntax(TCHAR *currfile)
{
#ifdef SUBSYSTEM_WINDOWS
	InitVerboseMode();
#endif

	_tprintf(_T("(C) 2021 CubicleSoft.  All Rights Reserved.\n\n"));

	_tprintf(_T("Syntax:  %s [options] [Text [Caption/Title]]\n\n"), currfile);

	_tprintf(_T("Options:\n"));

	_tprintf(_T("\t/v\n\
\tVerbose mode.\n\
\n\
\t/p\n\
\tParse Text for special escape sequences.\n\
\tOnly '\\n' and '\\\\' are supported.\n\
\n\
\t/f=Buttons\n\
\tSets the buttons of the message box.\n\
\tThe 'Buttons' can be one of:\n\
\tMB_ABORTRETRYIGNORE\n\
\tMB_CANCELTRYCONTINUE\n\
\tMB_OK (Default)\n\
\tMB_OKCANCEL\n\
\tMB_RETRYCANCEL\n\
\tMB_YESNO\n\
\tMB_YESNOCANCEL\n\
\n\
\t/f=Icon\n\
\tSets the icon of the message box.\n\
\tThe 'Icon' can be one of:\n\
\tMB_ICONERROR\n\
\tMB_ICONWARNING\n\
\tMB_ICONINFORMATION\n\
\tMB_ICONQUESTION\n\
\n\
\t/f=DefaultButton\n\
\tSets the default button for the message box.\n\
\tThe 'DefaultButton' can be one of:\n\
\tMB_DEFBUTTON1 (Default)\n\
\tMB_DEFBUTTON2\n\
\tMB_DEFBUTTON3\n\
\tMB_DEFBUTTON4\n\
\n\
\t/f=Modality\n\
\tSets the modality for the message box.\n\
\tThe 'Modality' can be one of:\n\
\tMB_APPLMODAL (Default)\n\
\tMB_SYSTEMMODAL\n\
\tMB_TASKMODAL\n\
\n\
\t/f=MiscFlag\n\
\tSets the miscellaneous flags for the message box.\n\
\tMultiple /f options can be specified.\n\
\tEach 'MiscFlag' can be one of:\n\
\tMB_SIMPLEBEEP (Only when Title is not used)\n\
\tMB_HELP (Probably won't work)\n\
\tMB_DEFAULT_DESKTOP_ONLY\n\
\tMB_RIGHT\n\
\tMB_RTLREADING\n\
\tMB_SETFOREGROUND\n\
\tMB_TOPMOST\n\
\tMB_SERVICE_NOTIFICATION\n\
\n\
\t/w=Milliseconds\n\
\tThe amount of time, in milliseconds, to wait.\n\
\tThe default behavior is to wait indefinitely.\n\
\tThis feature relies on an undocumented Windows API.\n\n"));

#ifdef SUBSYSTEM_WINDOWS
	_tprintf(_T("\t/attach\n"));
	_tprintf(_T("\t\tAttempt to attach to a parent console if it exists.\n\n"));
#endif
}

int _tmain(int argc, TCHAR **argv)
{
	bool verbose = false;
	UINT buttons = MB_OK;
	UINT icon = 0;
	UINT defbutton = MB_DEFBUTTON1;
	UINT modality = MB_APPLMODAL;
	bool simplebeep = false;
	bool parsetext = false;
	UINT flags = 0;
	HMODULE modulehandle = NULL;
	DWORD waitamount = INFINITE;

	// Process command-line options.
	int x;
	for (x = 1; x < argc; x++)
	{
		if (!_tcsicmp(argv[x], _T("/v")))  verbose = true;
		else if (!_tcsicmp(argv[x], _T("/?")) || !_tcsicmp(argv[x], _T("/h")))
		{
			DumpSyntax(argv[0]);

			return 0;
		}
		else if (!_tcsicmp(argv[x], _T("/p")))  parsetext = true;
		else if (!_tcsicmp(argv[x], _T("/f=MB_ABORTRETRYIGNORE")))  buttons = MB_ABORTRETRYIGNORE;
		else if (!_tcsicmp(argv[x], _T("/f=MB_CANCELTRYCONTINUE")))  buttons = MB_CANCELTRYCONTINUE;
		else if (!_tcsicmp(argv[x], _T("/f=MB_OK")))  buttons = MB_OK;
		else if (!_tcsicmp(argv[x], _T("/f=MB_OKCANCEL")))  buttons = MB_OKCANCEL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_RETRYCANCEL")))  buttons = MB_RETRYCANCEL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_YESNO")))  buttons = MB_YESNO;
		else if (!_tcsicmp(argv[x], _T("/f=MB_YESNOCANCEL")))  buttons = MB_YESNOCANCEL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_ICONERROR")))  icon = MB_ICONERROR;
		else if (!_tcsicmp(argv[x], _T("/f=MB_ICONWARNING")))  icon = MB_ICONWARNING;
		else if (!_tcsicmp(argv[x], _T("/f=MB_ICONINFORMATION")))  icon = MB_ICONINFORMATION;
		else if (!_tcsicmp(argv[x], _T("/f=MB_ICONQUESTION")))  icon = MB_ICONQUESTION;
		else if (!_tcsicmp(argv[x], _T("/f=MB_DEFBUTTON1")))  defbutton = MB_DEFBUTTON1;
		else if (!_tcsicmp(argv[x], _T("/f=MB_DEFBUTTON2")))  defbutton = MB_DEFBUTTON2;
		else if (!_tcsicmp(argv[x], _T("/f=MB_DEFBUTTON3")))  defbutton = MB_DEFBUTTON3;
		else if (!_tcsicmp(argv[x], _T("/f=MB_DEFBUTTON4")))  defbutton = MB_DEFBUTTON4;
		else if (!_tcsicmp(argv[x], _T("/f=MB_APPLMODAL")))  modality = MB_APPLMODAL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_SYSTEMMODAL")))  modality = MB_SYSTEMMODAL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_TASKMODAL")))  modality = MB_TASKMODAL;
		else if (!_tcsicmp(argv[x], _T("/f=MB_SIMPLEBEEP")))  simplebeep = true;
		else if (!_tcsicmp(argv[x], _T("/f=MB_HELP")))  flags |= MB_HELP;
		else if (!_tcsicmp(argv[x], _T("/f=MB_DEFAULT_DESKTOP_ONLY")))  flags |= MB_DEFAULT_DESKTOP_ONLY;
		else if (!_tcsicmp(argv[x], _T("/f=MB_RIGHT")))  flags |= MB_RIGHT;
		else if (!_tcsicmp(argv[x], _T("/f=MB_RTLREADING")))  flags |= MB_RTLREADING;
		else if (!_tcsicmp(argv[x], _T("/f=MB_SETFOREGROUND")))  flags |= MB_SETFOREGROUND;
		else if (!_tcsicmp(argv[x], _T("/f=MB_TOPMOST")))  flags |= MB_TOPMOST;
		else if (!_tcsicmp(argv[x], _T("/f=MB_SERVICE_NOTIFICATION")))  flags |= MB_SERVICE_NOTIFICATION;
		else if (!_tcsncicmp(argv[x], _T("/w="), 3))  waitamount = _tstoi(argv[x] + 3);
		else if (!_tcsicmp(argv[x], _T("/attach")))
		{
#ifdef SUBSYSTEM_WINDOWS
			// For the Windows subsystem only, attempt to attach to a parent console if it exists.
			InitVerboseMode();
#endif
		}
		else
		{
			// Probably reached the command to execute portion of the arguments.
			break;
		}
	}

	if (verbose)
	{
#ifdef SUBSYSTEM_WINDOWS
		InitVerboseMode();
#endif

		_tprintf(_T("Arguments:\n"));
		for (int x2 = 0; x2 < argc; x2++)
		{
			_tprintf(_T("\targv[%d] = %s\n"), x2, argv[x2]);
		}
		_tprintf(_T("\n"));
	}

	// Display a message box or play a system beep sound.
	int result;
	if (x == argc)
	{
		if (simplebeep)  icon = 0xFFFFFFFF;

		if (verbose)
		{
			_tprintf(_T("MessageBeep(\n"));
			_tprintf(_T("\tuType = "));
			if (icon == MB_OK)  _tprintf(_T("MB_OK"));
			else if (icon == MB_ICONERROR)  _tprintf(_T("MB_ICONERROR"));
			else if (icon == MB_ICONWARNING)  _tprintf(_T("MB_ICONWARNING"));
			else if (icon == MB_ICONINFORMATION)  _tprintf(_T("MB_ICONINFORMATION"));
			else if (icon == MB_ICONQUESTION)  _tprintf(_T("MB_ICONQUESTION"));
			else  _tprintf(_T("0xFFFFFF (MB_SIMPLEBEEP)"));
			_tprintf(_T("\n"));
			_tprintf(_T(");\n"));
		}

		result = (int)::MessageBeep(icon);
	}
	else
	{
		LPCTSTR maintext = argv[x];
		LPCTSTR caption = (x + 1 < argc ? argv[x + 1] : NULL);

		// Handle easier parsing of newlines in the text.
		if (parsetext)
		{
			LPTSTR maintext2 = (LPTSTR)malloc(_tcslen(maintext) * sizeof(TCHAR));

			size_t x3 = 0;

			for (size_t x2 = 0; maintext[x2]; x2++)
			{
				if (maintext[x2] == _T('\\') && maintext[x2 + 1] == _T('n'))
				{
					x2++;

					maintext2[x3++] = _T('\n');
				}
				else if (maintext[x2] == _T('\\') && maintext[x2 + 1] == _T('\\'))
				{
					x2++;

					maintext2[x3++] = _T('\\');
				}
				else
				{
					maintext2[x3++] = maintext[x2];
				}
			}

			maintext2[x3] = _T('\0');

			maintext = maintext2;
		}

		if (verbose)
		{
			_tprintf(_T("MessageBox(\n"));
			_tprintf(_T("\thWnd = 0x%p,\n"), (void *)::GetConsoleWindow());
			_tprintf(_T("\tlpText = %s,\n"), maintext);
			_tprintf(_T("\tlpCaption = %s,\n"), (caption != NULL ? caption : _T("NULL")));
			_tprintf(_T("\tuType = "));
			if (buttons == MB_ABORTRETRYIGNORE)  _tprintf(_T("MB_ABORTRETRYIGNORE"));
			else if (buttons == MB_CANCELTRYCONTINUE)  _tprintf(_T("MB_CANCELTRYCONTINUE"));
			else if (buttons == MB_OK)  _tprintf(_T("MB_OK"));
			else if (buttons == MB_OKCANCEL)  _tprintf(_T("MB_OKCANCEL"));
			else if (buttons == MB_RETRYCANCEL)  _tprintf(_T("MB_RETRYCANCEL"));
			else if (buttons == MB_YESNO)  _tprintf(_T("MB_YESNO"));
			else if (buttons == MB_YESNOCANCEL)  _tprintf(_T("MB_YESNOCANCEL"));

			if (icon == MB_ICONERROR)  _tprintf(_T(" | MB_ICONERROR"));
			else if (icon == MB_ICONWARNING)  _tprintf(_T(" | MB_ICONWARNING"));
			else if (icon == MB_ICONINFORMATION)  _tprintf(_T(" | MB_ICONINFORMATION"));
			else if (icon == MB_ICONQUESTION)  _tprintf(_T(" | MB_ICONQUESTION"));

			if (defbutton == MB_DEFBUTTON1)  _tprintf(_T(" | MB_DEFBUTTON1"));
			else if (defbutton == MB_DEFBUTTON2)  _tprintf(_T(" | MB_DEFBUTTON2"));
			else if (defbutton == MB_DEFBUTTON3)  _tprintf(_T(" | MB_DEFBUTTON3"));
			else if (defbutton == MB_DEFBUTTON4)  _tprintf(_T(" | MB_DEFBUTTON4"));

			if (modality == MB_APPLMODAL)  _tprintf(_T(" | MB_APPLMODAL"));
			else if (modality == MB_SYSTEMMODAL)  _tprintf(_T(" | MB_SYSTEMMODAL"));
			else if (modality == MB_TASKMODAL)  _tprintf(_T(" | MB_TASKMODAL"));

			if (flags & MB_HELP)  _tprintf(_T(" | MB_HELP"));
			if (flags & MB_DEFAULT_DESKTOP_ONLY)  _tprintf(_T(" | MB_DEFAULT_DESKTOP_ONLY"));
			if (flags & MB_RIGHT)  _tprintf(_T(" | MB_RIGHT"));
			if (flags & MB_RTLREADING)  _tprintf(_T(" | MB_RTLREADING"));
			if (flags & MB_SETFOREGROUND)  _tprintf(_T(" | MB_SETFOREGROUND"));
			if (flags & MB_TOPMOST)  _tprintf(_T(" | MB_TOPMOST"));
			if (flags & MB_SERVICE_NOTIFICATION)  _tprintf(_T(" | MB_SERVICE_NOTIFICATION"));
			_tprintf(_T("\n"));
			_tprintf(_T(");\n"));
		}

		bool displayed = false;

		if (waitamount != INFINITE)
		{
			HMODULE modulehandle = ::LoadLibrary(_T("user32.dll"));

#ifdef UNICODE
			MessageBoxTimeoutFunc TempMBTPtr = (MessageBoxTimeoutFunc)::GetProcAddress(modulehandle, "MessageBoxTimeoutW");
#else
			MessageBoxTimeoutFunc TempMBTPtr = (MessageBoxTimeoutFunc)::GetProcAddress(modulehandle, "MessageBoxTimeoutA");
#endif

			if (TempMBTPtr != NULL)
			{
				result = TempMBTPtr(::GetConsoleWindow(), maintext, caption, buttons | icon | defbutton | modality | flags, 0, waitamount);

				displayed = true;
			}
		}

		if (!displayed)  result = ::MessageBox(::GetConsoleWindow(), maintext, caption, buttons | icon | defbutton | modality | flags);
	}

	if (!result)
	{
#ifdef SUBSYSTEM_WINDOWS
		InitVerboseMode();
#endif

		DWORD errnum = ::GetLastError();
		LPTSTR errmsg = NULL;

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errnum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errmsg, 0, NULL);
		if (x == argc)  _tprintf(_T("An error occurred while calling MessageBeep():\n"));
		else  _tprintf(_T("An error occurred while calling MessageBox():\n"));
		if (errmsg == NULL)  _tprintf(_T("%d - Unknown error\n\n"), errnum);
		else
		{
			_tprintf(_T("%d - %s\n"), errnum, errmsg);
			::LocalFree(errmsg);
		}
	}

	if (modulehandle != NULL)  ::FreeLibrary(modulehandle);

	return result;
}

#ifdef SUBSYSTEM_WINDOWS
#ifndef UNICODE
// Swiped from:  https://stackoverflow.com/questions/291424/canonical-way-to-parse-the-command-line-into-arguments-in-plain-c-windows-api
LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT *pNumArgs)
{
	int retval;
	retval = ::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, NULL, 0);
	if (!SUCCEEDED(retval))  return NULL;

	LPWSTR lpWideCharStr = (LPWSTR)malloc(retval * sizeof(WCHAR));
	if (lpWideCharStr == NULL)  return NULL;

	retval = ::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retval);
	if (!SUCCEEDED(retval))
	{
		free(lpWideCharStr);

		return NULL;
	}

	int numArgs;
	LPWSTR* args;
	args = ::CommandLineToArgvW(lpWideCharStr, &numArgs);
	free(lpWideCharStr);
	if (args == NULL)  return NULL;

	int storage = numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; i++)
	{
		BOOL lpUsedDefaultChar = FALSE;
		retval = ::WideCharToMultiByte(CP_ACP, 0, args[i], -1, NULL, 0, NULL, &lpUsedDefaultChar);
		if (!SUCCEEDED(retval))
		{
			::LocalFree(args);

			return NULL;
		}

		storage += retval;
	}

	LPSTR* result = (LPSTR *)::LocalAlloc(LMEM_FIXED, storage);
	if (result == NULL)
	{
		::LocalFree(args);

		return NULL;
	}

	int bufLen = storage - numArgs * sizeof(LPSTR);
	LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++ i)
	{
		BOOL lpUsedDefaultChar = FALSE;
		retval = ::WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
		if (!SUCCEEDED(retval))
		{
			::LocalFree(result);
			::LocalFree(args);

			return NULL;
		}

		result[i] = buffer;
		buffer += retval;
		bufLen -= retval;
	}

	::LocalFree(args);

	*pNumArgs = numArgs;
	return result;
}
#endif

int CALLBACK WinMain(HINSTANCE /* hInstance */, HINSTANCE /* hPrevInstance */, LPSTR lpCmdLine, int /* nCmdShow */)
{
	int argc;
	TCHAR **argv;
	int result;

#ifdef UNICODE
	argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
#else
	argv = CommandLineToArgvA(lpCmdLine, &argc);
#endif

	if (argv == NULL)  return 0;

	result = _tmain(argc, argv);

	::LocalFree(argv);

	return result;
}
#endif
