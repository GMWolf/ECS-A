#pragma once
template<class T>
class SimpleAllocator {
public: 

	SimpleAllocator(int dummy) {
	}

	template<class... Ts>
	T* get(Ts... args);

	void free(T* obj);
};

template<class T>
template<class ...Ts>
inline T * SimpleAllocator<T>::get(Ts ...args)
{
	return new T(args...);
}

template<class T>
inline void SimpleAllocator<T>::free(T * obj)
{
	delete obj;
}
