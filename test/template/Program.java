import JLua.JLua;

public class Program {
    public static void main(String[] args) {
        JLua luaStream = new JLua("template.lua");

        luaStream.p_run("echo", "string", "Hello World", 1, Void.class);
    }
}
