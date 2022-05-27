# mchanges
mchanges is a tool which generates csv report for all the changed methods/functions body between two java files.</br>
For the comparison filenames needs to be same.</br>
| Command arguments | Function |
| ----------------- | -------- |
| --dir1  | Directory path     |
| --dir2  | Directory path     |
| --clist | Custom list (.txt) |

+ mchanges will iterate over all the java files in the directory, specified by **--dir1** and **--dir2**.
+ **--dir2** jave files will be getting compared with **--dir1**.
+ **--clist** instead of iterating over all the java files, you can specify selective java files in **.txt** file.
+ Filename paths present in **.txt** file needs to be relative to **--dir2**.
</br>
Examples:-</br>
To consider all the java files</br>

```
mchanges --dir1 /path/v1 --dir2 /path/v2
```


To consider specific java files</br>
```
mchanges --dir1 /path/v1 --dir2 /path/v2 --clist FileList.txt
```

Windows:-</br>
x64 pre-compiled code available [here](https://drive.google.com/file/d/11u1Wxwsm7Cnk2cTkEJmJMsSYH6tENqCI/view?usp=sharing).
