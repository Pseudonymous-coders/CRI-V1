def getApps():
    from subprocess import Popen, PIPE 
    import re
    coms = [] 
    bashList = 'for app in /usr/share/applications/*.desktop; do echo "${app:24:-8}"; done' 
    apps = Popen(bashList, stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0]
    apps = apps.split('\n')
    apps.remove('')
    appLen = len(apps)
    for app in apps:
       f = open('/usr/share/applications/'+app+'.desktop') 
       content = f.read()
       appCom = re.findall(r'Exec=(.*?)\n', content, re.DOTALL)
       coms.append(app)
    coms = filter(None, coms)
    for app in coms:
        app.split(" ")
        app = app[0]
    coms = "\n".join(coms)
    return coms

