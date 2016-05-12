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
        if len(appCom) > 0:
            coms.append(appCom[0])
    coms = filter(None, coms)
    for i in range(len(coms)):
        if " " in coms[i]:
            coms[i] = coms[i][:-3]
        if coms[i][0] == "/":
            coms[i] = ""
    coms = filter(None, coms)
    return "\n".join(coms)
