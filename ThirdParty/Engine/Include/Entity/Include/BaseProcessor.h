#pragma once
#include "ComponentFilter.h"
#include "World.h"

namespace Magma
{
	struct Event;
	class BaseProcessor
	{
	public:
		BaseProcessor(World& aWorld);
		BaseProcessor(World& aWorld, const ComponentFilter& aFilter);
		virtual ~BaseProcessor();

		virtual void Update(float aDelta) = 0;

		const CU::GrowingArray<Entity>& GetEntities();

		template<typename T>
		void AddComponent(Entity aEntity);

		template<typename T>
		void RemoveComponent(Entity aEntity);

		template<typename T>
		T& GetComponent(Entity aEntity);

		virtual void OnEvent(const Event& aEvent);

	private:
		void operator=(BaseProcessor&) = delete;
		World& myWorld;
		ComponentFilter myComponentFilter;
	};

	template<typename T>
	void BaseProcessor::AddComponent(Entity aEntity)
	{
		myWorld.AddComponent<T>(aEntity);
	}

	template<typename T>
	void BaseProcessor::RemoveComponent(Entity aEntity)
	{
		myWorld.RemoveComponent<T>(aEntity);
	}

	template<typename T>
	T& BaseProcessor::GetComponent(Entity aEntity)
	{
		return myWorld.GetComponent<T>(aEntity);
	}
}