CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = main.cc model/model.cc view/view.cc controller/controller.cc
OBJ = $(SRC:.cpp=.o)
EXEC = phone_forensic

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run:
	g++ -std=c++17 -Wall main.cpp model/model.cpp view/view.cpp controller/controller.cpp -o $(EXEC) && ./$(EXEC)