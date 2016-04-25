def BBG():
    import urllib2
    import json
    import time
    from os import system

    def internet_on():
        try:
            response=urllib2.urlopen('http://google.com',timeout=1)
            return True
        except urllib2.URLError as err: pass
        return False
    while 1:
        if internet_on():
            response = urllib2.urlopen('http://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US')
            data = json.load(response)
            url = "http://bing.com"+str(data['images'][0]['url'])
            system('wget -q '+url+' -O images/header.jpg')
        time.sleep(10800)
