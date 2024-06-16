# Compiler & Linker settings
CXX = g++
CXXFLAGS = -I ./inc -I ./third-party/CImg -I ./third-party/libjpeg -I ./Data-Loader -std=c++11
OPTFLAGS = -march=native -flto -funroll-loops -finline-functions -ffast-math -O3
WARNINGS = -g -Wall
LINKER = -L/usr/X11R6/lib -lm -lpthread -lX11 -L./third-party/libjpeg -ljpeg -lpng

# Valgrind for memory issue
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(SRCDIR)/photo_mosaic.cpp $(SRCDIR)/bit_field_filter.cpp
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$$(OBJDIR)/%.d,$(SRCS))

# Control the build verbosity
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

.PHONY: all install check clean cppcheck

# Name of the executables
TARGETS = Data_Loader Image Bit_Field Photo_Mosaic

all: $(TARGETS)

$(OBJDIR):
	@mkdir $(OBJDIR)

Data_Loader: Data_loader_demo.cpp $(OBJDIR)/data_loader.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

Image: Image_demo.cpp $(OBJS) $(OBJDIR)/data_loader.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

Bit_Field: Bit_field_demo.cpp $(OBJS) $(OBJDIR)/data_loader.o $(OBJDIR)/bit_field_filter.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

Photo_Mosaic: Photo_mosaic_demo.cpp $(OBJS) $(OBJDIR)/data_loader.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

# Include generated dependency files
-include $(DEPS)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(VECHO) "	CC\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -MMD -c $< -o $@

# Compilation rule for data_loader.o with explicit dependencies
$(OBJDIR)/data_loader.o: ./Data-Loader/data_loader.cpp ./Data-Loader/data_loader.h | $(OBJDIR) Makefile
	$(VECHO) "	CC\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -MMD -c $< -o $@

install:
	$(VECHO) "Installing third party dependencies\n"
	$(Q)chmod +x scripts/clone_env.sh  
	$(Q)./scripts/clone_env.sh  > /dev/null 2>&1
	$(VECHO) "Finished installing third party dependencies!!\n"

check: $(TARGETS)
	@echo "Running Valgrind on all executables..."
	$(foreach target, $(TARGETS), $(CHECKCC) $(CHECKFLAGS) ./$(target);)

clean:
	rm -rf $(OBJDIR) $(TARGETS)

# Cppcheck rule to check all source files excluding third-party directories
cppcheck: $(TARGETS)
	@echo "Running Cppcheck..."
	@which cppcheck || { echo "Cppcheck not found in PATH"; exit 1; }
	@cppcheck --version || { echo "Failed to run Cppcheck"; exit 1; }
	@for file in $(SRCDIR)/*.cpp; do \
		echo "Checking $$file..."; \
		cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem \
		--suppress=*:third-party/* -I ./inc -I ./Data-Loader $$file 2>> cppcheck_src_report.txt || { echo "Cppcheck failed on $$file"; exit 1; }; \
	done
	@echo "Cppcheck finished! Report saved to cppcheck_src_report.txt"
