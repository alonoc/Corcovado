#ifndef CORCOVADO_H_
#define CORCOVADO_H_

#include <cstddef>
#include <memory>
#include <iterator>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

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
	class mat_col_iterator
	{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::input_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		mat_col_iterator(pointer ptr, const std::size_t NumOfCols) : m_Ptr(ptr), m_NumberOfCols(NumOfCols) {}

		// pre-increment operator
		mat_col_iterator& operator++() { m_Ptr += m_NumberOfCols; return *this; }
		// pre-decrement operator
		mat_col_iterator& operator--() { m_Ptr -= m_NumberOfCols; return *this; }
		// post-increment operator
		mat_col_iterator operator++(int Dummy) { return mat_col_iterator(m_Ptr+m_NumberOfCols, m_NumberOfCols); }
		// post-decrement operator
		mat_col_iterator operator--(int Dummy) { return mat_col_iterator(m_Ptr-m_NumberOfCols, m_NumberOfCols); }

		// Equality/ inequality operators
		bool operator==(const mat_col_iterator& rhs) const { return m_Ptr == rhs.m_Ptr; }
		bool operator!=(const mat_col_iterator& rhs) const { return m_Ptr != rhs.m_Ptr; }

		// dereferenced
		reference operator*() { return *m_Ptr; }
		pointer operator->() { return m_Ptr; }
		
	private:
		std::size_t m_NumberOfCols;
		pointer m_Ptr;
	};

	template<typename InputIt, typename BinaryOp>
	void for_each(InputIt first1, InputIt last1, InputIt first2, InputIt last2, BinaryOp binary_op)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			binary_op(*first1, *first2);
		}
	}

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
		typedef mat_col_iterator<T> col_iterator;
		typedef mat_col_iterator<const T> const_col_iterator;

		// Constructors
		Mat(const std::size_t Rows, const std::size_t Columns);
		Mat(const std::size_t Rows, const std::size_t Columns, const T& DefaultValue);
		Mat(const std::initializer_list<std::initializer_list<T>>& InitMat);
		Mat(const Mat<T>& Rhs);
		Mat(Mat<T>&& Lhs);

		// Assign Operators
		Mat<T>& operator=(const Mat<T>& Rhs);
		Mat<T>& operator=(Mat<T>&& Lhs);

		// Destructor
		virtual ~Mat();

		// Getters
		std::size_t rows() const noexcept;
		std::size_t cols() const noexcept;
		std::size_t size() const noexcept;

		// Element Access methods
		T& at(const std::size_t Row, const std::size_t Column);
		const T& at(const std::size_t Row, const std::size_t Column) const;

		// Arithmetic Operators
		Mat<T> operator+(const Mat<T>& Rhs) const;
		Mat<T> operator-(const Mat<T>& Rhs) const;

		// Standard Iterators
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		// Reverse Iterators
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// Row Iterators
		row_iterator begin_row(const std::size_t Row);
		row_iterator end_row(const std::size_t Row);
		const_row_iterator begin_row(const std::size_t Row) const;
		const_row_iterator end_row(const std::size_t Row) const;
		const_row_iterator cbegin_row(const std::size_t Row) const;
		const_row_iterator cend_row(const std::size_t Row) const;

		// Col Iterators
		col_iterator begin_col(const std::size_t Col);
		col_iterator end_col(const std::size_t Col);
		const_col_iterator begin_col(const std::size_t Col) const;
		const_col_iterator end_col(const std::size_t Col) const;
		const_col_iterator cbegin_col(const std::size_t Col) const;
		const_col_iterator cend_col(const std::size_t Col) const;

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
		std::fill(mp_MatData.get(), mp_MatData.get() + m_Size, DefaultValue);
	}

	template<typename T>
	Mat<T>::Mat(const std::initializer_list<std::initializer_list<T>>& InitMat)
	{
		if(0 == InitMat.size())
		{
			throw std::invalid_argument("Is not possible to create an empty matrix");
		}
		else if(0 == InitMat.begin()->size())
		{
			throw std::invalid_argument("Is not possible to create a matrix with zero columns");
		}

		size_t PosOfRowFirstElement = 0;
		m_NumberOfRows = InitMat.size();
		m_NumberOfCols = InitMat.begin()->size();
		m_Size = m_NumberOfRows*m_NumberOfCols;
		mp_MatData.reset(new T[m_Size]);

		for(const auto& RowList : InitMat)
		{
			const size_t ColSize = RowList.size();
			if(ColSize != m_NumberOfCols)
			{
				throw std::invalid_argument("Is not possible to create a matrix with a different number of columns per row");
			}
			std::copy(RowList.begin(), RowList.end(), &mp_MatData[PosOfRowFirstElement]);
			PosOfRowFirstElement += m_NumberOfCols;
		}
	}

	template<typename T>
	Mat<T>::Mat(const Mat<T>& Rhs)
	{
		if(0 == Rhs.m_NumberOfRows || 0 == Rhs.m_NumberOfCols || nullptr == Rhs.mp_MatData)
		{
			throw std::invalid_argument("To copy constructor a matrix, the other matrix should be a valid one (Rows and columns != 0 and the data differnet than nullptr)");
		}
		m_NumberOfRows = Rhs.m_NumberOfRows;
		m_NumberOfCols = Rhs.m_NumberOfCols;
		m_Size = Rhs.m_Size;
		mp_MatData.reset(new T[m_Size]);
		std::copy(Rhs.mp_MatData.get(), Rhs.mp_MatData.get() + Rhs.m_Size, mp_MatData.get());
	}

	template<typename T>
	Mat<T>::Mat(Mat<T>&& Lhs)
	{		
		// Take members of LHS
		m_NumberOfRows = Lhs.m_NumberOfRows;
		m_NumberOfCols = Lhs.m_NumberOfCols;
		m_Size = Lhs.m_Size;
		mp_MatData = std::move(Lhs.mp_MatData);
		
		// Set Lhs members to invalid values
		Lhs.m_NumberOfRows = 0;
		Lhs.m_NumberOfCols = 0;
		Lhs.m_Size = 0;
	}

	template<typename T>
	Mat<T>& Mat<T>::operator=(const Mat<T>& Rhs)
	{
		if(this != &Rhs)
		{
			m_NumberOfRows = Rhs.m_NumberOfRows;
			m_NumberOfCols = Rhs.m_NumberOfCols;
			// If sizes are different then we have to allocate memory
			if(Rhs.m_Size != m_Size)
			{
				m_Size = Rhs.m_Size;
				mp_MatData.reset(new T[m_Size]);
			}
			std::copy(Rhs.mp_MatData.get(), Rhs.mp_MatData.get() + Rhs.m_Size, mp_MatData.get());
		}
		return *this;
	}
	
	template<typename T>
	Mat<T>& Mat<T>::operator=(Mat<T>&& Lhs)
	{
		// Take members of LHS
		m_NumberOfRows = Lhs.m_NumberOfRows;
		m_NumberOfCols = Lhs.m_NumberOfCols;
		m_Size = Lhs.m_Size;
		mp_MatData = std::move(Lhs.mp_MatData);
		
		// Set LHS members to invalid values
		Lhs.m_NumberOfRows = 0;
		Lhs.m_NumberOfCols = 0;
		Lhs.m_Size = 0;
		return *this;
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
	
	template<typename T>
	Mat<T> Mat<T>::operator+(const Mat<T>& Rhs) const
	{
		if(Rhs.m_NumberOfRows != this->m_NumberOfRows || Rhs.m_NumberOfCols != this->m_NumberOfCols)
		{
			throw std::invalid_argument("Cannot add two matrices with a different number of rows or columns");
		}
		Mat<T> result(m_NumberOfRows, m_NumberOfCols);
		auto result_it = result.begin();
		auto addition = [&result_it](const T& Val1, const T& Val2){ *result_it = Val1 + Val2; ++result_it; };
		for_each(this->cbegin(), this->cend(), Rhs.cbegin(), Rhs.cend(), addition);
		return result;
	}
	
	template<typename T>
	Mat<T> Mat<T>::operator-(const Mat<T>& Rhs) const
	{
		if(Rhs.m_NumberOfRows != this->m_NumberOfRows || Rhs.m_NumberOfCols != this->m_NumberOfCols)
		{
			throw std::invalid_argument("Cannot subtract two matrices with a different number of rows or columns");
		}
		Mat<T> result(m_NumberOfRows, m_NumberOfCols);
		auto result_it = result.begin();
		auto subtraction = [&result_it](const T& Val1, const T& Val2){ *result_it = Val1 - Val2; ++result_it; };
		for_each(this->cbegin(), this->cend(), Rhs.cbegin(), Rhs.cend(), subtraction);
		return result;
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
	typename Mat<T>::row_iterator Mat<T>::begin_row(const std::size_t Row)
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const std::size_t offset = m_NumberOfCols*Row;
		return row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::row_iterator Mat<T>::end_row(const std::size_t Row)
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const std::size_t offset = m_NumberOfCols*(Row+1);
		return row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::begin_row(const std::size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const std::size_t offset = m_NumberOfCols*Row;
		return const_row_iterator(mp_MatData.get() + offset);
	}

	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::end_row(const std::size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const std::size_t offset = m_NumberOfCols*(Row+1);
		return const_row_iterator(mp_MatData.get() + offset);
	}
	
	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::cbegin_row(const std::size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const size_t offset = m_NumberOfCols*Row;
		return const_row_iterator(mp_MatData.get() + offset);
	}
	
	template<typename T>
	typename Mat<T>::const_row_iterator Mat<T>::cend_row(const std::size_t Row) const
	{
		if(Row >= m_NumberOfRows)
		{
			throw std::out_of_range("Row index out of range");
		}
		const std::size_t offset = m_NumberOfCols*(Row+1);
		return const_row_iterator(mp_MatData.get() + offset);
	}

	/*===============================================================
	 *
	 *======================> COL ITERATORS <========================
	 *
	 *===============================================================
	 */
	template<typename T>
	typename Mat<T>::col_iterator Mat<T>::begin_col(const std::size_t Col)
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return col_iterator(mp_MatData.get() + Col, m_NumberOfCols);
	}

	template<typename T>
	typename Mat<T>::col_iterator Mat<T>::end_col(const std::size_t Col)
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return col_iterator(mp_MatData.get() + m_Size + Col, m_NumberOfCols);
	}

	template<typename T>
	typename Mat<T>::const_col_iterator Mat<T>::begin_col(const std::size_t Col) const
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return const_col_iterator(mp_MatData.get() + Col, m_NumberOfCols);
	}

	template<typename T>
	typename Mat<T>::const_col_iterator Mat<T>::end_col(const std::size_t Col) const
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return const_col_iterator(mp_MatData.get() + m_Size + Col, m_NumberOfCols);
	}

	template<typename T>
	typename Mat<T>::const_col_iterator Mat<T>::cbegin_col(const std::size_t Col) const
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return const_col_iterator(mp_MatData.get() + Col, m_NumberOfCols);
	}

	template<typename T>
	typename Mat<T>::const_col_iterator Mat<T>::cend_col(const std::size_t Col) const
	{
		if(Col >= m_NumberOfCols)
		{
			throw std::out_of_range("Column index out of range");
		}
		return const_col_iterator(mp_MatData.get() + m_Size + Col, m_NumberOfCols);
	}

	using imat = CorcoAlgebra::Mat<int>;
	using fmat = CorcoAlgebra::Mat<float>;
	using dmat = CorcoAlgebra::Mat<double>;
	using uimat = CorcoAlgebra::Mat<unsigned int>;
}

#endif