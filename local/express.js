/*
  *
  *******  part1.환경설정 *******
  *
*/
//1.1.설정-모듈 라이브러리
var express = require("express");//express
var app = express();
var request = require('request');//request
var port = process.env.PORT || 3000;
var cors = require('cors');//cross-browsing
var Ngocr = require("ng-ocr");
var mysql = require('mysql');//mysql
// ***********************************************************
// mysql-booster 관리
// var MysqlPoolBooster = require('mysql-pool-booster');
// mysql = MysqlPoolBooster(mysql);
// // db-configuration 적용
// mysql.createPool(db_config);
// ***********************************************************
//1.2.설정-연동
var connection = mysql.createConnection(//mysql
{
  host     : '127.0.0.1',//localhost로 하면 에러남
  user     : 'root',
  password : 'Flower5wantnight',
  database : 'KISA'
});
connection.connect();

app.use(express.static(__dirname + '/public'));//express
app.use(express.json());
app.use(express.urlencoded({extended:false}));

app.set('views', __dirname + '/views');//ejs
app.set('view engine', 'ejs');
app.engine('html', require('ejs').renderFile);

app.use(cors());//cross-browsing

/*
  *
  *******  part2.프론트-화면 렌더링 *******
  *
*/
//2-1. 메인
app.get('/', function (req, res) {
    res.render('index')
})
//2-2. 은행원 전적
app.get('/history', function (req, res) {
    res.render('history')
})
//2-$. 영업 결과
app.get('/results', function (req, res) {
    res.render('sales_results')
})
//2-4. 메인
app.get('/main', function (req, res) {
    res.render('main');
})
/*
  *
  ******* part3.서버-동작 선언 *******
  *
*/
//3-8. QR 결제
app.post('/', function (req, res) {
  // OCR the input image and output result to text file given by params.output
  // Ngocr.decodeFile("./text.png", function(error, data){
  //   console.log(data); // Hello World!
  // });
  // var fs = require("fs");
  // var buffer = fs.readFileSync("text2.png");
  // Ngocr.decodeBuffer(buffer, function(error, data){
  //   console.log(data); // Hello World!
// });
})
app.post('/withdrawQR', function (req, res) {
    var userId = req.decoded.userId;
    var finNum = req.body.qrFin;
    var sql = "SELECT unum, uaccessToken FROM user WHERE uid = ?";
    connection.query(sql,[userId], function(err, result){
        if(err){
            console.error(err);
            throw err;
        }
        else {
            // console.log(result[0].uaccessToken);
            var option = {
                method : "POST",
                url :'https://testapi.open-platform.or.kr/transfer/withdraw',
                headers : {
                    'Authorization' : 'Bearer ' + result[0].uaccessToken,
                    'Content-Type' : 'application/json; charset=UTF-8'
                },
                json : {
                    dps_print_content : '권성욱',
                    fintech_use_num : finNum,
                    tran_amt : 1000,
                    print_content : '권성욱',
                    tran_dtime : '20190523101921'
                }
            };
            request(option, function(err, response, body){
                if(err) throw err;
                else {
                    if(body.rsp_code == "A0000"){
                        res.json(1);
                    }
                    else {
                        res.json(2);
                    }
                }
            })
        }
    })
})
//3.$. 서버처리-대기
app.listen(3000);
console.log("Listening on port", port);
