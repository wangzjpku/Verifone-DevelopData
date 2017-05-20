#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svctxo.h>

int main (int argc, char *argv[])
{
	int hConsole = 0;
	hConsole = open("/dev/console",0);
	clrscr();
	set_cursor(1);

	write_at("HELLO!!",7,1,1);
    write_at("This is a sample",16,1,3);
    write_at("for VerixV ",10,1,4);
    write_at("in TXO Mode",11,1,5);

	close(hConsole);
	return 0;
}

