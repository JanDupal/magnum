#ifndef Magnum_Platform_GlxContextHandler_h
#define Magnum_Platform_GlxContextHandler_h
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
 * @brief Class Magnum::Platform::GlxContextHandler
 */

#include "OpenGL.h"
#include <GL/glx.h>
/* undef Xlib nonsense to avoid conflicts */
#undef Complex
#undef None
#undef Always

#include "AbstractContextHandler.h"

#include "corradeCompatibility.h"

namespace Magnum { namespace Platform {

/**
@brief GLX context

Creates OpenGL or OpenGL ES 2.0 context, if targeting OpenGL ES. Used in
GlxApplication.
*/
class GlxContextHandler: public AbstractContextHandler<Display*, VisualID, Window> {
    public:
        explicit GlxContextHandler() = default;
        ~GlxContextHandler();

        VisualID getVisualId(Display* nativeDisplay) override;
        void createContext(Window nativeWindow) override;

        /* This must be enabled, otherwise (on my NVidia) it crashes when creating VAO. WTF. */
        inline ExtensionWrangler::ExperimentalFeatures experimentalExtensionWranglerFeatures() const override {
            return ExtensionWrangler::ExperimentalFeatures::Enable;
        }

        inline void makeCurrent() override {
            glXMakeCurrent(display, window, context);
        }

        inline void swapBuffers() override {
            glXSwapBuffers(display, window);
        }

    private:
        Display* display;
        Window window;
        GLXFBConfig* configs;
        GLXContext context;
};

}}

#endif
