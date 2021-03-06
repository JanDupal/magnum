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

#include <limits>
#include <TestSuite/Tester.h>

#include "Math/Constants.h"
#include "Math/Geometry/Distance.h"

namespace Magnum { namespace Math { namespace Geometry { namespace Test {

class DistanceTest: public Corrade::TestSuite::Tester {
    public:
        DistanceTest();

        void linePoint2D();
        void linePoint3D();
        void lineSegmentPoint2D();
        void lineSegmentPoint3D();
};

typedef Math::Vector2<Float> Vector2;
typedef Math::Vector3<Float> Vector3;
typedef Math::Constants<Float> Constants;

DistanceTest::DistanceTest() {
    addTests({&DistanceTest::linePoint2D,
              &DistanceTest::linePoint3D,
              &DistanceTest::lineSegmentPoint2D,
              &DistanceTest::lineSegmentPoint3D});
}

void DistanceTest::linePoint2D() {
    Vector2 a(0.0f);
    Vector2 b(1.0f);

    /* Point on the line */
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector2(0.25f))), 0.0f);

    /* The distance should be the same for all equidistant points */
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector2(1.0f, 0.0f))), 1.0f/Constants::sqrt2());
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector2(1.0f, 0.0f)+Vector2(100.0f))), 1.0f/Constants::sqrt2());

    /* Be sure that *Squared() works the same, as it has slightly different implementation */
    CORRADE_COMPARE((Distance::linePointSquared(a, b, Vector2(1.0f, 0.0f))), 0.5f);
}

void DistanceTest::linePoint3D() {
    Vector3 a(0.0f);
    Vector3 b(1.0f);

    /* Point on the line */
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector3(0.25f))), 0.0f);

    /* The distance should be the same for all equidistant points */
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector3(1.0f, 0.0f, 1.0f))), Constants::sqrt2()/Constants::sqrt3());
    CORRADE_COMPARE((Distance::linePoint(a, b, Vector3(1.0f, 0.0f, 1.0f)+Vector3(100.0f))), Constants::sqrt2()/Constants::sqrt3());
}

void DistanceTest::lineSegmentPoint2D() {
    Vector2 a(0.0f);
    Vector2 b(1.0f);

    /* Point on the line segment */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(0.25f))), 0.0f);

    /* Point on the line, outside the segment, closer to A */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(-1.0f))), Constants::sqrt2());
    /* Be sure that *Squared() works the same, as it has slightly different implementation */
    CORRADE_COMPARE((Distance::lineSegmentPointSquared(a, b, Vector2(-1.0f))), 2.0f);

    /* Point on the line, outside the segment, closer to B */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(1.0f+1.0f/Constants::sqrt2()))), 1.0f);
    CORRADE_COMPARE((Distance::lineSegmentPointSquared(a, b, Vector2(1.0f+1.0f/Constants::sqrt2()))), 1.0f);

    /* Point next to the line segment */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(1.0f, 0.0f))), 1.0f/Constants::sqrt2());
    CORRADE_COMPARE((Distance::lineSegmentPointSquared(a, b, Vector2(1.0f, 0.0f))), 0.5f);

    /* Point outside the line segment, closer to A */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(1.0f, 0.0f)-Vector2(1.0f, 0.5f))), 0.5f);
    CORRADE_COMPARE((Distance::lineSegmentPointSquared(a, b, Vector2(1.0f, 0.0f)-Vector2(1.0f, 0.5f))), 0.25f);

    /* Point outside the line segment, closer to B */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector2(1.0f, 0.0f)+Vector2(0.5f, 1.0f))), 0.5f);
    CORRADE_COMPARE((Distance::lineSegmentPointSquared(a, b, Vector2(1.0f, 0.0f)+Vector2(0.5f, 1.0f))), 0.25f);
}

void DistanceTest::lineSegmentPoint3D() {
    Vector3 a(0.0f);
    Vector3 b(1.0f);

    /* Point on the line segment */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(0.25f))), 0.0f);

    /* Point on the line, outside the segment, closer to A */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(-1.0f))), Constants::sqrt3());

    /* Point on the line, outside the segment, closer to B */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(1.0f+1.0f/Constants::sqrt3()))), 1.0f);

    /* Point next to the line segment */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(1.0f, 0.0f, 1.0f))), Constants::sqrt2()/Constants::sqrt3());

    /* Point outside the line segment, closer to A */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(1.0f, 0.0f, 1.0f)-Vector3(1.0f))), 1.0f);

    /* Point outside the line segment, closer to B */
    CORRADE_COMPARE((Distance::lineSegmentPoint(a, b, Vector3(1.0f, 0.0f, 1.0f)+Vector3(1.0f))), Constants::sqrt2());
}

}}}}

CORRADE_TEST_MAIN(Magnum::Math::Geometry::Test::DistanceTest)
