# Установка зависимостей для CV

### Создание локального окружения
```bash
python -m venv .venv
```

#### Активация окружения:

Для Windows:

```bash
.venv\Scripts\Activate.ps1
```

Для Linux:

```bash
source .venv/bin/activate
```

 Если возникает ошибка во время активации (на Windows):
```bash
Set-ExecutionPolicy Unrestricted -Scope Process
```

#### Деактивация:
```bash
deactivate
```

#### Установка зависимостей из файла
```bash
python -m pip install -r requirements.txt
```

#### Создание файла requirements.txt
```bash
python -m pip freeze > requirements.txt
```

## Необходимые библиотеки

    - pip install opencv-python
    - pip install ultralytics

## Можно запустить код

    