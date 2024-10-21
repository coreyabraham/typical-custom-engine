#pragma once

namespace LuaInCPP
{
	/// <summary>
	/// An Enumeration that lists all possible return types for lua_pcall.
	/// </summary>
	enum LUA_PCALL_STATUS
	{
		// lua_pcall

		OKAY = 0,
		RUNTIME,
		MEMORY,
		ERROR_HANDLER,
		GC_META_METHOD,

		// custom

		STACK_POSITION,
		UNKNOWN
	};

	enum LUA_LOAD_STATUS
	{
		// lua_load

		OK = 0,
		SYNTAX,
		MEM,
		GC_META,

		// lua_loadfile

		ERR_FILE,

		// custom

		_UNKNOWN
	};
}