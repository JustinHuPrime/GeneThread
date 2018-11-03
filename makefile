# Copyright 2018 Justin Hu
#
# This file is part of GeneThread.
#
# GeneThread is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version. 
#
# GeneThread is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# GeneThread.  If not, see <https://www.gnu.org/licenses/>.

#command options
CC := g++
RM := rm -rf
MKDIR := mkdir -p


#File options
SRCDIR := src
SRCS := $(shell find -L -O3 $(SRCDIR)/ -type f -name '*.cc')

OBJDIR := bin
OBJS := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))

DEPDIR := dependencies
DEPS := $(patsubst $(SRCDIR)/%.cc,$(DEPDIR)/%.dep,$(SRCS))


#compiler configuration
WARNINGS := -Wall -Wextra -Wpedantic -Wpedantic-errors -Wmissing-include-dirs\
-Wswitch-default -Wuninitialized -Wstrict-overflow=5 -Wsuggest-override\
-Wfloat-equal -Wshadow -Wundef -Wunused-macros -Wcast-qual -Wcast-align=strict\
-Wconversion -Wzero-as-null-pointer-constant -Wformat=2 -Wuseless-cast\
-Wextra-semi -Wsign-conversion -Wlogical-op -Wmissing-declarations\
-Wredundant-decls -Winline -Winvalid-pch -Wdisabled-optimization\
-Wctor-dtor-privacy -Wnoexcept -Wnon-virtual-dtor -Wstrict-null-sentinel\
-Wold-style-cast -Woverloaded-virtual -Wsign-promo

#build-specific compiler options
DEBUGOPTIONS := -Og -ggdb -Wno-unused
RELEASEOPTIONS := -Os -D NDEBUG -Wunused

#libraries and included files
LIBS :=
INCLUDES := -I$(SRCDIR)

#final executable name
EXENAME := geneThread


.PHONY: debug release clean diagnose
.SECONDEXPANSION:


debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME)
	@echo "Done compiling debug."
	@echo ""
	@echo "Debug build finished."

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME)
	@echo "Done with release."
	@echo ""
	@echo "Release build finished."


clean:
	@echo "Removing $(DEPDIR)/, $(OBJDIR)/, and $(EXENAME)"
	@$(RM) $(OBJDIR) $(DEPDIR) $(EXENAME)


$(EXENAME): $(OBJS)
	@echo "Linking..."
	@$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,$(SRCDIR)/%.cc,$$@) $$(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@..."
	@clang-format -i $(filter-out %.dep,$^)
	@$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,$(SRCDIR)/%.cc,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(INCLUDES) -MM -MT $(patsubst $(DEPDIR)/%.dep,$(OBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


%/:
	@$(MKDIR) $@


-include $(DEPS)