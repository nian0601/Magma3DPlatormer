#pragma once

#include "CUMapIterator.h"
#include "HashHelper.h"
#include "GrowingArray.h"

namespace CU
{
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	class Map
	{
	public:
		Map();
		~Map();

		void Insert(const Key &aKey, const Value &aValue);
		Value& Get(const Key &aKey);
		const Value& Get(const Key &aKey) const;
		void Remove(const Key &aKey);
		bool KeyExists(const Key &aKey) const;
		Value& operator[](const Key &aKey);

		MapIterator<Key, Value> Begin();
		MapIterator<Key, Value> Next(MapIterator<Key, Value>& aCurrent);
		MapIterator<Key, Value> End();

		void Clear();

	private:
		struct KeyValuePair
		{
			Key myKey;
			Value myValue;
		};

		int FindKeyInBucket(int aBucketIndex, const Key& aKey) const;

		int OwnHash(const Key &aKey) const;
		GrowingArray<GrowingArray<KeyValuePair, int>, int> myBuckets;
	};


	//----- CONSTRUCTOR -----
	//------------------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	Map<Key, Value, StartSize, BucketSize>::Map()
	{
		myBuckets.Reserve(StartSize);

		for (int i = 0; i < myBuckets.Size(); ++i)
		{
			myBuckets[i].Init(BucketSize);
		}
	}


	//----- DECONSTRUCTOR ----
	//------------------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	Map<Key, Value, StartSize, BucketSize>::~Map()
	{

	}


	//----- INSERT -----
	//------------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	void Map<Key, Value, StartSize, BucketSize>::Insert(const Key &aKey, const Value &aValue)
	{
		if (KeyExists(aKey) == true)
		{
			operator[](aKey) = aValue;
			return;
		}

		int index = OwnHash(aKey);

		KeyValuePair pair;
		pair.myKey = aKey;
		pair.myValue = aValue;

		myBuckets[index].Add(pair);
	}


	//----- GET -----
	//---------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	Value& Map<Key, Value, StartSize, BucketSize>::Get(const Key &aKey)
	{
		int index = OwnHash(aKey);
		int keyIndex = FindKeyInBucket(index, aKey);
		assert(keyIndex != -1 && "[CUMap]: Tried to get an nonexisting Key.");

		return myBuckets[index][keyIndex].myValue;
	}

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	const Value& Map<Key, Value, StartSize, BucketSize>::Get(const Key &aKey) const
	{
		int index = OwnHash(aKey);
		int keyIndex = FindKeyInBucket(index, aKey);
		assert(keyIndex != -1 && "[CUMap]: Tried to get an nonexisting Key.");

		return myBuckets[index][keyIndex].myValue;
	}


	//----- DELETE -----
	//------------------

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	void Map<Key, Value, StartSize, BucketSize>::Remove(const Key &aKey)
	{
		assert(KeyExists(aKey) == true && "[CUMap]: Tried to delete an nonexisting Key.");

		int index = OwnHash(aKey);

		for (int i = 0; i < myBuckets[index].Size(); ++i)
		{
			if (myBuckets[index][i].myKey == aKey)
			{
				myBuckets[index].RemoveCyclicAtIndex(i);
				return;
			}
		}
	}


	//----- KEY_EXISTS -----
	//---------------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	bool Map<Key, Value, StartSize, BucketSize>::KeyExists(const Key &aKey) const
	{
		int index = OwnHash(aKey);
		return FindKeyInBucket(index, aKey) != -1;
	}


	//----- SUBSCRIPT_OPERATOR -----
	//------------------------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	Value& Map<Key, Value, StartSize, BucketSize>::operator[](const Key &aKey)
	{
		int index = OwnHash(aKey);
		if (FindKeyInBucket(index, aKey) == -1)
		{
			KeyValuePair pair;
			pair.myKey = aKey;
			pair.myValue = Value();

			CU::GrowingArray<KeyValuePair, int>& bucket = myBuckets[index];
			bucket.Add(pair);
			return bucket.GetLast().myValue;
		}

		return Get(aKey);
	}

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	MapIterator<Key, Value> Map<Key, Value, StartSize, BucketSize>::Begin()
	{
		int first = -1;
		int second = -1;
		Key* key = nullptr;
		Value* value = nullptr;

		for (int i = 0; i < myBuckets.Size(); ++i)
		{
			if (myBuckets[i].Size() > 0)
			{
				first = i;
				second = 0;

				key = &myBuckets[first][second].myKey;
				value = &myBuckets[first][second].myValue;
				break;
			}
		}

		return MapIterator<Key, Value>(first, second, key, value);
	}

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	MapIterator<Key, Value> Map<Key, Value, StartSize, BucketSize>::Next(MapIterator<Key, Value>& aCurrent)
	{
		int first = -1;
		int second = -1;
		Key* key = nullptr;
		Value* value = nullptr;

		int innerIndex = aCurrent.mySecondIndex + 1;

		for (int i = aCurrent.myFirstIndex; i < myBuckets.Size(); ++i)
		{
			GrowingArray<KeyValuePair, int>& bucket = myBuckets[i];
			for (int j = innerIndex; j < bucket.Size(); ++j)
			{
				first = i;
				second = j;

				key = &myBuckets[first][second].myKey;
				value = &myBuckets[first][second].myValue;
				break;
			}

			innerIndex = 0;
			if (key != nullptr)
			{
				break;
			}

		}

		return MapIterator<Key, Value>(first, second, key, value);
	}

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	MapIterator<Key, Value> Map<Key, Value, StartSize, BucketSize>::End()
	{
		return MapIterator<Key, Value>(-1, -1, nullptr, nullptr);
	}

	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	void Map<Key, Value, StartSize, BucketSize>::Clear()
	{
		for (int i = 0; i < myBuckets.Size(); ++i)
		{
			myBuckets[i].RemoveAll();
		}
	}


	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	int Map<Key, Value, StartSize, BucketSize>::FindKeyInBucket(int aBucketIndex, const Key& aKey) const
	{
		const CU::GrowingArray<KeyValuePair, int>& bucket = myBuckets[aBucketIndex];

		for (int i = 0; i < bucket.Size(); ++i)
		{
			if (bucket[i].myKey == aKey)
				return i;
		}

		return -1;
	}

	//----- HASH -----
	//----------------
	template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
	int Map<Key, Value, StartSize, BucketSize>::OwnHash(const Key &aKey) const
	{
		return Murmur(aKey) % StartSize;
	}
}