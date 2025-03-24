@echo off
ctypesgen -cpp="clang -E" -ldynemu_shared bindings.h -o ..\dynemu.py --insert-file=bindings.py