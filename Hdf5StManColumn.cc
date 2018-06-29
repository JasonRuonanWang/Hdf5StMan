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

#include "Hdf5StManColumn.h"


namespace casacore {

    Hdf5StManColumn::Hdf5StManColumn (Hdf5StMan* aParent, int aDataType, uInt aColNr, String columnName)
        :StManColumn (aDataType),
        itsStManPtr (aParent),
        itsCasaDataType(aDataType),
        itsColumnName(columnName)
    {

        switch (aDataType){
            case TpBool:
            case TpArrayBool:
                itsHdf5DataType = H5T_NATIVE_CHAR;
                itsDataTypeSize = sizeof(Bool);
                break;
            case TpChar:
            case TpArrayChar:
                itsHdf5DataType = H5T_NATIVE_CHAR;
                itsDataTypeSize = sizeof(Char);
                break;
            case TpUChar:
            case TpArrayUChar:
                itsHdf5DataType = H5T_NATIVE_UCHAR;
                itsDataTypeSize = sizeof(uChar);
                break;
            case TpShort:
            case TpArrayShort:
                itsHdf5DataType = H5T_NATIVE_SHORT;
                itsDataTypeSize = sizeof(Short);
                break;
            case TpUShort:
            case TpArrayUShort:
                itsHdf5DataType = H5T_NATIVE_USHORT;
                itsDataTypeSize = sizeof(uShort);
                break;
            case TpInt:
            case TpArrayInt:
                itsHdf5DataType = H5T_NATIVE_INT;
                itsDataTypeSize = sizeof(Int);
                break;
            case TpUInt:
            case TpArrayUInt:
                itsHdf5DataType = H5T_NATIVE_UINT;
                itsDataTypeSize = sizeof(uInt);
                break;
            case TpFloat:
            case TpArrayFloat:
                itsHdf5DataType = H5T_NATIVE_FLOAT;
                itsDataTypeSize = sizeof(Float);
                break;
            case TpDouble:
            case TpArrayDouble:
                itsHdf5DataType = H5T_NATIVE_DOUBLE;
                itsDataTypeSize = sizeof(Double);
                break;
            case TpComplex:
            case TpArrayComplex:
                itsHdf5DataType = H5T_NATIVE_DOUBLE;
                itsDataTypeSize = sizeof(Complex);
                break;
            case TpDComplex:
            case TpArrayDComplex:
                itsHdf5DataType = H5T_NATIVE_LDOUBLE;
                itsDataTypeSize = sizeof(DComplex);
                break;
            case TpString:
            case TpArrayString:
                itsDataTypeSize = -1;
                break;
        }
    }

    Hdf5StManColumn::~Hdf5StManColumn (){
    }

    String Hdf5StManColumn::getColumnName (){
        return itsColumnName;
    }

    int Hdf5StManColumn::getDataTypeSize(){
        return itsDataTypeSize;
    }

    int Hdf5StManColumn::getDataType(){
        return itsCasaDataType;
    }

    void Hdf5StManColumn::setShapeColumn(const IPosition& aShape){
        itsCasaShape = aShape;
        itsHdf5Shape.resize(aShape.size() + 1);
        for(int i=1; i<itsCasaShape.size() + 1; ++i){
            itsHdf5Shape[i] = itsCasaShape[i-1];
        }
    }

    IPosition Hdf5StManColumn::shape(uInt aRowNr){
        return itsCasaShape;
    }

    void Hdf5StManColumn::create (uInt aNrRows, hid_t hdf5File){
        itsHdf5File = hdf5File;
        const size_t dim = itsHdf5Shape.size();
        itsHdf5Shape[0] = aNrRows;
        itsHdf5DataSpace = H5Screate_simple(dim, itsHdf5Shape.data(), NULL);
        itsHdf5DataSet = H5Dcreate2(itsHdf5File, ("/" + itsColumnName).c_str(), itsHdf5DataType, itsHdf5DataSpace, H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
        for(auto i : itsHdf5Shape){
            std::cout << i << "  ";
        }
        std::cout << std::endl;
    }

    void Hdf5StManColumn::putArrayCommonV (uint64_t row, const void* data){
        const size_t dim = itsHdf5Shape.size();
        std::vector<hsize_t> start(dim);
        for(auto &i : start){
            i = 0;
        }
        start[0] = row;
        std::vector<hsize_t> stride(dim);
        for(auto &i : stride){
            i = 1;
        }
        std::vector<hsize_t> count = itsHdf5Shape;
        count[0] = 1;
        std::vector<hsize_t> block(dim);
        for(auto &i : block){
            i = 1;
        }
        hid_t localDataspace = H5Screate_simple(dim, count.data(), NULL);
        H5Sselect_hyperslab(itsHdf5DataSpace, H5S_SELECT_SET, start.data(), stride.data(), count.data(), block.data());
        H5Dwrite(itsHdf5DataSet, itsHdf5DataType, localDataspace, itsHdf5DataSpace, H5P_DEFAULT, data);
    }

    void Hdf5StManColumn::getArrayCommonV (uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* data){
    }

    // *** access a row for an array column ***
    void Hdf5StManColumn::putArrayV (uInt rownr, const void* dataPtr){
        Bool deleteIt;
        const void *data;
        switch (itsCasaDataType){
            case TpArrayBool:
            case TpBool:
                data = ((const Array<Bool>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Bool>*)dataPtr)->freeStorage((const Bool*&)data, deleteIt);
                break;
            case TpArrayChar:
            case TpChar:
                data = ((const Array<Char>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Char>*)dataPtr)->freeStorage((const Char*&)data, deleteIt);
                break;
            case TpArrayUChar:
            case TpUChar:
                data = ((const Array<uChar>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<uChar>*)dataPtr)->freeStorage((const uChar*&)data, deleteIt);
                break;
            case TpArrayShort:
            case TpShort:
                data = ((const Array<Short>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Short>*)dataPtr)->freeStorage((const Short*&)data, deleteIt);
                break;
            case TpArrayUShort:
            case TpUShort:
                data = ((const Array<uShort>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<uShort>*)dataPtr)->freeStorage((const uShort*&)data, deleteIt);
                break;
            case TpArrayInt:
            case TpInt:
                data = ((const Array<Int>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Int>*)dataPtr)->freeStorage((const Int*&)data, deleteIt);
                break;
            case TpArrayUInt:
            case TpUInt:
                data = ((const Array<uInt>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<uInt>*)dataPtr)->freeStorage((const uInt*&)data, deleteIt);
                break;
            case TpArrayFloat:
            case TpFloat:
                data = ((const Array<Float>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Float>*)dataPtr)->freeStorage((const Float*&)data, deleteIt);
                break;
            case TpArrayDouble:
            case TpDouble:
                data = ((const Array<Double>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Double>*)dataPtr)->freeStorage((const Double*&)data, deleteIt);
                break;
            case TpArrayComplex:
            case TpComplex:
                data = ((const Array<Complex>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<Complex>*)dataPtr)->freeStorage((const Complex*&)data, deleteIt);
                break;
            case TpArrayDComplex:
            case TpDComplex:
                data = ((const Array<DComplex>*)dataPtr)->getStorage (deleteIt);
                putArrayCommonV(rownr, data);
                ((const Array<DComplex>*)dataPtr)->freeStorage((const DComplex*&)data, deleteIt);
                break;
        }
    }
    void Hdf5StManColumn::getArrayV (uInt rownr, void* dataPtr){
        Slicer ns(IPosition(itsCasaShape.size(),0,0,0,0,0,0,0,0,0,0), itsCasaShape);
        getArrayWrapper(rownr, 1, ns, dataPtr);
    }

    // *** access a slice of a row for an array column ***
    // *** inactive by default
    // *** only active when canAccessSlice() returns true in child class
    void Hdf5StManColumn::getSliceV(uInt aRowNr, const Slicer& ns, void* dataPtr){
        getArrayWrapper(aRowNr, 1, ns, dataPtr);
    }

    // *** access all rows for an array column ***
    // *** inactive by default
    // *** only active when canAccessArrayColumn() returns true in child class
    void Hdf5StManColumn::getArrayColumnV(void* dataPtr){
        Slicer ns(IPosition(itsCasaShape.size(),0,0,0,0,0,0,0,0,0,0), itsCasaShape);
        getArrayWrapper(0, itsStManPtr->getNrRows(), ns, dataPtr);
    }

    // *** access a slice of all rows for an array column ***
    // *** inactive by default
    // *** only active when canAccessColumnSlice() returns true in child class
    void Hdf5StManColumn::getColumnSliceV(const Slicer& ns, void *dataPtr){
        getArrayWrapper(0, itsStManPtr->getNrRows(), ns, dataPtr);
    }


    void Hdf5StManColumn::getArrayWrapper(uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* dataPtr){
        Bool deleteIt;
        void *data;
        switch (itsCasaDataType){
            case TpArrayBool:
            case TpBool:
                data = ((Array<Bool>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Bool>*)dataPtr)->putStorage ((Bool *&)data, deleteIt);
                break;
            case TpArrayChar:
            case TpChar:
                data = ((Array<Char>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Char>*)dataPtr)->putStorage ((Char *&)data, deleteIt);
                break;
            case TpArrayUChar:
            case TpUChar:
                data = ((Array<uChar>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<uChar>*)dataPtr)->putStorage ((uChar *&)data, deleteIt);
                break;
            case TpArrayShort:
            case TpShort:
                data = ((Array<Short>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Short>*)dataPtr)->putStorage ((Short *&)data, deleteIt);
                break;
            case TpArrayUShort:
            case TpUShort:
                data = ((Array<uShort>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<uShort>*)dataPtr)->putStorage ((uShort *&)data, deleteIt);
                break;
            case TpArrayInt:
            case TpInt:
                data = ((Array<Int>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Int>*)dataPtr)->putStorage ((Int *&)data, deleteIt);
                break;
            case TpArrayUInt:
            case TpUInt:
                data = ((Array<uInt>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<uInt>*)dataPtr)->putStorage ((uInt *&)data, deleteIt);
                break;
            case TpArrayFloat:
            case TpFloat:
                data = ((Array<Float>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Float>*)dataPtr)->putStorage ((Float *&)data, deleteIt);
                break;
            case TpArrayDouble:
            case TpDouble:
                data = ((Array<Double>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Double>*)dataPtr)->putStorage ((Double *&)data, deleteIt);
                break;
            case TpArrayComplex:
            case TpComplex:
                data = ((Array<Complex>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<Complex>*)dataPtr)->putStorage ((Complex *&)data, deleteIt);
                break;
            case TpArrayDComplex:
            case TpDComplex:
                data = ((Array<DComplex>*)dataPtr)->getStorage (deleteIt);
                getArrayCommonV(rowStart, nrRows, ns, data);
                ((Array<DComplex>*)dataPtr)->putStorage ((DComplex *&)data, deleteIt);
                break;
        }
    }
}

