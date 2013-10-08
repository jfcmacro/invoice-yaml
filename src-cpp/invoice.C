#include "invoice.h"

item::item() : id(0), valueUnits(0.0f), units(0) { }

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

void item::setId(string& id) {
  istringstream in(id);
  int val;
  in >> val;
  this->id = val;
}

void item::setValUnit(string& id) {
  istringstream in(id);
  float val;
  in >> val;
  this->valueUnits = val;
}

void item::setUnits(string& id) {
  istringstream in(id);
  int val;
  in >> val;
  this->units = val;
}

invoice::invoice() 
  : id(0), items() { }

invoice::~invoice() {
}

void invoice::addItem(item i) {
  items.push_back(i);
}

string&
invoice::getName() const {
  return *(new string(this->name));
}

int
invoice::getId() const {
  return this->id;
}

i_items invoice::getItems() {
  return items.begin();
}

i_items invoice::getLastItem() {
  return items.end();
}

void invoice::setName(string& name) {
  this->name = name;
}

void invoice::setId(string& id) {
  istringstream in(id);
  int val;
  in >> val;
  this->id = val;
}
