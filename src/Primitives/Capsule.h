#ifndef Magnum_Primitives_Capsule_h
#define Magnum_Primitives_Capsule_h
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
 * @brief Class Magnum::Primitives::Capsule
 */

#include "Primitives/magnumPrimitivesVisibility.h"
#include "Trade/Trade.h"

namespace Magnum { namespace Primitives {

/**
@brief 3D capsule primitive

%Cylinder of radius `1` along Y axis with hemispheres instead of caps.
*/
class MAGNUM_PRIMITIVES_EXPORT Capsule {
    public:
        /** @brief Whether to generate texture coordinates */
        enum class TextureCoords: UnsignedByte {
            Generate,       /**< Generate texture coordinates */
            DontGenerate    /**< Don't generate texture coordinates */
        };

        /**
         * @brief Solid capsule
         * @param hemisphereRings Number of (face) rings for each hemisphere.
         *      Must be larger or equal to 1.
         * @param cylinderRings Number of (face) rings for cylinder. Must be
         *      larger or equal to 1.
         * @param segments      Number of (face) segments. Must be larger or
         *      equal to 3.
         * @param length        Length of the capsule, excluding hemispheres.
         * @param textureCoords Whether to generate texture coordinates.
         *
         * Indexed @ref Mesh::Primitive "Triangles" with normals and optional
         * 2D texture coordinates. If texture coordinates are generated,
         * vertices of one segment are duplicated for texture wrapping.
         */
        static Trade::MeshData3D solid(UnsignedInt hemisphereRings, UnsignedInt cylinderRings, UnsignedInt segments, Float length, TextureCoords textureCoords = TextureCoords::DontGenerate);
};

}}

#endif
