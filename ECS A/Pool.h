#pragma once
#include <queue>
template<class T>
class Pool {
public:

	Pool(int maxItems);

	template<class... Ts>
	T* get(Ts... args);

	void free(T* obj);

private:
	int maxItems;
	std::queue<T*> items;
};



template<class T>
inline Pool<T>::Pool(int maxItems) : maxItems(maxItems)
{

}

template<class T>
inline void Pool<T>::free(T * obj)
{
	if (items.size() >= maxItems) {
		delete(obj);
	}
	else {
		items.push(obj);
	}
}


template<class T>
template<class ...Ts>
inline T * Pool<T>::get(Ts ...args)
{
	if (items.empty()) {
		return new T(args...);
	} 

	T* ret = items.back();
	items.pop();
	return ret;
}
