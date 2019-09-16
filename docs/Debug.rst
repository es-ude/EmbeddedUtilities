-----
Debug
-----

Debug is a header only library.
It provides macros that allow
to create debug output, that
can be completely removed via
compiler optimization. The
advantage lies in the fact
that the compiler still sees
the statements and one can
be sure the functions are
still used correctly even
when debugging was not enabled
for a long time.

The listed function declarations
have to be implemented by the
user.

How to use
----------

Use the library by listing the ``@EmbeddedUtilities//:Debug`` target
in the ``deps`` attribute of your ``cc_*`` rule.

Then include the header where needed::
  
  #include "EmbeddedUtilities/Debug.h"

The header file provides the ``debug``
macro. Writing::

  debug(MyType, variable);

will expand to::

  printMyType(variable);

Additionally the header lists ``printType(Type argument)`` function
declarations for commonly used types. As stated above Debug is a 
header only library. None of the function declarations ships
with an implementation currently. You will have to implement
every function yourself. There are plans to change this and
implement efficient print functions.

To enable the compiler to remove content of unneeded debug
output you should write the statements like this::

  debug(String, "My debug output");

Do not use the debug function for Strings to print other things
in a way like this::

  char some_text[32] = {0};
  sprintf(some_text, "My Message with int %i", some_int);
  debug(String, some_text);

Doing this will in most cases lead to the array and the
format string ending up in your program memory.
Instead do the following::

  debug(String, "My Message with int ");
  debug(UInt8, some_int);

and implement both corresponding print functions.

To enable debug output compile the corresponding ``*.c`` files
with the ``-DDEBUG=1`` flag.
To disable the output specify ``-DDEBUG=0`` instead.
Not setting the debug flag at all will result in a warning.
  

EmbeddedUtilities/Debug.h
------------

**#include "EmbeddedUtilities/Debug.h"**

.. doxygenfile:: EmbeddedUtilities/Debug.h
