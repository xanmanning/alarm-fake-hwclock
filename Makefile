TARGET = fake-hwclock
SOURCES = fake-hwclock.c
PREFIX ?= $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin
INIT = $(PREFIX)/lib/systemd/system
DOCS = $(PREFIX)/share/man/man8
CC ?= gcc
all: $(TARGET)

$(TARGET):
	$(CC) -o $(TARGET) $(SOURCES)

install:
	install -D $(TARGET) $(BINDIR)/$(TARGET)
	install -d $(INIT)
	install -m644 systemd/$(TARGET).service $(INIT)/$(TARGET).service
	install -m644 systemd/$(TARGET).timer $(INIT)/$(TARGET).timer
	gzip --force --keep -9 man/$(TARGET).8
	install -d $(DOCS)
	install -m644 man/$(TARGET).8.gz $(DOCS)/$(TARGET).8.gz

clean:
	rm -f $(TARGET)

.PHONY: install
