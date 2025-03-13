# calculator-task

## Ubuntu 20.04
### Собрать приложение и deb пакет для linux ubuntu
Собрать в docker 
```bash
git clone https://github.com/alec-chicherini/calculator-task.git
cd calculator-task
git submodule init
git submodule update
docker build --target=qt_from_repo . -t calculator-task-build
idTempContainer=$(docker create calculator-task-build)
docker cp "$idTempContainer":/result/ .
docker rm "$idTempContainer"
```

## Запуск в Ubuntu 20.04
### Подготовить чистую машину для тестов Ubuntu 20.04
```
apt update
apt install ubuntu-desktop
apt install xrdp
#passwd

dpkg -i calculator-task_25.03_amd64.deb
apt-get install -f -y
```
## Запустить в Linux
Пуск - Стандартные - calculator-task

В консоли calculator-task

# Windows 10 Visual Studio 2022
## Установить Qt 6.8 - в Developer powershell VS2022
```
cd C:/
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = "C:/vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
vcpkg install qtbase
```

## Собрать в powershell
```
cd C:/
git clone https://github.com/alec-chicherini/calculator-task.git
cd calculator-task
git submodule init
git submodule update
mkdir build
cd build
cmake .. 
	-DCMAKE_PREFIX_PATH="C:/vcpkg/installed/x64-windows/share/Qt6"  
	-DCMAKE_INSTALL_PREFIX="C:/calculator-task/build/install" 
	-DPATH_QT_INSTALLED_DLL="C:/vcpkg/installed/x64-windows/bin/"
cmake --build . --config Release
cmake --install .
```

## Запустить
```
C:/calculator-task/build/install/calculator-task.exe
```
 