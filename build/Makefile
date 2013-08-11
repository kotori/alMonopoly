# !IMPORTANT!
# Source file names, must include them all.
Sources=database.cpp property.cpp player.cpp main.cpp

# Final executable name
Executable=monopoly

# Compiler flags
CFlags=-c -Wall -g -I/opt/development/Allegro/include/ -std=c++0x

# Linker flags
LDFlags=-lstdc++ -lsqlite3

# !IMPORTANT!
# You must change the line below to match your Allegro installation
PkgConfigFlags=`pkg-config --libs --static allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5`

# Common directories
ObjectDir=../obj/
SourceDir=../src/
BinDir=../bin/

# Required binaries
#CC=g++
CC=clang++
RM=rm -f
DIR_GUARD=@mkdir -p $(@D)
COMPRESSOR=upx

## DO NOT EDIT ANYTHING UNDER THIS LINE!!!!! ##
Objects=$(Sources:.cpp=.o)
CSources=$(addprefix $(SourceDir),$(Sources))
CObjects=$(addprefix $(ObjectDir),$(Objects))
CExecutable=$(addprefix $(BinDir),$(Executable))

all: $(CSources) $(CExecutable)

$(CExecutable): $(CObjects)
	$(DIR_GUARD)
	$(CC) $(CObjects) $(PkgConfigFlags) -o $@ $(LDFlags)

$(ObjectDir)%.o: $(SourceDir)%.cpp
	$(DIR_GUARD)
	$(CC) $(CFlags) $< -o $@

install:
	$(COMPRESSOR) $(CExecutable)

clean:
	@echo "Cleaning $(CObjects) and $(CExecutable)"
	@$(RM) $(CObjects)
	@$(RM) $(CExecutable)