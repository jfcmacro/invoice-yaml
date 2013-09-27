
all: invoiceYaml

INVOICE_OBJECTS=invoiceYaml.o invoice.o

invoiceYaml: $(INVOICE_OBJECTS)
	$(CXX) -o $@ $(INVOICE_OBJECTS)

invoiceYaml.o: invoiceYaml.C
	$(CXX) -c $<

invoice.o: invoice.C invoice.h
	$(CXX) -c $<

