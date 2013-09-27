#include "invoice.h"

item::item(int id, float valUnit, int units)
  : id(id), valueUnits(valUnit), units(units)
{ }

item::~item() { }

int item::getId() const {
  return id;
}

float item::getValUnit() const {
  return valueUnits;
}

int item::getUnits() const {
  return units;
}

float item::getCostItem() const {
  return (float) units * valueUnits;
}

invoice::invoice(int id, char* name) 
  : id(id), name(*name)
{
}

invoice::~invoice() { }

void invoice::addItem(item i) {
  items.push_back(i);
}
