PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE creds
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
  	hostname TEXT,
    login TEXT,
    password TEXT
);
INSERT INTO creds VALUES(34,'https://important_notes.com','qyqy','ereew');
INSERT INTO creds VALUES(35,'http://my-notes.ro','eba1f4b841a345b194126832f3ea52a8','b3a9f55f425d56febada60076c7f6308');
INSERT INTO creds VALUES(36,'http://not-my-notes.ro','a2845cc9f2d027e40e676082b9017b0a','ec544bccb47252ca813f3b5af2db26d4');
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('creds',36);
COMMIT;
