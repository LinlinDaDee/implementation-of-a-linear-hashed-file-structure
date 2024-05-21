make
rm  -f  R.*
./create R 3 2 "0,0:1,0:2,0:0,1:1,1:2,1"
./insert R < data1.txt
echo "==========================================="
./select R '1042,?,?' | sort
echo "==========================================="
./select R '?,horoscope,?' | sort
echo "==========================================="
./select R '?,?,shoes' | sort
echo "==========================================="
./select R '?,shoes,?' | sort
echo "==========================================="
./select R '?,chair,shoes' | sort
echo "==========================================="
./select R '?,shoes,chair' | sort
echo "==========================================="
./select R '?,?,?' | sort > test22.txt
wc -l test22.txt


