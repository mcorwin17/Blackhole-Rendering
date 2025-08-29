CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -march=native
TARGET = blackhole
SOURCE = main.cc

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean