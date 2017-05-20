----------------------------------------------
Font Generation Tool Version 2.1.0.06 
11 April 2007
----------------------------------------------

	Font Generation Tool is a GUI application that allows application
	developers to create, edit and manage display (VFT and FON files) 
	and printer font files (PFT files) and printer logo files 
	(LGO files) and converting Bitmap files to VFT or LGO files. 

	Printers supported are: Integrated Thermal Printer (ITP) of Verix 33xx, 
	Verix 3600, Verix 37xx, Verix V 5xxx ,Verix V 6xxx and P950 Printers. It also supports
	font and logo downloads to the Omni 3xxx ITP. The main advantage of using
	Font Generation Tool is that the user does not need to know or understand
	the font file formats.

	It also supports font and logo downloads to SC 5000 PINPads and V5 (VMT) PINPads.

Note: The About box in the Font Generation Tool will show the Version as 2.1.0.05 as the executable has
      not been changed from 2.1.0.05 to 2.1.0.06 release.


Features:
-------------

	* Generation of Display Font files (VFT and FON file formats)

	Allows the user to create user defined display font files in the VFT and 
	FON file formats in 6x8, 8x16 and 16x16 resolution. It also allows the user
	to edit existing VFT and FON files.


	* Generation of Printer Fonts (PFT file format)

	Allows the user to create user defined printer font files in the PFT
	file format in 5x8, 8x8 (supported only in P950), 8x14 
	(32 and 42 column resolution), 16x16, 24x24, 32x32, 48x48, and 64x64
	resolution. It also allows the user to edit existing PFT files and
	convert it to other supported printer font types. 


	* Generation of Printer Logo Image File (LGO)

	Allows the user to view printer logo files. Logo files can be used 
	to print graphics in the printer receipts.


	* Converting Bitmap Files to VFT and LGO Files

	Font Generation Tool supports conversion of bitmaps to display
	and logo files. To convert a bitmap file to display font file or logo
	file, the bitmap file must be monochromatic. The user can choose an 
	area not exceeding 128x64 (for 8 line display) , 128x128 
	(for 16 line display) pixels in size for the conversion of bitmap 
	to VFT file. To convert a bitmap file to printer logo file, the user
	has to select an area not exceeding 384x240 pixels, which is the 
	maximum resolution of printer.


	* Merging multiple VFT and FON font files

	Font Generation Tool supports merging multiple VFT and FON files into a 
	single VFT or FON file. A wizard is provided which will guide the user 
	through the steps required for merging. The user can select characters 
	by specifying the range from the input file to the output file. Also, 
	user can give an offset value so as to where the input file characters 
	need to be merged into the output file.


	* Download of Font File, Logo File and Firmware for the required unit.

====================
Custom Installation 
====================
User can select the components required.

=====================================================
Typical & Compact Installation
=====================================================

Directory structure of Font Generation Tool
-------------------------------------------

Font Generation Tool Installation directory
|
|-----> Bin (FontTool.exe, Help files and Download Batch Files)
|
|-----> Samples (Display font files, Printer font files, Big Font Files, 
		Logo files, Bitmap Files, ITP_Applet sample binaries)


---------------------
Minimum Requirements
---------------------
	To use this package, you must have the following:

 	 -  Windows 2000 or WindowsXP
  	 -  VISUAL STUDIO IDE
 	 -  Adobe Acrobat Reader 6.0 or above
 	 -  Omni 3750 terminal  - OS version  Q50018A1
  	 -  Vx510 terminal  	 - OS version QA0008A3
  	 -  Vx610 terminal	 _ OS Version QB0108A3
 	 -  Vx570 terminal	 _ OS Version QC0008A3	
 	 -  Vx670 terminal	 _ OS Version QD0008A3
	 -  V5(VMT) terminal	 _ OS Version QE0008A1 



To install the Font Generation Tool, follow these steps:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	1.   Close all the applications you are running.

	2.   If you have received the release image in a zip file, copy the zip file to a temporary folder. Extract the zip file with "Use Folder Names" option. Run the setup.exe from the extracted location.

	3.   If you have received the image in the form of installation CD, please insert it into your CD-ROM drive. Run <CD drive>:\setup.exe

	4.   The installation wizard creates the required files and displays the Welcome window.


	     The installation process will guide you through the rest of the process.

Changes from Ver 2.0 to 2.1.0
================================
1. The Windows Font Conversion feature is implemented, this will convert the Windows Standard fonts and the Windows Unicode fonts to Display Font file (VFT) and Big Font File(Fon) respectively.
2. Help File is updated for the windows Font Conversion feature implemented.
3. Samples Folder is updated with new vft files added, which are converted using Windows Standard Font Conversion feature.

Note:
1. Refer http://www.alanwood.net/unicode for Unicode fonts for various languages.
2. The Windows Unicode Font Option will convert the font to Big Font File format (FON), and Windows Standard Font option will convert the font to
  Display Font File Format (VFT).

Changes from Ver 1.4 to Ver 2.0 
================================
1. 16 line display feature is implemented. 
2. Option for saving the file vft file as 6X8, 8X16, and 16X16 has been done.
3. Short cut icon of installation and removal of tool is done.
4. Mfc42.dll is added for the tool to run on machine which does not have Visual studio . 
5. Partial Download feature has been removed.
6. Download issue on installation of tool other than default folder has been done.
7. Help file is changed to .chm format.



Deployment ITP option when files are downloaded to the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The option Deployment ITP will print the default 16x16.pft file when chosen


Known Limitations:
==================
1. The *.* fon type converted image might merge and *.* fon type converted may not get the exact conversion.
 
<EOD>
