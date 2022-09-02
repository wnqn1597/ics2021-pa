#include <stdio.h>

int main(){
	FILE *f = fopen("share/files/num", "r");
	fclose(f);
	return 0;
}
