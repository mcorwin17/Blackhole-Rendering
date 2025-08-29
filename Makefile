CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = blackhole
SOURCE = main.cc

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean