#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>         // istream, ostream
#include <ostream>          // endl
#include <vector>           // vector
#include <iterator>         // istream_iterator, ostream_iterator, back_inserter
#include <memory>           // allocator
#include <initializer_list> // initializer_list
#include <exception>        // out_of_range
#include <utility>          // move, swap, pair
#include <algorithm>        // copy_n


template<typename T, typename Alloc=std::allocator<T>>
class matrix {
private:
	typedef std::vector<T, Alloc> Storage;

public:
	typedef typename Storage::size_type              size_type;
	typedef typename Storage::difference_type        difference_type;
	typedef typename Storage::value_type             value_type;
	typedef typename Storage::reference              reference;
	typedef typename Storage::const_reference        const_reference;
	typedef typename Storage::iterator               iterator;
	typedef typename Storage::const_iterator         const_iterator;
	typedef typename Storage::reverse_iterator       reverse_iterator;
	typedef typename Storage::const_reverse_iterator const_reverse_iterator;

private:
	size_type _size1;
	size_type _size2;
	Storage   _storage;

public:
	matrix()
		: _size1(0), _size2(0), _storage()
	{}

	matrix(size_type s1, size_type s2)
		: _size1(s1), _size2(s2), _storage(s1*s2)
	{}
	
	template<typename R>
	matrix(size_type s1, size_type s2, const R& v)
		: _size1(s1), _size2(s2), _storage(s1*s2, v)
	{}

	matrix(const matrix& m)
		: _size1(m._size1)
		, _size2(m._size2)
		, _storage(m._storage)
	{}

	template<typename R>
	matrix(const matrix<R>& m)
		: _size1(m._size1)
		, _size2(m._size2)
		, _storage(m._storage)
	{}

	~matrix()
	{}

	bool empty() const
	{
		return _size1 == 0 && _size2 == 0;
	}

	operator bool() const
	{
		return !empty();
	}

	size_type size() const
	{
		return _storage.size();
	}
	
	size_type max_size() const
	{
		return _storage.max_size();
	}

	template<typename R>
	bool operator==(const matrix<R>& lvalue) const {
		return
			   _size1   == lvalue._size1
			&& _size2   == lvalue._size2
			&& _storage == lvalue._storage
			;
	}
	
	template<typename R>
	bool operator!=(const matrix<R>& lvalue) const {
		return !(*this == lvalue);
	}

	template<typename R>
	matrix& operator=(const matrix<R>& lvalue) {
		*this = std::move(matrix(lvalue));
		return *this;
	}

	void swap(const matrix& m) {
		std::swap(_storage, m._storage);
		std::swap(_size1, m._size1);
		std::swap(_size2, m._size2);
	}

	iterator begin() {
		return _storage.begin();
	}

	iterator end() {
		return _storage.end();
	}

	const_iterator begin() const {
		return _storage.end();
	}

	const_iterator end() const {
		return _storage.end();
	}

	reverse_iterator rbegin() {
		return _storage.rbegin();
	}

	reverse_iterator rend() {
		return _storage.rend();
	}

	const_reverse_iterator rbegin() const {
		return _storage.rbegin();
	}

	const_reverse_iterator rend() const {
		return _storage.rend();
	}

	void clear() {
		_size1 = 0;
		_size2 = 0;
		_storage.clear();
	}

	size_type size1() const {
		return _size1;
	}

	size_type size2() const {
		return _size2;
	}
	
	const_reference operator()(size_type x, size_type y) const {
		return _storage[x*_size2 + y];
	}

	reference operator()(size_type x, size_type y) {
		return _storage[x*_size2 + y];
		/*return
			const_cast<reference>(
				static_cast<const matrix<T>&>(*this).at(x, y));*/
	}

	const_reference operator()(std::pair<size_type, size_type> p) const {
		return (*this)(p.first, p.second);
	}

	reference operator()(std::pair<size_type, size_type> p) {
		return (*this)(p.first, p.second);
	}

	const_reference at(size_type x, size_type y) const {
        /*if(x < 0 || x >= _size1)
			throw std::out_of_range("Exceed size1.");
		if(y < 0 || y >= _size2)
            throw std::out_of_range("Exceed size2.");*/
		return (*this)(x, y);
	}

	reference at(size_type x, size_type y) {
        /*if(x < 0 || x >= _size1)
			throw std::out_of_range("Exceed size1.");
		if(y < 0 || y >= _size2)
            throw std::out_of_range("Exceed size2.");*/
		return (*this)(x, y);
		/*return
			const_cast<reference>(
				static_cast<const matrix<T>&>(*this).at(x, y));*/
	}

	const_reference at(std::pair<size_type, size_type> p) const {
		return (*this).at(p.first, p.second);
	}

	reference at(std::pair<size_type, size_type> p) {
		return (*this).at(p.first, p.second);
	}


	friend std::istream& operator>>(std::istream& is, matrix<T>& m) {
		is >> m._size1 >> m._size2;
		m._storage.clear();
		m._storage.reserve(m._size1*m._size2);
		std::copy_n
			( std::istream_iterator<T>(is)
			, m._size1*m._size2
			, std::back_inserter(m._storage)
			);

		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
		//os << m._size1 << " " << m._size2 << std::endl;
		for( auto it = m._storage.cbegin()
		   ; it != m._storage.cend()
		   ; it += m._size2)
		{
			std::copy_n
				( it
				, m._size2
				, std::ostream_iterator<T>(os, " ")
				);
			os << std::endl;
		}

		return os;
	}
};

template<typename T1, typename T2>
void swap(const matrix<T1>& m1, const matrix<T2>& m2)
{
	m1.swap(m2);
}
#endif
