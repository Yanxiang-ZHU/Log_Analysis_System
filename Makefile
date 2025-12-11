# Simple Makefile
CC = g++
CFLAGS = -std=c++11 -O2 -Wall
SRCS = main.cpp LinkedList.cpp CircularQueue.cpp BST.cpp KMP.cpp UndoStack.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = loganalyzer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)