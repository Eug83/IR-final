import os
import sys
import time

dataPath='./lrcc'
vocab='vocab'
invIndex='invIndex'
fileListPath='./file_list'
term_dict=dict()
invInd_dict=dict()
term_list=[]
file_dict=dict()
file_list=[]


def read_filelist():

	global fileListPath,file_dict

	count=0
	with open(fileListPath,'r',encoding='utf8') as fp:
		for line in fp:
			line=line.strip()
			file_dict[line]=count
			file_list.append(line)
			count=count+1

	return


def check_term(f,s,termCount):

	global term_dict,term_list,invInd_dict

	if s not in term_dict:
		term_dict[s]=termCount
		term_list.append(s)
		termCount += 1
		invInd_dict[s]=dict()
		invInd_dict[s][f]=1
	else:
		if f not in invInd_dict[s]:
			invInd_dict[s][f]=1
		else:
			invInd_dict[s][f] += 1

	return termCount


def build():

	global dataPath,file_list

	termCount=0

	for f in file_list:
		path = dataPath+f
		path=path.strip()
		with open(path,'r',encoding='big5',errors='ignore') as fp:
			firstFlag=0
			for line in fp:
				line=line.split(']')
				line=line[-1]
				if line in ['\n','\r\n']:
					continue
				line=line.strip()
				if firstFlag==0 and ('詞' in line  and '曲' in line):
					firstFlag=1
					continue
				if 'http' in line:
					continue
				elif '精品網頁' in line:
					continue
				elif 'www' in line:
					continue
				line=line.split(' ')
				for i in range(len(line)):
					s,flag='',0
					for c in line[i]:
						if str(c).isalpha() or str(c).isdigit():
							if c >= u'\u4e00' and c <= u'\u9fff':
								if flag==0:
									termCount=check_term(f,str(c),termCount)
								else:
									termCount=check_term(f,s,termCount)
									termCount=check_term(f,str(c),termCount)
									s,flag='',0
							else:
								s += str(c)
								flag=1
						else:
							if not s=='':
								termCount=check_term(f,s,termCount)
								flag,s=0,''
					if not s=='':
						termCount=check_term(f,s,termCount)

	return


def write_file():

	global dataPath,vocab,invIndex,term_dict,invInd_dict,term_list,file_dict,file_list
	
	with open(vocab,'w',encoding='utf8') as fp:
		for x in term_list:
			fp.write('%s\n' % (x))

	with open(invIndex,'w',encoding='utf8') as fp:
		for term in term_list:
			fp.write('%d -1 %d\n' % (term_dict[term],len(invInd_dict[term])))
			for key in invInd_dict[term]:
				fp.write('%d %d\n' % (file_dict[key],invInd_dict[term][key]))

	return


def main():

	print('Start processing...')
	startTime=time.time()
	print('Start reading file list...')
	read_filelist()
	print('Finish reading in %s seconds' % (time.time()-startTime))
	build()
	print('Finish building in %s seconds' % (time.time()-startTime))
	write_file()
	print('Finish processing in %s seconds' % (time.time()-startTime))

	return

if __name__=='__main__':
	main()
