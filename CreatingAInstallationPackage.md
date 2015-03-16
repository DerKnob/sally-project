When you are done you can create a installation package to upload this to the [Sally Community](http://www.sally-project.org).

## Building a Release Version ##

Go to your Visual Studio.
First make a cleanup to your project, by clicking right on the project and than selection **Cleanup**.
Than change the target to **Release**.

![http://fileserver.sally-project.de/dev/wiki/ReleaseBuild.jpg](http://fileserver.sally-project.de/dev/wiki/ReleaseBuild.jpg)

Make sure that you have applied the project settings also the the **Release** build: [Creating the first Application](CreatingTheFirstApplication.md) section **Make the Project Settings**
Than build the project again.

## Creating the Archive ##

Go to your application directory and delete all files with the following extension, because the user don't need these files:

**.exp** .ilk
**.lib** .pdb

Than you can zip the folder (zip the complete folder - not only the content and change the file extension from **.zip** to **.sallyapplication**.

Double click your new application package with the **.sallyapplication extension**. This should start the installer. Install the package and verify it in sally.

When you are done, you can upload the file into the [Sally Community](http://www.sally-project.org).

<font color='#AA2222'>
<h2>Warning</h2>
<blockquote>Make sure you changed the target back to <b>Debug</b> when you are done!<br>
</font></blockquote>


## Other useful links ##

