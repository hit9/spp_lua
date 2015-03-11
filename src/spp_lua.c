#ifndef __SPP_LUA
#define __SPP_LUA
#endif

#include "spp.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
extern "C" {
#endif

static void
parser_handler(struct spp_st *parser, char *s, size_t sz, int idx)
{

}

static int
parser_new(lua_State *L)
{
    spp_t **udata = lua_newuserdata(L, sizeof(spp_t *));
    luaL_getmetatable(L, "spp_parser");
    lua_setmetatable(L, -2);
    spp_t *parser = spp_new();
    parser->handler = &parser_handler;
    *udata = parser;
    return 1;
}

static int
parser_feed(lua_State *L)
{
    spp_t **udata = (spp_t **)(luaL_checkudata(L, 1, "spp_parser"));
    spp_t *parser = *udata;
    const char *input = luaL_checkstring(L, 2);
    if (spp_feed(parser, (char *)input) != SPP_OK)
        luaL_error(L, "No memory to feed data");
    hbuf_print(parser->buf);
    return 0;
}

static const struct luaL_Reg spp_parser_methods[] = {
    {"feed", parser_feed},
    {NULL, NULL}
};

static const struct luaL_Reg spp_lua_funcs[] = {
    {"new", parser_new},
    {NULL, NULL}
};

int
luaopen_spp_lua(lua_State *L)
{
    luaL_newmetatable(L, "spp_parser");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, spp_parser_methods, 0);

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
