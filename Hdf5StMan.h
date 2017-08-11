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

#ifndef HDF5STMAN_H
#define HDF5STMAN_H

#include <casacore/tables/DataMan/DataManager.h>
#include <casacore/tables/Tables/Table.h>
#include <casacore/casa/IO/AipsIO.h>


namespace casacore {

    class Hdf5StManColumn;

    class Hdf5StMan : public DataManager
    {
        public:
            Hdf5StMan() = default;
            ~Hdf5StMan() = default;
            virtual DataManager* clone() const;
            virtual String dataManagerType() const;
            virtual String dataManagerName() const;
            virtual void create (uInt aNrRows);
            virtual void open (uInt aRowNr, AipsIO&);
            virtual void resync (uInt aRowNr);
            virtual Bool flush (AipsIO&, Bool doFsync);
            DataManagerColumn* makeColumnMeta (const String& aName, int aDataType, const String& aDataTypeID, char columnType);
            virtual DataManagerColumn* makeScalarColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual DataManagerColumn* makeDirArrColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual DataManagerColumn* makeIndArrColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual void deleteManager();
            virtual void addRow (uInt aNrRows);
            static DataManager* makeObject (const casa::String& aDataManType, const casa::Record& spec);
            uInt getNrRows();

        private:
            String itsDataManName = "Hdf5StMan";
            uInt itsNrRows;
            uInt itsNrCols;
            int itsStManColumnType;
            PtrBlock<Hdf5StManColumn*> itsColumnPtrBlk;
    }; // end of class Hdf5StMan

    extern "C" void register_adiosstman();
} // end of namespace casa

#endif
