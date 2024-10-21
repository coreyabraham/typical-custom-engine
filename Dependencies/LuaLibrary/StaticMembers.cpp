#include "StaticMembers.h"

using namespace LuaInCPP;

const char* StatusPrints::PrintPcallStatus(LUA_PCALL_STATUS status)
{
	switch (status)
	{
	case LUA_PCALL_STATUS::OKAY: return "No problems returned.";
	case LUA_PCALL_STATUS::RUNTIME: return "A runtime error has occured.";
	case LUA_PCALL_STATUS::MEMORY: return "A memory allocation error has occured, the meessage handler was not called.";
	case LUA_PCALL_STATUS::ERROR_HANDLER: return "An error while running the message handler has occured.";
	case LUA_PCALL_STATUS::GC_META_METHOD: return "An error while running a __gc metamethod has occured. The message handler was not called.";

	case LUA_PCALL_STATUS::STACK_POSITION: return "An error trying to find and execute the target function on the stack has occured, it possibly doesn't exist.";
	default: return "An unknown error has occured.";
	}
}

const char* StatusPrints::PrintPcallStatus(int status)
{
	return PrintPcallStatus((LUA_PCALL_STATUS)status);
}

const char* StatusPrints::PrintLoadStatus(LUA_LOAD_STATUS status)
{
	switch (status)
	{
	case LUA_LOAD_STATUS::OK: return "No problems returned.";
	case LUA_LOAD_STATUS::SYNTAX: return "A syntax error during precompilation occured.";
	case LUA_LOAD_STATUS::MEM: return "A memory allocation error has occured, the message handler was not called.";
	case LUA_LOAD_STATUS::GC_META: return "An error while running a __gc metamethod has occured. The message handler was not called.";
	case LUA_LOAD_STATUS::ERR_FILE: return "A file related error has occured, the file potentially doesn't exist or cannot be opened and or read.";

	default: return "An unknown error has occured.";
	}
}

const char* StatusPrints::PrintLoadStatus(int status)
{
	return PrintLoadStatus((LUA_LOAD_STATUS)status);
}

bool LuaInCPP::StaticLua::CompileStringAndRun(const std::string& code)
{
	lua_State* luaState = luaL_newstate();
	luaL_openlibs(luaState);

	int result = luaL_dostring(luaState, code.c_str());
	return (result == 0);
}