CC = g++
CFILES = $(shell find src/ -type f -name '*.cpp')
OFILES = $(CFILES:.cpp=.o)

TARGET = build/zeta

.PHONY = clean all

$(TARGET): $(OFILES)
	@echo [LD] $(TARGET)
	@$(CC) $(OFILES) -o $@

%.o: %.cpp
	@echo [CC] $@
	@$(CC) -I src/include/ -std=c++20 -c $< -o $@

clean:
	@echo [CLEAN]
	@rm $(OFILES) $(TARGET)

all: clean $(TARGET)

