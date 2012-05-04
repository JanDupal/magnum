#ifndef Magnum_Physics_Plane_h
#define Magnum_Physics_Plane_h
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
 * @brief Class Magnum::Physics::Plane
 */

#include "AbstractShape.h"

#include "Line.h"
#include "LineSegment.h"

namespace Magnum { namespace Physics {

/** @brief Infinite plane, defined by position and normal */
class PHYSICS_EXPORT Plane: public AbstractShape {
    public:
        /** @brief Constructor */
        inline constexpr Plane(const Vector3& position, const Vector3& normal): _position(position), _transformedPosition(position), _normal(normal), _transformedNormal(normal) {}

        void applyTransformation(const Matrix4& transformation);

        bool collides(const AbstractShape* other) const;

        /** @brief Position */
        inline Vector3 position() const { return _position; }

        /** @brief Set position */
        inline void setPosition(const Vector3& position) {
            _position = position;
        }

        /** @brief Normal */
        inline Vector3 normal() const { return _normal; }

        /** @brief Set normal */
        inline void setNormal(const Vector3& normal) {
            _normal = normal;
        }

        /** @brief Transformed position */
        inline Vector3 transformedPosition() const {
            return _transformedPosition;
        }

        /** @brief Transformed normal */
        inline Vector3 transformedNormal() const {
            return _transformedNormal;
        }

        /** @brief Collision with line */
        bool operator%(const Line& other) const;

        /** @brief Collision with line segment */
        bool operator%(const LineSegment& other) const;

    protected:
        inline Type type() const { return Type::Plane; }

    private:
        Vector3 _position, _transformedPosition,
            _normal, _transformedNormal;
};

#ifndef DOXYGEN_GENERATING_OUTPUT
inline bool operator%(const Line& a, const Plane& b) { return b % a; }
inline bool operator%(const LineSegment& a, const Plane& b) { return b % a; }
#endif

}}

#endif