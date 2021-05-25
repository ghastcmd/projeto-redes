src = src
bin = bin
deps = bin/deps

target = bin/bin.out
pch = src/pch.hpp
gch = $(pch:.hpp=.hpp.gch)

flags = -Wall -Wextra -Werror -Wno-unused-variable -Wno-deprecated-declarations

SS = @
CC = g++

ifeq ($(OS),Windows_NT)
	target := $(target:.out=.exe)
	DOS := Windows
	deps := $(subst /,\,$(deps))
	libs := Ws2_32
	CC := clang++
else
	DOS := $(shell uname -s)
	libs := pthread
endif

libs_inc = $(addprefix -l,$(libs))

depends = $(patsubst %,$(deps)/%.d,$(basename $(notdir $(source))))

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(bin)/%.o,$(notdir $(basename $(source))))

build: ; $(SS)$(MAKE) -s --no-print-directory -j 4 compile

compile: $(gch) $(target) | $(deps)

run: compile ; $(target)

run_server: compile ; $(target) server
run_client: compile ; $(target) client

$(target): $(object)
	$(SS)echo Compiling $@ from $^
	$(SS)$(CC) $(flags) $^ -o $@ $(libs_inc)

vpath %.cpp $(src)
$(bin)/%.o: %.cpp
	$(SS)echo Compiling $< into $@
	$(SS)$(CC) -MMD -MT $@ -MP -MF $(deps)/$*.d $(flags) -c $< -o $@

$(gch): $(pch)
	$(SS)echo Compiling precompiled header
	$(SS)$(CC) -c $< -o $@

$(depends):
include $(depends)

rec=
ifeq ($(DOS),Linux)
	rec := -p
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

relatorio:
	$(SS)bash -c 'pandoc relatorio.md -o relatorio.pdf'