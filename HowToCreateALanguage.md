Sally has support for multiple languages. It's really easy to create a new language for sally. But first let us look at the language module of Sally and how it works.

## How to Start ##

You can download the 'translation starter package' here: [download](http://code.google.com/p/sally-project/downloads/detail?name=translation_starter_pack_1.9.zip)

Than rename the extracted folder to your language. You should keep **sally-project.language**. at the beginning and add the two characters for your iso code.

Your folder should now look like this **sally-project.language.de** (for german language)

## Creating your own language ##

Now you need to customize the files it self.

Change into your folder and you will find a hand of files files.

  * manifest.ini
  * sally-project.lang
  * ...

### manifest.ini ###

In the manifest.ini (located in the language folder) you will find informations about the language:

```
[info]
author=Christian Knobloch
name=Deutsch
version=1.0.0.1
uid=sally-project.language.de
isocode=de
```


| **Value** | **Description** |
|:----------|:----------------|
|author|The theme author|
|name|The language name (can be written in the destination language)|
|version|Version of the language file|
|uid|This is the uid which is the same as the folder name|
|isocode|The two characters ISOCode|

Everytime you change somthing to the language and upload it to the community you must change the version to a higher number. The uid should never change. If you need to change it, than also rename the folder name and we will than have a new language.

### The sally-project.lang file ###

The sally-project.lang is the sally main translation file. It can looks like this for the german language:

```
[lang]
Monday=Montag
Tuesday=Dienstag
Wednesday=Mittwoch
Thursday=Donnerstag
Friday=Freitag
Saturday=Samstag
Sunday=Sonntag
[...]
```


<font color='#22AA22'>
<h2>Info</h2>
<blockquote>You must alway keep the top line of the file like this: <a href='lang.md'>lang</a>
</font></blockquote>


As you can see on the left side of the equal sign you have the english wording. On the right side is the translation into your language. If you have a longer text then it is not allowed to have line breaks.

There are some special characters you must beware of. This special characters must be used in your translations:

| **Special Character** | **Meaning** |
|:----------------------|:------------|
|\n|Creates a line break: Test it\nNew Line=Testen\nNeue Zeile |
|%s|Is replaced with a string at runtime: We have found %s images=Wir haben %s Bilder gefunden|

This behaviour is the same for the sally .lang file and all plugin .lang files.

### What about all the other files? ###

All other files are for a specific application. For example: de.der-knob.sally.app.eradio.lang is the translation for the eRadio application. It's working like the sally-project.lang file.

When you create a translation, please translate at least these files:

  * de.der-knob.sally.app.community.lang
  * de.der-knob.sally.app.imagebrowser.lang
  * de.der-knob.sally.app.mediaplayer.lang
  * de.der-knob.sally.app.paintboard.lang

These are the main applications which are delivered with Sally.


<font color='#AA2222'>
<h2>Warning</h2>
<blockquote>Delete all not translated files!<br>
</font></blockquote>


## Create a Installation Package ##

When you are done, zip the folder. Than change the file extension from **.zip** to **.sallylanguage**.

Double click your new language package with the **.sallylanguage extension**. This should start the installer. Install the package and verify it in sally.

When you are done, you can upload the file into the [Sally Community](http://www.sally-project.org).

## FAQ ##

We know, that this tutorial is not complete. If you have any question contact us [webmaster@sally-project.org](mailto:webmaster@sally-project.org)