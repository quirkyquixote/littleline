#!/usr/bin/python

import sys
import re

c_comment = '/\*\*+(?:(?!\*/)(?!/\*)(?:\n|.))*\*/'
c_code = '(?:(?!\*/)(?!/\*)(?:\n|.))*'

c_ident = '(?<![_a-zA-Z0-9])[_a-zA-Z][_a-zA-Z0-9]*'
c_typename = c_ident+'(?:\s+'+c_ident+'|\s*\*)*'
c_funarg = c_typename+'\s*'+c_ident
c_funargs = '(?:void|'+c_funarg+'(?:,\s*'+c_funarg+')*)'
c_function = c_typename+'\s*'+c_ident+' *\(\s*'+c_funargs+'\s*\)'

c_variable = c_typename+'\s*'+c_ident+'(?=\s*;)'

c_struct = 'struct\s+'+c_ident+'(?=\s*\{)'

c_defargs = '(?:'+c_ident+'(?:, *'+c_ident+')*)?'
c_define = '(?<=#define) +'+c_ident+' *\( *'+c_defargs+' *\)'

c_comprefix = '(/\*+ ?| *\*/| *\* ?)'

print """
API Documentation
=================
"""

for path in sys.argv[1:]:
    with open(path) as fd:
        text = fd.read()
        p = re.compile('('+c_comment+')('+c_code+')')
        p0 = re.compile('(?<![^\n])'+c_comprefix)
        p1 = re.compile('(?<![^\n])'+c_function)
        p2 = re.compile('(?<![^\n])'+c_struct)
        p3 = re.compile(c_define)
        p4 = re.compile('(?<![^\n])'+c_variable)
        for match in p.finditer(text):
            print ''
            doc_prefix = ''
            prefix = '.. c:function:: '
            for m1 in p1.finditer(match.group(2)):
                print prefix+m1.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:type:: '
            for m2 in p2.finditer(match.group(2)):
                print prefix+m2.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:macro:: '
            for m3 in p3.finditer(match.group(2)):
                print prefix+m3.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:var:: '
            for m4 in p4.finditer(match.group(2)):
                print prefix+m4.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            for l in match.group(1).split('\n'):
                print p0.sub(doc_prefix, l)

