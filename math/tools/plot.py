#!/usr/bin/python

# ULP error plot tool.
#
# Copyright (c) 2019, Arm Limited.
# SPDX-License-Identifier: MIT

import numpy as np
import matplotlib.pyplot as plt
import sys
import re

# example usage:
# build/bin/ulp -e .0001 log 0.5 2.0 2345678 | math/tools/plot.py

def fhex(s):
	return float.fromhex(s)

def parse(f):
	xs = []
	ys = []
	es = []
	# Has to match the format used in ulp.c
	r = re.compile(r'[^ (]+\(([^ )]*)\) got [^ ]+ want ([^ ]+) [^ ]+ ulp err ([^ ]+)')
	for line in f:
		m = r.match(line)
		if m:
			x = fhex(m.group(1))
			y = fhex(m.group(2))
			e = float(m.group(3))
			xs.append(x)
			ys.append(y)
			es.append(e)
		elif line.startswith('PASS') or line.startswith('FAIL'):
			# Print the summary line
			print(line)
	return xs, ys, es

def plot(xs, ys, es):
	if len(xs) < 2:
		print('not enough samples')
		return
	a = min(xs)
	b = max(xs)
	fig, (ax0,ax1) = plt.subplots(nrows=2)
	es = map(abs, es) # ignore the sign
	emax = max(es)
	ax0.text(a+(b-a)*0.7, emax*0.8, '%s\n%g'%(emax.hex(),emax))
	ax0.plot(xs,es,'rx')
	ax0.grid()
	ax1.plot(xs,ys,'rx')
	ax1.grid()
	plt.show()

xs, ys, es = parse(sys.stdin)
plot(xs, ys, es)
