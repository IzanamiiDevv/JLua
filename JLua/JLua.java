package JLua;
import java.nio.file.Path;
import java.nio.file.Paths;


public class JLua {
    static {
        Path path = Paths.get("JLua/jlualib.dll");
        System.load(path.toAbsolutePath().toString());
    }

    String name;
    private native String invoke(String fname, String func, String t_args, String args, int argc);


    public JLua(String fname) {
        this.name = fname;
    }

    public void run(String func, String t_args,String args,int argc) {
        invoke(this.name, func, t_args,args, argc);
    }
}