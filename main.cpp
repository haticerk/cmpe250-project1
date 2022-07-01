#include "Character.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <list>
#include <stack>

using namespace std;

class HealthData{
	public: 
	string owner;
	string data;
	int initialHealth;
	HealthData(string name,int healthData){
		this->initialHealth= healthData;
		this->owner = name;
		this->data = to_string(healthData) +" ";
	}
	void setting(int newData){
		this-> data = data + to_string(newData) + " ";
	}
	void print(){
		cout << this->data <<endl;
	}
};


void noSpecialRound(Character *attaker, Character *defender, stack<Character>& ifDies){
	int attackPoint = attaker->attack - defender->defense;
	if(attackPoint>-1){
		defender->remainingHealth = defender->remainingHealth - attackPoint;
		if (defender->remainingHealth <= 0){
			defender->isAlive = false;
			defender->remainingHealth=0;
			ifDies.push(*defender);
		}
	}

}

void isAWizard(Character *wizard, stack<Character>& deads, vector<Character>& community, vector<HealthData> forInitialHealth){
	if(deads.empty()){
		return;
	}
	if(wizard->nRoundsSinceSpecial>50){
		if(deads.top().isAlive){
			for(int i=0; i<=4; i++){
				if(forInitialHealth[i].owner.compare(deads.top().name)==0){
					community[i].remainingHealth=forInitialHealth[i].initialHealth;
					break;
				}
			}

		}else{
			deads.top().isAlive= true;
			for(int i=0; i<=4; i++){
				if(forInitialHealth[i].owner.compare(deads.top().name)==0){
					community[i].remainingHealth=forInitialHealth[i].initialHealth;
					community[i].isAlive = true;
					community[i].nRoundsSinceSpecial=0;
					break;
				}
			}
		}

		wizard->nRoundsSinceSpecial=0;
	}
}

void isADwarf(Character *dwarf, Character *defender, stack<Character>& ifDies){
	if(dwarf->nRoundsSinceSpecial>20){
		int attackPoint = dwarf->attack - defender->defense;
		if(attackPoint>0){
			defender->remainingHealth= defender->remainingHealth - 2*attackPoint;
			if (defender->remainingHealth <= 0){
				defender->isAlive = false;
				defender->remainingHealth=0;
				ifDies.push(*defender);
			}
		}
		dwarf->nRoundsSinceSpecial=0;
	}else{
		noSpecialRound(dwarf, defender, ifDies);
	}

}
void isAnElf(Character *elf, Character *hobbit){
	if(elf->nRoundsSinceSpecial>10){
		int aid;
		aid = elf->remainingHealth/2;
		elf->remainingHealth= aid;
		hobbit->remainingHealth=hobbit->remainingHealth + aid;
		elf->nRoundsSinceSpecial=0;
	}
}

void updatingHealthData(vector<Character>& community, vector<HealthData>& data){
	for(int i=0; i<=4; i++){
		data[i].setting(community[i].remainingHealth);
	}
}


Character& selectingWarrior(string name, vector<Character>& community){
	for(int h=0; h<=4; h++){
		if(community[h].name.compare(name)==0){
			if(community[h].isAlive){
				return community[h];
			}
		}
	}

	vector<string> nameAlphabetic;
	for (int i = 0; i <=4; i++){
		nameAlphabetic.push_back(community[i].name); 
	}
	sort(nameAlphabetic.begin(),nameAlphabetic.end());

	for(int i=0; i<=4; i++){
		if(nameAlphabetic[i].compare(name)==0){
			for(int k=i; k<=3; k++){
				for(int h=0; h<5; h++){
					if(nameAlphabetic[k+1].compare(community[h].name)==0){
						if(community[h].isAlive){
							return community[h];
						}
					}
				}
			}
			for(int k=i; k>0; k--){
				for(int h=0; h<5; h++){
					if(nameAlphabetic[k-1].compare(community[h].name)==0){
						if(community[h].isAlive){
							return community[h];
						}
					}
				}
			}
		}
	}	
}

void vectorCommunity(vector<Character>& community, ifstream& inputFile, int rounds, vector<HealthData>& healthData){

	string name, type;
	int attack, defense, initialHealth;

	for(int i=0; i<=4; i++){
		inputFile >> name >> type >> attack >> defense >> initialHealth;

		Character *warrior = new Character(name, type, attack, defense, initialHealth, rounds);	
		HealthData *data = new HealthData (name, initialHealth);
		community.push_back(*warrior);
		healthData.push_back(*data);

	}
}

int main(int argc, char* argv[]) {

	ifstream inputFile;
	inputFile.open(agrv[1], ios::in);

	string winner;
	int warRound=0;
	int rounds;
	inputFile >> rounds;

	vector<HealthData> com1Data;
	vector<Character> community1;
	stack<Character> killedInCom1;
	vectorCommunity(community1,inputFile,rounds,com1Data);

	vector<HealthData> com2Data;
	vector<Character> community2;
	stack<Character> killedInCom2;
	vectorCommunity(community2,inputFile, rounds,com2Data);


	for(int i=1; i<=rounds;i++){

		string attacker, defender, ifSpecial;
		inputFile >> attacker >> defender >> ifSpecial ; 

		Character *attackerW;
		Character *defenderW; 
		

		if(i%2==1){
			attackerW = &selectingWarrior(attacker,community1);
			defenderW = &selectingWarrior(defender,community2);
		}else{
			attackerW = &selectingWarrior(attacker,community2);
			defenderW = &selectingWarrior(defender,community1);
		}

		if(ifSpecial.compare("NO-SPECIAL")==0){
			if(i%2==1){
				noSpecialRound(attackerW,defenderW,killedInCom2);
			}else{
				noSpecialRound(attackerW,defenderW,killedInCom1);
			}
		}else{
			if(attackerW->type.compare("Elves")==0){
				Character *hobbit;
				if(i%2==1){
					for(int k=0; k<=4; k++){
						if(community1[k].type.compare("Hobbit")==0){
							hobbit= &community1[k];
							break;
						}
					}
				}else{
					for(int k=0; k<=4; k++){
						if(community2[k].type.compare("Hobbit")==0){
							hobbit= &community2[k];
							break;
						}
					}
				}

				isAnElf(attackerW, hobbit);
				if(i%2==1){
					noSpecialRound(attackerW,defenderW,killedInCom2);
				}else{
					noSpecialRound(attackerW,defenderW,killedInCom1);
				}

			}else if(attackerW->type.compare("Dwarfs")==0){
				if(i%2==1){
					isADwarf(attackerW,defenderW,killedInCom2);
				}else{
					isADwarf(attackerW,defenderW,killedInCom1);
				}
		
			}else if(attackerW->type.compare("Wizards")==0){
				if(i%2==1){
					isAWizard(attackerW, killedInCom1, community1, com1Data);
					noSpecialRound(attackerW,defenderW,killedInCom2);
				}else{
					isAWizard(attackerW, killedInCom2, community2, com2Data);
					noSpecialRound(attackerW,defenderW,killedInCom1);
				}
			}else{
				if(i%2==1){
					noSpecialRound(attackerW,defenderW,killedInCom2);
				}else{
					noSpecialRound(attackerW,defenderW,killedInCom1);
				}

			}
		}

		for(int k=0; k<=4; k++){
				community1[k].nRoundsSinceSpecial++;
				community2[k].nRoundsSinceSpecial++;
		}
		updatingHealthData(community1,com1Data);
		updatingHealthData(community2,com2Data);

		warRound++;

		if(i%2==1){
			for(int k=0; k<=4; k++){
				if(community2[k].type.compare("Hobbit")==0){ 
					if(!community2[k].isAlive){
						winner = "Community-1";
						goto result;
					}
				}
			}
		}else{
			for(int k=0; k<=4; k++){
				if(community1[k].type.compare("Hobbit")==0){
					if(!community1[k].isAlive){
						winner = "Community-2";
						goto result;
					}
				}
			}
		}

		if(i%2==1){
			int deadNum=0;
			for(int k=0; k<=4; k++){
				if(!community2[k].isAlive){
					deadNum++;
					if(deadNum==4){
						winner = "Community-1";
						goto result;
					}
				}
			}	
		}else{
			int deadNum=0;
			for(int k=0; k<=4; k++){
				if(!community1[k].isAlive){
					deadNum++;
					if(deadNum==4){
						winner = "Community-2";
						goto result;
					}
				}
			}	
		}
	}
	result: 

	ofstream outputFile;
	outputFile.open(argv[2]);

	if(winner.compare("Community-1")==0){
		outputFile << "Community-1" << endl;
	}else if(winner.compare("Community-2")==0){
		outputFile << "Community-2" << endl;
	}else{
		outputFile << "Draw" << endl;
	}

	int casualties = 0;
	for(int i=0; i<=4; i++){
		if(!community1[i].isAlive){
			casualties++;
		}if(!community2[i].isAlive){
			casualties++;
		}
	}


	outputFile << warRound << endl;
	outputFile << casualties << endl;

	for(int i=0; i<=4; i++){
		outputFile << community1[i].name << " " << com1Data[i].data << endl; 
	}
 	
 	for(int i=0; i<=4; i++){
 		outputFile << community2[i].name << " " << com2Data[i].data << endl; 
 	}

 	outputFile.close();
	inputFile.close();

	return 0;
}
