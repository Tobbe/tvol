tVol - A tiny volume controling module written by Tobbe
=======================================================

This module is supposed to control the main system volume using simple bang
commands. What sets this module apart from all other volume changing modules
is that this one can also get the current volume. This can be very useful
when scripting.

In many ways this module is just me experimenting. It's an experiment with
writing a module that's as small as possible with regards to file size, and
it's an experiment with writing a module that's really only useful when used
in scripts.

The first experiment has proven very successful. The release binary size is
only 4096 bytes with the CRT statically linked! This size is obtainable using
both vc8 and vc9. The second experiment didn't turn out as well. To require a
script for even the simplest of functionality doesn't seem like such a good
idea to me any more.


.rc settings and commands
-------------------------

There are no settings to write in step.rc (or any of the included files, like
theme.rc), the only thing you have to do there is to load the module, using
either

    LoadModule "$ModulesDir$tVol-0.4.dll"

or using the newer NetLoadModule

    *NetLoadModule tVol-0.4


!bang commands
--------------

!tVolSet <volume>
    Sets the system volume to the value specified with <volume>. Minimum volume
    is 0 and full volume is 65535.

!tVolGet
    This writes the current system volume to the eVar $tVolVolume$. So if you
    run !tVolGet and then !alert $tVolVolume$ you will get a message box with
    the current volume.

!tVolToggleMute
    Use this !bang to mute or unmute the sound. !tVolToggleMute mutes the
    system sound when it isn't muted and unmutes it when it is muted.


Version history
---------------

Tobbe 2006-10-20 tVol 0.1
    Released the first version, 0.1.

Tobbe 2007-01-06 tVol 0.2
    Changed !tVolMute [false] to !tVolToggleMute since that is more usefull
    to me.

Tobbe 2007-02-28 tVol 0.3
    Cleaned up source. I had function definitions for functions that didn't
    exist. Unregistered bangs on module quit.

Tobbe 2009-03-02 tVol 0.4
    Included the tlibc project in the tVol solution, and added lsapi.h and
    lsapi.lib from the LiteStep SDK to the tVol source folder. This should
    simplify compilation for everyone.
    Added return statements after all error messages to only show one error
    box for every error.
