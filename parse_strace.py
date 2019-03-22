#!/usr/bin/env python

import sys 
import re

f = open("strace.log", "r")

lines=f.read().splitlines()
for line in lines:
	m = re.match("^[0-9]*  write\(3, \"([^\"]*)\", [0-9]*\) = [0-9]*$", line)
	if m:
		res = m.groups()[0]
		s = '"' + res + '"'
		print(s)
