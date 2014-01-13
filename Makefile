TARGET = fake-hwclock
SOURCES = fake-hwclock.c
PREFIX = $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET):
	gcc -o $(TARGET) $(SOURCES)

install:
	install -D $(TARGET) $(BINDIR)/$(TARGET)

.PHONY: install
