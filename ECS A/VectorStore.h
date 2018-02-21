#pragma once
#include <vector>
template<class T>
class VectorStore {
public:

	VectorStore() {
		items.reserve(1024);
	}

	template<class... Ts>
	T& emplace(int e, Ts... args);

	void erase(int e);

	T& operator[](int e);

private:
	std::vector<T> items;
};

template<class T>
template<class ...Ts>
inline T & VectorStore<T>::emplace(int e, Ts ...args)
{
	if (e >= items.size()) {
		items.resize((e + 1));
	}

	new (items.data() + e) T(args...);
	return items[e];
}

template<class T>
inline void VectorStore<T>::erase(int e)
{
}

template<class T>
inline T & VectorStore<T>::operator[](int e)
{
	return items[e];
}
