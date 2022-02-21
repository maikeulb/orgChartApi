CREATE TABLE jobs (
    id SERIAL PRIMARY KEY,
    title VARCHAR(50) UNIQUE NOT NULL
);

CREATE TABLE departments (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) UNIQUE NOT NULL
);

CREATE TABLE persons (
    id SERIAL PRIMARY KEY,
    job_id int NOT NULL,
    department_id int NOT NULL,
    manager_id int NOT NULL,
    first_name VARCHAR(50) UNIQUE NOT NULL,
    last_name VARCHAR(50) UNIQUE NOT NULL,
    hire_date DATE UNIQUE NOT NULL,
    UNIQUE (first_name, last_name),
    CONSTRAINT fk_job FOREIGN KEY(job_id) REFERENCES jobs(id) ON DELETE SET NULL,
    CONSTRAINT fk_department FOREIGN KEY(department_id) REFERENCES departments(id) ON DELETE SET NULL,
    CONSTRAINT fk_manager FOREIGN KEY(manager_id) REFERENCES persons(id) ON DELETE SET NULL
);

