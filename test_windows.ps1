echo "REFERENCE"

echo "10M"
./Project5_map.exe ./Project5_WIN/data/10M/a_10M_load.dat ./Project5_WIN/data/10M/a_10M_txns.dat 10000000 10000000
./Project5_map.exe ./Project5_WIN/data/10M/c_10M_load.dat ./Project5_WIN/data/10M/c_10M_txns.dat 10000000 10000000
./Project5_map.exe ./Project5_WIN/data/10M/e_10M_load.dat ./Project5_WIN/data/10M/e_10M_txns.dat 10000000 10000000

echo "50M"
./Project5_map.exe ./Project5_WIN/data/50M/a_50M_load.dat ./Project5_WIN/data/50M/a_50M_txns.dat 50000000 50000000
./Project5_map.exe ./Project5_WIN/data/50M/c_50M_load.dat ./Project5_WIN/data/50M/c_50M_txns.dat 50000000 50000000
./Project5_map.exe ./Project5_WIN/data/50M/e_50M_load.dat ./Project5_WIN/data/50M/e_50M_txns.dat 50000000 50000000


echo "500M"
./Project5_map.exe ./Project5_WIN/data/500M/a_500M_load.dat ./Project5_WIN/data/500M/a_500M_txns.dat 500000000 500000000
./Project5_map.exe ./Project5_WIN/data/500M/c_500M_load.dat ./Project5_WIN/data/500M/c_500M_txns.dat 500000000 500000000
./Project5_map.exe ./Project5_WIN/data/500M/e_500M_load.dat ./Project5_WIN/data/500M/e_500M_txns.dat 500000000 500000000


echo "MY CODE"

echo "10M"
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/10M/a_10M_load.dat ./Project5_WIN/data/10M/a_10M_txns.dat 10000000 10000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/10M/c_10M_load.dat ./Project5_WIN/data/10M/c_10M_txns.dat 10000000 10000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/10M/e_10M_load.dat ./Project5_WIN/data/10M/e_10M_txns.dat 10000000 10000000

echo "50M"
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/50M/a_50M_load.dat ./Project5_WIN/data/50M/a_50M_txns.dat 50000000 50000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/50M/c_50M_load.dat ./Project5_WIN/data/50M/c_50M_txns.dat 50000000 50000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/50M/e_50M_load.dat ./Project5_WIN/data/50M/e_50M_txns.dat 50000000 50000000


echo "500M"
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/500M/a_500M_load.dat ./Project5_WIN/data/500M/a_500M_txns.dat 500000000 500000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/500M/c_500M_load.dat ./Project5_WIN/data/500M/c_500M_txns.dat 500000000 500000000
./x64/Release/Project5_WIN.exe ./Project5_WIN/data/500M/e_500M_load.dat ./Project5_WIN/data/500M/e_500M_txns.dat 500000000 500000000
