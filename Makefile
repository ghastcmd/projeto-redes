src := src
bin := bin
deps := bin/deps

target := bin/bin.out
pch = src/pch.hpp
gch = $(pch:.hpp=.hpp.gch)

ifeq ($(OS),Windows_NT)
	target := $(target:.out=.exe)
	DOS := Windows
	deps := $(subst /,\,$(deps))
else
	DOS := $(shell uname -s)
endif

help:
	@echo $(deps)

SS = @
CC = g++

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(bin)/%.o,$(notdir $(basename $(source))))

build: ; $(SS)$(MAKE) -s --no-print-directory -j 4 compile

compile: $(gch) $(target) | $(deps)

run: compile
	$(target)

$(target): $(object)
	$(SS)$(CC) $^ -o $@

vpath %.cpp $(src)
$(bin)/%.o: %.cpp
	$(SS)$(CC) -MMD -MT $@ -MP -MF $(deps)/$*.d -c $< -o $@

$(gch): $(pch)
	$(SS)$(CC) -c $< -o $@

rec=
ifeq ($(DOS),Linux)
	rec := -r
else
	rec := 
endif

$(deps): ; $(SS)mkdir $(rec) $@

clean:
	$(SS)echo Cleaning bin folder
	$(SS)rm -f $(object)

clean_pch:
	$(SS)echo Cleaning pch compiled file
	$(SS)rm -f $(gch)
