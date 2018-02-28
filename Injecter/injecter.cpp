#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

#define ERREUR_OUVERTURE(F) \
	std::cout << "Erreur: ouverture du fichier " << (F) << " impossible" << std::endl;

/* Macro qui inject du code dans un flux */
#define CODE_INJECTION(FLUX, MESSAGE, NUMERO) \
	(FLUX) << "#include <iostream>\nusing namespace std;\nint main(){cout<<\"" << (MESSAGE) << " \"<<" << (NUMERO) << "<<endl;}";

void injectCode(const string& filepath, const string& message, const int& numero){

	std::ofstream file(filepath.c_str());

	if(!file.is_open()){
		ERREUR_OUVERTURE(filepath);
		return;
	}

	CODE_INJECTION(file, message, numero);

	file.close();
}

int main(int argc, char const *argv[])
{
	if(argc >= 3){
		int n = atoi(argv[1]);
		string message(argv[2]);

		for(int i = 0 ; i < n ; i++){
			stringstream ssName; ssName << "prog" << i << ".cpp";
			stringstream ssExe; ssExe << "prog" << i;

			injectCode(ssName.str(), message, i);
			
			if(argc > 3){
				string opt(argv[3]);
				if(opt == "-c" || opt == "-e"){
					stringstream ssCode; ssCode << "g++ " << ssName.str() << " -o" << ssExe.str();
					system(ssCode.str().c_str());
					if(opt == "-e"){
						remove(ssName.str().c_str());
					}
				}
			}
		}
	} 
	else{
		cout << "Usage: " << argv[0] << " <nombre> <message> [OPTION]" << endl;
		cout << "OPTIONS:" << endl;
		cout << "  -c => compile les fichiers créés." << endl;
		cout << "  -e => compile les fichiers créés et supprime les sources." << endl;
	}
	return 0;
}

