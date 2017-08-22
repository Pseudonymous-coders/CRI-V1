def ver():
    import re, urllib2
    from subprocess import Popen, PIPE
    CB = int(Popen("if [ -s /usr/local/bin/xiwi ]; then echo 1; else echo 0; fi", stdout=PIPE, shell=True).communicate()[0]) 
    if not CB:
        f = open("/home/eli/Downloads/.tmp/cridate/cri.cfg")
    else:
        f = open("/root/Downloads/.tmp/cridate/cri.cfg")
    current = f.read()
    versions = current
    latest = urllib2.urlopen("https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg").read()
    versions += "\n"+latest
    f.close()
    return versions
