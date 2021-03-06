#include "Platoon.h"

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

            // adds car to the platton, maintaining ordering.
            this->insert(car);
		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

void Platoon::prepend(Car* c)
{
  // if the platton is empty
  if (this->head == NULL) {
    this->head = c;
    this->tail = c;
    c->set_next(NULL);
    c->set_prev(NULL);
    return;
  }
  // else
  this->head->set_prev(c);
  c->set_next(this->head);
  c->set_prev(NULL);
  this->head = c;
  return;
}

void Platoon::append(Car* c)
{
  // if the platton is empty
  if (this->head == NULL) {
    this->head = c;
    this->tail = c;
    c->set_next(NULL);
    c->set_prev(NULL);
    return;
  }
  // else
  this->tail->set_next(c);
  c->set_prev(this->tail);
  c->set_next(NULL);
  this->tail = c;
  return;
}

void Platoon::remove(Car* c)
//the removed cars pointers will be sorted when its added to another platton
{
  // if theres only one car
  if (this->head == this->tail) {
    this->head = NULL;
    this->tail = NULL;
    return;
  }
  // if its the head
  if (this->head == c) {
    this->head = head->get_next();
    this->head->set_prev(NULL);
    return;
  }
  // if its the tail
  if (this->tail == c) {
    this->tail = tail->get_prev();
    this->tail->set_next(NULL);
    return;
  }
  // else, loop through remaining cars (!head, !tail)
  Car* current = this->head->get_next();
  while(current->get_next() != NULL) {
    if (current == c){
      current->get_prev()->set_next(current->get_next());
      current->get_next()->set_prev(current->get_prev());
      return;
    }
    current = current->get_next();
  }
}

// inserts a car in platoon based on its position. Assumes there are no conflicts
void Platoon::insert(Car *c)
{
  // if empty or behind the head
  if ((this->head == NULL) || (this->head->get_position() > c->get_position())) {
    prepend(c);
    return;
  }
  Car* current = this->head->get_next();
  while (current != NULL) {
    // Insert behind 'car'
    if (current->get_position() > c->get_position()) {
      c->set_next(current);
      c->set_prev(current->get_prev());
      c->get_prev()->set_next(c);
      current->set_prev(c);
      return;
    }
    current = current->get_next();
  }
  //looped through all so must be the lead car (tail)
  append(c);
  return;
}

// Checks if any car is occupying a specified position
const bool Platoon::pos_is_empty(int pos)
{
  Car* current = this->head;
  while (current != NULL) {
    if (current->get_position() == pos) {
      return false;
    }
    current = current->get_next();
  }
  return true;
}