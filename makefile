SRCDIR	= src
OUTDIR	= out
OBJDIR 	= $(OUTDIR)/obj
SRCS	= $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJS	= $(subst $(SRCDIR), $(OBJDIR), $(SRCS:.cpp=.o))
TARGET	= $(OUTDIR)/main

CC		= g++
CFLAGS	= -Wall -O2

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f ./out/main ./out/obj/**/*.o ./out/obj/*.o
