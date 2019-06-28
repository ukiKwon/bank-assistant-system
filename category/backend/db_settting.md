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
          cid VARCHAR(255) PRIMARY KEY,
          finid INTEGER,
          FOREIGN KEY (finid) REFERENCES finService (finid)
      );
  ```  
 - frontState : 창구 정보
  ```
  CREATE TABLE frontState (
      frontid VARCHAR(255) PRIMARY KEY,
      isbusy boolean DEFAULT '0',
      numofcustom INTEGER DEFAULT '0',
      expectedwaittime INTEGER DEFAULT '0'
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
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime) VALUES("b0001", 1, 2, 50);
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime) VALUES("b0002", 1, 4, 30);
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime) VALUES("b0003", 1, 3, 15);
INSERT INTO frontState(frontid, isbusy, numofcustom, expectedwaittime) VALUES("b0004", 1, 1, 40);`
