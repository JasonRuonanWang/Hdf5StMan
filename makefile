ifeq ($(VENDOR),cray)
	CC=cc -dynamic
else
	CC=clang++
endif

TARGET=libhdf5stman.so
SRC=Hdf5StMan.cc Hdf5StManColumn.cc

$(TARGET):$(SRC)
	$(CC) $(SRC) -fPIC --shared -o $(TARGET) -lcasa_tables -lcasa_casa -ladios -lmxml -lz -std=c++11

ifdef CASA_LIB
	cp $(TARGET) $(CASA_LIB)
endif
ifdef CASA_INC
	cp *.h $(CASA_INC)/casacore/tables/DataMan
endif

all:$(TARGET)

re:cl $(TARGET)
	

cl:
	rm -f *.so

clean:
	rm -rf *.so

