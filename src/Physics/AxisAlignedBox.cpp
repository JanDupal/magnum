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

#include "AxisAlignedBox.h"

#include "Math/Matrix3.h"
#include "Math/Matrix4.h"
#include "Physics/Point.h"

namespace Magnum { namespace Physics {

template<UnsignedInt dimensions> void AxisAlignedBox<dimensions>::applyTransformationMatrix(const typename DimensionTraits<dimensions>::MatrixType& matrix) {
    _transformedMin = matrix.transformPoint(_min);
    _transformedMax = matrix.transformPoint(_max);
}

template<UnsignedInt dimensions> bool AxisAlignedBox<dimensions>::collides(const AbstractShape<dimensions>* other) const {
    if(other->type() == AbstractShape<dimensions>::Type::Point)
        return *this % *static_cast<const Point<dimensions>*>(other);

    return AbstractShape<dimensions>::collides(other);
}

template<UnsignedInt dimensions> bool AxisAlignedBox<dimensions>::operator%(const Point<dimensions>& other) const {
    return (other.transformedPosition() >= _transformedMin).all() &&
           (other.transformedPosition() < _transformedMax).all();
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template class MAGNUM_PHYSICS_EXPORT AxisAlignedBox<2>;
template class MAGNUM_PHYSICS_EXPORT AxisAlignedBox<3>;
#endif

}}
