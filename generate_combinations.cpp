#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char* json_file = strdup(argv[1]);
	printf("\nJSON FILE being read is %s\n", json_file);
	FILE* fjson = fopen64(json_file, "r");
	fclose(fjson);
	return 0;
}
