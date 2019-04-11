#include "cq_lua_vm.h"
#include "cq_lua_convention.h"
#include "cqlog.hh"
#include "lua.hpp"

# ifdef _WIN32
#   include <windows.h>
# else
#   include <unistd.h>
# endif

static lua_State *_lua_state = nullptr;

static void register_func(const char *name, int32_t (*func)(lua_State *)) {
    if (_lua_state && name && func) {
        lua_register(_lua_state, name, func);
    }
}

static int traceback(lua_State *state) {
    
    lua_getglobal(state, "debug");
    lua_getfield(state, -1, "traceback");
    lua_pushvalue(state, 1);
    lua_pushinteger(state, 2);
    lua_call(state, 2, 1);
    const char *info = lua_tostring(state, -1);
    
    E("lua vm: runtime error:\n%s", info);
    
    return 1;
}

static void do_string(const char *code) {
    if (_lua_state == nullptr) {
        return;
    }
    
    //trackback function
    lua_pushcfunction(_lua_state, traceback);
    
    //load
    int error = luaL_loadstring(_lua_state, code);
    if (error) {
        const char *info = lua_tostring(_lua_state, -1);
        E("lua vm: syntax error:\n%s", info);
        return;
    }
    
    //execute
    int traceback = lua_gettop(_lua_state) - 1;
    lua_pcall(_lua_state, 0, 0, traceback);
}

static int64_t     check_integer(lua_State *s, int32_t i) {return luaL_checkinteger(s, i);}
static double      check_double (lua_State *s, int32_t i) {return luaL_checknumber (s, i);}
static const char *check_string (lua_State *s, int32_t i) {return luaL_checkstring (s, i);}

static void push_integer(lua_State *s, int64_t     v) {lua_pushinteger(s, v);}
static void push_double (lua_State *s, double      v) {lua_pushnumber (s, v);}
static void push_string (lua_State *s, const char *v) {lua_pushstring (s, v);}

void cq_lua_vm_open(const char *directory_path) {
    I("lua vm: open");
    
    cq_lua_vm_close();
    if (directory_path == nullptr) {
        return;
    }
    
    //new lua vm
    _lua_state = luaL_newstate();
    luaL_openlibs(_lua_state);
    
    //change work directory
#ifdef _WIN32
    SetCurrentDirectoryA(directory_path);
#else
    chdir(directory_path);
#endif
    do_string("package.cpath = ''");
    do_string("package.path = '?.lua'");
    
    //load third libraries
    _cq_lua_set_register_func_h(register_func);
    _cq_lua_set_do_string_h(do_string);
    
    _cq_lua_set_check_integer_h(check_integer);
    _cq_lua_set_check_double_h (check_double );
    _cq_lua_set_check_string_h (check_string );
    
    _cq_lua_set_push_integer_h(push_integer);
    _cq_lua_set_push_double_h (push_double );
    _cq_lua_set_push_string_h (push_string );
    
    cq_lua_register_added_func();
    cq_lua_do_added_string();
}

void cq_lua_vm_close() {
    if (_lua_state == nullptr) {
        return;
    }
    
    I("lua vm: close");
    
    //disable third library
    _cq_lua_set_register_func_h(nullptr);
    _cq_lua_set_do_string_h(nullptr);
    
    _cq_lua_set_check_integer_h(nullptr);
    _cq_lua_set_check_double_h (nullptr);
    _cq_lua_set_check_string_h (nullptr);
    
    _cq_lua_set_push_integer_h(nullptr);
    _cq_lua_set_push_double_h (nullptr);
    _cq_lua_set_push_string_h (nullptr);
    
    //delete lua vm
    lua_close(_lua_state);
    _lua_state = nullptr;
}
