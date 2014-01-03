for dir in $(ls -l | egrep '^d' | awk '{print $9}'); do echo $dir; done
