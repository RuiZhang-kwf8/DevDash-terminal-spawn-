CXX = g++
CXXFLAGS = -std=c++17 -O2

SRC = src/main.cpp src/order_book.cpp
OUT = engine

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)