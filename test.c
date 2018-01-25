#include <unistd.h>

int yes() {}
int no() {}

int main() {
	char a[0x20];
	int ret = read(0, a, 0x30);
	if (a[0] == 'A') {
		yes();
	}
	if (a[1] == 'B') {
		no();
	}
	return 0;
}
