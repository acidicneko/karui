CC = g++
CPPFILES = $(shell find src/ -type f -name '*.cpp')
CFILES = $(shell find src/ -type f -name '*.c')

OFILES = $(CFILES:.c=.o)
OFILES += $(CPPFILES:.cpp=.o)

TARGET = build/scbuild

.PHONY = clean all

$(TARGET): $(OFILES)
	@echo [LD] $(TARGET)
	@$(CC) $(OFILES) -o $@

%.o: %.cpp
	@echo [CC] $@
	@$(CC) -I src/include/ -std=c++20 -c $< -o $@

%.o: %.c
	@echo [CC] $@
	@gcc -I src/include/ -c $< -o $@

clean:
	@echo [CLEAN]
	@rm $(OFILES) $(TARGET)

all: clean $(TARGET)

