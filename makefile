CXX = g++
CXXFLAGS = -std=c++26 -Wall -Isrc

SRC = src/main.cc src/model/model.cc src/view/view.cc src/controller/controller.cc \
      src/evidence/evidence.cc src/util/sha256.cc src/util/json.cc src/util/args.cc
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
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC) && ./$(EXEC)

# 单元测试 + mock adb 集成测试（与 CTest 内容一致，供 make 用户使用）
test:
	$(CXX) $(CXXFLAGS) -Itests tests/test_main.cc tests/test_sha256.cc src/util/sha256.cc -o /tmp/test_sha256 && /tmp/test_sha256
	$(CXX) $(CXXFLAGS) -Itests tests/test_main.cc tests/test_json.cc src/util/json.cc -o /tmp/test_json && /tmp/test_json
	$(CXX) $(CXXFLAGS) -Itests tests/test_main.cc tests/test_args.cc src/util/args.cc -o /tmp/test_args && /tmp/test_args
	$(CXX) $(CXXFLAGS) -Itests tests/test_main.cc tests/test_custody.cc src/evidence/evidence.cc src/util/sha256.cc src/util/json.cc -o /tmp/test_custody && /tmp/test_custody
	PATH="$(CURDIR)/tests/mock:$$PATH" sh tests/integration_cli.sh ./$(EXEC) /tmp/lqz_itest
