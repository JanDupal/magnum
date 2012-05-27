#ifndef Magnum_Contexts_GlutContext_h
#define Magnum_Contexts_GlutContext_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Magnum::Contexts::GlutContext
 */

#include "Magnum.h"
#include <GL/freeglut.h>

#include "AbstractContext.h"

namespace Magnum { namespace Contexts {

/** @nosubgrouping
@brief GLUT context

Supports keyboard handling for limited subset of keys, mouse handling with
support for changing cursor and mouse tracking and warping.

You need to implement at least drawEvent() and viewportEvent() to be able to
draw on the screen.
*/
class GlutContext: public AbstractContext {
    public:
        /**
         * @brief Constructor
         * @param argc      Count of arguments of `main()` function
         * @param argv      Arguments of `main()` function
         * @param title     Window title
         * @param size      Window size
         *
         * Creates double-buffered RGBA window with depth and stencil buffers.
         */
        GlutContext(int& argc, char** argv, const std::string& title = "Magnum GLUT context", const Math::Vector2<GLsizei>& size = Math::Vector2<GLsizei>(800, 600));

        inline int exec() {
            glutMainLoop();
            return 0;
        }

        /** @{ @name Drawing functions */

    protected:
        /**
         * @brief Viewport event
         *
         * Called when viewport size changes. You should pass the new size to
         * Camera::viewport() function of your camera.
         */
        virtual void viewportEvent(const Math::Vector2<GLsizei>& size) = 0;

        /**
         * @brief Draw event
         *
         * Here implement your drawing functions, such as calling
         * Camera::draw(). After drawing is finished, call swapBuffers(). If
         * you want to draw immediately again, call also redraw().
         */
        virtual void drawEvent() = 0;

        /**
         * @brief Swap buffers
         *
         * Paints currently rendered framebuffer on screen.
         */
        inline void swapBuffers() {
            glutSwapBuffers();
        }

        /**
         * @brief Redraw immediately
         *
         * Marks the window for redrawing, resulting in call of drawEvent()
         * in the next iteration.
         */
        virtual inline void redraw() {
            glutPostRedisplay();
        }

        /*@}*/

        /** @{ @name Keyboard handling */

    public:
        /** @brief Key */
        enum class Key: int {
            Up = GLUT_KEY_UP,               /**< Up arrow */
            Down = GLUT_KEY_DOWN,           /**< Down arrow */
            Left = GLUT_KEY_LEFT,           /**< Left arrow */
            Right = GLUT_KEY_RIGHT,         /**< Right arrow */
            F1 = GLUT_KEY_F1,               /**< F1 */
            F2 = GLUT_KEY_F2,               /**< F2 */
            F3 = GLUT_KEY_F3,               /**< F3 */
            F4 = GLUT_KEY_F4,               /**< F4 */
            F5 = GLUT_KEY_F5,               /**< F5 */
            F6 = GLUT_KEY_F6,               /**< F6 */
            F7 = GLUT_KEY_F7,               /**< F7 */
            F8 = GLUT_KEY_F8,               /**< F8 */
            F9 = GLUT_KEY_F9,               /**< F9 */
            F10 = GLUT_KEY_F10,             /**< F10 */
            F11 = GLUT_KEY_F11,             /**< F11 */
            F12 = GLUT_KEY_F12,             /**< F12 */
            Home = GLUT_KEY_HOME,           /**< Home */
            End = GLUT_KEY_END,             /**< End */
            PageUp = GLUT_KEY_PAGE_UP,      /**< Page up */
            PageDown = GLUT_KEY_PAGE_DOWN   /**< Page down */
        };

    protected:
        /**
         * @brief Key event
         *
         * Called when an key is pressed. Default implementation does nothing.
         */
        virtual inline void keyEvent(Key key, const Math::Vector2<int>& position) {}

        /*@}*/

        /** @{ @name Mouse handling */

    public:
        /** @brief Mouse button */
        enum class MouseButton: int {
            Left = GLUT_LEFT_BUTTON,        /**< Left button */
            Middle = GLUT_MIDDLE_BUTTON,    /**< Middle button */
            Right = GLUT_RIGHT_BUTTON,      /**< Right button */
            WheelUp = 3,                    /**< Wheel up */
            WheelDown = 4                   /**< Wheel down */
        };

        /** @brief Mouse state */
        enum class MouseState: int {
            Up = GLUT_UP,                   /**< No button pressed */
            Down = GLUT_DOWN                /**< Button pressed */
        };

        /** @brief Mouse cursor */
        enum class MouseCursor: int {
            Default = GLUT_CURSOR_INHERIT,  /**< Default cursor provided by parent window */
            None = GLUT_CURSOR_NONE         /**< No cursor */
        };

        /**
         * @brief Enable or disable mouse tracking
         *
         * When mouse tracking is enabled, mouseMoveEvent() is called even
         * when no button is pressed. Mouse tracking is disabled by default.
         */
        inline void setMouseTracking(bool enabled) {
            glutPassiveMotionFunc(enabled ? staticMouseMoveEvent : nullptr);
        }

        /** @brief Set mouse cursor */
        inline void setMouseCursor(MouseCursor cursor) {
            glutSetCursor(static_cast<int>(cursor));
        }

        /** @brief Warp mouse cursor to given coordinates */
        inline void warpMouseCursor(const Math::Vector2<GLsizei>& position) {
            glutWarpPointer(position.x(), position.y());
        }

    protected:
        /**
         * @brief Mouse event
         *
         * Called when mouse button is pressed or released. Default
         * implementation does nothing.
         */
        virtual inline void mouseEvent(MouseButton button, MouseState state, const Math::Vector2<int>& position) {}

        /**
         * @brief Mouse move event
         *
         * Called when any mouse button is pressed and mouse is moved. Default
         * implementation does nothing.
         *
         * @see setMouseTracking()
         */
        virtual inline void mouseMoveEvent(const Math::Vector2<int>& position) {}

        /*@}*/

    private:
        inline static void staticViewportEvent(int x, int y) {
            instance->viewportEvent({x, y});
        }

        inline static void staticKeyEvent(int key, int x, int y) {
            instance->keyEvent(static_cast<Key>(key), {x, y});
        }

        inline static void staticMouseEvent(int button, int state, int x, int y) {
            instance->mouseEvent(static_cast<MouseButton>(button), static_cast<MouseState>(state), {x, y});
        }

        inline static void staticMouseMoveEvent(int x, int y) {
            instance->mouseMoveEvent({x, y});
        }

        inline static void staticDrawEvent() {
            instance->drawEvent();
        }

        static GlutContext* instance;

        int& argc;
        char** argv;
};

}}

#endif