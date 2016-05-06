def getApps():
    from subprocess import Popen, PIPE 
    import re
    coms = ""
    bashList = 'for app in /usr/share/applications/*.desktop; do echo "${app:24:-8}"; done' 
    apps = Popen(bashList, stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0]
    apps = apps.split('\n')
    apps.remove('')
    for i in range(len(apps)):
        f = open("/usr/share/applications/"+apps[i]+".desktop")
        content = f.read()
        content = re.findall('Exec=(.*?)\n', content, re.DOTALL)
        if content == (None or ""):
            pass
        print content[0]
        #coms = "\n".join(coms)

getApps()
