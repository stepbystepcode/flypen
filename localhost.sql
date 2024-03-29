-- Adminer 4.8.1 PostgreSQL 16.2 dump

DROP TABLE IF EXISTS "chat";
DROP SEQUENCE IF EXISTS chat_id_seq;
CREATE SEQUENCE chat_id_seq INCREMENT 1 MINVALUE 1 MAXVALUE 2147483647 CACHE 1;

CREATE TABLE "public"."chat" (
    "id" integer DEFAULT nextval('chat_id_seq') NOT NULL,
    "content" text NOT NULL,
    "sender" character varying(50) NOT NULL,
    "receiver" character varying(50) NOT NULL,
    "time" timestamp NOT NULL,
    "sender_isread" smallint NOT NULL,
    "receiver_isread" smallint NOT NULL,
    CONSTRAINT "chat_pkey" PRIMARY KEY ("id")
) WITH (oids = false);

INSERT INTO "chat" ("id", "content", "sender", "receiver", "time", "sender_isread", "receiver_isread") VALUES
(1,	'Welcome to flypen! We are glad to see you here!',	'FlypenTeam',	'bwb',	'2024-03-29 11:27:41.712719',	0,	1),
(2,	'1',	'bwb',	'lj',	'2024-03-29 13:33:36.10067',	1,	1),
(8,	'1',	'lj',	'bwb',	'2024-03-29 16:09:07.696584',	1,	0);

DROP TABLE IF EXISTS "file";
CREATE TABLE "public"."file" (
    "filename" text NOT NULL,
    CONSTRAINT "file_pkey" PRIMARY KEY ("filename")
) WITH (oids = false);


DROP TABLE IF EXISTS "users";
CREATE TABLE "public"."users" (
    "username" character varying(50) NOT NULL,
    "password" text NOT NULL,
    "avatar" integer NOT NULL,
    "friends" text NOT NULL,
    "createtime" timestamp NOT NULL,
    "req" text NOT NULL,
    CONSTRAINT "users_pkey" PRIMARY KEY ("username")
) WITH (oids = false);

INSERT INTO "users" ("username", "password", "avatar", "friends", "createtime", "req") VALUES
('FlypenTeam',	'FlypenTeam',	0,	'',	'2024-03-29 10:29:43.397413',	''),
('bwb',	'6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b',	9,	'FlypenTeam,lj',	'2024-03-29 11:27:41.680964',	''),
('lj',	'6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b',	2,	'FlypenTeam,bwb',	'2024-03-29 11:11:01.3027',	'');

-- 2024-03-29 16:14:55.438097+08
