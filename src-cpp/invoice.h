#ifndef INVOICE_H 
#define INVOICE_H
#include <string>
#include <vector>

using namespace std;


class item {

  item(int id, float valUnit, int units);
  ~item();

  int getId() const;
  float getValUnit() const;
  int getUnits() const;
  float getCostItem() const;
private:
  int id;
  float valueUnits;
  int units;
};

class invoice {

  invoice(int id, char *name);
  ~invoice();
  void addItem(item);

private:
  int id;
  string* name;
  vector<item> items;
};



#endif
