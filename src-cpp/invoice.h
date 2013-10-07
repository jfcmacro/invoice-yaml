#ifndef INVOICE_H 
#define INVOICE_H
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

class item {
 public:
  item(int id, float valUnit, int units);
  item();
  ~item();

  int getId() const;
  float getValUnit() const;
  int getUnits() const;
  float getCostItem() const;
  void setId(string& id);
  void setValUnit(string& id);
  void setUnits(string& id);
 private:
  int id;
  float valueUnits;
  int units;
};

typedef vector< item > items_t;
typedef vector< item >::iterator i_items;

class invoice {
 public:
  invoice();
  invoice(int id, char *name);
  ~invoice();
  void addItem(item);
  i_items getItems();
  void setName(string& name);
  void setId(string& id);
 private:
  int id;
  string* name;
  items_t items;
};

typedef vector< invoice > invoices;
typedef vector< invoice >::iterator i_invoices;

#endif
