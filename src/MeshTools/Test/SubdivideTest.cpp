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

#include "SubdivideTest.h"

#include <QtTest/QTest>

#include "MeshBuilder.h"
#include "MeshTools/Clean.h"
#include "MeshTools/Subdivide.h"

QTEST_APPLESS_MAIN(Magnum::MeshTools::Test::SubdivideTest)

using namespace std;

namespace Magnum { namespace MeshTools { namespace Test {

void SubdivideTest::subdivide() {
    MeshBuilder<Vector1> builder;
    builder.addVertex(0);
    builder.addVertex(2);
    builder.addVertex(6);
    builder.addVertex(8);
    builder.addFace(0, 1, 2);
    builder.addFace(1, 2, 3);

    MeshTools::subdivide(builder, interpolator);
    QVERIFY(builder.indices().size() == 24);

    QVERIFY((builder.vertices() == vector<Vector1>{0, 2, 6, 8, 1, 4, 3, 4, 7, 5}));
    QVERIFY((builder.indices() == vector<unsigned int>{4, 5, 6, 7, 8, 9, 0, 4, 6, 4, 1, 5, 6, 5, 2, 1, 7, 9, 7, 2, 8, 9, 8, 3}));

    MeshTools::clean(builder, 1);

    /* Vertices 0, 1, 2, 3, 4, 5, 6, 7, 8 */
    QVERIFY(builder.vertices().size() == 9);
}

}}}