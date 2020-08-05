compile: card.exe

card.exe: main.cpp Player.cpp Player.h card.generated.h
	g++ -std=c++11 -O3 -I../../../include/ *.cpp ../../../include/common/*.cpp -o card.exe

translate: card.generated.h

card.generated.h: card.xc
	java -Xss6M -jar ../../../ableC.jar card.xc --show-lifted-pp > card.generated.h


clean:
	rm -f *.i *.o *.exe *.out *.generated.h
