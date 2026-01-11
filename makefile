CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = main.cc model/model.cc view/view.cc controller/controller.cc
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
	g++ -std=c++17 -Wall main.cc model/model.cc view/view.cc controller/controller.cc -o $(EXEC) && ./$(EXEC)