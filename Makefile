# Define variables (conventionally in uppercase)
CXX = g++
TARGET = topend
OBJS = topcomputer.o pwmmath.o

# The default target (often 'all') builds the program
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Rule to compile main.o from main.cpp and utils.h
topcomputer.o: topcomputer.cpp pwmmath.hpp
	$(CXX) -c topcomputer.cpp

# Rule to compile utils.o from utils.cpp and utils.h
pwmmath.o: pwmmath.cpp pwmmath.hpp
	$(CXX) -c pwmmath.cpp

# Phony target to clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)
