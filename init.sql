-- 移除了 PostgreSQL 的 SEQUENCE (序列)
DROP TABLE IF EXISTS `chat`;

-- 使用 backticks (`) 代替双引号 (")
-- 使用 AUTO_INCREMENT 替代 SEQUENCE
-- 使用 varchar 替代 character varying
-- 使用 int/smallint 替代 integer/smallint
-- 添加了 InnoDB 引擎和 utf8mb4 字符集
CREATE TABLE `chat` (
    `id` int NOT NULL AUTO_INCREMENT,
    `content` text NOT NULL,
    `sender` varchar(50) NOT NULL,
    `receiver` varchar(50) NOT NULL,
    `time` timestamp NOT NULL,
    `sender_isread` smallint NOT NULL,
    `receiver_isread` smallint NOT NULL,
    CONSTRAINT `chat_pkey` PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- INSERT 语句在 MySQL 中是兼容的
INSERT INTO `chat` (`id`, `content`, `sender`, `receiver`, `time`, `sender_isread`, `receiver_isread`) VALUES
(1,	'Welcome to flypen! We are glad to see you here!',	'FlypenTeam',	'bwb',	'2024-03-29 11:27:41.712719',	0,	1),
(2,	'1',	'bwb',	'lj',	'2024-03-29 13:33:36.10067',	1,	1),
(8,	'1',	'lj',	'bwb',	'2024-03-29 16:09:07.696584',	1,	0);

DROP TABLE IF EXISTS `file`;

-- 注意：MySQL 不能在 'text' 类型的列上设置主键（除非指定前缀长度）
-- 鉴于这是文件名，'varchar(255)' 是一个更合适、更高效的选择
CREATE TABLE `file` (
    `filename` varchar(255) NOT NULL,
    CONSTRAINT `file_pkey` PRIMARY KEY (`filename`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `users`;

-- 'users' 表的转换
CREATE TABLE `users` (
    `username` varchar(50) NOT NULL,
    `password` text NOT NULL,
    `avatar` int NOT NULL,
    `friends` text NOT NULL,
    `createtime` timestamp NOT NULL,
    `req` text NOT NULL,
    CONSTRAINT `users_pkey` PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- INSERT 语句在 MySQL 中是兼容的
INSERT INTO `users` (`username`, `password`, `avatar`, `friends`, `createtime`, `req`) VALUES
('FlypenTeam',	'FlypenTeam',	0,	'',	'2024-03-29 10:29:43.397413',	''),
('bwb',	'6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b',	9,	'FlypenTeam,lj',	'2024-03-29 11:27:41.680964',	''),
('lj',	'6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b',	2,	'FlypenTeam,bwb',	'2024-03-29 11:11:01.3027',	'');

-- 2024-03-29 16:14:55.438097+08
