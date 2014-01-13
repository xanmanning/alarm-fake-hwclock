TARGET = fake-hwclock
SOURCES = fake-hwclock.c
BINDIR = /usr/bin

all: $(TARGET)

$(TARGET):
	gcc -o $(TARGET) $(SOURCES)

install:
	install -D $(TARGET) $(BINDIR)/$(TARGET)

.PHONY: install
