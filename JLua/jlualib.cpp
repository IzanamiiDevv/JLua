#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "include/lua.hpp"
#include "JLua_JLua.h"

enum Type {
    Void,
    String,
    Integer,
};

typedef struct {
    enum Type type;
    std::string value;
} Token;

typedef std::vector<Token> TokenStream;
TokenStream parseTokens(const char* args_t,const char* args, int argc);
void runLuaFunction(const TokenStream& tokens, const char* funcname, const char* fname, std::string& buffer, enum Type ret_t);

extern "C" {
    JNIEXPORT void JNICALL Java_JLua_JLua_v_1invoke(JNIEnv *env, jobject, jstring fn, jstring funcn,jstring _targs, jstring _args, jint _argc) {
        const char* fname = env->GetStringUTFChars(fn, nullptr);
        const char* funcname = env->GetStringUTFChars(funcn, nullptr);
        const char* p_targs = env->GetStringUTFChars(_targs, nullptr);
        const char* p_args = env->GetStringUTFChars(_args, nullptr);
        TokenStream tokens = parseTokens(p_targs, p_args, _argc);
        std::string buffer;
        runLuaFunction(tokens, funcname, fname, buffer, Type::Void);
        env->ReleaseStringUTFChars(fn, fname);
        env->ReleaseStringUTFChars(funcn, funcname);
        env->ReleaseStringUTFChars(_targs, p_targs);
        env->ReleaseStringUTFChars(_args, p_args);
    }

    JNIEXPORT jstring JNICALL Java_JLua_JLua_s_1invoke(JNIEnv *env, jobject, jstring fn, jstring funcn,jstring _targs, jstring _args, jint _argc) {
        const char* fname = env->GetStringUTFChars(fn, nullptr);
        const char* funcname = env->GetStringUTFChars(funcn, nullptr);
        const char* p_targs = env->GetStringUTFChars(_targs, nullptr);
        const char* p_args = env->GetStringUTFChars(_args, nullptr);
        TokenStream tokens = parseTokens(p_targs, p_args, _argc);
        std::string buffer;
        runLuaFunction(tokens, funcname, fname, buffer, Type::String);
        env->ReleaseStringUTFChars(fn, fname);
        env->ReleaseStringUTFChars(funcn, funcname);
        env->ReleaseStringUTFChars(_targs, p_targs);
        env->ReleaseStringUTFChars(_args, p_args);
        return env->NewStringUTF(buffer.c_str());
    }

    JNIEXPORT jint JNICALL Java_JLua_JLua_i_1invoke(JNIEnv *env, jobject, jstring fn, jstring funcn,jstring _targs, jstring _args, jint _argc) {
        const char* fname = env->GetStringUTFChars(fn, nullptr);
        const char* funcname = env->GetStringUTFChars(funcn, nullptr);
        const char* p_targs = env->GetStringUTFChars(_targs, nullptr);
        const char* p_args = env->GetStringUTFChars(_args, nullptr);
        TokenStream tokens = parseTokens(p_targs, p_args, _argc);
        std::string buffer;
        runLuaFunction(tokens, funcname, fname, buffer, Type::Integer);
        env->ReleaseStringUTFChars(fn, fname);
        env->ReleaseStringUTFChars(funcn, funcname);
        env->ReleaseStringUTFChars(_targs, p_targs);
        env->ReleaseStringUTFChars(_args, p_args);
        return std::stoi(buffer);
    }
}

TokenStream parseTokens(const char* args_t,const char* args, int argc) {
    TokenStream result;

    std::vector<std::string> types;
    std::stringstream ss_t(args_t);
    std::string type_str;
    while (std::getline(ss_t, type_str, '|'))
        types.push_back(type_str);

    std::vector<std::string> values;
    std::stringstream ss(args);
    std::string value_str;
    while (std::getline(ss, value_str, '|'))
        values.push_back(value_str);

    if (types.size() != argc || values.size() != argc) {
        std::cerr << "Error: Mismatch in the number of types or values.\n";
        return result;
    }

    for (int i = 0; i < argc; ++i) {
        Token token;

        //FIX NEED
        if (types[i] == "int") {
            token.type = Integer;
        } else if (types[i] == "string") {
            token.type = String;
        } else if (types[i] == "void") {
            token.type = Void;
        } else {
            std::cerr << "Error: Unsupported type '" << types[i] << "'.\n";
            continue;
        }

        token.value = values[i];

        result.push_back(token);
    }

    return result;
}

void runLuaFunction(const TokenStream& tokens, const char* funcname, const char* fname, std::string& buffer, enum Type ret_t) {
    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return;
    }

    luaL_openlibs(L);

    if (luaL_dofile(L, fname) != LUA_OK) {
        std::cerr << "Error loading Lua script: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return;
    }

    lua_getglobal(L, funcname);
    if (!lua_isfunction(L, -1)) {
        std::cerr << "Lua function '" << funcname << "' not found." << std::endl;
        lua_close(L);
        return;
    }

    for (const auto& token : tokens) {
        switch (token.type) {
            case Type::String:
                lua_pushstring(L, token.value.c_str());
                break;
            case Type::Integer:
                try {
                    lua_pushinteger(L, std::stoi(token.value));
                } catch (const std::invalid_argument&) {
                    std::cerr << "Invalid integer value in token: " << token.value << std::endl;
                    lua_close(L);
                    return;
                }
                break;
            default:
                std::cerr << "Unsupported token type." << std::endl;
                lua_close(L);
                return;
        }
    }

    if (lua_pcall(L, tokens.size(), 1, 0) != LUA_OK) {
        std::cerr << "Error running Lua function: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return;
    }


    switch (ret_t) {
        case Type::Void:
            //
        break;

        case Type::String:
            buffer = std::string(lua_tostring(L, -1));
        break;

        case Type::Integer:
            buffer = std::to_string(lua_tointeger(L, -1));
        break;
    }

    lua_pop(L, 1);
    lua_close(L);
}
