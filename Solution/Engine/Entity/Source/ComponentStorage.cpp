#include "stdafx.h"

#include "../Include/ComponentFilter.h"
#include "../Include/ComponentStorage.h"

namespace Magma
{
	ComponentStorage::ComponentStorage()
		: myComponents(MAX_NUMBER_OF_COMPONENTS)
		, myEntityComponents(128)
		, myEntitiesToReturn(128)
	{
		for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
		{
			myComponents.Add(ComponentArray());
			myComponents[i].Init(128);
		}
	}


	ComponentStorage::~ComponentStorage()
	{
	}

	void ComponentStorage::AddEntity()
	{
		myEntityComponents.Add(EntityComponentArray());
		EntityComponentArray& components = myEntityComponents.GetLast();

		for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
		{
			components[i] = -1;
		}
	}

	void ComponentStorage::AddComponent(Entity aEntity, BaseComponent* aComponent, unsigned int aComponentID)
	{
		myComponents[aComponentID].Add(aComponent);
		myEntityComponents[aEntity][aComponentID] = myComponents[aComponentID].Size() - 1;
	}

	BaseComponent& ComponentStorage::GetComponent(Entity aEntity, unsigned int aComponentID)
	{
		DL_ASSERT_EXP(HasComponent(aEntity, aComponentID), "Tried to Get an invalid component");

		int componentIndex = myEntityComponents[aEntity][aComponentID];
		return *myComponents[aComponentID][componentIndex];
	}

	bool ComponentStorage::HasComponent(Entity aEntity, unsigned int aComponentID)
	{
		if (myEntityComponents.Size() <= aEntity)
		{
			return false;
		}

		return myEntityComponents[aEntity][aComponentID] != -1;
	}

	const CU::GrowingArray<Entity>& ComponentStorage::GetEntities(const ComponentFilter& aFilter)
	{
		myEntitiesToReturn.RemoveAll();
		for (int i = 0; i < myEntityComponents.Size(); ++i)
		{
			if (aFilter.Compare(myEntityComponents[i]) == true)
			{
				myEntitiesToReturn.Add(i);
			}
		}

		return myEntitiesToReturn;
	}
}