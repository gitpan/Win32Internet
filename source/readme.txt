
To build Internet.pll, you need:

	- the Perl5 source
	- Microsoft ActiveX SDK (or just WININET.H and WININET.LIB...)

The two makefiles are:
	INTERNET.MAK    for Perl-Win32 version 5.001 (build 110)
	INTERNET300.MAK for Perl-Win32 version 5.003 (build 300 and higher)

You have to place the content of this directory in:

	(perl5)\dll-src\ext\win32\internet (for version 5.001) 
or
        (perl5)\ext\win32\internet         (for version 5.003)

where (perl5) is the path to your Perl5 source.

I've built it using Microsoft VC++ 4.1; please let me know if you have 
(or don't have) problems with other versions/compilers.

Send any comment to:

	Aldo Calpini	mailto:dada@divinf.it

