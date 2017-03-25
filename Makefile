# reminder
# -- rule --
# target : prerequisite
# <TAB> command
#
# Automatic variables:
# $@ target name
# $< first prerequisite name
# $^ all prerequisites separated by a space

# --------- Variables ---------
TARGET := program
PLATFORM :=
OBJS := extensions.o main.o bmp.o
DEBUG :=
CFLAGS := -c $(DEBUG) -Wall
LFLAGS_WIN := -lSDL2 -mwindows
LFLAGS_LNX := -lSDL2 -lm
INCLUDE := include
OBJDIR := obj
SRCDIR := src
# --- Options ---
.PHONY : windows linux clean
vpath %.c $(SRCDIR)/

# ----------- Rules -----------
# compile on windows (default)
windows: $(TARGET).exe

# compile on linux
linux: $(TARGET).out


$(TARGET).exe: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo Linking...
	gcc $^ $(PLATFORM) $(LFLAGS_WIN) -o $@
	@echo Linked

$(TARGET).out: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo linking...
	gcc $^ $(PLATFORM) $(LFLAGS_LNX) -o $@
	@echo Linked

$(addprefix $(OBJDIR)/,extensions.o main.o): $(INCLUDE)/extensions.h

$(OBJDIR)/%.o : %.c
	gcc -I$(INCLUDE) $(CFLAGS) $< $(PLATFORM) -o $@


clean:
	del /F $(OBJDIR)\*.o $(TARGET).out $(TARGET).exe
