#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <chrono>

using namespace std;

vector<int> multizbior;
vector<int> mapa;
vector<int> used;
int maxind = 0;
int maxElement = 0;
typedef chrono::high_resolution_clock Clock;
typedef Clock::time_point ClockTime;

void printExecutionTime(ClockTime start_time, ClockTime end_time)
{
	auto execution_time_ms = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
	auto execution_time_milis = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
	auto execution_time_sec = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
	auto execution_time_min = chrono::duration_cast<chrono::minutes>(end_time - start_time).count();

	cout << "Czas pracy: ";
	if (execution_time_min > 0)
		cout << "" << execution_time_min<< " Minut ";
	if (execution_time_sec > 0)
		cout << "" << execution_time_sec<< " Sekund ";
    if (execution_time_milis > 0)
        cout << "" << execution_time_milis % 1000<< " Milisekund i ";
    if (execution_time_ms > 0)
        cout << "" << execution_time_ms % 1000<< " Mikrosekund\n";
}

bool readFromFile(const string &filename) {
    fstream file(filename);
    if (file.is_open()) {
        string line;
        if (getline(file, line)) {
            istringstream lineStream(line);
            int number;
            while (lineStream >> ws >> number) {
                multizbior.push_back(number);
            }
        }
        file.close();
        cout<<"Plik zostal poprawnie odczytany"<<endl;
        return true;
    } else {
        cout<<"Blad z odczytem pliku. Zamykam program"<<endl;
        return false;
    }
}

bool checkNewtonFormula(const vector<int> &multizbior) {
    int A = multizbior.size();
    double n = sqrt(1 + (8 * A));
    if(floor(n) == n){
        int k = (n-3)/2;
        cout<<endl;
        cout<<"Przewidywana liczba ciec: "<<k<<endl;
        maxind = k+1;
        cout<<"Przewidywana liczba elementow mapy: "<<maxind<<endl;
        return true;
    }else{
        cout<<"W wczytanej instancji wystepuje blad. Zamykam program"<<endl;
        return false;
    }
}

void pierwszyElement(vector<int> &mapa, vector<int> &multizbior) {
    int firstElement = multizbior[multizbior.size() - 1] - multizbior[multizbior.size() - 2];
    maxElement = multizbior[multizbior.size() - 1];
    mapa.push_back(firstElement);
    used.push_back(firstElement);
}

void znajdzMape(int ind, bool &istnieje){
    bool znaleziono = false;
	if(ind == maxind){ //sprawdzenie czy znaleziono rozwiazanie
		set<int> multizbiorCaly;
    	multizbiorCaly.insert(multizbior.begin(), multizbior.end());
    	set<int> uzyteCaly;
    	uzyteCaly.insert(used.begin(), used.end());
		if(multizbiorCaly == uzyteCaly){cout<<endl;
			cout << "Multizbior: " << endl;
			for(int i = 0; i < multizbior.size(); i++)
				cout << multizbior[i] << " ";
			cout << endl;
			cout<<endl;
			cout << "Used: " << endl;
			for(int i = 0; i < used.size(); i++)
				cout << used[i] << " ";
			cout << endl;
		    cout<<endl;
			cout << "Znaleziono rozwiazanie: " << endl;
			for(int i = 0; i < mapa.size(); i++)
				cout << mapa[i] << " ";
			cout << endl;
			istnieje = true;
			znaleziono = true;
		}
	}

	if(!znaleziono){ //jesli nie znaleziono to kontynuujemy szukanie
        //WYBIERANIE I SPRAWDZANIE CZY KANDYDAT BYL UZYTY
		int sprawdzonyElement = 0;
		for(int i = 0; i < multizbior.size(); i++){
			int elelement_wMultizbiorze = 0;
			int element_wUsed = 0;
			for(int j = 0; j < multizbior.size(); j++){
                if(multizbior[i] == multizbior[j]){
                    elelement_wMultizbiorze++;
				}
			}
			for(int j = 0; j < used.size(); j++){
                if(multizbior[i] == used[j]){
                    element_wUsed++;
                }
			}

			if(elelement_wMultizbiorze <= element_wUsed || multizbior[i] == sprawdzonyElement){
                continue;
			}

			int pamiecMapy = used.size(); //zapisujemy aktualna wielkosc mapy. jest to swego rodzaju pamiec ktora pomoze przywrocic nam poprzedni stan
			mapa.push_back(multizbior[i]);
			used.push_back(multizbior[i]);
			int suma = multizbior[i]; //do suma dodawany jest sprawdzany element
			bool sumaIstnieje = false;
			bool next = false;

			//SPRAWDZENIE CZY SUMA Z MAPY WYSTEPUJE W MULTIZBIORZE
			for(int j = mapa.size()-2; j >= 0; j--){
				int suma_multizbior = 0;
				int suma_used = 0;
				suma += mapa[j]; //kazda iteracja dodaje nowy element do sumy, zeby sprawdziæ czy ta suma istnieje
				for(int k = 0; k < multizbior.size(); k++){
                    if(multizbior[k] == suma){
						suma_multizbior++;
						sumaIstnieje = true;
					}
				}

				for(int l = 0; l < used.size(); l++){
                    if(used[l] == suma){
                        suma_used++;
                    }
				}

				if(suma_multizbior > suma_used){
                    used.push_back(suma);
				}

				if(!sumaIstnieje || suma_multizbior <= suma_used){
					next = true;
					for(int j = used.size()-1; j >= pamiecMapy; j--){ //odejmuje po kolei ostatni elemenet do momentu az usunie wszystkie sumy ktore dodal w tym kroku
                        used.pop_back();
					}
					break;
				}
			}
            //SPRAWDZENIE CZY ROZNICA MIEDZY NAJWIEKSZYM ELEMENTEM I NAJWIEKSZA SUMA ISTNIEJE W MULTIZBIORZE
			if(!next){ //jesli sumy sie zgadzaja czyli bool 'next' nie został ustawiony na true to kontynuuujemy sprawdzanie tego kandydata
				int dyst_multizbior = 0;
				int dyst_used = 0;
				int dyst_doKonca = maxElement - suma; //od najwiekszego elementu ktory jest dlugoscia calej mapy odejmujemy wczesniejsza najwieksza sume
				for(int j = 0; j < multizbior.size(); j++){
                    if(dyst_doKonca == multizbior[j]){
                        dyst_multizbior++;
                    }
				}

				for(int j = 0; j < used.size(); j++){
                    if(dyst_doKonca == used[j]){
                        dyst_used++;
                    }
				}

				if(dyst_multizbior > dyst_used && ind < maxind){
                    znajdzMape(ind+1, istnieje);
				}

				else if(ind == maxind - 1 && dyst_multizbior == 0 && suma == maxElement){
                    znajdzMape(ind+1, istnieje);
				}
			}
			sprawdzonyElement = mapa.back(); //przypisujemy elemenet ktory zostal sprawdzony zeby wiedziec na jakim skonczylismy
			mapa.pop_back(); //usuwamy kandydata z mapy ktory okazal sie bledny
			for(int j = used.size()-1; j >= pamiecMapy; j--){ //usuwamy wszystkie elementy ktore dodalismy do 'used' w tym kroku. Przywracamy go do dlugosci ktora wczesniej zapisalismy...
                used.pop_back();
			}

			if(istnieje)
				break;
		}
	}
}

int main()
{
    ClockTime start_time = Clock::now();

    if(!readFromFile("C:/Users/Basia/Desktop/AKwB_4/Gotowe/Testy/Test.txt")){
        return 0;
    }

    cout<<"Wczytany multizbior:"<<endl;
    for(int i = 0; i < multizbior.size(); i++){
        cout << multizbior[i] << " ";
    }
    cout << endl;
    sort(multizbior.begin(), multizbior.end());
    cout<<"Multizbior posortowany rosnaco:"<<endl;
    for(int i = 0; i < multizbior.size(); i++){
        cout << multizbior[i] << " ";
    }
    cout << endl;

    if(checkNewtonFormula(multizbior)){
        pierwszyElement(mapa, multizbior);
    }else{
        return 0;
    }

    bool mapExists = false;
    znajdzMape(1, mapExists);
    ClockTime end_time = Clock::now();
    if(!mapExists){
        cout<<"Nie znaleziono mapy"<<endl;
    }else{
        printExecutionTime(start_time, end_time);
    }
    return 0;
}
