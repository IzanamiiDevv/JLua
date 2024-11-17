@echo off
:: Set the JDK path
set JDK_PATH="C:\Program Files\Java\jdk-22"

cd JLua
echo Creating Java Header
javac -h . JLua.java && echo Done Compiling Java as Header
echo Compiling DLL
g++ -shared -o jlualib.dll jlualib.cpp -I%JDK_PATH%\include -I%JDK_PATH%\include\win32 -Iinclude -I. -L. -llua54 && g++ -shared -o jlualib.so jlualib.cpp -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -Iinclude -I. -llua54 -fPIC && echo Done Compiling DLL
cd ..

::javac Program.java