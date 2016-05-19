import os
print os.system("if [! -e /root/Downloads/.tmp/NOROOT ]; then; return 1; else; return 0; fi")
