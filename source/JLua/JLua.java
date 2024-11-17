package JLua;
import java.nio.file.Path;
import java.nio.file.Paths;


public class JLua {
    static {
        System.loadLibrary("JLua/jlualib");
        /*
        Path path = Paths.get("JLua/jlualib.dll");
        System.load(path.toAbsolutePath().toString());
         */
    }

    String name;
    private native void v_invoke(String fname, String func, String t_args, String args, int argc);
    private native String s_invoke(String fname, String func, String t_args, String args, int argc);
    private native int i_invoke(String fname, String func, String t_args, String args, int argc);


    public JLua(String fname) {
        this.name = fname;
    }

    public <T> T p_run(String func, String t_args, String args, int argc, Class<T> t_ret) {
        try {
            if (t_ret == Void.class) {
                v_invoke(name, func, t_args, args, argc);
                return null;
            } else if (t_ret == String.class) {
                return t_ret.cast(s_invoke(name, func, t_args, args, argc));
            } else if (t_ret == Integer.class || t_ret == int.class) {
                return t_ret.cast(i_invoke(name, func, t_args, args, argc));
            } else {
                throw new IllegalArgumentException("Unsupported return type: " + t_ret);
            }
        } catch (Exception e) {
            throw new RuntimeException("Failed to invoke the native method", e);
        }
    }
}