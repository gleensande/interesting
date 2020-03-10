TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   include

SRCS = \
       src/main.cpp \
	   src/csv_parser.cpp \
	   src/matrix.cpp



.PHONY: all clean

all: $(SRCS)
	$(CXX) -std=c++11 -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)
