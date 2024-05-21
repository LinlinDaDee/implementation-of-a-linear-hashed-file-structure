make
rm  -f  R.*
./create  R  3  4  "0,0:0,1:0,2:1,0:1,1:2,0"
./gendata 100000 3 100000 | ./insert R
./stats R
./select R '?,?,?' | sort > test22.txt
wc -l test22.txt