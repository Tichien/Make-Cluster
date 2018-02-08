#include <iostream>
#include "MillerRabin.cpp"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[]){

    int iteration = 5;

    long long num = strtol(argv[1], NULL, 0);

	//cout<<"Enter integer to test primality: ";

    //cin>>num;

    if (MillerRabin(num, iteration))

        cout<<num<<" is prime"<<endl;

    else

        cout<<num<<" is not prime"<<endl;

    return 0;

}
