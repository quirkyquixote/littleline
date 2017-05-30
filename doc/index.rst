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

A table with readline-like key bindings assuming ANSI escape sequences is
provided by default, and has the following:

<dl>
<dt>C-a</dt>    <dd>Move to the beginning of the current line</dd>
<dt>C-b</dt>    <dd>Move back a character</dd>
<dt>C-c</dt>    <dd>Kill the process</dd>
<dt>C-d</dt>    <dd>Delete the character under the cursor</dd>
<dt>C-e</dt>    <dd>Move to the end of the current line</dd>
<dt>C-f</dt>    <dd>Move forward one character</dd>
<dt>C-h</dt>    <dd>Same as C-b C-d</dd>
<dt>C-j</dt>    <dd>Push line to the history and return it</dd>
<dt>C-k</dt>    <dd>Kill the text from the cursor to the end of the line</dd>
<dt>C-n</dt>    <dd>Move forward through the history</dd>
<dt>C-p</dt>    <dd>Move back through the history</dd>
<dt>C-q</dt>    <dd>NOT A BINDING: disables console output</dd>
<dt>C-s</dt>    <dd>NOT A BINDING: enables console output</dd>
<dt>C-u</dt>    <dd>Kill the text from the beginning to the line to the cursor</dd>
<dt>C-v</dt>    <dd>Add the next character to the line verbatim</dd>
<dt>C-w</dt>    <dd>Kill the word before the cursor</dd>
<dt>C-y</dt>    <dd>Yank the last killed test into the line</dd>

<dt>M-b</dt>    <dd>Move backward a word</dd>
<dt>M-f</dt>    <dd>Move forward a word</dd>

<dt>Up</dt>     <dd>Move back through the history, same as C-p</dd>
<dt>Down</dt>   <dd>Move forward through the history, same as C-n</dd>
<dt>Left</dt>   <dd>Move back a character, same as C-b</dd>
<dt>Right</dt>  <dd>Move right a character, same as C-f</dd>
<dt>Backsp.</dt><dd>Erase previous character, same as C-b C-d</dd>
<dt>Delete</dt> <dd>Delete the character under the cursor, same as C-d</dd>
<dt>Home</dt>   <dd>Move to the beginning of the current line, same as C-a</dd>
<dt>End</dt>    <dd>Move to the end of the current line, same as C-b</dd>
<dt>Return</dt> <dd>Push line to the history and return it, same as C-j</dd>
</dl>

Requirements
------------

Just ANSI C.

Installation
------------

LittleLine provides a simple make script; to build from scratch::

    make all
    make install

Contents
========

.. toctree::
   :maxdepth: 2

   apidoc
   usage


Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

