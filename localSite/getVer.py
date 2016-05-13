def ver():
    import re, urllib2
    f = open("/root/Downloads/.tmp/cridate/cri.cfg")
    current = f.read()
    versions = current
    latest = urllib2.urlopen("https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg").read()
    versions += "\n"+latest
    return versions
