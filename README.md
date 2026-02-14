# Doom64EX-Classic
Doom64EX Classic - for those that preferred 2.5 over EX-Plus

# Changes from EX 2.5

1. SDL3 Support
2. SDL3 Gamepad Support
3. Multiplatform modern CMake build system
4. Perfect frametimes and interpolation
5. Fully 64bit compatible

EX-Classic is essentially EX 2.5, just fixed up, cleaned up and modernised to the latest standard of SDL3.  There will be a few bugs that will be ironed out, but this only relates to Gamepad support.  There will be some backports from EX-Plus (for interpolation of the automap and the Gamepad fixes).

Note to self-builders, you'll want to use the provided lpng1514 zip in the 'lib' directory.  Only this version allows the WadGen tool to correctly unpack the ROM and also, only this version works with the palette animations.  I've been fighting with lpng16 for years and to be honest, this just prevents headaches and it works.

# Why?

I understand that EX-Plus is not suitable for many *NIX distro's and communities and for that reason, I made this, for those that not only need a GPL'd version, but for those that also don't want to use the remaster but the original N64 ROM.  But please note, it ONLY supports the ROM IWAD, meaning, only mods suitable for EX 2.5 will run.  This will not change.

Saying that..  enjoy!
