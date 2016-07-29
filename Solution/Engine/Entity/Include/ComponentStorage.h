#pragma once

#include "CustomEntityTypes.h"
#include <GrowingArray.h>
#include <StaticArray.h>


namespace Magma
{
	struct ComponentFilter;

	class BaseComponent;
	class ComponentStorage
	{
	public:
		ComponentStorage();
		~ComponentStorage();

		void AddEntity();

		void AddComponent(Entity aEntity, BaseComponent* aComponent, unsigned int aComponentID);
		BaseComponent& GetComponent(Entity aEntity, unsigned int aComponentID);
		bool HasComponent(Entity aEntity, unsigned int aComponentID);

		const CU::GrowingArray<Entity>& GetEntities(const ComponentFilter& aFilter);

	private:
		CU::GrowingArray<ComponentArray> myComponents;
		CU::GrowingArray<EntityComponentArray> myEntityComponents;

		CU::GrowingArray<Entity> myEntitiesToReturn;
	};
}