/*
  * Declaration
*/
var express =require("express");
var app = express();
var request = require("request");
var port = process.env.PORT || 443;
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
//back-processing
app.get("/users", (req, res) => {
   console.log("who get in here/users");
   res.json(users)
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
app.listen(443);
console.log("Listening on port", port);
