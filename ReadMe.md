# Org Chart Api

Restful API built using [Drogon](https://github.com/drogonframework/drogon).

Endpoints
---------

### Persons
| Method     | URI                                   | Action                                    |
|------------|---------------------------------------|-------------------------------------------|
| `GET`      | `/persons?limit={}&offset={}&sort_field={}&sort_order={}` | `Retrieve all persons`    |
| `GET`      | `/persons/{id}`                       | `Retrieve person`                         |
| `GET`      | `/persons/{id}/reports`               | `Retrieve person direct reports`          |
| `POST`     | `/persons`                            | `Create person`                           |
| `PUT`      | `/persons/{id}`                       | `Update person`                           |
| `DELETE`   | `/persons/{id}          `             | `Delete person`                           |

### Departments
| Method     | URI                                   | Action                                    |
|------------|---------------------------------------|-------------------------------------------|
| `GET`      | `/departments?limit={}&offset={}&sort_field={}&sort_order={}`   | `Retrieve all departments`     |
| `GET`      | `/departments/{id}`                   | `Retrieve department`                     |
| `GET`      | `/departments/{id}/persons`           | `Retrieve department persons`             |
| `POST`     | `/departments`                        | `Create department`                       |
| `PUT`      | `/departments/{id}`                   | `Update department`                       |
| `DELETE`   | `/departments/{id}`                   | `Delete department`                       |

### Jobs
| Method     | URI                                   | Action                                    |
|------------|---------------------------------------|-------------------------------------------|
| `GET`      | `/jobs?limit={}&offset={}&sort_fields={}&sort_order={}` | `Retrieve all jobs`     |
| `GET`      | `/jobs/{id}`                          | `Retrieve job`                            |
| `GET`      | `/jobs/{id}/persons`                  | `Retrieve job persons`                    |
| `POST`     | `/jobs`                               | `Create job`                              |
| `PUT`      | `/jobs/{id}`                          | `Update job`                              |
| `DELETE`   | `/jobs/{id}`                          | `Delete job`                              |

### Auth
| Method     | URI                                   | Action                                    |
|------------|---------------------------------------|-------------------------------------------|
| `POST`     | `/auth/register`                      | `Register User and obtain JWT token`      |
| `POST`     | `/auth/login`                         | `Login User `                             |

How to build the project
---
### Installation
See drogon documentation [here](https://github.com/an-tao/drogon/wiki/ENG-02-Installation#System-Requirements)! 

### Verify Installation
Confirm the database development environment using 'drogon_ctl -v':
```
     _                                                                                                                                                                                                       [0/365]
  __| |_ __ ___   __ _  ___  _ __
 / _` | '__/ _ \ / _` |/ _ \| '_ \
| (_| | | | (_) | (_| | (_) | | | |
 \__,_|_|  \___/ \__, |\___/|_| |_|
                 |___/

A utility for drogon
Version: 1.7.5
Git commit: fc68b8c92c8c202d8cc58d83629d6e8c8701fc47
Compilation:
  Compiler: /Library/Developer/CommandLineTools/usr/bin/c++
  Compiler ID: AppleClang
  Compilation flags: -std=c++17 -I/usr/local/include
Libraries:
  postgresql: yes  (batch mode: no)
  mariadb: yes
  sqlite3: yes
  openssl: no
  brotli: yes
  boost: no
  hiredis: no
  c-ares: yes
```

### Setup Database
Start a postgres server. <br />
`docker run --name postgres -e POSTGRES_PASSWORD=password -d postgres`

Log into postgres using `psql` to create a `org_chart` database. <br />
`psql 'postgresql://postgres:password@127.0.0.1:5432/org_chart'`

Create and seed the tables. <br />
`psql 'postgresql://postgres:password@127.0.0.1:5432/org_chart' -f scripts/create_db.sql` <br />
`psql 'postgresql://postgres:password@127.0.0.1:5432/org_chart' -f scripts/seed_db.sql`

### Build
```
git clone https://github.com/maikeulb/orgChartApi
git submodule update --init
mkdir build
cd build
cmake ..
make
```
### Run
Make the necessary changes to `config.json` and run the project `./orgChart`

Usage
---------------
`http get localhost:3000/persons offset=1 limit=25 sort_field=id sort_order=asc`
```
[
   {
        "department": {
            "id": 1,
            "name": "Product"
        },
        "first_name": "Gary",
        "hire_date": "2018-04-07 01:00:00",
        "id": 2,
        "job": {
            "id": 2,
            "title": "M1"
        },
        "last_name": "Reed",
        "manager": {
            "department_id": 1,
            "first_name": "Sabryna",
            "hire_date": "2014-02-01",
            "id": 1,
            "job_id": 1,
            "last_name": "Peers",
            "manager_id": 1
        }
    },
    {
        "department": {
            "id": 1,
            "name": "Product"
        },
        "first_name": "Madonna",
        "hire_date": "2018-03-08",
        "id": 3,
        "job": {
            "id": 2,
            "title": "M1"
        },
        "last_name": "Axl",
        "manager": {
            "department_id": 1,
            "first_name": "Sabryna",
            "hire_date": "2014-02-01",
            "id": 1,
            "job_id": 1,
            "last_name": "Peers",
            "manager_id": 1
        }
    },
    {
        "department": {
            "id": 1,
            "name": "Product"
        },
        "first_name": "Marcia",
        "hire_date": "2020-01-11",
        "id": 4,
        "job": {
            "id": 4,
            "title": "E5"
        },
        "last_name": "Stuart",
        "manager": {
            "department_id": 1,
            "first_name": "Gary",
            "hire_date": "2018-04-07 01:00:00",
            "id": 2,
            "job_id": 2,
            "last_name": "Reed",
            "manager_id": 1
        }
    },
...
]
```
