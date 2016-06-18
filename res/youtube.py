#!/usr/bin/python
import webbrowser

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

def time_search(keyword_path, videoid, song_path):
  keyword_list=[]
  key_count=0
  with open(keyword_path,'r', encoding='big5') as keyword_f:
    for word in keyword_f:
      word=word.rstrip('\n')
      keyword_list.append(word)
      key_count = key_count+1
  #print ("%s" % keyword_list)
  with open(song_path,'r', encoding='big5') as song_f:
    find_word=0
    for line in song_f:
      #print("%s" % line)
      i=0
      while i < key_count:
        if keyword_list[i] in line:
          j=0
          for j in range(len(line)):
            if line.find("[", j) == -1:
              break
          sec=int(line[j+3:j+5])
          final_url=videoid+"&feature=youtu.be&t="+line[j+1]+"m"+str(sec)+"s"
          print ("%s" % final_url);
          webbrowser.open(final_url)
          find_word=1
          break
        i=i+1

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
      #if options.q[0] in title and options.q[1] in title:
      #  best_url=search_result["id"]["videoId"]
      #print ("%s" %title)
      if options.q[0] not in title or (options.q[1] not in title):
        tmp_result=3
      else:        
        #if 'mv' in title:
        #  tmp_result=1
        #elif 'music video' in title:
        #  tmp_result=1
        #else:
       
        tmp_result=0
     
      if tmp_result < best_result:
        best_url=search_result["id"]["videoId"]
        best_result=tmp_result

    #find result with singer and song name, no mv...
      if tmp_result==0:
        break
      
  return best_url  

if __name__ == "__main__":
  f = open('search.txt','r')
  keyword_path='keyword.txt'

  #set argparse
  argparser.add_argument("--q", help="Search term", default='')
  argparser.add_argument("--max-results", help="Max results", default=25)
  args = argparser.parse_args()

  #while loop until proper video is found
  find_song=0
  count=0
  while find_song==0 and count<10:
    search_var=f.readline()
    song_path=search_var.rstrip('\n')
    #get singer and song name
    #search_var is a list, search[0]=singer, search_var[1]=song name
    search_var=search_var.rstrip('.lrc\n')
    search_var=search_var.split('/')
    if len(search_var) > 2:
      length=len(search_var)
      search_var=search_var[length-2:length]

    #search by keyword
    args.q=search_var
    try:
      videoid=youtube_search(args)
      #check if video is found
      if videoid != "not found":
        videoid='https://www.youtube.com/watch?v='+videoid 
        find_song=1
        break

    except HttpError as e:
      print ("An HTTP error %d occurred:\n%s" % (e.resp.status, e.content))
 
  #get lrc time and combine url with time
  if find_song==1:
  
    #modify path for windows    
    #song_path=song_path.replace('/', '\\')
 
    time_search(keyword_path, videoid, "lrcc"+song_path)

  else:
    print("not found")
