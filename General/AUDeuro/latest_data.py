#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 11 19:09:01 2023

@author: stratos
"""

import requests

url = "https://www.rba.gov.au/statistics/tables/xls-hist/2023-current.xls"

response = requests.get(url)
#response.content


with open ("2023-current.xls", "wb") as f:
   f.write(response.content)
