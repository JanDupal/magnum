#ifndef Magnum_Physics_ShapeGroup_h
#define Magnum_Physics_ShapeGroup_h
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
 * @brief Class Magnum::Physics::ShapeGroup, typedef Magnum::Physics::ShapeGroup2D, Magnum::Physics::ShapeGroup3D
 */

#include "AbstractShape.h"

#include <functional>
#include <type_traits>
#include <utility>

#include "corradeCompatibility.h"

namespace Magnum { namespace Physics {

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Implementation {
    enum GroupOperation {
        RefA = 0x01,
        RefB = 0x02,
        RefAB = 0x03,
//         Complement = 1 << 2,
//         Union = 2 << 2,
//         Intersection = 3 << 2,
//         Difference = 4 << 2,
//         Xor = 5 << 2,
        And = 6 << 2,
        Or = 7 << 2,
        Not = 8 << 2,
        FirstObjectOnly = 9 << 2,
        AlwaysFalse = 10 << 2
    };
}
#define enableIfIsBaseType typename std::enable_if<std::is_base_of<AbstractShape<T::Dimensions>, T>::value, ShapeGroup<T::Dimensions>>::type
#define enableIfAreBaseType typename std::enable_if<T::Dimensions == U::Dimensions && std::is_base_of<AbstractShape<T::Dimensions>, T>::value && std::is_base_of<AbstractShape<T::Dimensions>, U>::value, ShapeGroup<T::Dimensions>>::type
#endif

/**
@brief Shape group

Result of logical operations on shapes.
See @ref collision-detection for brief introduction.
@see ShapeGroup2D, ShapeGroup3D
*/
template<UnsignedInt dimensions> class MAGNUM_PHYSICS_EXPORT ShapeGroup: public AbstractShape<dimensions> {
    #ifndef DOXYGEN_GENERATING_OUTPUT
//     template<class T> friend constexpr operator~(const T& a) -> enableIfIsBaseType;
//     template<class T> friend constexpr operator~(T&& a) -> enableIfIsBaseType;
//     template<class T> friend constexpr operator~(T& a) -> enableIfIsBaseType;
    template<class T> friend constexpr auto operator!(const T& a) -> enableIfIsBaseType;
    template<class T> friend constexpr auto operator!(T&& a) -> enableIfIsBaseType;
    template<class T> friend constexpr auto operator!(T& a) -> enableIfIsBaseType;

    #define friendOp(char) \
        template<class T, class U> friend constexpr auto operator char(const T& a, const U& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(const T& a, U&& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(T&& a, const U& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(T&& a, U&& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(const T& a, std::reference_wrapper<U> b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(T&& a, std::reference_wrapper<U> b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(std::reference_wrapper<T> a, const U& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(std::reference_wrapper<T> a, U&& b) -> enableIfAreBaseType; \
        template<class T, class U> friend constexpr auto operator char(std::reference_wrapper<T> a, std::reference_wrapper<U> b) -> enableIfAreBaseType;
//     friendOp(|)
//     friendOp(&)
//     friendOp(-)
//     friendOp(^)
    friendOp(&&)
    friendOp(||)
    #undef friendOp
    #endif

    ShapeGroup(const ShapeGroup&) = delete;
    ShapeGroup& operator=(const ShapeGroup&) = delete;

    public:
        /** @brief Default constructor */
        inline explicit ShapeGroup(): operation(Implementation::GroupOperation::AlwaysFalse), a(nullptr), b(nullptr) {}

        /** @brief Move constructor */
        ShapeGroup(ShapeGroup&& other);

        /** @brief Destructor */
        ~ShapeGroup();

        /** @brief Move assignment */
        ShapeGroup& operator=(ShapeGroup&& other);

        inline typename AbstractShape<dimensions>::Type type() const override {
            return AbstractShape<dimensions>::Type::ShapeGroup;
        }

        void applyTransformationMatrix(const typename DimensionTraits<dimensions>::MatrixType& matrix) override;

        bool collides(const AbstractShape<dimensions>* other) const override;

        /**
         * @brief First object in the group
         *
         * If there is no such object, returns `nullptr`.
         */
        inline AbstractShape<dimensions>* first() { return a; }

        /**
         * @brief Second object in the group
         *
         * If there is no such object, returns `nullptr`.
         */
        inline AbstractShape<dimensions>* second() { return b; }

    private:
        inline ShapeGroup(int operation, AbstractShape<dimensions>* a, AbstractShape<dimensions>* b): operation(operation), a(a), b(b) {}

        int operation;
        AbstractShape<dimensions>* a;
        AbstractShape<dimensions>* b;
};

/** @brief Two-dimensional shape group */
typedef ShapeGroup<2> ShapeGroup2D;

/** @brief Three-dimensional shape group */
typedef ShapeGroup<3> ShapeGroup3D;

// /* @brief Complement of shape */
// template<class T> inline constexpr enableIfIsBaseType operator~(const T& a) {
//     return ShapeGroup(ShapeGroup::Complement, new T(a), nullptr);
// }
// #ifndef DOXYGEN_GENERATING_OUTPUT
// template<class T> inline constexpr enableIfIsBaseType operator~(T&& a) {
//     return ShapeGroup(ShapeGroup::Complement, new T(std::forward<T>(a)), nullptr);
// }
// template<class T> inline constexpr enableIfIsBaseType operator~(T& a) {
//     return ShapeGroup(ShapeGroup::Complement|ShapeGroup::RefA, &a.get(), nullptr);
// }
// #endif

/** @relates ShapeGroup
@brief Logical NOT of shape
*/
template<class T> inline constexpr auto operator!(const T& a) -> enableIfIsBaseType {
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::Not, new T(a), nullptr);
}
#ifndef DOXYGEN_GENERATING_OUTPUT
template<class T> inline constexpr auto operator!(T&& a) -> enableIfIsBaseType {
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::Not, new T(std::forward<T>(a)), nullptr);
}
template<class T> inline constexpr auto operator!(T& a) -> enableIfIsBaseType {
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::Not|Implementation::GroupOperation::RefA, &a.get(), nullptr);
}
#endif

#ifdef DOXYGEN_GENERATING_OUTPUT
// /* @brief Union of two shapes */
// template<class T, class U> inline constexpr ShapeGroup operator&(T a, U b);
//
// /* @brief Intersection of two shapes */
// template<class T, class U> inline constexpr ShapeGroup operator&(T a, U b);
//
// /* @brief Difference of two shapes */
// template<class T, class U> inline constexpr ShapeGroup operator-(T a, U b);
//
// /* @brief XOR of two shapes */
// template<class T, class U> inline constexpr ShapeGroup operator^(T a, U b);
/** @relates ShapeGroup
@brief Logical AND of two shapes

[Short-circuit evaluation](http://en.wikipedia.org/wiki/Short-circuit_evaluation)
is used here, so this operation can be used for providing simplified shape
version, because collision with @p b is computed only if @p a collides.
See @ref collision-detection-shape-simplification for an example.
*/
template<UnsignedInt dimensions, class T, class U> inline constexpr ShapeGroup<dimensions> operator&&(T a, U b);

/** @relates ShapeGroup
@brief Logical OR of two shapes

[Short-circuit evaluation](http://en.wikipedia.org/wiki/Short-circuit_evaluation)
is used, so if collision with @p a is detected, collision with @p b is not
computed.
*/
template<UnsignedInt dimensions, class T, class U> inline constexpr ShapeGroup<dimensions> operator||(T a, U b);
#else
#define op(type, char)                                                      \
template<class T, class U> inline constexpr auto operator char(const T& a, const U& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type, new T(a), new U(b));                \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(const T& a, U&& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type, new T(a), new U(std::forward<U>(b))); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(T&& a, const U& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type, new T(std::forward<T>(a)), new U(b)); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(T&& a, U&& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type, new T(std::forward<T>(a)), new U(std::forward<U>(b))); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(const T& a, std::reference_wrapper<U> b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type|Implementation::GroupOperation::RefB, new T(a), &b.get()); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(T&& a, std::reference_wrapper<U> b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type|Implementation::GroupOperation::RefB, new T(std::forward<T>(a)), &b.get()); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(std::reference_wrapper<T> a, const U& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type|Implementation::GroupOperation::RefA, &a.get(), new U(b)); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(std::reference_wrapper<T> a, U&& b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type|Implementation::GroupOperation::RefA, &a.get(), new U(std::forward<U>(b))); \
}                                                                           \
template<class T, class U> inline constexpr auto operator char(std::reference_wrapper<T> a, std::reference_wrapper<U> b) -> enableIfAreBaseType { \
    return ShapeGroup<T::Dimensions>(Implementation::GroupOperation::type|Implementation::GroupOperation::RefAB, &a.get(), &b.get()); \
}
// op(Union, |)
// op(Intersection, &)
// op(Difference, -)
// op(Xor, ^)
op(And, &&)
op(Or, ||)
#undef op
#endif

#undef enableIfIsBaseType
#undef enableIfAreBaseType

}}

#endif
