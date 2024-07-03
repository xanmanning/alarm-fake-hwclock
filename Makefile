TARGET = fake-hwclock
SOURCES = fake-hwclock.cpp NTPClient.cpp
PREFIX ?= $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin
INIT = $(PREFIX)/lib/systemd/system
DOCS = $(PREFIX)/share/man/man8
CXX ?= g++
LDFLAGS ?=
all: $(TARGET)

$(TARGET):
	$(CXX) -o $(TARGET) $(SOURCES) $(LDFLAGS)

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
