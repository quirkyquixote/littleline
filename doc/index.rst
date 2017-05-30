.. Zebu documentation master file, created by
   sphinx-quickstart on Sat May 27 14:12:47 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

LittleLine - a Poor Person's ReadLine
=====================================

This library is a poor person's clone of the GNU readline library, originally
coded because I needed it in a context where I wasn't allowed to GPL.

It doesn't implement everything readline does: doesn't have undo functionality
and instead of a kill ring, there is a single string clipboard.

All customization must be done directly though C by passing data to the library
functions. It has a customizable prompt, size of the history, the option to
load and save the history from a file, and keyboard bindings that can be passed
at creation time from a C table.

Requirements
------------

Just ANSI C.

Installation
------------

LittleLine provides a simple make script; to build from scratch::

    make all
    make install

Default Key Bindings
--------------------

A table with readline-like key bindings assuming ANSI escape sequences is
provided by default, and has the following:

C-a
        Move to the beginning of the current line
C-b
	Move back a character
C-c
	Kill the process
C-d
	Delete the character under the cursor
C-e
	Move to the end of the current line
C-f
	Move forward one character
C-h
	Same as C-b C-d
C-j
	Push line to the history and return it
C-k
	Kill the text from the cursor to the end of the line
C-n
	Move forward through the history
C-p
	Move back through the history
C-q
	NOT A BINDING: disables console output
C-s
	NOT A BINDING: enables console output
C-u
	Kill the text from the beginning to the line to the cursor
C-v
	Add the next character to the line verbatim
C-w
	Kill the word before the cursor
C-y
	Yank the last killed test into the line

M-b
	Move backward a word
M-f
	Move forward a word

Up
	Move back through the history, same as C-p
Down
	Move forward through the history, same as C-n
Left
	Move back a character, same as C-b
Right
	Move right a character, same as C-f
Backsp.
        Erase previous character, same as C-b C-d
Delete
	Delete the character under the cursor, same as C-d
Home
	Move to the beginning of the current line, same as C-a
End
	Move to the end of the current line, same as C-b
Return
	Push line to the history and return it, same as C-j

Contents
========

.. toctree::
   :maxdepth: 2

   apidoc


Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

