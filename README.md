MessageBox()/MessageBeep() Windows API Command-Line Utility
===========================================================

A complete, robust command-line utility to construct highly customized calls to the MessageBox() and MessageBeep() Windows APIs.  Released under a MIT or LGPL license.

This project is intended primarily for use from batch files (.bat) and other scripts to display a modal message dialog.  If it can be done with MessageBox(), it can be done with this command-line program.

Why would you need this?  To easily display native Windows message boxes from command-line scripts that also returns the user's response (i.e. which button was pressed).  Useful for handling critical application/script failures.

[![Donate](https://cubiclesoft.com/res/donate-shield.png)](https://cubiclesoft.com/donate/) [![Discord](https://img.shields.io/discord/777282089980526602?label=chat&logo=discord)](https://cubiclesoft.com/product-support/github/)

Features
--------

* Command-line action!
* Verbose mode tells you exactly how MessageBox() or MessageBeep() will be called.  No more guessing!
* Pre-built binaries using Visual Studio (statically linked C++ runtime, minimal file size of ~85K, direct Win32 API calls).
* Windows subsystem variant.
* Unicode support.
* Offers almost everything MessageBox() and MessageBeep() offers.
* Has a liberal open source license.  MIT or LGPL, your choice.
* Sits on GitHub for all of that pull request and issue tracker goodness to easily submit changes and ideas respectively.

Useful Information
------------------

Running the command with the `/?` option will display the options:

```
(C) 2021 CubicleSoft.  All Rights Reserved.

Syntax:  messagebox.exe [options] [Text [Caption/Title]]

Options:
        /v
        Verbose mode.

        /p
        Parse Text for special escape sequences.
        Only '\n' and '\\' are supported.

        /f=Buttons
        Sets the buttons of the message box.
        The 'Buttons' can be one of:
        MB_ABORTRETRYIGNORE
        MB_CANCELTRYCONTINUE
        MB_OK (Default)
        MB_OKCANCEL
        MB_RETRYCANCEL
        MB_YESNO
        MB_YESNOCANCEL

        /f=Icon
        Sets the icon of the message box.
        The 'Icon' can be one of:
        MB_ICONERROR
        MB_ICONWARNING
        MB_ICONINFORMATION
        MB_ICONQUESTION

        /f=DefaultButton
        Sets the default button for the message box.
        The 'DefaultButton' can be one of:
        MB_DEFBUTTON1 (Default)
        MB_DEFBUTTON2
        MB_DEFBUTTON3
        MB_DEFBUTTON4

        /f=Modality
        Sets the modality for the message box.
        The 'Modality' can be one of:
        MB_APPLMODAL (Default)
        MB_SYSTEMMODAL
        MB_TASKMODAL

        /f=MiscFlag
        Sets the miscellaneous flags for the message box.
        Multiple /f options can be specified.
        Each 'MiscFlag' can be one of:
        MB_SIMPLEBEEP (Only when Title is not used)
        MB_HELP (Probably won't work)
        MB_DEFAULT_DESKTOP_ONLY
        MB_RIGHT
        MB_RTLREADING
        MB_SETFOREGROUND
        MB_TOPMOST
        MB_SERVICE_NOTIFICATION

        /w=Milliseconds
        The amount of time, in milliseconds, to wait.
        The default behavior is to wait indefinitely.
        This feature relies on an undocumented Windows API.
```

Example with verbose output:

```
C:\>messagebox.exe /v /f=MB_OKCANCEL /f=MB_ICONWARNING "The application crashed.  See the log file for details.  Press OK to continue or Cancel to exit now." "Oh Dear!"
Arguments:
        argv[0] = messagebox.exe
        argv[1] = /v
        argv[2] = /f=MB_OKCANCEL
        argv[3] = /f=MB_ICONWARNING
        argv[4] = The application crashed.  See the log file for details.  Press OK to continue or Cancel to exit now.
        argv[5] = Oh Dear!

MessageBox(
        hWnd = 0x008F0D4A,
        lpText = The application crashed.  See the log file for details.  Press OK to continue or Cancel to exit now.,
        lpCaption = Oh Dear!,
        uType = MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON1 | MB_APPLMODAL
);
```

Which waits for the message box to be closed and returns the return code of the selected button.

Windows Subsystem Variant
-------------------------

While `messagebox.exe` is intended for use with console apps, `messagebox-win.exe` is intended for detached console and GUI applications.  Starting `messagebox.exe` in certain situations will briefly flash a console window before displaying the error message.  Calling `messagebox-win.exe` instead will no longer show the console window.

Why not just use `messagebox-win.exe`?  Since `messagebox-win.exe` starts as a Windows GUI application, there is the tendency for it to be run in the background and the message box that displays may not behave as a modal dialog.  The software is a little bit trickier to work with as a result.  It's also a few KB larger than `messagebox.exe`.

There is one additional option specifically for `messagebox-win.exe` called `/attach` which attempts to attach to the console of the parent process (if any).

Sources
-------

The MessageBox() and MessageBeep() APIs on MSDN Library have the intimate details on each option:

* https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-messagebox
* https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-messagebeep
