PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE personal(id int, datetime datetime, char text, pinyin text, zhuyin text, edef text, notes text);
COMMIT;
