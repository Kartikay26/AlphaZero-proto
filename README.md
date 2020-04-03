# Project Omega

To run:

    $ cd build/
    $ cmake ..
    $ make
    $ ./omega
    $ ./omega | ../graph.py
    $ ./omega | awk '{print $1; fflush()}' | ../hist.py