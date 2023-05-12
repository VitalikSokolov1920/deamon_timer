if [ ! -d "build" ]; then
    mkdir build
fi

cd build || return

cmake .. -DSKIP_BUILD_TEST=on -DBUILD_SHARED_LIBS=off -DBUILD_DOC=off -DINSTALL_TEST=off

cmake --build .

# обязательный параметр
export PGUSER='postgres'
# обязательный параметр
export PGPASSWORD='root'
# обязательный параметр
export PGDBNAME='test'
# путь к файлу логирования(необязательный параметр)
# значение по умолчанию: ./ivt_log.log
export LOG_PATH='../IVT_log.log'
# путь к директории для резервного копирования, слеш в конце ставить не нужно(необязательный параметр)
# значение по умолчанию: ./backup
export DUMP_PATH='../ivt_dump'

./IVT_deamon ../config.txt
