#ifndef Magnum_Physics_visibilityPhysics_h
#define Magnum_Physics_visibilityPhysics_h
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

#ifdef _WIN32
    #ifdef MagnumPhysics_EXPORTS
        #define PHYSICS_EXPORT __declspec(dllexport)
    #else
        #define PHYSICS_EXPORT __declspec(dllimport)
    #endif
#else
    #define PHYSICS_EXPORT __attribute__ ((visibility ("default")))
#endif

#endif