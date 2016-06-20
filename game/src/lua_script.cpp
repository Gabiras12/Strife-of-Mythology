#include "luascript.h"

LuaScript::LuaScript(const std::string& fname) {
    L = luaL_newstate();
    if (luaL_loadfile(L, fname.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cout<<"Error: failed to load ("<<fname<<")"<<std::endl;
        L = nullptr;
    }

    if(L) luaL_openlibs(L);
}

int
LuaScript::load_function(std::vector<int> args, std::string func_name)
{
    lua_getglobal(L, func_name.c_str());

    for (auto it : args) {
        lua_pushnumber(L, it);
    }

    /* call the function with n arguments, return 1 result */
    lua_call(L, args.size(), 1);

    auto result = (int)lua_tointeger(L, -1);
    lua_pop(L, 1);

    return result;
}

LuaScript::~LuaScript() {
    if(L) lua_close(L);
}

void LuaScript::printError(const std::string& variableName, const std::string& reason) {
    std::cout<<"Error: can't get ["<<variableName<<"]. "<<reason<<std::endl;
}

std::vector<int> LuaScript::getIntVector(const std::string& name) {
    std::vector<int> v;
    lua_gettostack(name.c_str());
    if(lua_isnil(L, -1)) { // array is not found
        return std::vector<int>();
    }
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        v.push_back((int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    clean();
    return v;
}

std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
    std::string code =
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end";
    luaL_loadstring(L,
            code.c_str());
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys");
    lua_pushstring(L, name.c_str());
    lua_pcall(L, 1 , 1, 0);
    std::string test = lua_tostring(L, -1);
    std::vector<std::string> strings;
    std::string temp = "";
    for(unsigned int i = 0; i < test.size(); i++) {
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    clean();
    return strings;
}
