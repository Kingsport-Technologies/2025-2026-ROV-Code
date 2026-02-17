# Define variables (conventionally in uppercase)
CXX = g++
TARGET = topend
OBJS = topcomputer.o pwmmath.o websocketserver.o

# The default target (often 'all') builds the program
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

topcomputer.o: topcomputer.cpp websocketserver.hpp
	$(CXX) -c topcomputer.cpp
pwmmath.o: pwmmath.cpp pwmmath.hpp
	$(CXX) -c pwmmath.cpp
websocketserver.o: websocketserver.cpp websocketserver.hpp pwmmath.hpp
	$(CXX) -c websocketserver.cpp
# Phony target to clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)
