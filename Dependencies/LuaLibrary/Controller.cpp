#include "Controller.h"

using namespace LuaInCPP;

Controller::~Controller()
{
	for (int i = 0; i < objects.size(); i++)
	{
		DestroyObject(i);
	}

	objects.clear();
}

LuaObject* Controller::CreateObject(const char* global)
{
	objects.push_back(new LuaObject(global));
	return objects.back();
}

bool Controller::DestroyObject(int index)
{
	delete objects[index];
	objects.erase(objects.begin() + index);

	return true;
}

LuaObject* Controller::GetObject(int index)
{
	return objects[index];
}

bool Controller::IsEmpty()
{
	return objects.empty();
}

const size_t Controller::Count()
{
	return objects.size();
}

void Controller::ClearAllObjects()
{
	objects.clear();
}