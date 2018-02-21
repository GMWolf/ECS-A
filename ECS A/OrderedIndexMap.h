#pragma once
#include <vector>

namespace ecsa {

	template<class T>
	class OrderedIndexMap
	{
	public:
		OrderedIndexMap();
		~OrderedIndexMap();
		template<class... Ts>
		void emplace(int key, Ts... item);

		void erase(int key);

		T& get(int key);

		T& operator[](int key);

		void reserve(int capacity);


	private:
		std::vector<int> offsets;
		std::vector<T> elements;
	};

	template<class T>
	inline OrderedIndexMap<T>::OrderedIndexMap()
	{
	}

	template<class T>
	inline OrderedIndexMap<T>::~OrderedIndexMap()
	{
	}

	template<class T>
	template<class... Ts>
	inline void OrderedIndexMap<T>::emplace(int key, Ts... args)
	{
		if (key >= offsets.size()) {
			offsets.resize(key+1, -1);
		}

		//get previous key
		int pk;
		for (pk = key; offsets[pk] != -1 && pk >= 0; pk--) {
		}
		int offset = (pk == -1) ? 0 : (offsets[pk] + 1);

		//insert element
		
		elements.insert(elements.begin() + offset, T(args...));
		offsets[key] = offset;

		//adjust offsets
		for (int i = key + 1; i < offsets.size(); i++) {
			if (offsets[i] != -1) {
				offsets[i]++;
			}
		}
	}

	template<class T>
	inline void OrderedIndexMap<T>::erase(int key)
	{
		//remove element
		elements.erase(elements.begin() + offsets[key]);

		//fix offsets
		for (int i = key; i < offsets.size(); i++) {
			offsets[i]--;
		}

	}

	template<class T>
	inline T & OrderedIndexMap<T>::get(int key)
	{
		if (offsets[key] != -1) {
			return operator[](key);
		}
		throw std::out_of_range("Key does not have element associated with it.");
	}

	template<class T>
	inline T & OrderedIndexMap<T>::operator[](int key)
	{
		return elements[offsets[key]];
	}

	template<class T>
	inline void OrderedIndexMap<T>::reserve(int capacity)
	{
		elements.reserve(capacity);
	}


}