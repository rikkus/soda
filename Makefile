CC        = gcc
CXX       = g++
CFLAGS    = -pipe -Wall -W -g
CXXFLAGS  = -pipe -Wall -W -g
INCPATH   = -I$(QTDIR)/include -I$(KDEDIR)/include
LINK      = g++
LIBS      = -L$(KDEDIR)/lib -L$(QTDIR)/lib -L/usr/X11R6/lib \
            -lDCOP -lqt -lXext -lX11 -lm

HEADERS   = SODAParser.h
SOURCES   = SODAParser.cpp test.cpp
OBJECTS   = SODAParser.o test.o
TARGET    = test

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) -o $(TARGET) $(OBJECTS) $(LIBS)


Makefile: soda.pro
	tmake soda.pro -o Makefile

clean:
	-rm -f $(OBJECTS) $(TARGET)
	-rm -f *~ core

SODAParser.o: SODAParser.cpp SODAParser.h

test.o: test.cpp SODAParser.h

