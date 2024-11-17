@echo off
:: Set the JDK path
set JDK_PATH="C:\Program Files\Java\jdk-22"

cd JLua
javac -h . JLua.java
g++ -shared -o jlualib.dll jlualib.cpp -I%JDK_PATH%\include -I%JDK_PATH%\include\win32 -Iinclude -L. -llua54
cd ..

::javac Program.java