# Remove any default suffix rules
.SUFFIXES:

# Add only the necessary suffixes
.SUFFIXES: .cpp .o

# Name of the executable
EXEC = tp5

# List of source files
SOURCES = main_cryptanalysis.cpp

# Object files list (automatically generated from source files)
OBJETS = $(SOURCES:.cpp=.o)

# Compiler and flags
CCPP = g++
CFLAGS = -W -Wall -Wextra -pedantic -std=c++0x -I/usr/X11R6/include
LFLAGS = -L. -L/usr/X11R6/lib -lpthread -lX11 -lXext -Dcimg_use_xshm -lm

# Rule for building the executable
$(EXEC): $(OBJETS)
	$(CCPP) -o $(EXEC) $(OBJETS) $(LFLAGS)

# Rule for compiling .cpp files into .o object files
.cpp.o:
	$(CCPP) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files
clean:
	rm -f $(OBJETS)

# Clear rule to remove the executable
clear:
	rm -f $(EXEC)

# Rule to generate dependencies automatically
depend:
	$(CCPP) -MM $(SOURCES) > dependances.d

# Include generated dependencies (if they exist)
-include dependances.d

# Dependencies section
# If no dependencies are generated yet, this will be empty until "make depend" is run
