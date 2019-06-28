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
    console.log(req.body);
    var bid = req.body.bid;
    var sql = 'SELECT frontid, isbusy, numofcustom, waitcustomlist FROM `kbas`.`frontState` WHERE frontid=?';
    connection.query(sql,[bid], function (error, results) {
        if (error) throw error;
        else {
            console.log("SQL query is successed!!!!");
            if (results[0].numofcustom > 0) {
                //print-test
                var customArray = results[0].waitcustomlist;
                for (i = 0; i < customArray.length; ++i) {
                    console.log('>> wait_custom(' + i + ') :' + customArray[i]);
                }
                //search custom
                //var sql = 'SELECT cid, cname FROM `kbas`.`custom` WHERE cid=?';
            } else {
                res.json('등록정보가 없습니다');
            }
        }
    });
})
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
