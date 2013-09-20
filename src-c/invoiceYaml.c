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

int
main(int argc, char *argv[]) {
  yaml_parser_t parser;
  yaml_event_t event;
  FILE *infile;
  int cont = TRUE;
  pitem_t pitem = NULL;
  pinvoice_t pinvoice = NULL;
  invoices_t invoces = NULL;

  if (argc != 2) {
    usage(argv[0]);
  }

  infile = fopen(argv[1], "r");
  
  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser, infile);

  do {
    yaml_parser_parse(&parser, &event);
    switch (event.type) {
    case YAML_STREAM_START_EVENT:
      fprintf(stdout, "STREAM-START\n");
      break;
    case YAML_STREAM_END_EVENT:
      fprintf(stdout, "STREAM-END\n");
      cont = FALSE;
      break;
    case YAML_DOCUMENT_START_EVENT:
      fprintf(stdout, "DOCUMENT-START\n");
      break;
    case YAML_DOCUMENT_END_EVENT:
      fprintf(stdout, "DOCUMENT-END\n");
      break;
    case YAML_ALIAS_EVENT:
      fprintf(stdout, "ALIAS\n");
      break;
    case YAML_SCALAR_EVENT:
      fprintf(stdout, "SCALAR\n");
      fprintf(stdout, "anchor: %s tag: %s value: %s\n",
	      event.data.scalar.anchor,
	      event.data.scalar.tag,
	      event.data.scalar.value);
      break;
    case YAML_SEQUENCE_START_EVENT:
      fprintf(stdout, "SEQUENCE-START\n");
      fprintf(stdout, "anchor: %s tag: %s\n",
	      event.data.sequence_start.anchor,
	      event.data.sequence_start.tag);
      break;
    case YAML_SEQUENCE_END_EVENT:
      fprintf(stdout, "SEQUENCE-END\n");
      break;
    case YAML_MAPPING_START_EVENT:
      fprintf(stdout, "MAPPING-START\n");
      fprintf(stdout, "anchor: %s tag: %s\n",
	      event.data.mapping_start.anchor,
	      event.data.mapping_start.tag);
      break;
    case YAML_MAPPING_END_EVENT:
      fprintf(stdout, "MAPPING-END\n");
      break;
    default:
      fprintf(stdout, "Unknown event: %d\n", event.type);
      break;
    }
    
    yaml_event_delete(&event);
  } while (cont);

  yaml_parser_delete(&parser);
  return 0;
}
