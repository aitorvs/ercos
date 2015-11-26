# ERCOS
ERCOS is an operating system for embedded platforms. I developed originally for the Sparc architecture, specifically for ERC-32 processor which is commonly used in spacecrafts.
Later on, support for x86 was added.

The project is currently not updated anymore but I make the source code available as I think it might be of interest for people wanting to know operating system internals.

## Configuration

The operating system comes with a graphical user interface based on Linux Tk tools to get it configured. The interface is very simple and allows the user to select and configure the target processor/architecture.

The command to configure the kernel is simply ```make xconfig```

Depending on the architecture, some configuration options may vary but in general it is possible to configure:

- Architecture dependen options
- Load options. Allows to specify where the binay will be linked
- Debug options
- Kernel configuraiton options. Settings specific to the OS kernel, like numer of priorities, kernel stack size, etc
- Memory management, enable/disable MMU if supported by the architecture
- Posix interface. Allows to include a Posix API interface
- Kernel trace options. Include kernel system trace information
- Tests. Some tests can be compiled ready to execute on the target platform

## Compilation

Once the configuration is performed just run ```make``` command and OS will be compiled.

The compilation will place the kernel library inside ```image``` directory called ```libercos.a```.

If a test was also compiled, the image ready to uploaded to the target processor will also be placed inside ```image``` folder.

## Porting ERCOS

I make the source code available more as reference code for people who want to know about the internals of an operating system but, however it is possible to port ERCOS to any other platform.

To port ERCOS:

1. Edit the config.in file and inside ```choice Vendor/Product``` add the following line

```YOURPLATFORM CONFIG_DEFAULTS_PLATFORM_NEWARCHNAME_NEWPLATFORMNAME \ ```

and substitude ```YOURPLATFOR``` and ```NEWARCHNAME``` and ```NEWPLATFORMNAME``` with the correct name values.

2. Add the configuration menu specific to the architecture
 - create a sub-folder ```config/<platformName>```
 - create three files, ```config.in```, ```defconfig``` and ```link_<platformName>.ld```. The first contains the configuration menus for your platform.
 The second contains the default configuration (not mandatory). The third will be the link script for the linker.
 Take the ```config/DUMMY``` folder as example

3. create a directory with the platform-specific source code.
  ```
  $ mkdir -p arch/newArch/newPlatform
  ```
  
  Each directory must have a ```Makefile```. You can use ```arch/NOARCH``` as example or what needs to be coded.
  

The project is no longer being updated and I have no plans to do it but, if you have any questions please ask.
  
Developed By
--------------------

Aitor Viana Sanchez - aitor.viana.sanchez@gmail.com>

<a href="https://twitter.com/aitorvs">
  <img alt="Follow me on Twitter"
       src="https://raw.github.com/ManuelPeinado/NumericPageIndicator/master/art/twitter.png" />
</a>
<a href="https://plus.google.com/+AitorViana">
  <img alt="Follow me on Twitter"
       src="https://raw.github.com/ManuelPeinado/NumericPageIndicator/master/art/google-plus.png" />
</a>
<a href="https://www.linkedin.com/in/aitorvs">
  <img alt="Follow me on Twitter"
       src="https://raw.github.com/ManuelPeinado/NumericPageIndicator/master/art/linkedin.png" />


License
-----------

    Copyright 2013 Aitor Viana

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
