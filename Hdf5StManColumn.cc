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

#include "Hdf5StManColumn.h"


namespace casacore {

    Hdf5StManColumn::Hdf5StManColumn (Hdf5StMan* aParent, int aDataType, uInt aColNr)
        :StManColumn (aDataType),
        itsStManPtr (aParent),
        itsCasaDataType(aDataType),
        itsShape(0)
    {
    }

    Hdf5StManColumn::~Hdf5StManColumn (){
    }

    void Hdf5StManColumn::setColumnName (String aName){
        itsColumnName = aName;
    }

    String Hdf5StManColumn::getColumnName (){
        return itsColumnName;
    }

    IPosition Hdf5StManColumn::getShapeColumn (){
        return itsShape;
    }

    int Hdf5StManColumn::getDataTypeSize(){
        return itsDataTypeSize;
    }

    int Hdf5StManColumn::getDataType(){
        return itsCasaDataType;
    }

    // ------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------

    void Hdf5StManColumn::putArrayMetaV (uint64_t row, const void* data){
    }

    void Hdf5StManColumn::getArrayMetaV (uint64_t rowStart, uint64_t nrRows, const Slicer& ns, void* data){
    }


    // *** access a row for an array column ***
    void Hdf5StManColumn::putArrayV (uInt rownr, const void* dataPtr){
        Bool deleteIt;
        const void *data;
        switch (itsCasaDataType){
            case TpArrayBool:
            case TpBool:
                data = ((const Array<Bool>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Bool>*)dataPtr)->freeStorage((const Bool*&)data, deleteIt);
                break;
            case TpArrayChar:
            case TpChar:
                data = ((const Array<Char>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Char>*)dataPtr)->freeStorage((const Char*&)data, deleteIt);
                break;
            case TpArrayUChar:
            case TpUChar:
                data = ((const Array<uChar>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<uChar>*)dataPtr)->freeStorage((const uChar*&)data, deleteIt);
                break;
            case TpArrayShort:
            case TpShort:
                data = ((const Array<Short>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Short>*)dataPtr)->freeStorage((const Short*&)data, deleteIt);
                break;
            case TpArrayUShort:
            case TpUShort:
                data = ((const Array<uShort>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<uShort>*)dataPtr)->freeStorage((const uShort*&)data, deleteIt);
                break;
            case TpArrayInt:
            case TpInt:
                data = ((const Array<Int>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Int>*)dataPtr)->freeStorage((const Int*&)data, deleteIt);
                break;
            case TpArrayUInt:
            case TpUInt:
                data = ((const Array<uInt>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<uInt>*)dataPtr)->freeStorage((const uInt*&)data, deleteIt);
                break;
            case TpArrayFloat:
            case TpFloat:
                data = ((const Array<Float>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Float>*)dataPtr)->freeStorage((const Float*&)data, deleteIt);
                break;
            case TpArrayDouble:
            case TpDouble:
                data = ((const Array<Double>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Double>*)dataPtr)->freeStorage((const Double*&)data, deleteIt);
                break;
            case TpArrayComplex:
            case TpComplex:
                data = ((const Array<Complex>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<Complex>*)dataPtr)->freeStorage((const Complex*&)data, deleteIt);
                break;
            case TpArrayDComplex:
            case TpDComplex:
                data = ((const Array<DComplex>*)dataPtr)->getStorage (deleteIt);
                putArrayMetaV(rownr, data);
                ((const Array<DComplex>*)dataPtr)->freeStorage((const DComplex*&)data, deleteIt);
                break;
        }
    }
    void Hdf5StManColumn::getArrayV (uInt rownr, void* dataPtr){
        Slicer ns(IPosition(itsShape.size(),0,0,0,0,0,0,0,0,0,0), itsShape);
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
        Slicer ns(IPosition(itsShape.size(),0,0,0,0,0,0,0,0,0,0), itsShape);
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
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Bool>*)dataPtr)->putStorage ((Bool *&)data, deleteIt);
                break;
            case TpArrayChar:
            case TpChar:
                data = ((Array<Char>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Char>*)dataPtr)->putStorage ((Char *&)data, deleteIt);
                break;
            case TpArrayUChar:
            case TpUChar:
                data = ((Array<uChar>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<uChar>*)dataPtr)->putStorage ((uChar *&)data, deleteIt);
                break;
            case TpArrayShort:
            case TpShort:
                data = ((Array<Short>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Short>*)dataPtr)->putStorage ((Short *&)data, deleteIt);
                break;
            case TpArrayUShort:
            case TpUShort:
                data = ((Array<uShort>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<uShort>*)dataPtr)->putStorage ((uShort *&)data, deleteIt);
                break;
            case TpArrayInt:
            case TpInt:
                data = ((Array<Int>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Int>*)dataPtr)->putStorage ((Int *&)data, deleteIt);
                break;
            case TpArrayUInt:
            case TpUInt:
                data = ((Array<uInt>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<uInt>*)dataPtr)->putStorage ((uInt *&)data, deleteIt);
                break;
            case TpArrayFloat:
            case TpFloat:
                data = ((Array<Float>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Float>*)dataPtr)->putStorage ((Float *&)data, deleteIt);
                break;
            case TpArrayDouble:
            case TpDouble:
                data = ((Array<Double>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Double>*)dataPtr)->putStorage ((Double *&)data, deleteIt);
                break;
            case TpArrayComplex:
            case TpComplex:
                data = ((Array<Complex>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<Complex>*)dataPtr)->putStorage ((Complex *&)data, deleteIt);
                break;
            case TpArrayDComplex:
            case TpDComplex:
                data = ((Array<DComplex>*)dataPtr)->getStorage (deleteIt);
                getArrayMetaV(rowStart, nrRows, ns, data);
                ((Array<DComplex>*)dataPtr)->putStorage ((DComplex *&)data, deleteIt);
                break;
        }
    }
}

