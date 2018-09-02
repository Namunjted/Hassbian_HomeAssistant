1
2
3
4
5
6
7
8
9
10
#!/bin/bash
 
git add .
git status
echo -n "Enter the Description for the Change: " 
read MSG
git commit -m "$MSG"
git push origin master 
 
exit
