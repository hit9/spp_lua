#ifndef __SPP_LUA
#define __SPP_LUA
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
extern "C" {
#endif

static int
parse(lua_State *L)
{
    for (int i = 1; i < 4; i++) {
        lua_createtable(L, 0 /* narr */, i /* nrec */);
        lua_pushnumber(L, i);
        lua_pushstring(L, "string");
        lua_settable(L, -3);
    }
    return 1;
}

static const struct luaL_Reg spp_lua_funcs[] = {
    {"parse", parse},
    {NULL, NULL}
};

int
luaopen_spp_lua(lua_State *L)
{
    if (LUA_VERSION_NUM <= 501) {
        luaL_register(L, "spp_lua", spp_lua_funcs);
    } else {
        lua_newtable(L);
        luaL_setfuncs(L, spp_lua_funcs, 0);
    }
    return 1;
}
#ifdef __cplusplus
}
#endif
