#include <iostream>
#include <iomanip>
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

  invoices* ivcs = new invoices();

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);
  for (unsigned i = 0; i < doc.size(); i++) {
    invoice ivc;
    doc[i] >> ivc;
    ivcs->push_back(ivc);
  }

  return *ivcs;
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
    cout << "ID: " << (*it).getId() << endl;
    cout << "NAME: " << (*it).getName() << endl;
    for(i_items itms = (*it).getItems();
	itms != (*it).getLastItem();
	++itms) {
      cout << setw(5) << setfill(' ') << itms->getId()
	   << '\t' << fixed << setw(6) << itms->getCostItem()
	   << '\t' << setw(5) << setfill(' ') << itms->getUnits()
	   << '\t' << fixed << setw(8) << setfill(' ') << 
	(itms->getCostItem() * itms->getUnits()) << endl;
    }
  }
  return 0;
}
