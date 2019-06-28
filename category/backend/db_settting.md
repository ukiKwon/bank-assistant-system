#AWS 데이터베이스 이름 : kbas  
(1) 테이블 구성  
 - finService : 금융 서비스 종류
```
  CREATE TABLE finService (
        finid INTEGER AUTO_INCREMENT,
        finname VARCHAR(255) NOT NULL,
        PRIMARY KEY(finid)
  );
```  
 - lastFinService : 지난 금융 이력 정보
  ```
    CREATE TABLE lastFinService (
          cid VARCHAR(255),
          finid INTEGER,
          FOREIGN KEY (finid) REFERENCES finService (finid),
          FOREIGN KEY (cid) REFERENCES custom (cid)
      );
  ```  
 - frontState : 창구 정보
  ```
  CREATE TABLE frontState (
      frontid VARCHAR(255) PRIMARY KEY,
      isbusy boolean DEFAULT '0',
      numofcustom INTEGER DEFAULT '0',
      expectedwaittime INTEGER DEFAULT '0'
      waitcustomlist VARCHAR(255) DEFAULT NULL;
  );
  ```  
  - custom : 고객 정보
  ```
  CREATE TABLE custom (
      cid VARCHAR(255) PRIMARY KEY,
      cname VARCHAR(20) NOT NULL
  );
  ```
(2) 데이터셋  
 - finService
  {"예금(deposit)", "적금(savings)", "대출(loan)", "재발급(repulish)"}
  `INSERT INTO finService(finname) VALUES("deposit");INSERT INTO finService(finname) VALUES("savings");INSERT INTO finService(finname) VALUES("loan");INSERT INTO finService(finname) VALUES("repulish");`  

- lastFinService
  `INSERT INTO lastFinService(cid, finid) VALUES("c0000", 1);INSERT INTO lastFinService(cid, finid) VALUES("c0001", 2);
  INSERT INTO lastFinService(cid, finid) VALUES("c0002", 3);INSERT INTO lastFinService(cid, finid) VALUES("c0003", 4);
  INSERT INTO lastFinService(cid, finid) VALUES("c0004", 1);INSERT INTO lastFinService(cid, finid) VALUES("c0005", 2);
  INSERT INTO lastFinService(cid, finid) VALUES("c0006", 3);INSERT INTO lastFinService(cid, finid) VALUES("c0007", 4);
  INSERT INTO lastFinService(cid, finid) VALUES("c0008", 1);INSERT INTO lastFinService(cid, finid) VALUES("c0009", 2);`  

- frontState
`INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime) VALUES("b0000", 0, 0, 0);
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime, waitcustomlist) VALUES("b0001", 1, 2, 50, "c0001:c0002:");
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime, waitcustomlist) VALUES("b0002", 1, 4, 30, "c0003:c0004:c0005:c0006:");
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime, waitcustomlist) VALUES("b0003", 1, 3, 15, "c0007:c0008:c0009:");
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime, waitcustomlist) VALUES("b0004", 1, 1, 40, "c0000:");`  

- custom
`INSERT INTO custom (cid, cname) VALUES ("c0000", "oshwan");INSERT INTO custom (cid, cname) VALUES ("c0001", "ksunuk");
INSERT INTO custom (cid, cname) VALUES ("c0002", "sungchgu");INSERT INTO custom (cid, cname) VALUES ("c0003", "ishun");
INSERT INTO custom (cid, cname) VALUES ("c0004", "kbmin");INSERT INTO custom (cid, cname) VALUES ("c0005", "kisa");
INSERT INTO custom (cid, cname) VALUES ("c0006", "songjki");INSERT INTO custom (cid, cname) VALUES ("c0007", "songhea");
INSERT INTO custom (cid, cname) VALUES ("c0008", "hanga");INSERT INTO custom (cid, cname) VALUES ("c0009", "yjhun");`  

(3) 예약어  
  - 데이터셋 모두 호출  
  `SELECT * FROM finService; SELECT * FROM lastFinService; SELECT * FROM frontState;SELECT * FROM custom;`  
