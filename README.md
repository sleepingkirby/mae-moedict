Traditional Chinese Dicitonary/臺灣正體辭典
https://github.com/sleepingkirby/mae-moedict

*NOTE: Current compile is for devuan/debian/ubuntu with additional compiles for maemo and windows*


my first QT application. Made for nokia n900. Also works in linux (tested with Devuan/Debian) Works with qt4.8 only. Requires SQLite and QT4.
Definitions based off of moedict pack files:
https://github.com/g0v/moedict-app/tree/gh-pages/pack
json to sql conversion done by moedict2sqlite (I'll upload that soon)

Did this in a rush so there aren't a lot of documentation comments in code.  Because I wanted it done as soon as possible. But, also, QT is horrible to work with. Bad/wrong documentation. Some functions don't work as they should. Inconsistent function parameters for the same class. Can't do much without rewriting your own classes. Needs classes for virtually everything. Some functions just don't plain work (set scroll bars, set dimensions via qss), the css-like syntax is used but only missing other css syntax as well as changed the format of the declarations for no good reason. qss can't be applied to named elements (or at least, no way I could find in the documentation).  And other reasons I don't remember at the moment. 


Installation:
The packaging procedures for maemo via QT is a bit buggy didn't have the time to work it out. Put the mae-moedict binary somewhere you can run it. I personally put it under /opt/maemo/dev/mae-moedict/
Put the assets folder (as is) into one of the following places (the program will yell at you if you don't):

- <homepath, linux, maemo or windows>/.mae-moedict/
- <maemo homepath/MyDocs>/.mae-moedict/assets/
- /media/mmc1/.mae-moedict/ (maemo external storage path)
- current working directory of "mae-moedict.exe"

Run mae-moedict binary and have fun learning.

*NOTE* the voices folder in the assets folder is empty for good reason. 1) all the files run 2.1GB. Don't want to upload that to GitHub. 2, the 36k+ files overwhelms the vfat file system. If you want to get the files, you'll have to do it yourself.
Here's how I did it:
Get on a linux computer
open the terminal
copy over the sqlite db from assets/db/mae-moedict.db into the assets folder
do:
--------------------------
mkdir voices
cd voices
for n in `sqlite3 ../mae-moedict.db  "select num from hold where num!=''"`;do wget https://203146b5091e8f0aafda-15d41c68795720c6e932125f5ace0c70.ssl.cf1.rackcdn.com/$n.ogg;done
--------------------------


That will get all the audio files that exists in the DB into the voices directory
I only copied over about 6.9k of the files over as those are the files that I'm most interested in.
If you run:
--------------------------
for x in `ls`;do n=${#x}; if test $n -le 12;then cp $x.ogg <destination path>;fi;done 
--------------------------
that should copy the files that are words and some phrases into the destination path.


If your on phone partition is full, you can redirect the program to search for the sound files in a different folder. Since vfat doesn't allow for sym links, I had programmed this in.
Make in assets/voice/
make a text file called gohere.txt
make sure that file has ONLY the path that you want it redirect to. (Mine's /media/mmc1/mae-moedict/voices/)
It'll now look into that folder instead of the default path. 

UPDATE 2016/07/23
TextBrowser objects replaced with WebView objects. Allows for full html and CSS tags. Scroll bars also work in WebView. There's still no good way to indicat when it's loading from list of characters to definition. But I'm pretty happy with the way it functions now

UPDATE 2016/07/25
Added Free, English and About (to make sure the link to the source is available in the program) and Copy functionality (because that needs to be done manually, apparently.).

UPDATE 2016/09/04
Diverted some styles, previously hardcoded, into the style sheets in assets. This is to allow for easier/customization of font sizes, colors, etc. Useful when compiling for different OS's with different themes. 

UPDATE 2017/09/13
Put all unicode characters meant for the interface into the sql table to allow for the windows version to not choke on compile. Added and uploaded a windows version via QT 5.6

UPDATE 2017/09/30
Updated the return window for the tab stroke so that any returned result that is part of a separate stroke will be on a new line. It will also group the results by radical and label as such to allow for someone to more easily find the character they're searching for.
The radical return will also put the results on it's own new line if the results have multiple strokes.
Added a basic .sql file so that any updated/new .db from "moedict2sqlite" can have the labels needed for the program added.

UPDATE 2017/12/13
Changed the "num" column in the table "hold" to text from string. This prevents the truncation of the num that has any leading zero's. This allows for download of all audio files.

Known Issues: 
- Due to vfat and the 36000+ ogg files, you can't load it all the files into the same directory. If audio is a must, one of the following must happen:
  1) device a tree directory structure to hold all the audio files.
  2) use a file system like ext2 (just not fat) to hold the files. 
- Audio in definitions will play, but you can't hear the audio. Probably due to QT limitations/bug.
- Added a work around to include the path + mplayer to the definitions. Can be copied and pasted into the terminal.


# mae-moedict
