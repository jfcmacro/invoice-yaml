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
typedef struct GSList *items_t;

struct invoice {
  int id;
  char* name;
  items_t *items;
};

typedef struct invoice invoice_t;
typedef struct invoice* pinvoice_t;
typedef GSList* invoices_t;

static void
usage(char *progname) {
  fprintf(stderr, "Usage: %s <file>\n", progname);
  exit(1);
}

enum state { start = 0, end = 13, error = 14 };

typedef enum state state_t;

int
main(int argc, char *argv[]) {
  yaml_parser_t parser;
  yaml_event_t event;
  FILE *infile;
  int cont = TRUE;
  pitem_t pitem = NULL;
  pinvoice_t pinvoice = NULL;
  invoices_t invoices = NULL;
  state_t state = start;
  int retvalue = 0;

  if (argc != 2) {
    usage(argv[0]);
  }

  infile = fopen(argv[1], "r");
  
  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser, infile);

  do {
    yaml_parser_parse(&parser, &event);
    switch (state) {
    case start:
      switch(event.type) {
      case YAML_STREAM_START_EVENT:
	state = 1;
	break;
      default:
	state = error;
	break;
      }
      break;

    case 1:
      switch (event.type) {
      case YAML_STREAM_END_EVENT:
	state = end;
	break;
      case YAML_DOCUMENT_START_EVENT:
	state = 2;
	break;
      default:
	state = error;
      }
      break;

    case 2: 
      switch (event.type) {
      case YAML_SEQUENCE_START_EVENT:
	state = 3;
	break;
      case YAML_DOCUMENT_END_EVENT:
	state = 1;
	break;

      default:
	state = error;
	break;
      }
      break;

    case 3:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT:
	state = 4;
	pinvoice = (pinvoice_t) malloc(sizeof(pinvoice_t));
	invoices = g_slist_append(invoices, pinvoice);
	break;
      case YAML_SEQUENCE_END_EVENT:
	state = 2;
	break;
      }
      break;

    case 4:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	if (strcmp(event.data.scalar.value, "invoice") == 0) {
	  state = 5;
	}
	if (strcmp(event.data.scalar.value, "client") == 0) {
	  state = 6;
	}
	if (strcmp(event.data.scalar.value, "items") == 0) {
	  pinvoice->items = NULL;
	  state = 7;
	}
      }
      break;

    case 5:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pinvoice->id = atoi(event.data.scalar.value);
	state = 4;
	break;
      default:
	state = error;
      }
      break;

    case 6:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pinvoice->name = (char *) malloc(sizeof(char) * 
					 (event.data.scalar.length + 1));
	strcpy(pinvoice->name, event.data.scalar.value);
	pinvoice->name[event.data.scalar.length] = '\0';
	state = 4;
	break;

      default:
	state = error;
      }
      break;

    case 7:
      switch(event.type) {
      case YAML_SEQUENCE_START_EVENT:
	state = 8;
	break;
      case YAML_MAPPING_END_EVENT:
	state = 6;
      default:
	state = error;
	break;
      }
      break;

    case 8:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT:
	pitem = (pitem_t) malloc(sizeof(item_t));
	pinvoice->items = g_slist_append(pinvoice->items, pitem);
	state = 9;
	break;
      case YAML_SEQUENCE_END_EVENT:
	state = 7;
	break;
      default:
	state = error;
	break;
      }
      break;

    case 9:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	if (strcmp(event.data.scalar.value, "item") == 0) {
	  state = 10;
	}
	if (strcmp(event.data.scalar.value, "unitvalue") == 0) {
	  state = 11;
	}
	if (strcmp(event.data.scalar.value, "units") == 0) {
	  state = 12;
	}
	break;
      default:
	state = error;
	break;
      }
      break;

    case 10:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->id = atoi(event.data.scalar.value);
	state = 9;
	break;
      default:
	state = error;
      }
      break;
    
    case 11:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->valueUnit = atof(event.data.scalar.value);
	state = 9;
	break;
      default:
	state = error;
      }
      break;
    
    case 12:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
	pitem->units = atoi(event.data.scalar.value);
	state = 9;
	break;
      default:
	state = error;
      }
      break;

    case end:
      cont = FALSE;
      break;

    case error:
      cont = FALSE;
      retvalue = 1;
      break;
    }
    yaml_event_delete(&event);
  } while (cont);

  yaml_parser_delete(&parser);
  return retvalue;
}
