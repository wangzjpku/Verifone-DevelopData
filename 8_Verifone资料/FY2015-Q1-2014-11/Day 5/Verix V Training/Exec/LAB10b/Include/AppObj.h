#ifndef __CAPPOBJ_H
#define __CAPPOBJ_H

//#include "applicationobj_c.h"
#include "applicationobj.h"

typedef struct tagAppObj
{
	ApplicationObj super;
	short (*SetParam)(short paramId, unsigned char *buffer, short size);
}AppObj; 

AppObj *CreateAppObj(short type);

#endif