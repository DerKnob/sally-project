## Visual Studios ##

We are using Visual Studio 2008 for the development.

But you should be able to create plugins for sally with the following list:

**Visual Studio 2008**

Also the Visual C++ Express is working. This one is free and you can download it from the here: http://www.microsoft.com/express/downloads/

Visual Studio 2010 has some problems... I will fix that soon.

## Additional SDKs ##

### DirectX ###

To create a plugin for Sally you will need the DirectX SDK installed.

http://www.microsoft.com/downloads/details.aspx?FamilyID=2c7da5fb-ffbb-4af6-8c66-651cbd28ca15&displaylang=en

After installation open Visual Studio and select in the menubar:

Tools->Options
Then on the left: Projects and Solutions->VC++ Directories.
Then on the rigth top at "Show directories for:" choose "Include files" and add the path to the include files folder:

For example: "C:\Program Files (x86)\Microsoft DirectX SDK (February 2010)\Include"

Then do the same for "Library Files".
On the rigth top at "Show directories for:" choose "Library files" and add the path to the library files folders.

The pathes here should be about: "C:\Program Files (x86)\Microsoft DirectX SDK (February 2010)\Lib\x86"


### SallyAPI ###

Download the latest sallyAPI from here: [How to use the SallyAPI](HowToUseTheSallyAPI.md)

Unzip the archive and execute the install.bat. It contains the sallyAPI include and lib files. Also all 3d Party libs and includes are included in this Zip file.