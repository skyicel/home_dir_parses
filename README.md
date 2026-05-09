# Media Scanner

Сканирует директорию и сохраняет список мультимедийных файлов в `~/.media_files`.

## Сборка

```bash
make
```

## Запуск

```bash
# По умолчанию (домашняя папка, интервал 10 секунд)
./media_scanner

# С указанием пути и интервала
./media_scanner --path /home/vadim/Music --interval 30

# Справка
./media_scanner --help
```

## Параметры

| Параметр | Описание |
|----------|----------|
| `--path <dir>` | Путь к директории для сканирования |
| `--interval <sec>` | Интервал сканирования в секундах |
| `--help` | Показать справку |

## Результат

Файл `~/.media_files` в формате JSON:

```json
{
    "audio": ["file1.mp3", "file2.wav"],
    "video": ["movie.mpg"],
    "images": ["photo.jpg"]
}
```

## Остановка

Нажмите `Ctrl+C`