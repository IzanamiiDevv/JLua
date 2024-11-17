@echo off
:: Set the JDK path
set JDK_PATH="C:\Program Files\Java\jdk-22"

cd JLua
:: Create Java Header ::
echo [ Creating Java Header ]
javac -h . JLua.java && echo Done

:: Create DLL ::
echo [ Creating DLL File ]
g++ -shared -o jlualib.dll jlualib.cpp -I%JDK_PATH%\include -I%JDK_PATH%\include\win32 -Iinclude -I. -L. -llua54 && echo Done
cd ..