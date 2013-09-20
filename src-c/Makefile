
all: invoiceYaml

INVOICECFLAGS=`pkg-config --cflags glib-2.0`
INVOICELIBS=`pkg-config --libs glib-2.0` -lyaml

invoiceYaml: invoiceYaml.o
	$(CC)  -o $@ $< $(INVOICELIBS)

invoiceYaml.o: invoiceYaml.c
	echo "Hello"
	$(CC) $(INVOICECFLAGS) -c $< 

clean:
	rm -f invoiceYaml.o invoiceYaml

