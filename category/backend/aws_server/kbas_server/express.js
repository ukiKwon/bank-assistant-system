/*
  * Declaration
*/
var express =require("express");
var app = express();
var request = require("request");
var port = process.env.PORT || 3000;
var mysql =require("mysql");

var connection = mysql.createConnection(//mysql
{
  host     : '127.0.0.1',
  user     : 'root',
  password : 'kbas',
  database : 'kbas'
});
//sample-users;
let users = [
  {
    id: 1,
    name: "osh"
  },
  {
    id: 2,
    name: "ksu"
  },
  {
    id: 3,
    name: "sck"
  },
  {
    id: 4,
    name: "lsh"
  }
]
//back-configuration
connection.connect();
app.use(express.static(__dirname + '/public'));//express
app.use(express.json());
app.use(express.urlencoded({extended:false}));

//front-rendering
app.get("/", (req, res, next) => {
  res.send('hello world!');
});
/*
  * back-processing
*/
app.get("/users", (req, res) => {
   console.log("who get in here/users");
   res.json(users)
});
//(1) 현재 은행원의 대기열 조회(Search a current banker's wait que assigned)
app.post("/bankque", (req, res) => {
    console.log(">> banker is lookup the Bank Que");
    var bid = req.body.bid;
    var sql = 'SELECT frontid, isbusy, numofcustom, waitcustomlist FROM `kbas`.`frontState` WHERE frontid=?';
    connection.query(sql,[bid], function (error, results) {
        if (error) throw error;
        else {
              if (results[0].numofcustom > 0) {
                  //chop the waiting custom listss
                  var customArray = results[0].waitcustomlist.split(':');
                  //print_test
                  // for (i = 0; i < customArray.length; ++i) {
                  //     console.log(">> split_wait_res : " + customArray[i]);
                  // }
                  // if (customArray[customArray.length - 1] == '') {
                  //     console.log(">> last_val_type is 0 empty");
                  // }
                  // else if (customArray[customArray.length - 1] == ' ') {
                  //     console.log(">> last_val_type is space");
                  // }
                  // else if (customArray[customArray.length - 1] == null) {
                  //     console.log(">> last_val_type is null");
                  // }
                  // else if (customArray[customArray.length - 1] == " ") {
                  //     console.log(">> last_val_type is String empty");
                  // }
                  //get the first custom who the front the line
                  if (customArray[0] != null || customArray[0] != '') {
                      var sql = 'SELECT cid, cname FROM `kbas`.`custom` WHERE cid=?';
                      connection.query(sql,[customArray[0]], function (error, results) {
                          if (error) throw error;
                          else {
                              console.log(">> This banker will service for : " + results[0].cname);
                              var custom_name = results[0].cname;
                              res.json(results[0].cid + ":" + results[0].cname);
                          }
                      })
                  } else {
                    console.log(">> (state:10002) This banker is in lazy state!!! Make work!!!");
                    res.json("10002")
                  }
              } else {
                  console.log(">> (state:10001) This banker is in lazy state!!! Make work!!!");
                  res.json("10001");
              }
        }//else
    })//connection
});
//(2) 고객에게 현재 대기열 안내
app.post("/finsearch", (req, res) => {
    console.log(">> custom is lookup the proper bank");
    var cid = req.body.cid;
    var sql = 'SELECT finid FROM `kbas`.`lastFinService` WHERE cid=?';
    connection.query(sql,[cid], function (error, results) {
        if (error) throw error;
        else {
              var sql = 'SELECT finname FROM `kbas`.`finService` WHERE finid=?';
              connection.query(sql,[results[0].findid], function (error, results) {
                  if (error) throw error;
                  else {
                      console.log(">> This custom were serviced by " + results[0].finname);
                      res.json(results[0].finname);
                  }
              })
        }
    })
});

app.post("/post", (req, res) => {
   console.log("who get in here post /users");
   var inputData;
   req.on("data", (data) => {
       inputData = JSON.parse(data);
   });
   req.on("end", () => {
       console.log("user_id : "+inputData.user_id + " , name : "+inputData.name);
   });
   res.write("OK!");
   res.end();
});
//Listen
app.listen(3000);
console.log("Listening on port", port);
