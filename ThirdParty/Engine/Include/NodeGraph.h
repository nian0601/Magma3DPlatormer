#pragma once

#include <CUMap.h>
#include <CUString.h>

namespace Magma
{
	class Node;
	class NodeGraph
	{
	public:
		NodeGraph();
		NodeGraph(const CU::String& aFilePath);
		~NodeGraph();

	private:
		CU::Map<unsigned long long, Node*> myNodes;
	};
}