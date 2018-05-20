PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
DROP TABLE IF EXISTS lbls;
CREATE TABLE lbls(id int primary key, eng text, chn text, lang text);
INSERT INTO "lbls" VALUES(1,'zhuyin','注音','trad_chn');
INSERT INTO "lbls" VALUES(2,'stroke','筆畫','trad_chn');
INSERT INTO "lbls" VALUES(3,'rad','部首','trad_chn');
INSERT INTO "lbls" VALUES(4,'free','自由','trad_chn');
INSERT INTO "lbls" VALUES(5,'eng','英文','trad_chn');
INSERT INTO "lbls" VALUES(6,'part','部','trad_chn');
INSERT INTO "lbls" VALUES(7,'hua','畫','trad_chn');
INSERT INTO "lbls" VALUES(8,'not','無','trad_chn');
INSERT INTO "lbls" VALUES(9,'search','搜尋','trad_chn');
INSERT INTO "lbls" VALUES(10,'searching','搜尋中...','trad_chn');
INSERT INTO "lbls" VALUES(11,'ifuseful','如果你覺得好用，請你考慮捐款我的','trad_chn');
INSERT INTO "lbls" VALUES(12,'bugsugg','歡迎所有的錯誤報告和功能請求','trad_chn');
INSERT INTO "lbls" VALUES(13,'learn','學習','trad_chn');
INSERT INTO "lbls" VALUES(14,'rand','任意','trad_chn');
INSERT INTO "lbls" VALUES(15,'all','全部','trad_chn');
INSERT INTO "lbls" VALUES(16,'pers','私人','trad_chn');
INSERT INTO "lbls" VALUES(17,'dict','辭典','trad_chn');
INSERT INTO "lbls" VALUES(18,'add','加','trad_chn');
COMMIT;
