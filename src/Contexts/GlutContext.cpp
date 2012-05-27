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

#include "GlutContext.h"

using namespace Corrade::Utility;

namespace Magnum { namespace Contexts {

GlutContext* GlutContext::instance = nullptr;

GlutContext::GlutContext(int& argc, char** argv, const std::string& title, const Math::Vector2<GLsizei>& size): argc(argc), argv(argv) {
    /* Save global instance */
    instance = this;

    /* Init GLUT */
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(size.x(), size.y());
    glutCreateWindow(title.c_str());
    glutReshapeFunc(staticViewportEvent);
    glutSpecialFunc(staticKeyEvent);
    glutMouseFunc(staticMouseEvent);
    glutMotionFunc(staticMouseMoveEvent);
    glutDisplayFunc(staticDrawEvent);

    /* Init GLEW */
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        Error() << "GlutContext: cannot initialize GLEW:" << glewGetErrorString(err);
        exit(1);
    }
}

}}