#include <stdio.h>
#include <unistd.h>

void call_me() {
	printf("GJ!\n");
}

void vuln() {
	char buf[64];
	read(0, buf, 128);
}

int main() {
	vuln();
	return 0;
}
