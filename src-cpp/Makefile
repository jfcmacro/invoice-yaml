
all: invoiceYaml

CXXFLAGS=-g
LDFLAGS=-lyaml-cpp

INVOICE_OBJECTS=invoiceYaml.o invoice.o

invoiceYaml: $(INVOICE_OBJECTS)
	$(CXX) -o $@ $(INVOICE_OBJECTS) $(LDFLAGS)

invoiceYaml.o: invoiceYaml.C
	$(CXX) $(CXXFLAGS) -c $<

invoice.o: invoice.C invoice.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *.C~ Makefile~
