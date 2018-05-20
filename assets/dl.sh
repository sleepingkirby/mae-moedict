#!/bin/bash

for n in `sqlite3 ./db/mae-moedict.db  "select num from hold where num!=''"`
do  
if test -e "./$n.ogg"
then echo "$n already exists"
else 
wget https://203146b5091e8f0aafda-15d41c68795720c6e932125f5ace0c70.ssl.cf1.rackcdn.com/$n.ogg -O ./voices/$n.ogg 
fi
done
