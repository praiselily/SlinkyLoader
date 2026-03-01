# SlinkyLoader
```
      __    __
      \/----\/
       \0  0/    WOOF!
       _\  /_
     _|  \/  |_
    | | |  | | |
   _| | |  | | |_
  "---|_|--|_|---"

            - SLINKY!
```
> Cracks were made by Pluto Solutions, all credits to them. 
> https://t.me/s/plutosolutions
---

## What is SlinkyLoader?

SlinkyLoader is a lightweight DLL injector built for use with Pluto Solutions modules. It automatically handles downloading, setup, and injection

It uses **LoadLibrary injection via CreateRemoteThread**, the target process loads the DLLs natively through its own `LoadLibraryA` call.

---

## Features

- Automatically locates a running `javaw.exe` process and injects without any input required
- Falls back to manual PID entry if no `javaw.exe` is found
- Downloads missing DLLs from Pluto Solutions automatically
- Adds a Windows Defender exclusion for the current directory before downloading to prevent false positive detections
---

## Requirements

- Windows 10 / 11
- Run as **Administrator** (required for Defender exclusion and process injection)
- A running `javaw.exe` process (or any target process PID)
- Internet connection (only needed if DLLs are not already present in the directory)

---

## Usage

1. Download the latest `SlinkyLoader.exe` from [Releases](https://github.com/praiselily/SlinkyLoader/releases/tag/Cracked)
2. Place it anywhere on your system
3. **Right click → Run as Administrator**
4. If prompted with a UAC dialog, accept it — this is for the Defender exclusion
5. If `javaw.exe` is running it will inject automatically. If not, enter the target PID when prompted

---

## Notes

- The DLLs (`slinky_library.dll` and `slinkyhook.dll`) will be downloaded into the same directory as the injector on first run and reused on subsequent runs
- Windows Defender may flag the DLLs as suspicious — this is a false positive. The Defender exclusion added on launch prevents this
- Make sure your target process and the injector are the same architecture (both 64-bit)

---

## Building from Source

Requires **MinGW-w64 (g++)**:

```bash
g++ -o injector.exe injector.cpp -lurlmon -lshell32 -static -m64 -Wl,--subsystem,console
```

---

## Disclaimer

I do no own this crack. I believe it's safe for use as i haven't had any issues with it, but use is at own risk. Any harm that may happen to your device upon utilizing this crack is done so at own digression. 
If you are pluto solutions and want this repository taken down, reach me on discord @praiselily.
