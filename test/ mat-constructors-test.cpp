#include "corcovado.h"
#include "gtest/gtest.h"

using namespace CorcoAlgebra;

template<typename T>
class MatConstructorTest : public ::testing::Test
{
public:
	typedef Mat<T> Matrix;

	T GetDefaultValue() const noexcept;
};

typedef ::testing::Types<int,float,double> MatTypes;
TYPED_TEST_CASE(MatConstructorTest, MatTypes);

template<>
int MatConstructorTest<int>::GetDefaultValue() const noexcept
{
	return 10;
}

template<>
float MatConstructorTest<float>::GetDefaultValue() const noexcept
{
	return 4.5;
}

template<>
double MatConstructorTest<double>::GetDefaultValue() const noexcept
{
	return 12.5;
}

TYPED_TEST(MatConstructorTest, DefaultConstructorRowsAndColsHigherThanZeroSucceds)
{
	// Arrange
	const size_t Rows = 3;
	const size_t Cols = 2;
	const size_t MatSize = Rows*Cols;

	// Act [calling default constructor]
	typename TestFixture::Matrix TestMat(Rows, Cols);

	// Assert
	ASSERT_EQ(Rows, TestMat.rows());
	ASSERT_EQ(Cols, TestMat.cols());
	ASSERT_EQ(MatSize, TestMat.size());
}

TYPED_TEST(MatConstructorTest, ConstructorWithDefaultValueAllElementsOfTheMatrixAreInitilizaedWithIt)
{
	// Arrange
	const size_t Rows = 3;
	const size_t Cols = 6;
	const size_t MatSize = Rows*Cols;
	const TypeParam DefaultValue = this->GetDefaultValue();

	// Act [calling constructor with default value]
	typename TestFixture::Matrix TestMat(Rows, Cols, DefaultValue);
	
	// Assert
	ASSERT_EQ(Rows, TestMat.rows());
	ASSERT_EQ(Cols, TestMat.cols());
	ASSERT_EQ(MatSize, TestMat.size());

	// Check that all the elements of the matrix are equal to the default value passed by paremeter in the constructor
	for(size_t RowIndex = 0; RowIndex < Rows; RowIndex++)
	{
		for(size_t ColIndex = 0; ColIndex < Cols; ColIndex++)
		{
			ASSERT_EQ(DefaultValue, TestMat.at(RowIndex, ColIndex)) << " Matrix missmatch in row = " << RowIndex << ", col = " << ColIndex;
		}
	}
}