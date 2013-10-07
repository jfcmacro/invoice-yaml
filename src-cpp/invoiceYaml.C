#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <yaml-cpp/yaml.h>
#include "invoice.h"

using namespace std;

void operator >> (const YAML::Node& node, item& item) {
  string id;
  string  valUnit;
  string  units;
  node["item"] >> id;
  node["unitvalue"] >> valUnit;
  node["units"] >> units;

  item.setId(id);
  item.setValUnit(valUnit);
  item.setUnits(units);
}

void operator >> (const YAML::Node& node, invoice &invoice) {
  string id;
  node["invoice"] >> id;
  string name;
  node["client"] >> name;
  invoice.setId(id);
  invoice.setName(name);
  const YAML::Node& items = node["items"];
  
  for (unsigned i = 0; i < items.size(); i++) {
    item it; 
    items[i] >> it;
    invoice.addItem(it);
  }
}

invoices&
parserInvoicesFile(char *filename) {
  ifstream fin(filename);

  invoices* invo = new invoices();

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);
  for (unsigned i = 0; i < doc.size(); i++) {
    invoice invoice;
    doc[i] >> invoice;
    invo->push_back(invoice);
  }

  return *invo;
}

void
usage(const char* name) {
  cerr << "Usage: " << name << " <yamlfile>.yaml" << endl;
  exit(0);
}

int
main(int argc, char *argv[]) {

  if (argc != 2) {
    usage(argv[0]);
  }
  invoices& inv = parserInvoicesFile(argv[1]);
  
  for(i_invoices it = inv.begin();
      it != inv.end();
      ++it) {
    cout << (*it).getId() << endl;
    cout << (*it).getName() << endl;
  }
  return 0;
}
