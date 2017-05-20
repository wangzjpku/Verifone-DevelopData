#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svctxo.h>
#include <stdio.h>



void doCountDown(int iStartCount);

int main (int argc, char *argv[])
{
	int hConsole = 0, iStartCount=10;
	hConsole = open("/dev/console",0);
	clrscr();
	set_cursor(1);

	write_at("HELLO!!",7,1,1);
    write_at("This is a sample",16,1,3);
    write_at("for Verix ",10,1,4);
    write_at("in TXO Mode",11,1,5);

    clrscr();
    doCountDown(iStartCount);

	close(hConsole);
	return 0;
}

void doCountDown(int iStartCount)
{
    char szDispStr[21];
    set_cursor(0);
    while(1)
    {
        sprintf(szDispStr,"COUNTDOWN %02d",iStartCount);
        write_at(szDispStr,strlen(szDispStr),1,1);
        SVC_WAIT(1000);
        iStartCount --;
        if (0==iStartCount)
        {
            clrscr();
            strcpy(szDispStr,"BLAST OFF");
            write_at(szDispStr,strlen(szDispStr),1,1);
            SVC_WAIT(3000);
            break;
        }

    }

}
