# Mariner

![Static Badge](https://img.shields.io/badge/visual_studio-2022-purple)
![Static Badge](https://img.shields.io/badge/c%2B%2B-green)
![Static Badge](https://img.shields.io/badge/roblox-red)
![Static Badge](https://img.shields.io/badge/2013-blue)

A dynamic-link library hook used to add/modify functionality within legacy ROBLOX game clients.  
Mariner has been tested to work on the late 2013 game client.

## Requirements

Mariner requires the following in order to build and import:

- Visual Studio 2022 (ensure you have the Visual C++ packages installed)
- StudPE
- A computer, as it would be painful to live without one, wouldn't it?

### Optional, but highly recommended

- x64dbg
- IDA Pro

These are recommended as they will allow you to not only reverse engineer subroutines but also importantly, find their respective addresses. Mariner already has late 2013 addresses by default.

## Building and importing

- Build the source code using Visual Studio 2022
- Move the Mariner.dll file to your game directory. This can be made easier by creating a post-build event in Properties
- Open either Studio or Player using StudPE and go to the Functions tab
- Right-click on the Imported Functions side panel on the left and add a new import
- Select Mariner.dll and click Select func
- Select the ImportMariner export and then add it to the list.
- Click ADD and then OK

## Functions

Mariner detours subroutines within the client, which are given via their specific addresses (i.e. 2631E0). Therefore, if you want Mariner to apply its patches to a client of your choice, you must edit the addresses to which Mariner defaults to addresses for the subroutines you wish to detour.  

## Functionality

- [x] HTTP rewrite
- [x] Trust check
- [ ] Public key
