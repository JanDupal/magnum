#ifndef Magnum_TextureTools_DistanceField_h
#define Magnum_TextureTools_DistanceField_h
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
 * @brief Function Magnum::TextureTools::distanceField()
 */

#include "Magnum.h"

#include "TextureTools/magnumTextureToolsVisibility.h"

namespace Magnum { namespace TextureTools {

/**
@brief Create signed distance field
@param input        Input texture
@param output       Output texture
@param rectangle    Rectangle in output texture where to render
@param radius       Max lookup radius in input texture

Converts binary image (stored in red channel of @p input) to signed distance
field (stored in red channel in @p rectangle of @p output). The purpose of this
function is to convert high-resolution binary image (such as vector artwork or
font glyphs) to low-resolution grayscale image. The image will then occupy much
less memory and can be scaled without aliasing issues. Additionally it provides
foundation for features like outlining, glow or drop shadow essentialy for free.

For each pixel inside @p rectangle the algorithm looks at corresponding pixel in
@p input and tries to find nearest pixel of opposite color in area given by
@p radius. Signed distance between the points is then saved as value of given
pixel in @p output. Value of `0` means that the pixel was originally colored
white and nearest black pixel is farther than @p radius, value of `1` means that
the pixel was originally black and nearest white pixel is farther than
@p radius. Values around `0.5` are around edges.

The resulting texture can be used with bilinear filtering. It can be converted
back to binary form in shader using e.g. GLSL `smoothstep()` function with step
around `0.5` to create antialiased edges. Or you can exploit the distance field
features to create many other effects. See also Shaders::DistanceFieldVectorShader.

Based on: *Chris Green - Improved Alpha-Tested Magnification for Vector Textures
and Special Effects, SIGGRAPH 2007,
http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf*

@attention This is GPU-only implementation, so it expects active context.
*/
void MAGNUM_TEXTURETOOLS_EXPORT distanceField(Texture2D* input, Texture2D* output, const Rectanglei& rectangle, Int radius);

}}

#endif
