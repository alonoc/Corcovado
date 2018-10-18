#ifndef CORCOVADO_H_
#define CORCOVADO_H_

#include <cstddef>
#include <memory>
#include <iterator>

namespace CorcoAlgebra
{

	template<typename T>
	class mat_iterator
	{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::input_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		mat_iterator(pointer ptr) : m_Ptr(ptr) {}

		// pre-increment operator
		mat_iterator& operator++() { ++m_Ptr; return *this; }
		// pre-decrement operator
		mat_iterator& operator--() { --m_Ptr; return *this; }
		// post-increment operator
		mat_iterator operator++(int Dummy) { return mat_iterator(++m_Ptr); }
		// post-decrement operator
		mat_iterator operator--(int Dummy) { return mat_iterator(--m_Ptr); }

		// Equality/ inequality operators
		bool operator==(const mat_iterator& rhs) const { return m_Ptr == rhs.m_Ptr; }
		bool operator!=(const mat_iterator& rhs) const { return m_Ptr != rhs.m_Ptr; }

		// dereferenced
		reference operator*() { return *m_Ptr; }
		pointer operator->() { return m_Ptr; }

	private:
		pointer m_Ptr;
	};

	template<typename T>
	class Mat
	{
	public:
		// Iterators
		typedef mat_iterator<T> iterator;
		typedef mat_iterator<const T> const_iterator;

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef iterator row_iterator;
		typedef const_iterator const_row_iterator;

		// Constructors
		Mat(const std::size_t Rows, const std::size_t Columns);
		Mat(const std::size_t Rows, const std::size_t Columns, const T& DefaultValue);

		// Destructor
		virtual ~Mat();

		// Getters
		std::size_t rows() const noexcept;
		std::size_t cols() const noexcept;
		std::size_t size() const noexcept;

		// Element Access methods
		T& at(const std::size_t Row, const std::size_t Column);
		const T& at(const std::size_t Row, const std::size_t Column) const;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		reverse_iterator rbegin();
		reverse_iterator rend();

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		row_iterator begin_row(const size_t Row);
		row_iterator end_row(const size_t Row);

		const_row_iterator begin_row(const size_t Row) const;
		const_row_iterator end_row(const size_t Row) const;

		const_row_iterator cbegin_row(const size_t Row) const;
		const_row_iterator cend_row(const size_t Row) const;

	private:
		std::unique_ptr<T[]> mp_MatData;
		std::size_t m_NumberOfRows;
		std::size_t m_NumberOfCols;
		std::size_t m_Size;
	};

	template<typename T>
	Mat<T>::Mat(const std::size_t Rows, const std::size_t Columns) : m_NumberOfRows(Rows), m_NumberOfCols(Columns), m_Size(Rows*Columns)
	{
		if(0 == Rows || 0 == Columns)
		{
			throw std::invalid_argument("Number of rows/columns must be higher than 0");
		}
		mp_MatData.reset(new T[m_Size]);
	}

	template<typename T>
	Mat<T>::Mat(const std::size_t Rows, const std::size_t Columns, const T& DefaultValue) : Mat(Rows, Columns)
	{
		std::fill(&mp_MatData[0], &mp_MatData[0] + m_Size, DefaultValue);
	}

	template<typename T>
	Mat<T>::~Mat()
	{

	}

	template<typename T>
	inline std::size_t Mat<T>::rows() const noexcept
	{
		return m_NumberOfRows;
	}

	template<typename T>
	inline std::size_t Mat<T>::cols() const noexcept
	{
		return m_NumberOfCols;
	}

	template<typename T>
	inline std::size_t Mat<T>::size() const noexcept
	{
		return m_Size;
	}

	template<typename T>
	T& Mat<T>::at(const std::size_t Row, const std::size_t Column)
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		else if(Column >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		const size_t ElementPosition = (Row * m_NumberOfCols) + Column;
		return mp_MatData[ElementPosition];
	}
	
	template<typename T>
	const T& Mat<T>::at(const std::size_t Row, const std::size_t Column) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		else if(Column >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		const size_t ElementPosition = (Row * m_NumberOfCols) + Column;
		return mp_MatData[ElementPosition];
	}

	/*===============================================================
	 *
	 *==================> STANDARD ITERATORS <=======================
	 *
	 *===============================================================
	 */
	template<typename T>
	typename Mat<T>::iterator Mat<T>::begin()
	{
		return iterator(mp_MatData.get());
	}

	template<typename T>
	typename Mat<T>::iterator Mat<T>::end()
	{
		return iterator(mp_MatData.get() + m_Size);
	}

	template<typename T>
	typename Mat<T>::const_iterator Mat<T>::begin() const
	{
		return const_iterator(mp_MatData.get());
	}

	template<typename T>
	typename Mat<T>::const_iterator Mat<T>::end() const
	{
		return const_iterator(mp_MatData.get() + m_Size);
	}

	template<typename T>
	typename Mat<T>::const_iterator Mat<T>::cbegin() const noexcept
	{
		return const_iterator(mp_MatData.get());
	}

	template<typename T>
	typename Mat<T>::const_iterator Mat<T>::cend() const noexcept
	{
		return const_iterator(mp_MatData.get() + m_Size);
	}

	/*===============================================================
	 *
	 *===================> REVERSE ITERATORS <=======================
	 *
	 *===============================================================
	 */
	template<typename T>
	typename Mat<T>::reverse_iterator Mat<T>::rbegin()
	{
		return reverse_iterator(mp_MatData.get() + m_Size);
	}

	template<typename T>
	typename Mat<T>::reverse_iterator Mat<T>::rend()
	{
		return reverse_iterator(mp_MatData.get());
	}

	template<typename T>
	typename Mat<T>::const_reverse_iterator Mat<T>::rbegin() const
	{
		return const_reverse_iterator(mp_MatData.get() + m_Size);
	}

	template<typename T>
	typename Mat<T>::const_reverse_iterator Mat<T>::rend() const
	{
		return const_reverse_iterator(mp_MatData.get());
	}

	template<typename T>
	typename Mat<T>::const_reverse_iterator Mat<T>::crbegin() const noexcept
	{
		return const_reverse_iterator(mp_MatData.get() + m_Size);
	}

	template<typename T>
	typename Mat<T>::const_reverse_iterator Mat<T>::crend() const noexcept
	{
		return const_reverse_iterator(mp_MatData.get());
	}

	/*===============================================================
	 *
	 *======================> ROW ITERATORS <========================
	 *
	 *===============================================================
	 */
	template<typename T>
	typename Mat<T>::row_iterator Mat<T>::begin_row(const size_t Row)
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*Row;
		return row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::row_iterator Mat<T>::end_row(const size_t Row)
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*(Row+1);
		return row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::begin_row(const size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*Row;
		return const_row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::end_row(const size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*(Row+1);
		return const_row_iterator(mp_MatData.get() + offset);
	}
	
	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::cbegin_row(const size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*Row;
		return const_row_iterator(mp_MatData.get() + offset);
	}
	
	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::cend_row(const size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*(Row+1);
		return const_row_iterator(mp_MatData.get() + offset);
	}

	using imat = CorcoAlgebra::Mat<int>;
	using fmat = CorcoAlgebra::Mat<float>;
	using dmat = CorcoAlgebra::Mat<double>;
	using uimat = CorcoAlgebra::Mat<unsigned int>;
}

#endif