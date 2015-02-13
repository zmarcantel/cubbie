PROG=cubbie
OUTPATH=bin

MAIN_FILES=$(wildcard *.cc)

CFLAGS=-O2

BOOST=$(shell brew --prefix boost)
BOOST_LIBS=program_options,system,thread-mt

LIBDIR=$(BOOST)
INCLUDE=$(BOOST)

LIBS=$(shell echo boost_$(BOOST_LIBS) | sed "s/,/ -lboost_/g")


default: dirs
	g++ $(CFLAGS) -o $(OUTPATH)/$(PROG) -I$(INCLUDE) -L$(LIBDIR) -l$(LIBS) $(MAIN_FILES)

dirs:
	mkdir -p $(OUTPATH)

clean:
	rm -rf $(OUTPATH)

run: default
	@echo "Done building..."
	@$(OUTPATH)/$(PROG)

help: default
	@echo "Dumping help of compiled program..."
	@echo "-----------------------------------\n\n"
	@$(OUTPATH)/$(PROG) --help
	
