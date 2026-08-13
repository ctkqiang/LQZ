CXX = g++
CXXFLAGS = -std=c++26 -Wall

SRC = src/main.cc src/model/model.cc src/view/view.cc src/controller/controller.cc
OBJ = $(SRC:.cc=.o)
EXEC = phone_forensic

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run:
	g++ -std=c++26 -Wall src/main.cc src/model/model.cc src/view/view.cc src/controller/controller.cc -o $(EXEC) && ./$(EXEC)