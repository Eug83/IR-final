import urllib.request
from html.parser import HTMLParser

MAXBYTE = 3000000


class NParser(HTMLParser):
	parsedData = []
	tagRemain = 0
	tagWeWant = ["p", "title", "div"]

	def handle_starttag(self, tag, attrs):
		#self.parsedData.append(("s", tag))
		#print("start:", tag)
		if (tag in self.tagWeWant):
			self.tagRemain = self.tagRemain + 1
		#print(self.tagRemain)
	
	def handle_endtag(self, tag):
		#self.parsedData.append(("e", tag))
		#print("end:", tag);
		if (tag in self.tagWeWant):
			self.tagRemain = self.tagRemain - 1
		#print(self.tagRemain)
	
	def handle_data(self, data):
		#self.parsedData.append(("d", data))
		#print("data:", data);

		if (self.tagRemain != 0):
			if ("script" in self.get_starttag_text()):
				return
			if ("title" in self.get_starttag_text()):
				print(data)
				return
			print(data)
			#print(self.get_starttag_text())


def getHTML(url):
	webPage = urllib.request.urlopen(url)
	return webPage.read(MAXBYTE).decode("utf-8")

def parseHTML(data):
	parser = NParser()
	parser.feed(data)
	parsedData = parser.parsedData



def getQuery(url):
	data = getHTML(url)
	content = parseHTML(data)
	#count(content)
	#list = makeQueryList()
	#return list

if __name__ == '__main__':
	#getQuery("http://hsing16.pixnet.net/blog/post/33292894")
	#getQuery("http://hsing16.pixnet.net/blog/post/32270925")
	#getQuery("http://travelmous2013.pixnet.net/blog/post/411878827")
	#getQuery("https://www.dcard.tw/f")