#ifndef Magnum_SceneGraph_AbstractFeature_h
#define Magnum_SceneGraph_AbstractFeature_h
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
 * @brief Class Magnum::SceneGraph::AbstractFeature, alias Magnum::SceneGraph::AbstractFeature2D, Magnum::SceneGraph::AbstractFeature3D
 */

#include <Containers/EnumSet.h>
#include <Containers/LinkedList.h>

#include "Magnum.h"
#include "SceneGraph/AbstractObject.h"

namespace Magnum { namespace SceneGraph {

namespace Implementation {
    enum class FeatureCachedTransformation: UnsignedByte {
        Absolute = 1 << 0,
        InvertedAbsolute = 1 << 1
    };

    typedef Corrade::Containers::EnumSet<FeatureCachedTransformation, UnsignedByte> FeatureCachedTransformations;

    CORRADE_ENUMSET_OPERATORS(FeatureCachedTransformations)
}

/**
@brief Base for object features

Contained in Object, takes care of transformation caching. See @ref scenegraph
for introduction.

Uses Corrade::Containers::LinkedList for accessing holder object and sibling
features.

@section AbstractFeature-subclassing Subclassing

Feature is templated on dimension count and underlying transformation type, so
it can be used only on object having transformation with the same dimension
count and type.

@subsection AbstractFeature-subclassing-caching Caching transformations in features

Features can cache absolute transformation of the object instead of computing
it from scratch every time to achieve better performance. See
@ref scenegraph-caching for introduction.

In order to have caching, you must enable it first, because by default the
caching is disabled. You can enable it using setCachedTransformations() and
then implement corresponding cleaning function(s) -- either clean(),
cleanInverted() or both. Example:
@code
class CachingFeature: public SceneGraph::AbstractFeature3D<> {
    public:
        CachingFeature(SceneGraph::AbstractObject3D<>* object): SceneGraph::AbstractFeature3D<>(object) {
            setCachedTransformations(CachedTransformation::Absolute);
        }

    protected:
        void clean(const Matrix4& absoluteTransformationMatrix) override {
            absolutePosition = absoluteTransformationMatrix.translation();
        }

    private:
        Vector3 absolutePosition;
};
@endcode

Before using the cached value explicitly request object cleaning by calling
`object()->setClean()`.

@subsection AbstractFeature-subclassing-transformation Accessing object transformation

Features has by default access only to AbstractObject, which is base of Object
not depending on any particular transformation implementation. This has the
advantage that features doesn't have to be implemented for all possible
transformation implementations, thus preventing code duplication. However it
is impossible to transform the object using only pointer to AbstractObject.

The transformations have interfaces for common functionality, so the feature
can use that interface instead of being specialized for all relevant
transformation implementations. Using small trick we are able to get pointer
to both AbstractObject and needed transformation from one constructor
parameter:
@code
class TransformingFeature: public SceneGraph::AbstractFeature3D<> {
    public:
        template<class T> inline TransformingFeature(SceneGraph::Object<T>* object):
            SceneGraph::AbstractFeature3D<>(object), transformation(object) {}

    private:
        SceneGraph::AbstractTranslationRotation3D<>* transformation;
};
@endcode
If we take for example @ref Object "Object<MatrixTransformation3D<>>", it is
derived from @ref AbstractObject "AbstractObject3D<>" and
@ref MatrixTransformation3D "MatrixTransformation3D<>", which is derived from
@ref AbstractTranslationRotationScaling3D "AbstractTranslationRotationScaling3D<>",
which is derived from
@ref AbstractTranslationRotation3D "AbstractTranslationRotation3D<>",
which is automatically extracted from the pointer in our constructor.

@see AbstractFeature2D, AbstractFeature3D
*/
#ifndef DOXYGEN_GENERATING_OUTPUT
template<UnsignedInt dimensions, class T> class AbstractFeature: private Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>
#else
template<UnsignedInt dimensions, class T = Float> class AbstractFeature
#endif
{
    friend class Corrade::Containers::LinkedList<AbstractFeature<dimensions, T>>;
    friend class Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>;
    template<class Transformation> friend class Object;

    public:
        /**
         * @brief Constructor
         * @param object    %Object holding this feature
         */
        inline explicit AbstractFeature(AbstractObject<dimensions, T>* object) {
            object->Corrade::Containers::template LinkedList<AbstractFeature<dimensions, T>>::insert(this);
        }

        virtual ~AbstractFeature() = 0;

        /** @brief %Object holding this feature */
        inline AbstractObject<dimensions, T>* object() {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::list();
        }

        /** @overload */
        inline const AbstractObject<dimensions, T>* object() const {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::list();
        }

        /** @brief Previous feature or `nullptr`, if this is first feature */
        inline AbstractFeature<dimensions, T>* previousFeature() {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::previous();
        }

        /** @overload */
        inline const AbstractFeature<dimensions, T>* previousFeature() const {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::previous();
        }

        /** @brief Next feature or `nullptr`, if this is last feature */
        inline AbstractFeature<dimensions, T>* nextFeature() {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::next();
        }

        /** @overload */
        inline const AbstractFeature<dimensions, T>* nextFeature() const {
            return Corrade::Containers::LinkedListItem<AbstractFeature<dimensions, T>, AbstractObject<dimensions, T>>::next();
        }

        /**
         * @{ @name Transformation caching
         *
         * See @ref scenegraph-caching for more information.
         */

        /**
         * @brief Which transformation to cache in this feature
         *
         * @see @ref scenegraph-caching, CachedTransformations,
         *      setCachedTransformations(), clean(), cleanInverted()
         * @todo Provide also simpler representations from which could benefit
         *      other transformation implementations, as they won't need to
         *      e.g. create transformation matrix from quaternion?
         * @todo Move outside templated class so it's easier to type
         */
        #ifndef DOXYGEN_GENERATING_OUTPUT
        typedef Implementation::FeatureCachedTransformation CachedTransformation;
        #else
        enum class CachedTransformation: UnsignedByte {
            /**
             * Absolute transformation is cached.
             *
             * If enabled, clean() is called when cleaning object.
             */
            Absolute = 1 << 0,

            /**
             * Inverted absolute transformation is cached.
             *
             * If enabled, cleanInverted() is called when cleaning object.
             */
            InvertedAbsolute = 1 << 1
        };
        #endif

        /**
         * @brief Which transformations to cache in this feature
         *
         * @see @ref scenegraph-caching, setCachedTransformations(), clean(),
         *      cleanInverted()
         */
        #ifndef DOXYGEN_GENERATING_OUTPUT
        typedef Implementation::FeatureCachedTransformations CachedTransformations;
        #else
        typedef Corrade::Containers::EnumSet<CachedTransformation, UnsignedByte> CachedTransformations;
        #endif

        /**
         * @brief Which transformations are cached
         *
         * @see @ref scenegraph-caching, clean(), cleanInverted()
         */
        inline CachedTransformations cachedTransformations() const { return _cachedTransformations; }

    protected:
        /**
         * @brief Set transformations to be cached
         *
         * Based on which transformation types are enabled, clean() or
         * cleanInverted() is called when cleaning absolute object
         * transformation.
         *
         * Nothing is enabled by default.
         * @see @ref scenegraph-caching
         */
        inline void setCachedTransformations(CachedTransformations transformations) { _cachedTransformations = transformations; }

        /**
         * @brief Mark feature as dirty
         *
         * Reimplement only if you want to invalidate some external data when
         * object is marked as dirty. All expensive computations should be
         * done in clean() and cleanInverted().
         *
         * Default implementation does nothing.
         * @see @ref scenegraph-caching
         */
        inline virtual void markDirty() {}

        /**
         * @brief Clean data based on absolute transformation
         *
         * When object is cleaned and
         * @ref CachedTransformation "CachedTransformation::Absolute" is
         * enabled in setCachedTransformations(), this function is called to
         * recalculate data based on absolute object transformation.
         *
         * Default implementation does nothing.
         * @see @ref scenegraph-caching, cleanInverted()
         */
        virtual void clean(const typename DimensionTraits<dimensions, T>::MatrixType& absoluteTransformationMatrix);

        /**
         * @brief Clean data based on inverted absolute transformation
         *
         * When object is cleaned and
         * @ref CachedTransformation "CachedTransformation::InvertedAbsolute"
         * is enabled in setCachedTransformations(), this function is called
         * to recalculate data based on inverted absolute object
         * transformation.
         *
         * Default implementation does nothing.
         * @see @ref scenegraph-caching, clean()
         */
        virtual void cleanInverted(const typename DimensionTraits<dimensions, T>::MatrixType& invertedAbsoluteTransformationMatrix);

        /*@}*/

    private:
        CachedTransformations _cachedTransformations;
};

template<UnsignedInt dimensions, class T> inline AbstractFeature<dimensions, T>::~AbstractFeature() {}
template<UnsignedInt dimensions, class T> inline void AbstractFeature<dimensions, T>::clean(const typename DimensionTraits<dimensions, T>::MatrixType&) {}
template<UnsignedInt dimensions, class T> inline void AbstractFeature<dimensions, T>::cleanInverted(const typename DimensionTraits<dimensions, T>::MatrixType&) {}

#ifndef CORRADE_GCC46_COMPATIBILITY
/**
@brief Base for two-dimensional features

Convenience alternative to <tt>%AbstractFeature<2, T></tt>. See AbstractFeature
for more information.
@note Not available on GCC < 4.7. Use <tt>%AbstractFeature<2, T></tt> instead.
@see AbstractFeature3D
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T = Float>
#else
template<class T>
#endif
using AbstractFeature2D = AbstractFeature<2, T>;

/**
@brief Base for three-dimensional features

Convenience alternative to <tt>%AbstractFeature<3, T></tt>. See AbstractFeature
for more information.
@note Not available on GCC < 4.7. Use <tt>%AbstractFeature<3, T></tt> instead.
@see AbstractFeature2D
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T = Float>
#else
template<class T>
#endif
using AbstractFeature3D = AbstractFeature<3, T>;
#endif

}}

#endif
