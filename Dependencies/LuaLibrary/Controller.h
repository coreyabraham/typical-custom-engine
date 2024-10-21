#pragma once

#include "LuaObject.h"

namespace LuaInCPP
{
	/// <summary>
	/// A General Controller that handles all Objects being utilized.
	/// ( Used as a Singleton, please only initialize one of these! )
	/// </summary>
	class Controller
	{
		std::vector<LuaObject*> objects = {};

	public:
		Controller() = default;
		~Controller();

		LuaObject* CreateObject(const char* global = "self");
		bool DestroyObject(int index);

		LuaObject* GetObject(int index);

		bool IsEmpty();
		const size_t Count();

		void ClearAllObjects();
	};
}