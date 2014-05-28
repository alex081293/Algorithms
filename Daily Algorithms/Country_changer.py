import os
import glob
import json
from pprint import pprint
path = "C:/Users/alexander.moran/Documents/GitHub/Secure/common/countries/iso3166-2"

fileNameList = []
secondList = []
i = 0
import os
for file in os.listdir(path):
	if file.endswith(".json"):
		openFile = path + "/" + file
		readFile = open(openFile, 'r')
		firstLine = readFile.readline()
		if firstLine == '[]':
			i += 1
			fileNameList.append(file)

print fileNameList

i = 0
path2 = "C:/Users/alexander.moran/Documents/GitHub/Secure/common/countries/iso3166"
for file in fileNameList:
	if os.path.isfile(path2 + "/" + file):
		secondList.append(file)
		i +=1

print secondList
