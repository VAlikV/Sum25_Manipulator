# Установка компонентов и зависимостей

## Установка VSCode
- Ссылка: https://code.visualstudio.com/download
- Скачать в расширении .deb
- В директории, куда скачен файл, выполнить команду для установки:

    ```bash
    sudo dpkg -i <название файла>
    ```
- В самом vscode установить расширения в разделе `Extensions`:
    - Cmake Tools
    - Python
    - Далее по желанию, там много интресного

## Устанновка CMake

```bash
sudo apt install snapd
sudo snap install cmake
```
*Можно и через apt, но через snap более новая версия

## Установка библиотек для C++
### Eigen и его зависимости:

```bash
sudo apt install libboost-dev libblas-dev liblapack-dev
sudo apt install libeigen3-dev
```
### drake

- По инструкции с сайта https://drake.mit.edu/apt.html (да, могут быть ошибки, не стесняйтесь обращаться)
- Команды 
    ```bash
    export PATH="/opt/drake/bin${PATH:+:${PATH}}"
    export PYTHONPATH="/opt/drake/lib/python$(python3 -c 'import sys; print("{0}.{1}".format(*sys.version_info))')/site-packages${PYTHONPATH:+:${PYTHONPATH}}"
    ```
    вписать в файл `/home/"user"/.bashrc`. В этом файле записаны скрипты, которые выполняются каждый раз при запуске консоли (просто факт для сведения)

- Теперь нужно убрать репозитории drake из репозиториев apt:
    - Открыть /etc/apt/sources.list
    - Зайти в раздел Other Software
    - Убрать галочку рядом с тем, что имеет в названии drake)

- По идее готово)

