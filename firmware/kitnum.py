#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import datetime as dt
t0 = dt.date(year=2017, month=1, day=25)
today = dt.date.today()
days_since_t0 = (today-t0).days
print(f"{days_since_t0:04}")
