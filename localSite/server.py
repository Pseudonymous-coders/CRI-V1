import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import socket
import os
import sys
from subprocess import Popen, PIPE
import urllib2
import json
import re
import sqlite3
import time
from threading import Thread, Timer 
from getApps import getApps

def BGImg():
    def internet_on():
        try:
            response=urllib2.urlopen("http://google.com", timeout=1)
            return True
        except urllib2.URLError as err: pass
        return False
    while 1:
        if internet_on():
            response = urllib2.urlopen('http://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US')
            data = json.load(response)
            url = "http://bing.com"+str(data['images'][0]['url'])
            os.system('wget -q '+url+' -O /var/www/images/header.jpg')
            print "Got new BG Image"
            time.sleep(3600*3)
        else:
            time.sleep(3600)
def installer(self, pkg):
    Popen("echo y | apt-get install "+pkg, shell=True, executable="/bin/bash")
    print "Done installing: "+pkg
    self.write_message("DONEINSTALL")

class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print 'new connection'

    def on_message(self, message):
        print 'message received:  %s' % message
        if message == "APPLIST":
            print "Sending APPLIST"
            apps = getApps() 
            self.write_message(apps) 

        if message[:3] == "RUN":
            print "running: "+message[3:]
            #Popen("gtk-launch "+message[3:]+".desktop", shell=True, executable="/bin/bash")
            Popen("xiwi "+message[3:], shell=True, executable="/bin/bash")
        if message[:7] == "INSTALL":
            print "installing: "+message[7:]
            install = Thread(target=installer, args=(self, message[7:],))
            install.start()

        if message[:6] == "SEARCH":
            toSearch = 'apt-cache search '+message[6:]
            searches = str(Popen('apt-cache search '+message[6:], stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0])
            self.write_message(searches)

        if message[:6] == "REMOVE":
            print "removing: "+message[6:]
            os.system("echo y | apt-get remove "+message[6:])
            print "Done removing "+message[6:]

    def on_close(self):
        print 'connection closed'

    def check_origin(self, origin):
        return True

    def do_message(self, message):
        os.system(message)
application = tornado.web.Application([
    (r'/ws', WSHandler),
])

thread = Thread(target=BGImg)
thread.daemon = True
thread.start()
http_server = tornado.httpserver.HTTPServer(application)
http_server.listen(9098)
myIP = socket.gethostbyname(socket.gethostname())
print '*** Websocket Server Started at %s***' % myIP
tornado.ioloop.IOLoop.instance().start()
