@echo off
javac -h ./include JLua.java
g++ -shared -o jlualib.dll -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/win32" -L./lib -llua54