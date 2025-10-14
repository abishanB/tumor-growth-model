
CXX = g++
CXXFLAGS = -std=c++17
INCLUDES = \
    -I./matplotlib-cpp \
    -I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12 \
    -I/Library/Frameworks/Python.framework/Versions/3.12/lib/python3.12/site-packages/numpy/_core/include

LDFLAGS = \
    -L/Library/Frameworks/Python.framework/Versions/3.12/lib \
    -lpython3.12

# Target and source
TARGET = plot
SRC = plot.cpp

# Default build rule
$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)