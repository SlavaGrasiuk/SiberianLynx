#include <cstdio>

/*
==================
main
==================
*/
int main() {
	auto file = fopen("crd.txt", "r");
	char text[100];
	fread(text, sizeof text, 1, file);

	__asm("nop");

	fclose(file);

	return 0;
}
