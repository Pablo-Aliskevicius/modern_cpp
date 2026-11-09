
@echo off
if not exist out mkdir out

for %%F in (*.c) do cl /std:c17 /O2 /Foout\ "%%F" /Fe:"out\%%~nF.exe"
