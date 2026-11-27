CXX ?= c++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = backupstore
SRCS = $(wildcard *.cpp)

$(TARGET): $(SRCS) $(wildcard *.hpp)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
