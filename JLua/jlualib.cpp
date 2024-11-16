#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "include/lua.hpp"
#include "include/JLua_JLua.h"

enum Type {
    String,
    Integer,
};

typedef struct {
    enum Type type;
    const char* value;
} Token;

typedef std::vector<Token> TokenStream;
TokenStream parseTokens(const char* args_t,const char* args, char token);

extern "C" {
    JNIEXPORT jstring JNICALL Java_JLua_JLua_invoke(JNIEnv *env, jobject, jstring fn, jstring funcn,jstring _targs, jstring _args, jint _argc) {
        const char* fname = env->GetStringUTFChars(fn, nullptr);
        const char* funcname = env->GetStringUTFChars(funcn, nullptr);
        const char* p_targs = env->GetStringUTFChars(_targs, nullptr);
        const char* p_args = env->GetStringUTFChars(_args, nullptr);
        TokenStream tokens = parseTokens(p_targs, p_args, _argc);

        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        if(luaL_dofile(L, fname) != 0) {
            std::cout << "Error Loading the File" << std::endl;
            return;
        }

        for(int i = 0; i < _argc; i++) {
        }

        lua_close(L);
        env->ReleaseStringUTFChars(fn, fname);
        env->ReleaseStringUTFChars(funcn, funcname);
        env->ReleaseStringUTFChars(_targs, p_targs);
        env->ReleaseStringUTFChars(_args, p_args);
    }
}

TokenStream parseTokens(const char* args_t,const char* args, int argc) {
    TokenStream result;

    std::vector<std::string> types;
    std::stringstream ss_t(args_t);
    std::string type_str;
    while (std::getline(ss_t, type_str, '|')) {
        types.push_back(type_str);
    }

    std::vector<std::string> values;
    std::stringstream ss(args);
    std::string value_str;
    while (std::getline(ss, value_str, '|')) {
        values.push_back(value_str);
    }

    if (types.size() != argc || values.size() != argc) {
        std::cerr << "Error: Mismatch in the number of types or values.\n";
        return result;
    }
    for (int i = 0; i < argc; ++i) {
        Token token;

        if (types[i] == "int") {
            token.type = Integer;
        } else if (types[i] == "string") {
            token.type = String;
        } else {
            std::cerr << "Error: Unsupported type '" << types[i] << "'.\n";
            continue;
        }

        token.value = values[i].c_str();

        result.push_back(token);
    }

    return result;
}