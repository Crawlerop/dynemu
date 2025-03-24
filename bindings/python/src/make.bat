@echo off
ctypesgen --cpp="clang -E" -ldynemu bindings.h -o ..\dynemu.py --insert-file=bindings.py