INSERT INTO job(id, title) VALUES
    (1, 'CEO'),
    (2, 'M1'),
    (3, 'E4'),
    (4, 'E5');

INSERT INTO department(id, name) VALUES
    (1, 'Product'),
    (2, 'Infrastructure');

INSERT INTO person(id, job_id, department_id, manager_id, first_name, last_name, hire_date) VALUES
    (1, 1, 1, 1, 'Sabryna', 'Peers', '2014-02-01'),
    (2, 2, 1, 1, 'Tayler', 'Shantee', '2018-04-07'),
    (3, 2, 1, 1, 'Madonna', 'Axl', '2018-03-08'),
    (4, 4, 1, 2, 'Marcia', 'Stuart', '2020-01-11'),
    (5, 3, 1, 2, 'Cliff', 'Rosalind', '2021-02-15'),
    (6, 3, 1, 3, 'Lake', 'Philippa', '2022-05-21'),
    (7, 3, 1, 3, 'Wynne', 'Walker', '2021-12-31'),
    (8, 2, 2, 1, 'Sterling', 'Haley', '2019-11-02'),
    (9, 2, 2, 8, 'Melissa', 'Garland', '2017-08-05'),
    (10, 4, 2, 8, 'Leon', 'JayLee', '2022-02-17'),
    (11, 4, 2, 8, 'Kaylie', 'Elyse', '2021-01-18'),
    (12, 4, 2, 8, 'Yancey', 'Trenton', '2022-03-02');

INSERT INTO users(id, username, password) VALUES
    (1, 'admin', 'password');
