all: compile


run: build/card.exe
	build/card.exe


compile: build/card.exe

build/card.exe: build/Makefile main.cpp Player.cpp Player.h CardEncoder.cpp CardEncoder.h DataUtil.cpp DataUtil.h card.generated.h
	$(MAKE) -C build all MAKEFLAGS=--no-print-directory


translate: card.generated.h

card.generated.h: card.xc
	java -Xss6M -jar ../../ableC.jar card.xc --show-lifted-pp > card.generated.h
		
	
prepare: build/Makefile

build/Makefile: CMakeLists.txt $(if $(wildcard build),,build) $(if $(wildcard card.generated.h),,card.generated.h)
	cd build &&\
	cmake -DCMAKE_PREFIX_PATH=/home/fenix/PyTorch/libtorch .. &&\
	cd ..	

build:
	if [ ! -d build ]; then mkdir build; fi


clean:
	rm -f *.i *.o *.exe *.out *.generated.h; \
	$(MAKE) -C build clean MAKEFLAGS=--no-print-directory

deep_clean:
	rm -f *.i *.o *.exe *.out *.generated.h; \
	rm -rf build 
