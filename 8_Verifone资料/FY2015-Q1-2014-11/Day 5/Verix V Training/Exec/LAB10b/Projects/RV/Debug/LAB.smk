#------------------------------------------------------------ 
#
#       LAB.SMK - NMAKE file to build the LAB sample
#				application using the SDS tools
#
#-------------------------------------------------------------


#
# Paths
#


ACT2000=$(VACT2000) 

# SDS Tools Related Paths

SDSIncludes = $(VRXSDK)\include

#  App Source Paths
SrcDir = ..\..\..\Source

ACTIncludes = $(ACT2000)\include
TCPIPIncludes = $(VTCPIP)\include
UCLIncludes = $(VUCL)\include

#  Compiler/Linker/Outhdr Output Paths
ObjDir = ..\..\..\obj
OutDir = ..\..\..\output\RV\files\Debug
OSfile = D:\VERIXV\EXEC\LAB\Gid0


ACTLibraries = $(ACT2000)\output\RV\files\static\Debug
TCPLibraries = $(VTCPIP)\OutPut\RV\Files\Static\Debug
UCLLibraries = $(VUCL)\OutPut\RV\Files\Static\Debug


#
# Options for Tools
#

# Compiler Options
Includes = -I$(SDSIncludes) -I$(ACTIncludes) -I$(TCPIPIncludes) -I$(UCLIncludes)
# for release version change the COptions to 
COptions =  -DLOGSYS_FLAG


#
# Dependencies
#

AppObjects = \
        $(ObjDir)\LAB.o	\
        $(ObjDir)\sender.o	\
        $(ObjDir)\appobj.o

#Using ACT shared library.

ACTLibs =						\
$(ACTLibraries)\act2000.a	\
$(TCPLibraries)\vtcpip.a \
$(UCLLibraries)\ucl.a

	
#
#  sample Target Definition
#

pseudoOut : $(OutDir)\LAB.out
	$(VRXSDK)\bin\vrxhdr -s 15000 -h 5000 $(OutDir)\LAB.out

#For 3xxx, use the following lines to sign the out and scf file. 
	
	"$(VSFSTOOL)\filesignature" .\LAB.fst -nogui

	copy $(OutDir)\LAB.out.p7s $(OutDir)\signfile\ram\LAB.p7s
	del $(OutDir)\LAB.out.p7s
	
$(OutDir)\LAB.out : $(AppObjects)
  $(VRXSDK)\bin\vrxcc $(AppObjects) $(ACTLibs) -o LAB.out
	move  LAB.out $(OutDir)

$(OSfile)\free.scf:    $(SDSFiles)\free.sc
    $(VRXSDK)\bin\mkscf -fgid2 $(SDSFiles)\free $(OSfile)\free.scf _notused
	

######  Compile #######

$(ObjDir)\LAB.o : $(SrcDir)\LAB.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\LAB.c | "$(DTKTOOLS)\fmterr.exe"
	move LAB.o	$(ObjDir)

$(ObjDir)\Sender.o : $(SrcDir)\Sender.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\Sender.c | "$(DTKTOOLS)\fmterr.exe"
	move Sender.o	$(ObjDir)

$(ObjDir)\Appobj.o : $(SrcDir)\Appobj.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\Appobj.c | "$(DTKTOOLS)\fmterr.exe"
	move Appobj.o	$(ObjDir)
