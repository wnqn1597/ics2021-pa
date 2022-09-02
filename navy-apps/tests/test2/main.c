#include <stdio.h>

int main(){
	printf("TEST2\n");
	FILE *f = fopen("share/files/num", "r");
	fclose(f);
	return 0;
}
