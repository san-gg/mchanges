# mchanges
mchanges is a tool which generates csv report for all the changed methods/function body or newly added functions between two java files.</br>
</br>
| Command arguments | Function                                       |
| ----------------- | ---------------------------------------------- |
| --dir1            | Directory path                                 |
| --dir2            | Directory path                                 |
| --clist           | Custom list (.txt)                             |
| --method-changes  | Generates CSV reports for method changes       |
| --new-methods     | Generates CSV reports for newly added methods  |

+ mchanges will iterate over all the java files in the directory, (specified by **--dir1** and **--dir2**) and reports fuction body changes or reports newly added functions or both (based on the given options --method-changes, --new-methods).
+ **--dir2** jave files will be compared with **--dir1**.
+ **--clist** instead of iterating over all the java files, you can specify selective java files.
+ File paths present in **.txt**(--clist), needs to be relative to directory mentioned in **--dir2**.
+ If --method-changes or --new-methods neither of them is specified then by default function body changes is selected i.e --method-changes .
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

To get the report of newly added files for all the java files</br>

```
mchanges --dir1 /path/v1 --dir2 /path/v2 --new-methods
```

To get the report of newly added files and function body changes for specific java files</br>

```
mchanges --dir1 /path/v1 --dir2 /path/v2 --clist FileList.txt --method-changes --new-methods
```
</br>
Note:-</br>
Currently, inner classes are ignored.</br>
Class name which is same as File name are only considered.</br>
</br>
Future Dev:-</br>
Modify csv output.</br>
Generate report as HTML.</br>
Support for inner classes.</br>
Support for multiple java classes within java file.</br></br>

Windows:-</br>
x64 compiled code available [here](https://drive.google.com/file/d/1tLuv2SkUaUTU9HE829CBj9fgzIQRdYeb/view?usp=share_link).