#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	FILE *fp;
	char partitions[60];
	char str[60];
	
	fp = popen("sinfo -o '%P'", "r");
	if (fp == NULL) {
		printf("Failed executing command");
		return(-1);
	}
	int i=0;
	while (fgets(str, 60, fp)!=NULL) {
		partitions[i]=str;
		i++;
	}
	for(int i=0, ;
	pclose(fp);
	
	return 0;
}
