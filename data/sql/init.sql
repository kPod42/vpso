-- Включаем поддержку внешних ключей (на будущее)
PRAGMA foreign_keys = ON;

-- Таблица для хранения всех экспериментов метода Монте-Карло
CREATE TABLE IF NOT EXISTS experiments (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    R            REAL    NOT NULL,           -- радиус окружности
    x0           REAL    NOT NULL,           -- координата центра X
    y0           REAL    NOT NULL,           -- координата центра Y
    C            REAL    NOT NULL,           -- расстояние хорды от оси
    orientation  TEXT    NOT NULL CHECK (orientation IN ('vertical','horizontal')),
    N            INTEGER NOT NULL,           -- количество сгенерированных точек
    S_est        REAL    NOT NULL,           -- оценка площади
    error        REAL,                       -- относительная погрешность, %
    timestamp    DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Индекс для быстрого доступа к последним экспериментам
CREATE INDEX IF NOT EXISTS idx_experiments_ts
    ON experiments(timestamp DESC);
