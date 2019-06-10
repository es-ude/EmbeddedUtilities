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

Util/Debug.h
------------

**#include "Util/Debug.h"**

.. doxygenfile:: Util/Debug.h
