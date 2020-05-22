#!python.exe
# -*- coding: utf-8 -*-
'''Example introspecting application folder path.

USAGE
-----

Add the path to this script's folder to the Windows
$env:PYTHONPATH:

```powershell
$env:PYTHONPATH += "C:\cygwin64\home\mike\chromation\dev-kit-mike\firmware\python;"
```

(that is where my example-path.py script is)

Now run this script as a module:

```bash
$ python.exe -m example-path
```

'''

from pathlib import Path


if __name__ == '__main__':
    path = Path(__file__)
    print()
    print("path.cwd()\n"
        f"Launched {path.name} from:\n"
        f"  {path.cwd()}"
        )
    print("path.name\n"
        "Run script:\n"
        f"  {path.name}\n"
        )
    print("path.absolute()\n"
        f"Absolute path to {path.name} is:\n"
        f"  {path.absolute()}\n"
        )
    print("path.stem\n"
        "Module name is:\n"
        f"  {path.stem}\n"
        f"Run {path.name} as a MODULE with:\n"
        f"  $ python.exe -m {path.stem}\n"
        )
    print("path.parent\n"
        "Module folder:\n"
        f"  {path.parent}\n"
        )
