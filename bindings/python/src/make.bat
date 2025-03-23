@echo off
ctypesgen.exe -ldynemu_shared bindings.h -o ..\dynemu.py --insert-file=bindings.py