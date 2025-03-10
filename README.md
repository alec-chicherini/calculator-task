# calculator-task

<!--Собрать клиент и deb пакет для linux-->
## Собрать latest клиент и deb пакет для linux ubuntu
Собрать в docker 
```bash
git clone https://github.com/alec-chicherini/calculator-task.git
cd calculator-task
docker build --target=qt_from_repo . -t calculator-task-build
idTempContainer=$(docker create calculator-task-build)
docker cp "$idTempContainer":/result/*.deb .
docker rm "$idTempContainer"
```

# Запуск в Linux
## Подготовить чистую машину для тестов Ubuntu 20.04
```
apt update
apt install ubuntu-desktop
apt install xrdp
#passwd

dpkg -i calculator-task_25.03_amd64.deb
apt-get install -f -y
```
## Запустить игру
Пуск - Стандартные - calculator-task
 