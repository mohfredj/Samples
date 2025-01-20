# repository for personal exemples
------------------ useful git cmd --------------------

1-	Delete local remote branch from local repor :

-	git remote prune origin
-	
2-	delete local branch which are merged then removed from remote
 	
-	git branch --merged | grep -E -v "(develop|master)" | xargs git branch -d
-	
3-	delete local tags removed from remote
 	
-	git tag -l | xargs git tag -d && git fetch –t
-	
4-	After fetching, remove any remote-tracking branches which no longer exist on the remote
 	
-	git fetch –prune
-	
5-	LFS : # disable ssl verification everywhere
 	
git lfs install

git config http.sslVerify false


6-	Compare local to remote:

git log -p HEAD..FETCH_HEAD

