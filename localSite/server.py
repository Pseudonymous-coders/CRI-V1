import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import socket
import os
import sys
from subprocess import Popen, PIPE, STDOUT
import urllib2
import json
import re
import sqlite3
import time
from threading import Thread, Timer 
from getApps import getApps
from getVer import ver


CB = int(Popen("if [ -s /usr/local/bin/xiwi ]; then echo 1; else echo 0; fi", stdout=PIPE, shell=True).communicate()[0]) 

if CB:
    NOROOT = os.path.isfile("/root/Downloads/.tmp/NOROOT")
else:
    NOROOT = os.path.isfile("/home/eli/Downloads/.tmp/NOROOT")


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
            if CB is not 1:
                os.system('wget -q '+url+' -O /var/www/html/localSite/images/header.jpg')
            else:
                os.system('wget -q '+url+' -O /var/www/images/header.jpg')

            print "Got new BG Image"
            time.sleep(3600*1)
        else:
            time.sleep(3600)
'''def installer(self, pkg):
    Popen("echo y | apt-get -qq install "+pkg, shell=True, executable="/bin/bash")
    print "Done installing: "+pkg
    self.write_message("DONEINSTALL")
'''
class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print 'new connection'

    def on_message(self, message):
        print 'message received:  %s' % message
        if NOROOT:
            self.write_message("NOROOT")
            print "NO ROOT!!! Sending message..."

        if message == "connecred":
            self.write_message("connected")

        if message == "APPLIST":
            if NOROOT:
                self.write_message("NOROOT")
            else:
                print "Sending APPLIST"
                apps = getApps() 
                self.write_message("\n".join(apps)) 

        if message[:3] == "RUN":
            print "running: "+message[3:]
            if not CB:
                Popen(message[3:], shell=True, executable="/bin/bash")
            else:
                Popen("xiwi "+message[3:], shell=True, executable="/bin/bash")
        if message[:7] == "INSTALL":
            print "installing: "+message[7:]
            os.system("echo y | apt-get -qq install "+message[7:])
            print "Done installing: "+message[7:]
            self.write_message("DONEINSTALL")
            # install = Thread(target=installer, args=(self, message[7:],))
            # install.start()

        if message[:6] == "SEARCH":
            toSearch = 'apt-cache search '+message[6:]
            searches = str(Popen('apt-cache search '+message[6:], stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0])
            self.write_message(searches)

        if message[:6] == "REMOVE":
            print "removing: "+message[6:]
            os.system("echo y | apt-get remove "+message[6:])
            print "Done removing "+message[6:]
            print "Sending Applist"
            apps = getApps() 
            self.write_message("REMOVED")
            self.write_message("\n".join(apps)) 

        
        if message[:3] == "VER":
            print "Sending versions"
            self.write_message("VER"+str(ver())) 
        
        if message[:7] == "STOPALL":
            print "Stopping all apps"
            os.system("pkill x")
            print "Stopped all apps"

        if message[:6] == "ADDPPA":
            print "Adding PPA: "+message[6:]
            Popen("echo \n | add-apt-repository "+message[6:])
            print "Added PPA"
            self.write_message("PPADONE")

        if message[:6] == "ADDDEB":
            print "Installing Deb: "+message[6:]
            os.system("debins /root/Downloads/")+message[6:]
            print "Installed Deb"
            self.write_messsage("DEBDONE")

        if message == "UPDATEAPPS":
            print "Updating apps"
            os.system("apt-get update")
            os.system("echo y | apt-get upgrade")
            self.write_message("DONEUPAPP")
            print "Done updating apps"

        if message == "UPDATECRI":
            print "Updating CRI"
            process = Popen("updatecri", shell=True, stdout=PIPE, stderr=STDOUT, executable="/bin/bash")
            while True:
                    nextline = process.stdout.readline()
                    if nextline == '' and process.poll() is not None:
                        self.write_message("FAILUPDATE")
                        break
                    if "DONE" in nextline:
                        break
                    try:
                        perc = int(nextline)
                    except ValueError:
                        continue
                    print "Updating: "+str(perc)+"%"
                    self.write_message("PERC"+str(perc))
                    sys.stdout.flush()

            self.write_message("DONEUPDATE")

        if message[:7] == "CRIMOVE":
           if CB:
                f = open("/root/Downloads/.tmp/REMOVE", "w")
           else:
                f = open("/home/eli/Downloads/.tmp/REMOVE", "w")
           f.write("REMOVE")
           f.close()

        if message[:9] == "CRINSTALL":
            if CB:
                f = open("/root/Downloads/.tmp/REINSTALL", "w")
            else:
                f = open("/home/eli/Downloads/.tmp/REINSTALL", "w")
            f.write("REINSTALL")
            f.close()


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
