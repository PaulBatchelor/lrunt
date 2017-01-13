#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <runt.h>

static int l_foo(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    int num = lua_tointeger(L, 2);
    printf("we are here! %s %d\n", s, num);
    lua_pushinteger(L, 1234);
    return 1;
}

static int l_init(lua_State *L)
{
    runt_vm *vm;
    unsigned char *mem;
    runt_cell *cells;
    long size;
    unsigned int ncells;
  
    ncells = lua_tointeger(L, 1); 
    size = lua_tointeger(L, 2);
    mem = (unsigned char *) lua_newuserdata(L, size);
    cells = (runt_cell *)lua_newuserdata(L, ncells * sizeof(runt_cell));
    vm = (runt_vm *)lua_newuserdata(L, sizeof(runt_vm));
    runt_init(vm);
    runt_cell_pool_set(vm, cells, ncells);
    runt_cell_pool_init(vm);
    runt_memory_pool_set(vm, mem, size);

    runt_load_stdlib(vm);
    runt_set_state(vm, RUNT_MODE_INTERACTIVE, RUNT_ON);

    lua_pushlightuserdata(L, vm);
    return 1;
}

static int l_parse(lua_State *L)
{
    const char *str;
    runt_vm *vm;
    vm = (runt_vm *)lua_touserdata(L, 1);
    str = lua_tostring(L, 2);
    runt_pmark_set(vm);
    runt_compile(vm, str);
    runt_pmark_free(vm);
    return 0;
}

static int l_pop(lua_State *L)
{
    const char *str;
    runt_vm *vm;
    runt_int rc;
    runt_stacklet *s;
    float x = 0;
    vm = (runt_vm *)lua_touserdata(L, 1);
    rc = runt_ppop(vm, &s);

    if(rc == RUNT_NOT_OK) {
        return 0;
    }

    x = s->f;

    lua_pushnumber(L, x);
    
    return 1;
}

static int l_push(lua_State *L)
{
    const char *str;
    runt_vm *vm;
    runt_int rc;
    runt_stacklet *s;
    float x = 0;
    vm = (runt_vm *)lua_touserdata(L, 1);
    x = lua_tonumber(L, 2);
    rc = runt_ppush(vm, &s);

    if(rc == RUNT_NOT_OK) {
        return 0;
    }

    s->f = x;
    return 0;
}

static int l_exec(lua_State *L)
{
    const char *str;
    runt_vm *vm;
    runt_int rc;
    runt_uint id;
    vm = (runt_vm *)lua_touserdata(L, 1);
    id = lua_tointeger(L, 2);

    runt_cell_id_exec(vm, id);

    return 0;
}

static const struct luaL_Reg lrunt[] = { 
    {"foo", l_foo},
    {"init", l_init},
    {"parse", l_parse},
    {"pop", l_pop},
    {"push", l_push},
    {"exec", l_exec},
    {NULL, NULL}
};

int luaopen_lrunt(lua_State *L)
{
    luaL_openlib(L, "lrunt", lrunt, 0);
    return 1;
}

