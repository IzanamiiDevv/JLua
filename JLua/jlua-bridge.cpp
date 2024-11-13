#include <jni.h>
#include <iostream>
#include "include/lua.hpp"
#include "include/JLua_JLua.h"

extern "C" {

JNIEXPORT jlong JNICALL Java_LuaBridge_initLua(JNIEnv *env, jobject obj) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    return reinterpret_cast<jlong>(L);
}

JNIEXPORT void JNICALL Java_LuaBridge_loadScript(JNIEnv *env, jobject obj, jlong luaStatePtr, jstring scriptPath) {
    const char *path = env->GetStringUTFChars(scriptPath, nullptr);
    lua_State *L = reinterpret_cast<lua_State *>(luaStatePtr);

    if (luaL_dofile(L, path) != LUA_OK) {
        std::cerr << "Error loading Lua script: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    env->ReleaseStringUTFChars(scriptPath, path);
}

JNIEXPORT jstring JNICALL Java_LuaBridge_callFunction(JNIEnv *env, jobject obj, jlong luaStatePtr, jstring functionName, jstring argument) {
    lua_State *L = reinterpret_cast<lua_State *>(luaStatePtr);
    const char *funcName = env->GetStringUTFChars(functionName, nullptr);
    const char *arg = env->GetStringUTFChars(argument, nullptr);

    lua_getglobal(L, funcName);
    lua_pushstring(L, arg);

    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        std::cerr << "Error calling Lua function: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        env->ReleaseStringUTFChars(functionName, funcName);
        env->ReleaseStringUTFChars(argument, arg);
        return nullptr;
    }

    const char *result = lua_tostring(L, -1);
    jstring javaResult = env->NewStringUTF(result);
    lua_pop(L, 1);

    env->ReleaseStringUTFChars(functionName, funcName);
    env->ReleaseStringUTFChars(argument, arg);
    return javaResult;
}

JNIEXPORT void JNICALL Java_LuaBridge_closeLua(JNIEnv *env, jobject obj, jlong luaStatePtr) {
    lua_State *L = reinterpret_cast<lua_State *>(luaStatePtr);
    lua_close(L);
}

}