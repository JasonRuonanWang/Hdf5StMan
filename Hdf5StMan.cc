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

    Hdf5StMan::Hdf5StMan()
        :DataManager()
    {
    }
#ifdef HAVE_MPI
    Hdf5StMan::Hdf5StMan(MPI_Comm mpiComm)
        :DataManager(),
        itsMpiComm(mpiComm)
    {
    }
#endif

    DataManager* Hdf5StMan::makeObject (const String& aDataManType, const Record& spec){
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
        std::string itsFileName = fileName();

#ifdef HAVE_MPI
        // broadcast filename from Rank 0 to other ranks
        int itsFileNameLen = itsFileName.length();
        MPI_Bcast(&itsFileNameLen, 1, MPI_INT, 0, itsMpiComm);
        std::vector<char> itsFileNameVec(itsFileNameLen + 1);
        std::memcpy(itsFileNameVec.data(), itsFileName.data(), itsFileNameLen);
        MPI_Bcast(itsFileNameVec.data(), itsFileNameLen, MPI_CHAR, 0, itsMpiComm);
        itsFileNameVec[itsFileNameLen] = '\0';
        itsFileName = itsFileNameVec.data();
        // define parallel IO for HDF5
        hid_t access_template = H5Pcreate (H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(access_template, itsMpiComm, MPI_INFO_NULL);
#else
        hid_t access_template = H5P_DEFAULT;
#endif
        itsHdf5File = H5Fcreate(itsFileName.data(), H5F_ACC_TRUNC, H5P_DEFAULT, access_template);
        for(int i=0; i<ncolumn(); ++i){
            itsColumnPtrBlk[i]->create(aNrRows, itsHdf5File);
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
        Hdf5StManColumn* aColumn = new Hdf5StManColumn (this, aDataType, ncolumn(), name);
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

