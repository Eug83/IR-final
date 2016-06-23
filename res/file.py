import os
import sys

file_list='../data/file_list'

f = open(file_list,'w+',encoding='utf8',errors='ignore')

for dirPath, dirName, files in os.walk(sys.argv[1]):
	for file in files:
		if file.endswith(".lrc"):
			tmp = os.path.join(dirPath, file)
			tmp2 = tmp[4:len(tmp)]
			#file_name = tmp2.replace('\\',"/")
			f.write("%s\n" % (file_name))

f.close