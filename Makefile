CC=g++
CPPFLAGS= -std=c++17 -stdlib=libc++ -g
SRCDIR := src
BUILDDIR := build
TESTDIR := test
TARGET := tests

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

TESTSOURCES := $(shell find $(TESTDIR)/$(SRCDIR) -type f -name "*.$(SRCEXT)")
TESTNAMES := $(patsubst $(TESTDIR)/$(SRCDIR)/%,$(TESTDIR)/%,$(TESTSOURCES:.$(SRCEXT)=))
TESTOBJECTS := $(patsubst $(TESTDIR)/$(SRCDIR)/%,$(TESTDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))

INC := -I ../include


# $(TARGET) : $(TARGET).o $(OBJECTS)
# 	$(CC) $(CPPFLAGS) $(TARGET).o $(OBJECTS) -o bin/$@
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CPPFLAGS) -c $(INC) $< -o $@
$(TARGET).o : $(TARGET).cpp
	$(CC) $(CPPFLAGS) -c  $(INC) $< -o $@ 
	
#=== test recipes
tests : $(TESTNAMES)
	@echo "Building tests $(TESTSOURCES) $(TESTNAMES) $(TESTOBJECTS)"
$(TESTNAMES) : $(TESTOBJECTS) $(OBJECTS)
	$(CC) $(CPPFLAGS) -Wall $^ -o $@	
$(TESTDIR)/%.o: $(TESTDIR)/$(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CPPFLAGS) -Wall -c $(INC) $(TESTDIR) $< -o $@

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -f $(TARGET).o $(BUILDDIR)/*.o bin/$(TARGET)"; $(RM) -f $(TARGET).o $(BUILDDIR)/*.o bin/$(TARGET) && $(RM) -f $(OBJECTS)
cleantest:
	$(RM) -f $(TESTOBJECTS) $(TESTNAMES)
