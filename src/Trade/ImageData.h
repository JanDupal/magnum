#ifndef Magnum_Trade_ImageData_h
#define Magnum_Trade_ImageData_h
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
 * @brief Class Magnum::Trade::ImageData
 */

#include "AbstractTexture.h"

namespace Magnum { namespace Trade {

/**
@brief %Image

Provides access to image data and additional information about data type and
dimensions. Can be used in the same situations as Image and BufferedImage.
*/
template<size_t imageDimensions> class ImageData {
    ImageData<imageDimensions>(const ImageData<imageDimensions>& other) = delete;
    ImageData<imageDimensions>(ImageData<imageDimensions>&& other) = delete;
    ImageData<imageDimensions>& operator=(const ImageData<imageDimensions>& other) = delete;
    ImageData<imageDimensions>& operator=(ImageData<imageDimensions>&& other) = delete;

    public:
        const static size_t Dimensions = imageDimensions;   /**< @brief %Image dimension count */

        /**
         * @brief Constructor
         * @param colorFormat       Color format of passed data. Data size
         *      per color channel is detected from format of passed data array.
         * @param dimensions        %Image dimensions
         * @param data              %Image data
         *
         * @attention Note that the image data are not copied on construction,
         * but they are deleted on class destruction.
         */
        template<class T> inline ImageData(AbstractTexture::ColorFormat colorFormat, const Math::Vector<GLsizei, Dimensions>& dimensions, const T* data): _colorFormat(colorFormat), _type(TypeTraits<typename TypeTraits<T>::TextureType>::glType()), _dimensions(dimensions), _data(reinterpret_cast<const char*>(data)) {}

        /** @brief Destructor */
        inline virtual ~ImageData() { delete[] _data; }

        /** @brief %Image dimensions */
        inline const Math::Vector<GLsizei, Dimensions>& dimensions() const { return _dimensions; }

        /** @brief Color format */
        inline AbstractTexture::ColorFormat colorFormat() const { return _colorFormat; }

        /** @brief Data type */
        inline Type type() const { return _type; }

        /** @brief Pointer to raw data */
        inline const void* data() const { return _data; }

    private:
        AbstractTexture::ColorFormat _colorFormat;
        Type _type;
        Math::Vector<GLsizei, Dimensions> _dimensions;
        const char* _data;
};

/** @brief One-dimensional image */
typedef ImageData<1> ImageData1D;

/** @brief Two-dimensional image */
typedef ImageData<2> ImageData2D;

/** @brief Three-dimensional image */
typedef ImageData<3> ImageData3D;

}}

#endif