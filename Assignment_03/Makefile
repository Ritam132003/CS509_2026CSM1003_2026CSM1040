CC = gcc
CFLAGS = -Wall -Wextra -O2 -IInclude
LDFLAGS = -lm

# On Windows, QueryPerformanceCounter is provided by the system and needs no extra library.

TARGET = assignment3_buddy
SOURCES = Driver/main.c Src/csr.c Src/gradient_descent.c Src/maxflow.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
