src = src
bin = bin
deps = bin/deps

target_ext=.exe
target = bin/bin$(target_ext)

pch = src/pch.hpp
gch = $(pch:.hpp=.hpp.gch)

flags = -Wall -Wextra -Werror -Wno-unused-variable -Wno-deprecated-declarations

SS = @
CC = g++

ifeq ($(OS),Windows_NT)
	DOS := Windows
	deps := $(subst /,\,$(deps))
	libs := Ws2_32
	CC := clang++
else
	target_ext=.out
	DOS := $(shell uname -s)
	libs := pthread
endif

libs_inc = $(addprefix -l,$(libs))

depends = $(patsubst %,$(deps)/%.d,$(basename $(notdir $(source))))
defines = $(addprefix -D,$(defs))

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(bin)/%.o,$(notdir $(basename $(source))))

.PHONY: compile_client compile_server

build: ; $(SS)$(MAKE) -s --no-print-directory -j 4 compile

compile: $(deps) $(gch) $(target)

run: compile ; $(target)
	@echo $(target)

help: compile_client another

compile_client:
	$(eval defs+= CLIENT)
	$(eval target=$(bin)/client$(target_ext))

compile_server:
	$(eval defs+= SERVER)
	$(eval target=$(bin)/server$(target_ext))

client_bin = $(bin)/client.out
server_bin = $(bin)/server.out

client:
	$(SS)$(MAKE) -s --no-print-directory -j 4 target=$(client_bin) defs+=CLIENT compile

server:
	$(SS)$(MAKE) -s --no-print-directory -j 4 target=$(server_bin) defs+=SERVER compile

run_server: server ; $(server_bin)
run_client: client ; $(client_bin)

$(target): $(object)
	$(SS)echo Compiling $@ from $^
	$(SS)$(CC) $(flags) $^ -o $@ $(libs_inc) $(defines)

vpath %.cpp $(src)
$(bin)/%.o: %.cpp
	$(SS)echo Compiling $< into $@
	$(SS)$(CC) -MMD -MT $@ -MP -MF $(deps)/$*.d $(flags) -c $< -o $@ $(defines)

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