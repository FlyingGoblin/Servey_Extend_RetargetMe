
IMAGE RETARGETING SURVEY SYSTEM
-------------------------------

This package contains the system we developed for conducting the image retargeting user study
reported in the paper:

Michael Rubinstein, Diego Gutierrez, Olga Sorkine and Ariel Shamir, A Coparative Study of Image
Retargeting, ACM Transactions on Graphics, Vol. 29(5) (Proceedings SIGGRAPH Asia 2010)

The code is written in HTML, PHP and javascript, and is free to use under the license agreement
below. We ask that you cite our paper if you use or extend any part of this code. Also note that the
code includes third-party libraries, for which the terms of use are subject to the owner's
discretion.

Please report any bugs or other issues.


VERSION HISTORY

v2.3 (2010-08-30) initial public release


REQUIREMENTS

The survey system is a web-based application, and requires an HTTP webserver and PHP to run.
The code was deveoped and tested on a Windows XP 32bit machine, running Apache server 2.2.15 with
PHP 5.2.13. The code also ran successfully on SunOS, running Apache 2.2.0 (Fedora) with PHP 5.1.6.
The official recommended browser is Firefox (>=3.5), although we have also tested it with Internet
Explorer and Google Chrome. No specific issues were reported on any browser, including Opera and
Apple's Safari.


INSTALLING

0. Install (if does not exist already) Apache HTTP server (http://httpd.apache.org/) and PHP
(http://www.php.net/) on the target machine. Both are fairly straightforward to install and
configure following their online documentation.
1. Unpack the zip file under the required directory, accessible to the webserver (e.g. C:\Program
Files\Apache Software Foundation\Apache2.2\htdocs on Windows).
2. IMPORTANT: make sure the webserver (typically www-data or www account on Unix machines) have full
write permissions to the survey directory (for the error log), as well as the 'bin', 'sessions' and
'tmp' subdirectories (for normal operation).


RUNNING

To test the installation, type in your browser: http://<SERVER_ADDRESS>/survey/index.php?mode=0
If all works well, you should see the first page of the survey which contains instructions for the
participants. It you don't see that page, try running http://<SERVER_ADDRESS>/survey/serverInfo.php
This should present general information on the server IF the webserver and PHP are installed and
configured properly. The system writes errors to the log file error_log.txt under the survey
directory, and you might want to consult this log as well for possible problems.

TODO: detail modes of operations, general description of the system structure, and output.


LICENSE

Copyright (c) 2010 Michael Rubinstein, Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.




















