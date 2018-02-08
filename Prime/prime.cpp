#include <iostream>
#include <string>
#include <cstdlib>
#include "prime.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc > 1){

		int iteration = 5;
		long long num = 0;
		long long a = 0;
		long long b = 0;	
		
		bool testprime = false;
		bool printprime = false;
		bool sumprime = false;
		bool naive = false;


		
		for(int i = 0 ; i < argc ; i++){
			string opt(argv[i]);
			if(opt == "-p"){
				printprime = true;
				a = atoi(argv[i + 1]);
				b = atoi(argv[i + 2]);
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

		long long sum = 0;

		if(testprime){
			if(MillerRabin(num, iteration))
				cout << num << " est premier !" << endl;
			else
				cout << num << " n'est pas premier !" << endl; 
		}

		for(long long i = a ; i < b ; i++){
			
			bool premier = false;

			if(!naive)
				premier = MillerRabin(i, iteration);
			else
				premier = estPremierNaif(i);

			if(premier){
				sum += i;
				if(printprime)
					cout << i << endl;
			}
		}

		if(sumprime)
			cout << "Somme des nombre premiers de " << a << " a " << b << " = " << sum << endl;
	}
	else{
		cout << "Usage: " << argv[0] << " [OPTION] ([OPTION]) ([OPTION])"<< endl;
		cout << "OPTIONS:" << endl;
		cout << "  -p <from> <to> => affiche les nombres premiers" << endl;
		cout << "  -s <from> <to> => somme des nombres premiers" << endl;
		cout << "  -t <entier> => test si le nombre est premier" << endl;
		cout << "  -i <entier> => nombre d'iteration de MillerRabin" << endl;
		cout << "  -n => utillise l'algo naif pour les calculs" << endl;
	}
	return 0;
}