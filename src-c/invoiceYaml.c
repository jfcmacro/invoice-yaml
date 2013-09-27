/*
 * Author: Juan Francisco Cardona Mc'Cormick
 * Date:   20/09/2013
 *
 * Purpose: To show how to write an parser for YAML file
 *
 * History of Modifications:
 * 27/09/2013 - Correcting some errors and adding a procesing
 *              for each invoice.
 */

#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

struct item {
  int id;
  float valueUnit;
  int units;
};

typedef struct item item_t;
typedef struct item* pitem_t;

struct invoice {
  int id;
  char* name;
  GSList* items;
};

typedef struct invoice invoice_t;
typedef struct invoice* pinvoice_t;

static void
usage(char *progname) {
  fprintf(stderr, "Usage: %s <file>\n", progname);
  exit(1);
}

// I guess this is too pedantic, but to drop all warnings requerided that
// level.
enum state { start = 0, one, two, three, four, five, six, seven, eight,
	     nine, ten, eleven, twelve,
	     end = 13, error = 14 };

typedef enum state state_t;

float valueItems(pitem_t item) {
  return (float) item->units * item->valueUnit;
}

void showItem(pitem_t item) {
  fprintf(stdout, "%5d\t%8.2f\t%5d\t%8.2f\n",
	  item->id, item->valueUnit, item->units,
	  valueItems(item));
}

void showInvoice(pinvoice_t invoice) {
  GList* node = NULL;
  fprintf(stdout, "ID: %d\nNAME: %s\n", invoice->id, invoice->name);
  float total = 0.0f;
  for (node = invoice->items; node; node = node->next) {
    total += valueItems((pitem_t) node->data);
    showItem((pitem_t) node->data);
  }
  fprintf(stdout, "\t\t\t\t%8.2f\n", total);
}

enum state showErrorState(enum state curr_state, 
			  yaml_event_type_t event_type) {
  fprintf(stderr, "Error at state %d found event: %d\n", 
	  curr_state, event_type);
  return error;
}

GSList* parsingInvoicesFile(const char* filename) {
  yaml_parser_t parser;
  yaml_event_t event;
  FILE *infile;
  int cont = TRUE;
  pitem_t pitem = NULL;
  pinvoice_t pinvoice = NULL;
  GSList* invoices = NULL;
  state_t state = start;

  infile = fopen(filename, "r");

  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser, infile);

  do {
    yaml_parser_parse(&parser, &event);
    switch (state) {
    case start:
      switch(event.type) {
      case YAML_STREAM_START_EVENT:
	state = one;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case one:
      switch (event.type) {
      case YAML_STREAM_END_EVENT:
	state = end;
	break;
      case YAML_DOCUMENT_START_EVENT:
	state = two;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case two:
      switch (event.type) {
      case YAML_SEQUENCE_START_EVENT:
	state = three;
	break;

      case YAML_DOCUMENT_END_EVENT:
	state = one;
	break;

      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case three:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT:
	state = four;
	pinvoice = (pinvoice_t) malloc(sizeof(invoice_t));
	invoices = g_slist_append(invoices, pinvoice);
	break;
      case YAML_SEQUENCE_END_EVENT:
	state = two;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case four:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	if (strcmp((char*)event.data.scalar.value,
		   "invoice") == 0) {
	  state = five;
	}
	if (strcmp((char*)event.data.scalar.value, "client") == 0) {
	  state = six;
	}
	if (strcmp((char *)event.data.scalar.value, "items") == 0) {
	  pinvoice->items = NULL;
	  state = seven;
	}
	break;

      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case five:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pinvoice->id = atoi((char*)event.data.scalar.value);
	state = four;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case six:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pinvoice->name = (char *) malloc(sizeof(char) *
					 (event.data.scalar.length + 1));
	strcpy(pinvoice->name, (char*) event.data.scalar.value);
	pinvoice->name[event.data.scalar.length] = '\0';
	state = four;
	break;

      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case seven:
      switch(event.type) {
      case YAML_SEQUENCE_START_EVENT:
	state = eight;
	break;
      case YAML_MAPPING_END_EVENT:
	state = three;
	break;

      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case eight:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT:
	pitem = (pitem_t) malloc(sizeof(item_t));
	pinvoice->items = g_slist_append(pinvoice->items, pitem);
	state = nine;
	break;
      case YAML_SEQUENCE_END_EVENT:
	state = seven;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case nine:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	if (strcmp((char *) event.data.scalar.value, "item") == 0) {
	  state = ten;
	}
	if (strcmp((char *) event.data.scalar.value, "unitvalue") == 0) {
	  state = eleven;
	}
	if (strcmp((char *) event.data.scalar.value, "units") == 0) {
	  state = twelve;
	}
	break;
      case YAML_MAPPING_END_EVENT:
	state = eight;
	break;

      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case ten:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->id = atoi((char*)event.data.scalar.value);
	state = nine;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case eleven:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->valueUnit = atof((char *) event.data.scalar.value);
	state = nine;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case twelve:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->units = atoi((char*)event.data.scalar.value);
	state = nine;
	break;
      default:
	state = showErrorState(state, event.type);
	break;
      }
      break;

    case end:
      cont = FALSE;
      break;

    case error:
      cont = FALSE;
      return NULL;
      break;
    }
    yaml_event_delete(&event);
  } while (cont);

  yaml_parser_delete(&parser);
  return invoices;
}

int
main(int argc, char *argv[]) {
  GSList *invoices, *node;

  if (argc != 2) {
    usage(argv[0]);
  }

  invoices = parsingInvoicesFile(argv[1]);

  for (node = invoices; node; node = node->next) {
    showInvoice((pinvoice_t) node->data);
  }
  return 0;
}
