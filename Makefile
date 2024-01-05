OBJ = main.o m1cycles.o functions.o
CXX = g++
CC = gcc
EXE = hist-len
OPT = -O2
CXXFLAGS = -std=c++11 -g $(OPT)
DEP = $(OBJ:.o=.d)

.PHONY: all clean

all: $(EXE)

$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $(EXE)

%.o: %.cc
	$(CXX) -MMD $(CXXFLAGS) -c $<

%.o: %.s
	$(CC) -MMD -c $< 




-include $(DEP)

clean:
	rm -rf $(EXE) $(OBJ) $(DEP)
