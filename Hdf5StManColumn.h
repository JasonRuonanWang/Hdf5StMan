//    (c) University of Western Australia
//    International Centre of Radio Astronomy Research
//    M468, 35 Stirling Hwy
//    Crawley, Perth WA 6009
//    Australia
//
//    Shanghai Astronomical Observatory, Chinese Academy of Sciences
//    80 Nandan Road, Shanghai 200030, China
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
//    lbq@shao.ac.cn, jason.wang@icrar.org

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

            IPosition getShapeColumn();

            int getDataTypeSize();
            int getDataType();
            void setColumnName(String aName);
            String getColumnName();

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
            virtual void getArrayMetaV (uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* data);
            virtual void putArrayMetaV (uint64_t row, const void* data);

            // StMan pointer
            Hdf5StMan *itsStManPtr;

            // Column property
            String itsColumnName;
            char itsColumnType;  // 's' for scalar, 'd' for direct array, 'i' for indirect array
            IPosition itsShape;
            int itsDataTypeSize;
            int itsCasaDataType;

    };

}


#endif
