#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svctxo.h>
#include <stdio.h>
#include <aclconio.h>
#include <aclstr.h>



void doCountDown(void);

char szKeyMap[MAX_ALPNUM_KEYS][CHAR_PER_KEY]={
           "0- +Q",   "1Z.AB",   "2CDEF",  "3GHIJ",  "4KLMN",  "5OPRS",
           "6TUVW",   "7XY*,",   "8'\"#=",  "9:$~^",  "{}[]\\","&%()!"};


int main (int argc, char *argv[])
{
	int hConsole = 0;
	hConsole = open("/dev/console",0);
	clrscr();
	set_cursor(1);

	write_at("HELLO!!",7,1,1);
    write_at("This is a sample",16,1,3);
    write_at("for Verix ",10,1,4);
    write_at("in TXO Mode",11,1,5);

    clrscr();
    doCountDown();

	close(hConsole);
	return 0;
}

void doCountDown()
{
    char szDispStr[21],buff[21];
    int iStartCount=0; 
    memset(buff,0x0,sizeof(buff));
    set_cursor(0);

    write_at("Enter Start Count",17,1,1);

	window(1,2,21,2);
	gotoxy(1,1);

    getkbd_entry(0,"",buff,(unsigned int) 100000,
					(unsigned int) NUMERIC,szKeyMap,sizeof(szKeyMap), 2, 1);
    iStartCount = strn2int(buff,2);

    window(1,1,21,8);
    clrscr();

    while(1)
    {
        if (0==iStartCount)
        {
            clrscr();
            strcpy(szDispStr,"BLAST OFF");
            write_at(szDispStr,strlen(szDispStr),1,1);
            error_tone();
            SVC_WAIT(200);
            error_tone();
            SVC_WAIT(200);
            error_tone();
            break;
        }
        sprintf(szDispStr,"COUNTDOWN %02d",iStartCount);
        write_at(szDispStr,strlen(szDispStr),1,1);
        normal_tone();
        SVC_WAIT(1000);
        iStartCount --;
    }

}