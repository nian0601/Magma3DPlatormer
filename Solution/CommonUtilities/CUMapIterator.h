#pragma once

namespace CU
{
	template<typename Key, typename Value>
	struct MapIterator
	{
		MapIterator(int aFirst, int aSecond, Key* aKey, Value* aValue)
			: myFirstIndex(aFirst)
			, mySecondIndex(aSecond)
			, myKey(aKey)
			, myValue(aValue)
		{}

		bool operator==(const MapIterator &aIterator) const;
		bool operator!=(const MapIterator &aIterator) const;

		Key& First() { return *myKey; }
		Value& Second() {	return *myValue;}

		int myFirstIndex;
		int mySecondIndex;

	private:
		Key* myKey;
		Value* myValue;
	};

	template<typename Key, typename Value>
	bool MapIterator<Key, Value>::operator==(const MapIterator &aIterator) const
	{
		return myFirstIndex == aIterator.myFirstIndex && mySecondIndex == aIterator.mySecondIndex;
	}

	template<typename Key, typename Value>
	bool MapIterator<Key, Value>::operator!=(const MapIterator &aIterator) const
	{
		return (*this == aIterator) == false;
	}
}