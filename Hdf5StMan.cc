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
        itsNrCols = ncolumn();
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
        return makeColumnMeta(name, aDataType, dataTypeId, 's');
    }

    DataManagerColumn* Hdf5StMan::makeDirArrColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnMeta(name, aDataType, dataTypeId, 'd');
    }

    DataManagerColumn* Hdf5StMan::makeIndArrColumn (const String& name, int aDataType, const String& dataTypeId){
#ifdef ADIOSSTMAN_FORCE_DIRECT_ARRAY
        return makeColumnMeta(name, aDataType, dataTypeId, 'd');
#else
        return makeColumnMeta(name, aDataType, dataTypeId, 'i');
#endif
    }

    DataManagerColumn* Hdf5StMan::makeColumnMeta (const String& name, int aDataType, const String& dataTypeId, char columnType){
        if (ncolumn() >= itsColumnPtrBlk.nelements()) {
            itsColumnPtrBlk.resize (itsColumnPtrBlk.nelements() + 32);
        }
        Hdf5StManColumn* aColumn = new Hdf5StManColumn (this, aDataType, ncolumn());
        aColumn->setColumnName(name);
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

