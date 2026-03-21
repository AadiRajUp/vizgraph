CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_FLAGS = -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

TARGET = visualizer
SRCS = main.cpp utils.cpp linegraph.cpp piechart.cpp donutchart.cpp bargraph.cpp histogram.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFML_FLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -IC:/SFML/include -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).exe