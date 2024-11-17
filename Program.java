import JLua.JLua;

public class Program {
    public static void main(String[] args) {
        JLua lua = new JLua("math.lua");

        String out = lua.p_run("printssss", "string", "Hello World", 1, String.class);
        System.out.println(out);

        int resp = lua.p_run("add", "int|int", "10|10", 2, Integer.class);

        System.out.println(resp);
    }
}