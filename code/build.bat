@echo off

IF NOT EXIST "../build" mkdir "../build"

pushd "../build"
cl -Od -nologo -Gm- -GR- -EHsc- -FC -Z7 ../code/main.cpp /link -opt:ref
popd