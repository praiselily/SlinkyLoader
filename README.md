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
> DLLs provided by Pluto Solutions — Made with love by lily

---

## What is SlinkyLoader?

SlinkyLoader is a lightweight DLL injector built for use with Pluto Solutions modules. It automatically handles downloading, setup, and injection — just launch it and let it do its thing.

It uses **LoadLibrary injection via CreateRemoteThread**, the target process loads the DLLs natively through its own `LoadLibraryA` call.

---

## Features

- Automatically locates a running `javaw.exe` process and injects without any input required
- Falls back to manual PID entry if no `javaw.exe` is found
- Downloads missing DLLs from Pluto Solutions automatically
- Adds a Windows Defender exclusion for the current directory before downloading to prevent false positive detections
- Colored console UI with clean minimal output

---

## Requirements

- Windows 10 / 11
- Run as **Administrator** (required for Defender exclusion and process injection)
- A running `javaw.exe` process (or any target process PID)
- Internet connection (only needed if DLLs are not already present in the directory)

---

## Usage

1. Download the latest `injector.exe` from [Releases](https://github.com/praiselily/SlinkyLoader/releases)
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

This tool is intended for personal and legitimate modding use only. Usage against online services in violation of their Terms of Service is your own responsibility.
