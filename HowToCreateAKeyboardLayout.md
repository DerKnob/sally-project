The On-Screen-Keyboard in Sally can be customized to display all buttons known from a normal keyboard.


## How to Start ##

You can download the 'keyboard starter package' here: [download](http://code.google.com/p/sally-project/downloads/detail?name=keyboard_starter_pack_1.0.zip)

Than rename the extracted folder to your language. You should keep **sally-project.keyboard**. at the beginning and add the two characters for your iso code.

Your folder should now look like this **sally-project.keyboard.de** (for german language)


## Creating your own keyboard layout ##

Now you need to customize the files it self.

Change into your folder and you will find 3 files.

  * manifest.ini
  * sally-project.layout
  * sally-project.smart


### sally-project.keyboard.layout ###

The sally-project.keyboard.layout file contains the keyboard map. Important is that you must have the exact line length (fill with spaces) and you must have twelve lines in the file:


| **File content** | **Description** | **Line length** |
|:-----------------|:----------------|:----------------|
|^1234567890ß´|The four line of the keybaord with small letters|13|
|qwertzuiopü+|  |12|
|asdfghjklöä#|  |12|
|<yxcvbnm,.-|  |11|
|°!"§$%&/()=?`|The four line of the keybaord with big letters|13|
|QWERTZUIOPÜ|  |12|
|ASDFGHJKLÖÄ'|  |12|
|>YXCVBNM;:|  |11|
|²³ \|The four line of the keybaord with ALT-Gr pressed|13|
|@ € ~|  | 12 |
|  |  |12|
|  |  |11|


### sally-project.keyboard.smart ###

The sally-project.keyboard.smart file has only a list with common words for the language. The words are sperated by a new line. In german I have searched for a list of the 1000 mosted used words. It must be sorted alphabetically:

```
ab
abend
Abend
aber
Aber
acht
AG
Aktien
Alle
alle
allein
allen
aller
allerdings
Allerdings
[...]
```


### manifest.ini ###

The manifest.ini is very important. Here you have to specify some main informations.

Here is an exmaple:

```
[info]
author=Christian Knobloch
name=Deutsch
version=1.0.0.1
uid=sally-project.keyboard.de
isocode=de
```

| **Value** | **Descrption** |
|:----------|:---------------|
|author|The theme author|
|name|The language name (can be written in the destination language)|
|version|Version of the keyboard file|
|uid|This is the uid which is the same as the folder name|
|isocode|The two characters ISOCode|

Everytime you change something to your keyboard file and upload it to the community please increase the version number. Otherwise the Sally autoupdate for this plugin will not work.


## Create a Installation Package ##

When you are done, than zip the folder.

Important here is, that you need to zip the folder (e.g. sally-project.keyboard.de), not only the content!!!

Make sure if you open the zip file, the ui folder name is the first level you can see.

Than change the file extension from **.zip** to **.sallykeyboard**.

Double click your new language package with the **.sallykeyboard** extension. This should start the installer. Install the package and verify it in Sally.

When you are done, you can upload the file into the [Sally Community](http://www.sally-project.org).


## FAQ ##

We know, that this tutorial is not complete. If you have any question contact us [webmaster@sally-project.org](mailto:webmaster@sally-project.org)