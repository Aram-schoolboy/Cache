# Автотесты [![Ci/CD](../../actions/workflows/classroom.yml/badge.svg?branch=main&event=workflow_dispatch)](../../actions/workflows/classroom.yml)

> [!IMPORTANT]
> Если работа выполняется на `C++`, то файл, содержащий функцию `main` должен называться `main.cpp`. Иначе – `main.c`.

В автотесте ваш код собирается и запускается несколько раз, в некоторых запусках будет указан ключ `--bin`.

В качестве входного файла для аргумента `--asm` в автотесте подаётся `rv32.asm`.

> [!WARNING]
> Если работа выполняется на `lite`, то в таком случае при команде `--bin` необходимо написать в поток вывода ошибок `"Compile asm code to binary not supported"` и завершиться с нулевым кодом возврата.
> Если какая-то из политик вытеснения не реализована, то выводится `unsupported`, пример `pLRU:\tunsupported\n`.

## Что лежит в репозитории

* `template_dont_edit` – директория с шаблоном для отчёта.

## Работа с шаблонами

1. Копируем шаблон(ы) в корень репозитория.
2. Работаем в скопированных шаблонах.

# Что нужно загружать в репозиторий

1. Код проекта.
2. Файл `rv32.asm` с кодом ассемблера RISC-V.
3. [normal] Приложить результат перевода в машинный код в файл `rv32.bin`.
4. Отчёт. Без отчёта работа не проверяется!

Проверяются только файлы из корня репозитория. Даже отчёт.
