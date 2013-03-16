CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		utf-text-flip.o

LIBS =

TARGET =	utf-text-flip

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
