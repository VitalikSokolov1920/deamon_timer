if [ ! -d "build" ]; then
    mkdir build
fi

cd build || return

cmake ..

cmake --build .

export PGUSER='postgres'
export PGPASSWORD='root'
export PGDBNAME='test'
# путь к файлу
export LOG_PATH='../IVT_log.log'
# путь к директории
export DUMP_PATH='../ivt_dump'

./practice ../config.txt
