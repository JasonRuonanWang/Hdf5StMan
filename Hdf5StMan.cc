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

    DataManager* Hdf5StMan::makeObject (const casa::String& aDataManType, const casa::Record& spec){
        return new Hdf5StMan();
    }

    DataManager* Hdf5StMan::clone() const
    {
        return new Hdf5StMan;
    }

    String Hdf5StMan::dataManagerType() const
    {
        return itsDataManName;
    }

    void Hdf5StMan::addRow (uInt aNrRows){
        cout << "Hdf5StMan Error: addRow not supported!" << endl;
    }

    void Hdf5StMan::create (uInt aNrRows){
        itsNrRows = aNrRows;
        itsHdf5File = H5Fcreate(fileName().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        for(int i=0; i<ncolumn(); ++i){
            itsColumnPtrBlk[i]->setHdf5File(itsHdf5File);
            itsColumnPtrBlk[i]->create(aNrRows);
        }
    }

    void Hdf5StMan::open (uInt aNrRows, AipsIO& ios){
        ios.getstart(itsDataManName);
        ios >> itsDataManName;
        ios >> itsStManColumnType;
        ios.getend();

        itsNrRows = aNrRows;
    }

    void Hdf5StMan::deleteManager(){
    }

    DataManagerColumn* Hdf5StMan::makeScalarColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnCommon(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Hdf5StMan::makeDirArrColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnCommon(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Hdf5StMan::makeIndArrColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnCommon(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Hdf5StMan::makeColumnCommon (const String& name, int aDataType, const String& dataTypeId){
        if (ncolumn() >= itsColumnPtrBlk.nelements()) {
            itsColumnPtrBlk.resize (itsColumnPtrBlk.nelements() + 32);
        }
        Hdf5StManColumn* aColumn = new Hdf5StManColumn (this, aDataType, ncolumn());
        aColumn->setName(name);
        itsColumnPtrBlk[ncolumn()] = aColumn;
        return aColumn;
    }

    uInt Hdf5StMan::getNrRows(){
        return itsNrRows;
    }

    void Hdf5StMan::resync (uInt aNrRows){
    }

    Bool Hdf5StMan::flush (AipsIO& ios, Bool doFsync){
        ios.putstart(itsDataManName, 2);
        ios << itsDataManName;
        ios << itsStManColumnType;
        ios.putend();
        return true;
    }

    String Hdf5StMan::dataManagerName() const
    {
        return itsDataManName;
    }

    void register_adiosstman(){
        DataManager::registerCtor ("Hdf5StMan", Hdf5StMan::makeObject);
    }
}

