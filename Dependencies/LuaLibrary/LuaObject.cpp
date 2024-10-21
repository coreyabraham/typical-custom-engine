#include <fstream>
#include "LuaObject.h"

using namespace LuaInCPP;

LuaObject::LuaObject(const char* global)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	SetPointerVar(luaState, global, this);
	globalname = global;
}

bool LuaObject::LoadScript(const char* file, const char* funcName)
{
	if (loaded) return true;

	int status = luaL_dofile(luaState, file);

	if (status)
	{
		lua_tostring(luaState, -1);
		return false;
	}

	lua_getglobal(luaState, funcName);
	status = lua_pcall(luaState, 0, 0, NULL);

	filepath = file;
	loaded = true;

	return true;
}

bool LuaObject::LoadScript(std::string file, const char* funcName)
{
	return LoadScript(file.c_str(), funcName);
}

bool LuaObject::ReloadScript(const char* funcName)
{
	loaded = false;
	return LoadScript(filepath, funcName);
}

std::string LuaObject::CheckHeader()
{
	std::ifstream file(filepath);

	if (!file.is_open()) return "N/A";

	if (file.fail())
	{
		file.close();
		return "N/A";
	}

	std::string firstLine;
	std::getline(file, firstLine);

	auto found = firstLine.find_first_not_of(" \t");

	if (found == std::string::npos) return "N/A";
	if (firstLine[found] != '#') return "N/A";

	// this could be improved to delete any whitespace after the # if the header is formatted like this for example:
	// # this is a header
	// instead of the expected:
	// #this is a header

	firstLine.erase(0, 1);

	return firstLine;
}

void LuaObject::RegisterLuaFunction(const char* functionName, lua_CFunction luaFunction)
{
	if (!loaded) return;

	lua_register(luaState, functionName, luaFunction);
}

LUA_PCALL_STATUS LuaObject::CallFunction(int argCount, int returnCount)
{
	if (!loaded) return LUA_PCALL_STATUS::UNKNOWN;

	if (!lua_isfunction(luaState, -(argCount + 1)))
	{
		return LUA_PCALL_STATUS::STACK_POSITION;
	}

	return (LUA_PCALL_STATUS)lua_pcall(luaState, argCount, returnCount, NULL);
}

bool LuaObject::PushFunction(const char* functionName)
{
	if (!loaded) return false;

	lua_getglobal(luaState, functionName);
	return (lua_isfunction(luaState, -1) != false);
}

LUA_PCALL_STATUS LuaObject::RunFunction(const char* functionName, int argCount, int returnCount)
{
	if (!loaded) return LUA_PCALL_STATUS::UNKNOWN;

	bool result = PushFunction(functionName);
	if (!result) return LUA_PCALL_STATUS::ERROR_HANDLER;

	return CallFunction(argCount, returnCount);
}

void LuaObject::SetPointerVar(lua_State* state, const char* varName, void* value)
{
	if (!loaded) return;

	lua_pushlightuserdata(state, value);
	lua_setglobal(state, varName);
}

void* LuaObject::GetPointerVar(lua_State* state, const char* varName)
{
	if (!loaded) return NULL;

	lua_getglobal(state, varName);

	if (lua_isuserdata(state, -1) == false)
	{
		return NULL;
	}

	void* vpVal = (void*)lua_topointer(state, -1);
	lua_pop(state, 1);
	return vpVal;
}