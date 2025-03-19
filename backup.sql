PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "creds" (
	"id"	INTEGER,
	"hostname"	TEXT NOT NULL UNIQUE,
	"login"	TEXT NOT NULL,
	"password"	TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
);
INSERT INTO creds VALUES(1,'https://important_notes.com','qyqy','ereew');
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('creds',1);
COMMIT;
