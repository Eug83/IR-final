import urllib.request
from html.parser import HTMLParser

MAXBYTE = 3000000


class NParser(HTMLParser):
	_parsedData = ""
	_tagRemain = 0
	_tagWeWant = ["p", "title"]

	def handle_starttag(self, tag, attrs):
		if (tag in self._tagWeWant):
			self._tagRemain = self._tagRemain + 1
	
	def handle_endtag(self, tag):
		if (tag in self._tagWeWant):
			self._tagRemain = self._tagRemain - 1
	
	def handle_data(self, data):
		if (self._tagRemain != 0):
			#return if data is javascript
			if ("script" in self.get_starttag_text()):
				return
			#replace special char
			replace = [b'\n', b'\r', b'\t', b'\xc2\xa0']
			byte = str.encode(data)
			for r in replace:
				byte = byte.replace(r, b' ')
			data = byte.decode("utf-8")
			#title can have a high weight
			if ("title" in self.get_starttag_text()):
				#do something
				return
			self._parsedData = self._parsedData + data
	
	def getParsedData(self):
		return self._parsedData.replace(" ", "")


def getHTML(url):
	webPage = urllib.request.urlopen(url)
	return webPage.read(MAXBYTE).decode("utf-8")

def parseHTML(data):
	parser = NParser()
	parser.feed(data)
	parsedData = parser.getParsedData()
	print(parsedData)


def getQuery(url):
	data = getHTML(url)
	content = parseHTML(data)
	#count(content)
	#list = makeQueryList()
	#return list

if __name__ == '__main__':
	#JP food
	getQuery("http://hsing16.pixnet.net/blog/post/33292894")
	#steak
	#getQuery("http://hsing16.pixnet.net/blog/post/32270925")
	#make notebook
	#getQuery("http://travelmous2013.pixnet.net/blog/post/411878827")
	#for testing other webpage
	#getQuery("https://www.dcard.tw/f")