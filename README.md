sign
====

Digital Sign program written in qt. The program is intended for use with a raspberry pi for use as a digital sign to display content on a television or other display.

Installation:
====
Place the contents of the config folder into the .config directory of the user that will be executing the application.


Config file
====
serverUrl=https://yourserver.lan/mystuff	full path to the directory that contians your pages.
defaultPage=default.html	the default page to display if no others are available
defaultDuration=10	default length of time to stay on a page in seconds
serverConf=6.cfg	name of config file supplied by the server. contains the page name and duration for pages supplied by the server

logLevel=10	sets the verbosity of the program's output. Possible values: 0-10
logToFile=true	sets flag to write output to file. Possible values: true|false 


Server Config
====
The syntax of the server config follows the following format, any deviation will cause errata to occur and there is minimal error checking at present:

Syntax
Use page,duration pairs separated by a ';'. 
All entries must be on one line.
Do not place a ';' atfter the last entry.

Example: page1,30;page2,120,page3,60


Notes
====
Currently no log levels exist except for 0 or 10. Additional levels to come.
Currently the program only expects one line in the server config file. Additional lines are discarded.


Todo
====
Add addttional levels of verbosity
Add additional error checking for file I/O
Add additional information to readme
