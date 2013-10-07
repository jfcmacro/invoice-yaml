
all: invoiceYaml

LDFLAGS=-lyaml-cpp

INVOICE_OBJECTS=invoiceYaml.o invoice.o

invoiceYaml: $(INVOICE_OBJECTS)
	$(CXX) -o $@ $(INVOICE_OBJECTS) $(LDFLAGS)

invoiceYaml.o: invoiceYaml.C
	$(CXX) -c $<

invoice.o: invoice.C invoice.h
	$(CXX) -c $<

