def ver():
    import re, urllib2
    f = open("/home/eli/Downloads/.tmp/cridate/cri.cfg")
    current = f.read()
    versions = "CURRENT"+current+"ENDCURRENT..."
    latest = urllib2.urlopen("https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg").read()
    versions += "LATEST"+latest+"ENDLATEST"
    return versions
