SRCDIR	= src
OUTDIR	= out
OBJDIR	= $(OUTDIR)/obj
SRCS	= $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJS	= $(subst $(SRCDIR), $(OBJDIR), $(SRCS:.cpp=.o))
TARGET	= $(OUTDIR)/main
CC		= g++
CFLAGS	= -Wall -O0

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

TESTDIR 		= test
TEST_SRCDIR		= $(TESTDIR)/src
TEST_OUTDIR		= $(TESTDIR)/out
TEST_EXTDIR		= $(TESTDIR)/external
TEST_SRCS		= $(wildcard $(TEST_SRCDIR)/*.cpp)
TEST_TARGETS	= $(subst $(TEST_SRCDIR), $(TEST_OUTDIR), $(TEST_SRCS:.cpp=))
TEST_CFLAGS		= -Wall -pthread -lgtest_main -lgtest

test: $(TEST_TARGETS)

$(TEST_OUTDIR)/cards_test:	$(TEST_SRCDIR)/cards_test.cpp $(OBJDIR)/cards.o
	$(CC) $(TEST_CFLAGS) -o $@ $^

$(TEST_OUTDIR)/hand_test:	$(TEST_SRCDIR)/hand_test.cpp $(OBJDIR)/hand.o $(OBJDIR)/cards.o
	$(CC) $(TEST_CFLAGS) -o $@ $^

clean:
	rm -f ./out/main ./out/obj/**/*.o ./out/obj/*.o ./test/out/*
