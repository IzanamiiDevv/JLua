import JLua.JLua;

public class Program {
    public static void main(String[] args) {
        JLua lua = new JLua("math.lua");

        lua.run("add", "int,int", "10,10", 2);
    }
}
