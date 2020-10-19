#include "TrafficModel.h"
#include <iostream>

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}


/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
  // creates ordered list of cars to move (right to left, top to bottom)
  // the linked lists will be be shuffled during update so will loose thier order
	vector<vector<Car*>> order;
  for (unsigned int i = 0 ; i < this->platoons.size(); ++i) {
    //adds a new empty vector to represent lane
    order.push_back({});
    Car* car = this->platoons[i].get_tail();
    while (car != NULL) {
      order[i].push_back(car);
      car = car->get_prev();
    }
  }

  for (unsigned int i = 0; i < order.size(); ++i) {
    for (unsigned int j = 0; j < order[i].size(); ++j) {
      Car* c = order[i][j];
      int light = get_lane_change_command(c->get_id());

      if (light == 0){
        //move forwards if next position is empty
        if (this->platoons[i].pos_is_empty(c->get_position() + 1)) {
          c->set_position(c->get_position() + 1);
        }
        //moves on to next car
        continue;
      }
      //LANE CHANGE:
      // convertes light signal to a lane
      light -= (1 + light%2);
      // if the lane we want to move to doesnt exist, move forward
      int si = (int)i;
      if ((si+light < 0) || (si+light == int(this->platoons.size()))) {
        if (this->platoons[i].pos_is_empty(c->get_position() + 1)) {
          c->set_position(c->get_position() + 1);
        }
        continue;
      }
      // If we can change lanes then do so otherwise move forwards
      if (this->platoons[i+light].pos_is_empty(c->get_position())){
        platoons[i].remove(c);
        platoons[i+light].insert(c);
        continue;
      }
      if (this->platoons[i].pos_is_empty(c->get_position() + 1)) {
        c->set_position(c->get_position() + 1);
      }
    }
  }
}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

//
// IMPORTANT: DO NOT CHANGE THE FUNCTIONS BELOW THIS LINE
//

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}

//Get the state of cars as a 3D vector representing car data in lane/pos
vector<vector<vector<int>>> TrafficModel::get_car_state(){

	vector<string> state = get_system_state();
	vector<vector<vector<int>>> cars;
	string remainingCars;
	string newCar;

	//Parse state into vector of car data
	for (unsigned int i = 0; i < state.size(); i++){
		vector<vector<int>> carRow;
		remainingCars = state[i];
		remainingCars.push_back(';');

		//Parse string of entire lane into individual car data
		while (remainingCars.size() > 1) {
			remainingCars = remainingCars.substr(1);
			size_t pos = remainingCars.find(";");
			newCar = remainingCars.substr(1,pos-2);

			carRow.push_back(parse_car_data(newCar));

			if(pos!=string::npos){
				remainingCars = remainingCars.substr(pos);
			} else {
				remainingCars = "";
			}
		}
		cars.push_back(carRow);
	}
	return cars;
}

//Parse string in form (id,lane,pos,turn) into vector of ints
vector<int> TrafficModel::parse_car_data (string& car){
	vector<int> carData;
	string delimiter = ",";
	size_t last = 0;
	size_t next = 0;
	int index = 0;

	while ((next = car.find(delimiter, last)) != string::npos) {
		carData.push_back(stoi(car.substr(last, next-last)));
		last = next + 1;
		index++;
	}
	carData.push_back(stoi(car.substr(last)));
	return carData;
}
