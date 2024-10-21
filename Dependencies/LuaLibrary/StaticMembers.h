#pragma once

#include "Enums.h"
#include "LuaObject.h"

namespace LuaInCPP
{
	/// <summary>
	/// Return Error Codes based on Lua states.
	/// </summary>
	class StatusPrints
	{
	public:
		static const char* PrintPcallStatus(LUA_PCALL_STATUS status);
		static const char* PrintPcallStatus(int status);

		static const char* PrintLoadStatus(LUA_LOAD_STATUS status);
		static const char* PrintLoadStatus(int status);
	};

	/// <summary>
	/// Run methods that aren't tied an Instance, completely independent from a class.
	/// </summary>
	class StaticLua
	{
	public:
		static bool CompileStringAndRun(const std::string& code);
	};
}