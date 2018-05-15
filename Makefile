CC = g++
CFLAGS = -std=c++11 -c -fPIC -shared
CFLAGS_STATIC = -std=c++11 -c -fPIC
INCLUDES = -I/home/wondrash/c++/include
SRCS = ReadColumn.cpp props.cpp
OBJS = $(SRCS:.cpp=.o)

default: libprops.so
	@echo "libprops.so has been compiled"

static: libprops.a
	@echo "libprops.a has been compiled"

libprops.a: $(OBJS)
	ar rvs /home/wondrash/c++/lib/libprops.a $(OBJS)

libprops.so: $(OBJS)
	$(CC) -shared $(OBJS) -o /home/wondrash/c++/lib/libprops.so

%.o:%.cpp
	$(CC) $(CFLAGS_STATIC) $(INCLUDES) $< -o $@

clean:
	rm $(OBJS) /home/wondrash/c++/lib/libprops.so /home/wondrash/c++/lib/libprops.a
