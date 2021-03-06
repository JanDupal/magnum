#ifndef Magnum_Math_Matrix_h
#define Magnum_Math_Matrix_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Class Magnum::Math::Matrix
 */

#include "RectangularMatrix.h"

namespace Magnum { namespace Math {

namespace Implementation {
    template<std::size_t size, class T> class MatrixDeterminant;
}

/**
@brief Square matrix
@tparam size    %Matrix size
@tparam T       Data type

See @ref matrix-vector for brief introduction.

@configurationvalueref{Magnum::Math::Matrix}
@see Magnum::Matrix2, Magnum::Matrix2d
 */
template<std::size_t size, class T> class Matrix: public RectangularMatrix<size, size, T> {
    public:
        const static std::size_t Size = size; /**< @brief %Matrix size */

        /** @brief Pass to constructor to create zero-filled matrix */
        enum ZeroType { Zero };

        /**
         * @brief Zero-filled matrix constructor
         *
         * Use this constructor by calling `Matrix m(Matrix::Zero);`.
         */
        inline constexpr explicit Matrix(ZeroType) {}

        /** @brief Pass to constructor to create identity matrix */
        enum IdentityType { Identity };

        /**
         * @brief Default constructor
         *
         * You can also explicitly call this constructor with
         * `Matrix m(Matrix::Identity);`. Optional parameter @p value allows
         * you to specify value on diagonal.
         * @todo use constexpr fromDiagonal() for this when it's done
         */
        inline /*implicit*/ Matrix(IdentityType = Identity, T value = T(1)) {
            for(std::size_t i = 0; i != size; ++i)
                (*this)[i][i] = value;
        }

        /**
         * @brief %Matrix from column vectors
         * @param first First column vector
         * @param next  Next column vectors
         */
        template<class ...U> inline constexpr /*implicit*/ Matrix(const Vector<size, T>& first, const U&... next): RectangularMatrix<size, size, T>(first, next...) {}

        /**
         * @brief Construct matrix from another of different type
         *
         * Performs only default casting on the values, no rounding or
         * anything else. Example usage:
         * @code
         * Matrix<2, Float> floatingPoint({1.3f, 2.7f},
         *                                {-15.0f, 7.0f});
         * Matrix<2, Byte> integral(floatingPoint);
         * // integral == {{1, 2}, {-15, 7}}
         * @endcode
         */
        template<class U> inline constexpr explicit Matrix(const RectangularMatrix<size, size, U>& other): RectangularMatrix<size, size, T>(other) {}

        /** @brief Construct matrix from external representation */
        template<class U, class V = decltype(Implementation::RectangularMatrixConverter<size, size, T, U>::from(std::declval<U>()))> inline constexpr explicit Matrix(const U& other): RectangularMatrix<size, size, T>(Implementation::RectangularMatrixConverter<size, size, T, U>::from(other)) {}

        /** @brief Copy constructor */
        inline constexpr Matrix(const RectangularMatrix<size, size, T>& other): RectangularMatrix<size, size, T>(other) {}

        /**
         * @brief Whether the matrix is orthogonal
         *
         * The matrix is orthogonal if its transpose is equal to its inverse: @f[
         *      Q^T = Q^{-1}
         * @f]
         * @see transposed(), inverted(), Matrix3::isRigidTransformation(),
         *      Matrix4::isRigidTransformation()
         */
        bool isOrthogonal() const {
            /* Normality */
            for(std::size_t i = 0; i != size; ++i)
                if(!(*this)[i].isNormalized()) return false;

            /* Orthogonality */
            for(std::size_t i = 0; i != size-1; ++i)
                for(std::size_t j = i+1; j != size; ++j)
                    if(Vector<size, T>::dot((*this)[i], (*this)[j]) > TypeTraits<T>::epsilon())
                        return false;

            return true;
        }

        /**
         * @brief Trace of the matrix
         *
         * @f[
         * tr(A) = \sum_{i=1}^n a_{i,i}
         * @f]
         */
        T trace() const {
            return this->diagonal().sum();
        }

        /** @brief %Matrix without given column and row */
        Matrix<size-1, T> ij(std::size_t skipCol, std::size_t skipRow) const {
            Matrix<size-1, T> out(Matrix<size-1, T>::Zero);

            for(std::size_t col = 0; col != size-1; ++col)
                for(std::size_t row = 0; row != size-1; ++row)
                    out[col][row] = (*this)[col + (col >= skipCol)]
                                           [row + (row >= skipRow)];

            return out;
        }

        /**
         * @brief Determinant
         *
         * Computed recursively using Laplace's formula: @f[
         *      \det(A) = \sum_{j=1}^n (-1)^{i+j} a_{i,j} \det(A^{i,j})
         * @f] @f$ A^{i, j} @f$ is matrix without i-th row and j-th column, see
         * ij(). The formula is expanded down to 2x2 matrix, where the
         * determinant is computed directly: @f[
         *      \det(A) = a_{0, 0} a_{1, 1} - a_{1, 0} a_{0, 1}
         * @f]
         */
        inline T determinant() const { return Implementation::MatrixDeterminant<size, T>()(*this); }

        /**
         * @brief Inverted matrix
         *
         * Computed using Cramer's rule: @f[
         *      A^{-1} = \frac{1}{\det(A)} Adj(A)
         * @f]
         * See invertedOrthogonal(), Matrix3::invertedRigid() and Matrix4::invertedRigid()
         * which are faster alternatives for particular matrix types.
         */
        Matrix<size, T> inverted() const {
            Matrix<size, T> out(Zero);

            T _determinant = determinant();

            for(std::size_t col = 0; col != size; ++col)
                for(std::size_t row = 0; row != size; ++row)
                    out[col][row] = (((row+col) & 1) ? -1 : 1)*ij(row, col).determinant()/_determinant;

            return out;
        }

        /**
         * @brief Inverted orthogonal matrix
         *
         * Equivalent to transposed(), expects that the matrix is orthogonal. @f[
         *      A^{-1} = A^T
         * @f]
         * @see inverted(), isOrthogonal(), Matrix3::invertedRigid(),
         *      Matrix4::invertedRigid()
         */
        inline Matrix<size, T> invertedOrthogonal() const {
            CORRADE_ASSERT(isOrthogonal(),
                "Math::Matrix::invertedOrthogonal(): the matrix is not orthogonal", {});
            return this->transposed();
        }

        #ifndef DOXYGEN_GENERATING_OUTPUT
        /* Reimplementation of functions to return correct type */
        inline Matrix<size, T> operator*(const Matrix<size, T>& other) const {
            return RectangularMatrix<size, size, T>::operator*(other);
        }
        template<std::size_t otherCols> inline RectangularMatrix<otherCols, size, T> operator*(const RectangularMatrix<otherCols, size, T>& other) const {
            return RectangularMatrix<size, size, T>::operator*(other);
        }
        inline Vector<size, T> operator*(const Vector<size, T>& other) const {
            return RectangularMatrix<size, size, T>::operator*(other);
        }
        MAGNUM_RECTANGULARMATRIX_SUBCLASS_IMPLEMENTATION(size, size, Matrix<size, T>)
        #endif
};

#ifndef DOXYGEN_GENERATING_OUTPUT
template<std::size_t size, class T, class U> inline typename std::enable_if<std::is_arithmetic<U>::value, Matrix<size, T>>::type operator*(U number, const Matrix<size, T>& matrix) {
    return number*RectangularMatrix<size, size, T>(matrix);
}
template<std::size_t size, class T, class U> inline typename std::enable_if<std::is_arithmetic<U>::value, Matrix<size, T>>::type operator/(U number, const Matrix<size, T>& matrix) {
    return number/RectangularMatrix<size, size, T>(matrix);
}
template<std::size_t size, class T> inline Matrix<size, T> operator*(const Vector<size, T>& vector, const RectangularMatrix<size, 1, T>& matrix) {
    return RectangularMatrix<1, size, T>(vector)*matrix;
}
#endif

/** @debugoperator{Magnum::Math::Matrix} */
template<std::size_t size, class T> inline Corrade::Utility::Debug operator<<(Corrade::Utility::Debug debug, const Matrix<size, T>& value) {
    return debug << static_cast<const RectangularMatrix<size, size, T>&>(value);
}

#ifndef DOXYGEN_GENERATING_OUTPUT
#define MAGNUM_MATRIX_SUBCLASS_IMPLEMENTATION(Type, VectorType, size)       \
    inline VectorType<T>& operator[](std::size_t col) {                     \
        return static_cast<VectorType<T>&>(Matrix<size, T>::operator[](col)); \
    }                                                                       \
    inline constexpr const VectorType<T> operator[](std::size_t col) const { \
        return VectorType<T>(Matrix<size, T>::operator[](col));             \
    }                                                                       \
    inline VectorType<T> row(std::size_t row) const {                       \
        return VectorType<T>(Matrix<size, T>::row(row));                    \
    }                                                                       \
                                                                            \
    inline Type<T> operator*(const Matrix<size, T>& other) const {          \
        return Matrix<size, T>::operator*(other);                           \
    }                                                                       \
    template<std::size_t otherCols> inline RectangularMatrix<otherCols, size, T> operator*(const RectangularMatrix<otherCols, size, T>& other) const { \
        return Matrix<size, T>::operator*(other);                           \
    }                                                                       \
    inline VectorType<T> operator*(const Vector<size, T>& other) const {    \
        return Matrix<size, T>::operator*(other);                           \
    }                                                                       \
                                                                            \
    inline Type<T> transposed() const { return Matrix<size, T>::transposed(); } \
    inline Type<T> inverted() const { return Matrix<size, T>::inverted(); } \
    inline Type<T> invertedOrthogonal() const {                             \
        return Matrix<size, T>::invertedOrthogonal();                       \
    }

#define MAGNUM_MATRIX_SUBCLASS_OPERATOR_IMPLEMENTATION(Type, size)          \
    template<class T, class U> inline typename std::enable_if<std::is_arithmetic<U>::value, Type<T>>::type operator*(U number, const Type<T>& matrix) { \
        return number*Matrix<size, T>(matrix);                              \
    }                                                                       \
    template<class T, class U> inline typename std::enable_if<std::is_arithmetic<U>::value, Type<T>>::type operator/(U number, const Type<T>& matrix) { \
        return number/Matrix<size, T>(matrix);                              \
    }                                                                       \
    template<class T> inline Type<T> operator*(const Vector<size, T>& vector, const RectangularMatrix<size, 1, T>& matrix) { \
        return RectangularMatrix<1, size, T>(vector)*matrix;                \
    }

namespace Implementation {

template<std::size_t size, class T> class MatrixDeterminant {
    public:
        T operator()(const Matrix<size, T>& m) {
            T out(0);

            for(std::size_t col = 0; col != size; ++col)
                out += ((col & 1) ? -1 : 1)*m[col][0]*m.ij(col, 0).determinant();

            return out;
        }
};

template<class T> class MatrixDeterminant<2, T> {
    public:
        inline constexpr T operator()(const Matrix<2, T>& m) {
            return m[0][0]*m[1][1] - m[1][0]*m[0][1];
        }
};

template<class T> class MatrixDeterminant<1, T> {
    public:
        inline constexpr T operator()(const Matrix<1, T>& m) {
            return m[0][0];
        }
};

}
#endif

}}

namespace Corrade { namespace Utility {
    /** @configurationvalue{Magnum::Math::Matrix} */
    template<std::size_t size, class T> struct ConfigurationValue<Magnum::Math::Matrix<size, T>>: public ConfigurationValue<Magnum::Math::RectangularMatrix<size, size, T>> {};
}}

#endif
