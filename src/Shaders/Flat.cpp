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

#include "Flat.h"

#include <Utility/Resource.h>

#include "Extensions.h"
#include "Shader.h"

namespace Magnum { namespace Shaders {

namespace {
    template<UnsignedInt> constexpr const char* vertexShaderName();
    template<> constexpr const char* vertexShaderName<2>() { return "Flat2D.vert"; }
    template<> constexpr const char* vertexShaderName<3>() { return "Flat3D.vert"; }
}

template<UnsignedInt dimensions> Flat<dimensions>::Flat(): transformationProjectionMatrixUniform(0), colorUniform(1) {
    Corrade::Utility::Resource rs("MagnumShaders");

    #ifndef MAGNUM_TARGET_GLES
    Version v = Context::current()->supportedVersion({Version::GL320, Version::GL210});
    #else
    Version v = Context::current()->supportedVersion({Version::GLES300, Version::GLES200});
    #endif

    attachShader(Shader(v, Shader::Type::Vertex)
        .addSource(rs.get("compatibility.glsl"))
        .addSource(rs.get(vertexShaderName<dimensions>())));

    attachShader(Shader(v, Shader::Type::Fragment)
        .addSource(rs.get("compatibility.glsl"))
        .addSource(rs.get("Flat.frag")));

    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::explicit_attrib_location>() ||
        Context::current()->version() == Version::GL210)
    #else
    if(!Context::current()->isVersionSupported(Version::GLES300))
    #endif
    {
        bindAttributeLocation(Position::Location, "position");
    }

    link();

    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::explicit_uniform_location>())
    #endif
    {
        transformationProjectionMatrixUniform = uniformLocation("transformationProjectionMatrix");
        colorUniform = uniformLocation("color");
    }
}

template class Flat<2>;
template class Flat<3>;

}}
