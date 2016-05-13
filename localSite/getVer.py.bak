def ver():
    import re,urllib2
    finals = {}
    finals['versions'] = {}
    finals['versions']['current'] = {}
    finals['versions']['latest'] = {}
    f = open("/home/eli/Downloads/.tmp/cridate/cri.cfg")
    criCfg = f.read()
    finals['versions']['current']['ver'] = re.findall(r'VERSION(.*?)ENDVERSION', criCfg, re.DOTALL)[0]
    finals['versions']['current']['release'] = re.findall(r'NAME(.*?)ENDNAME', criCfg, re.DOTALL)[0]
    finals['versions']['current']['date'] = re.findall(r'DATE(.*?)ENDDATE', criCfg, re.DOTALL)[0]
    content = urllib2.urlopen("https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg").read()
    finals['versions']['latest']['ver'] = re.findall(r'VERSION(.*?)ENDVERSION', content, re.DOTALL)[0]
    finals['versions']['latest']['release'] = re.findall(r'NAME(.*?)ENDNAME', content, re.DOTALL)[0]
    finals['versions']['latest']['date'] = re.findall(r'DATE(.*?)ENDDATE', content, re.DOTALL)[0]
    if finals['versions']['current']['ver'] != finals['versions']['latest']['ver']:
        finals['update'] = "Yes" 
    else:
        finals['update'] = "No" 
    return finals
