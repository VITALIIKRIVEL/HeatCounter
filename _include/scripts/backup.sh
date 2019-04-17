#!/bin/bash
# Скрипт резервного копирования

project=_include
folder=/home/nayk/Projects/${project}

if ! [ -d $folder ]; then
	echo "Каталог ${folder} не найден."
	exit 0
fi

dir1=/home/nayk/.other/Backup/Projects/${project}

if ! [ -d $dir1 ]; then
	mkdir $dir1
fi

if ! [ -d $dir1 ]; then
	echo "Каталог ${dir1} не найден."
	exit 0
fi

dir2=/mnt/bart-srv/BACKUP/teterin-pc/Projects/${project}
dir3=/home/backup/Projects/${project}
dropbox=/home/nayk/Dropbox/nayk/Projects/${project}
dt=`date +%F`
# имя файла ввиде даты ГГГГ-ММ-ДД
file_name=${project}_${dt}.tar.gz
echo "Дата: ${dt}"
echo "Резервное копирование в файл ${dir1}/${file_name}, пожалуйста подождите..."
# создаем архив:
tar -cvpzf ${dir1}/${file_name} ${folder}
echo "Копирование архива..."
./del-old-backup.sh $dir1 5

sleep 3s

if [ -d $dir2 ]; then
	./del-old-backup.sh $dir2 8
	cp -f ${dir1}/${file_name} ${dir2}
fi

if [ -d $dir3 ]; then
	./del-old-backup.sh $dir3 10
	cp -f ${dir1}/${file_name} ${dir3}
fi


if [ -d $dropbox ]; then
	./del-old-backup.sh $dropbox 2
	cp -f ${dir1}/${file_name} ${dropbox}
fi

echo "Процесс завершен."
exit 0
