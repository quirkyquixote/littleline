#!/usr/bin/python

import sys
import re

c_comment = '/\*\*+(?:(?!\*/)(?!/\*)(?:\n|.))*\*/'
c_code = '(?:(?!\*/)(?!/\*)(?:\n|.))*'

c_ident = '(?<![_a-zA-Z0-9])[_a-zA-Z][_a-zA-Z0-9]*'
c_typename = c_ident+'(?:\s+'+c_ident+'|\s*\*)*'
c_function = c_typename+'\s*'+c_ident+' *\((?:.|\n)*\)\s*(?=[;{])'

c_variable = c_typename+'\s*'+c_ident+'\s*(?:\[(?:.|\n)*?\])*(?=\s*;)'

c_compound = '(struct|enum|union)\s+'+c_ident+'(?=\s*\{)'
c_anon_compound = '(struct|enum|union)\s+(?=\s*\{)'

c_defargs = '(?:'+c_ident+'(?:, *'+c_ident+')*)?'
c_define = '(?<=#define) +'+c_ident+'(?: *\( *'+c_defargs+' *\))?'

c_com_prefix = '(/\*+ ?| *\* ?)'
c_com_suffix = '\*/'

print """
API Documentation
=================
"""

for path in sys.argv[1:]:
    with open(path) as fd:
        text = fd.read()
        p = re.compile('('+c_comment+')('+c_code+')')
        p_com_prefix = re.compile('(?<![^\n])'+c_com_prefix)
        p_com_suffix = re.compile(c_com_suffix)
        p_function = re.compile('(?<![^\n])'+c_function)
        p_compound = re.compile('(?<![^\n])'+c_compound)
        p_anon_compound = re.compile('(?<![^\n])'+c_anon_compound)
        p_define = re.compile(c_define)
        p_variable = re.compile('(?<![^\n])'+c_variable)
        for match in p.finditer(text):
            print ''
            doc_prefix = ''
            prefix = '.. c:function:: '
            for m in p_function.finditer(match.group(2)):
                print prefix+m.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:type:: '
            for m in p_compound.finditer(match.group(2)):
                print prefix+m.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:type:: '
            for m in p_anon_compound.finditer(match.group(2)):
                print prefix+'Anonymous '+m.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:macro:: '
            for m in p_define.finditer(match.group(2)):
                print prefix+m.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            prefix = '.. c:var:: '
            for m in p_variable.finditer(match.group(2)):
                print prefix+m.group().replace('\n', ' ')
                prefix = '        '
                doc_prefix = '   '
            print ''
            for l in match.group(1).split('\n'):
                print p_com_prefix.sub(doc_prefix, p_com_suffix.sub('', l))

