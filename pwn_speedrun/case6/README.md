# Pwn speedrun | Case6

## Информация

> Для окончательно победой над арбалетами нам нужно получить доступ ко всем оставшимся сервисам.
> 
> Мы обнаружили множество простых сетевых сервисов, думаю ты быстро справишься.
> 
> `nc <ip> 33050`
>

## Запуск

Отдать командам бинарник из deploy/static и ip-адрес сервера

```sh
cd deploy
docker-compose -p speedrun_pwn_case6 up --build -d 
```


## Описание

ELF 64bit, C, no strip, no pack

Суть задания - осуществить one-byte-overflow на стеке 

## Решение

1. При создании пользователя вводим строку /bin/sh
2. Делаем one-byte overflow и переписываем saved rbp
3. Располагаем на стеке роп-цепочку для вызова system с аргументов в виде указателя на строку

[Пример эксплоита](solve/exploit.py)


## Флаг

`Cup{99a22a4190241ff92712f177023a2797b2bd1fc77b54dc76007158f8e0e117cc}`