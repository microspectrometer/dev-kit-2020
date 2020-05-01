#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Test hardware is visible.'''

from chromaspeclib.simple import ChromaSpecSimpleInterface
si = ChromaSpecSimpleInterface(serial_number='091103')
print(si)


