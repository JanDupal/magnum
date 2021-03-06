#ifndef Magnum_Math_Math_h
#define Magnum_Math_Math_h
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
 * @brief Forward declarations for Magnum::Math namespace
 */

#include <cstddef>

namespace Magnum { namespace Math {

/** @todo Denormals to zero */

/** @todoc Remove `ifndef` when Doxygen is sane again */
#ifndef DOXYGEN_GENERATING_OUTPUT
/* Class Constants used only statically */

template<class> class Complex;
template<class> class Dual;
template<class> class DualComplex;
template<class> class DualQuaternion;

template<std::size_t, class> class Matrix;
template<class> class Matrix3;
template<class> class Matrix4;

template<class> class Quaternion;

template<std::size_t, std::size_t, class> class RectangularMatrix;

template<template<class> class, class> class Unit;
template<class> class Deg;
template<class> class Rad;

template<std::size_t, class> class Vector;
template<class> class Vector2;
template<class> class Vector3;
template<class> class Vector4;

namespace Geometry {
    template<class> class Rectangle;
}
#endif

}}

#endif
