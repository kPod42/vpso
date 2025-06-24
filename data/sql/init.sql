-- Включаем поддержку внешних ключей (на будущее)
PRAGMA foreign_keys = ON;

-- Таблица для хранения всех экспериментов метода Монте-Карло
CREATE TABLE IF NOT EXISTS experiments (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    R           REAL,
    x0          REAL,
    y0          REAL,
    C           REAL,
    orientation TEXT,
    N           INTEGER,
    S_est       REAL,
    error       REAL,
    ts          TEXT          -- unix-time
);

-- Индекс для быстрого доступа к последним экспериментам
CREATE INDEX IF NOT EXISTS idx_experiments_ts
    ON experiments(ts DESC);
