def getApps():
    from subprocess import Popen, PIPE 
    import re
    CB = int(Popen("if [ -s /usr/local/bin/xiwi ]; then echo 1; else echo 0; fi", stdout=PIPE, shell=True).communicate()[0]) 
    if CB is not 1:
        icons = Popen("ls /var/www/html/localSite/images/icons/", stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0]
    else:
        icons = Popen("ls /var/www/images/icons/", stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0]
    coms = [] 
    bashList = 'for app in /usr/share/applications/*.desktop; do echo "${app:24:-8}"; done' 
    apps = Popen(bashList, stdout=PIPE, shell=True, executable="/bin/bash").communicate()[0]
    apps = apps.split('\n')
    apps.remove('')
    appLen = len(apps)
    f = open("/var/www/no.list")
    noList = f.read()
    noList = noList.split("\n")
    f.close()
    for app in apps:
        f = open('/usr/share/applications/'+app+'.desktop') 
        content = f.read()
        appName = re.findall(r'Name=(.*?)\n', content, re.DOTALL)
        appCom = re.findall(r'Exec=(.*?)\n', content, re.DOTALL)
        appIcon = re.findall(r'Icon=(.*?)\n', content, re.DOTALL)
        appCom = filter(None, appCom)
        appCom.append("None")
        if app in noList:
            continue
        if appCom[0] is "None":
            continue
        appCom.remove("None")
        for app,name,icon in zip(appCom,appName,appIcon):
            if "$@" in app:
                continue
            if app[-2] is ("%" or "$"):
                app = app[:-3]
            if icon+".png" not in icons:
                icon = "defIcon" 
            if icon is None:
                icon = "defIcon"
            coms.append(app+" : "+name+" : "+icon)
    return coms
