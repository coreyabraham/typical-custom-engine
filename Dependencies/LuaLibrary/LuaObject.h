#pragma once

#include "Common.h"
#include "Enums.h"

namespace LuaInCPP
{
	/// <summary>
	/// A Lua Instance that's used to store Lua bindings in an OOP context.
	/// ( Controls only one Script unless you unload the current script then load a different one! )
	/// </summary>
	class LuaObject
	{
	private:
		lua_State* luaState;
		bool loaded = false;

		std::string filepath;
		std::string globalname;

	public:
		LuaObject(const char* global = "self");
		~LuaObject() = default;

		bool LoadScript(const char* filepath, const char* funcName);
		bool LoadScript(std::string filepath, const char* funcName);
		bool ReloadScript(const char* funcName);

		std::string CheckHeader();
		std::string GetFilePath() const { return filepath; }

		lua_State* GetState() const { return luaState; }
		const char* GetGlobalKeyword() const { return globalname.c_str(); }
		bool IsLoaded() const { return loaded; }

		void RegisterLuaFunction(const char* functionName, lua_CFunction luaFunction);
		LUA_PCALL_STATUS CallFunction(int argCount, int returnCount);

		bool PushFunction(const char* functionName);
		LUA_PCALL_STATUS RunFunction(const char* functionName, int argCount, int returnCount);

		void SetPointerVar(lua_State* state, const char* varName, void* value);
		void* GetPointerVar(lua_State* state, const char* varName);
	};
}