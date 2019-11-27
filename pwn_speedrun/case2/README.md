# Pwn speedrun | Case2

## Информация

> Для окончательно победой над арбалетами нам нужно получить доступ ко всем оставшимся сервисам.
> 
> Мы обнаружили множество простых сетевых сервисов, думаю ты быстро справишься.
> 
> `nc <ip> 33046`
>
> `flag in /tmp/flag.txt`

## Запуск

Отдать командам бинарник из deploy/static и ip-адрес сервера

```sh
cd deploy
docker-compose -p speedrun_pwn_case2 up --build -d 
```


## Описание

ELF 64bit, C, no strip, no pack

Суть задания - проэксплуатировать переполнение буфера на сетке через выполнение кода на стеке или ROP цепочку

## Решение

1. Имеем исполняемый стек и запись в .bss 
2. Можем записать в .bss jmp-гаджет с помощью которого попасть на стек
3. Берём простой шелл-код и выполняем его на стеке.

[Пример эксплоита](solve/exploit.py)


## Флаг

`Cup{47759575671a70b361315fb3956570782c5f2462164fe63e3da02066d69a4199}`