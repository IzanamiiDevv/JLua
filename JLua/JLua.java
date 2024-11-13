package JLua;

public class JLua {
    static {
        System.loadLibrary("jlualib");
    }

    public native long initLua();
    public native void loadScript(long luaState, String scriptPath);
    public native String callFunction(long luaState, String functionName, String argument);
    public native void closeLua(long luaState);
}