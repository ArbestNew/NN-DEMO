# code version: 0
git pull
sh clean.sh

cd sw
pwd
make

cd ../tb
pwd
make | tee ../soft_tb_make_result

sh run.sh