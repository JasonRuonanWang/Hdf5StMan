//    (c) Oak Ridge National Laboratory
//    1 Bethel Valley Road, Oak Ridge, TN 37830, United States
//
//    This library is free software: you can redistribute it and/or
//    modify it under the terms of the GNU General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this library. If not, see <http://www.gnu.org/licenses/>.
//
//    Any bugs, questions, concerns and/or suggestions please email to
//    wangr1@ornl.gov or jason.ruonan.wang@gmail.com

#ifndef ADIOSSTMANCOLUMN_H
#define ADIOSSTMANCOLUMN_H

#include "Hdf5StMan.h"

#include <casacore/tables/DataMan/StManColumn.h>
#include <casacore/casa/Arrays/Array.h>

namespace casacore {

    class Hdf5StManColumn : public StManColumn
    {
        public:

            Hdf5StManColumn (Hdf5StMan* aParent, int aDataType, uInt aColNr);
            ~Hdf5StManColumn ();

            virtual void setShapeColumn(const IPosition& aShape);
            virtual IPosition shape(uInt aRowNr);

            int getDataTypeSize();
            int getDataType();
            void setName(String columnName);
            void setHdf5File(hid_t hdf5File);
            String getColumnName();
            void create (uInt aNrRows);

            // *** access a row for an array column ***
            virtual void putArrayV (uInt rownr, const void* dataPtr);
            virtual void getArrayV (uInt rownr, void* dataPtr);

            // *** access a slice of a row for an array column ***
            // *** inactive by default
            // *** only active when canAccessSlice() returns true in child class
            virtual void getSliceV (uInt aRowNr, const Slicer& ns, void* dataPtr);

            // *** access all rows for an array column ***
            // *** inactive by default
            // *** only active when canAccessArrayColumn() returns true in child class
            virtual void getArrayColumnV(void* dataPtr);

            // *** access a slice of all rows for an array column ***
            // *** inactive by default
            // *** only active when canAccessColumnSlice() returns true in child class
            virtual void getColumnSliceV(const Slicer& ns, void *dataPtr);

        protected:

            void getArrayWrapper(uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* dataPtr);
            virtual void getArrayCommonV (uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* data);
            virtual void putArrayCommonV (uint64_t row, const void* data);

            // StMan pointer
            Hdf5StMan *itsStManPtr;
            hid_t itsHdf5File;

            // Column property
            String itsColumnName;
            char itsColumnType;  // 's' for scalar, 'd' for direct array, 'i' for indirect array
            IPosition itsCasaShape;
            std::vector<hsize_t> itsHdf5Shape;
            int itsDataTypeSize;
            int itsCasaDataType;
            hid_t itsHdf5DataType;
            hid_t itsHdf5DataSpace;
            hid_t itsHdf5DataSet;

    };

}


#endif
