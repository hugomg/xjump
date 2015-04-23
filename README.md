# FALLING TOWER Version 2.9

A game where you jump up and up while the tower collapses under your feet.

## Installation

If you performed a git clone instead of downloading the source tarball,
see the Development section of this README for instructions on how to
generate the configure script.

Once you have the configure script, install xjump using the usual
configure + make install pattern:

    ./configure
    make
    sudo make install
    
Run `./configure -- help` for a list of configuration options to tweak
install locations, etc.
    
To allow xjump to update the highscore file on behalf of regular user,
your system must have a user and group namd `games`. We expect this to
be standard in Linux distributions.

## Game controls

To move: arrow keys, WASD, IJKL or the keypad arrow keys  
To jump: Up or Down arrow keys, Z or SPACE  
To pause and unpause: P or the Pause keypad.  
To exit the game: Shift+Q  

## Configuring xjump

To configure xjump create a configuration file at $HOME/.config/xjump.config
You can use the following keys during configuration:

* themeFile : Path to a custom sprite file. Ex.: /usr/share/xjump/themes/jumpnbump.xpm
* theme : Name of a custom sprite theme bundled with xjump. Ex.: jumpnbump
* highscoreFile : Path to a different highscore file

Here is an example of an xjump configuration file:

    # This is a comment
    theme = jumpnbump
    highscoreFile = /home/me/xjumpscore

## Custom graphics

Xjump can use custom textures in xpm format. The required dimensions are 144x64.

In the following diagram, each letter represents a 16x16 pixel area.

    11223344A
    11223344B
    55667788C
    55667788D

Legend:
  
    1 = Standing, moving left  (1/2)
    2 = Standing, moving right (1/2)
    3 = Standing, moving left  (2/2)
    4 = Standing, moving right (2/2)
    5 = Going up, moving left
    6 = Going up, moving right
    7 = Falling,  moving left
    8 = Falling,  moving right
    
    A = Wall background
    B = Left column
    C = Right column
    D = Platform

For examples, see the themes folder.

## Development

To generate the configure script and the makefile templates you must have
the GNU autotools installed and run autoreconf:

    autoreconf --install

If you want to delete all thoss files the autotools generated, the best way 
is via Git itself:

    git clean -xd
