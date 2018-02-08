#include <iostream>
#include <string>
#include <cstdlib>
#include "prime.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc > 1){

		int iteration = 6;

		long long num = 0, sum = 0;
		long long a = 0, aP = 0;
		long long b = 0, bP = 0;

		bool testprime = false;
		bool printprime = false;
		bool sumprime = false;
		bool naive = false;

		for(int i = 0 ; i < argc ; i++){

			string opt(argv[i]);

			if(opt == "-p"){
				printprime = true;
				aP = atoi(argv[i + 1]);
				bP = atoi(argv[i + 2]);
			}
			else if(opt == "-s"){
				sumprime = true;
				a = atoi(argv[i + 1]);
				b = atoi(argv[i + 2]);
			}
			else if(opt == "-t"){
				testprime = true;
				num = atoi(argv[i+1]); 
			}
			else if(opt == "-n"){
				naive = true;
			}
			else if(opt == "-i"){
				iteration = atoi(argv[i + 1]);
			}
		}

		if(!a && !b){a = aP; b = bP;}
		if(!aP && !bP){aP = a; bP = b;}

		if(testprime){

			bool premier = false;

			if(!naive)
				premier = MillerRabin(num, iteration);
			else
				premier = estPremierNaif(num);

			if(premier)
				cout << num << " est premier !" << endl;
			else
				cout << num << " n'est pas premier !" << endl; 
		}

		long long aMin = min(aP, a);
		long long bMax = max(bP, b);

		for(long long i = aMin ; i <= bMax ; i++){
			
			bool premier = false;

			if(!naive)
				premier = MillerRabin(i, iteration);
			else
				premier = estPremierNaif(i);

			if(premier){
				if(sumprime && i >= a && i<= b)
					sum += i;
				if(printprime && i >= aP && i <= bP)
					cout << i << endl;
			}
		}

		if(sumprime)
			cout << "Somme des nombres premiers de " << a << " a " << b << " = " << sum << endl;
	}
	else{
		cout << "Usage: " << argv[0] << " [OPTION] ([OPTION]) ([OPTION])"<< endl;
		cout << "OPTIONS:" << endl;
		cout << "  -p <from> <to> => affiche les nombres premiers" << endl;
		cout << "  -s <from> <to> => somme des nombres premiers" << endl;
		cout << "  -t <entier> => test si le nombre est premier" << endl;
		cout << "  -i <entier> => nombre d'iteration de MillerRabin (precision)" << endl;
		cout << "  -n => utillise l'algo naif pour les calculs (plus lent)" << endl;
	}
	return 0;
}