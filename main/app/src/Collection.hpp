#pragma once

#include <iostream>
#include <vector>

template<typename T>
class Collection {

private:
	T** m_elements;
	unsigned int m_size;

private:
	void swap(unsigned int i, unsigned int j);
	void quickSort(int i, int j, int pivot_index);

public:
    class Iterator {
    private:
        T** current;

    public:
        Iterator(T** ptr) : current(ptr) {}
        T* operator*() const { return *current; }
        Iterator& operator++() { ++current; return *this;}
        bool operator!=(const Iterator& other) const { return current != other.current;}
    };

public:
	Collection();
	Collection(const std::vector<T*>& a);

    inline Iterator begin() { return Iterator(m_elements); }
    inline Iterator end() { return Iterator(m_elements + m_size); }

	inline T** getElements() const { return m_elements; }
	inline unsigned int getSize() const { return m_size; }
	inline void sortElements() { quickSort(-1, 0, m_size - 1); }

	void insertElement(T* element);
	void removeElement(T* element);
	T* getElement(unsigned int index) const;

	~Collection();
};

template<class T>
void Collection<T>::swap(unsigned int i, unsigned int j) {
	T* tmp = m_elements[i];
	m_elements[i] = m_elements[j];
	m_elements[j] = tmp;
}

template<typename T>
void Collection<T>::quickSort(int i, int j, int pivot_index) {
	if (j == pivot_index || pivot_index < 0)
		return;

	while (j < pivot_index) {
		if (*m_elements[j] >= *m_elements[pivot_index])
			swap(++i, j);
		j++;
	}
	swap(i + 1, pivot_index);
	quickSort(i, i + 1, pivot_index);
	quickSort(-1, 0, i);
}

template<typename T>
Collection<T>::Collection() {
	m_elements = nullptr;
	m_size = 0;
}

template<typename T>
Collection<T>::Collection(const std::vector<T*>& a) {
	m_size = a.size();
	m_elements = new T * [m_size];
	for (int i = 0; i < m_size; ++i) {
		m_elements[i] = new T;
		*m_elements[i] = *a[i];
	}
}

template<typename T>
void Collection<T>::insertElement(T* element) {
	if (m_size == 0) {
		m_elements = new T * [++m_size];
		m_elements[0] = element;
		return;
	}

	T** newElements = new T * [m_size + 1];
	for (int i = 0; i < m_size; ++i)
		newElements[i] = m_elements[i];
	newElements[m_size] = element;

	delete[] m_elements;
	m_elements = newElements;
	m_size++;
}

template<typename T>
void Collection<T>::removeElement(T* element) {
	int index = 0;
	bool exists = false;
	for (int i = 0; i < m_size; ++i) {
		if (element == m_elements[i]) {
			index = i;
			exists = true;
			break;
		}
	}
	if (exists) {
		if (m_size > 1) {
			T** newElements = new T * [m_size - 1];
			for (int i = 0; i < index; ++i)
				newElements[i] = m_elements[i];

			for (int i = index + 1; i < m_size; ++i)
				newElements[i - 1] = m_elements[i];

			delete[] m_elements;
			m_elements = newElements;
			--m_size;
		}
		else if (m_size == 1) {
			delete[] m_elements;
			m_elements = nullptr;
			--m_size;
		}
	}
	else throw "Collection does not contain the given element!";
}

template<typename T>
T* Collection<T>::getElement(unsigned int index) const {
	if (index < 0 || index >= m_size)
		throw "Index out of bounds for collection!";
	else
		return m_elements[index];
}

template<typename T>
Collection<T>::~Collection() {
	for (int i = 0; i < m_size; ++i)
		delete m_elements[i];
	delete[] m_elements;
}