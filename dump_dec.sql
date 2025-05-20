PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE creds (id INTEGER PRIMARY KEY AUTOINCREMENT, hostname TEXT, login TEXT, password TEXT, salt TEXT);
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('creds',0);
COMMIT;
