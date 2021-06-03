# ProjectLibrary
# 这是第一次上传的2048项目,后续会上传其他项目.
# 项目语言以C/C++/C#/Python为主.
# 具体的项目介绍以及使用流程后续有空再说明.
# 欢迎关注,拉取,留言.
# CSDN博客网址:https://www.blog.csdn.net/zhd5120153951
# github地址: https://www.github.com/zhd5120153951
# 第二次上传日期为:2021.6.3;本次记录下一git如何把不同的项目,上传到同一个远端仓库
具体流程如下:
①在本地仓库创建好项目,或者把原项目建成git版本控制的本地仓库.
②打开git bash,cd到当前目录
③如果没有创建git,则先使用git init初始化当前本地仓库(此时该目录下会多出一个.git文件--这个是git版本控制的相关文件)
④在远端仓库创建一个仓库,或者已经创建好了,并且复制下该仓库的地址(一般是:https://github.com/..........)
⑤把本地仓库和GitHub远端仓库关联起来--指令为:git remote add origin https://github.com/zhd5120153951/ProjectLibrary.git
⑥此时可以查看添加了哪些文件,git status;显示没有添加则用git add添加对应的文件或者文件夹
⑦添加后,本地仓库已经更新了,这时可以把本地仓库内容提交到远端仓库;git commit -m "提交内容\信息\补充说明"
⑧最后一步了直接推送到远端:git push origin master
备注:这只是master分支的,如果有了其他的分支,情况应该是一样的,只是分支有差别而已.
