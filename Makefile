CXX = g++
CXXFLAGS = -std=c++17

INCLUDES = \
    -I./matplotlib-cpp \
    -I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12 \
    -I/Library/Frameworks/Python.framework/Versions/3.12/lib/python3.12/site-packages/numpy/_core/include

LDFLAGS = \
    -L/Library/Frameworks/Python.framework/Versions/3.12/lib \
    -lpython3.12

# Executables
DEMO_TARGET = demo
MAIN_TARGET = main

# Sources
DEMO_SRC = demo.cpp
MAIN_SRC = main.cpp

all: $(DEMO_TARGET) $(MAIN_TARGET)

$(DEMO_TARGET): $(DEMO_SRC)
	$(CXX) $(DEMO_SRC) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $(DEMO_TARGET)

$(MAIN_TARGET): $(MAIN_SRC)
	$(CXX) $(MAIN_SRC) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $(MAIN_TARGET)

clean:
	rm -f $(DEMO_TARGET) $(MAIN_TARGET)