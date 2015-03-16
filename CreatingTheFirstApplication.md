Here you will learn how to create a new Sally application by using Visual Studio.

<font color='#AA2222'>
<h2>Warning</h2>
<blockquote>I would suggest that you are not using your normal Sally installation for the testing. Make a copy of the complete folder for example from <b>C:\Program Files (x86)\Sally Project</b> to <b>C:\dev\Sally Project</b>. So it will later easier to test the installer. Also you can remove all other plugins from this installation (remove the subfolders from the application folder).}}<br>
</font></blockquote>

<font color='#22AA22'>
<h2>Info</h2>
<blockquote>All screenshots are from a German version of Visual Studio 2008 - I will change this as soon as possible.<br>
</font></blockquote>

## Creating the Project ##

![http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew2.jpg](http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew2.jpg)
![http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew3.jpg](http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew3.jpg)

  * First open your Visual Studio
  * Select **File** -> **New** -> **Project...** from the menu bar


  * Select from the left side **Visual C++** and than **Win32**. On the right side select **Win32-Project**.
  * Than type in the name of your application. e.g. **TestApp** and select a storage location.
  * Confirm this dialog by pressing **Ok** (see Screenshot 1).


  * On upcoming wizard, select **Application Options** from the left menu and than select **DLL** and **create empty Project**.
  * Click Finish (see Screenshot 2).


## Creating the First Files ##

![http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew4.jpg](http://fileserver.sally-project.de/dev/wiki/DevFirstAppNew4.jpg)

Now we need to create some basic files.
We need the following files:

  * dllmain.cpp
  * define.h
  * App.cpp
  * App.h

To do this, make a right click on your project an than select Add -> New Element
Select **Code** from the left side and type the name of the first file **dllmain.cpp** and repeat this for all other files (see Screenshot 3).

Now we can add the code to the files.

#### dllmain.cpp ####

```
#include <sallyAPI\sallyAPI.h>

#pragma comment(lib, "sallyAPI.lib")
#define EXPORT __declspec (dllexport) 
#define DLL_IMPORT_SALLY_API 1

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	return TRUE;
}

#include "App.h"

extern "C" EXPORT SallyAPI::GUI::CApplicationWindow* 
	CreateApplication(SallyAPI::GUI::CGUIBaseObject *parent, int graphicId,
									const std::string& pluginPath)
{
	CApp* app;

	app = new CApp(parent, graphicId, pluginPath);
	return app;
}
```

#### define.h ####

This will be empty for now

```


```

#### App.h ####

```
#pragma once
#include <sallyAPI\sallyAPI.h>
#include "Define.h"

class CApp :
	public SallyAPI::GUI::CApplicationWindow
{
private:

public:
	CApp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CApp();
};

```

#### App.cpp ####

```
#include "App.h"

CApp::CApp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
}


CApp::~CApp()
{
}
```

## Create the Resource Folders ##

Navigate now to your Sally installation.
For example: **C:\dev\Sally Project**

Open the application folder and create a new folder based. This should represent an unique id.
Please follow this instructions to create the folder name:

An example: **de.der-knob.sally.app.calculator**

If you have a homepage, than use this URL. If you don't, than just think of what your URLwould be ;)
The first part of the folder is my URL reverse http://www.der-knob.de => de.der-knob

Than the **sally** part indicates that it belongs to the project sally and app says that it is an application. The last part **calculator** is the name of the application.


#### manifest.ini ####

Create inside this folder a file named manifest.ini

```
[info]
apiVersion=0.7.7.1
name=Paint Board
author=Christian Knobloch
version=1.0.0.6
uid=de.der-knob.sally.app.paintboard
```

| **Value** | **Description** |
|:----------|:----------------|
|apiVersion|The version number of the API you are using|
|name|The name of the application|
|author|The name of the author|
|version|Version number of the application|
|uid|This is the uid which is the same as the folder name|

#### pictures ####

Than create inside your app folder a new folder named **pictures**.
Here you can place a png image with 128x128 named **app.png**.
This image will be the application icon.

For example:

![http://fileserver.sally-project.de/dev/wiki/App.png](http://fileserver.sally-project.de/dev/wiki/App.png)

## Make the Project Settings ##

![http://fileserver.sally-project.de/dev/wiki/VsProperties1.jpg](http://fileserver.sally-project.de/dev/wiki/VsProperties1.jpg)

Now we have to adjust the project settings.
Go back to your Visual Studio and select **Project** -> **Properties** from the menu bar.
Select **General** from the left side and change the **Zeichnsatz** to **Multy-Byte** and click **Accept** (see Screenshot 4).

Than select Linker from the left side and change the fist property "Output file".
Change this to the **installation folder of Sally** + **applications** + **your application folder** and leave the **$(ProjectName).dll** at the end

For example: **C:\dev\Sally Project\applications\de.der-knob.sally.app.myproject\$(ProjectName).dll**

Than you can close this dialog by pressing **OK**

## First Build ##

Hit F5 to build the project. By the upcoming dialog, select the sally.exe

For example: **C:\dev\Sally Project\sally.exe**

#### Possible Problems ####

When you get the following LINK error than you don't have the permission to write to this folder:

```
1>LINK : fatal error LNK1104: cannot open file 'C:\dev\Sally Project\applications\de.der-knob.sally.app.myproject\myProject.dll'
```

Please start your VisualStudio as Administrator (right click on the icon -> Run as administrator)

## Other useful links ##

  * Next Tutorial [Creating basic Controls](CreatingBasicControls.md)

  * Overview [How to use the SallyAPI](HowToUseTheSallyAPI.md)