Mapcir Cron - Lightweight job scheduling for Linux
==================================================

> It is a drop-in replacement for crontab and allows you to make sure that your jobs only run on auspicious timings.

## Requirements

- C++ >= 11
- [nlohmann](https://github.com/nlohmann/json)

## Installation
To install the Mapcir Cron, please execute the following commands.

Download: 

```bash
$ git clone https://github.com/mapcirio/mapcir-cron.git
$ cd mapcir-cron
$ cd build
```

Debug:
```bash
$ cmake ..
```

Release: 
```bash
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```

Build:
```bash
$ make
```

Run:
``` bash
$ ./mapcir-cron --f=../configs.json
```

Show help:
```bash
$ ./mapcir-cron --help
```

## Configuration
```bash
[
    {
        "name": "test",
        "description": "echo current timestamp",
        "cmd": "date +%s",
        "interval": 1000,
        "debug": true
    }
    ...
]
```