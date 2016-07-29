#pragma once
#include "ComponentFilter.h"
#include "World.h"

namespace Magma
{
	class BaseProcessor
	{
	public:
		BaseProcessor(World& aWorld);
		BaseProcessor(World& aWorld, const ComponentFilter& aFilter);
		virtual ~BaseProcessor();

		virtual void Update(float aDelta) = 0;

		const CU::GrowingArray<Entity>& GetEntities();

		template<typename T>
		T& GetComponent(Entity aEntity);

	private:
		void operator=(BaseProcessor&) = delete;
		World& myWorld;
		ComponentFilter myComponentFilter;
	};

	template<typename T>
	T& BaseProcessor::GetComponent(Entity aEntity)
	{
		return myWorld.GetComponent<T>(aEntity);
	}
}