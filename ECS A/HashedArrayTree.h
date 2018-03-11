#pragma once
#include <vector>
#include <assert.h>
#include "Pool.h"
#include "SimpleAllocator.h"
template<class T>
struct Chunk {
	Chunk(int capacity) : items(new T[capacity]) {

	}

	~Chunk() {
		delete[] items;
	}

	T& operator[](unsigned int i) {
		return items[i];
	}

	void put(unsigned int i, T& o)
	{
		items[i] == o;
		if (usage.size() <= i)
		{
			usage.resize(i);
		}
		usage[i] = true;
	}

	template<class ...Ts>
	void emplace(unsigned int i, Ts&&... args)
	{
		new (items + i) T(std::forward<Ts>(args)...);
		if (usage.size() <= i)
		{
			usage.resize(i+1);
		}
		usage[i] = true;
	}

	void erase(unsigned int i)
	{
		usage[i] = false;
	}

	T* items;
	vector_bool usage;
};



template<class T>
class HashedArrayTree {
public:
	HashedArrayTree(unsigned int chunkSizeLog);
	HashedArrayTree();
	~HashedArrayTree();

	template<class... Ts>
	T& emplace(int e, Ts&&... args);

	void erase(int e);

	T& operator[](int e);

private:
	int getChunkIndex(int e);
	Chunk<T>* getChunk(int e);
	int getIndex(int e);

	unsigned int chunkSize;
	unsigned int chunkSizeLog;
	unsigned int mask;
	std::vector<Chunk<T>*> chunks;

	CHUNK_ALLOCATOR<Chunk<T>> allocator;
};

template<class T>
inline HashedArrayTree<T>::HashedArrayTree(unsigned int chunkSizeLog) : chunkSizeLog(chunkSizeLog), chunkSize(1 << chunkSizeLog), mask(chunkSize - 1), allocator(2)
{
}

template<class T>
inline HashedArrayTree<T>::HashedArrayTree() : HashedArrayTree(8)
{
}

template<class T>
inline HashedArrayTree<T>::~HashedArrayTree()
{
}

template<class T>
inline void HashedArrayTree<T>::erase(int e)
{
	int chunkIndex = getChunkIndex(e);
	Chunk<T>* chunk = chunks[chunkIndex];
	chunk->erase(getIndex(e));

	if (std::find(chunk->usage.begin(), chunk->usage.end(), true) == chunk->usage.end()) {
		allocator.free(chunk);
		chunks[chunkIndex] = nullptr;
	}
	
}

template<class T>
inline T & HashedArrayTree<T>::operator[](int e)
{
	return (*getChunk(e))[getIndex(e)];
}

template<class T>
inline int HashedArrayTree<T>::getChunkIndex(int e)
{
	return e >> chunkSizeLog; 
}

template<class T>
inline int HashedArrayTree<T>::getIndex(int e)
{
	return e & mask;
}

template<class T>
inline Chunk<T>* HashedArrayTree<T>::getChunk(int e)
{
	int index = getChunkIndex(e);
	if (index >= chunks.size()) {
		chunks.resize(index+1, nullptr);
	}
	return chunks[index];
}



template<class T>
template<class ...Ts>
inline T & HashedArrayTree<T>::emplace(int e, Ts&& ...args)
{

	int chunkIndex = getChunkIndex(e);

	if (chunkIndex >= chunks.size()) {
		chunks.resize(chunkIndex + 1, nullptr);
	}

	if (chunks[chunkIndex] == nullptr){
		chunks[chunkIndex] = allocator.get(chunkSize);
	}

	Chunk<T>* chunk = chunks[chunkIndex];

	int index = getIndex(e);
	chunk->emplace(index, std::forward<Ts>(args)...);
	return (*chunk)[index];
}
