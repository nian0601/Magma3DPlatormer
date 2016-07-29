#pragma once

#include "ComponentStorage.h"
#include "CustomEntityTypes.h"
#include "TypeID.h"
#include <GrowingArray.h>


namespace Magma
{
	struct ComponentFilter;

	class BaseProcessor;
	class World
	{
	public:
		World();
		~World();

		Entity CreateEntity();
		void Update(float aDelta);
		const CU::GrowingArray<Entity>& GetEntities(const ComponentFilter& aFilter);

		template<typename T>
		void AddComponent(Entity aEntity);

		template<typename T>
		T& GetComponent(Entity aEntity);

		template<typename T>
		void AddProcessor();

	private:
		Entity myNextEntity;

		ComponentStorage* myComponentStorage;
		CU::GrowingArray<BaseProcessor*> myProcessors;
	};


	template<typename T>
	void World::AddComponent(Entity aEntity)
	{
		T* component = new T();
		myComponentStorage->AddComponent(aEntity, component, TypeID<BaseComponent>::GetID<T>());
	}

	template<typename T>
	T& World::GetComponent(Entity aEntity)
	{
		return static_cast<T&>(myComponentStorage->GetComponent(aEntity, TypeID<BaseComponent>::GetID<T>()));
	}

	template<typename T>
	void World::AddProcessor()
	{
		myProcessors.Add(new T(*this));
	}
}