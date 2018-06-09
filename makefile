ifeq ($(VENDOR),cray)
	CC=cc -dynamic
else
	CC=mpic++
endif

TARGET=libhdf5stman.so
SRC=Hdf5StMan.cc Hdf5StManColumn.cc
DIRS=tests

FLAGS=-DHAVE_MPI


$(TARGET):$(SRC)
	$(CC) $(SRC) -fPIC --shared -o $(TARGET) -lcasa_tables -lcasa_casa -lhdf5 -std=c++11 $(FLAGS)

ifdef CASA_LIB
	cp $(TARGET) $(CASA_LIB)
endif
ifdef CASA_INC
	cp *.h $(CASA_INC)/casacore/tables/DataMan
endif

all:$(TARGET)
	for d in $(DIRS); do(cd $$d; rm -f $(TARGET); ln -sf ../$(TARGET) ./; make);  done

all:$(TARGET)

re:cl $(TARGET)
	

cl:
	rm -f *.so

clean:
	rm -rf *.so
	for d in $(DIRS); do( cd $$d; make clean);  done

ln:$(TARGET)
	for d in $(DIRS); do(cd $$d; rm -f $(TARGET); ln -sf ../$(TARGET) ./);  done
