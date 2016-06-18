#!/usr/bin/python

from apiclient.discovery import build
from apiclient.errors import HttpError
from oauth2client.tools import argparser

# Set DEVELOPER_KEY to the API key value from the APIs & auth > Registered apps
# tab of
#   https://cloud.google.com/console
# Please ensure that you have enabled the YouTube Data API for your project.

DEVELOPER_KEY = "AIzaSyAAWsdajiwk9v84_ZctzmFbqnmXADVTBxM"
YOUTUBE_API_SERVICE_NAME = "youtube"
YOUTUBE_API_VERSION = "v3"

def time_search(keyword_path, lrc_path, videoid):
  keyword_list=[]
  key_count=0
  with open(keyword_path,'r', encoding='big5') as keyword_f:
    for word in keyword_f:
      keyword_list.append(word)
      key_count = key_count+1
      
  with open(lrc_path,'r', encoding='big5') as lrc_f:
    find_word=0
    time_list=[]
    for line in lrc_f:
      #print("%s" % line)
      for i in range(len(keyword_list)):
        if keyword_list[i] in line:
          for j in range(len(line)):
            j=line.find("[")
          print("%d" % j)
          print ("https://www.youtube.com/watch?v=%s&feature=youtu.be&t=%sm%ss\n" % (videoid, line[2], line[4:6]));
          find_word=1
          break
      if find_word==1:
        break

  return



def youtube_search(options):
  youtube = build(YOUTUBE_API_SERVICE_NAME, YOUTUBE_API_VERSION,
    developerKey=DEVELOPER_KEY)

  # Call the search.list method to retrieve results matching the specified
  # query term.
  search_response = youtube.search().list(
    q=options.q,
    part="id,snippet",
    maxResults=options.max_results
  ).execute()

  videos = []

  # Add each result to the appropriate list, and then display the lists of
  # matching videos, channels, and playlists.
  best_result=3
  best_url='not found'
  for search_result in search_response.get("items", []):
    if search_result["id"]["kind"] == "youtube#video":
      #videos.append("%s (%s)" % (search_result["snippet"]["title"],search_result["id"]["videoId"]))  
      title=search_result["snippet"]["title"].lower()
      #print ("%s %s" % (title, options.q))
      if options.q[0] not in title or (options.q[1] not in title):
        tmp_result=3
      else:
        if 'MV' in title:
          tmp_result=1
        elif 'music video' in title:
          tmp_result=1
        else:
          tmp_result=0

      if tmp_result < best_result:
        best_url=search_result["id"]["videoId"]
        best_result=tmp_result

    #find result with singer and song name, no mv...
      if tmp_result==0:
        break
    #print ("https://www.youtube.com/watch?v=%s %d" % (best_url, best_result))
  return best_url  
  #print ("Videos:\n", "\n".join(videos), "\n")


if __name__ == "__main__":
  f = open('search.txt','r')
  keyword_path='keyword.txt'
  lrc_path='Ask  For  More.lrc'

  #set argparse
  argparser.add_argument("--q", help="Search term", default='')
  argparser.add_argument("--max-results", help="Max results", default=25)
  args = argparser.parse_args()

  #while loop until proper video is found
  find_song=0
  count=0
  while find_song==0 and count<10:
    search_var=f.readline()
    #get singer and song name
    #search_var is a list, search[0]=singer, search_var[1]=song name
    search_var=search_var.rstrip('\n')
    search_var=search_var.split('/')
    if len(search_var) > 2:
      length=len(search_var)
      search_var=search_var[length-2:length]
    #print ("%s" % search_var)

    #search by keyword
    args.q=search_var
    try:
      videoid=youtube_search(args)
      #check if video is found
      if videoid != "not found":
        videoid='https://www.youtube.com/watch?v='+videoid 
        print("%s" % videoid)
        find_song=1
        break

    except HttpError as e:
      print ("An HTTP error %d occurred:\n%s" % (e.resp.status, e.content))
 
  #get lrc time and combine url with time
  #if find_song==1:
  #  time_search(keyword_path, lrc_path, videoid)
  #else:
   # print("not found")
