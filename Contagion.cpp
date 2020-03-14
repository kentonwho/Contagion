#include <iostream> 
#include <cmath> 
#include <random> 
#include <ctime> 
#include <vector> 
#include <algorithm> 

using namespace::std ; 
 
class Person {
	private: 
		string status; 
		int daysLeft = -1;
	public:  
		Person() { status = "susceptible"; } ;
		Person(string initial) { status = initial;  } ; 
		Person(int toGo) { status = "sick";daysLeft = toGo; } ;  
		
		string status_string() { return status ;  };
		void setStatus(string stat) { status = stat;  }; 
  
		int daysToGo() { if(status == "sick")  
					return daysLeft + 1 ;
				 else
					cout << "Error: Person is not sick" << endl; 
				 } ; 

		void update() {
			if (daysLeft == 0) {
				status = "recovered"; 
				daysLeft = NULL; }
			else if (status == "sick" ) { 
				daysLeft = daysLeft - 1; } 
			
		 	}  
		
		void infect(int n) { if (status == "susceptible") {daysLeft = n; status = "infected";} };
		void forceSick(int shed) { status = "sick"; daysLeft = shed; } ; 
		void sicken() { if (status == "infected") { status = "sick"; }};  

 		bool  bless() { if (status == "vaccinated") {
					return false;} 
				else {
					status = "vaccinated"; 
					return true;} } ;

		bool is_stable() {
			if (status == "recovered" || status == "vaccinated")  
				return true; 
			else 
				return false; 
		 } ;
}; 

class Population { 
	private: 
		int numOfInfected;  
		int connections; 
		float probability; 
		int total;	 
		vector<Person> population;
		int shedPeriod; 
	public: 
		Population() {	} ; 
		Population(int numOfPeople) { 
			for (int i = 0; i < numOfPeople; i++) { 
				population.push_back(Person()); }     
		};
			
		Population(int numOfPeople, int cons, float prob) { 
			for (int i = 0; i < numOfPeople; i++) { 
				population.push_back(Person()); } 
			connections = cons; 
			probability = prob; 
		};

		Population(int numOfPeople, int cons, float prob, int shedding) { 
			for (int i = 0; i < numOfPeople; i++) { 
				population.push_back(Person()); } 
			connections = cons; 
			probability = prob; 
			shedPeriod = shedding; 
		};

		void spreadDisease() { for (int i = 0; i < population.size(); i++) {
						if  ((population.at(i)).status_string() == "sick") { 
							for (int j = -(connections/2); j<((connections-connections/2)+1); j++) { 
									float bad_luck = (float) rand()/(float)RAND_MAX; 
									if (bad_luck>(1.0-probability)){
										int hold = population.size(); 
										if ((j+i) >= 0)  {
											(population.at((j+i)%hold)).infect(shedPeriod); }
										else {
											(population.at(hold+((j+i)%hold))).infect(shedPeriod);
										} 
									}
							}		
						 } 
					}
					for (auto &i : population) {
						if (i.status_string() == "infected") { 
							i.setStatus("sick"); }
					}
					updateNumOfInfected(); 
					}; 
 
		void update() { for (auto &i : population) { i.update(); }};  

		void sicken(int i) {	(population.at(i)).infect(5); (population.at(i)).sicken(); };
  
		void vaccinate(float percent) { int size = (int) population.size()*percent; 
						int index = 0;    
						while (index < size) {
							//cout << index << endl; 
							int good_luck = (int) (float) (population.size())*(rand()/(float)RAND_MAX); 
								if ((population.at(good_luck)).bless()) 
									index ++;
							}
						}; 
						
		void setConnections(int i) {connections = i; }; 
		

		bool isCured() { if (numOfInfected == 0) { 
					return true;} 
				 else {
					return false;} };  
		bool herdImmune() { for (auto i: population) { 
					if (i.status_string() == "susceptible") { 
						return true; }	
					
					else{
						return false;} 
				}	}; 
	
		void updateNumOfInfected() {  numOfInfected = 0; 
						for (auto i : population) { 
							if (i.status_string() == "sick") { 
								numOfInfected ++ ; }
						}
					}; 

		void updateTotal() { total = 0; 				
					for (auto i : population) { 
						if (i.status_string() == "sick" || i.status_string() == "recovered") { 
							total++ ; 
				}
				} }; 
		int percVacc() { int tot =0; 
					for (auto i: population) { 
						if (i.status_string() == "vaccinated") { 
							tot ++ ;	}} return tot;}; 
		int getTotal() {  return total;}; 
	
	 	void forceSick(int n,int shed) { (population.at(n)).forceSick(shed); } ; 
	

		int getNumOfInfected() { return numOfInfected; } ; 
		int getSize() { return population.size(); } ;
		vector<Person> getPop() { return population; } ;  

};

int main() {
	srand(time(NULL)); 
	int size; 
	int connections; 
	float probability;
	float percent;  
	int shedding; 
	cout << "Enter the size of the population: " ; 
	cin >> size	; 
	cout << "Enter the number of contacts per person: " ; 
	cin >> connections;
	cout << "Enter the probability of transmission(0-1.0): ";  
	cin >> probability;
	cout << "enter the percent of people vaccinated: "; 
	cin >> percent ; 
	cout << "Enter the shedding period of the virus: "; 
	cin >> shedding ; 
	Population Austin = Population(size,connections,probability,shedding);
	Austin.vaccinate(percent);  
	Austin.forceSick(size/2,shedding); 
	Austin.updateNumOfInfected(); 
	int day = 0;
	//	for (int i = 0; i < Austin.getSize(); i++) { 
	//		cout << Austin.getPop().at(i).status_string() << " " ; 
	//}	
	while(! Austin.isCured()) {
		cout << "On day " << day << " Austin has " << Austin.getNumOfInfected() << " sick people " << endl;
		Austin.updateTotal();   
		cout << "Saturation: " << (Austin.getTotal()/(float)size)*100 << " percent" << endl; 
		cout << endl;  
		Austin.spreadDisease(); //cout << "Disease spread" << endl; 
		Austin.update(); 	//cout << "updated" << endl; 
		Austin.updateNumOfInfected(); //cout << "updateNumofinfected" << endl; 
		day ++;  
	} 
	if (Austin.herdImmune()) 
		cout << "Herd Immunity acheived" << endl; 
 	
	cout << "Austin is cured after " << day << " days" << endl;
//	cout << Austin.percVacc() << endl; 
//		for (int i = 0; i < Austin.getSize(); i++) { 
//			cout << Austin.getPop().at(i).status_string() << " " ; 
//	}	

}	 
 

