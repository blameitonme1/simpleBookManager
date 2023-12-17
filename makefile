GPP=g++

all: bookmanager
	./bookmanager

bookmanager: bookmanager.cpp
	$(GPP) -o bookmanager bookmanager.cpp

clear:
	rm -f bookmanager