SOURCES=$(shell find src -type f -iname '*.c')

HEADERS=$(shell find include -type f -iname '*.h')

OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

TARGET=bin/stegowav

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(HEADERS)

clean:
	rm -f $(TARGET) $(OBJECTS)
