#pragma once

#include "LuaFiles/lua.hpp"
#include <functional>

namespace Magma
{
	class ScriptSystem
	{
	public:
		static ScriptSystem* GetInstance();
		static void Destroy();

		void SetRegisterFunction(std::function<void(ScriptSystem&, lua_State*)> aFunction);
		lua_State* CreateState(const CU::String& aFilePath);
		void SetCurrentState(lua_State* aState);

		void RegisterFunction(const CU::String& aNameInLua, lua_CFunction aFunction);

		void CallFunction(const CU::String& aName);

		void GetArgFromLua(int aIndex, char* aString);
		void GetArgFromLua(int aIndex, int& aInt);
		void GetArgFromLua(int aIndex, float& aFloat);
		void GetArgFromLua(int aIndex, bool& aBool);

	private:
		ScriptSystem();
		~ScriptSystem();

		void CheckError(int aErrorCode);

		lua_State* myCurrentState;
		std::function<void(ScriptSystem&, lua_State*)> myCppRegisterFunction;

		static ScriptSystem* ourInstance;
	};
}