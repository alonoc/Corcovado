#ifndef CORCOVADO_H_
#define CORCOVADO_H_

#include <cstddef>
#include <memory>

namespace CorcoAlgebra
{
	template<typename T>
	class Mat
	{
	public:
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
}

using imat 		= CorcoAlgebra::Mat<int>;
using uimat 	= CorcoAlgebra::Mat<unsigned int>;
using fmat 		= CorcoAlgebra::Mat<float>;
using dmat 		= CorcoAlgebra::Mat<double>;

#endif