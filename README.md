# mchanges
mchanges is a tool which generates csv report for all the changed methods/function body between two java files.</br>
</br>
| Command arguments | Function |
| ----------------- | -------- |
| --dir1  | Directory path     |
| --dir2  | Directory path     |
| --clist | Custom list (.txt) |

+ mchanges will iterate over all the java files in the directory, specified by **--dir1** and **--dir2**.
+ **--dir2** jave files will be compared with **--dir1**.
+ **--clist** instead of iterating over all the java files, you can specify selective java files.
+ File paths present in **.txt**(--clist), need to be relative to mentioned in **--dir2**.
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
</br>
Note:-</br>
Currently, inner class functions are ignored.</br>
Class name which is same as File name are only considered for the comparison.</br>
</br>
Future Dev:-</br>
Modify csv output.</br>
Generate report as HTML.</br>
Support for inner class functions comparison.</br>
Support for multiple java classes within java file.</br></br>

Windows:-</br>
x64 compiled code available [here](https://drive.google.com/file/d/1Wwk3OqHp688gk0veZNbIaMiQajtwGk4T/view?usp=sharing).