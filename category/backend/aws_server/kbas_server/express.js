
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
connection.connect();

app.use(express.static(__dirname + '/public'));//express
app.use(express.json());
app.use(express.urlencoded({extended:false}));

app.get('/', (req, res, next) => {
    res.send('hello world!');
});
//Listen
app.listen(443);
console.log("Listening on port", port);
