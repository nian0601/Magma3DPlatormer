#pragma once

#include <CUString.h>

namespace tinyxml2
{
	class XMLElement;
}

class XMLReader;

struct lua_State;

namespace Magma
{
	class NodeGraph;
	class Node
	{
	public:
		Node();
		Node(XMLReader& aReader, tinyxml2::XMLElement* aNodeElement, NodeGraph* aGraph);
		~Node();

		void Execute();

		unsigned long long GetUID() const;
		bool IsInitNode() const;

	private:
		NodeGraph* myGraph;
		unsigned long long myUID;
		bool myIsInitNode;
		CU::String myFileName;

		lua_State* myLuaState;
	};

	inline unsigned long long Node::GetUID() const
	{
		return myUID;
	}

	inline bool Node::IsInitNode() const
	{
		return myIsInitNode;
	}
}