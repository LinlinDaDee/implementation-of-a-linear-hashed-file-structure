make

rm R.*

./create  R  3  4  "0,0:0,1:0,2:1,0:1,1:2,0"
./insert R < data1.txt
./stats  R