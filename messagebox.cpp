// A simple program whose sole job is to execute modal MessageBox()/MessageBeep() API calls and returning the result.
// Useful for notifying the user and/or asking Yes/No/FileNotFound questions.  Limited to what MessageBox() can do.
//
// (C) 2018 CubicleSoft.  All Rights Reserved.

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

void DumpSyntax(TCHAR *currfile)
{
	_tprintf(_T("(C) 2018 CubicleSoft.  All Rights Reserved.\n\n"));

	_tprintf(_T("Syntax:  %s [options] [Title [Caption]]\n\n"), currfile);

	_tprintf(_T("Options:\n"));

	_tprintf(_T("\t/v\n"));
	_tprintf(_T("\tVerbose mode.\n\n"));

	_tprintf(_T("\t/f=Buttons\n"));
	_tprintf(_T("\t\tSets the buttons of the message box.\n"));
	_tprintf(_T("\t\tThe 'Buttons' can be one of:\n"));
	_tprintf(_T("\t\tMB_ABORTRETRYIGNORE\n"));
	_tprintf(_T("\t\tMB_CANCELTRYCONTINUE\n"));
	_tprintf(_T("\t\tMB_OK (Default)\n"));
	_tprintf(_T("\t\tMB_OKCANCEL\n"));
	_tprintf(_T("\t\tMB_RETRYCANCEL\n"));
	_tprintf(_T("\t\tMB_YESNO\n"));
	_tprintf(_T("\t\tMB_YESNOCANCEL\n\n"));

	_tprintf(_T("\t/f=Icon\n"));
	_tprintf(_T("\t\tSets the icon of the message box.\n"));
	_tprintf(_T("\t\tThe 'Icon' can be one of:\n"));
	_tprintf(_T("\t\tMB_ICONERROR\n"));
	_tprintf(_T("\t\tMB_ICONWARNING\n"));
	_tprintf(_T("\t\tMB_ICONINFORMATION\n"));
	_tprintf(_T("\t\tMB_ICONQUESTION\n\n"));

	_tprintf(_T("\t/f=DefaultButton\n"));
	_tprintf(_T("\t\tSets the default button for the message box.\n"));
	_tprintf(_T("\t\tThe 'DefaultButton' can be one of:\n"));
	_tprintf(_T("\t\tMB_DEFBUTTON1 (Default)\n"));
	_tprintf(_T("\t\tMB_DEFBUTTON2\n"));
	_tprintf(_T("\t\tMB_DEFBUTTON3\n"));
	_tprintf(_T("\t\tMB_DEFBUTTON4\n\n"));

	_tprintf(_T("\t/f=Modality\n"));
	_tprintf(_T("\t\tSets the modality for the message box.\n"));
	_tprintf(_T("\t\tThe 'Modality' can be one of:\n"));
	_tprintf(_T("\t\tMB_APPLMODAL (Default)\n"));
	_tprintf(_T("\t\tMB_SYSTEMMODAL\n"));
	_tprintf(_T("\t\tMB_TASKMODAL\n\n"));

	_tprintf(_T("\t/f=MiscFlag\n"));
	_tprintf(_T("\t\tSets the miscellaneous flags for the message box.\n"));
	_tprintf(_T("\t\tMultiple /f options can be specified.\n"));
	_tprintf(_T("\t\tEach 'MiscFlag' can be one of:\n"));
	_tprintf(_T("\t\tMB_SIMPLEBEEP (Only when Title is not used)\n"));
	_tprintf(_T("\t\tMB_HELP (Probably won't work)\n"));
	_tprintf(_T("\t\tMB_DEFAULT_DESKTOP_ONLY\n"));
	_tprintf(_T("\t\tMB_RIGHT\n"));
	_tprintf(_T("\t\tMB_RTLREADING\n"));
	_tprintf(_T("\t\tMB_SETFOREGROUND\n"));
	_tprintf(_T("\t\tMB_TOPMOST\n"));
	_tprintf(_T("\t\tMB_SERVICE_NOTIFICATION\n\n"));
}

int _tmain(int argc, TCHAR **argv)
{
	bool verbose = false;
	UINT buttons = MB_OK;
	UINT icon = 0;
	UINT defbutton = MB_DEFBUTTON1;
	UINT modality = MB_APPLMODAL;
	bool simplebeep = false;
	UINT flags = 0;

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
		else
		{
			// Probably reached the command to execute portion of the arguments.
			break;
		}
	}

	if (verbose)
	{
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
		if (verbose)
		{
			_tprintf(_T("MessageBox(\n"));
			_tprintf(_T("\thWnd = 0x%p,\n"), (void *)::GetConsoleWindow());
			_tprintf(_T("\tlpText = %s,\n"), argv[x]);
			_tprintf(_T("\tlpCaption = %s,\n"), (x + 1 < argc ? argv[x + 1] : _T("NULL")));
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

		result = ::MessageBox(::GetConsoleWindow(), argv[x], (x + 1 < argc ? argv[x + 1] : NULL), buttons | icon | defbutton | modality | flags);
	}

	if (!result)
	{
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

	return result;
}
