Theming was one of the features we implemented at first place into Sally.

## Sally Theme Module ##

The complete user interface (UI) in Sally consists of image provieded by a theme.

All installed themes can be found in the sally install folder "C:\Program Files (x86)\Sally Project\" and here in the subfolder "themes". All themes should have an unique folder name. We are using the java package naming conventions. For example:

de.der-knob.sally.theme.clean
de.der-knob.sally.theme.ilovepuma
de.der-knob.sally.theme.whiteui

Here you can see, that all those themes are created from one person (my self).

de.der-knob is reverse my URL (without the www and http): http://www.der-knob.de => de.der-knob

Than the **sally** part indicates that it belongs to the project sally and theme says that it is a theme. The last part **clean** or **ilovepuma** is the name of the theme.

<font color='#22AA22'>
<h2>Info</h2>
<blockquote>The foldername should not contain spaces or any special characters.<br>
</font></blockquote>


### UI Controls ###

The UI control images can be found in the subfolder gui.

For example a button has three images. The left part, the right part and the middle part of the button.

The middle part (only 1 pixel with - so it's hard to see) is stretched to fill the space between the left and right image of the button. Than we have the button with the same images as pressed button:

And also (for a button) we have a disabled state and a default button.

We tried to have a consistent naming of the files. So here is the table of how it should be:

| **Name starting with** | **Description** |
|:-----------------------|:----------------|
| n| normal state of the control |
| d| disabled state of the control |
| p| pressed state of the control |
| db| default button face of the control |

Besides the controls also the icons uses in the UI are bundled in a theme.

### Icons ###

The icons can be found in **icons**, **mimetypes**, **onscreen** and **falgs** subfolders of the theme.

If you want to replace the icons here, you should replace the icon with a icon in the same width and heigh. Here is a list of the subfolders and the images which can be found in there:

| **Subfolder** | **Description** |
|:--------------|:----------------|
|flags |country flags|
|icons|action icons|
|mimetypes|icons for file mimetypes (audio, vidoe, text, ...)|
|onscreen|icons for the on screen menu|

### manifest.ini ###

In the manifest.ini (located in the theme folder) you will find at the top the informations about the theme:

```
[info]
author=Christian Knobloch
name=Clean
version=2.0.0.1
uid=de.der-knob.de.sally.theme.clean
```

| **Value** | **Descrption** |
|:----------|:---------------|
|author|The theme author|
|name|The (short) name of the theme (equals to the theme folder name)|
|version|Version of the theme|
|uid|This is the uid which is the same as the theme folder name|

Everytime you change somthing to the theme and upload it to the community you must change the version to a higher number. The uid should never change. If you need to change it, than also rename the folder name and we will than have a new theme.

Blow the info part you can find the different font settings. Each font used at in the UI is defined here. The easiest way, would be you leave everything as it is and only change the font color.

```
[menu.font.small]
r=0
g=0
b=0
size=7
bold=1
italic=0
face=Arial
```

| **Value** | **Description** |
|:----------|:----------------|
|menu.font.small|The header describes the identifier for this type|
|r, g, b|This the the RGB color value of the font. Possible values: 0 - 255|
|size|The size of the font|
|bold|Bold true (1) or false (0)|
|italic|Italic true (1) or false (0)|
|face|The font used for this type|

## How to Start ##

The best thing would be, that you copy one of the existing themes and rename it (please use the naming conversion descriped above). You should leave the new theme folder in the sally theme folder, so you can directly test the theme in sally.

Open the theme.ini and change the [info](info.md) area.

Than you exchange the images with your own images and change the font faces in the theme.ini.

## Create a Installation Package ##

When you are done, than zip the folder.

<font color='#AA2222'>
<h2>Warning</h2>
<blockquote>Here is important, that you need to zip the folder (e.g. de.der-knob.sally.theme.ilovepuma), not only the content!!!<br>
</font></blockquote>

Make sure if you open the zip file, the ui folder name is the first level you can see.}}


Than change the file extension from **.zip** to **.sallytheme**.

You can now move your new theme from the sally theme folder (just move it to a backup folder) and double click your new theme package with the **.sallytheme** extension. This should start the installer. Install the package and verify it in sally.

When you are done, you can upload the file into the [Sally Community](http://www.sally-project.org).

## FAQ ##

We know, that this tutorial is not complete. If you have any question contact us [webmaster@sally-project.org](mailto:webmaster@sally-project.org)